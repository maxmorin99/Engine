#pragma once

#include "Engine/Core.h"
#include <string>

namespace Core
{
	class World;
	class IInput;
	class ILogger;

	class Object
	{
	public:
		Object() = default;
		virtual ~Object() = default;
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void Draw() {};
		virtual void Destroy();

	protected:
		std::string mId = "0";


		World* GetWorld() const;
		IInput* GetInput() const;
		ILogger* GetLogger() const;

	public:
		inline void SetId(const std::string& Id) { mId = Id; }
		inline std::string GetId() const { return mId; }
	};
}