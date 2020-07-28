#include "TournamentModule.h"

using namespace BWAPI;

void TMState::init()
{
    if (initialized)
    {
        return;
    }

    selfName        = BWAPI::Broodwar->self()->getName();
    enemyName       = BWAPI::Broodwar->enemy() ? BWAPI::Broodwar->enemy()->getName() : "";
    selfRaceInit    = BWAPI::Broodwar->self()->getRace().getName();
    enemyRaceInit   = BWAPI::Broodwar->enemy() ? BWAPI::Broodwar->enemy()->getRace().getName() : "";
    selfRace        = selfRaceInit;
    enemyRace       = enemyRaceInit;
    mapName         = BWAPI::Broodwar->mapName();

    initialized     = true;
}

void TMState::update(std::vector<int> & times, int frameCountTimeoutsExceededNew, int gameFrameLimit, int gameElapsedTimeLimit, int gameElapsedTimeNew, int totFrameTimeNew, int avgFrameTimeNew, int maxFrameTimeNew, int minFrameTimeNew)
{
    if (!initialized)
    {
        init();
    }

    frameCount = BWAPI::Broodwar->getFrameCount();
    if (gameFrameLimit > 0 && exceededFrameCountLimit == 0 && frameCount > gameFrameLimit)
    {
        exceededFrameCountLimit = 1;
        frameCountFrameCountLimitExceeded = frameCount;
    }

    selfRace = BWAPI::Broodwar->self()->getRace().getName();
    // Note: enemy race might be "Unknown" until we have scouted them.
    if (BWAPI::Broodwar->enemy())
    {
        enemyRace   = BWAPI::Broodwar->enemy()->getRace().getName();
    }

    selfKillScore           = BWAPI::Broodwar->self()->getKillScore();
    selfBuildingScore       = BWAPI::Broodwar->self()->getBuildingScore();
    selfRazingScore         = BWAPI::Broodwar->self()->getRazingScore();
    selfUnitScore           = BWAPI::Broodwar->self()->getUnitScore();
    selfCustomScore         = BWAPI::Broodwar->self()->getCustomScore();
    selfGatheredMinerals    = BWAPI::Broodwar->self()->gatheredMinerals();
    selfSpentMinerals       = BWAPI::Broodwar->self()->spentMinerals();
    selfRepairedMinerals    = BWAPI::Broodwar->self()->repairedMinerals();
    selfRefundedMinerals    = BWAPI::Broodwar->self()->refundedMinerals();
    selfGatheredGas         = BWAPI::Broodwar->self()->gatheredGas();
    selfSpentGas            = BWAPI::Broodwar->self()->spentGas();
    selfRepairedGas         = BWAPI::Broodwar->self()->repairedGas();
    selfRefundedGas         = BWAPI::Broodwar->self()->refundedGas();
    selfSupplyUsed          = BWAPI::Broodwar->self()->supplyUsed();
    selfSupplyTotal         = BWAPI::Broodwar->self()->supplyTotal();

    selfScore       = selfKillScore
                    + selfBuildingScore
                    + selfRazingScore
                    + selfGatheredMinerals
                    + selfGatheredGas;

    timeOutExceeded = times;
    if (frameCountTimeoutsExceededNew > 0 && exceededTimeoutsLimit == 0)
    {
        exceededTimeoutsLimit = 1;
        frameCountTimeoutsExceeded = frameCountTimeoutsExceededNew;
    }

    defeated        = BWAPI::Broodwar->self()->isDefeated();
    victorious      = BWAPI::Broodwar->self()->isVictorious();
    gameElapsedTime = gameElapsedTimeNew;
    totFrameTime    = totFrameTimeNew;
    avgFrameTime    = avgFrameTimeNew;
    maxFrameTime    = maxFrameTimeNew;
    minFrameTime    = minFrameTimeNew;
    if (gameElapsedTimeLimit > 0 && exceededElapsedTimeLimit == 0 && gameElapsedTimeNew > gameElapsedTimeLimit)
    {
        exceededElapsedTimeLimit = 1;
        frameCountElapsedTimeExceeded = frameCount;
    }
}

