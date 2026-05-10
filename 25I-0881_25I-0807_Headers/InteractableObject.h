#pragma once

#include "Entity.h"

class InteractableObject : public Entity
{
public:
  virtual ~InteractableObject() = 0;
  virtual void interact() = 0;
};
