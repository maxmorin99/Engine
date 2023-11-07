#pragma once

#include "Component.h"
#include <vector>
#include <functional>


namespace Core
{
	template <typename T1, typename T2, typename T3>
	class SubjectThreeParams;

	template <typename T1, typename T2, typename T3>
	class IObserverThreeParams;

	enum class ECollisionEvent : uint8_t
	{
		Begin = 0,
		End = 1,

		MAX
	};

	enum class ECollisionDirection : uint8_t
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,

		MAX
	};

	class CollisionComponent : public Component
	{
	public:
		CollisionComponent(Object* InOwner);
		virtual void Start() override;
		virtual void Destroy() override;
		virtual void SetCollisionLocation(const Vector<float>& NewLoc);

		/** Called when this component begins overlapping with another component */
		virtual void OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp, const Vector<float>& CollisionPoint);

		/** Called when this component ends overlapping with another component */
		virtual void OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component gets blocked by another component */
		virtual void OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp, const Vector<float>& CollisionPoint);

		bool IsOverlappingWith(Object* OtherObject) const;

		void BindOnCollisionOverlapBegin(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		void BindOnCollisionOverlapEnd(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		void BindOnCollisionHit(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		void UnBindOnCollisionOverlapBegin(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		void UnBindOnCollisionOverlapEnd(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		void UnBindOnCollisionHit(IObserverThreeParams<Object*, CollisionComponent*, const Vector<float>&>* O);
		

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

		SubjectThreeParams<Object*, CollisionComponent*, const Vector<float>&>* mSubjectOnCollisionOverlapBegin = nullptr;
		SubjectThreeParams<Object*, CollisionComponent*, const Vector<float>&>* mSubjectOnCollisionOverlapEnd = nullptr;
		SubjectThreeParams<Object*, CollisionComponent*, const Vector<float>&>* mSubjectOnCollisionHit = nullptr;

		Vector<float> mCollisionPoint = Vector<float>::ZeroVector();

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