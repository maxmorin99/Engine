#pragma once

#include "Engine/Core.h"
#include <string>
#include <unordered_map>

namespace Core
{
	class Component;
	class TransformComponent;

	class Object final
	{
		static size_t sId;
		static std::string sName;

	public:
		Object();
		~Object() = default;
		void Start();
		void Update(float DeltaTime);
		void Render();
		void Destroy();
		void UseOldLocation();
		Object* Clone();

		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent() const;

		CollisionComponent* GetCollisionComponent() const;

	private:
		std::string mId = "\0";

		std::vector<std::string> mTags;

		bool bStarted = false;


		/* Components --------------------------------------------- */

		TransformComponent* mTransform = nullptr;


		std::vector<Component*> mComponents;
		std::vector<IDrawable*> mDrawable;
		std::vector<IUpdatable*> mUpdatable;
		std::unordered_map<const type_info*, Component*> mComponentsByType;

		size_t GenerateId();
		void AddComponent(Component* comp);

	public:
		inline std::string GetId() const { return mId; }
		

		/**
		* @brief Gets the location of the object.
		*
		* @return The current location of the object as a Vector<float>.
		*/
		const Vector<float> GetLocation() const;

		/**
		* @brief Gets the location of the object.
		*
		* @param OutX A pointer to store the X-coordinate of the object's location.
		* @param OutY A pointer to store the Y-coordinate of the object's location.
		*/
		void GetLocation(float* OutX, float* OutY);

		/**
		* @brief Gets the center location of the object's sprite.
		*
		* @return The center location of the object's sprite as a Vector<float>.
		*/
		Vector<float> GetSpriteCenterLocation() const;

		/**
		* @brief Sets the location of the object.
		*
		* @param NewLoc The new location to set for the object.
		*/
		void SetLocation(const Vector<float>& NewLoc);

		/**
		* @brief Sets the location of the object.
		*
		* @param NewX The new X-coordinate for the object's location.
		* @param NewY The new Y-coordinate for the object's location.
		*/
		void SetLocation(float NewX, float NewY);

		/**
		* @brief Sets the center location of the object.
		*
		* @param NewLoc The new center location to set for the object.
		*/
		void SetCenterLocation(const Vector<float>& NewLoc);

		/**
		* @brief Sets the top-left location of the object's image.
		*
		* @param NewLoc The new top-left location to set for the object's image.
		*/
		void SetImgTopLeftLocation(const Vector<float>& NewLoc);

		/**
		* @brief Gets the size of the object.
		*
		* @return The current size of the object as a Vector<float>.
		*/
		const Vector<float> GetSize() const;

		/**
		* @brief Gets the size of the object.
		*
		* @param OutW A pointer to store the width of the object.
		* @param OutH A pointer to store the height of the object.
		*/
		void GetSize(float* OutW, float* OutH) const;

		/**
		* @brief Sets the size of the object.
		*
		* @param NewSize The new size to set for the object.
		*/
		void SetSize(const Vector<float>& NewSize);

		/**
		* @brief Sets the size of the object.
		*
		* @param NewW The new width for the object.
		* @param NewH The new height for the object.
		*/
		void SetSize(float NewW, float NewH);

		/**
		* @brief Gets the rotation angle of the object.
		*
		* @return The current rotation angle of the object.
		*/
		float GetRotation() const;

		/**
		* @brief Sets the rotation angle of the object.
		*
		* @param NewRot The new rotation angle to set for the object.
		*/
		void SetRotation(float NewRot);

		/**
		* @brief Gets the forward vector of the object based on its rotation.
		*
		* @return The forward vector as a Vector<float>.
		*/
		Vector<float> GetForwardVector() const;

		/**
		* @brief Gets the velocity vector of the object.
		*
		* @return The current velocity of the object as a Vector<float>.
		*/
		Vector<float> GetVelocity() const;

		/**
		* @brief Sets the flip of the object.
		*
		* @param InFlip The flip settings to apply to the object.
		*/
		void SetFlip(const Flip& InFlip);

		/**
		* @brief Gets the flip of the object.
		*
		* @return The current flip settings of the object.
		*/
		Flip GetFlip() const;

		/**
		* @brief Adds a tag to the object.
		*
		* @param Tag The tag to add to the object.
		*/
		void AddTag(const std::string& Tag);

		/**
		* @brief Checks if the object has a specific tag.
		*
		* @param Tag The tag to check for.
		* @return A boolean indicating whether the object has the specified tag.
		*/
		bool HasTag(const std::string& Tag) const;

		/**
		* @brief Checks if the object has started.
		*
		* @return A boolean indicating whether the object has started.
		*/
		inline bool IsStarted() const { return bStarted; }

		/**
		* @brief Gets all tags associated with the object.
		*
		* @return A vector of strings representing the tags of the object.
		*/
		inline std::vector<std::string> GetTags() const { return mTags; }

		/**
		* @brief Gets the offset from the center of the object.
		*
		* @return The offset from the center as a Vector<float>.
		*/
		Vector<float> GetCenterOffset() const;

		/**
		* @brief Gets the center location of the object.
		*
		* @return The center location of the object as a Vector<float>.
		*/
		Vector<float> GetCenterLocation() const;
	};


	/* Template function definitions -------------------------- */

	template<typename T>
	inline T* Object::AddComponent()
	{
		Component* NewComponent = new T(this);
		mComponents.push_back(NewComponent);

		IUpdatable* Updatable = dynamic_cast<IUpdatable*>(NewComponent);
		if (Updatable)
		{
			mUpdatable.push_back(Updatable);
		}
		IDrawable* Drawable = dynamic_cast<IDrawable*>(NewComponent);
		if (Drawable)
		{
			mDrawable.push_back(Drawable);
		}

		const type_info* Type = &typeid(*NewComponent);
		mComponentsByType.emplace(Type, NewComponent);
		return static_cast<T*>(NewComponent);
	}

	template<typename T>
	inline T* Object::GetComponent() const
	{
		const type_info* Type = &(typeid(T));
		if (!Type) return nullptr;
		if (mComponentsByType.count(Type) > 0)
		{
			return static_cast<T*>(mComponentsByType.at(Type));
		}
		return nullptr;
	}
}