# UML Compliance Checklist

## Abstract Classes

All required UML abstract classes now have at least one pure virtual function, usually a pure virtual destructor with a definition to preserve shared behavior:

| Abstract class | Pure virtual member |
|---|---|
| Entity | `virtual ~Entity() = 0` |
| DamageableEntity | `virtual ~DamageableEntity() = 0` |
| Soldier | `virtual ~Soldier() = 0` |
| TransformationState | `virtual ~TransformationState() = 0`, plus `enter`, `update`, `exit` |
| Enemy | `virtual ~Enemy() = 0` |
| EnemyAIState | `virtual ~EnemyAIState() = 0`, plus `update` |
| Vehicle | `virtual ~Vehicle() = 0` |
| GroundVehicle | `virtual ~GroundVehicle() = 0` |
| AerialVehicle | `virtual ~AerialVehicle() = 0` |
| AquaticVehicle | `virtual ~AquaticVehicle() = 0` |
| Projectile | `virtual ~Projectile() = 0` |
| StraightProjectile | `virtual ~StraightProjectile() = 0` |
| BallisticProjectile | `virtual ~BallisticProjectile() = 0` |
| Level | `virtual ~Level() = 0` |
| Collectible | `virtual ~Collectible() = 0` |
| InteractableObject | `virtual ~InteractableObject() = 0`, plus `interact` |
| Weapon | `virtual void fire(...) = 0` |
| ProjectileWeapon | `virtual ~ProjectileWeapon() = 0` |
| GameState | `handleInput`, `update`, `draw` |

## Concrete Classes

Concrete classes required by gameplay remain instantiable:
- `Game`, `GameStateManager`, `PlayState`, `EntityManager`, `LevelManager`
- `GameLevel`
- `PlayerSoldier`
- `RebelSoldier`, `ShieldedSoldier`, `BazookaSoldier`, `GrenadeSoldier`, `ZombieEnemy`, `MummyEnemy`, `MartianInfantry`, `MartianPod`, `BossEnemy`
- `FlyingTara`, `M15ABradley`, `EnemySub`
- `MetalSlug`, `SlugFlyer`, `SlugMariner`
- `BasicStraightProjectile`, `VehicleBullet`, `EnemyBullet`, `EnemyDownwardBullet`, `EnemyStraightRocketProjectile`, `SlugFlyerRocketProjectile`
- `GrenadeProjectile`, `RocketProjectile`, `EnemyGrenadeProjectile`, `EnemyRocketProjectile`, `BallisticProjectile` subclasses in `VehicleSpecialProjectiles`, `MarinerMissile`
- `ItemCollectible`
- `Prisoner`
- `BasicProjectileWeapon`
- `NormalTransformationState`, `ZombieTransformationState`, `MummyTransformationState`
- `PatrolAIState`, `AttackAIState`, `RetreatAIState`, `DeathFingerprintAIState`, `BossPhaseAIState`

## Composition Relationships

| Owner | Owned object | Where deletion happens |
|---|---|---|
| GameStateManager | `GameState* currentState` | `GameStateManager::~GameStateManager` through `changeState`/destructor cleanup |
| EntityManager | `DynamicArray<Entity*> entities` | `clear`, `removeInactive`, `clearBossPhaseTransientEntities` |
| LevelManager | `Level* currentLevel` | `LevelManager::clear`, called by destructor and `loadLevel` |
| Soldier | `TransformationState* transformation` | `Soldier::~Soldier` and `Soldier::setTransformation` before replacement |

## Aggregation Relationships

| Aggregator | Aggregated object | Ownership confirmation |
|---|---|---|
| Soldier | `Weapon* activeWeapon` | Non-owning pointer; `Soldier` does not delete it. `PlayerSoldier` owns its concrete `BasicProjectileWeapon` member. |
| Enemy | `EnemyAIState* aiState` | Non-owning pointer; `Enemy` does not delete it. Existing enemy overrides remain the default when no state is assigned. |

## Inheritance Chains

- `Entity -> DamageableEntity -> Soldier -> PlayerSoldier`
- `Entity -> DamageableEntity -> Enemy -> concrete enemies`
- `Entity -> DamageableEntity -> Enemy -> EnemyVehicle -> GroundVehicleEnemy/AerialVehicleEnemy/AquaticVehicleEnemy`
- `Entity -> DamageableEntity -> Vehicle -> GroundVehicle -> MetalSlug`
- `Entity -> DamageableEntity -> Vehicle -> AerialVehicle -> SlugFlyer`
- `Entity -> DamageableEntity -> Vehicle -> AquaticVehicle -> SlugMariner`
- `Entity -> Projectile -> StraightProjectile -> BasicStraightProjectile`
- `Entity -> Projectile -> StraightProjectile -> straight projectile subclasses`
- `Entity -> Projectile -> BallisticProjectile -> ballistic projectile subclasses`
- `Entity -> Collectible -> ItemCollectible`
- `Entity -> InteractableObject -> Prisoner`
- `GameState -> PlayState`
- `Weapon -> ProjectileWeapon -> BasicProjectileWeapon`
- `Level -> GameLevel`

## Defendable Additions

| Added class | Parent class | Why it is necessary | Why it does not violate UML |
|---|---|---|---|
| GameLevel | Level | Concrete map implementation after `Level` became abstract | UML allows concrete level implementations below abstract `Level` |
| ItemCollectible | Collectible | Concrete pickup implementation after `Collectible` became abstract | Preserves collectible behavior under required root |
| MetalSlug | GroundVehicle | Replaces direct `Vehicle` instantiation for the player tank | Matches required vehicle tier |
| BasicStraightProjectile | StraightProjectile | Replaces direct straight bullet instantiation | Keeps straight bullets under required projectile tier |
| BasicProjectileWeapon | ProjectileWeapon | Existing projectile firing behavior needed a concrete strategy | Fits `Weapon -> ProjectileWeapon` path |
| Transformation state subclasses | TransformationState | Required for zombie/mummy state pattern | Directly implements UML state pattern |
| Enemy AI state subclasses | EnemyAIState | Required for enemy AI state pattern hooks | Directly implements UML strategy/state pattern |
| InteractableObject | Entity | Required UML root for interactables | Prisoner now uses it |

## Remaining Deviations

The codebase still keeps much of the detailed zombie/mummy animation and enemy-specific behavior inside `PlayerSoldier` and concrete enemy subclasses. The UML-compliant state objects and aggregation hooks are present, but behavior was not aggressively moved to avoid breaking working gameplay.

## Verification

- Full local syntax pass succeeded with `g++ -std=c++20 -Iinclude -fsyntax-only`.
- Full local object compilation succeeded for every root `.cpp` file.
- Search found no remaining direct `new Level`, `new Collectible`, `new Vehicle`, or `new StraightProjectile` usage.
