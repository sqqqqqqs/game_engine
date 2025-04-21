//
//  TextDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/2/7.
//

#include <stdio.h>
#include "TextDB.h"
#include "EngineCheck.h"
#include "Renderer.h"
#include <iostream>



void TextDB::LoadFonts(){
    TTF_Init();
    std::string fontsDirectory  = "resources/fonts";
    if (!std::filesystem::exists(fontsDirectory)) {
        return;
    }
    for (const auto& fontFile : std::filesystem::directory_iterator(fontsDirectory)) {
        if (fontFile.path().filename() == ".DS_Store") continue;
        LoadFontFile(fontFile.path().string(), 16);
    }
}

void TextDB::LoadFontFile(const std::string &filePath,int fontSize){
    std::string fontName = std::filesystem::path(filePath).stem().stem().string();
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (!font) {
        std::cout << "[Error] Failed to load font: " << filePath;
        std::exit(0);
    }
    loadedFonts[fontName][fontSize] = font;
}

TTF_Font* TextDB::GetFontByNameAndSize(std::string fontName, int fontSize){
    if(loadedFonts.find(fontName)!=loadedFonts.end()){
        if (loadedFonts[fontName].find(fontSize) != loadedFonts[fontName].end()){
            return loadedFonts[fontName][fontSize];
        }
        std::string fontPath = "resources/fonts/" + fontName + ".ttf";
        TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
        loadedFonts[fontName][fontSize] = font;
        return font;
    }
    std::cout << "error: font " << fontName << " missing";
    exit(0);
}

void TextDB::DrawText(const std::string &text_content,int x,int y){
    if (!font) {
        std::cout << "Error: Font not loaded!" << std::endl;
        return;
    }
    
    
    //the color is determined for the time being
    SDL_Color textColor = {255, 255, 255, 255};
    
    
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text_content.c_str(), textColor);
    if(!textSurface)return;
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Renderer::main_renderer, textSurface);
    if (!textTexture) return;
    SDL_FRect renderQuad;
    renderQuad.x = x;
    renderQuad.y = y;
    renderQuad.w = textSurface->w;
    renderQuad.h = textSurface->h;
    SDL_FreeSurface(textSurface);
    
    Helper::SDL_RenderCopy(Renderer::main_renderer, textTexture, nullptr, &renderQuad);
    SDL_DestroyTexture(textTexture);
    
    
}

void TextDB::AddDialogue(const std::string &text) {
    active_dialogues.push_back(text);
}

void TextDB::ClearDialogue() {
    active_dialogues.clear();
}

void TextDB::RenderDialogue() {
    size_t m = active_dialogues.size();
    for (size_t i = 0; i < m; ++i) {
        int y_pos = Renderer::window_height - 50 - static_cast<int>((m - 1 - i) * 50);
        DrawText(active_dialogues[i], 25, y_pos);
    }
}