void TMState::ended(int isWinnerInOnEndNew)
{
    gameOver = 1;
    isWinnerInOnEnd = isWinnerInOnEndNew;
}

bool TMState::write(std::string filename)
{
    if (!initialized)
    {
        // For safety.
        return false;
    }

    // Prepare the content that will be written to the file.
    std::ostringstream oss;
    oss << stateFileVer             << '\n';
    oss << selfName                 << '\n';
    oss << enemyName                << '\n';
    oss << mapName                  << '\n';
    oss << frameCount               << '\n';
    oss << selfScore                << '\n';
    oss << enemyScore               << '\n';
    oss << gameElapsedTime          << '\n';
    oss << defeated                 << '\n';
    oss << victorious               << '\n';
    oss << gameOver                 << '\n';
    oss << exceededFrameCountLimit  << '\n';
    oss << timeOutExceeded.size()   << '\n';

    for (size_t i(0); i<timeOutExceeded.size(); ++i)
    {
        oss << timeOutExceeded[i] << '\n';
    }

    oss << isWinnerInOnEnd                      << '\n';
    oss << exceededTimeoutsLimit                << '\n';
    oss << exceededElapsedTimeLimit             << '\n';
    oss << frameCountTimeoutsExceeded           << '\n';
    oss << frameCountElapsedTimeExceeded        << '\n';
    oss << frameCountFrameCountLimitExceeded    << '\n';
    oss << totFrameTime                         << '\n';
    oss << avgFrameTime                         << '\n';
    oss << maxFrameTime                         << '\n';
    oss << minFrameTime                         << '\n';
    oss << selfKillScore                        << '\n';
    oss << selfBuildingScore                    << '\n';
    oss << selfRazingScore                      << '\n';
    oss << selfUnitScore                        << '\n';
    oss << selfCustomScore                      << '\n';
    oss << selfGatheredMinerals                 << '\n';
    oss << selfSpentMinerals                    << '\n';
    oss << selfRepairedMinerals                 << '\n';
    oss << selfRefundedMinerals                 << '\n';
    oss << selfGatheredGas                      << '\n';
    oss << selfSpentGas                         << '\n';
    oss << selfRepairedGas                      << '\n';
    oss << selfRefundedGas                      << '\n';
    oss << selfSupplyUsed                       << '\n';
    oss << selfSupplyTotal                      << '\n';
    oss << selfRace                             << '\n';
    oss << enemyRace                            << '\n';
    oss << selfRaceInit                         << '\n';
    oss << enemyRaceInit                        << '\n';
    oss << randomSeed                           << '\n';

    // Write the file (rewrite it from scratch if it already exists).
    std::ofstream outfile(filename.c_str(), std::ios::out);
    if (outfile)
    {
        outfile << oss.str();
        outfile.flush();
        return (bool)outfile;
    }
    else
    {
        return false;
    }
}

// Shared between TM and TMAI.
int cmdOptimizLevelInit = 1;
int cmdOptimizLevelMin = 0;
bool disableBotSendText = true;
bool disableBotPrintf = true;
bool disableBotSetTextSize = true;

TMAI::TMAI()
{
    gameTimer.start();

    char buffer[MAX_PATH];
    buffer[0] = '\0';
    GetModuleFileName(NULL, buffer, MAX_PATH);
    moduleFileName = buffer;

    frameTimes.resize(100000);
    timerLimits.push_back(55);
    timerLimitsBound.push_back(320);
    timerLimits.push_back(1000);
    timerLimitsBound.push_back(10);
    timerLimits.push_back(10000);
    timerLimitsBound.push_back(1);
    timerLimitsExceeded.push_back(0);
    timerLimitsExceeded.push_back(0);
    timerLimitsExceeded.push_back(0);

    diagnostics = parseOptions();
}

TMAI::~TMAI() _NOEXCEPT
{
    try
    {
        if (!gameOver && state.initialized)
        {
            state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, (int)gameTimer.getElapsedTimeInMilliSec(), totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
            state.write(stateFileName);
        }
    }
    catch (...)
    {
    }
}

