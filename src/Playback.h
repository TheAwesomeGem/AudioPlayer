// Copyright (c) Topaz Centuallas 2022.

#pragma once

#include "Player.h"


using PlaylistId = xg::Guid;

class Playback
{
public:
    struct Playlist
    {
        // TODO: Add naming for playlist
        std::vector<MediaId> medias;
        size_t currentIndex;
    };

    bool Init();

    void AddAll(); // TODO: Debug method to add all medias to the playlist

    void Resume();

    void Pause();

    void Skip();

private:
    Player player;
    Playlist playlist; // TODO: We want many playlists using PlaylistId
};
