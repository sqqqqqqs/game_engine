//
//  Renderer.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/2/6.
//
#include <stdio.h>
#include "SceneDB.h"
#include "ImageDB.h"
#include "Engine.h"
#include "SDL2/SDL.h"
#include "EngineCheck.h"
#include "Helper.h"
#include <iostream>
#include "Input.h"

SDL_Window* Renderer::main_window = nullptr;
SDL_Renderer* Renderer::main_renderer = nullptr;
int Renderer::window_width = 640;
int Renderer::window_height = 360;
int Renderer::clear_color_r = 255;
int Renderer::clear_color_g = 255;
int Renderer::clear_color_b = 255;
float Renderer::pixel_per_unit_distance = 100.0f;
glm::vec2 Renderer::cam_offset = {0.0f, 0.0f};
float Renderer::zoom_factor = 1.0f;
float Renderer::cam_ease_factor = 1.0f;
glm::vec2 Renderer::current_camera_position = {0.0f,0.0f};



void Renderer::InitialRenderer(){
    std::string game_title =EngineCheck::GetGameTitle();
    
    main_window = Helper::SDL_CreateWindow(game_title.c_str(), 0, 0, window_width, window_height, 0);
    main_renderer = Helper::SDL_CreateRenderer(main_window, -1, SDL_RENDERER_PRESENTVSYNC|SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(main_renderer,clear_color_r,clear_color_g,clear_color_b,255);
    SDL_RenderClear(main_renderer);
}


void Renderer::InitializaWindow(int width, int height){
    window_width =width;
    window_height = height;
}

void Renderer::InitializClearColor(int a, int b, int c){
    clear_color_r = a;
    clear_color_g = b;
    clear_color_b = c;
}

void Renderer::InitializeCameraOffset(float a, float b){
    cam_offset.x = a;
    cam_offset.y = b;
}

void Renderer::InitializeZoomFactor(float a){
    zoom_factor = a;
}

void Renderer::InitializeCamEaseFactor(float a){
    cam_ease_factor = a;
}


void Renderer::DrawText(const std::string &str_content, float x, float y, std::string font_name, float font_size, float r, float g, float b, float a){
    Text new_text;
    new_text.str_content = str_content;
    new_text.screen_ordinate_x = static_cast<int>(x);
    new_text.screen_ordinate_y = static_cast<int>(y);
    new_text.font_name = font_name;
    new_text.font_size = static_cast<int>(font_size);
    new_text.color.r = static_cast<int>(r);
    new_text.color.g = static_cast<int>(g);
    new_text.color.b = static_cast<int>(b);
    new_text.color.a = static_cast<int>(a);
    texts_to_draw.push(new_text);
}

void Renderer::RenderText(){
    while(!texts_to_draw.empty()){
        Text current = texts_to_draw.front();
        
        TTF_Font* font  = TextDB::GetFontByNameAndSize(current.font_name, current.font_size);
        SDL_Surface* textSurface = TTF_RenderText_Solid(font,current.str_content.c_str(),current.color);
        if(!textSurface)return;
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::main_renderer, textSurface);
        if (!textTexture) return;
        SDL_FRect render_quad= {
            static_cast<float>(current.screen_ordinate_x),
            static_cast<float>(current.screen_ordinate_y),
            static_cast<float>(textSurface->w),
            static_cast<float>(textSurface->h),
        };
        Helper::SDL_RenderCopy(Renderer::main_renderer, textTexture, NULL, &render_quad);
        SDL_FreeSurface(textSurface);
        texts_to_draw.pop();
    }
}

void Renderer::DrawUI(std::string image_name, float x, float y){
    ImageDrawRequest new_image;
    new_image.image_name = image_name;
    new_image.x = static_cast<int>(x);
    new_image.y = static_cast<int>(y);
    new_image.rotation_degrees = 0 ;
    new_image.scale_x = 1.0f;
    new_image.scale_y = 1.0f;
    new_image.pivot_x = 0.5f;
    new_image.pivot_y = 0.5f;
    new_image.r = 255;
    new_image.g = 255;
    new_image.b = 255;
    new_image.a = 255;
    new_image.sorting_order = 0;
    ui_images_to_draw.push_back(new_image);
}

