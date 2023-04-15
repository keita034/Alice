#pragma once
#pragma once
#pragma warning(push)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4619)
#pragma warning(disable: 4668)
#pragma warning(disable: 5039)

#include<array>

#pragma warning(pop)

namespace AliceMathF
{
#pragma region Matrix4

	template<typename T>
	class Matrix4T
	{
	public:
		std::array<std::array<T, 4>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4T(
			T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23,
			T m30, T m31, T m32, T m33);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4T<T>& operator=(const Matrix4T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4T<T>& operator=(Matrix4T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4T<T> Transpose();

	};

	template<typename T>
	inline Matrix4T<T>::Matrix4T(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23, T m30, T m31, T m32, T m33)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	template<typename T>
	inline Matrix4T<T>& Matrix4T<T>::operator=(const Matrix4T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4T<T>& Matrix4T<T>::operator=(Matrix4T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4T<T> Matrix4T<T>::Transpose()
	{
		Matrix4T<T> tmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix4x3T
	{
	public:
		std::array<std::array<T, 4>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3T(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22,
			T m30, T m31, T m32);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x3T<T>& operator=(const Matrix4x3T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x3T<T>& operator=(Matrix4x3T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix4x3T<T>::Matrix4x3T(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22, T m30, T m31, T m32)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
	}

	template<typename T>
	inline Matrix4x3T<T>& Matrix4x3T<T>::operator=(const Matrix4x3T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4x3T<T>& Matrix4x3T<T>::operator=(Matrix4x3T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4x3T<T> Matrix4x3T<T>::Transpose()
	{
		Matrix4T<T> tmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix4x2T
	{
	public:
		std::array<std::array<T, 4>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x2T(
			T m00, T m01,
			T m10, T m11,
			T m20, T m21,
			T m30, T m31);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x2T<T>& operator=(const Matrix4x2T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x2T<T>& operator=(Matrix4x2T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix4x2T<T>::Matrix4x2T(T m00, T m01, T m10, T m11, T m20, T m21, T m30, T m31)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;

		m[2][0] = m20;
		m[2][1] = m21;

		m[3][0] = m30;
		m[3][1] = m31;
	}

	template<typename T>
	inline Matrix4x2T<T>& Matrix4x2T<T>::operator=(const Matrix4x2T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4x2T<T>& Matrix4x2T<T>::operator=(Matrix4x2T<T>& _m)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4x2T<T> Matrix4x2T<T>::Transpose()
	{
		Matrix4T<T> tmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

#pragma endregion

#pragma region Matrix3

	template<typename T>
	class Matrix3x4T
	{
	public:
		std::array<std::array<T, 3>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4T(
			T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13,
			T m20, T m21, T m22, T m23);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x4T<T>& operator=(const Matrix3x4T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x4T<T>& operator=(Matrix3x4T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x4T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x4T<T>::Matrix3x4T(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13, T m20, T m21, T m22, T m23)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
	}

	template<typename T>
	inline Matrix3x4T<T>& Matrix3x4T<T>::operator=(const Matrix3x4T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x4T<T>& Matrix3x4T<T>::operator=(Matrix3x4T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x4T<T> Matrix3x4T<T>::Transpose()
	{
		Matrix3x4T<T> tmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix3x3T
	{
	public:
		std::array<std::array<T, 3>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x3T(
			T m00, T m01, T m02,
			T m10, T m11, T m12,
			T m20, T m21, T m22);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x3T<T>& operator=(const Matrix3x3T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x3T<T>& operator=(Matrix3x3T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x3T<T>::Matrix3x3T(T m00, T m01, T m02, T m10, T m11, T m12, T m20, T m21, T m22)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
	}

	template<typename T>
	inline Matrix3x3T<T>& Matrix3x3T<T>::operator=(const Matrix3x3T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x3T<T>& Matrix3x3T<T>::operator=(Matrix3x3T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x3T<T> Matrix3x3T<T>::Transpose()
	{
		Matrix3x3T<T> tmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix3x2T
	{
	public:
		std::array<std::array<T, 3>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x2T(
			T m00, T m01,
			T m10, T m11,
			T m20, T m21);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x2T<T>& operator=(const Matrix3x2T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x2T<T>& operator=(Matrix3x2T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x2T<T>::Matrix3x2T(T m00, T m01, T m10, T m11, T m20, T m21)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;

		m[2][0] = m20;
		m[2][1] = m21;
	}

	template<typename T>
	inline Matrix3x2T<T>& Matrix3x2T<T>::operator=(const Matrix3x2T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x2T<T>& Matrix3x2T<T>::operator=(Matrix3x2T<T>& _m)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x2T<T> Matrix3x2T<T>::Transpose()
	{
		Matrix3x2T<T> tmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

#pragma endregion

#pragma region Matrix2

	template<typename T>
	class Matrix2x4T
	{
	public:
		std::array<std::array<T, 2>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x4T(
			T m00, T m01, T m02, T m03,
			T m10, T m11, T m12, T m13);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x4T<T>& operator=(const Matrix2x4T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x4T<T>& operator=(Matrix2x4T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x4T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x4T<T>::Matrix2x4T(T m00, T m01, T m02, T m03, T m10, T m11, T m12, T m13)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
	}


	template<typename T>
	inline Matrix2x4T<T>& Matrix2x4T<T>::operator=(const Matrix2x4T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x4T<T>& Matrix2x4T<T>::operator=(Matrix2x4T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x4T<T> Matrix2x4T<T>::Transpose()
	{
		Matrix2x4T<T> tmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix2x3T
	{
	public:
		std::array<std::array<T, 2>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x3T(
			T m00, T m01, T m02,
			T m10, T m11, T m12);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x3T<T>& operator=(const Matrix2x3T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x3T<T>& operator=(Matrix2x3T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x3T<T>::Matrix2x3T(T m00, T m01, T m02, T m10, T m11, T m12)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
	}

	template<typename T>
	inline Matrix2x3T<T>& Matrix2x3T<T>::operator=(const Matrix2x3T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x3T<T>& Matrix2x3T<T>::operator=(Matrix2x3T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x3T<T> Matrix2x3T<T>::Transpose()
	{
		Matrix2x3T<T> tmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

	template<typename T>
	class Matrix2x2T
	{
	public:
		std::array<std::array<T, 2>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x2T(
			T m00, T m01,
			T m10, T m11);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x2T<T>& operator=(const Matrix2x2T<T>& _m);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x2T<T>& operator=(Matrix2x2T<T>& _m);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x2T<T>::Matrix2x2T(T m00, T m01, T m10, T m11)
	{
		m[0][0] = m00;
		m[0][1] = m01;

		m[1][0] = m10;
		m[1][1] = m11;
	}

	template<typename T>
	inline Matrix2x2T<T>& Matrix2x2T<T>::operator=(const Matrix2x2T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x2T<T>& Matrix2x2T<T>::operator=(Matrix2x2T<T>& _m)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = _m.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x2T<T> Matrix2x2T<T>::Transpose()
	{
		Matrix2x2T<T> tmp(*this);
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T f = tmp.m[i][j];
				tmp.m[i][j] = tmp.m[j][i];
				tmp.m[j][i] = f;
			}
		}

		return tmp;
	}

#pragma endregion

}