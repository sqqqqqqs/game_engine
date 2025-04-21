//
//  NetworkComponent.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/4/13.
//

#include <stdio.h>
#include "NetworkComponent.h"
#include "SceneDB.h"
#include <iostream>
#include <sstream>

using asio::ip::tcp;

void NetworkComponent::OnStart() {
    try {
        if (is_server) {
            std::cout << "[Server] Preparing to accept on port " << port << "..." << std::endl;
            acceptor = std::make_unique<tcp::acceptor>(io, tcp::endpoint(tcp::v4(), port));
            acceptor->non_blocking(true);
            socket = std::make_unique<tcp::socket>(io);
        } else {
            std::cout << "[Client] Socket prepared (waiting for connection to become available)..." << std::endl;
            socket = std::make_unique<tcp::socket>(io);
        }
    } catch (const std::exception& e) {
        std::cout << "[NetworkComponent::OnStart] Error: " << e.what() << std::endl;
    }
}

void NetworkComponent::OnUpdate() {
    if (!connected && is_server && acceptor && socket) {
        try {
            acceptor->accept(*socket);  //
            socket->non_blocking(true);
            connected = true;
            std::cout << "[Server] Client connected!" << std::endl;
        } catch (const asio::system_error& e) {
            if (e.code() == asio::error::would_block) {
                std::cout << "[Server] Still waiting for client to connect..." << std::endl;
                return;
            } else {
                std::cout << "[Server] Accept error: " << e.what() << std::endl;
                return;
            }
        }
    }
    if (!connected && !is_server) {
        if (connect_retry_cooldown > 0) {
            connect_retry_cooldown--;
            return;
        }
        try {
            if (!socket || !socket->is_open()) {
                socket = std::make_unique<tcp::socket>(io);
            }
            
            tcp::endpoint endpoint(asio::ip::make_address(server_ip), port);
            socket->connect(endpoint);
            socket->non_blocking(true);
            connected = true;
            std::cout << "[Client] Connected to server at " << server_ip << ":" << port << std::endl;
        } catch (const asio::system_error& e) {
            std::cout << "[Client] Connect error: " << e.what() << " (retrying...)" << std::endl;
            connect_retry_cooldown = connect_retry_interval;
            socket.reset();
            return;
        }
    }
    
    if (!connected || !socket || !socket->is_open()) return;
    
    try {
        if (send_mode) {
            auto t = actor->GetComponent("Transform");
            float x = t["x"].cast<float>();
            float y = t["y"].cast<float>();

            std::ostringstream ss;
            ss << x << "," << y;
            
            if (Input::GetKeyDown("space")) {
                ss << ",FIRE_" << actor->name;
            }
            
            std::string msg = ss.str();
            socket->write_some(asio::buffer(msg));
            std::cout << "[SEND] " << actor->name << " → " << msg << std::endl;
            }
        else {
            char buffer[64] = {0};
            asio::error_code ec;
            socket->read_some(asio::buffer(buffer), ec);
            
            if (ec == asio::error::would_block || ec == asio::error::try_again) {
                return;
            } else if (ec) {
                std::cout << "[RECV] Read error: " << ec.message() << std::endl;
                return;
            }
            
            float x = 0.0f, y = 0.0f;
            char action[32] = "";
            int parsed = sscanf(buffer, "%f,%f,%31s", &x, &y, action);
            auto targetActor = SceneDB::FindActor(target_actor_key);
            if (targetActor) {
                auto t = targetActor->GetComponent("Transform");
                t["x"] = x;
                t["y"] = y;
                std::cout << "[RECV] Updated " << target_actor_key << " to (" << x << ", " << y << ")" << std::endl;
            }
            if (parsed == 3 && std::string(action).rfind("FIRE_", 0) == 0) {
                std::string who = std::string(action).substr(5);
                if (who == "PlayerA") {
                    std::cout << "[RECV] FIRE from PlayerA, spawn ProjectileA\n";
                    auto bullet = SceneDB::AddActorWithTemplate("ProjectileA");
                    auto t = bullet->GetComponent("Transform");
                    t["x"] = x;
                    t["y"] = y;
                }else if (who == "PlayerB") {
                    std::cout << "[RECV] FIRE from PlayerB, spawn ProjectileB\n";
                    auto bullet = SceneDB::AddActorWithTemplate("ProjectileB");
                    auto t = bullet->GetComponent("Transform");
                    t["x"] = x;
                    t["y"] = y;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cout << "[NetworkComponent::OnUpdate] Error: " << e.what() << std::endl;
    }
}
