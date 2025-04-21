//
//  Actor.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/12.
//

#include <stdio.h>
#include "Actor.h"

int ComponentsAddition::Num_AddComponentsCalled = 0;
std::map<std::string, std::shared_ptr<luabridge::LuaRef>> components;

void Actor::ProcessOnStartQueue() {
    if (onStartQueue.empty()) return;

    std::sort(onStartQueue.begin(), onStartQueue.end(), [](const std::shared_ptr<luabridge::LuaRef>& a, const std::shared_ptr<luabridge::LuaRef>& b) {
        return (*a)["key"].cast<std::string>() < (*b)["key"].cast<std::string>();
    });

    for (auto& component : onStartQueue) {
        if (component->operator[]("OnStart").isFunction() && (*component)["enabled"]==true && (*component)["started"]==false) {
        try {
            component->operator[]("OnStart")(*component);
        } catch (const luabridge::LuaException& e) {
            std::string error_message = e.what();
            std::replace(error_message.begin(),error_message.end(),'\\','/');
            std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
        }
        }
        component->operator[]("started") = true;
    }
    onStartQueue.clear();
}


Actor& Actor::operator=(const Actor& other) {
    if (this == &other) return *this;

    name = other.name;
    components.clear();
    onStartQueue.clear();

    for (const auto& [type, componentMap] : other.components) {
        for (const auto& [key, originalComponent] : componentMap) {
            luabridge::LuaRef newComponent = luabridge::newTable(ComponentDB::luaState);
            if(type!="Rigidbody"&&type!="ParticleSystem"&&type!="Animator"&&type!="Network"){
                ComponentDB::EstablishInheritance(newComponent, *originalComponent);
            }else{
                if(type=="Rigidbody"){
                    RigidbodyComponent* rigidbody = new RigidbodyComponent((*originalComponent).cast<RigidbodyComponent>());
                    newComponent = luabridge::LuaRef(ComponentDB::luaState, rigidbody);
                }else if(type=="ParticleSystem"){
                    ParticleSystem* particlesystem = new ParticleSystem((*originalComponent).cast<ParticleSystem>());
                    newComponent = luabridge::LuaRef(ComponentDB::luaState, particlesystem);
                }else if(type=="Animator"){
                    AnimatorComponent* anim = new AnimatorComponent((*originalComponent).cast<AnimatorComponent>());
                    newComponent = luabridge::LuaRef(ComponentDB::luaState, anim);
                }else if(type=="Network"){
                    NetworkComponent* net = new NetworkComponent();
                    newComponent = luabridge::LuaRef(ComponentDB::luaState, net);
                }
            }
            components[type][key] = std::make_shared<luabridge::LuaRef>(newComponent);
            InjectConvenienceReference(components[type][key]);
            
            if ((*components[type][key])["OnUpdate"].isFunction()) {
                update_components[key] = components[type][key];
            }
            if ((*components[type][key])["OnLateUpdate"].isFunction()) {
                late_update_components[key] = components[type][key];
            }
            if ((*components[type][key])["OnCollisionEnter"].isFunction()) {
                collision_enter_components.push(key);
            }
            if ((*components[type][key])["OnCollisionExit"].isFunction()) {
                collision_exit_components.push(key);
            }
            if ((*components[type][key])["OnTriggerEnter"].isFunction()) {
                trigger_enter_components.push(key);
            }
            if ((*components[type][key])["OnTriggerExit"].isFunction()) {
                trigger_exit_components.push(key);
            }
            if ((*components[type][key])["OnDestroy"].isFunction()) {
                on_destroy_components.insert(key);
            }
            
                onStartQueue.push_back(components[type][key]);
            }
        }
    return *this;
}

std::string Actor::GetName(){
    return name;
}

int Actor::GetID(){
    return id;
}

void Actor::InjectConvenienceReference(std::shared_ptr<luabridge::LuaRef> component_ref){
    (*component_ref)["actor"] = this;
}

luabridge::LuaRef Actor::GetComponentByKey(std::string key){
    for (auto& [type, componentMap] : components) {
        if (componentMap.find(key) != componentMap.end() && (*componentMap[key])["enabled"]==true) {
            return *componentMap[key];
        }
    }
    return luabridge::LuaRef(ComponentDB::luaState);
}

