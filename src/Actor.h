//
//  Actor.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/12.
//

#ifndef Actor_h
#define Actor_h
#include <vector>
#include <string>
#include <map>
#include "lua.hpp"
#include "LuaBridge.h"
#include "RigidBody.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "NetworkComponent.h"
#include "ComponentDB.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <optional>
#include <memory>
#include <queue>


class Collision;
class NetworkComponent;
class Actor{
public:
    int id;
    std::string name="";
    bool dont_destory_on_load = false;
    std::unordered_map<std::string, std::map<std::string, std::shared_ptr<luabridge::LuaRef>>> components;
    std::map<std::string, std::shared_ptr<luabridge::LuaRef>> components_to_add;
    std::vector<std::string> components_to_remove;
    
    std::vector<std::shared_ptr<luabridge::LuaRef>> onStartQueue;
    
    std::map<std::string, std::shared_ptr<luabridge::LuaRef>> update_components;
    std::map<std::string, std::shared_ptr<luabridge::LuaRef>> late_update_components;
    
    std::priority_queue<std::string> collision_enter_components;
    std::priority_queue<std::string> collision_exit_components;
    void CollisionEnter(Collision collision);
    void CollisionExit(Collision collision);
    
    
    std::priority_queue<std::string> trigger_enter_components;
    std::priority_queue<std::string> trigger_exit_components;
    void TriggerEnter(Collision collision);
    void TriggerExit(Collision collision);
    
    std::unordered_set<std::string> on_destroy_components;
    
    
    
    
    void ProcessOnStartQueue();
    void ProcessAddedComponents();
    void ProcessRemovedComponents();
    
    void InjectConvenienceReference(std::shared_ptr<luabridge::LuaRef> component_ref);
    void Update();
    void LateUpdate();
    void ReportError(std::string & actor_name, const luabridge::LuaException & e);
    
    
    std::string GetName();
    int GetID();
    luabridge::LuaRef GetComponentByKey(std::string key);
    luabridge::LuaRef GetComponent(std::string type_name);
    luabridge::LuaRef GetComponents(std::string type_name);
    
    luabridge::LuaRef AddComponent(std::string type_name);
    void RemoveComponent(luabridge::LuaRef component_ref);
    
    
    
    
    
    
    
    static inline int next_actor_id = 0;
    Actor() : id(next_actor_id++) {
    }
    Actor& operator=(const Actor& other);
    
private:
    
};

class ComponentsAddition{
public:
    static int Num_AddComponentsCalled;
};
#endif /* Actor_h */
