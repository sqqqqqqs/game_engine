//
//  main.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/1/21.
//
#define ASIO_STANDALONE
#include <iostream>
#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"
#include "Engine.h"
#include "Renderer.h"
#include "Lua/lua.hpp"
#include "LuaBridge/LuaBridge.h"
#include "EngineCheck.h"
#include "box2d/box2d.h"
#include "asio/asio.hpp"


int main(int argc, char* argv[]){
    std::string initialScene = "client";  
    if (argc > 1) {
        initialScene = argv[1];
    }
    EngineCheck::SetInitialScene(initialScene);
    Engine engine;
    engine.GameLoop();
    return 0;
}

