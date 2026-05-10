#include "EnemyAIState.h"

#include "Enemy.h"

using namespace std;
using namespace sf;

EnemyAIState::~EnemyAIState()
{
}

void PatrolAIState::update(Enemy& enemy, float deltaTime)
{
}

void AttackAIState::update(Enemy& enemy, float deltaTime)
{
}

void RetreatAIState::update(Enemy& enemy, float deltaTime)
{
}

void DeathFingerprintAIState::update(Enemy& enemy, float deltaTime)
{
}

void BossPhaseAIState::update(Enemy& enemy, float deltaTime)
{
}
