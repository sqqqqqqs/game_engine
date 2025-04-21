//
//  NetworkComponent.h
//  game_engine
//
//  Created by 李一锴 on 2025/4/13.
//

#ifndef NetworkComponent_h
#define NetworkComponent_h

#define ASIO_STANDALONE
#include "asio/asio.hpp"
#include "Application.h"
#include <memory>
#include <string>
#include "Actor.h"

class NetworkComponent {
public:
    std::string type = "Network";
    std::string key = "???";
    bool enabled = true;
    bool started = false;
    Actor* actor = nullptr;

    bool is_server = false;                      // if this is the server
    std::string target_actor_key = "";           // target actor
    int port = 12345;                            // supervising/connecting port
    std::string server_ip = "127.0.0.1";         // client connecting ip
    bool send_mode = true;

    void OnStart();
    void OnUpdate();
    NetworkComponent() = default;
    NetworkComponent(const NetworkComponent&) = delete;
    NetworkComponent& operator=(const NetworkComponent&) = delete;
private:
    asio::io_context io;
    std::unique_ptr<asio::ip::tcp::acceptor> acceptor;
    std::unique_ptr<asio::ip::tcp::socket> socket;
    bool connected = false;
    bool has_attempted_connect = false;
    bool waiting_for_accept = false;
    bool accept_attempted = false;
    int connect_retry_cooldown = 0;
    const int connect_retry_interval = 60;
};

#endif /* NetworkComponent_h */
