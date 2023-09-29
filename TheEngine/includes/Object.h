#pragma once

#include "Engine/Core.h"
#include <string>
#include <unordered_map>

namespace Core
{
	class IWorld;
	class IInput;
	class ILogger;
	class IGraphic;
	class Component;

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
		void AddComponent();

		template<typename T>
		Component* GetComponent() const;

	private:
		std::string mId = "\0";
		Vector2D<float> mLocation = Vector2D<float>::ZeroVectorF;
		std::vector<Component*> mComponents;
		std::vector<IDrawable*> mDrawable;
		std::vector<IUpdatable*> mUpdatable;
		std::unordered_map<const type_info*, Component*> mComponentsByType;

		IWorld& GetWorld() const;
		IInput& GetInput() const;
		ILogger& GetLogger() const;
		IGraphic& GetGraphic() const;

		size_t GenerateId();

	public:
		inline void SetId(const std::string& Id) { mId = Id; }
		inline std::string GetId() const { return mId; }
		inline void SetLocation(const Vector2D<float>& InLocation) { mLocation = InLocation; }
		void GetLocation(Vector2D<float>& outLocation) const;
	};

	template<typename T>
	inline void Object::AddComponent()
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
	}

	template<typename T>
	inline Component* Object::GetComponent() const
	{
		const type_info* Type = &(typeid(T));
		if (mComponentsByType.count(Type) > 0)
		{
			return mComponentsByType.at(Type);
		}
		return nullptr;
	}
}