#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

class MovementSystem: public System {

    public:
        MovementSystem() {

            RequireComponent<TransformComponent>();
            RequireComponent<RigidBodyComponent>();
        }


        void SubscribeToEvents( const std::unique_ptr<EventBus>& eventBus ) {

            eventBus->SubscribeToEvent<CollisionEvent>( this, &MovementSystem::OnCollision );
        }


        void OnCollision( CollisionEvent& event ) {

            Entity a = event.a;
            Entity b = event.b;

            if ( a.BelongsToGroup( "obstacles" ) && b.BelongsToGroup( "enemies" ) ) {

                OnEnemyHitsObstacle( b, a );
            }
            if ( b.BelongsToGroup( "obstacles" ) && a.BelongsToGroup( "enemies" ) ) {

                OnEnemyHitsObstacle( a, b );
            }

            Logger::Log( "Collision event emitted: " + std::to_string( a.GetId() ) + " and " + std::to_string( b.GetId() ) );
        }


        void OnEnemyHitsObstacle( Entity enemy, Entity obstacle ) {

            if ( enemy.HasComponent<RigidBodyComponent>() ) {

                auto& rigidbody = enemy.GetComponent<RigidBodyComponent>();

                if ( rigidbody.velocity.x != 0 ) {

                    rigidbody.velocity.x *= -1;
                }

                if ( rigidbody.velocity.y != 0 ) {

                    rigidbody.velocity.y *= -1;
                }
            }
        }


        void Update( double deltaTime ) {

            // Loop all entities that the system is interested in
            for ( auto entity: GetSystemEntities() ) {

                // Update entity position based on its velocity
                auto& transform = entity.GetComponent<TransformComponent>();
                const auto rigidbody = entity.GetComponent<RigidBodyComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime; 
                transform.position.y += rigidbody.velocity.y * deltaTime; 

                bool isEntityOutsideMap = ( 

                    transform.position.x < 0 ||
                    transform.position.x > Game::mapWidth ||
                    
                    transform.position.y < 0 ||
                    transform.position.y > Game::mapHeight
                 );
                
                // Kill all entities that move outside the map boundaries
                if ( isEntityOutsideMap && !entity.HasTag( "player" ) ) {

                    entity.Kill();
                }
            }
        }
};

#endif