void Renderer::DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sortingOrder){
    ImageDrawRequest new_image;
    new_image.image_name = image_name;
    new_image.x = static_cast<int>(x);
    new_image.y = static_cast<int>(y);
    new_image.rotation_degrees = 0 ;
    new_image.scale_x = 1.0f;
    new_image.scale_y = 1.0f;
    new_image.pivot_x = 0.5f;
    new_image.pivot_y = 0.5f;
    new_image.r = static_cast<int>(r);
    new_image.g = static_cast<int>(g);
    new_image.b = static_cast<int>(b);
    new_image.a = static_cast<int>(a);
    new_image.sorting_order = static_cast<int>(sortingOrder);
    ui_images_to_draw.push_back(new_image);
}

void Renderer::RenderUI(){
    std::stable_sort(ui_images_to_draw.begin(), ui_images_to_draw.end(),[](const ImageDrawRequest& a, const ImageDrawRequest& b) {
        return a.sorting_order < b.sorting_order;
    });
    for(auto current:ui_images_to_draw){
        SDL_FRect rect;
        SDL_Texture* tex = ImageDB::GetImage(current.image_name);
        Helper::SDL_QueryTexture(tex, &rect.w, &rect.h);
        
        int flip = SDL_FLIP_NONE;
        if (current.scale_x < 0) {
            flip |= SDL_FLIP_HORIZONTAL;
        }
        if (current.scale_y < 0) {
            flip |= SDL_FLIP_VERTICAL;
        }
        float x_scale = glm::abs(current.scale_x);
        float y_scale = glm::abs(current.scale_y);
        
        rect.w *= x_scale;
        rect.h *= y_scale;
        
        SDL_FPoint pivot_point = {
            current.pivot_x * rect.w,
            current.pivot_y * rect.h
        };
        
        rect.x = static_cast<int>(current.x);
        rect.y = static_cast<int>(current.y);
        
        SDL_SetTextureColorMod(tex, current.r, current.g, current.b);
        SDL_SetTextureAlphaMod(tex, current.a);
        
        Helper::SDL_RenderCopyEx(-1, "none", Renderer::main_renderer, tex, NULL, &rect, static_cast<int>(current.rotation_degrees), &pivot_point, static_cast<SDL_RendererFlip>(flip));
        
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }
    ui_images_to_draw.clear();
}

void Renderer::Draw(std::string image_name, float x, float y){
    ImageDrawRequest new_image;
    new_image.image_name = image_name;
    new_image.x = x;
    new_image.y = y;
    new_image.rotation_degrees = 0 ;
    new_image.scale_x = 1.0f;
    new_image.scale_y = 1.0f;
    new_image.pivot_x = 0.5f;
    new_image.pivot_y = 0.5f;
    new_image.r = 255;
    new_image.g = 255;
    new_image.b = 255;
    new_image.a = 255;
    new_image.sorting_order = 0;
    images_to_draw.push_back(new_image);
}

void Renderer::DrawEx(std::string image_name, float x, float y, float rotationDegrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sortingOrder){
    ImageDrawRequest new_image;
    new_image.image_name = image_name;
    new_image.x = x;
    new_image.y = y;
    new_image.rotation_degrees = static_cast<int>(rotationDegrees);
    new_image.scale_x = scale_x;
    new_image.scale_y = scale_y;
    new_image.pivot_x = pivot_x;
    new_image.pivot_y = pivot_y;
    new_image.r = static_cast<int>(r);
    new_image.g = static_cast<int>(g);
    new_image.b = static_cast<int>(b);
    new_image.a = static_cast<int>(a);
    new_image.sorting_order = static_cast<int>(sortingOrder);
    images_to_draw.push_back(new_image);
}

