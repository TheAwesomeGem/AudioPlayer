// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include <memory>
#include <unordered_map>
#include <crossguid/guid.hpp>
#include "../thirdparty/miniaudio.h"
#include "../Channel.h"


enum class AudioEventType
{
    Finish
};

struct AudioEvent
{
    xg::Guid id;
    AudioEventType type;
};

class AudioEngine
{
public:
    ~AudioEngine();

    bool Init();

    size_t Add(xg::Guid id, const char* fileName);

    void Remove(xg::Guid id);

    void Start(xg::Guid id);

    void Stop(xg::Guid id);

    void UpdateSounds();

    [[nodiscard]] float GetAudioLength(xg::Guid id) const
    {
        float length = 0.0F;

        ma_sound_get_length_in_seconds(const_cast<ma_sound*>(audios.at(id).get()), &length);

        return length;
    }

    [[nodiscard]] float GetAudioPosition(xg::Guid id) const
    {
        float position = 0.0F;

        ma_sound_get_cursor_in_seconds(const_cast<ma_sound*>(audios.at(id).get()), &position);

        return position;
    }

    [[nodiscard]] bool IsPlaying(xg::Guid id) const
    {
        return ma_sound_is_playing(audios.at(id).get());
    }

    [[nodiscard]] size_t Count() const
    {
        return audios.size();
    }

    [[nodiscard]] UnbufferedChannel<AudioEvent>& GetEventChannel()
    {
        return eventChannel;
    }

private:
    std::unique_ptr<ma_engine> engine;
    std::unordered_map<xg::Guid, std::unique_ptr<ma_sound>> audios;
    UnbufferedChannel<AudioEvent> eventChannel;
    std::thread backgroundThread;
    std::atomic_bool isDestroyed = false;
};
