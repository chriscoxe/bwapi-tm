#include "AutoObserver.h"

void AutoObserver::onFrame()
{
    bool pickUnitToFollow = !_observerFollowingUnit || !_observerFollowingUnit->exists() || (BWAPI::Broodwar->getFrameCount() - _cameraLastMoved > _unitFollowFrames);

    if (pickUnitToFollow)
    {
        for (BWAPI::UnitInterface * unit : BWAPI::Broodwar->getAllUnits())
        {
            if (unit->isUnderAttack() || unit->isAttacking())
            {
                _cameraLastMoved = BWAPI::Broodwar->getFrameCount();
                _unitFollowFrames = 6;
                _observerFollowingUnit = unit;
                pickUnitToFollow = false;
                break;
            }
        }
    }

    if (pickUnitToFollow)
    {
        for (BWAPI::UnitInterface * unit : BWAPI::Broodwar->getAllUnits())
        {
            if (unit->isBeingConstructed() && (unit->getRemainingBuildTime() < 12))
            {
                _cameraLastMoved = BWAPI::Broodwar->getFrameCount();
                _unitFollowFrames = 24;
                _observerFollowingUnit = unit;
                pickUnitToFollow = false;
                break;
            }
        }
    }

    if (_observerFollowingUnit && _observerFollowingUnit->exists())
    {
        BWAPI::Broodwar->setScreenPosition(_observerFollowingUnit->getPosition() - BWAPI::Position(320, 180));
    }
}

void AutoObserver::onUnitCreate(BWAPI::Unit unit)
{
    const int mult = 3;

    if (BWAPI::Broodwar->getFrameCount() - _cameraLastMovedOnUnitCreate < _cameraMoveTime*mult)
    {
        return;
    }

    BWAPI::Broodwar->setScreenPosition(unit->getPosition() - BWAPI::Position(320, 240));
    _cameraLastMovedOnUnitCreate = BWAPI::Broodwar->getFrameCount();
}