void TMAI::onStart()
{
    Broodwar->setGUI(drawGUI);

    if (drawTournamentInfo)
    {
        BWAPI::Broodwar->printf("Path is %s", moduleFileName.c_str());

        for (auto& diagnostic : diagnostics)
        {
            BWAPI::Broodwar->printf("%s", diagnostic.c_str());
        }
    }

    // Set the command optimization level (reduces high APM, size of bloated replays, etc).
    Broodwar->setCommandOptimizationLevel(cmdOptimizLevelInit);
    Broodwar->setLocalSpeed(localSpeed);
    Broodwar->setFrameSkip(frameSkip);

    if (!disableUserInput && !Broodwar->isFlagEnabled(BWAPI::Flag::UserInput))
    {
        Broodwar->enableFlag(BWAPI::Flag::UserInput);
    }

    state.init();

    if (autoObs)
    {
        autoObserver.onStart();
    }
}

void TMAI::onEnd(bool isWinner)
{
    state.ended((int)isWinner);
    state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, (int)gameTimer.getElapsedTimeInMilliSec(), totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
    state.write(stateFileName);
    gameOver = 1;

    if (autoObs)
    {
        autoObserver.onEnd(isWinner);
    }
}

void TMAI::onFrame()
{
    const int lastEventTime = BWAPI::Broodwar->getLastEventTime();
    const int frameCount = BWAPI::Broodwar->getFrameCount();

    // Add the frame times for this frame.
    frameTimes[frameCount] += lastEventTime;

    // If the user is not allowed to change some settings, we keep re-applying them,
    // in case the user changed something they are not allowed to by typing chat messages.
    if (disableUserInputLocalSpeed)
    {
        Broodwar->setLocalSpeed(localSpeed);
    }

    if (autoResumeGame && Broodwar->isPaused())
    {
        Broodwar->resumeGame();
    }

    if ((int)frameTimes.size() < frameCount + 10)
    {
        frameTimes.push_back(0);
    }

    // Check for per-frame timeouts.
    if (frameCount > 0 && frameCount >= frameThreshTimed + 1)
    {
        // Update some timer-related metrics.
        const int lastFrameTime = frameTimes[frameCount - 1];
        totFrameTime += lastFrameTime;
        const int numFramesCounted = frameCount - frameThreshTimed;
        avgFrameTime = (numFramesCounted > 0 ? (totFrameTime / numFramesCounted) : 0);

        if (lastFrameTime > maxFrameTime)
        {
            maxFrameTime = lastFrameTime;
        }

        if (lastFrameTime < minFrameTime)
        {
            minFrameTime = lastFrameTime;
        }

        // Leave the game if the timer exceeded any frame time limits more than the
        // allowed number of occurrences.
        int frameCountTimeoutsExceededOld = frameCountTimeoutsExceeded;
        for (size_t t(0); t < timerLimits.size(); ++t)
        {
            if (lastFrameTime > timerLimits[t])
            {
                ++timerLimitsExceeded[t];
                if (timerLimitsExceeded[t] >= timerLimitsBound[t])
                {
                    if (frameCountTimeoutsExceeded == 0)
                    {
                        frameCountTimeoutsExceeded = frameCount;
                    }

                    Broodwar->sendText("TIMEOUT on %d ms", timerLimits[t]);
                    Broodwar->leaveGame();
                }
            }
        }

        // Update the state file if the game hasn't ended yet and it's the first
        // occasion that any per-frame timeouts have exceeded the allowed number
        // of occurrences.
        if (frameCountTimeoutsExceeded > 0 && frameCountTimeoutsExceededOld == 0 && frameCount != state.frameCount && !gameOver)
        {
            state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, (int)gameTimer.getElapsedTimeInMilliSec(), totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
            state.write(stateFileName);
        }
    }

    // Check game frame limit.
    if (gameFrameLimit > 0 && frameCount > gameFrameLimit)
    {
        Broodwar->sendText("Game time limit of %d frames reached, exiting", gameFrameLimit);
        Broodwar->leaveGame();
        if (state.exceededFrameCountLimit == 0 && frameCount != state.frameCount && !gameOver)
        {
            state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, (int)gameTimer.getElapsedTimeInMilliSec(), totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
            state.write(stateFileName);
        }
    }

    // Check game elapsed time limit.
    if (gameElapsedTimeLimit > 0)
    {
        const int gameElapsedTime = (int)gameTimer.getElapsedTimeInMilliSec();
        if (gameElapsedTime > gameElapsedTimeLimit)
        {
            Broodwar->sendText("Game time limit of %d milliseconds reached, exiting", gameElapsedTimeLimit);
            Broodwar->leaveGame();
            if (state.exceededElapsedTimeLimit == 0 && frameCount != state.frameCount && !gameOver)
            {
                state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, gameElapsedTime, totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
                state.write(stateFileName);
            }
        }
    }

    // Occasionally update the game state file.
    if (frameCount == 0 || (frameCount != state.frameCount && !gameOver))
    {
        const int gameElapsedTime = (int)gameTimer.getElapsedTimeInMilliSec();
        if (frameCount == 0 ||
            (stateFileUpdateTimePeriod > 0 && gameElapsedTime >= state.gameElapsedTime + stateFileUpdateTimePeriod) ||
            (stateFileUpdateFramePeriod > 0 && frameCount % stateFileUpdateFramePeriod == 0))
        {
            state.update(timerLimitsExceeded, frameCountTimeoutsExceeded, gameFrameLimit, gameElapsedTimeLimit, gameElapsedTime, totFrameTime, avgFrameTime, maxFrameTime, minFrameTime);
            state.write(stateFileName);
        }
    }

    if (autoObs)
    {
        autoObserver.onFrame();
    }

    drawTMSettings(10, 10);

    if (drawUnitInfo)
    {
        drawUnitInformation(440,6);
    }

    if (!disableDrawGameTimer)
    {
        drawGameTimer(340, 353);
    }
}

