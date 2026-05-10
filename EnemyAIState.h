#pragma once

class Enemy;

class EnemyAIState
{
public:
  virtual ~EnemyAIState() = 0;
  virtual void update(Enemy& enemy, float deltaTime) = 0;
};

class PatrolAIState : public EnemyAIState
{
public:
  void update(Enemy& enemy, float deltaTime) override;
};

class AttackAIState : public EnemyAIState
{
public:
  void update(Enemy& enemy, float deltaTime) override;
};

class RetreatAIState : public EnemyAIState
{
public:
  void update(Enemy& enemy, float deltaTime) override;
};

class DeathFingerprintAIState : public EnemyAIState
{
public:
  void update(Enemy& enemy, float deltaTime) override;
};

class BossPhaseAIState : public EnemyAIState
{
public:
  void update(Enemy& enemy, float deltaTime) override;
};
