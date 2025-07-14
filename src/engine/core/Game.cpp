#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
// Define a struct to represent a color
struct Color {
    int r, g, b, a;
};
#include <memory>
#include <string>
#include "ImageSelector.h"
#include "FileUtils.h"

class GameObject {
public:
    GameObject(int x, int y, int width, int height, Color color)
        : x_(x), y_(y), width_(width), height_(height), color_(color), texture_(nullptr), flip_(SDL_FLIP_NONE) {}

    GameObject(int x, int y, SDL_Texture* texture)
        : x_(x), y_(y), texture_(texture), flip_(SDL_FLIP_NONE) {
        if (texture_) {
            SDL_QueryTexture(texture_, NULL, NULL, &width_, &height_);
        } else {
            width_ = 0;
            height_ = 0;
        }
    }

    ~GameObject() {
        if (texture_) {
            SDL_DestroyTexture(texture_);
        }
    }

    // Delete copy constructor and copy assignment operator to prevent copying
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    void render(SDL_Renderer* renderer) {
        if (texture_) {
            SDL_Rect dstRect = { x_, y_, width_, height_ };
            SDL_RenderCopyEx(renderer, texture_, NULL, &dstRect, 0, NULL, flip_);
        } else {
            SDL_Rect rect = { x_, y_, width_, height_ };
            SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void update() {
        // Add update logic here
    }

    bool containsPoint(int px, int py) {
        return px >= x_ && px <= x_ + width_ && py >= y_ && py <= y_ + height_;
    }

    void setPosition(int x, int y) {
        x_ = x;
        y_ = y;
    }

    void flipHorizontal() {
        if (flip_ == SDL_FLIP_NONE) {
            flip_ = SDL_FLIP_HORIZONTAL;
        } else {
            flip_ = SDL_FLIP_NONE;
        }
    }

    int getX() const { return x_; }
    int getY() const { return y_; }

private:
    int x_, y_, width_, height_;
    Color color_;
    SDL_Texture* texture_;
    SDL_RendererFlip flip_;
};

class Game {
public:
    Game() : window_(nullptr), renderer_(nullptr), draggedObjectIndex_(-1), running_(true), imageSelector_(nullptr), draggingSprite_(false), draggingSpriteTexture_(nullptr) {}

    void init() {
        // Initialize SDL and create a window
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
            running_ = false;
            return;
        }
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) {
            SDL_Log("Unable to initialize SDL_image: %s", IMG_GetError());
            running_ = false;
            return;
        }
        // Create ./img directory if it does not exist
        if (!FileUtils::createDirectoryIfNotExists("./img")) {
            SDL_Log("Failed to create ./img directory");
        } else {
            SDL_Log("./img directory created or already exists");
        }
        window_ = SDL_CreateWindow("Game Engine", 100, 100, 1024, 768, SDL_WINDOW_SHOWN);
        if (!window_) {
            SDL_Log("Could not create window: %s", SDL_GetError());
            running_ = false;
            return;
        }
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer_) {
            SDL_Log("Could not create renderer: %s", SDL_GetError());
            running_ = false;
            return;
        }
        imageSelector_ = std::make_unique<ImageSelector>(renderer_, "./img");
    }

    void render() {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
        SDL_RenderClear(renderer_);
        // Render game objects
        for (auto& objPtr : objects_) {
            objPtr->render(renderer_);
        }
        // Render image selector GUI panel
        if (imageSelector_) {
            imageSelector_->render(10, 10);
        }
        // Render dragging sprite if any
        if (draggingSprite_ && draggingSpriteTexture_) {
            SDL_Rect dstRect = { draggingSpriteX_ - 32, draggingSpriteY_ - 32, 64, 64 };
            SDL_RenderCopy(renderer_, draggingSpriteTexture_, NULL, &dstRect);
        }
        SDL_RenderPresent(renderer_);
    }

    void update() {
        // Update game objects
        for (auto& objPtr : objects_) {
            objPtr->update();
        }
    }

    void addObject(std::unique_ptr<GameObject> obj) {
        objects_.push_back(std::move(obj));
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            default:
                if (imageSelector_) {
                    imageSelector_->handleEvent(event);
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x;
                    int my = event.button.y;
                    // Check if clicked on image selector
                    if (imageSelector_ && mx >= 10 && mx <= 74 && my >= 10 && my <= 10 + (int)(70 * imageSelector_->getImageCount())) {
                        // Start dragging selected image
                        std::string selectedPath = imageSelector_->getSelectedImagePath();
                        if (!selectedPath.empty()) {
                            SDL_Surface* surface = IMG_Load(selectedPath.c_str());
                            if (surface) {
                                draggingSpriteTexture_ = SDL_CreateTextureFromSurface(renderer_, surface);
                                SDL_FreeSurface(surface);
                                draggingSpriteX_ = mx;
                                draggingSpriteY_ = my;
                                draggingSprite_ = true;
                            }
                        }
                    } else {
                        for (int i = 0; i < (int)objects_.size(); ++i) {
                            if (objects_[i]->containsPoint(mx, my)) {
                                draggedObjectIndex_ = i;
                                dragOffsetX_ = mx - objects_[i]->getX();
                                dragOffsetY_ = my - objects_[i]->getY();
                                break;
                            }
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (draggingSprite_) {
                        // Add new sprite at drop position
                        auto newObj = std::make_unique<GameObject>(draggingSpriteX_, draggingSpriteY_, draggingSpriteTexture_);
                        addObject(std::move(newObj));
                        draggingSpriteTexture_ = nullptr;
                        draggingSprite_ = false;
                    }
                    draggedObjectIndex_ = -1;
                }
                break;
            case SDL_MOUSEMOTION:
                if (draggedObjectIndex_ != -1) {
                    int mx = event.motion.x;
                    int my = event.motion.y;
                    objects_[draggedObjectIndex_]->setPosition(mx - dragOffsetX_, my - dragOffsetY_);
                }
                if (draggingSprite_) {
                    draggingSpriteX_ = event.motion.x;
                    draggingSpriteY_ = event.motion.y;
                }
                break;
            }
        }
    }

    bool isRunning() const { return running_; }

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::vector<std::unique_ptr<GameObject>> objects_;
    int draggedObjectIndex_;
    int dragOffsetX_;
    int dragOffsetY_;
    bool running_;
    std::unique_ptr<ImageSelector> imageSelector_;
    bool draggingSprite_;
    SDL_Texture* draggingSpriteTexture_;
    int draggingSpriteX_;
    int draggingSpriteY_;
};

int main() {
    Game game;
    game.init();
    // Create a game object
    auto obj = std::make_unique<GameObject>(100, 100, 200, 200, Color{ 255, 0, 0, 255 });
    game.addObject(std::move(obj));
    // Main loop
    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
    }
    return 0;
}
