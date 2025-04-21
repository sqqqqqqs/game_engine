//
//  TemplateDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/1/30.
//

#ifndef TemplateDB_h
#define TemplateDB_h

#include <string>
#include "Actor.h"
#include <unordered_map>
#include "rapidjson/document.h"
#include <optional>
#include "SceneDB.h"

class TemplateDB{
public:
    static void LoadTemplates();
    static bool hasTemplate(const std::string& template_name);
    static const Actor GetTemplate(const std::string& name);
    
private:
    static std::unordered_map<size_t, Actor> templates;
};



#endif /* TemplateDB_h */