void TMAI::drawTMSettings(int x, int y)
{
    int drawX = x;
    int drawY = y;
    int width = 120;

    if (drawBotNames)
    {
        BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Huge);
        Broodwar->drawTextScreen(drawX, drawY, "\x07%s \x04vs. \x06%s", BWAPI::Broodwar->self()->getName().c_str(), (BWAPI::Broodwar->enemy() ? BWAPI::Broodwar->enemy()->getName().c_str() : ""));
        drawY += 18;

        BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Large);
        Broodwar->drawTextScreen(drawX, drawY, "\x03%s", BWAPI::Broodwar->mapFileName().c_str());
        BWAPI::Broodwar->setTextSize();
        drawY += 30;
    }

    /*Broodwar->drawTextScreen(drawX, drawY, "\x04 Player Name:");
    Broodwar->drawTextScreen(drawX+width, drawY, "\x07 %s", BWAPI::Broodwar->self()->getName().c_str());
    drawY += 10;

    Broodwar->drawTextScreen(drawX, drawY, "\x04 Enemy Name:");
    Broodwar->drawTextScreen(drawX+width, drawY, "\x07 %s", BWAPI::Broodwar->enemy()->getName().c_str());
    drawY += 10;

    Broodwar->drawTextScreen(drawX, drawY, "\x04 Map Filename:");
    Broodwar->drawTextScreen(drawX+width, drawY, " %s", BWAPI::Broodwar->mapFileName().c_str());
    drawY += 20;*/

    if (drawTournamentInfo)
    {
        BWAPI::Broodwar->drawTextScreen(drawX, drawY, "\x04 Current Game Time: ");
        BWAPI::Broodwar->drawTextScreen(drawX + width, drawY, " %d", BWAPI::Broodwar->getFrameCount());
        drawY += 10;


        BWAPI::Broodwar->drawTextScreen(drawX, drawY, "\x04 Game Time Limit: ");
        BWAPI::Broodwar->drawTextScreen(drawX + width, drawY, " %d", gameFrameLimit);
        drawY += 10;

        BWAPI::Broodwar->drawTextScreen(drawX, drawY, "\x04 BWAPI Local Speed: ");
        BWAPI::Broodwar->drawTextScreen(drawX + width, drawY, " %d", localSpeed);
        drawY += 10;

        BWAPI::Broodwar->drawTextScreen(drawX, drawY, "\x04 BWAPI Frame Skip: ");
        BWAPI::Broodwar->drawTextScreen(drawX + width, drawY, " %d", frameSkip);
        drawY += 10;

        drawY += 10;
        for (size_t t(0); t<timerLimits.size(); ++t)
        {
            BWAPI::Broodwar->drawTextScreen(drawX, drawY, "\x04 # Frames > %d ms ", timerLimits[t]);
            BWAPI::Broodwar->drawTextScreen(drawX + width, drawY, " %d   (Max %d)", timerLimitsExceeded[t], timerLimitsBound[t]);

            drawY += 10;
        }
    }
}

