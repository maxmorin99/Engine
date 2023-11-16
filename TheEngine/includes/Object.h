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

	public:
		inline std::string GetId() const { return mId; }

		/* Transform get/set -------------------------------------- */

		const Vector<float> GetLocation() const;
		void GetLocation(float* OutX, float* OutY);
		Vector<float> GetCenterLocation() const;
		void SetLocation(const Vector<float>& NewLoc);
		void SetLocation(float NewX, float NewY);
		void SetCenterLocation(const Vector<float>& NewLoc);
		const Vector<float> GetSize() const;
		void GetSize(float* OutW, float* OutH) const;
		void SetSize(const Vector<float>& NewSize);
		void SetSize(float NewW, float NewH);
		float GetRotation() const;
		void SetRotation(float NewRot);
		Vector<float> GetForwardVector() const;
		Vector<float> GetVelocity() const;
		void SetFlip(const Flip& InFlip);
		Flip GetFlip() const;
		void AddTag(const std::string& Tag);
		bool HasTag(const std::string& Tag) const;
		inline bool IsStarted() const { return bStarted; }
		inline std::vector<std::string> GetTags() const { return mTags; }
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
		if (mComponentsByType.count(Type) > 0)
		{
			return static_cast<T*>(mComponentsByType.at(Type));
		}
		return nullptr;
	}
}