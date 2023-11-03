//#include "Services/EventManager.h"
//
//
//Core::EventManager* Core::EventManager::mInstance = nullptr;
//size_t Core::EventManager::sNextFunId = 0;
//
//void Core::EventManager::Register(const std::string& EventName, FunctionPtrNoParams FunPtr)
//{
//    if (GetInstance()->mEvents.count(EventName) == 0)
//    {
//        FunctionList FunList;
//        FunctionNoParams FunPtrData;
//        
//        FunPtrData.Id = sNextFunId++;
//        FunList.push_back(FunPtrData);
//
//        GetInstance()->mEvents.emplace(EventName, FunList);
//    }
//    else
//    {
//        FunctionList FunList = GetInstance()->mEvents[EventName];
//        FunctionNoParams FunPtrData;
//
//        FunPtrData.Id = sNextFunId++;
//        FunList.push_back(FunPtrData);
//    }
//}
//
//void Core::EventManager::UnRegister(const std::string& EventName, FunctionPtrNoParams FunPtr)
//{
//    if (GetInstance()->mEvents.count(EventName) == 0) return;
//    FunctionList FunList = GetInstance()->mEvents[EventName];
//    
//    for (int i = 0; i < FunList.size(); i++)
//    {
//        FunctionNoParams F = FunList[i];
//        std::function<void()> P = F.FunPtr;
//        std::function<void()> P2 = FunPtr;
//        //if(F.FunPtr == FunPtr)
//    }
//}
//
//void Core::EventManager::Invoke(const std::string& EventName)
//{
//}
//
//Core::EventManager* Core::EventManager::GetInstance()
//{
//    if (!mInstance)
//    {
//        mInstance = new EventManager();
//    }
//    return mInstance;
//}
