#pragma once

namespace Core
{
	template<typename T>
	class IObserverOneParam
	{
	public:
		virtual ~IObserverOneParam() = default;
		virtual void OnNotify(const T& Value) = 0;
	};

	template<typename T1, typename T2>
	class IObserverTwoParams
	{
	public:
		virtual ~IObserverTwoParams() = default;
		virtual void OnNotify(const T1& Value1, const T2& Value2) = 0;
	};

	template<typename T1, typename T2, typename T3>
	class IObserverThreeParams
	{
	public:
		virtual ~IObserverThreeParams() = default;
		virtual void OnNotify(const T1& Value1, const T2& Value2, const T3& Value3) = 0;
	};
}