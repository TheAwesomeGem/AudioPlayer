// Copyright (c) Topaz Centuallas 2022.

#include <thread>
#include "AudioEngine.h"


AudioEngine::~AudioEngine()
{
    if (engine)
    {
        ma_engine_uninit(engine.get());
    }

    for (auto& [id, sound]: audios)
    {
        ma_sound_uninit(sound.get());
        sound.reset(nullptr);
    }

    isDestroyed.store(true);

    if (backgroundThread.joinable())
    {
        backgroundThread.join();
    }
}

bool AudioEngine::Init()
{
    if (engine)
    {
        return false;
    }

    engine = std::make_unique<ma_engine>();

    ma_result result;
    result = ma_engine_init(nullptr, engine.get());

    if (result != MA_SUCCESS)
    {
        engine.reset(nullptr);

        return false;
    }

    std::thread thread{&AudioEngine::UpdateSounds, this };

    backgroundThread = std::move(thread);

    return true;
}

size_t AudioEngine::Add(xg::Guid id, const char* fileName)
{
    auto sound = std::make_unique<ma_sound>();
    ma_result result;
    result = ma_sound_init_from_file(engine.get(), fileName, 0, nullptr, nullptr, sound.get());

    if (result != MA_SUCCESS)
    {
        return -1;
    }

    audios[id] = std::move(sound);

    return 0;
}

void AudioEngine::Remove(xg::Guid id)
{
    ma_sound_uninit(audios[id].get());
    audios[id].reset(nullptr);
    audios.erase(id);
}

void AudioEngine::Start(xg::Guid id)
{
    ma_sound_start(audios[id].get());
}

void AudioEngine::Stop(xg::Guid id)
{
    ma_sound_stop(audios[id].get());
}

void AudioEngine::UpdateSounds()
{
    while(!isDestroyed)
    {
        // TODO: Fix data races by creating a thread safe unordered_map.
        for(auto& [key, value] : audios)
        {
            if(ma_sound_at_end(value.get()))
            {
                eventChannel.send(AudioEvent{key, AudioEventType::Finish});
            }
        }
    }
}
