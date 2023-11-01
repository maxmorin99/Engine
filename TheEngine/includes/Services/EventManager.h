#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>



namespace Core
{
	struct FunctionNoParams
	{
		size_t Id;
		FunctionPtrNoParams FunPtr;
	};

	using FunctionPtrNoParams = std::function<void(void)>;
	using FunctionList = std::vector<FunctionNoParams>;

	class EventManager
	{
		static size_t sNextFunId;
	public:
		static void Register(const std::string& EventName, FunctionPtrNoParams FunPtr);
		static void UnRegister(const std::string& EventName, FunctionPtrNoParams FunPtr);
		static void Invoke(const std::string& EventName);


	private:
		EventManager();
		static EventManager* GetInstance();

		static EventManager* mInstance;
		std::unordered_map<std::string, FunctionList> mEvents;
	};
}