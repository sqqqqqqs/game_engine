//
//  ParticleSystem.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/25.
//

#ifndef ParticleSystem_h
#define ParticleSystem_h
#include "Helper.h"
#include "Actor.h"
#include "ImageDB.h"
#include "Renderer.h"
#include <optional>

class Actor;

struct Particle {
    float starting_x_position;
    float starting_y_position;
    float starting_scale;
    float rotation;
    int start_frame = 0;
    bool is_active = true;
    float velocity_x = 0.0f;
    float velocity_y = 0.0f;
    float rotation_speed = 0.0f;
};

class ParticleSystem{
public:
    std::string type = "ParticleSystem";
    std::string key = "???";
    bool enabled = true;
    bool started = false;
    Actor* actor = nullptr;
    
    float x = 0.0f;
    float y = 0.0f;
    int frames_between_bursts = 1;
    int local_frame_number = 0;
    int burst_quantity = 1;
    float start_scale_min = 1.0f;
    float start_scale_max = 1.0f;
    float rotation_min = 0.0f;
    float rotation_max = 0.0f;
    
    int start_color_r = 255;
    int start_color_g = 255;
    int start_color_b = 255;
    int start_color_a = 255;
    
    std::string image = "";
    int sorting_order = 9999;
    
    int duration_frames = 300;
    std::queue<int> free_list;
    int number_of_particles = 0;
    
    float start_speed_min = 0.0f;
    float start_speed_max = 0.0f;
    
    float rotation_speed_min = 0.0f;
    float rotation_speed_max = 0.0f;
    
    float gravity_scale_x = 0.0f;
    float gravity_scale_y = 0.0f;
    
    float drag_factor = 1.0f;
    float angular_drag_factor = 1.0f;
    
    float end_scale = -999.9f;
    int end_color_r = -999;
    int end_color_g = -999;
    int end_color_b = -999;
    int end_color_a = -999;
    
    bool emission_allowed = true;
    
    float emit_angle_min = 0.0f;
    float emit_angle_max = 360.0f;
    float emit_radius_min = 0.0f;
    float emit_radius_max = 0.5f;
    
    std::vector<Particle> particles;
    
    void OnStart();
    void OnUpdate();
    void Stop();
    void Play();
    void Burst();
private:
    RandomEngine emit_angle_distribution;
    RandomEngine emit_radius_distribution;
    RandomEngine rotation_distribution;
    RandomEngine scale_distribution;
    RandomEngine speed_distribution;
    RandomEngine rotation_speed_distribution;
    void SpawnParticle(int burst_quantity);
    bool burst_called_from_update = false;
};

#endif /* ParticleSystem_h */
