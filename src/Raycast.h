//
//  Raycast.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/22.
//

#ifndef Raycast_h
#define Raycast_h
#include "Actor.h"
#include "ComponentDB.h"
#include "box2d/box2d.h"
#include "RigidBody.h"


class HitResult{
public:
    Actor* actor;
    b2Vec2 point;
    b2Vec2 normal;
    bool is_trigger;
};

class Physics{
public:
    static HitResult* Raycast(b2Vec2 pos, b2Vec2 dir, float dist);
    static luabridge::LuaRef RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist);
};

class RayCastCallback: public b2RayCastCallback{
public:
    float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
    bool hasHit = false;
    std::map<float, HitResult*> hitResult;
};


#endif /* Raycast_h */
