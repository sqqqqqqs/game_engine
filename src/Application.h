//
//  Application.h
//  game_engine
//
//  Created by 李一锴 on 2025/3/13.
//

#ifndef Application_h
#define Application_h
#include "Helper.h"

class Application{
public:
    static void Quit();
    static void Sleep(int dur_ms);
    static int GetFrame();
    static void OpenURL(const std::string& url);
    
};

#endif /* Application_h */
