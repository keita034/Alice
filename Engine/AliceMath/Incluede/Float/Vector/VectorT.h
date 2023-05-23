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
		Vector4T(T x, T y, T z, T w);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector4T<T>& operator=(const Vector4T<T>& vec);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector4T<T>& operator=(Vector4T<T>& vec);
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
	inline Vector4T<T>::Vector4T(T x, T y, T z, T w) : x(x), y(y), z(z), w(w)
	{
	}

	template<typename T>
	inline Vector4T<T>& Vector4T<T>::operator=(const Vector4T<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;

		return *this;
	}

	template<typename T>
	inline const Vector4T<T>& Vector4T<T>::operator=(Vector4T<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = vec.w;

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
		Vector3T(T x, T y, T z);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector3T<T>& operator=(const Vector3T<T>& vec);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector3T<T>& operator=(Vector3T<T>& vec);

	};

	template<typename T>
	inline Vector3T<T>::Vector3T()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
		z = static_cast<T>(0);
	}

	template<typename T>
	inline Vector3T<T>::Vector3T(T x, T y, T z) : x(x), y(y), z(z)
	{
	}

	template<typename T>
	inline Vector3T<T>& Vector3T<T>::operator=(const Vector3T<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

		return *this;
	}

	template<typename T>
	inline const Vector3T<T>& Vector3T<T>::operator=(Vector3T<T>& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;

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
		Vector2T(T x, T y);//成分を指定して生成

		/// <summary>
		/// 代入演算子
		/// </summary>
		Vector2T<T>& operator=(const Vector2T<T>& vec);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Vector2T<T>& operator=(Vector2T<T>& vec);
	};

	template<typename T>
	inline Vector2T<T>::Vector2T()
	{
		x = static_cast<T>(0);
		y = static_cast<T>(0);
	}

	template<typename T>
	inline Vector2T<T>::Vector2T(T x, T y) : x(x), y(y)
	{
	}

	template<typename T>
	inline Vector2T<T>& Vector2T<T>::operator=(const Vector2T<T>& vec)
	{
		x = vec.x;
		y = vec.y;

		return *this;
	}

	template<typename T>
	inline const Vector2T<T>& Vector2T<T>::operator=(Vector2T<T>& vec)
	{
		x = vec.x;
		y = vec.y;

		return *this;
	}
}