luabridge::LuaRef Actor::GetComponent(std::string type_name){
    if(!components.empty()){
        if (components.find(type_name) != components.end()) {
            for (auto& [key, component] : components[type_name]) {
                if ((*component)["enabled"]==true) {
                    return *component;
                }
            }
        }
        return luabridge::LuaRef(ComponentDB::luaState);
    }else{
        return luabridge::LuaRef(ComponentDB::luaState);
    }
}

luabridge::LuaRef Actor::GetComponents(std::string type_name){
    luabridge::LuaRef componentTable = luabridge::newTable(ComponentDB::luaState);
    int index = 1;
    if (components.find(type_name) != components.end()) {
        for (const auto& [key, component] : components[type_name]) {
            if ((*component)["enabled"]==true) {
                componentTable[index++] = *component;
                }
            }
        }
        return componentTable;
    }

void Actor::Update(){
    for (auto& [key, component] : update_components) {
        if ((*component)["enabled"]==true) {
        try {
            (*component)["OnUpdate"](*component);
        } catch (const luabridge::LuaException& e) {
            std::string error_message = e.what();
            std::replace(error_message.begin(),error_message.end(),'\\','/');
            std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
            }
        }
    }
}

void Actor::LateUpdate(){
    for (auto& [key, component] : late_update_components) {
        if ((*component)["enabled"]==true) {
        try {
            (*component)["OnLateUpdate"](*component);
        } catch (const luabridge::LuaException& e) {
            std::string error_message = e.what();
            std::replace(error_message.begin(),error_message.end(),'\\','/');
            std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
            }
        }
    }
}

luabridge::LuaRef Actor::AddComponent(std::string type_name){
    luabridge::LuaRef newComponent = luabridge::newTable(ComponentDB::luaState);
    std::string key = "r" + std::to_string(ComponentsAddition::Num_AddComponentsCalled);
    ComponentsAddition::Num_AddComponentsCalled++;
    if(type_name=="Rigidbody"){
        newComponent = ComponentDB::NewRigidBody();
    }else if(type_name=="ParticleSystem"){
        newComponent = ComponentDB::NewParticleSystem();
    }else if(type_name=="Animator"){
        newComponent = ComponentDB::NewAnimator();
    }else if(type_name=="Network"){
        newComponent = ComponentDB::NewNetwork();
    }else{
        ComponentDB::EstablishInheritance(newComponent, *ComponentDB::GetComponentByName(type_name));
    }
    newComponent["type"] = type_name;
    newComponent["key"] = key;
    newComponent["enabled"] = true;
    newComponent["started"] = false;
    auto componentPtr = std::make_shared<luabridge::LuaRef>(newComponent);
    InjectConvenienceReference(componentPtr);
    components_to_add[key] = componentPtr;
    return newComponent;
}

void Actor::RemoveComponent(luabridge::LuaRef component_ref){
    component_ref["enabled"] = false;
    components_to_remove.push_back(component_ref["key"]);
}

void Actor::ProcessAddedComponents(){
    std::vector<std::string> finished;
    for(auto& component: components_to_add){
        if((*component.second)["enabled"]==true){
            (*component.second)["started"]=true;
            if((*component.second)["OnStart"].isFunction()){
                try {
                    (*component.second)["OnStart"](*component.second);
                } catch (const luabridge::LuaException& e) {
                    std::string error_message = e.what();
                    std::replace(error_message.begin(),error_message.end(),'\\','/');
                    std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
                }
            }
            finished.push_back(component.first);
            std::string type = (*component.second)["type"].cast<std::string>();
            components[type][component.first] = component.second;
            if((*component.second)["OnUpdate"].isFunction()){
                update_components[component.first] = component.second;
            }
            if((*component.second)["OnLateUpdate"].isFunction()){
                late_update_components[component.first] = component.second;
            }
            if((*component.second)["OnCollisionEnter"].isFunction()){
                collision_enter_components.push(component.first);
            }
            if((*component.second)["OnCollisionExit"].isFunction()){
                collision_exit_components.push(component.first);
            }
            if((*component.second)["OnTriggerEnter"].isFunction()){
                trigger_enter_components.push(component.first);
            }
            if((*component.second)["OnTriggerExit"].isFunction()){
                trigger_exit_components.push(component.first);
            }
            if((*component.second)["OnDestroy"].isFunction()){
                on_destroy_components.insert(component.first);
            }
            
        }
    }
    for(auto key:finished){
        components_to_add.erase(key);
    }
}

