//
//  Application.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/3/13.
//

#include <stdio.h>
#include "Application.h"
#include <thread>
#include <chrono>


void Application::Quit(){
    exit(0);
}

void Application::Sleep(int dur_ms){
    std::this_thread::sleep_for(std::chrono::milliseconds(dur_ms));
}

int Application::GetFrame(){
    return Helper::GetFrameNumber();
}

void Application::OpenURL(const std::string &url){
#ifdef _WIN32
    std::string command = "start " + url;
#endif
#ifdef __APPLE__
    std::string command = "open " + url;
#endif
#ifdef __linux__
    std::string command = "xdg-open " + url;
#endif
    system(command.c_str());

}