void TMAI::drawUnitInformation(int x, int y)
{
    std::string prefix = "\x04";

    //BWAPI::Broodwar->drawBoxScreen(x-5, y-4, x+200, y+200, BWAPI::Colors::Black, true);

    //BWAPI::Broodwar->drawTextScreen(x, y, "\x04 Unit Information: %s", BWAPI::Broodwar->self()->getRace().getName().c_str());
    BWAPI::Broodwar->drawTextScreen(x, y+20, "\x04%s's Units", BWAPI::Broodwar->self()->getName().c_str());
    BWAPI::Broodwar->drawTextScreen(x+160, y+20, "\x04#");
    BWAPI::Broodwar->drawTextScreen(x+180, y+20, "\x04X");

    int yspace = 0;

    // for each unit in the queue
    for each (BWAPI::UnitType t in BWAPI::UnitTypes::allUnitTypes())
    {
        int numUnits = BWAPI::Broodwar->self()->completedUnitCount(t) + BWAPI::Broodwar->self()->incompleteUnitCount(t);
        int numDeadUnits = BWAPI::Broodwar->self()->deadUnitCount(t);

        // if there exist units in the vector
        if (numUnits > 0)
        {
            if (t.isWorker())           { prefix = "\x0F"; }
            else if (t.isDetector())    { prefix = "\x07"; }
            else if (t.canAttack())     { prefix = "\x08"; }
            else if (t.isBuilding())    { prefix = "\x03"; }
            else                        { prefix = "\x04"; }

            BWAPI::Broodwar->drawTextScreen(x, y+40+((yspace)*10), "%s%s", prefix.c_str(), t.getName().c_str());
            BWAPI::Broodwar->drawTextScreen(x+160, y+40+((yspace)*10), "%s%d", prefix.c_str(), numUnits);
            BWAPI::Broodwar->drawTextScreen(x+180, y+40+((yspace++)*10), "%s%d", prefix.c_str(), numDeadUnits);
        }
    }
}

void TMAI::drawGameTimer(int x, int y)
{
    if (!disableDrawGameTimer)
    {
        const int gameTime = BWAPI::Broodwar->getFrameCount() * 42;
        const int gameTimeHPart = gameTime / (60 * 60 * 1000);
        const int gameTimeMPart = (gameTime / (60 * 1000)) - (gameTimeHPart * 60);
        const int gameTimeSPart = (gameTime / 1000) - (gameTimeMPart * 60) - (gameTimeHPart * (60 * 60));

        if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
        {
            // The HUD in the Brood War UI is a little lower as Terran.
            y += 4;
        }

        BWAPI::Broodwar->setTextSize(BWAPI::Text::Size::Small);
        if (gameTimeHPart > 0)
        {
            BWAPI::Broodwar->drawTextScreen(x, y, "\x01%.2d:%.2d:%.2d", gameTimeHPart, gameTimeMPart, gameTimeSPart);
        }
        else
        {
            x += 4;
            BWAPI::Broodwar->drawTextScreen(x, y, "\x01%.2d:%.2d", gameTimeMPart, gameTimeSPart);
        }

        BWAPI::Broodwar->setTextSize();
    }
}

void TMAI::onSendText(std::string text)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onSendText(text);
    }
}

void TMAI::onReceiveText(BWAPI::Player player, std::string text)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onReceiveText(player, text);
    }
}

void TMAI::onPlayerLeft(BWAPI::Player player)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onPlayerLeft(player);
    }
}

void TMAI::onNukeDetect(BWAPI::Position target)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onNukeDetect(target);
    }
}

void TMAI::onUnitDiscover(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitDiscover(unit);
    }
}

void TMAI::onUnitEvade(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitEvade(unit);
    }
}

void TMAI::onUnitShow(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitShow(unit);
    }
}

