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

		template<typename T>
		T* AddComponent();

		template<typename T>
		T* GetComponent() const;

	private:
		std::string mId = "\0";

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
		void SetLocation(const Vector<float>& NewLoc);
		void SetLocation(float NewX, float NewY);
		const Vector<int> GetSize() const;
		void GetSize(int* OutW, int* OutH) const;
		void SetSize(const Vector<int>& NewSize);
		void SetSize(int NewW, int NewH);
		float GetRotation() const;
		void SetRotation(float NewRot);
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