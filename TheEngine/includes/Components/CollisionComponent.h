#pragma once

#include "Component.h"
#include <vector>
#include <functional>
#include <string>
#include "Interfaces/IUpdatable.h"


namespace Core
{
	template <typename T>
	class Subject;

	template <typename T>
	class IObserver;

	enum class ECollisionEvent : uint8_t
	{
		Begin = 0,
		End = 1,

		MAX
	};
	

	class CollisionComponent : public Component, public IUpdatable
	{
	public:
		CollisionComponent(Object* InOwner);
		virtual void Start() override;
		virtual void Update(float DeltaTime) override;
		virtual void Destroy() override;
		virtual void SetCollisionLocation(const Vector<float>& NewLoc);
		virtual Vector<float> GetCollisionLocation() const;

		/** Called when this component begins overlapping with another component */
		virtual void OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component ends overlapping with another component */
		virtual void OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component gets blocked by another component */
		virtual void OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp);

		bool IsOverlappingWith(Object* OtherObject) const;

		void BindOnCollisionOverlapBegin(IObserver<std::unordered_map<std::string, void*>>* O);
		void BindOnCollisionOverlapEnd(IObserver<std::unordered_map<std::string, void*>>* O);
		void BindOnCollisionHit(IObserver<std::unordered_map<std::string, void*>>* O);
		void UnBindOnCollisionOverlapBegin(IObserver<std::unordered_map<std::string, void*>>* O);
		void UnBindOnCollisionOverlapEnd(IObserver<std::unordered_map<std::string, void*>>* O);
		void UnBindOnCollisionHit(IObserver<std::unordered_map<std::string, void*>>* O);
		

	protected:
		/** Shape of the collision */
		ECollisionShape mCollisionType = ECollisionShape::Rectangle;

		/** Collision channel of this collision component */
		ECollisionChannel mCollisionChannel;

		/** Map of the collision response by channels. 
		*	Ex: [Player][Block], [Ennemy][Ignore]
		*/
		std::unordered_map<ECollisionChannel, ECollisionResponse> mCollisionResponseToChannels;

		/** List of the last frame overlapping objects with this component */
		std::vector<Object*> mOverlappingObjects;

		/** List of the current overlapping objects with this component */
		std::vector<Object*> mCurrentlyOverlappingObjects;

		Subject<std::unordered_map<std::string, void*>>* mSubjectOnCollisionOverlapBegin = nullptr;
		Subject<std::unordered_map<std::string, void*>>* mSubjectOnCollisionOverlapEnd = nullptr;
		Subject<std::unordered_map<std::string, void*>>* mSubjectOnCollisionHit = nullptr;

	public:
		inline void SetCollisionType(const ECollisionShape& Type) { mCollisionType = Type; }
		inline ECollisionShape GetCollisionType() const { return mCollisionType; }
		ECollisionResponse GetCollisionResponseToChannel(const ECollisionChannel& Channel);
		void AddCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response);
		inline void SetCollisionChannel(const ECollisionChannel& Channel) { mCollisionChannel = Channel; }
		inline ECollisionChannel GetCollisionChannel() const { return mCollisionChannel; }
		void AddOverlappingObject(Object* Obj);
		void RemoveOverlappingObject(Object* Obj);
		inline std::vector<Object*> GetOverlappingObjects() const { return mOverlappingObjects; }
		inline std::unordered_map<ECollisionChannel, ECollisionResponse> GetCollisionResponseToChannels() const { return mCollisionResponseToChannels; }		
	};
}