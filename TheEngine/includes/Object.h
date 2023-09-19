#pragma once

namespace Core
{
	class Object
	{
	public:
		virtual ~Object() = default;
		virtual void Init() {};
		virtual void Start() {};
		virtual void Update(float deltaTime) {};
		virtual void Destroy() {};

	protected:
		size_t mId = 0;

	public:
		inline void SetId(const size_t& Id) { mId = Id; }
	};
}