#pragma once

class Soldier;

class TransformationState
{
public:
  virtual ~TransformationState() = 0;
  virtual void enter(Soldier& soldier) = 0;
  virtual void update(Soldier& soldier, float deltaTime) = 0;
  virtual void exit(Soldier& soldier) = 0;
};

class NormalTransformationState : public TransformationState
{
public:
  void enter(Soldier& soldier) override;
  void update(Soldier& soldier, float deltaTime) override;
  void exit(Soldier& soldier) override;
};

class ZombieTransformationState : public TransformationState
{
public:
  void enter(Soldier& soldier) override;
  void update(Soldier& soldier, float deltaTime) override;
  void exit(Soldier& soldier) override;
};

class MummyTransformationState : public TransformationState
{
public:
  void enter(Soldier& soldier) override;
  void update(Soldier& soldier, float deltaTime) override;
  void exit(Soldier& soldier) override;
};
