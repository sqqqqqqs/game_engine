//
//  Raycast.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/22.
//

#include <stdio.h>
#include "Raycast.h"


HitResult* Physics::Raycast(b2Vec2 pos, b2Vec2 dir, float dist){
    if(dist <= 0.0f || World::physics_on==false){
        return luabridge::LuaRef(ComponentDB::luaState);
    }
    RayCastCallback callback;
    b2Vec2 point2 = dir.operator_mul(dist).operator_add(pos);
    World::world->RayCast(&callback, pos, point2);
    if(callback.hasHit==true){
        return callback.hitResult.begin()->second;
    }
    return luabridge::LuaRef(ComponentDB::luaState);
}

luabridge::LuaRef Physics::RaycastAll(b2Vec2 pos, b2Vec2 dir, float dist){
    luabridge::LuaRef hitresults = luabridge::newTable(ComponentDB::luaState);
    if(dist <= 0.0f || World::physics_on==false){
        return hitresults;
    }
    RayCastCallback callback;
    b2Vec2 point2 = dir.operator_mul(dist).operator_add(pos);
    World::world->RayCast(&callback, pos, point2);
    if(callback.hasHit==true){
        int index = 1;
        for(const auto & [dist, hit]: callback.hitResult){
            hitresults[index++] = hit;
        }
    }
    return hitresults;
}

float RayCastCallback::ReportFixture(b2Fixture *fixture, const b2Vec2 &point, const b2Vec2 &normal, float fraction){
    Actor* actor = reinterpret_cast<Actor*>(fixture->GetUserData().pointer);
    if(actor!=nullptr){
        HitResult* hit = new HitResult;
        hit->point = point;
        hit->actor = actor;
        hit->normal = normal;
        if(fixture->IsSensor()==true){
            hit->is_trigger = true;
        }else{
            hit->is_trigger = false;
        }
        hitResult[fraction] = hit;
        hasHit = true;
        return 1.0f;
    }
    return -1.0f;
}
