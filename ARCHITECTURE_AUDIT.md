# Architecture Audit

Source of truth: `OOP_UML_render.png`.

## Current UML Compliance Table

| Current class | Current base class | Required UML base class | Compliant | Required fix | Risk |
|---|---|---|---|---|---|
| Game | none | concrete Game | Yes | None | Safe |
| GameStateManager | none | concrete manager owning GameState | Yes | Confirm destructor deletes current state | Safe |
| GameState | none | abstract GameState | Yes | None | Safe |
| PlayState | GameState | GameState | Yes | None | Safe |
| EntityManager | none | concrete manager owning Entity pool | Yes | Confirm clear/removeInactive delete owned entities | Safe |
| LevelManager | none | concrete manager owning Level | Yes | Keep destructor/clear deleting current level | Safe |
| Entity | none | abstract Entity | No | Make abstract while preserving shared update/draw helpers | Safe |
| DamageableEntity | Entity | abstract DamagableEntity/DamageableEntity | No | Make abstract while preserving default damage behavior | Safe |
| Soldier | DamageableEntity | abstract Soldier | No | Make abstract, add owned TransformationState pointer | Moderate |
| PlayerSoldier | Soldier | concrete PlayerSoldier | Yes | Keep concrete and route weapon through Soldier aggregation | Safe |
| TransformationState | missing | abstract TransformationState | No | Add abstract state base and concrete Normal/Zombie/Mummy states | Safe |
| Enemy | DamageableEntity | abstract Enemy | No | Make abstract, add aggregated EnemyAIState pointer | Moderate |
| EnemyAIState | missing | abstract EnemyAIState | No | Add abstract AI state base and simple defendable concrete states | Safe |
| Enemy concrete types | Enemy | Enemy or defensible subclass | Yes | None | Safe |
| Weapon | none | abstract Weapon | Yes | None | Safe |
| ProjectileWeapon | Weapon | abstract ProjectileWeapon | No | Make abstract and add concrete BasicProjectileWeapon for existing firing logic | Moderate |
| Vehicle | DamageableEntity | abstract Vehicle | No | Make abstract and stop direct instantiation | Moderate |
| GroundVehicle | missing | abstract GroundVehicle -> Vehicle | No | Add abstract tier class | Safe |
| AerialVehicle | missing | abstract AerialVehicle -> Vehicle | No | Add abstract tier class and move SlugFlyer under it | Safe |
| AquaticVehicle | missing | abstract AquaticVehicle -> Vehicle | No | Add abstract tier class and move SlugMariner under it | Safe |
| MetalSlug | missing | concrete GroundVehicle | No | Add thin MetalSlug class over existing Vehicle behavior | Safe |
| SlugFlyer | Vehicle | AerialVehicle | No | Change base to AerialVehicle | Safe |
| SlugMariner | Vehicle | AquaticVehicle | No | Change base to AquaticVehicle | Safe |
| Projectile | Entity | abstract Projectile | No | Make abstract while preserving shared projectile behavior | Safe |
| StraightProjectile | Projectile | abstract StraightProjectile | No | Make abstract and add concrete BasicStraightProjectile | Moderate |
| BallisticProjectile | Projectile | abstract BallisticProjectile | No | Make abstract, add default constructor for existing derived projectiles | Moderate |
| Bullet-style projectiles | Projectile | StraightProjectile where straight | Partial | Move direct straight projectiles under StraightProjectile or BasicStraightProjectile | Moderate |
| Grenade/rocket-style projectiles | Projectile | BallisticProjectile where arcing/explosive | Partial | Move arcing/explosive projectiles under BallisticProjectile | Moderate |
| Level | none | abstract Level | No | Make abstract and add concrete GameLevel preserving existing behavior | Moderate |
| Collectible | Entity | abstract Collectible | No | Make abstract and add concrete ItemCollectible preserving existing behavior | Moderate |
| InteractableObject | missing | abstract InteractableObject | No | Add base and move Prisoner under it | Safe |
| Prisoner | Entity | InteractableObject | No | Change base to InteractableObject | Safe |

