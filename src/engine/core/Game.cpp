#include <SDL2/SDL.h>
#include <vector>
// Define a struct to represent a color
struct Color {
    int r, g, b, a;
};
// Define a GameObject class
class GameObject {
public:
    GameObject(int x, int y, int width, int height, Color color)
        : x_(x), y_(y), width_(width), height_(height), color_(color) {}

    void render(SDL_Renderer* renderer) {
        SDL_Rect rect = { x_, y_, width_, height_ };
        SDL_SetRenderDrawColor(renderer, color_.r, color_.g, color_.b, color_.a);
        SDL_RenderFillRect(renderer, &rect);
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

    int getX() const { return x_; }
    int getY() const { return y_; }

private:
    int x_, y_, width_, height_;
    Color color_;
};
// Define a Game class to manage game objects and rendering
class Game {
public:
    Game() : renderer_(nullptr), objects_(), draggedObjectIndex_(-1) {}

    void init() {
        // Initialize SDL and create a window
        SDL_Init(SDL_INIT_VIDEO);
        window_ = SDL_CreateWindow("Game Engine", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
        renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    }

    void render() {
        SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 255);
        SDL_RenderClear(renderer_);
        // Render game objects
        for (auto& obj : objects_) {
            obj.render(renderer_);
        }
        SDL_RenderPresent(renderer_);
    }

    void update() {
        // Update game objects
        for (auto& obj : objects_) {
            obj.update();
        }
    }

    void addObject(GameObject obj) {
        objects_.push_back(obj);
    }

    void handleEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mx = event.button.x;
                    int my = event.button.y;
                    for (int i = 0; i < objects_.size(); ++i) {
                        if (objects_[i].containsPoint(mx, my)) {
                            draggedObjectIndex_ = i;
                            dragOffsetX_ = mx - objects_[i].getX();
                            dragOffsetY_ = my - objects_[i].getY();
                            break;
                        }
                    }
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    draggedObjectIndex_ = -1;
                }
                break;
            case SDL_MOUSEMOTION:
                if (draggedObjectIndex_ != -1) {
                    int mx = event.motion.x;
                    int my = event.motion.y;
                    objects_[draggedObjectIndex_].setPosition(mx - dragOffsetX_, my - dragOffsetY_);
                }
                break;
            }
        }
    }

    bool isRunning() const { return running_; }

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    std::vector<GameObject> objects_;
    int draggedObjectIndex_;
    int dragOffsetX_;
    int dragOffsetY_;
    bool running_ = true;
};

int main() {
    Game game;
    game.init();
    // Create a game object
    GameObject obj(100, 100, 200, 200, { 255, 0, 0, 255 });
    game.addObject(obj);
    // Main loop
    while (game.isRunning()) {
        game.handleEvents();
        game.update();
        game.render();
    }
    return 0;
}
