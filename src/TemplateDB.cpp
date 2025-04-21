//
//  TemplateDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/1/30.
//
#include "TemplateDB.h"
#include "EngineUtils.h"
#include <filesystem>
#include <iostream>

std::unordered_map<std::size_t, Actor> TemplateDB::templates;

void TemplateDB::LoadTemplates(){
    std::string tempPath = "resources/actor_templates/";
    
    if (!std::filesystem::exists(tempPath)) {
        return;
    }
    for(const auto& entry:std::filesystem::directory_iterator(tempPath)){
        if(entry.path().extension()==".template"){
            rapidjson::Document TempData;
            EngineUtils::ReadJsonFile(entry.path().string(), TempData);
            std::string templateName = entry.path().stem().string();
            size_t template_hash = std::hash<std::string>{}(templateName);
            
            Actor templateActor;
            templateActor.name = TempData.HasMember("name") ? TempData["name"].GetString() : "";
            if (TempData.HasMember("components")) {
                for (const auto& componentEntry : TempData["components"].GetObject()) {
                    std::string componentKey = componentEntry.name.GetString();
                    SceneDB::AddComponentToActor(templateActor, componentKey, componentEntry.value);
                }
            }
            templates[template_hash] = templateActor;
        }
    }
    
}

bool TemplateDB::hasTemplate(const std::string &template_name){
    size_t hash = std::hash<std::string>{}(template_name);
    return templates.find(hash)!=templates.end();
}

const Actor TemplateDB::GetTemplate(const std::string& template_name) {
    size_t hash = std::hash<std::string>{}(template_name);
    return templates.at(hash);
}
