#pragma warning(push)
#pragma warning(disable: 4668)
#pragma warning(disable: 4820)
#pragma warning(disable: 5039)

#include<cassert>

#pragma warning(pop)

#include "Shader.h"
#include<AliceFunctionUtility.h>

#pragma comment(lib, "d3dcompiler.lib")

void Shader::Create(const std::string& fileName, const std::string& entryPoint, const std::string& target, const ShaderType& type_)
{
	HRESULT result = S_OK;

	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

	std::wstring wFileName;
	wFileName = AliceFunctionUtility::StringToWstring(fileName);

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		wFileName.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPoint.c_str(), target.c_str(), // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&blob, &errorBlob);

	// エラーなら
	if (FAILED(result))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	};

	shaderBytecode.pShaderBytecode = blob->GetBufferPointer();
	shaderBytecode.BytecodeLength = blob->GetBufferSize();

	type = type_;
}

D3D12_SHADER_BYTECODE* Shader::GetShader()
{
	return &shaderBytecode;
}

Shader::ShaderType Shader::GetType()
{
	return type;
}

bool Shader::Copy(Shader* shader)
{
	shaderBytecode = shader->shaderBytecode;
	blob = shader->blob;
	type = shader->type;
	return true;
}
