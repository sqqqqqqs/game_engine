//
//  AudioDB.cpp
//  game_engine
//
//  Created by 李一锴 on 2025/2/7.
//

#include <stdio.h>
#include "AudioDB.h"
#include "EngineCheck.h"



void AudioDB::LoadAudio() {
    const std::string audioDirectory = "resources/audio";
    if (!std::filesystem::exists(audioDirectory)) {
        return;
    }
    AudioHelper::Mix_OpenAudio(48000, AUDIO_S16SYS, 1, 1024);
    for (const auto& audioFile : std::filesystem::directory_iterator(audioDirectory)) {
        LoadAudioFile(audioFile.path().string());
    }
    AudioHelper::Mix_AllocateChannels(50);
}

void AudioDB::LoadAudioFile(const std::string &filePath){
    std::string audioName = std::filesystem::path(filePath).stem().stem().string();
    Mix_Chunk* sound = AudioHelper::Mix_LoadWAV(filePath.c_str());
    if (!sound) {
        return;
    }
    loaded_audios[audioName] = sound;
}

void AudioDB::Play(int channel, std::string clip_name, bool does_loop){
    if(loaded_audios.find(clip_name)!= loaded_audios.end()){
        int Loops = does_loop ? -1 : 0;
        AudioHelper::Mix_PlayChannel(channel, loaded_audios[clip_name], Loops);
        return;
    }
    std::cout << "error: failed to play audio clip " << clip_name;
    std::exit(0);
}

void AudioDB::Halt(int channel){
    AudioHelper::Mix_HaltChannel(channel);
}

void AudioDB::SetVolume(int channel, float volume){
    AudioHelper::Mix_Volume(channel, static_cast<int>(volume));
}







