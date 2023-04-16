#pragma once
#pragma warning(push)
#pragma warning(disable: 4265)
#pragma warning(disable: 4365)
#pragma warning(disable: 4514)
#pragma warning(disable: 4625)
#pragma warning(disable: 4626)
#pragma warning(disable: 4668)
#pragma warning(disable: 4710)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)
#pragma warning(disable: 5204)
#pragma warning(disable: 5220)

#include<d3d12.h>
#include<wrl.h>
#include<string>
#include<d3dx12.h>

#pragma warning(pop)


/// <summary>
/// シェーダオブジェクト
/// </summary>
class Shader
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

private:
	//タイプ
	ShaderType type = ShaderType::VS;
	char PADING[4]{};

	//シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	//シェーダバイトコード
	D3D12_SHADER_BYTECODE shaderBytecode{};
public:

public:
	/// <summary>
	/// シェーダオブジェクトを生成
	/// </summary>
	/// <param name="fileName">シェーダのファイル名</param>
	/// <param name="entryPoint">エントリーポイント(初期値:main)</param>
	/// <param name="target">シェーダのバージョン(初期値:vs_5_0)</param>
	/// <param name="type_">シェーダの種類(初期値:VS)</param>
	void Create(const std::string& fileName, const std::string& entryPoint = "main", const std::string& target= "vs_5_0", const ShaderType& type_ = ShaderType::VS);

	/// <summary>
	/// シェーダオブジェクトを取得
	/// </summary>
	/// <returns>シェーダオブジェクト(D3D12_SHADER_BYTECODE)</returns>
	D3D12_SHADER_BYTECODE* GetShader();

	/// <summary>
	/// タイプ
	/// </summary>
	/// <returns>タイプ</returns>
	ShaderType GetType();

	/// <summary>
	/// シェーダーデータをコピーする
	/// </summary>
	/// <param name="shader">コピー元シェーダー</param>
	/// <returns>コピー出来たか</returns>
	bool Copy(Shader* shader);

private:
};

