//
//  TextDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/2/7.
//

#ifndef TextDB_h
#define TextDB_h

#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <unordered_map>

class TextDB{
public:
    static void LoadFonts();
    static void LoadFontFile(const std::string& filePath, int fontSize);
    static TTF_Font* GetFontByNameAndSize(std::string fontName, int fontSize);
    static void DrawText(const std::string& text_content,int x, int y);
    static void AddDialogue(const std::string &text);
    static void ClearDialogue();
    static void RenderDialogue();
    
private:
    static inline std::vector<std::string> intro_texts;
    static inline size_t current_text_index = 0;
    static inline TTF_Font* font = nullptr;
    static inline std::vector<std::string> active_dialogues;
    static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> loadedFonts;
};

struct Text{
    std::string str_content;
    int screen_ordinate_x;
    int screen_ordinate_y;
    std::string font_name;
    int font_size;
    SDL_Color color;
};


#endif /* TextDB_h */
