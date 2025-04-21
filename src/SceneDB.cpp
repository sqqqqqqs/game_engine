//
//  SceneDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/1/29.
//

#include "SceneDB.h"
#include "EngineUtils.h"
#include "TemplateDB.h"
#include "AudioDB.h"
#include "Renderer.h"
#include "ImageDB.h"
#include <iostream>
#include <filesystem>

Scene SceneDB::current_scene = Scene();
std::string SceneDB::next_scene_name = "";
std::vector<Actor*> SceneDB::actors_to_add;
std::vector<Actor*> SceneDB::actors_to_destroy;


bool SceneDB::LoadScene(const std::string &sceneName){
    std::string scenePath = "resources/scenes/"+ sceneName + ".scene";
    if(!std::filesystem::exists(scenePath)){
        std::cout<<"error: scene "+ sceneName +" is missing";
        std::exit(0);
    }
    rapidjson::Document sceneData;
    EngineUtils::ReadJsonFile(scenePath, sceneData);
    
    std::vector<Actor*> persistent_actors;
    
    for (Actor* actor : current_scene.actors) {
        if (actor->dont_destory_on_load) {
            persistent_actors.push_back(actor);
        } else {
            delete actor;
        }
    }
    current_scene.actors.clear();
    current_scene.actors = std::move(persistent_actors);
    current_scene.scene_name = sceneName;
    
    for(const auto &actorData:sceneData["actors"].GetArray()){
        Actor* actor = new Actor();
        
        if(actorData.HasMember("template")){
            std::string template_path = actorData["template"].GetString();
            
            if(!TemplateDB::hasTemplate(template_path)){
                std::cout<<"error: template " << template_path << " is missing";
                std::exit(0);
            }
            *actor =TemplateDB::GetTemplate(template_path);
        }
        actor->id = Actor::next_actor_id;
        if (actorData.HasMember("name")) actor->name = actorData["name"].GetString();
        current_scene.actors.push_back(actor);
        
        if (actorData.HasMember("components")) {
            for (const auto& componentEntry : actorData["components"].GetObject()) {
                std::string componentKey = componentEntry.name.GetString();
                
                AddComponentToActor(*actor, componentKey, componentEntry.value);
                }
        }

    }
    return true;
}

void SceneDB::AddComponentToActor(Actor& actor, const std::string& key, const rapidjson::Value& componentData) {
    std::string type = componentData.HasMember("type") ? componentData["type"].GetString() : "";
    if(!type.empty()&&(actor.components.find(type) == actor.components.end() || actor.components[type].find(key) == actor.components[type].end())){
        if (ComponentDB::componentTables.find(type) == ComponentDB::componentTables.end()&&type!="Rigidbody"&&type!="ParticleSystem"&&type!="Animator"&&type!="Network") {
            std::cout << "error: failed to locate component " << type;
            std::exit(0);
        }
        luabridge::LuaRef newComponent = luabridge::newTable(ComponentDB::luaState);
        if(type=="Rigidbody"){
            newComponent = ComponentDB::NewRigidBody();
        }else if(type=="ParticleSystem"){
            newComponent = ComponentDB::NewParticleSystem();
        }else if(type=="Animator"){
            newComponent = ComponentDB::NewAnimator();
        }else if(type=="Network"){
            newComponent = ComponentDB::NewNetwork();
        }else{
            ComponentDB::EstablishInheritance(newComponent, *ComponentDB::componentTables[type].get());
        }
        
        newComponent["key"] = key;
        newComponent["type"] = type;
        newComponent["enabled"] = true;
        newComponent["started"] = false;
        
        for (auto itr = componentData.MemberBegin(); itr != componentData.MemberEnd(); ++itr) {
            std::string propertyName = itr->name.GetString();
            if (propertyName == "type") continue;
            
            if (itr->value.IsString()) {
                newComponent[propertyName] = itr->value.GetString();
            } else if (itr->value.IsBool()) {
                newComponent[propertyName] = itr->value.GetBool();
            } else if (itr->value.IsFloat()) {
                newComponent[propertyName] = itr->value.GetFloat();
            } else if (itr->value.IsInt()) {
                newComponent[propertyName] = itr->value.GetInt();
            } else if (itr->value.IsArray()) {
                luabridge::LuaRef arr = luabridge::LuaRef::newTable(ComponentDB::luaState);
                int index = 1;
                for (auto& element : itr->value.GetArray()) {
                    if (element.IsString())
                        arr[index++] = element.GetString();
                    else if (element.IsInt())
                        arr[index++] = element.GetInt();
                    else if (element.IsFloat())
                        arr[index++] = element.GetFloat();
                    else if (element.IsBool())
                        arr[index++] = element.GetBool();
                    else
                        std::cout << "[Warning] Unsupported array element type in component: " << propertyName << std::endl;
                }
                newComponent[propertyName] = arr;
            }
        }
        
        auto componentPtr = std::make_shared<luabridge::LuaRef>(newComponent);
        actor.InjectConvenienceReference(componentPtr);
        actor.components[type][key] = componentPtr;
        if((*componentPtr)["OnUpdate"].isFunction()){
            actor.update_components[key] = componentPtr;
        }
        if((*componentPtr)["OnLateUpdate"].isFunction()){
            actor.late_update_components[key] = componentPtr;
        }
        if((*componentPtr)["OnCollisionEnter"].isFunction()){
            actor.collision_enter_components.push(key);
        }
        if((*componentPtr)["OnCollisionExit"].isFunction()){
            actor.collision_exit_components.push(key);
        }
        if((*componentPtr)["OnTriggerEnter"].isFunction()){
            actor.trigger_enter_components.push(key);
        }
        if((*componentPtr)["OnTriggerExit"].isFunction()){
            actor.trigger_exit_components.push(key);
        }
        if((*componentPtr)["OnDestroy"].isFunction()){
            actor.on_destroy_components.insert(key);
        }
        
        actor.onStartQueue.push_back(componentPtr);
    }else{
        for (auto& [existingType, componentMap] : actor.components) {
            if (componentMap.find(key) != componentMap.end()) {
                auto& existingComponent = componentMap[key];
                for (auto itr = componentData.MemberBegin(); itr != componentData.MemberEnd(); ++itr) {
                    std::string propertyName = itr->name.GetString();
                    if (propertyName == "type") continue;
                    
                    if (itr->value.IsString()) {
                        (*existingComponent)[propertyName] = itr->value.GetString();
                    } else if (itr->value.IsBool()) {
                        (*existingComponent)[propertyName] = itr->value.GetBool();
                    } else if (itr->value.IsFloat()) {
                        (*existingComponent)[propertyName] = itr->value.GetFloat();
                    } else if (itr->value.IsInt()) {
                        (*existingComponent)[propertyName] = itr->value.GetInt();
                    }
                }
                return;
            }
        }
    }
}


