//
//  AudioDB.h
//  game_engine
//
//  Created by 李一锴 on 2025/2/7.
//

#ifndef AudioDB_h
#define AudioDB_h

#include <string>
#include "SDL2_mixer/SDL_mixer.h"
#include "AudioHelper.h"

class AudioDB{
public:
    static void InitSFX();
    static void LoadAudio();
    static void LoadAudioFile(const std::string& filePath);
    
    static void LoadSFX(const std::string& name);
    
    static void Play(int channel, std::string clip_name, bool does_loop);
    static void Halt(int channel);
    static void SetVolume(int channel, float volume);

    
private:
    static inline std::unordered_map<std::string, Mix_Chunk*> loaded_audios;
};




#endif /* AudioDB_h */
