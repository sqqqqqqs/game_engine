//
//  EngineCheck.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/1/29.
//
#include "EngineCheck.h"
#include "Renderer.h"
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include "EngineUtils.h"
#include "TemplateDB.h"

rapidjson::Document EngineCheck::configData;
rapidjson::Document EngineCheck::CameraConfigData;
std::string EngineCheck::initial_scene;

void EngineCheck::CheckResources(){
    if (!std::filesystem::exists("resources")) {
        std::cout << "error: resources/ missing";
        std::exit(0);
    }
    if (!std::filesystem::exists("resources/game.config")) {
        std::cout << "error: resources/game.config missing";
        std::exit(0);
    }
    TemplateDB::LoadTemplates();
}

void EngineCheck::LoadGameConfig(const std::string& configPath) {
    EngineUtils::ReadJsonFile(configPath, configData);
    
}

void EngineCheck::LoadRenderingConfig(const std::string& configPath){
    if(!std::filesystem::exists(configPath)){
        return;
    }
    EngineUtils::ReadJsonFile(configPath, CameraConfigData);
    
    int x_res = CameraConfigData.HasMember("x_resolution") ? CameraConfigData["x_resolution"].GetInt() : 640;
    int y_res = CameraConfigData.HasMember("y_resolution") ? CameraConfigData["y_resolution"].GetInt() : 360;
    int color_r =CameraConfigData.HasMember("clear_color_r") ? CameraConfigData["clear_color_r"].GetInt() : 255;
    int color_g =CameraConfigData.HasMember("clear_color_g") ? CameraConfigData["clear_color_g"].GetInt() : 255;
    int color_b =CameraConfigData.HasMember("clear_color_b") ? CameraConfigData["clear_color_b"].GetInt() : 255;
    float cam_offset_x =CameraConfigData.HasMember("cam_offset_x") ? CameraConfigData["cam_offset_x"].GetFloat() : 0.0f;
    float cam_offset_y =CameraConfigData.HasMember("cam_offset_y") ? CameraConfigData["cam_offset_y"].GetFloat() : 0.0f;
    float zoom_factor = CameraConfigData.HasMember("zoom_factor") ? CameraConfigData["zoom_factor"].GetFloat() : 1.0f;
    float cam_ease_factor = CameraConfigData.HasMember("cam_ease_factor") ? CameraConfigData["cam_ease_factor"].GetFloat() : 1.0f;

    Renderer::InitializaWindow(x_res, y_res);
    Renderer::InitializClearColor(color_r, color_g, color_b);
    Renderer::InitializeCameraOffset(cam_offset_x, cam_offset_y);
    Renderer::InitializeZoomFactor(zoom_factor);
    Renderer::InitializeCamEaseFactor(cam_ease_factor);
}

int EngineCheck::GetCameraWidth() {
    return Renderer::window_width;
}

int EngineCheck::GetCameraHeight() {
    return Renderer::window_height;
}

std::string EngineCheck::GetGameStartMessage() {
    return configData.HasMember("game_start_message") ? configData["game_start_message"].GetString() : "";
}

std::string EngineCheck::GetGameOverBadMessage() {
    return configData.HasMember("game_over_bad_message") ? configData["game_over_bad_message"].GetString() : "";
}

std::string EngineCheck::GetGameOverGoodMessage() {
    return configData.HasMember("game_over_good_message") ? configData["game_over_good_message"].GetString() : "";
}

std::string EngineCheck::GetInitialScene(){
    if(!configData.HasMember("initial_scene")){
        return initial_scene;
//        std::cout<<"error: initial_scene unspecified";
//        std::exit(0);
    }
    return configData["initial_scene"].GetString();
}

void EngineCheck::SetInitialScene(std::string name){
    if (name == "server") {
        initial_scene = "server";
    } else {
        initial_scene = "client";
    }
}


std::string EngineCheck::GetGameTitle() {
    return configData.HasMember("game_title") ? configData["game_title"].GetString() : "";
}


std::string EngineCheck::GetFontName() {
    if (configData.HasMember("font") && configData["font"].IsString()) {
        return configData["font"].GetString();  
    }
    return "";
}



