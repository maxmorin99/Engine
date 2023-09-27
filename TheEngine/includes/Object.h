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

	class Object
	{
	private:
		static size_t sId;
		static std::string sName;
	public:
		Object();
		virtual ~Object() = default;
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void Render();
		virtual void Destroy();

	private:
		std::string mId = "\0";
		std::string mTexturePath;
		size_t mTextureId = -1;
		Vector2D<float> mLocation = Vector2D<float>::ZeroVectorF;
		float mW = 0.f;
		float mH = 0.f;

		IWorld& GetWorld() const;
		IInput& GetInput() const;
		ILogger& GetLogger() const;
		IGraphic& GetGraphic() const;

		size_t GenerateId();

	public:
		inline void SetId(const std::string& Id) { mId = Id; }
		inline std::string GetId() const { return mId; }
		inline void SetTexturePath(const std::string& InPath) { mTexturePath = InPath; }
		inline std::string GetTexturePath() const { return mTexturePath; }
		inline void SetLocation(const Vector2D<float>& InLocation) { mLocation = InLocation; }
		void GetLocation(Vector2D<float>& outLocation) const;
		inline void SetSize(float Width, float Height) { mW = Width, mH = Height; }
		void GetSize(float* OutW, float* OutH) const;
	};
}