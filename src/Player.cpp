// Copyright (c) Topaz Centuallas 2022.

#include "Player.h"


bool Player::Init()
{
    return audioEngine.Init();
}

bool Player::Load(const char* fileName)
{
    MediaId id = xg::newGuid();
    audioEngine.Add(id, fileName);
    mediaInfos[id] = MediaInfo{fileName};

    if (!currentMediaId.isValid())
    {
        currentMediaId = id;
    }

    return true;
}

bool Player::Play(MediaId mediaId)
{
    // TODO: Maybe seek back?

    Pause();
    currentMediaId = mediaId;
    Resume();

    return true;
}

void Player::Resume()
{
    audioEngine.Start(currentMediaId);
}

void Player::Pause()
{
    audioEngine.Stop(currentMediaId);
}
