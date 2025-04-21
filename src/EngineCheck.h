//
//  EngineCheck.h
//  game_engine
//
//  Created by 李一锴 on 2025/1/29.
//

#ifndef EngineCheck_h
#define EngineCheck_h

#include <string>
#include <vector>
#include "glm/glm.hpp"
#include "rapidjson/document.h"

class EngineCheck {
public:
    static void CheckResources();
    static void LoadGameConfig(const std::string& configPath);
    static void LoadRenderingConfig(const std::string& configPath);
    
    static std::string GetGameStartMessage();
    static std::string GetGameOverBadMessage();
    static std::string GetGameOverGoodMessage();
    
    static int GetCameraWidth();
    static int GetCameraHeight();
    
    static std::string GetInitialScene();
    
    static std::string GetGameTitle();
    static std::string GetFontName();
    
    static void SetInitialScene(std::string name);
    

private:
    static rapidjson::Document configData;
    static rapidjson::Document CameraConfigData;
    static std::string initial_scene;
};


#endif /* EngineCheck_h */
