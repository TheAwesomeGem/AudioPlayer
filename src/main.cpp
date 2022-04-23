#include <iostream>
#include <string>
#include "Playback.h"


void ToLower(std::string& src)
{
    std::transform(src.begin(), src.end(), src.begin(), [](char ch)
    {
        return std::tolower(ch);
    });
}

// TODO: Add a nice UI
int main()
{
    std::string cmd;
    Playback playback;

    if (!playback.Init())
    {
        printf("Unable to initialize the Playback.\n");
    }

    do
    {
        printf("Type your command: \n");
        std::cin >> cmd;
        ToLower(cmd);

        if (cmd == "addall")
        {
            playback.AddAll();
        }
        else if (cmd == "resume")
        {
            playback.Resume();
        }
        else if (cmd == "pause")
        {
            playback.Pause();
        }
        else if (cmd == "skip")
        {
            playback.Skip();
        }
        else if (cmd == "debug")
        {
            // printf("%f/%f\n", player.GetMediaPosition(), player.GetMediaLength());
        }
    }
    while (cmd != "exit");

    return 0;
}
