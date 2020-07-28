#include <BWAPI.h>

#include "TournamentModule.h"

extern "C" __declspec(dllexport) void gameInit(BWAPI::Game* game)
{
    BWAPI::BroodwarPtr = game;
}

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}

extern "C" __declspec(dllexport) BWAPI::AIModule* newTournamentAI()
{
    return new TMAI();
}

extern "C" __declspec(dllexport) BWAPI::TournamentModule* newTournamentModule()
{
    return new TM();
}
