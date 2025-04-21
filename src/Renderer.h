//
//  Renderer.h
//  game_engine
//
//  Created by 李一锴 on 2025/2/6.
//

#ifndef Renderer_h
#define Renderer_h

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "Renderer.h"
#include "Helper.h"
#include "TextDB.h"
#include "ImageDB.h"
#include "glm/glm.hpp"
#include <string>
#include <queue>

struct ImageDrawRequest;
struct Pixel{
    float x;
    float y;
    int r;
    int g;
    int b;
    int a;
};

class Renderer {
public:
    static int window_width;
    static int window_height;
    static int clear_color_r;
    static int clear_color_g;
    static int clear_color_b;
    static glm::vec2 cam_offset;
    static float zoom_factor;
    static float cam_ease_factor;
    static float pixel_per_unit_distance;
    static SDL_Window* main_window;
    static SDL_Renderer* main_renderer;
    
    
    static void InitialRenderer();
    
    static void InitializaWindow(int width, int height);
    static void InitializClearColor(int color_a, int color_b, int color_c);
    static void InitializeCameraOffset(float a,float b);
    static void InitializeZoomFactor(float a);
    static void InitializeCamEaseFactor(float a);
    
    static void DrawText(const std::string &str_content, float x, float y, std::string font_name, float font_size,float r, float g, float b, float a);
    static void RenderText();
    
    static void DrawUI(std::string image_name, float x, float y);
    static void DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sortingOrder);
    static void RenderUI();
    
    static void Draw(std::string image_name, float x, float y);
    static void DrawEx(std::string image_name, float x, float y, float rotationDegrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sortingOrder);
    static void RenderSceneImages();
    
    static void DrawPixel(float x, float y, float r, float g, float b, float a);
    static void RenderPixels();
    
    static void SetCameraPosition(float x, float y);
    static float GetCameraPositionX();
    static float GetCameraPositionY();
    static void SetZoom(float zoomfactor);
    static float GetZoom();
    

private:
    static glm::vec2 current_camera_position;
    static inline bool setFirstCamera = false;
    static inline std::queue<Text> texts_to_draw;
    static inline std::vector<ImageDrawRequest> ui_images_to_draw;
    static inline std::vector<ImageDrawRequest> images_to_draw;
    static inline std::queue<Pixel> pixel_to_draw;
    

};




#endif /* Renderer_h */
