//
//  ComponentDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/10.
//

#ifndef ComponentDB_h
#define ComponentDB_h
#include "lua.hpp"
#include "LuaBridge.h"
#include "SceneDB.h"
#include "Renderer.h"
#include <unordered_map>
#include "Actor.h"
#include "Input.h"
#include "AudioDB.h"
#include "Application.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include "RigidBody.h"
#include "box2d/box2d.h"
#include "Raycast.h"
#include "EventBus.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "NetworkComponent.h"

class ComponentDB{
    
public:
    static inline lua_State* luaState;
    static inline std::unordered_map<std::string, std::shared_ptr<luabridge::LuaRef>> componentTables;
    static void InitilizeLuaState();
    static void LoadLuaComponents();
    static void CreatingGlobal();
    static std::shared_ptr<luabridge::LuaRef> GetComponentByName(std::string componentName);
    static void EstablishInheritance(luabridge::LuaRef & instance_table, luabridge::LuaRef & parent_table);
    static luabridge::LuaRef NewRigidBody();
    static luabridge::LuaRef NewParticleSystem();
    static luabridge::LuaRef NewAnimator();
    static luabridge::LuaRef NewNetwork();
    
private:
    static void CppLog(std::string message);
};

#endif /* ComponentDB_h */
