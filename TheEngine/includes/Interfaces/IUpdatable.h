#pragma once

namespace Core
{
	class IUpdatable
	{
	public:
		
		/**
		* @brief Called for updating the object that implements this interface.
		*/
		virtual void Update(float DeltaTime) = 0;
	};
}