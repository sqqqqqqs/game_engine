//
//  ImageDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/2/6.
//

#ifndef ImageDB_h
#define ImageDB_h
#include <string>
#include <unordered_map>
#include "SDL2_image/SDL_image.h"
#include "SDL2/SDL.h"
#include <vector>
#include "Renderer.h"

class ImageDB{
public:
    static void LoadImages();
    static SDL_Texture* GetImage(const std::string& image_name);
    static void CreateDefaultParticleTextureWithName(const std::string & name);
    
    
private:
    static inline std::unordered_map<std::string,SDL_Texture*> images_loaded;
    static SDL_Texture* LoadTexture(const std::string& file_path);
};


struct ImageDrawRequest
{
    std::string image_name;
    float x;
    float y;
    int rotation_degrees;
    float scale_x;
    float scale_y;
    float pivot_x;
    float pivot_y;
    int r;
    int g;
    int b;
    int a;
    int sorting_order;
};



#endif /* ImageDB_h */
