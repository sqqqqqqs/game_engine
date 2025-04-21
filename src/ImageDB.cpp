//
//  ImageDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/2/6.
//

#include <stdio.h>
#include "ImageDB.h"
#include "EngineCheck.h"
#include "SceneDB.h"

void ImageDB::LoadImages(){
    const std::string imageDirectory = "resources/images";
    if (!std::filesystem::exists(imageDirectory)) {
        return;
    }
    for (const auto& imageFile : std::filesystem::directory_iterator(imageDirectory)) {
        SDL_Texture* img = LoadTexture(imageFile.path().string().c_str());
        std::string imageName = imageFile.path().filename().stem().stem().string();
        images_loaded[imageName] = img;
    }
}

SDL_Texture* ImageDB::GetImage(const std::string& image_name){
    auto it = images_loaded.find(image_name);
    if (it != images_loaded.end()) {
        return it->second;
    }
    std::cout << "error: missing image " << image_name;
    std::exit(0);
}

SDL_Texture* ImageDB::LoadTexture(const std::string& file_path) {
    SDL_Texture* texture = IMG_LoadTexture(Renderer::main_renderer, file_path.c_str());
    return texture;
}

void ImageDB::CreateDefaultParticleTextureWithName(const std::string &name){
    if(images_loaded.find(name)!=images_loaded.end()){
        return;
    }
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, 8, 8, 32, SDL_PIXELFORMAT_RGBA8888);
    Uint32 white_color = SDL_MapRGBA(surface->format, 255, 255, 255, 255);
    SDL_FillRect(surface, NULL, white_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Renderer::main_renderer, surface);
    SDL_FreeSurface(surface);
    images_loaded[name] = texture;
}

