#pragma once

#include "Component.h"
#include <vector>


namespace Core
{
	class CollisionComponent : public Component
	{
	public:
		CollisionComponent(Object* InOwner);
		virtual void Destroy() override;

	protected:
		/** Shape of the collision */
		ECollisionShape mCollisionType = ECollisionShape::Rectangle;

		/** Collision channel of this collision component */
		ECollisionChannel mCollisionChannel;

		/** Map of the collision response by channels. 
		*	Ex: [Player][Block], [Ennemy][Ignore]
		*/
		std::unordered_map<ECollisionChannel, ECollisionResponse> mCollisionResponseToChannels;

		/** List of the current overlapping objects with this component */
		std::vector<Object*> mOverlappingObjects;

	public:
		inline void SetCollisionType(const ECollisionShape& Type) { mCollisionType = Type; }
		inline ECollisionShape GetCollisionType() const { return mCollisionType; }
		ECollisionResponse GetCollisionResponseToChannel(const ECollisionChannel& Channel);
		void AddCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response);
		inline void SetCollisionChannel(const ECollisionChannel& Channel) { mCollisionChannel = Channel; }
		inline ECollisionChannel GetCollisionChannel() const { return mCollisionChannel; }
		void AddOverlappingObjects(Object* Obj);
		inline std::vector<Object*> GetOverlappingObjects() const { return mOverlappingObjects; }
	};
}