#pragma once
namespace AliceMathF
{
	template<typename T>
	//4次元ベクトル
	class Vector4T
	{
	public:
		T x;//x成分
		T y;//y成分
		T z;//z成分
		T w;//w

	public:
		//コンストラクタ
		Vector4T();//零ベクトル
		Vector4T(T x_, T y_, T z_, T w_);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector4T<T>& operator=(const Vector4T<T>& vec_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector4T<T>& operator=(Vector4T<T>& vec_);
	};

	template<typename T>
	inline Vector4T<T>::Vector4T()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
		w = static_cast<T>(0);
	}

	template<typename T>
	inline Vector4T<T>::Vector4T(T x_, T y_, T z_, T w_) : x(x_), y(y_), z(z_), w(w_)
	{
	}

	template<typename T>
	inline Vector4T<T>& Vector4T<T>::operator=(const Vector4T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
		w = vec_.w;

		return *this;
	}

	template<typename T>
	inline const Vector4T<T>& Vector4T<T>::operator=(Vector4T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;
		w = vec_.w;

		return *this;
	}

	//3次元ベクトル
	template<typename T>
	class Vector3T
	{
	public:
		T x;//x成分
		T y;//y成分
		T z;//z成分

	public:
		//コンストラクタ
		Vector3T();//零ベクトル
		Vector3T(T x_, T y, T z);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector3T<T>& operator=(const Vector3T<T>& vec_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector3T<T>& operator=(Vector3T<T>& vec_);

	};

	template<typename T>
	inline Vector3T<T>::Vector3T()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}

	template<typename T>
	inline Vector3T<T>::Vector3T(T x_, T y_, T z_) : x(x_), y(y_), z(z_)
	{
	}

	template<typename T>
	inline Vector3T<T>& Vector3T<T>::operator=(const Vector3T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;

		return *this;
	}

	template<typename T>
	inline const Vector3T<T>& Vector3T<T>::operator=(Vector3T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;
		z = vec_.z;

		return *this;
	}

	template<typename T>
	class Vector2T
	{
	public:
		T x;//x成分
		T y;//y成分

	public:
		//コンストラクタ
		Vector2T();//零ベクトル
		Vector2T(T x_, T y_);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector2T<T>& operator=(const Vector2T<T>& vec_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector2T<T>& operator=(Vector2T<T>& vec_);
	};

	template<typename T>
	inline Vector2T<T>::Vector2T()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
	}

	template<typename T>
	inline Vector2T<T>::Vector2T(T x_, T y_) : x(x_), y(y_)
	{
	}

	template<typename T>
	inline Vector2T<T>& Vector2T<T>::operator=(const Vector2T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;

		return *this;
	}

	template<typename T>
	inline const Vector2T<T>& Vector2T<T>::operator=(Vector2T<T>& vec_)
	{
		x = vec_.x;
		y = vec_.y;

		return *this;
	}
}

