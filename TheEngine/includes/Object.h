#pragma once

namespace Core
{
	class World;

	class Object
	{
	public:
		virtual ~Object() = default;
		virtual void Init() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void Destroy() {};

	protected:
		const char* mId = "0";

		World* GetWorld() const;

	public:
		inline void SetId(const char* Id) { mId = Id; }
		inline const char* GetId() const { return mId; }
	};
}