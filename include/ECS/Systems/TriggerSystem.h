#ifndef TRIGGERSYSTEM_H
#define TRIGGERSYSTEM_H

#include <System.h>


class TriggerSystem : public System
{
    public:
        TriggerSystem(Triggers& t, Positions& p, Velocities& v, Renderers& r);
        virtual ~TriggerSystem();

        virtual void update(float dt);

        void checkPointTriggered(Entity entity, CheckPointTriggerComponent& checkPoint);
        void CueAttackTriggered(Entity entity, CueAttackTriggerComponent& cueAttack);
        void EndLevelTriggered(Entity entity, EndLevelTriggerComponent& endLevel);
        void HoleTriggered(Entity entity, HoleTriggerComponent& hole);
        void WarpTriggered(Entity entity, WarpTriggerComponent& warp);

    protected:

    private:
        Triggers* triggers;
        Positions* positions;
        Velocities* velocities;
        Renderers* renderers;
};

#endif // TRIGGERSYSTEM_H