void TMAI::onUnitHide(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitHide(unit);
    }
}

void TMAI::onUnitCreate(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitCreate(unit);
    }
}

void TMAI::onUnitDestroy(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitDestroy(unit);
    }
}

void TMAI::onUnitMorph(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitMorph(unit);
    }
}

void TMAI::onUnitComplete(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitComplete(unit);
    }
}

void TMAI::onUnitRenegade(BWAPI::Unit unit)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onUnitRenegade(unit);
    }
}

void TMAI::onSaveGame(std::string gameName)
{
    frameTimes[BWAPI::Broodwar->getFrameCount()] += BWAPI::Broodwar->getLastEventTime();
    if (autoObs)
    {
        autoObserver.onSaveGame(gameName);
    }
}

bool TM::onAction(BWAPI::Tournament::ActionID actionType, void *parameter)
{
    switch ( actionType )
    {
        case Tournament::EnableFlag:
            switch ( *(int*)parameter )
            {
                case Flag::CompleteMapInformation:      return false;
                case Flag::UserInput:                   return false;
                default:                                break;
            }
            // If more flags are added, by default disallow unrecognized flags.
            return false;

        case Tournament::PauseGame:                     return false;
    //  case Tournament::RestartGame:                   return false;
        case Tournament::ResumeGame:                    return false;
        case Tournament::SetFrameSkip:                  return false;
        case Tournament::SetGUI:                        return false;
        case Tournament::SetLocalSpeed:                 return false;
        case Tournament::SetMap:                        return false;
        case Tournament::LeaveGame:                     return true;
    //  case Tournament::ChangeRace:                    return false;
        case Tournament::SetLatCom:                     return true;
        case Tournament::SetTextSize:                   return !disableBotSetTextSize;
        case Tournament::SendText:                      return !disableBotSendText;
        case Tournament::Printf:                        return !disableBotPrintf;
        case Tournament::SetCommandOptimizationLevel:
            return *(int*)parameter >= cmdOptimizLevelMin;

        default:                                        break;
    }

    return true;
}

void TM::onFirstAdvertisement()
{
}

std::string TMAI::envVar(const char *var)
{
    const unsigned long bufSize = (1 << 15) - 1;
    char buf[bufSize];
    buf[0] = '\0';
    GetEnvironmentVariableA(var, buf, bufSize);
    return std::string(buf);
}

bool& TMAI::parseField(std::istringstream& iss, bool& field)
{
    // Skip leading whitespace.
    iss >> std::ws;
    std::string val;
    iss >> val;
    if (!val.empty())
    {
        field = (val != "false");
    }

    return field;
}

std::string& TMAI::parseField(std::istringstream& iss, std::string& field)
{
    // Skip leading whitespace.
    iss >> std::ws;
    std::string val;
    std::getline(iss, val);
    if (!val.empty())
    {
        field = val;
    }

    return field;
}

template<typename T>
T& TMAI::parseField(std::istringstream& iss, T& field)
{
    // Skip leading whitespace.
    iss >> std::ws;
    if (iss.peek() != EOF)
    {
        iss >> field;
    }

    return field;
}

