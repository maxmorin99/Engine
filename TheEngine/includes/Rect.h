
namespace Core
{
	template <class T>
	class Rect
	{
	private:
		T _X;
		T _Y;
		T _W;
		T _H;

	public:
		void SetPosition(T X, T InY);
		void SetSize(T W, T H);
		void GetPosition(T* OutX, T* OutY) const;
		void GetSize(T* OutW, T* OutH) const;
	};

	template<class T>
	inline void Rect<T>::SetPosition(T X, T Y)
	{
		_X = X;
		_Y = Y;
	}
	template<class T>
	inline void Rect<T>::SetSize(T W, T H)
	{
		_W = W;
		_H = H;
	}
	template<class T>
	inline void Rect<T>::GetPosition(T* OutX, T* OutY) const
	{
		*OutX = _X;
		*OutY = _Y;
	}
	template<class T>
	inline void Rect<T>::GetSize(T* OutW, T* OutH) const
	{
		*OutW = _W;
		*OutH = _H;
	}
}