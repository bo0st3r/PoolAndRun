#ifndef SYSTEM_H
#define SYSTEM_H

#include <unordered_map>
#include <vector>

#include <EntityManagerSingleton.h>
#include <ComponentManagerSingleton.h>
#include <Vector2fMath.h>
#include <ConstraintEnum.h>

#include <Component.h>
#include <PositionComponent.h>
#include <VelocityComponent.h>
#include <GravityComponent.h>
#include <RendererComponent.h>
#include <ColliderComponent.h>
#include <CharacterComponent.h>
#include <ConstraintComponent.h>
#include <ControllerComponent.h>
#include <UIComponent.h>

#include <TriggerComponent.h>
#include <CheckPointTriggerComponent.h>
#include <CueAttackTriggerComponent.h>
#include <EndLevelTriggerComponent.h>
#include <HoleTriggerComponent.h>
#include <WarpTriggerComponent.h>

using Positions = EntityComponentMap<PositionComponent>;
using Velocities = EntityComponentMap<VelocityComponent>;
using Gravities = EntityComponentMap<GravityComponent>;
using Renderers = EntityComponentMap<RendererComponent>;
using Colliders = EntityComponentMap<ColliderComponent>;
using Triggers = EntityComponentMap<TriggerComponent>;
using Characters = EntityComponentMap<CharacterComponent>;
using Constraints = EntityComponentMap<ConstraintComponent>;
using Respawns = EntityComponentMap<RespawnComponent>;
using Controllers = EntityComponentMap<ControllerComponent>;
using UIs = EntityComponentMap<UIComponent>;

class System
{
    public:
        virtual void update(float dt)=0; //le system update en tenant compte du temps depuis la derni�re update
    protected:

    private:
};

#endif // SYSTEM_H
