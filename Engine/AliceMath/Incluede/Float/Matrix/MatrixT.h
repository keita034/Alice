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
	struct Matrix4T
	{
		std::array<std::array<T, 4>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4T(
			T m00_, T m01_, T m02_, T m03_,
			T m10_, T m11_, T m12_, T m13_,
			T m20_, T m21_, T m22_, T m23_,
			T m30_, T m31_, T m32_, T m33_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4T<T>& operator=(const Matrix4T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4T<T>& operator=(Matrix4T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4T<T> Transpose();

	};

	template<typename T>
	inline Matrix4T<T>::Matrix4T(T m00_, T m01_, T m02_, T m03_, T m10_, T m11_, T m12_, T m13_, T m20_, T m21_, T m22_, T m23_, T m30_, T m31_, T m32_, T m33_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;
		m[0][3] = m03_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
		m[1][3] = m13_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;
		m[2][3] = m23_;

		m[3][0] = m30_;
		m[3][1] = m31_;
		m[3][2] = m32_;
		m[3][3] = m33_;
	}

	template<typename T>
	inline Matrix4T<T>& Matrix4T<T>::operator=(const Matrix4T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4T<T>& Matrix4T<T>::operator=(Matrix4T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4T<T> Matrix4T<T>::Transpose()
	{
		Matrix4T<T> lTmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix4x3T
	{
		std::array<std::array<T, 4>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x3T(
			T m00_, T m01_, T m02_,
			T m10_, T m11_, T m12_,
			T m20_, T m21_, T m22_,
			T m30_, T m31_, T m32_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x3T<T>& operator=(const Matrix4x3T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x3T<T>& operator=(Matrix4x3T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix4x3T<T>::Matrix4x3T(T m00_, T m01_, T m02_, T m10_, T m11_, T m12_, T m20_, T m21_, T m22_, T m30_, T m31_, T m32_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;

		m[3][0] = m30_;
		m[3][1] = m31_;
		m[3][2] = m32_;
	}

	template<typename T>
	inline Matrix4x3T<T>& Matrix4x3T<T>::operator=(const Matrix4x3T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4x3T<T>& Matrix4x3T<T>::operator=(Matrix4x3T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4x3T<T> Matrix4x3T<T>::Transpose()
	{
		Matrix4T<T> lTmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix4x2T
	{
		std::array<std::array<T, 4>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix4x2T(
			T m00_, T m01_,
			T m10_, T m11_,
			T m20_, T m21_,
			T m30_, T m31_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix4x2T<T>& operator=(const Matrix4x2T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix4x2T<T>& operator=(Matrix4x2T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix4x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix4x2T<T>::Matrix4x2T(T m00_, T m01_, T m10_, T m11_, T m20_, T m21_, T m30_, T m31_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;

		m[2][0] = m20_;
		m[2][1] = m21_;

		m[3][0] = m30_;
		m[3][1] = m31_;
	}

	template<typename T>
	inline Matrix4x2T<T>& Matrix4x2T<T>::operator=(const Matrix4x2T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix4x2T<T>& Matrix4x2T<T>::operator=(Matrix4x2T<T>& m_)
	{
		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix4x2T<T> Matrix4x2T<T>::Transpose()
	{
		Matrix4T<T> lTmp(*this);

		for (size_t i = 0; i < 4; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

#pragma endregion

#pragma region Matrix3

	template<typename T>
	struct Matrix3x4T
	{
		std::array<std::array<T, 3>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x4T(
			T m00_, T m01_, T m02_, T m03_,
			T m10_, T m11_, T m12_, T m13_,
			T m20_, T m21_, T m22_, T m23_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x4T<T>& operator=(const Matrix3x4T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x4T<T>& operator=(Matrix3x4T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x4T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x4T<T>::Matrix3x4T(T m00_, T m01_, T m02_, T m03_, T m10_, T m11_, T m12_, T m13_, T m20_, T m21_, T m22_, T m23_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;
		m[0][3] = m03_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
		m[1][3] = m13_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;
		m[2][3] = m23_;
	}

	template<typename T>
	inline Matrix3x4T<T>& Matrix3x4T<T>::operator=(const Matrix3x4T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x4T<T>& Matrix3x4T<T>::operator=(Matrix3x4T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x4T<T> Matrix3x4T<T>::Transpose()
	{
		Matrix3x4T<T> lTmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix3x3T
	{
		std::array<std::array<T, 3>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x3T(
			T m00_, T m01_, T m02_,
			T m10_, T m11_, T m12_,
			T m20_, T m21_, T m22_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x3T<T>& operator=(const Matrix3x3T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x3T<T>& operator=(Matrix3x3T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x3T<T>::Matrix3x3T(T m00_, T m01_, T m02_, T m10_, T m11_, T m12_, T m20_, T m21_, T m22_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;

		m[2][0] = m20_;
		m[2][1] = m21_;
		m[2][2] = m22_;
	}

	template<typename T>
	inline Matrix3x3T<T>& Matrix3x3T<T>::operator=(const Matrix3x3T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x3T<T>& Matrix3x3T<T>::operator=(Matrix3x3T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x3T<T> Matrix3x3T<T>::Transpose()
	{
		Matrix3x3T<T> lTmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix3x2T
	{

		std::array<std::array<T, 3>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix3x2T(
			T m00_, T m01_,
			T m10_, T m11_,
			T m20_, T m21_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix3x2T<T>& operator=(const Matrix3x2T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix3x2T<T>& operator=(Matrix3x2T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix3x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix3x2T<T>::Matrix3x2T(T m00_, T m01_, T m10_, T m11_, T m20_, T m21_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;

		m[2][0] = m20_;
		m[2][1] = m21_;
	}

	template<typename T>
	inline Matrix3x2T<T>& Matrix3x2T<T>::operator=(const Matrix3x2T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix3x2T<T>& Matrix3x2T<T>::operator=(Matrix3x2T<T>& m_)
	{
		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix3x2T<T> Matrix3x2T<T>::Transpose()
	{
		Matrix3x2T<T> lTmp(*this);

		for (size_t i = 0; i < 3; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

#pragma endregion

#pragma region Matrix2

	template<typename T>
	struct Matrix2x4T
	{
		std::array<std::array<T, 2>, 4>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x4T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x4T(
			T m00_, T m01_, T m02_, T m03_,
			T m10_, T m11_, T m12_, T m13_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x4T<T>& operator=(const Matrix2x4T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x4T<T>& operator=(Matrix2x4T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x4T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x4T<T>::Matrix2x4T(T m00_, T m01_, T m02_, T m03_, T m10_, T m11_, T m12_, T m13_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;
		m[0][3] = m03_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
		m[1][3] = m13_;
	}


	template<typename T>
	inline Matrix2x4T<T>& Matrix2x4T<T>::operator=(const Matrix2x4T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x4T<T>& Matrix2x4T<T>::operator=(Matrix2x4T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 4; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x4T<T> Matrix2x4T<T>::Transpose()
	{
		Matrix2x4T<T> lTmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 4; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix2x3T
	{
		std::array<std::array<T, 2>, 3>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x3T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x3T(
			T m00_, T m01_, T m02_,
			T m10_, T m11_, T m12_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x3T<T>& operator=(const Matrix2x3T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x3T<T>& operator=(Matrix2x3T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x3T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x3T<T>::Matrix2x3T(T m00_, T m01_, T m02_, T m10_, T m11_, T m12_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;
		m[0][2] = m02_;

		m[1][0] = m10_;
		m[1][1] = m11_;
		m[1][2] = m12_;
	}

	template<typename T>
	inline Matrix2x3T<T>& Matrix2x3T<T>::operator=(const Matrix2x3T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x3T<T>& Matrix2x3T<T>::operator=(Matrix2x3T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 3; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x3T<T> Matrix2x3T<T>::Transpose()
	{
		Matrix2x3T<T> lTmp(*this);

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 3; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

	template<typename T>
	struct Matrix2x2T
	{
		std::array<std::array<T, 2>, 2>m;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x2T() = default;

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Matrix2x2T(
			T m00_, T m01_,
			T m10_, T m11_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		Matrix2x2T<T>& operator=(const Matrix2x2T<T>& m_);

		/// <summary>
		/// 代入演算子
		/// </summary>
		const Matrix2x2T<T>& operator=(Matrix2x2T<T>& m_);

		/// <summary>
		/// 転置行列
		/// </summary>
		Matrix2x2T<T> Transpose();

	};

	template<typename T>
	inline Matrix2x2T<T>::Matrix2x2T(T m00_, T m01_, T m10_, T m11_)
	{
		m[0][0] = m00_;
		m[0][1] = m01_;

		m[1][0] = m10_;
		m[1][1] = m11_;
	}

	template<typename T>
	inline Matrix2x2T<T>& Matrix2x2T<T>::operator=(const Matrix2x2T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline const Matrix2x2T<T>& Matrix2x2T<T>::operator=(Matrix2x2T<T>& m_)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m[i][j] = m_.m[i][j];
			}
		}

		return *this;
	}

	template<typename T>
	inline Matrix2x2T<T> Matrix2x2T<T>::Transpose()
	{
		Matrix2x2T<T> lTmp(*this);
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = i; j < 2; j++)
			{
				T lF = lTmp.m[i][j];
				lTmp.m[i][j] = lTmp.m[j][i];
				lTmp.m[j][i] = lF;
			}
		}

		return lTmp;
	}

#pragma endregion

}