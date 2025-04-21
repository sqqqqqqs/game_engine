//
//  ComponentDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/10.
//

#include <stdio.h>
#include "ComponentDB.h"


void ComponentDB::InitilizeLuaState(){
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
}

void ComponentDB::LoadLuaComponents() {
    std::string componentDirectoryPath = "resources/component_types";

    if (std::filesystem::exists(componentDirectoryPath)) {
        for (const auto& componentFile : std::filesystem::directory_iterator(componentDirectoryPath)) {
            if (componentFile.path().extension() == ".lua") {
                std::string componentName = componentFile.path().stem().string();
                
                
                if (luaL_dofile(luaState, componentFile.path().string().c_str()) != LUA_OK) {
                    std::cout << "problem with lua file " << componentName;
                    std::exit(0);
                }


                luabridge::LuaRef componentTable = luabridge::getGlobal(luaState, componentName.c_str());
                if (!componentTable.isTable()) {
                    std::cout << "problem with lua file " << componentName;
                    std::exit(0);
                }

                componentTables[componentName] = std::make_shared<luabridge::LuaRef>(componentTable);
            }
        }
    }
}

void ComponentDB::CreatingGlobal(){
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Debug")
        .addFunction("Log", ComponentDB::CppLog)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginClass<Actor>("Actor")
        .addFunction("GetName", &Actor::GetName)
        .addFunction("GetID", &Actor::GetID)
        .addFunction("GetComponentByKey", &Actor::GetComponentByKey)
        .addFunction("GetComponent", &Actor::GetComponent)
        .addFunction("GetComponents", &Actor::GetComponents)
        .addFunction("AddComponent", &Actor::AddComponent)
        .addFunction("RemoveComponent", &Actor::RemoveComponent)
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Actor")
        .addFunction("Find", SceneDB::FindActor)
        .addFunction("FindAll", SceneDB::FindAllActors)
        .addFunction("Instantiate", SceneDB::AddActorWithTemplate)
        .addFunction("Destroy", SceneDB::Destroy)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Application")
        .addFunction("Quit", Application::Quit)
        .addFunction("Sleep", Application::Sleep)
        .addFunction("GetFrame", Application::GetFrame)
        .addFunction("OpenURL", Application::OpenURL)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginClass<glm::vec2>("vec2")
        .addProperty("x", &glm::vec2::x)
        .addProperty("y", &glm::vec2::y)
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Input")
        .addFunction("GetKey", Input::GetKey)
        .addFunction("GetKeyDown", Input::GetKeyDown)
        .addFunction("GetKeyUp", Input::GetKeyUp)
        .addFunction("GetMousePosition", Input::GetMousePosition)
        .addFunction("GetMouseButton", Input::GetMouseButton)
        .addFunction("GetMouseButtonDown", Input::GetMouseButtonDown)
        .addFunction("GetMouseButtonUp", Input::GetMouseButtonUp)
        .addFunction("GetMouseScrollDelta", Input::GetMouseScrollDelta)
        .addFunction("ShowCursor", Input::ShowCursor)
        .addFunction("HideCursor", Input::HideCursor)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Text")
        .addFunction("Draw", Renderer::DrawText)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Audio")
        .addFunction("Play", AudioDB::Play)
        .addFunction("Halt", AudioDB::Halt)
        .addFunction("SetVolume", AudioDB::SetVolume)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Image")
        .addFunction("DrawUI", Renderer::DrawUI)
        .addFunction("DrawUIEx", Renderer::DrawUIEx)
        .addFunction("Draw", Renderer::Draw)
        .addFunction("DrawEx", Renderer::DrawEx)
        .addFunction("DrawPixel", Renderer::DrawPixel)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Camera")
        .addFunction("SetPosition", Renderer::SetCameraPosition)
        .addFunction("GetPositionX", Renderer::GetCameraPositionX)
        .addFunction("GetPositionY", Renderer::GetCameraPositionY)
        .addFunction("SetZoom", Renderer::SetZoom)
        .addFunction("GetZoom", Renderer::GetZoom)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Scene")
        .addFunction("Load", SceneDB::LoadNewScene)
        .addFunction("GetCurrent", SceneDB::GetCurrentSceneName)
        .addFunction("DontDestroy", SceneDB::DontDestroy)
        .endNamespace();
    
    luabridge::getGlobalNamespace(luaState)
        .beginClass<b2Vec2>("Vector2")
        .addConstructor<void(*) (float, float)>()
        .addProperty("x", &b2Vec2::x)
        .addProperty("y", &b2Vec2::y)
        .addFunction("Normalize", &b2Vec2::Normalize)
        .addFunction("Length", &b2Vec2::Length)
        .addFunction("__add", &b2Vec2::operator_add)
        .addFunction("__sub", &b2Vec2::operator_sub)
        .addFunction("__mul", &b2Vec2::operator_mul)
        .addStaticFunction("Distance", &b2Distance)
        .addStaticFunction("Dot", static_cast<float (*)(const b2Vec2&, const b2Vec2&)>(&b2Dot))
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
        .beginClass<Collision>("collision")
        .addData("other", &Collision::other)
        .addData("point", &Collision::point)
        .addData("relative_velocity", &Collision::relative_velocity)
        .addData("normal", &Collision::normal)
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
        .beginClass<RigidbodyComponent>("Rigidbody")
        .addData("type", &RigidbodyComponent::type)
        .addData("key", &RigidbodyComponent::key)
        .addData("enabled", &RigidbodyComponent::enabled)
        .addData("started", &RigidbodyComponent::started)
        .addData("actor", &RigidbodyComponent::actor)
        .addData("x", &RigidbodyComponent::x)
        .addData("y", &RigidbodyComponent::y)
        .addData("body_type", &RigidbodyComponent::body_type)
        .addData("precise", &RigidbodyComponent::precise)
        .addData("gravity_scale", &RigidbodyComponent::gravity_scale)
        .addData("density", &RigidbodyComponent::density)
        .addData("angular_friction", &RigidbodyComponent::angular_friction)
        .addData("rotation", &RigidbodyComponent::rotation)
        .addData("has_collider", &RigidbodyComponent::has_collider)
        .addData("has_trigger", &RigidbodyComponent::has_trigger)
        .addData("lock_rotation", &RigidbodyComponent::lock_rotation)
    
        //Test1
        .addFunction("OnStart", &RigidbodyComponent::OnStart)
        .addFunction("GetPosition", &RigidbodyComponent::GetPosition)
        .addFunction("GetRotation", &RigidbodyComponent::GetRotation)
    
        //Test2
        .addFunction("AddForce", &RigidbodyComponent::AddForce)
        .addFunction("SetVelocity", &RigidbodyComponent::SetVelocity)
        .addFunction("SetPosition", &RigidbodyComponent::SetPosition)
        .addFunction("SetRotation", &RigidbodyComponent::SetRotation)
        .addFunction("SetAngularVelocity", &RigidbodyComponent::SetAngularVelocity)
        .addFunction("SetGravityScale", &RigidbodyComponent::SetGravityScale)
        .addFunction("SetUpDirection", &RigidbodyComponent::SetUpDirection)
        .addFunction("SetRightDirection", &RigidbodyComponent::SetRightDirection)
        .addFunction("GetVelocity", &RigidbodyComponent::GetVelocity)
        .addFunction("GetAngularVelocity", &RigidbodyComponent::GetAngularVelocity)
        .addFunction("GetGravityScale", &RigidbodyComponent::GetGravityScale)
        .addFunction("GetUpDirection", &RigidbodyComponent::GetUpDirection)
        .addFunction("GetRightDirection", &RigidbodyComponent::GetRightDirection)
    
        //Test3
        .addData("width", &RigidbodyComponent::width)
        .addData("height", &RigidbodyComponent::height)
        .addData("radius", &RigidbodyComponent::radius)
        .addData("friction", &RigidbodyComponent::friction)
        .addData("bounciness", &RigidbodyComponent::bounciness)
        .addData("collider_type", &RigidbodyComponent::collider_type)
    
        //Test5
        .addData("trigger_type", &RigidbodyComponent::trigger_type)
        .addData("trigger_width", &RigidbodyComponent::trigger_width)
        .addData("trigger_height", &RigidbodyComponent::trigger_height)
        .addData("trigger_radius", &RigidbodyComponent::trigger_radius)
    
        //Test7
        .addFunction("OnDestroy", &RigidbodyComponent::OnDestroy)
    
    
        .endClass();
    
    //Test6
    luabridge::getGlobalNamespace(luaState)
        .beginClass<HitResult>("HitResult")
        .addData("actor", &HitResult::actor)
        .addData("point", &HitResult::point)
        .addData("is_trigger", &HitResult::is_trigger)
        .addData("normal", &HitResult::normal)
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Physics")
        .addFunction("Raycast", Physics::Raycast)
        .addFunction("RaycastAll", Physics::RaycastAll)
        .endNamespace();
    
    //Test8
    luabridge::getGlobalNamespace(luaState)
        .beginNamespace("Event")
        .addFunction("Publish", Event::Publish)
        .addFunction("Subscribe", Event::Subscribe)
        .addFunction("Unsubscribe", Event::Unsubscribe)
        .endNamespace();
    
    //HW9
    luabridge::getGlobalNamespace(luaState)
        .beginClass<ParticleSystem>("ParticleSystem")
        .addData("type", &ParticleSystem::type)
        .addData("key", &ParticleSystem::key)
        .addData("enabled", &ParticleSystem::enabled)
        .addData("started", &ParticleSystem::started)
        .addData("actor", &ParticleSystem::actor)
        .addData("x", &ParticleSystem::x)
        .addData("y", &ParticleSystem::y)
        .addData("frames_between_bursts", &ParticleSystem::frames_between_bursts)
        .addData("burst_quantity", &ParticleSystem::burst_quantity)
        .addData("start_scale_min", &ParticleSystem::start_scale_min)
        .addData("start_scale_max", &ParticleSystem::start_scale_max)
        .addData("rotation_min", &ParticleSystem::rotation_min)
        .addData("rotation_max", &ParticleSystem::rotation_max)
        .addData("start_color_r", &ParticleSystem::start_color_r)
        .addData("start_color_g", &ParticleSystem::start_color_g)
        .addData("start_color_b", &ParticleSystem::start_color_b)
        .addData("start_color_a", &ParticleSystem::start_color_a)
        .addData("emit_radius_min", &ParticleSystem::emit_radius_min)
        .addData("emit_radius_max", &ParticleSystem::emit_radius_max)
        .addData("emit_angle_min", &ParticleSystem::emit_angle_min)
        .addData("emit_angle_max", &ParticleSystem::emit_angle_max)
        .addData("image", &ParticleSystem::image)
        .addData("sorting_order", &ParticleSystem::sorting_order)
        .addData("duration_frames", &ParticleSystem::duration_frames)
        .addData("start_speed_min", &ParticleSystem::start_speed_min)
        .addData("start_speed_max", &ParticleSystem::start_speed_max)
        .addData("rotation_speed_min", &ParticleSystem::rotation_speed_min)
        .addData("rotation_speed_max", &ParticleSystem::rotation_speed_max)
        .addData("gravity_scale_x", &ParticleSystem::gravity_scale_x)
        .addData("gravity_scale_y", &ParticleSystem::gravity_scale_y)
        .addData("drag_factor", &ParticleSystem::drag_factor)
        .addData("angular_drag_factor", &ParticleSystem::angular_drag_factor)
        .addData("end_scale", &ParticleSystem::end_scale)
        .addData("end_color_r", &ParticleSystem::end_color_r)
        .addData("end_color_g", &ParticleSystem::end_color_g)
        .addData("end_color_b", &ParticleSystem::end_color_b)
        .addData("end_color_a", &ParticleSystem::end_color_a)
        .addFunction("OnStart", &ParticleSystem::OnStart)
        .addFunction("OnUpdate", &ParticleSystem::OnUpdate)
        .addFunction("Stop", &ParticleSystem::Stop)
        .addFunction("Play", &ParticleSystem::Play)
        .addFunction("Burst", &ParticleSystem::Burst)
        .endClass();
    
    //custom feature
    luabridge::getGlobalNamespace(luaState)
        .beginClass<AnimatorComponent>("Animator")
        .addData("type", &AnimatorComponent::type)
        .addData("key", &AnimatorComponent::key)
        .addData("enabled", &AnimatorComponent::enabled)
        .addData("started", &AnimatorComponent::started)
        .addData("actor", &AnimatorComponent::actor)
        .addData("frame_duration", &AnimatorComponent::frame_duration)
        .addData("sprite_prefix", &AnimatorComponent::sprite_prefix)
        .addData("frame_count", &AnimatorComponent::frame_count)
        .addData("loop", &AnimatorComponent::loop)
        .addData("loops_before_destroy", &AnimatorComponent::loops_before_destroy)
        .addData("destroy_on_finish", &AnimatorComponent::destroy_on_finish)
        .addFunction("OnStart", &AnimatorComponent::OnStart)
        .addFunction("OnUpdate", &AnimatorComponent::OnUpdate)
        .endClass();
    
    luabridge::getGlobalNamespace(luaState)
    .beginClass<NetworkComponent>("Network")
        .addData("type", &NetworkComponent::type)
        .addData("key", &NetworkComponent::key)
        .addData("enabled", &NetworkComponent::enabled)
        .addData("started", &NetworkComponent::started)
        .addData("actor", &NetworkComponent::actor)
        .addData("is_server", &NetworkComponent::is_server)
        .addData("target_actor_key", &NetworkComponent::target_actor_key)
        .addData("port", &NetworkComponent::port)
        .addData("send_mode", &NetworkComponent::send_mode)
        .addData("server_ip", &NetworkComponent::server_ip)
        .addFunction("OnStart", &NetworkComponent::OnStart)
        .addFunction("OnUpdate", &NetworkComponent::OnUpdate)
    .endClass();
}

