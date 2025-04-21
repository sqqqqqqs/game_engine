//
//  EventBus.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/22.
//

#ifndef EventBus_h
#define EventBus_h
#include "lua.hpp"
#include "LuaBridge.h"
#include <string>
#include <vector>
#include <tuple>
#include <memory>
#include <unordered_map>

class Event{
public:
    static void Publish(std::string event_type, luabridge::LuaRef event_object);
    static void Subscribe(std::string event_type, luabridge::LuaRef component, luabridge::LuaRef function);
    static void Unsubscribe(std::string event_type, luabridge::LuaRef  component, luabridge::LuaRef  function);
    static void ApplyQueuedChanges();
    
private:
    using SubPair = std::pair<std::shared_ptr<luabridge::LuaRef>, std::shared_ptr<luabridge::LuaRef>>;
    static std::unordered_map<std::string, std::vector<SubPair>> active_subs;
    static std::vector<std::tuple<std::string, std::shared_ptr<luabridge::LuaRef>, std::shared_ptr<luabridge::LuaRef>>> subscribe_queue;
    static std::vector<std::tuple<std::string, std::shared_ptr<luabridge::LuaRef>, std::shared_ptr<luabridge::LuaRef>>> unsubscribe_queue;
};


#endif /* EventBus_h */
