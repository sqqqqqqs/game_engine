//
//  ParticleSystem.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/25.
//

#include <stdio.h>
#include "ParticleSystem.h"


void ParticleSystem::OnStart(){
    started = true;
    ImageDB::CreateDefaultParticleTextureWithName("default_particle");
    emit_angle_distribution.Configure(emit_angle_min, emit_angle_max, 298);
    emit_radius_distribution.Configure(emit_radius_min, emit_radius_max, 404);
    scale_distribution.Configure(start_scale_min, start_scale_max, 494);
    rotation_distribution.Configure(rotation_min, rotation_max, 440);
    speed_distribution.Configure(start_speed_min, start_speed_max, 498);
    rotation_speed_distribution.Configure(rotation_speed_min, rotation_speed_max, 305);
    
    if(frames_between_bursts < 1){
        frames_between_bursts = 1;
    }
    if(burst_quantity < 1){
        burst_quantity = 1;
    }
    if(start_color_r < 0 || start_color_r > 255){
        start_color_r = 255;
    }
    if(start_color_g < 0 || start_color_g > 255){
        start_color_g = 255;
    }
    if(start_color_b < 0 || start_color_b > 255){
        start_color_b = 255;
    }
    if(start_color_a < 0 || start_color_a > 255){
        start_color_a = 255;
    }
    if(duration_frames < 1){
        duration_frames = 1;
    }
}

void ParticleSystem::OnUpdate(){
    burst_called_from_update = true;
    Burst();
    for(int i = 0; i < particles.size(); i++){
        Particle& p = particles[i];
        if (!p.is_active) continue;
        int frames_alive = local_frame_number - p.start_frame;
        if (frames_alive >= duration_frames) {
            p.is_active = false;
            free_list.push(i);
            number_of_particles--;
            continue;
        }
        
        p.velocity_x += gravity_scale_x;
        p.velocity_y += gravity_scale_y;
        
        p.velocity_x *= drag_factor;
        p.velocity_y *= drag_factor;
        
        p.rotation_speed = p.rotation_speed * angular_drag_factor;
        
        p.starting_x_position += p.velocity_x;
        p.starting_y_position += p.velocity_y;
        p.rotation += p.rotation_speed;
        
        float life_progress = static_cast<float>(frames_alive) / duration_frames;
        int color_r = start_color_r;
        int color_g = start_color_g;
        int color_b = start_color_b;
        int color_a = start_color_a;
        float scale_draw = p.starting_scale;
        if(end_scale!=-999.9f){
            scale_draw = glm::mix(p.starting_scale, end_scale, life_progress);
        }
        if(end_color_a >=0 && end_color_a <= 255){
            color_a = glm::mix(start_color_a,end_color_a,life_progress);
        }
        if(end_color_g >=0 && end_color_g <= 255){
            color_g = glm::mix(start_color_g,end_color_g,life_progress);
        }
        if(end_color_b >=0 && end_color_b <= 255){
            color_b = glm::mix(start_color_b,end_color_b,life_progress);
        }
        if(end_color_r >=0 && end_color_r <= 255){
            color_r = glm::mix(start_color_r,end_color_r,life_progress);
        }
        if(image==""){
            Renderer::DrawEx("default_particle", p.starting_x_position, p.starting_y_position, p.rotation, scale_draw, scale_draw, 0.5f, 0.5f, color_r, color_g, color_b, color_a, sorting_order);
        }else{
            Renderer::DrawEx(image, p.starting_x_position, p.starting_y_position, p.rotation, scale_draw, scale_draw, 0.5f, 0.5f, color_r, color_g, color_b, color_a, sorting_order);
        }
    }
    burst_called_from_update = false;
    local_frame_number++;
}


void ParticleSystem::Burst(){
    if((local_frame_number % frames_between_bursts == 0 && emission_allowed ==true)||!burst_called_from_update){
        SpawnParticle(burst_quantity);
    }
}


void ParticleSystem::SpawnParticle(int burst_quantity) {
    int time_remaining = burst_quantity;
    while(time_remaining > 0){
        Particle new_particle;
        float angle_deg = emit_angle_distribution.Sample();
        float radius = emit_radius_distribution.Sample();
        float start_scale = scale_distribution.Sample();
        float rotation = rotation_distribution.Sample();
        float speed = speed_distribution.Sample();
        
        float angle_rad = glm::radians(angle_deg);
        float cos_angle = glm::cos(angle_rad);
        float sin_angle = glm::sin(angle_rad);
        
        float starting_x_position = x + cos_angle * radius;
        float starting_y_position = y + sin_angle * radius;
        
        new_particle.starting_x_position = starting_x_position;
        new_particle.starting_y_position = starting_y_position;
        new_particle.starting_scale = start_scale;
        new_particle.rotation = rotation;
        new_particle.start_frame = local_frame_number;
        new_particle.velocity_x = speed * cos_angle;
        new_particle.velocity_y = speed * sin_angle;
        new_particle.rotation_speed = rotation_speed_distribution.Sample();
        
        
        if (!free_list.empty()) {
            int slot = free_list.front();
            free_list.pop();
            particles[slot] = new_particle;
        } else {
            particles.push_back(new_particle);
        }
        
        number_of_particles++;
        time_remaining--;
    }
}

void ParticleSystem::Stop(){
    emission_allowed = false;
}

void ParticleSystem::Play(){
    emission_allowed = true;
}
