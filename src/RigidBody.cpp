//
//  RigidBody.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/20.
//

#include <stdio.h>
#include "RigidBody.h"
#include "box2d/box2d.h"
b2World* World::world;
bool World::physics_on = false;


void World::InitWorld(){
    if(!physics_on){
        physics_on = true;
        b2Vec2 gravity = {0.0f,9.8f};
        world = new b2World(gravity); 
        world->SetContactListener(new Contactlistener());
    }
}

void World::PhysicsStep(){
    if(physics_on){
        world->Step(1.0f / 60.0f, 8, 3);
    }
}


void RigidbodyComponent::OnStart(){
    World::InitWorld();
    b2BodyDef bodydef;
    
    if(body_type=="dynamic"){
        bodydef.type = b2_dynamicBody;
    }else if(body_type=="static"){
        bodydef.type = b2_staticBody;
    }else if(body_type=="kinematic"){
        bodydef.type = b2_kinematicBody;
    }
    
    bodydef.position.x = x;
    bodydef.position.y = y;
    
    bodydef.bullet = precise;
    bodydef.gravityScale = gravity_scale;
    bodydef.angularDamping = angular_friction;
    bodydef.angle = rotation * (b2_pi / 180.0f);
    bodydef.fixedRotation = lock_rotation;
    
    body = World::world->CreateBody(&bodydef);
    
    if (!has_collider && !has_trigger){
        b2PolygonShape phantom_shape;
        phantom_shape.SetAsBox(width * 0.5f, height * 0.5f);
            
        b2FixtureDef phantom_fixture_def;
        phantom_fixture_def.shape = &phantom_shape;
        phantom_fixture_def.density = density;
        
        phantom_fixture_def.isSensor = true;
        body->CreateFixture(&phantom_fixture_def);
    }
    if(has_collider){
        b2FixtureDef fixture;
        fixture.density = density;
        fixture.isSensor = false;
        fixture.restitution = bounciness;
        fixture.friction = friction;
        if(collider_type=="box"){
            b2PolygonShape box_shape;
            box_shape.SetAsBox(0.5f * width, 0.5f * height);
            fixture.shape = &box_shape;
            fixture.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&fixture);
        }else if(collider_type=="circle"){
            b2CircleShape circle_shape;
            circle_shape.m_radius = radius;
            fixture.shape = &circle_shape;
            fixture.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&fixture);
        }
    }
    if(has_trigger){
        b2FixtureDef fixture;
        fixture.density = density;
        fixture.isSensor = true;
        fixture.restitution = bounciness;
        fixture.friction = friction;
        if(trigger_type=="box"){
            b2PolygonShape box_shape;
            box_shape.SetAsBox(0.5f * trigger_width, 0.5f * trigger_height);
            fixture.shape = &box_shape;
            fixture.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&fixture);
        }else if(trigger_type=="circle"){
            b2CircleShape circle_shape;
            circle_shape.m_radius = trigger_radius;
            fixture.shape = &circle_shape;
            fixture.userData.pointer = reinterpret_cast<uintptr_t>(actor);
            body->CreateFixture(&fixture);
        }
    }
}




//functions
b2Vec2 RigidbodyComponent::GetPosition(){
    if(body==nullptr){
        return b2Vec2(x,y);
    }
    return body->GetPosition();
}

float RigidbodyComponent::GetRotation(){
    return body->GetAngle() * (180.0f / b2_pi);
}

void RigidbodyComponent::AddForce(b2Vec2 force){
    body->ApplyForceToCenter(force, true);
}

void RigidbodyComponent::SetVelocity(b2Vec2 velocity){
    body->SetLinearVelocity(velocity);
}

void RigidbodyComponent::SetPosition(b2Vec2 position){
    if(body==nullptr){
        x = position.x;
        y = position.y;
    }else{
        body->SetTransform(position, GetRotation());
    }
}

void RigidbodyComponent::SetRotation(float degrees_clockwise){
    body->SetTransform(GetPosition(), degrees_clockwise * (b2_pi / 180.0f));
}

void RigidbodyComponent::SetAngularVelocity(float degrees_clockwise){
    body->SetAngularVelocity(degrees_clockwise * (b2_pi / 180.0f));
}

void RigidbodyComponent::SetGravityScale(float gravity_scale){
    body->SetGravityScale(gravity_scale);
}

void RigidbodyComponent::SetUpDirection(b2Vec2 direction){
    direction.Normalize();
    float new_angle_radians = glm::atan(direction.x, -direction.y);
    body->SetTransform(GetPosition(), new_angle_radians);
}

void RigidbodyComponent::SetRightDirection(b2Vec2 direction){
    direction.Normalize();
    float upAngle = glm::atan(direction.x, -direction.y);
    float rightAngle = upAngle - (b2_pi / 2.0f);
    body->SetTransform(GetPosition(), rightAngle);
}

b2Vec2 RigidbodyComponent::GetVelocity(){
    return body->GetLinearVelocity();
}

float RigidbodyComponent::GetAngularVelocity(){
    return body->GetAngularVelocity() * (180.0f / b2_pi);
}

float RigidbodyComponent::GetGravityScale(){
    return body->GetGravityScale();
}

b2Vec2 RigidbodyComponent::GetUpDirection(){
    float angle = body->GetAngle();
    b2Vec2 result = b2Vec2(glm::sin(angle), -glm::cos(angle));
    result.Normalize();
    return result;
}

b2Vec2 RigidbodyComponent::GetRightDirection(){
    float angle = body->GetAngle();
    b2Vec2 result = b2Vec2(glm::cos(angle), glm::sin(angle));
    result.Normalize();
    return result;
}

void RigidbodyComponent::OnDestroy(){
    World::world->DestroyBody(body);
}
