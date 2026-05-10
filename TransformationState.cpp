#include "TransformationState.h"

#include "Soldier.h"

using namespace std;
using namespace sf;

TransformationState::~TransformationState()
{
}

void NormalTransformationState::enter(Soldier& soldier)
{
}

void NormalTransformationState::update(Soldier& soldier, float deltaTime)
{
}

void NormalTransformationState::exit(Soldier& soldier)
{
}

void ZombieTransformationState::enter(Soldier& soldier)
{
}

void ZombieTransformationState::update(Soldier& soldier, float deltaTime)
{
}

void ZombieTransformationState::exit(Soldier& soldier)
{
}

void MummyTransformationState::enter(Soldier& soldier)
{
}

void MummyTransformationState::update(Soldier& soldier, float deltaTime)
{
}

void MummyTransformationState::exit(Soldier& soldier)
{
}
