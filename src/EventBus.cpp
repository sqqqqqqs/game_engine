//
//  EventBus.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/22.
//

#include <stdio.h>
#include "EventBus.h"

std::unordered_map<std::string, std::vector<Event::SubPair>> Event::active_subs;
std::vector<std::tuple<std::string, std::shared_ptr<luabridge::LuaRef>, std::shared_ptr<luabridge::LuaRef>>> Event::subscribe_queue;
std::vector<std::tuple<std::string, std::shared_ptr<luabridge::LuaRef>, std::shared_ptr<luabridge::LuaRef>>> Event::unsubscribe_queue;


void Event::Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function){
    subscribe_queue.emplace_back(event_type,std::make_shared<luabridge::LuaRef>(component),std::make_shared<luabridge::LuaRef>(function));
}


void Event::Unsubscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function){
    unsubscribe_queue.emplace_back(event_type,std::make_shared<luabridge::LuaRef>(component),std::make_shared<luabridge::LuaRef>(function));
}

void Event::Publish(std::string event_type, luabridge::LuaRef event_object){
    std::unordered_map<std::string, std::vector<SubPair>>::iterator it = active_subs.find(event_type);
    if (active_subs.find(event_type) == active_subs.end()) return;
    for (auto iter = it->second.begin(); iter != it->second.end(); ++iter) {
        const std::shared_ptr<luabridge::LuaRef>& component = iter->first;
        const std::shared_ptr<luabridge::LuaRef>& function = iter->second;
        if (function && function->isFunction()) {
            (*function)(*component, event_object);
        }
    }
}

void Event::ApplyQueuedChanges(){
    for (auto it = unsubscribe_queue.begin(); it != unsubscribe_queue.end(); ++it) {
        const std::string& type = std::get<0>(*it);
        const std::shared_ptr<luabridge::LuaRef>& comp = std::get<1>(*it);
        const std::shared_ptr<luabridge::LuaRef>& func = std::get<2>(*it);

        std::vector<SubPair>& subs = active_subs[type];
        subs.erase(std::remove_if(subs.begin(), subs.end(), [&](const SubPair& p) { return p.first->tostring() == comp->tostring() && p.second->tostring() == func->tostring();}), subs.end());
        }
    unsubscribe_queue.clear();
    for (auto it = subscribe_queue.begin(); it != subscribe_queue.end(); ++it) {
        const std::string& type = std::get<0>(*it);
        const std::shared_ptr<luabridge::LuaRef>& comp = std::get<1>(*it);
        const std::shared_ptr<luabridge::LuaRef>& func = std::get<2>(*it);
        active_subs[type].push_back(std::make_pair(comp, func));
    }
    subscribe_queue.clear();
}

