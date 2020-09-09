#pragma once
#include <BWAPI.h>
#include <vector>
#include <windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <fstream>
#include "Timer.h"
#include "AutoObserver.h"

class TMState
{
public:

    std::vector<int> timeOutExceeded;
    const std::string stateFileVer = "3.0.1";
    std::string selfName;
    std::string enemyName;
    std::string selfRaceInit;
    std::string enemyRaceInit;
    std::string selfRace;
    std::string enemyRace;
    std::string mapName;
    int exceededTimeoutsLimit = 0;
    int frameCountTimeoutsExceeded = 0;
    int frameCount = 0;
    int exceededFrameCountLimit = 0;
    int frameCountFrameCountLimitExceeded = 0;
    int selfKillScore = 0;
    int selfBuildingScore = 0;
    int selfRazingScore = 0;
    int selfUnitScore = 0;
    int selfCustomScore = 0;
    int selfGatheredMinerals = 0;
    int selfSpentMinerals = 0;
    int selfRepairedMinerals = 0;
    int selfRefundedMinerals = 0;
    int selfGatheredGas = 0;
    int selfSpentGas = 0;
    int selfRepairedGas = 0;
    int selfRefundedGas = 0;
    int selfSupplyUsed = 0;
    int selfSupplyTotal = 0;
    int selfScore = 0;
    int enemyScore = 0;
    int gameElapsedTime = 0;
    int exceededElapsedTimeLimit = 0;
    int frameCountElapsedTimeExceeded = 0;
    int defeated = 0;
    int victorious = 0;
    int gameOver = 0;
    int isWinnerInOnEnd = 0;
    int totFrameTime = 0;
    int avgFrameTime = 0;
    int maxFrameTime = 0;
    int minFrameTime = 999999;
    unsigned int randomSeed = 0;
    bool initialized = false;

    void init();
    void update(std::vector<int> & times, int frameCountTimeoutsExceededNew, int gameFrameLimit, int gameElapsedTimeLimit, int gameElapsedTimeNew, int totFrameTimeNew, int avgFrameTimeNew, int maxFrameTimeNew, int minFrameTimeNew);
    void ended(int isWinnerInOnEnd);
    bool write(std::string filename);
};

class TM : public BWAPI::TournamentModule
{
public:

    virtual bool onAction(BWAPI::Tournament::ActionID actionType, void *parameter = nullptr) override;
    virtual void onFirstAdvertisement() override;
};

class TMAI : public BWAPI::AIModule
{
    Timer gameTimer;
    AutoObserver autoObserver;
    TMState state;
    std::vector<std::string> diagnostics;
    std::vector<int> frameTimes;
    std::vector<int> timerLimits;
    std::vector<int> timerLimitsBound;
    std::vector<int> timerLimitsExceeded;
    std::string settingsFileName = "bwapi-data\\tm_settings.ini";
    std::string stateFileName = "gameState.txt";
    std::string moduleFileName;
    int gameFrameLimit = 85714;
    int gameElapsedTimeLimit = 0;
    int frameThreshTimed = 11;
    int stateFileUpdateTimePeriod = 5000;
    int stateFileUpdateFramePeriod = 0;
    int totFrameTime = 0;
    int avgFrameTime = 0;
    int maxFrameTime = 0;
    int minFrameTime = 999999;
    int localSpeed = 0;
    int frameSkip = 0;
    int frameCountTimeoutsExceeded = 0;
    int oldFrameCount = -1;
    int numPrevEventsThisFrame = 0;
    bool gameOver = false;
    bool disableUserInput = true;
    bool disableUserInputLocalSpeed = true;
    bool drawUnitInfo = true;
    bool drawTournamentInfo = true;
    bool drawBotNames = true;
    bool disableDrawGameTimer = true;
    bool drawGUI = true;
    bool autoObs = true;
    bool autoResumeGame = true;
    bool eventTimesVaried = false;

public:

    TMAI();
    virtual ~TMAI() noexcept;
    virtual void updateFrameTimers();
    virtual void drawUnitInformation(int x, int y);
    virtual void drawTMSettings(int x, int y);
    virtual void drawGameTimer(int x, int y);
    // Assumes no unicode, ASCII only.
    virtual std::string envVar(const char *var);
    virtual bool& parseField(std::istringstream& iss, bool& field);
    virtual std::string& parseField(std::istringstream& iss, std::string& field);
    template<typename T> T& parseField(std::istringstream& iss, T& field);
    // Returns diagnostic message(s), if any.
    virtual std::vector<std::string> parseOptions();

    virtual void onStart() override;
    virtual void onEnd(bool isWinner) override;
    virtual void onFrame() override;
    virtual void onSendText(std::string text) override;
    virtual void onReceiveText(BWAPI::Player player, std::string text) override;
    virtual void onPlayerLeft(BWAPI::Player player) override;
    virtual void onNukeDetect(BWAPI::Position target) override;
    virtual void onUnitDiscover(BWAPI::Unit unit) override;
    virtual void onUnitEvade(BWAPI::Unit unit) override;
    virtual void onUnitShow(BWAPI::Unit unit) override;
    virtual void onUnitHide(BWAPI::Unit unit) override;
    virtual void onUnitCreate(BWAPI::Unit unit) override;
    virtual void onUnitDestroy(BWAPI::Unit unit) override;
    virtual void onUnitMorph(BWAPI::Unit unit) override;
    virtual void onUnitComplete(BWAPI::Unit unit) override;
    virtual void onUnitRenegade(BWAPI::Unit unit) override;
    virtual void onSaveGame(std::string gameName) override;
};
