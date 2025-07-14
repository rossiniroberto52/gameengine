#include "ImageSelector.h"
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <dirent.h>
#include <cstring>

ImageSelector::ImageSelector(SDL_Renderer* renderer, const std::string& imageDir)
    : renderer_(renderer), imageDir_(imageDir), selectedIndex_(-1) {
    loadImages();
}

ImageSelector::~ImageSelector() {
    for (auto tex : textures_) {
        SDL_DestroyTexture(tex);
    }
}

void ImageSelector::loadImages() {
    textures_.clear();
    imagePaths_.clear();
    DIR* dir = opendir(imageDir_.c_str());
    if (!dir) {
        std::cerr << "Failed to open directory: " << imageDir_ << std::endl;
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename(entry->d_name);
        std::string lowerFilename = filename;
        std::transform(lowerFilename.begin(), lowerFilename.end(), lowerFilename.begin(), ::tolower);
        if (lowerFilename.size() >= 4) {
            if (lowerFilename.compare(lowerFilename.size() - 4, 4, ".png") == 0 ||
                lowerFilename.compare(lowerFilename.size() - 4, 4, ".jpg") == 0 ||
                (lowerFilename.size() >= 5 && lowerFilename.compare(lowerFilename.size() - 5, 5, ".jpeg") == 0)) {
                std::string fullPath = imageDir_ + "/" + filename;
                SDL_Texture* tex = loadTexture(fullPath);
                if (tex) {
                    textures_.push_back(tex);
                    imagePaths_.push_back(fullPath);
                } else {
                    std::cerr << "Failed to load texture: " << fullPath << std::endl;
                }
            }
        }
    }
    closedir(dir);
}

SDL_Texture* ImageSelector::loadTexture(const std::string& path) {
    SDL_Surface* surface = IMG_Load(path.c_str());
    if (!surface) {
        std::cerr << "IMG_Load failed for " << path << ": " << IMG_GetError() << std::endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void ImageSelector::render(int x, int y) {
    int offsetY = y;
    for (size_t i = 0; i < textures_.size(); ++i) {
        SDL_Rect dstRect = { x, offsetY, 64, 64 };
        SDL_RenderCopy(renderer_, textures_[i], NULL, &dstRect);
        offsetY += 70;
    }
}

void ImageSelector::handleEvent(const SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mx = event.button.x;
        int my = event.button.y;
        int x = 10; // GUI panel x position
        int y = 10; // GUI panel y start position
        for (size_t i = 0; i < textures_.size(); ++i) {
            SDL_Rect rect = { x, y + (int)(i * 70), 64, 64 };
            if (mx >= rect.x && mx <= rect.x + rect.w && my >= rect.y && my <= rect.y + rect.h) {
                selectedIndex_ = (int)i;
                break;
            }
        }
    }
}

std::string ImageSelector::getSelectedImagePath() const {
    if (selectedIndex_ >= 0 && selectedIndex_ < (int)imagePaths_.size()) {
        return imagePaths_[selectedIndex_];
    }
    return "";
}

int ImageSelector::getImageCount() const {
    return (int)textures_.size();
}
