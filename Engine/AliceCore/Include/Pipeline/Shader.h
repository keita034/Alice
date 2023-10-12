#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3d12.h>
#include<string>
#include<memory>

ALICE_SUPPRESS_WARNINGS_END

/// <summary>
/// シェーダオブジェクト(インターフェース)
/// </summary>
class IShader
{
public:
	//タイプ
	enum class ShaderType
	{
		VS,//バーテクス
		PS,//ピクセル
		DS,//ドメイン
		HS,//ハル
		GS,//ジオメトリ
		CS,//コンピュート
	};

	/// <summary>
	/// シェーダオブジェクトを生成
	/// </summary>
	/// <param name="fileName">シェーダのファイル名</param>
	/// <param name="entryPoint">エントリーポイント(初期値:main)</param>
	/// <param name="target">シェーダのバージョン(初期値:vs_5_0)</param>
	/// <param name="type_">シェーダの種類(初期値:VS)</param>
	virtual void Create(const std::string& fileName_, const std::string& entryPoint_, const std::string& target_, const ShaderType& type_) = 0;

	/// <summary>
	/// コピー
	/// </summary>
	/// <param name="shader_">コピー元</param>
	virtual void Copy(IShader* shader_) = 0;

	/// <summary>
	/// シェーダオブジェクトを取得
	/// </summary>
	/// <returns>シェーダオブジェクト(D3D12_SHADER_BYTECODE)</returns>
	virtual D3D12_SHADER_BYTECODE* GetShader() = 0;

	/// <summary>
	/// タイプ
	/// </summary>
	/// <returns>タイプ</returns>
	virtual const ShaderType& GetType() = 0;

	IShader() = default;
	virtual ~IShader() = default;
};

/// <summary>
/// シェーダオブジェクトの生成(ユニーク)
/// </summary>
/// <param name="fileName">シェーダのファイル名</param>
/// <param name="entryPoint">エントリーポイント(初期値:main)</param>
/// <param name="target">シェーダのバージョン(初期値:vs_5_0)</param>
/// <param name="type_">シェーダの種類(初期値:VS)</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IShader> CreateUniqueShader(const std::string& fileName_, const std::string& entryPoint_ = "main", const std::string& target_ = "vs_5_0", const IShader::ShaderType& type_ = IShader::ShaderType::VS);

/// <summary>
/// シェーダオブジェクトの生成(シェアード)
/// </summary>
/// <param name="fileName">シェーダのファイル名</param>
/// <param name="entryPoint">エントリーポイント(初期値:main)</param>
/// <param name="target">シェーダのバージョン(初期値:vs_5_0)</param>
/// <param name="type_">シェーダの種類(初期値:VS)</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IShader> CreateSharedShader(const std::string& fileName_, const std::string& entryPoint_ = "main", const std::string& target_ = "vs_5_0", const IShader::ShaderType& type_ = IShader::ShaderType::VS);

/// <summary>
/// シェーダオブジェクトをコピーして生成(ユニーク)
/// </summary>
/// <param name="shader_">コピー元</param>
/// <returns>生成されたポインタ</returns>
std::unique_ptr<IShader> CopyUniqueShader(IShader* shader_);

/// <summary>
/// シェーダオブジェクトをコピーして生成(シェアード)
/// </summary>
/// <param name="shader_">コピー元</param>
/// <returns>生成されたポインタ</returns>
std::shared_ptr<IShader> CopySharedShader(IShader* shader_);