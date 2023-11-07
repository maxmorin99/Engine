#pragma once

#include "Interfaces/IObserver.h"
#include <vector>
#include <iterator>

namespace Core
{
	template<typename T>
	class Subject
	{
	public:
		~Subject() = default;
		void AddListener(IObserver<T>* O);
		void RemoveListener(IObserver<T>* O);
		void Invoke(const T& Value);

	private:
		std::vector<IObserver<T>*> mObservers;
	};

	//template<typename T>
	//class SubjectOneParam
	//{
	//public:
	//	~SubjectOneParam() = default;
	//	void AddListener(IObserverOneParam<T>* O);
	//	void RemoveListener(IObserverOneParam<T>* O);
	//	void Invoke(const T& Value);

	//private:
	//	std::vector<IObserverOneParam<T>*> mObservers;
	//};

	//template<typename T1, typename T2>
	//class SubjectTwoParams
	//{
	//public:
	//	~SubjectTwoParams() = default;
	//	void AddListener(IObserverTwoParams<T1, T2>* O);
	//	void RemoveListener(IObserverTwoParams<T1, T2>* O);
	//	void Invoke(const T1& Value1, const T2& Value2);

	//private:
	//	std::vector<IObserverTwoParams<T1, T2>*> mObservers;
	//};

	//template<typename T1, typename T2, typename T3>
	//class SubjectThreeParams
	//{
	//public:
	//	~SubjectThreeParams() = default;
	//	void AddListener(IObserverThreeParams<T1, T2, T3>* O);
	//	void RemoveListener(IObserverThreeParams<T1, T2, T3>* O);
	//	void Invoke(const T1& Value1, const T2& Value2, const T3& Value3);

	//private:
	//	std::vector<IObserverThreeParams<T1, T2, T3>*> mObservers;
	//};


	//// One param ------------------------------------------------------------------------------------------- //

	//template<typename T>
	//inline void SubjectOneParam<T>::AddListener(IObserverOneParam<T>* O)
	//{
	//	mObservers.push_back(O);
	//}
	//template<typename T>
	//inline void SubjectOneParam<T>::RemoveListener(IObserverOneParam<T>* O)
	//{
	//	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), O), mObservers.end());
	//}
	//template<typename T>
	//inline void SubjectOneParam<T>::Invoke(const T& Value)
	//{
	//	for (IObserverOneParam<T>* O : mObservers)
	//	{
	//		O->OnNotify(Value);
	//	}
	//}


	//// Two params ------------------------------------------------------------------------------------------ //

	//template<typename T1, typename T2>
	//inline void SubjectTwoParams<T1, T2>::AddListener(IObserverTwoParams<T1, T2>* O)
	//{
	//	mObservers.push_back(O);
	//}
	//template<typename T1, typename T2>
	//inline void SubjectTwoParams<T1, T2>::RemoveListener(IObserverTwoParams<T1, T2>* O)
	//{
	//	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), O), mObservers.end());
	//}
	//template<typename T1, typename T2>
	//inline void SubjectTwoParams<T1, T2>::Invoke(const T1& Value1, const T2& Value2)
	//{
	//	for (IObserverTwoParams<T1, T2>* O : mObservers)
	//	{
	//		O->OnNotify(Value1, Value2);
	//	}
	//}


	//// Three params ---------------------------------------------------------------------------------------- //

	//template<typename T1, typename T2, typename T3>
	//inline void SubjectThreeParams<T1, T2, T3>::AddListener(IObserverThreeParams<T1, T2, T3>* O)
	//{
	//	mObservers.push_back(O);
	//}
	//template<typename T1, typename T2, typename T3>
	//inline void SubjectThreeParams<T1, T2, T3>::RemoveListener(IObserverThreeParams<T1, T2, T3>* O)
	//{
	//	mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), O), mObservers.end());
	//}
	//template<typename T1, typename T2, typename T3>
	//inline void SubjectThreeParams<T1, T2, T3>::Invoke(const T1& Value1, const T2& Value2, const T3& Value3)
	//{
	//	for (IObserverThreeParams<T1, T2, T3>* O : mObservers)
	//	{
	//		O->OnNotify(Value1, Value2, Value3);
	//	}
	//}

	// Subject
	template<typename T>
	inline void Subject<T>::AddListener(IObserver<T>* O)
	{
		mObservers.push_back(O);
	}
	template<typename T>
	inline void Subject<T>::RemoveListener(IObserver<T>* O)
	{
		mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), O), mObservers.end());
	}
	template<typename T>
	inline void Subject<T>::Invoke(const T& Value)
	{
		for (IObserver<T>* O : mObservers)
		{
			O->OnNotify(Value);
		}
	}
}