void Renderer::RenderSceneImages(){
    std::stable_sort(images_to_draw.begin(), images_to_draw.end(),[](const ImageDrawRequest& a, const ImageDrawRequest& b) {
        return a.sorting_order < b.sorting_order;
    });
    SDL_RenderSetScale(Renderer::main_renderer, zoom_factor, zoom_factor);
    for(auto current:images_to_draw){
        
        glm::vec2 final_rendering_position = glm::vec2(current.x, current.y) - current_camera_position;
        SDL_FRect rect;
        SDL_Texture* tex = ImageDB::GetImage(current.image_name);
        Helper::SDL_QueryTexture(tex, &rect.w, &rect.h);
        int flip = SDL_FLIP_NONE;
        if (current.scale_x < 0) {
            flip |= SDL_FLIP_HORIZONTAL;
        }
        if (current.scale_y < 0) {
            flip |= SDL_FLIP_VERTICAL;
        }
        float x_scale = glm::abs(current.scale_x);
        float y_scale = glm::abs(current.scale_y);
        rect.w *= x_scale;
        rect.h *= y_scale;
        SDL_FPoint pivot_point = {
            current.pivot_x * rect.w,
            current.pivot_y * rect.h
        };
        glm::ivec2 cam_dimention = glm::ivec2{window_width, window_height};
        
        rect.x = final_rendering_position.x * pixel_per_unit_distance + cam_dimention.x * 0.5f * (1.0f/zoom_factor) - pivot_point.x;
        rect.y = final_rendering_position.y * pixel_per_unit_distance + cam_dimention.y * 0.5f * (1.0f/zoom_factor) - pivot_point.y;
        
        SDL_SetTextureColorMod(tex, current.r, current.g, current.b);
        SDL_SetTextureAlphaMod(tex, current.a);
        
        Helper::SDL_RenderCopyEx(-1, "none", Renderer::main_renderer, tex, NULL, &rect, static_cast<int>(current.rotation_degrees), &pivot_point, static_cast<SDL_RendererFlip>(flip));
        
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 255);
    }
    SDL_RenderSetScale(Renderer::main_renderer, 1.0f, 1.0f);
    images_to_draw.clear();
}

void Renderer::DrawPixel(float x, float y, float r, float g, float b, float a){
    Pixel new_pixel;
    new_pixel.x = static_cast<int>(x);
    new_pixel.y = static_cast<int>(y);
    new_pixel.r = static_cast<int>(r);
    new_pixel.g = static_cast<int>(g);
    new_pixel.b = static_cast<int>(b);
    new_pixel.a = static_cast<int>(a);
    pixel_to_draw.push(new_pixel);
}

void Renderer::RenderPixels(){
    SDL_SetRenderDrawBlendMode(Renderer::main_renderer, SDL_BLENDMODE_BLEND);
    while(!pixel_to_draw.empty()){
        Pixel current = pixel_to_draw.front();
        SDL_SetRenderDrawColor(Renderer::main_renderer, current.r, current.g, current.b, current.a);
        SDL_RenderDrawPoint(Renderer::main_renderer, current.x, current.y);
        SDL_SetRenderDrawColor(Renderer::main_renderer, clear_color_r, clear_color_g, clear_color_b, 255);
        pixel_to_draw.pop();
    }
    SDL_SetRenderDrawBlendMode(Renderer::main_renderer, SDL_BLENDMODE_NONE);
}

void Renderer::SetCameraPosition(float x, float y){
    current_camera_position.x = x;
    current_camera_position.y = y;
}

float Renderer::GetCameraPositionX(){
    return current_camera_position.x;
}

float Renderer::GetCameraPositionY(){
    return current_camera_position.y;
}

void Renderer::SetZoom(float zoomfactor){
    zoom_factor = zoomfactor;
}

float Renderer::GetZoom(){
    return zoom_factor;
}
