std::shared_ptr<luabridge::LuaRef> ComponentDB::GetComponentByName(std::string componentName){
    if(componentTables.find(componentName)!=componentTables.end()){
        return componentTables[componentName];
    }
    std::cout << "error: failed to locate component " << componentName;
    std::exit(0);
}

void ComponentDB::EstablishInheritance(luabridge::LuaRef & instance_table, luabridge::LuaRef & parent_table)
{
    /* We must create a metatable to establish inheritance in Lua */
    luabridge::LuaRef new_metatable = luabridge::newTable(luaState);
    new_metatable["__index"] = parent_table;
    
    /* We must use the raw lua C-API (lua stack) to preform a "setmetatable" operation */
    instance_table.push(luaState);
    new_metatable.push(luaState);
    lua_setmetatable(luaState, -2);
    lua_pop(luaState, 1);
}


void ComponentDB::CppLog(std::string message){
    std::cout<<message<<std::endl;
}

luabridge::LuaRef ComponentDB::NewRigidBody(){
    RigidbodyComponent* rigidbody = new RigidbodyComponent();
    luabridge::LuaRef newComponent(luaState,rigidbody);
    return newComponent;
}


luabridge::LuaRef ComponentDB::NewParticleSystem(){
    ParticleSystem* particlesystem = new ParticleSystem();
    luabridge::LuaRef newComponent(luaState,particlesystem);
    return newComponent;
}

luabridge::LuaRef ComponentDB::NewAnimator() {
    AnimatorComponent* anim = new AnimatorComponent();
    luabridge::LuaRef newComponent(luaState,anim);
    return newComponent;
}

luabridge::LuaRef ComponentDB::NewNetwork() {
    NetworkComponent* net = new NetworkComponent();
    luabridge::LuaRef newComponent(luaState,net);
    return newComponent;
}




