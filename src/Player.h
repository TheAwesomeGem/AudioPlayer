// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include <memory>
#include <list>
#include "audio/AudioEngine.h"


/***
 * TODO: Send events back to playback such as media end so that playlist can progress.
 * TODO: Ensure mapping is synchronized after removing the audio
 * TODO: Provide currently playable media name in an unformatted fashion.
 * TODO: Add more player controls such as fast forward, volume, equalizer, etc.
 */
using MediaId = xg::Guid;

class Player
{
public:
    struct MediaInfo
    {
        std::string name;
    };

    bool Init();

    bool Load(const char* fileName);

    bool Play(MediaId mediaId);

    void Resume();

    void Pause();

    [[nodiscard]] float GetMediaLength() const
    {
        return audioEngine.GetAudioLength(currentMediaId);
    }

    [[nodiscard]] float GetMediaPosition() const
    {
        return audioEngine.GetAudioPosition(currentMediaId);
    }

    std::vector<MediaId> GetALlMedias()
    {
        std::vector<MediaId> medias;

        std::transform(mediaInfos.begin(), mediaInfos.end(), std::back_inserter(medias), [](auto&& element)
        {
            return element.first;
        });

        return medias;
    }

    [[nodiscard]] size_t MediaCount() const
    {
        return mediaInfos.size();
    }

    [[nodiscard]] UnbufferedChannel<AudioEvent>& GetEventChannel()
    {
        return audioEngine.GetEventChannel();
    }

private:
    AudioEngine audioEngine;
    MediaId currentMediaId;
    std::unordered_map<MediaId, MediaInfo> mediaInfos;
};
