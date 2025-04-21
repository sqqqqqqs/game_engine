//
//  ContactListener.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/21.
//

#ifndef ContactListener_h
#define ContactListener_h

#include "box2d/box2d.h"
#include "Actor.h"

class Actor;
class Contactlistener:public b2ContactListener
{
public:
    void BeginContact(b2Contact* contact) override;
    void EndContact(b2Contact* contact) override;
};

class Collision{
public:
    Actor* other;
    b2Vec2 point;
    b2Vec2 relative_velocity;
    b2Vec2 normal;
};


#endif /* ContactListener_h */
