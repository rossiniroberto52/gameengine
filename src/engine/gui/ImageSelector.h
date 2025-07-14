#ifndef IMAGE_SELECTOR_H
#define IMAGE_SELECTOR_H

#include <vector>
#include <string>
#include <SDL2/SDL.h>

class ImageSelector {
public:
    ImageSelector(SDL_Renderer* renderer, const std::string& imageDir);
    ~ImageSelector();

    void render(int x, int y);
    void handleEvent(const SDL_Event& event);
    std::string getSelectedImagePath() const;
    int getImageCount() const;

private:
    SDL_Renderer* renderer_;
    std::string imageDir_;
    std::vector<std::string> imagePaths_;
    std::vector<SDL_Texture*> textures_;
    int selectedIndex_;

    void loadImages();
    SDL_Texture* loadTexture(const std::string& path);
};

#endif // IMAGE_SELECTOR_H
