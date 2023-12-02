#pragma once

namespace Core
{
	template<typename T>
	class IObserver
	{
	public:
		virtual ~IObserver() = default;


		/**
		* @brief Function called when the binded subjects calls Invoke().
		 */
		virtual void OnNotify(const T& Value) = 0;
	};
}