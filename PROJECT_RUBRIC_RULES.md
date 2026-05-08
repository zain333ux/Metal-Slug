# OOP Project Rubric Rules

Use this checklist while implementing the project.

## Highest Priority
- Follow the instructor UML skeleton and defend every added class.
- Use encapsulation, inheritance, polymorphism, and virtual functions clearly.
- Avoid public data members.
- Avoid `dynamic_cast`, `typeid`, and long type-checking chains.
- Avoid unnecessary advanced design patterns.
- Avoid STL `vector`; use the custom `DynamicArray`.
- Manage dynamic memory carefully and avoid leaks/dangling pointers.
- Use delta time for movement and animations.

## Marks To Prioritize
- OOP concepts and viva.
- Core mechanics: pistol, knife, aiming, grenades, damage states.
- Enemy system with animations.
- Weapons and projectile animations.
- Levels/game modes.
- Scoring and highscore.
- Collectibles, vehicles, bosses, and special features after core is stable.

## Current Implementation Direction
- `EntityManager` owns all entities through one `DynamicArray<Entity*>`.
- Typed non-owning arrays track players, enemies, and projectiles for collision.
- Sprite sheets may remain temporary until cleaned assets are ready.
- Separate leg animation is postponed until full-body animation and gameplay are stable.
