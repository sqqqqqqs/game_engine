//
//  Engine.h
//  game_engine
//
//  Created by 李一锴 on 2025/1/21.
//

#ifndef Engine_h
#define Engine_h

#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "glm/glm.hpp"
#include "EngineUtils.h"
#include "rapidjson/document.h"
#include "Renderer.h"
#include "SceneDB.h"
#include "RigidBody.h"
#include "EventBus.h"

class Engine{
public:
    Engine();
    void RenderScene();
    void ShowStatus();
    void promptInput();
    void GameLoop();
    
    
private:
    bool running;
    std::unordered_set<int> nearby_sfx_played;
    
    bool sceneSwitched = false;
    
    void HandleInput(const std::string& input);
    void RenderCameraView();
    void QuitGame();
    glm::ivec2 GetDirectionOfInput(const std::string& input);
    
    
    
};

#endif /* Engine_h */
