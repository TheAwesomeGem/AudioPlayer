// Copyright (c) Topaz Centuallas 2022.

#include <filesystem>
#include "Playback.h"


bool Playback::Init()
{
    if (!player.Init())
    {
        return false;
    }

    // TODO: Move filesystem out of this. Bad practice!
    for (const auto& entry: std::filesystem::directory_iterator(std::filesystem::current_path()))
    {
        player.Load(entry.path().filename().string().c_str());
    }

    if (player.MediaCount() < 1)
    {
        return false;
    }

    return true;
}

void Playback::AddAll()
{
    std::vector<MediaId> medias = player.GetALlMedias();

    playlist.medias.insert(playlist.medias.begin(), medias.begin(), medias.end());

    playlist.currentIndex = 0;

    player.Play(playlist.medias[playlist.currentIndex]);
}

void Playback::Resume()
{
    player.Resume();
}

void Playback::Pause()
{
    player.Pause();
}

void Playback::Skip()
{
    ++playlist.currentIndex %= playlist.medias.size();

    // TODO: Should Playlist just be data?

    player.Play(playlist.medias[playlist.currentIndex]);
}
