//
//  Animator.h
//  game_engine
//
//  Created by 李一锴 on 2025/4/10.
//

#ifndef Animator_h
#define Animator_h

#include <vector>
#include <string>
#include "Actor.h"

class AnimatorComponent {
public:
    std::string type = "Animator";
    std::string key = "???";
    bool enabled = true;
    bool started = false;
    Actor* actor = nullptr;

    std::vector<std::string> frames;
    std::string sprite_prefix = "";
    int frame_count = 1;
    int frame_duration = 10;
    bool loop = true;

    int current_frame_index = 0;
    int frame_counter = 0;

    std::string target_image_name = "";
    bool destroy_on_finish = false;
    int loops_before_destroy = 0;

    void OnUpdate();
    void OnStart();

    std::string GetCurrentFrame();
private:
    int loop_counter = 0;
};


#endif /* Animator_h */
