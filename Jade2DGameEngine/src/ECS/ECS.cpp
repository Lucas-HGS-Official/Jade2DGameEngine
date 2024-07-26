#include "ECS.h"
#include "../Logger/Logger.h"

int BaseComponent::nextId = 0;

///////////////////////////////////////////////////////////////////////////////////////////////
// Entity
///////////////////////////////////////////////////////////////////////////////////////////////
int Entity::GetId() const {

	return id;
}



///////////////////////////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////////////////////////
void System::AddEntityToSystem( Entity entity ) {
	
	entities.push_back( entity );
}


void System::RemoveEntityFromSystem( Entity entity ) {
	
	entities.erase(
		std::remove_if(
			entities.begin(),
			entities.end(),
			[&entity]( Entity other ) { return entity == other; }
			),
		entities.end()
		);
}


std::vector<Entity> System::GetSystemEntities() const {
	
	return entities;
}


const Signature& System::GetComponentSignature() const {
	
	return componentSignature;
}



///////////////////////////////////////////////////////////////////////////////////////////////
// Registry
///////////////////////////////////////////////////////////////////////////////////////////////
Entity Registry::CreateEntity() {

	int entityId;
	entityId = numEntities++;
	Entity entity( entityId );
	entity.registry = this;
	entitiesToBeAdded.insert( entity );

	if ( entityId >= entityComponentSignatures.size() ) {

		entityComponentSignatures.resize( entityId + 1 );
	}

	Logger::Log( "Entity created with id = " + std::to_string ( entityId ) );
	
	return entity;
}


void Registry::KillEntity( Entity entity ) {

	entitiesToBeKilled.insert( entity );
}


void Registry::AddEntityToSystems( Entity entity ) {

	const auto entityId = entity.GetId();
	const auto& entityComponentSignature = entityComponentSignatures[entityId];

	for ( auto& system: systems ) {

		const auto& systemComponentSignature = system.second->GetComponentSignature();
		
		bool isInterested = ( entityComponentSignature & systemComponentSignature ) == systemComponentSignature;
		if ( isInterested ) {

			system.second->AddEntityToSystem( entity );
		}
	}
}


void Registry::RemoveEntityFromSystems( Entity entity ) {

	for ( auto system: systems ) {

		system.second->RemoveEntityFromSystem( entity );
	}
}


void Registry::Update() {

	// Processing the entities to be created
	for ( auto entity: entitiesToBeAdded ) {

		AddEntityToSystems( entity );
	}

	entitiesToBeAdded.clear();


	for ( auto entity: entitiesToBeAdded ) {

		RemoveEntityFromSystems( entity );

		freeIds.push_back( entity.GetId() );
	}

	entitiesToBeKilled.clear();
}