#pragma once
#include <BWAPI.h>
#include <vector>
#include <windows.h>
#include <Shlwapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Timer.h"
#include "AutoObserver.h"

class TMState
{
public:

    std::vector<int> timeOutExceeded;
    const std::string stateFileVer;
    std::string selfName;
    std::string enemyName;
    std::string selfRaceInit;
    std::string enemyRaceInit;
    std::string selfRace;
    std::string enemyRace;
    std::string mapName;
    int exceededTimeoutsLimit;
    int frameCountTimeoutsExceeded;
    int frameCount;
    int exceededFrameCountLimit;
    int frameCountFrameCountLimitExceeded;
    int selfKillScore;
    int selfBuildingScore;
    int selfRazingScore;
    int selfUnitScore;
    int selfCustomScore;
    int selfGatheredMinerals;
    int selfSpentMinerals;
    int selfRepairedMinerals;
    int selfRefundedMinerals;
    int selfGatheredGas;
    int selfSpentGas;
    int selfRepairedGas;
    int selfRefundedGas;
    int selfSupplyUsed;
    int selfSupplyTotal;
    int selfScore;
    int enemyScore;
    int gameElapsedTime;
    int exceededElapsedTimeLimit;
    int frameCountElapsedTimeExceeded;
    int defeated;
    int victorious;
    int gameOver;
    int isWinnerInOnEnd;
    int totFrameTime;
    int avgFrameTime;
    int maxFrameTime;
    int minFrameTime;
    unsigned int randomSeed;
    bool initialized;

    TMState();
    void init();
    void update(std::vector<int> & times, int frameCountTimeoutsExceededNew, int gameFrameLimit, int gameElapsedTimeLimit, int gameElapsedTimeNew, int totFrameTimeNew, int avgFrameTimeNew, int maxFrameTimeNew, int minFrameTimeNew);
    void ended(int isWinnerInOnEnd);
    bool write(std::string filename);
};

class TM : public BWAPI::TournamentModule
{
public:

    virtual bool onAction(int actionType, void *parameter = NULL) override;
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
    std::string settingsFileName;
    std::string stateFileName;
    std::string moduleFileName;
    int gameFrameLimit;
    int gameElapsedTimeLimit;
    int frameThreshTimed;
    int stateFileUpdateTimePeriod;
    int stateFileUpdateFramePeriod;
    int totFrameTime;
    int avgFrameTime;
    int maxFrameTime;
    int minFrameTime;
    int localSpeed;
    int frameSkip;
    int frameCountTimeoutsExceeded;
    bool gameOver;
    bool disableUserInput;
    bool disableUserInputLocalSpeed;
    bool drawUnitInfo;
    bool drawTournamentInfo;
    bool drawBotNames;
    bool disableDrawGameTimer;
    bool drawGUI;
    bool autoObs;
    bool autoResumeGame;

public:

    TMAI();
    virtual ~TMAI();
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
    virtual void onReceiveText(BWAPI::Player* player, std::string text) override;
    virtual void onPlayerLeft(BWAPI::Player* player) override;
    virtual void onNukeDetect(BWAPI::Position target) override;
    virtual void onUnitDiscover(BWAPI::Unit* unit) override;
    virtual void onUnitEvade(BWAPI::Unit* unit) override;
    virtual void onUnitShow(BWAPI::Unit* unit) override;
    virtual void onUnitHide(BWAPI::Unit* unit) override;
    virtual void onUnitCreate(BWAPI::Unit* unit) override;
    virtual void onUnitDestroy(BWAPI::Unit* unit) override;
    virtual void onUnitMorph(BWAPI::Unit* unit) override;
    virtual void onUnitComplete(BWAPI::Unit* unit) override;
    virtual void onUnitRenegade(BWAPI::Unit* unit) override;
    virtual void onSaveGame(std::string gameName) override;
    virtual void onPlayerDropped(BWAPI::Player* player) override;
};
