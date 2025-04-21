//
//  Engine.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/1/21.
//
#include "Engine.h"
#include "EngineCheck.h"
#include "ImageDB.h"
#include "TextDB.h"
#include "Input.h"
#include "AudioDB.h"
#include "ComponentDB.h"


Engine::Engine() : running(true){
    
    //Initialize Lua
    ComponentDB::InitilizeLuaState();
    ComponentDB::CreatingGlobal();
    ComponentDB::LoadLuaComponents();
    
    
    //check all resources needed when launching
    EngineCheck::CheckResources();
    EngineCheck::LoadGameConfig("resources/game.config");
    EngineCheck::LoadRenderingConfig("resources/rendering.config");
    TextDB::LoadFonts();
    AudioDB::LoadAudio();
    
    //initialize the rendering
    Renderer::InitialRenderer();
    ImageDB::LoadImages();
    std::string initial_scene_name = EngineCheck::GetInitialScene();
    
    
    
    //load all the actors
    SceneDB::LoadScene(initial_scene_name);
    
    for(Actor* actor:SceneDB::GetActors()){
        actor->ProcessOnStartQueue();
    }
    Input::Init();
    
}




//main loop
//here
    

void Engine::GameLoop(){
    while(running){
        SDL_Event event;
        while (Helper::SDL_PollEvent(&event)) {
            Input::ProcessEvent(event);
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_KEYDOWN:
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    break;
                default:
                    break;
            }
        }
        SDL_RenderClear(Renderer::main_renderer);
        
        SceneDB::ProcessNewScene();
        //main game update logic
        SceneDB::UpdateActors();

        SceneDB::LateUpdateActors();
        Input::LateUpdate();
        
        Event::ApplyQueuedChanges();
        
        World::PhysicsStep();
        
        Renderer::RenderSceneImages();
        Renderer::RenderUI();
        Renderer::RenderText();
        Renderer::RenderPixels();
        Helper::SDL_RenderPresent(Renderer::main_renderer);
    }
}


void Engine::QuitGame() {
    running = false;
}




