#pragma once

#include "Component.h"
#include <vector>
#include <functional>


namespace Core
{
	template <typename T1, typename T2>
	class SubjectTwoParams;

	template <typename T1, typename T2>
	class IObserverTwoParams;

	enum class ECollisionEvent : uint8_t
	{
		Begin = 0,
		End = 1,

		MAX
	};

	class CollisionComponent : public Component
	{
	public:
		CollisionComponent(Object* InOwner);
		virtual void Destroy() override;

		/** Called when this component begins overlapping with another component */
		virtual void OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component ends overlapping with another component */
		virtual void OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component gets blocked by another component */
		virtual void OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp);

		bool IsOverlappingWith(Object* OtherObject) const;

		void BindOnCollisionOverlapBegin(IObserverTwoParams<Object*, CollisionComponent*>* O);
		void BindOnCollisionOverlapEnd(IObserverTwoParams<Object*, CollisionComponent*>* O);
		void BindOnCollisionHit(IObserverTwoParams<Object*, CollisionComponent*>* O);
		void UnBindOnCollisionOverlapBegin(IObserverTwoParams<Object*, CollisionComponent*>* O);
		void UnBindOnCollisionOverlapEnd(IObserverTwoParams<Object*, CollisionComponent*>* O);
		void UnBindOnCollisionHit(IObserverTwoParams<Object*, CollisionComponent*>* O);
		

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

		SubjectTwoParams<Object*, CollisionComponent*>* mSubjectOnCollisionOverlapBegin = nullptr;
		SubjectTwoParams<Object*, CollisionComponent*>* mSubjectOnCollisionOverlapEnd = nullptr;
		SubjectTwoParams<Object*, CollisionComponent*>* mSubjectOnCollisionHit = nullptr;

	public:
		inline void SetCollisionType(const ECollisionShape& Type) { mCollisionType = Type; }
		inline ECollisionShape GetCollisionType() const { return mCollisionType; }
		ECollisionResponse GetCollisionResponseToChannel(const ECollisionChannel& Channel);
		void AddCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response);
		inline void SetCollisionChannel(const ECollisionChannel& Channel) { mCollisionChannel = Channel; }
		inline ECollisionChannel GetCollisionChannel() const { return mCollisionChannel; }
		void AddOverlappingObject(Object* Obj);
		inline std::vector<Object*> GetOverlappingObjects() const { return mOverlappingObjects; }
		inline std::unordered_map<ECollisionChannel, ECollisionResponse> GetCollisionResponseToChannels() const { return mCollisionResponseToChannels; }
	};
}