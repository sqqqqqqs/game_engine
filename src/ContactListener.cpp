//
//  ContactListener.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/21.
//

#include <stdio.h>
#include "ContactListener.h"

void Contactlistener::BeginContact(b2Contact *contact){
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);
    Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);
    
    
    if(actorA!=nullptr&&actorB!=nullptr){
        if(!fixtureA->IsSensor() && !fixtureB->IsSensor()){//two collider
            b2WorldManifold world_manifold;
            contact->GetWorldManifold(&world_manifold);
            Collision collision;
            collision.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
            collision.point = world_manifold.points[0];
            collision.normal = world_manifold.normal;
            //first for A
            collision.other = actorB;
            actorA->CollisionEnter(collision);
            //then for B
            collision.other = actorA;
            actorB->CollisionEnter(collision);
        }else if(fixtureA->IsSensor() && fixtureB->IsSensor()){//two trigger
            b2WorldManifold world_manifold;
            contact->GetWorldManifold(&world_manifold);
            Collision collision;
            collision.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
            collision.point = b2Vec2(-999.0f, -999.0f);
            collision.normal = b2Vec2(-999.0f, -999.0f);
            //first for A
            collision.other = actorB;
            actorA->TriggerEnter(collision);
            //then for B
            collision.other = actorA;
            actorB->TriggerEnter(collision);
        }
    }
    
}


void Contactlistener::EndContact(b2Contact *contact){
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    Actor* actorA = reinterpret_cast<Actor*>(fixtureA->GetUserData().pointer);
    Actor* actorB = reinterpret_cast<Actor*>(fixtureB->GetUserData().pointer);
    
    if(actorA!=nullptr&&actorB!=nullptr){
        b2WorldManifold world_manifold;
        contact->GetWorldManifold(&world_manifold);
        Collision collision;
        collision.relative_velocity = fixtureA->GetBody()->GetLinearVelocity() - fixtureB->GetBody()->GetLinearVelocity();
        collision.point = b2Vec2(-999.0f, -999.0f);
        collision.normal = b2Vec2(-999.0f, -999.0f);
        if(!fixtureA->IsSensor() && !fixtureB->IsSensor()){//two colliders
            //first for A
            collision.other = actorB;
            actorA->CollisionExit(collision);
            //then for B
            collision.other = actorA;
            actorB->CollisionExit(collision);
        }else if(fixtureA->IsSensor() && fixtureB->IsSensor()){//two triggers
            //first for A
            collision.other = actorB;
            actorA->TriggerExit(collision);
            //then for B
            collision.other = actorA;
            actorB->TriggerExit(collision);
        }
    }
}
