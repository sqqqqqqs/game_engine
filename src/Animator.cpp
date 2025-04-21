//
//  Animator.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/4/10.
//

#include <stdio.h>
#include "Animator.h"
#include "Renderer.h"

void AnimatorComponent::OnStart() {
    frames.clear();
    for (int i = 1; i <= frame_count; ++i) {
        frames.push_back(sprite_prefix + std::to_string(i));
    }
    current_frame_index = 0;
    frame_counter = 0;
    if (!frames.empty()) {
        target_image_name = GetCurrentFrame();
    }
}

void AnimatorComponent::OnUpdate() {
    if (!enabled || frames.empty()) return;
    if (frames.size()==0) return;

    frame_counter++;
    if (frame_counter >= frame_duration) {
        frame_counter = 0;
        current_frame_index++;

        if (current_frame_index >= frames.size()) {
            if (loop) {
                current_frame_index = 0;
                loop_counter++;
                if (destroy_on_finish && loops_before_destroy > 0 && loop_counter >= loops_before_destroy && actor != nullptr) {
                    actor->RemoveComponent(actor->GetComponent("Animator"));
                    return;
                }
            }else current_frame_index = static_cast<int>(frames.size()) - 1;
            if (loops_before_destroy == 0 && destroy_on_finish && actor != nullptr) {
                actor->RemoveComponent(actor->GetComponent("Animator"));
                return;
            }
        }
    }

    target_image_name = GetCurrentFrame();
    luabridge::LuaRef rbRef = actor->GetComponent("Rigidbody");
    luabridge::LuaRef trRef = actor->GetComponent("Transform");
    
    if (rbRef.isNil() && trRef.isNil()) return;
    float x = 0.0f, y = 0.0f, rot = 0.0f;
    if (!rbRef.isNil()) {
        RigidbodyComponent* rb = rbRef.cast<RigidbodyComponent*>();
        if (!rb) return;
        b2Vec2 pos = rb->GetPosition();
        x = pos.x;
        y = pos.y;
        rot = rb->GetRotation();
    } else if (!trRef.isNil()) {
        if (trRef["x"].isNumber()) x = trRef["x"].cast<float>();
        if (trRef["y"].isNumber()) y = trRef["y"].cast<float>();
        if (trRef["rotation"].isNumber()) rot = trRef["rotation"].cast<float>();
    }
    Renderer::DrawEx(target_image_name, x, y, rot, 1.0f, 1.0f, 0.5f, 0.5f, 255, 255, 255, 255, 0);

}

std::string AnimatorComponent::GetCurrentFrame() {
    return frames[current_frame_index];
}


