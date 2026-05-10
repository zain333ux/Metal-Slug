#include "EnemyAIState.h"

#include "Enemy.h"

using namespace std;
using namespace sf;

EnemyAIState::~EnemyAIState()
{
}

void PatrolAIState::update(Enemy& enemy, float deltaTime)
{
	// hook for patrol behavior if enemy wants swappable AI
}

void AttackAIState::update(Enemy& enemy, float deltaTime)
{
	// attack logic can be moved here without changing Enemy interface
}

void RetreatAIState::update(Enemy& enemy, float deltaTime)
{
}

void DeathFingerprintAIState::update(Enemy& enemy, float deltaTime)
{
	// death behavior state kept for UML demo explanation
}

void BossPhaseAIState::update(Enemy& enemy, float deltaTime)
{
	// boss phase AI can plug in here later
}
