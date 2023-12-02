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
		virtual Component* Clone(Object* Owner) override;
		virtual void SetupClone(Component* Child) override;

		/** Set the location of this collision
		*	@param NewLoc: Location to set the collision
		*/
		virtual void SetCollisionLocation(const Vector<float>& NewLoc);

		/** Get the collision location */
		virtual Vector<float> GetCollisionLocation() const;

		/** Called when this component begins overlapping with another component
		*	@param OtherObject: Object that started overlapping with this component
		*	@param OtherComp: Component that started overlapping with this component
		*/
		virtual void OnCollisionOverlapBegin(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component ends overlapping with another component
		*	@param OtherObject: Object that ended overlapping with this component
		*	@param OtherComp: Component that ended overlapping with this component
		*/
		virtual void OnCollisionOverlapEnd(Object* OtherObject, CollisionComponent* OtherComp);

		/** Called when this component gets blocked by another component
		*	@param OtherObject: Object that hit this component
		*	@param OtherComp: Component that hit this component
		*/
		virtual void OnCollisionHit(Object* OtherObject, CollisionComponent* OtherComp);

		/** Is this component overlapping with a specific object
		*	@param OtherObject: Object to check
		*/
		bool IsOverlappingWith(Object* OtherObject) const;

		/** 
		*	Bind observer to this collision's subject.
		*/
		void BindOnCollisionOverlapBegin(IObserver<std::unordered_map<std::string, void*>>* O);
		void BindOnCollisionOverlapEnd(IObserver<std::unordered_map<std::string, void*>>* O);
		void BindOnCollisionHit(IObserver<std::unordered_map<std::string, void*>>* O);

		/**
		*	Unbind observer to this collision's subject.
		*/
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
		/** Set the collision type that represents the shape of the collision
		*	@param Type: Type to use
		*/
		inline void SetCollisionType(const ECollisionShape& Type) { mCollisionType = Type; }

		/** Get the collision type of this component */
		inline ECollisionShape GetCollisionType() const { return mCollisionType; }

		/** Get the collision response to a specific collision channel
		*	@param Channel: Channel to check
		*/
		ECollisionResponse GetCollisionResponseToChannel(const ECollisionChannel& Channel);

		/** Set the collision response to a specific collision channel.
		*	@param Channel: Channel to set the response
		*	@param Response: Collision response to set to the specified collision channel
		*/
		void SetCollisionResponseToChannel(const ECollisionChannel& Channel, const ECollisionResponse& Response);
		
		/** Set the collision response to all collision channels.
		*	@param Response: Collision response to set to all collision channels
		*/
		void SetCollisionResponseToAllChannels(const ECollisionResponse& Response);

		/** Set the channel to use for this collision
		*	@param Channel: Channel to use
		*/
		inline void SetCollisionChannel(const ECollisionChannel& Channel) { mCollisionChannel = Channel; }

		/** Get the collision channel that this component uses */
		inline ECollisionChannel GetCollisionChannel() const { return mCollisionChannel; }

		/** Add an object to the overlapping list
		*	@param Obj: Object to add to the overlapping list
		*/
		void AddOverlappingObject(Object* Obj);

		/** Remove an object from the overlapping list
		*	@param Obj: Object to remove to the overlapping list
		*/
		void RemoveOverlappingObject(Object* Obj);

		/** Get a list of all the current overlapping objects */
		inline std::vector<Object*> GetOverlappingObjects() const { return mOverlappingObjects; }

		/** Get a map representing the collision responses to all the different collision channels */
		inline std::unordered_map<ECollisionChannel, ECollisionResponse> GetCollisionResponseToChannels() const { return mCollisionResponseToChannels; }		
	};
}