void Actor::ProcessRemovedComponents(){
    std::sort(components_to_remove.begin(), components_to_remove.end());
    for(auto& component_key: components_to_remove){
        for (auto& [type, componentMap] : components) {
            if(componentMap.find(component_key) != componentMap.end()&&on_destroy_components.find(component_key)!=on_destroy_components.end()){
                luabridge::LuaRef OnDestroy = (*componentMap[component_key])["OnDestroy"];
                OnDestroy(*componentMap[component_key]);
                on_destroy_components.erase(component_key);
            }
            componentMap.erase(component_key);
        }
        update_components.erase(component_key);
        late_update_components.erase(component_key);
    }
    components_to_remove.clear();
}

//HW8 part
void Actor::CollisionEnter(Collision collision){
    if(collision_enter_components.empty()){
        return;
    }
    for(int i = 0; i < collision_enter_components.size(); i++){
        std::string current_component_key = collision_enter_components.top();
        collision_enter_components.pop();
        for (auto& [type, componentMap] : components) {
            if (componentMap.find(current_component_key) != componentMap.end()) {
                if((*componentMap[current_component_key])["enabled"]==true &&(*componentMap[current_component_key])["started"]==true){
                    try {
                        (*componentMap[current_component_key])["OnCollisionEnter"](*componentMap[current_component_key],collision);
                    } catch (const luabridge::LuaException& e) {
                        std::string error_message = e.what();
                        std::replace(error_message.begin(),error_message.end(),'\\','/');
                        std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
                    }
                    collision_enter_components.push(current_component_key);
                }
                }
            }
        }
        
    }

void Actor::CollisionExit(Collision collision){
    if(collision_exit_components.empty()){
        return;
    }
    for(int i = 0; i < collision_exit_components.size(); i++){
        std::string current_component_key = collision_exit_components.top();
        collision_exit_components.pop();
        for (auto& [type, componentMap] : components) {
            if (componentMap.find(current_component_key) != componentMap.end()) {
                if((*componentMap[current_component_key])["enabled"]==true &&(*componentMap[current_component_key])["started"]==true){
                    try {
                        (*componentMap[current_component_key])["OnCollisionExit"](*componentMap[current_component_key],collision);
                    } catch (const luabridge::LuaException& e) {
                        std::string error_message = e.what();
                        std::replace(error_message.begin(),error_message.end(),'\\','/');
                        std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
                    }
                    collision_exit_components.push(current_component_key);
                }
                }
            }
        }
}

void Actor::TriggerEnter(Collision collision){
    if(trigger_enter_components.empty()){
        return;
    }
    for(int i = 0; i < trigger_enter_components.size(); i++){
        std::string current_component_key = trigger_enter_components.top();
        trigger_enter_components.pop();
        for (auto& [type, componentMap] : components) {
            if (componentMap.find(current_component_key) != componentMap.end()) {
                if((*componentMap[current_component_key])["enabled"]==true &&(*componentMap[current_component_key])["started"]==true){
                    try {
                        (*componentMap[current_component_key])["OnTriggerEnter"](*componentMap[current_component_key],collision);
                    } catch (const luabridge::LuaException& e) {
                        std::string error_message = e.what();
                        std::replace(error_message.begin(),error_message.end(),'\\','/');
                        std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
                    }
                    trigger_enter_components.push(current_component_key);
                }
                }
            }
        }
}

void Actor::TriggerExit(Collision collision){
    if(trigger_exit_components.empty()){
        return;
    }
    for(int i = 0; i < trigger_exit_components.size(); i++){
        std::string current_component_key = trigger_exit_components.top();
        trigger_exit_components.pop();
        for (auto& [type, componentMap] : components) {
            if (componentMap.find(current_component_key) != componentMap.end()) {
                if((*componentMap[current_component_key])["enabled"]==true &&(*componentMap[current_component_key])["started"]==true){
                    try {
                        (*componentMap[current_component_key])["OnTriggerExit"](*componentMap[current_component_key],collision);
                    } catch (const luabridge::LuaException& e) {
                        std::string error_message = e.what();
                        std::replace(error_message.begin(),error_message.end(),'\\','/');
                        std::cout << "\033[31m" << name << " : " << error_message << "\033[0m" << std::endl;
                    }
                    trigger_exit_components.push(current_component_key);
                }
                }
            }
        }
}
    















void ReportError(std::string & actor_name, luabridge::LuaException & e){
    std::string error_message = e.what();
    std::replace(error_message.begin(),error_message.end(),'\\','/');
    std::cout << "\033[31m" << actor_name << " : " << error_message << "\033[0m" << std::endl;
}