Actor* SceneDB::FindActor(std::string name){
    for (Actor* actor : current_scene.actors) {
        if (actor->name == name && std::find(actors_to_destroy.begin(), actors_to_destroy.end(), actor) == actors_to_destroy.end()) {
            return actor;
        }
    }
    for (Actor* actor : actors_to_add) {
        if (actor->name == name) {
            return actor;
        }
    }
    return nullptr;
}


luabridge::LuaRef SceneDB::FindAllActors(std::string name) {
    luabridge::LuaRef actorTable = luabridge::newTable(ComponentDB::luaState);
    int index = 1;

    for (Actor* actor : current_scene.actors) {
        if (actor->name == name && std::find(actors_to_destroy.begin(), actors_to_destroy.end(), actor) == actors_to_destroy.end()) {
            actorTable[index++] = actor;
        }
    }
    for(Actor* actor : actors_to_add){
        if (actor->name == name) {
            actorTable[index++] = actor;
        }
    }
    return actorTable;
}


void SceneDB::UpdateActors(){
    if(Helper::GetFrameNumber()!=0){
        ProcessAddedActors();
    }
    for(Actor* actor: current_scene.actors){
        actor->ProcessAddedComponents();
    }
    for(Actor* actor: current_scene.actors){
        actor->Update();
    }
    for(Actor* actor: current_scene.actors){
        actor->ProcessRemovedComponents();
    }
}

void SceneDB::LateUpdateActors() {
    for (Actor* actor : current_scene.actors) {
        actor->LateUpdate();
    }
    ProcessDestroyedActors();
}

Actor* SceneDB::AddActorWithTemplate(std::string actor_template_name){
    Actor* actor = new Actor();
    *actor =TemplateDB::GetTemplate(actor_template_name);
    actors_to_add.push_back(actor);
    actor->id = Actor::next_actor_id;
    return actor;
}

void SceneDB::ProcessAddedActors(){
    for(Actor* actor:actors_to_add){
        for(auto& [type, componentMap]: actor->components){
            for (auto& [key, component] : componentMap) {
                if((*component)["enabled"]==true){
                    (*component)["started"]=true;
                    if((*component)["OnStart"].isFunction()){
                        try {
                            (*component)["OnStart"](*component);
                        } catch (const luabridge::LuaException& e) {
                            std::string error_message = e.what();
                            std::replace(error_message.begin(),error_message.end(),'\\','/');
                            std::cout << "\033[31m" << actor->name << " : " << error_message << "\033[0m" << std::endl;
                        }
                    }
                }
            }
        }
        current_scene.actors.push_back(actor);
    }
    actors_to_add.clear();
    
}

void SceneDB::Destroy(Actor *actor){
    for(auto& [type, componentMap]: actor->components){
        for (auto& [key, component] : componentMap) {
            (*component)["enabled"]=false;
            actor->components_to_remove.push_back(key);
        }
    }
    actors_to_destroy.push_back(actor);
}

void SceneDB::ProcessDestroyedActors(){
    for(Actor* actor:actors_to_destroy){
        actor->ProcessRemovedComponents();
        auto it = std::find(current_scene.actors.begin(), current_scene.actors.end(), actor);
        if (it != current_scene.actors.end()) {
            delete actor;
            current_scene.actors.erase(it);
        }
    }
    actors_to_destroy.clear();
}

std::vector<Actor*>& SceneDB::GetActors(){
    return current_scene.actors;
}

Actor* SceneDB::GetActorById(int id) {
    for (Actor* actor : current_scene.actors) {
        if (actor->id == id) {
            return actor;
        }
    }
    return nullptr; 
}


void SceneDB::LoadNewScene(const std::string &sceneName){
    SceneDB::next_scene_name = sceneName;
}

void SceneDB::ProcessNewScene(){
    if(!SceneDB::next_scene_name.empty()){
        LoadScene(next_scene_name);
        next_scene_name.clear();
        for(Actor* actor:SceneDB::GetActors()){
            actor->ProcessOnStartQueue();
        }
    }
}

std::string SceneDB::GetCurrentSceneName(){
    return current_scene.scene_name;
}

void SceneDB::DontDestroy(Actor *actor){
    if(actor){
        actor->dont_destory_on_load = true;
    }
}

