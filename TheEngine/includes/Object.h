#pragma once

#include <string>

namespace Core
{
	class World;
	class IInput;

	class Object
	{
	public:
		virtual ~Object() = default;
		virtual void Init() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void Destroy() {};

	protected:
		std::string mId = "0";


		World* GetWorld() const;
		IInput* Input() const;

	public:
		inline void SetId(const std::string& Id) { mId = Id; }
		inline std::string GetId() const { return mId; }
	};
}