#include "TransformationState.h"
#include "Soldier.h"

using namespace std;
using namespace sf;

TransformationState::~TransformationState()
{
}

void NormalTransformationState::enter(Soldier& soldier)
{
	// normal state has no extra setup right now
}

void NormalTransformationState::update(Soldier& soldier, float deltaTime)
{
	// base player movement still handles normal form
}

void NormalTransformationState::exit(Soldier& soldier)
{
}

void ZombieTransformationState::enter(Soldier& soldier)
{
	// zombie setup is handled by PlayerSoldier sprites
}

void ZombieTransformationState::update(Soldier& soldier, float deltaTime)
{

}

void ZombieTransformationState::exit(Soldier& soldier)
{
}

void MummyTransformationState::enter(Soldier& soldier)
{
	// mummy form uses same hook as other transformations
}

void MummyTransformationState::update(Soldier& soldier, float deltaTime)
{
	// actual movement limits stay in PlayerSoldier for now
}

void MummyTransformationState::exit(Soldier& soldier)
{
}
