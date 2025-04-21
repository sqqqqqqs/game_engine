//
//  SceneDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/1/29.
//

#ifndef SceneDB_h
#define SceneDB_h

#include <vector>
#include <string>
#include <map>
#include "Actor.h"
#include "lua.hpp"
#include "LuaBridge.h"
#include "ComponentDB.h"
#include <unordered_map>
#include <algorithm>
#include <optional>
#include <memory>
#include "rapidjson/document.h"
#include "glm/glm.hpp"
#include "Actor.h"

class Actor;

class Scene{
public:
    std::string scene_name;
    std::vector<Actor*> actors;
    
};

class SceneDB{
public:
    static Scene current_scene;
    static std::string next_scene_name;
    static bool LoadScene(const std::string& sceneName);
    static void AddComponentToActor(Actor& actor, const std::string& key, const rapidjson::Value& componentData);
    static Actor* FindActor(std::string name);
    static luabridge::LuaRef FindAllActors(std::string name);
    static void UpdateActors();
    static void LateUpdateActors();
    static Actor* AddActorWithTemplate(std::string actor_template_name);
    static std::vector<Actor*> actors_to_add;
    static void ProcessAddedActors();
    static void Destroy(Actor* actor);
    static std::vector<Actor*> actors_to_destroy;
    static void ProcessDestroyedActors();
    
    
    static void LoadNewScene(const std::string& sceneName);
    static void ProcessNewScene();
    static std::string GetCurrentSceneName();
    static void DontDestroy(Actor* actor);
    
    
    
    static std::vector<Actor*>& GetActors();
    static Actor* GetActorById(int id);
    
    
private:
    //static std::vector<Actor*> actors;
};

#endif /* SceneDB_h */
