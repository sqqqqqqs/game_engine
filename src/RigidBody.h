//
//  RigidBody.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/20.
//

#ifndef RigidBody_h
#define RigidBody_h
#include<string>
#include "glm/glm.hpp"
#include "Actor.h"
#include "box2d/box2d.h"
#include "ContactListener.h"

class Actor;

class RigidbodyComponent{
public:
    std::string type = "Rigidbody";
    std::string key = "???";
    bool enabled = true;
    bool started = false;
    Actor* actor = nullptr;
    
    float x = 0.0f;
    float y = 0.0f;
    std::string body_type = "dynamic";
    bool precise = true;
    float gravity_scale = 1.0f;
    float density = 1.0f;
    float angular_friction = 0.3f;
    float rotation = 0.0f;
    bool lock_rotation = false;
    
    
    //collider-related properties
    bool has_collider = true;
    std::string collider_type = "box";
    float width = 1.0f;
    float height = 1.0f;
    float radius = 0.5f;
    float friction = 0.3f;
    float bounciness = 0.3f;
    
    
    //trigger-related properties
    bool has_trigger = true;
    std::string trigger_type = "box";
    float trigger_width = 1.0f;
    float trigger_height = 1.0f;
    float trigger_radius = 0.5f;
    
    
    b2Body* body = nullptr;
    
    
    //get functions
    b2Vec2 GetPosition();
    float GetRotation();
    b2Vec2 GetVelocity();
    float GetAngularVelocity();
    float GetGravityScale();
    b2Vec2 GetUpDirection();
    b2Vec2 GetRightDirection();
    
    //add functions
    void AddForce(b2Vec2 force);
    
    //set functions
    void SetVelocity(b2Vec2 velocity);
    void SetPosition(b2Vec2 position);
    void SetRotation(float degrees_clockwise);
    void SetAngularVelocity(float degrees_clockwise);
    void SetGravityScale(float gravity_scale);
    void SetUpDirection(b2Vec2 direction);
    void SetRightDirection(b2Vec2 direction);
    

    void OnStart();
    void OnDestroy();
};

class World{
public:
    static b2World* world;
    static bool physics_on;
    static void InitWorld();
    static void PhysicsStep();
};


#endif /* RigidBody_h */
