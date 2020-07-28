#pragma once

#include "BWAPI.h"

class AutoObserver : public BWAPI::AIModule
{
    const int                   _cameraMoveTime = 48;
    int                         _cameraLastMovedOnUnitCreate = 0;
    int                         _cameraLastMoved = 0;
    int                         _unitFollowFrames = 0;
    BWAPI::UnitInterface *      _observerFollowingUnit = nullptr;

public:

    virtual void onFrame() override;
    virtual void onUnitCreate(BWAPI::Unit unit) override;
};