std::vector<std::string> TMAI::parseOptions()
{
    parseField(std::istringstream(envVar("TM_SETTINGS_FILE")), settingsFileName);

    std::vector<std::string> lines;
    std::vector<std::string> diagnostics;

    //
    // Read the settings file, if any.
    //

    {
        // Set up the file.
        std::ifstream fin(settingsFileName.c_str());
        if (!fin.is_open())
        {
            diagnostics.push_back("Tournament Module Settings File Not Found, Using Defaults");
        }
        else
        {
            std::string line;

            while (fin.good())
            {
                // Get the line and set up the string stream.
                getline(fin, line);

                // Skip blank lines and comments.
                if (line.length() > 1 && line[0] != '#')
                {
                    lines.push_back(line);
                }
            }
        }
    }

    bool clearedForFirstTimeout = false;

    // Process the settings file.
    for (size_t l(0); l<lines.size(); ++l)
    {
        std::istringstream iss(lines[l]);
        std::string option;
        iss >> option;

        if (option == "TM_LOCAL_SPEED" || option == "LocalSpeed")
        {
            parseField(iss, localSpeed);
        }
        else if (option == "TM_FRAME_SKIP" || option == "FrameSkip")
        {
            parseField(iss, frameSkip);
        }
        else if (option == "TM_TIMEOUTS" || option == "Timeouts" || option == "Timeout")
        {
            if (option != "Timeout" || !clearedForFirstTimeout)
            {
                timerLimits.clear();
                timerLimitsBound.clear();
                timerLimitsExceeded.clear();
                if (option == "Timeout")
                {
                    clearedForFirstTimeout = true;
                }
            }

            int timeLimit = 0;
            int bound = 0;
            parseField(iss, timeLimit);
            parseField(iss, bound);

            if (option == "Timeout")
            {
                if (timeLimit > 0 && bound > 0)
                {
                    timerLimits.push_back(timeLimit);
                    timerLimitsBound.push_back(bound);
                    timerLimitsExceeded.push_back(0);
                }
            }
            else
            {
                while (iss && timeLimit > 0 && bound > 0)
                {
                    timerLimits.push_back(timeLimit);
                    timerLimitsBound.push_back(bound);
                    timerLimitsExceeded.push_back(0);
                    timeLimit = 0;
                    bound = 0;
                    parseField(iss, timeLimit);
                    parseField(iss, bound);
                }
            }
        }
        else if (option == "TM_GAME_FRAME_LIMIT" || option == "GameFrameLimit")
        {
            parseField(iss, gameFrameLimit);
        }
        else if (option == "TM_FRAME_THRESH_TIMED")
        {
            parseField(iss, frameThreshTimed);
        }
        else if (option == "TM_GAME_TIME_LIMIT")
        {
            parseField(iss, gameElapsedTimeLimit);
        }
        else if (option == "TM_CMD_OPTIMIZ_LEVEL_INIT")
        {
            parseField(iss, cmdOptimizLevelInit);
        }
        else if (option == "TM_CMD_OPTIMIZ_LEVEL_MIN")
        {
            parseField(iss, cmdOptimizLevelMin);
        }
        else if (option == "TM_DISABLE_USER_INPUT")
        {
            parseField(iss, disableUserInput);
        }
        else if (option == "TM_DISABLE_USER_INPUT_LOCAL_SPEED")
        {
            parseField(iss, disableUserInputLocalSpeed);
        }
        else if (option == "TM_DISABLE_BOT_SENDTEXT")
        {
            parseField(iss, disableBotSendText);
        }
        else if (option == "TM_DISABLE_BOT_PRINTF")
        {
            parseField(iss, disableBotPrintf);
        }
        else if (option == "TM_DISABLE_BOT_SETTEXTSIZE")
        {
            parseField(iss, disableBotSetTextSize);
        }
        else if (option == "TM_DRAW_UNIT_INFO" || option == "DrawUnitInfo")
        {
            parseField(iss, drawUnitInfo);
        }
        else if (option == "TM_DRAW_TOURNAMENT_INFO" || option == "DrawTournamentInfo")
        {
            parseField(iss, drawTournamentInfo);
        }
        else if (option == "TM_DRAW_BOT_NAMES" || option == "DrawBotNames")
        {
            parseField(iss, drawBotNames);
        }
        else if (option == "TM_DISABLE_DRAW_GAME_TIMER")
        {
            parseField(iss, disableDrawGameTimer);
        }
        else if (option == "TM_DRAW_GUI")
        {
            parseField(iss, drawGUI);
        }
        else if (option == "TM_AUTO_OBS")
        {
            parseField(iss, autoObs);
        }
        else if (option == "TM_AUTO_RESUME_GAME")
        {
            parseField(iss, autoResumeGame);
        }
        else if (option == "TM_STATE_FILE")
        {
            parseField(iss, stateFileName);
        }
        else if (option == "TM_STATE_FILE_UPDATE_TIME_PERIOD")
        {
            parseField(iss, stateFileUpdateTimePeriod);
        }
        else if (option == "TM_STATE_FILE_UPDATE_FRAME_PERIOD")
        {
            parseField(iss, stateFileUpdateFramePeriod);
        }
        else
        {
            diagnostics.push_back(std::string("Invalid Option in Tournament Module Settings: ") + option);
        }
    }

    //
    // Override the settings from environment variable(s), if any.
    //

    parseField(std::istringstream(envVar("TM_LOCAL_SPEED")), localSpeed);
    parseField(std::istringstream(envVar("TM_FRAME_SKIP")), frameSkip);

    {
        // Timeouts based on the total amount of time taken to run the AI
        // module callbacks for individual frame(s). Specified via a
        // space-delimited set of at least one pair(s) of timeout values. The
        // first value of each pair is the number of milliseconds and the
        // second value of each pair is the number of frames. Examples:
        // TM_TIMEOUTS="0 0" means no (per-frame) timeouts.
        // TM_TIMEOUTS="55 320 1000 10 10000 1" means timeout when
        // 320 frames exceed 55 milliseconds or
        // 10 frames exceed 1000 milliseconds or
        // 1 frame exceeds 10000 milliseconds.
        std::string timeoutsStr;
        parseField(std::istringstream(envVar("TM_TIMEOUTS")), timeoutsStr);
        if (!timeoutsStr.empty())
        {
            timerLimits.clear();
            timerLimitsBound.clear();
            timerLimitsExceeded.clear();

            int timeLimit = 0;
            int bound = 0;
            std::istringstream iss(timeoutsStr);
            parseField(iss, timeLimit);
            parseField(iss, bound);

            while (iss && timeLimit > 0 && bound > 0)
            {
                timerLimits.push_back(timeLimit);
                timerLimitsBound.push_back(bound);
                timerLimitsExceeded.push_back(0);
                timeLimit = 0;
                bound = 0;
                parseField(iss, timeLimit);
                parseField(iss, bound);
            }
        }
    }

    parseField(std::istringstream(envVar("TM_GAME_FRAME_LIMIT")), gameFrameLimit);
    parseField(std::istringstream(envVar("TM_FRAME_THRESH_TIMED")), frameThreshTimed);
    parseField(std::istringstream(envVar("TM_GAME_TIME_LIMIT")), gameElapsedTimeLimit);
    parseField(std::istringstream(envVar("TM_CMD_OPTIMIZ_LEVEL_INIT")), cmdOptimizLevelInit);
    parseField(std::istringstream(envVar("TM_CMD_OPTIMIZ_LEVEL_MIN")), cmdOptimizLevelMin);
    parseField(std::istringstream(envVar("TM_DISABLE_USER_INPUT")), disableUserInput);
    parseField(std::istringstream(envVar("TM_DISABLE_USER_INPUT_LOCAL_SPEED")), disableUserInputLocalSpeed);
    parseField(std::istringstream(envVar("TM_DISABLE_BOT_SENDTEXT")), disableBotSendText);
    parseField(std::istringstream(envVar("TM_DISABLE_BOT_PRINTF")), disableBotPrintf);
    parseField(std::istringstream(envVar("TM_DISABLE_BOT_SETTEXTSIZE")), disableBotSetTextSize);
    parseField(std::istringstream(envVar("TM_DRAW_UNIT_INFO")), drawUnitInfo);
    parseField(std::istringstream(envVar("TM_DRAW_TOURNAMENT_INFO")), drawTournamentInfo);
    parseField(std::istringstream(envVar("TM_DRAW_BOT_NAMES")), drawBotNames);
    parseField(std::istringstream(envVar("TM_DISABLE_DRAW_GAME_TIMER")), disableDrawGameTimer);
    parseField(std::istringstream(envVar("TM_DRAW_GUI")), drawGUI);
    parseField(std::istringstream(envVar("TM_AUTO_OBS")), autoObs);
    parseField(std::istringstream(envVar("TM_AUTO_RESUME_GAME")), autoResumeGame);
    parseField(std::istringstream(envVar("TM_STATE_FILE")), stateFileName);
    parseField(std::istringstream(envVar("TM_STATE_FILE_UPDATE_TIME_PERIOD")), stateFileUpdateTimePeriod);
    parseField(std::istringstream(envVar("TM_STATE_FILE_UPDATE_FRAME_PERIOD")), stateFileUpdateFramePeriod);

    return diagnostics;
}