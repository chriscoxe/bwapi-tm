#pragma once

#include "BWAPI.h"

class AutoObserver : public BWAPI::AIModule
{
    const int                   _cameraMoveTime;
    int                         _cameraLastMovedOnUnitCreate;
    int                         _cameraLastMoved;
    int                         _unitFollowFrames;
    BWAPI::Unit*                _observerFollowingUnit;

public:

    AutoObserver();
    virtual void onFrame() override;
    virtual void onUnitCreate(BWAPI::Unit* unit) override;
};