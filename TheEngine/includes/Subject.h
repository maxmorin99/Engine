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

		/**
		* @brief Adds an observer to the list of registered observers.
		*
		* @param O A pointer to the observer to be added.
		*/
		void AddListener(IObserver<T>* O);

		/**
		* @brief Removes an observer from the list of registered observers.
		*
		* @param O A pointer to the observer to be removed.
		*/
		void RemoveListener(IObserver<T>* O);

		/**
		* @brief Invokes the notification for all registered observers with the specified value.
		*
		* @param Value The value to notify observers with.
		*/
		void Invoke(const T& Value);

	private:
		std::vector<IObserver<T>*> mObservers;
	};
	

	/**
	* @brief Adds the specified observer to the list of registered observers.
	*
	* @param O A pointer to the observer to be added.
	*/
	template<typename T>
	inline void Subject<T>::AddListener(IObserver<T>* O)
	{
		mObservers.push_back(O);
	}

	/**
	* @brief Removes the specified observer from the list of registered observers.
	*
	* @param O A pointer to the observer to be removed.
	*/
	template<typename T>
	inline void Subject<T>::RemoveListener(IObserver<T>* O)
	{
		mObservers.erase(std::remove(mObservers.begin(), mObservers.end(), O), mObservers.end());
	}

	/**
	* @brief Invokes the notification for all registered observers with the specified value.
	*
	* @param Value The value to notify observers with.
	*/
	template<typename T>
	inline void Subject<T>::Invoke(const T& Value)
	{
		for (IObserver<T>* O : mObservers)
		{
			O->OnNotify(Value);
		}
	}
}