## Ownership Findings

Composition relationships already mostly exist:
- `GameStateManager` owns and deletes its `GameState* currentState`.
- `EntityManager` owns all `Entity*` in its `DynamicArray` and deletes them in `clear`, `removeInactive`, and boss transient cleanup.
- `LevelManager` owns and deletes `currentLevel`.

Required fixes:
- `Soldier` does not currently own a `TransformationState`. Add `TransformationState* transformation` and delete/replace it safely.

Aggregation relationships needing clarification:
- `Soldier` currently does not expose a `Weapon*` strategy pointer. Add `Weapon* activeWeapon` as a non-owning aggregate. `PlayerSoldier` owns its concrete weapon member and passes its address to `Soldier`.
- `Enemy` currently has direct AI methods but no `EnemyAIState*`. Add non-owning `EnemyAIState* aiState`; `Enemy` does not delete it.

## Defendable Added Classes

These classes are planned because the UML marks existing behavior bases abstract while the current game still needs concrete playable objects:
- `GameLevel`: concrete implementation of abstract `Level`; preserves the existing map behavior.
- `ItemCollectible`: concrete implementation of abstract `Collectible`; preserves all current pickup behavior.
- `MetalSlug`: concrete `GroundVehicle`; preserves the current base tank behavior that was previously instantiated as `Vehicle`.
- `BasicProjectileWeapon`: concrete implementation of abstract `ProjectileWeapon`; preserves current bullet firing logic.
- `BasicStraightProjectile`: concrete implementation of abstract `StraightProjectile`; preserves current pistol bullet behavior.
- `NormalTransformationState`, `ZombieTransformationState`, `MummyTransformationState`: concrete state objects supporting the UML transformation state pattern.
- `PatrolAIState`, `AttackAIState`, `RetreatAIState`, `DeathFingerprintAIState`, `BossPhaseAIState`: defendable AI state strategy classes. Existing enemies can continue using direct overrides unless a state is assigned.

## Refactor Result

Implemented fixes:
- `Entity`, `DamageableEntity`, `Soldier`, `Enemy`, `Vehicle`, `Projectile`, `StraightProjectile`, `BallisticProjectile`, `Level`, `Collectible`, and `ProjectileWeapon` are now abstract.
- Direct runtime instantiations of abstract UML roots were replaced with `GameLevel`, `ItemCollectible`, `MetalSlug`, `BasicStraightProjectile`, and `BasicProjectileWeapon`.
- `GroundVehicle`, `AerialVehicle`, and `AquaticVehicle` now sit between `Vehicle` and the concrete vehicle types.
- `SlugFlyer` now inherits `AerialVehicle`; `SlugMariner` now inherits `AquaticVehicle`; `MetalSlug` inherits `GroundVehicle`.
- `Prisoner` now inherits `InteractableObject`.
- `Soldier` owns a `TransformationState` and deletes/replaces it in `Soldier`.
- `Soldier` aggregates `Weapon` through a non-owning `Weapon*`.
- `Enemy` aggregates `EnemyAIState` through a non-owning pointer and does not delete it.
- Visual Studio project files include the new architecture `.h` and `.cpp` files.

## Remaining Risk After Refactor

The refactor preserves existing gameplay by adding UML-compliant hooks rather than fully migrating all enemy and transformation behavior into state subclasses. This is defendable because the UML skeleton and ownership/aggregation arcs are present, while the existing concrete enemy overrides continue to preserve tested gameplay. A deeper future refactor could move more of `PlayerSoldier` zombie/mummy animation and each enemy's update logic into the new state objects, but that would be higher behavioral risk.

## Remaining Risk Before Refactor

The safest compliance path is to make UML roots abstract using pure virtual destructors with definitions. This avoids broad gameplay rewrites while satisfying abstract-class requirements. The riskiest edits are replacing direct instantiation of classes that must become abstract (`Level`, `Collectible`, `Vehicle`, `StraightProjectile`, `ProjectileWeapon`) with thin concrete subclasses.
