// Copyright (c) Topaz Centuallas 2022.

#include "AudioEngine.h"


AudioEngine::~AudioEngine()
{
    if (engine)
    {
        ma_engine_uninit(engine.get());
    }

    for (auto [id, sound]: audios)
    {
        ma_sound_uninit(sound);
        delete sound;
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

    return true;
}

size_t AudioEngine::Add(xg::Guid id, const char* fileName)
{
    ma_sound* sound = new ma_sound;
    ma_result result;
    result = ma_sound_init_from_file(engine.get(), fileName, 0, nullptr, nullptr, sound);

    if (result != MA_SUCCESS)
    {
        return -1;
    }

    audios[id] = sound;

    return 0;
}

void AudioEngine::Remove(xg::Guid id)
{
    ma_sound_uninit(audios[id]);
    delete audios[id];
    audios.erase(id);
}

void AudioEngine::Start(xg::Guid id)
{
    ma_sound_start(audios[id]);
}

void AudioEngine::Stop(xg::Guid id)
{
    ma_sound_stop(audios[id]);
}
