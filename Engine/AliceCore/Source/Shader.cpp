#include "Shader.h"

#pragma warning(push)
#pragma warning(disable: 4061)
#pragma warning(disable: 4062)
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

#include<wrl.h>
#include<directx/d3dx12.h>
#include<d3dcompiler.h>
#include<cassert>
#include<filesystem>

#pragma warning(pop)

#pragma comment(lib, "d3dcompiler.lib")

std::wstring StringToWstring(const std::string& string)
{
	auto const dest_size = ::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, nullptr, 0U);
	std::vector<wchar_t> dest(static_cast<const uint64_t>(dest_size), L'\0');
	if (::MultiByteToWideChar(CP_ACP, 0U, string.data(), -1, dest.data(), static_cast<int32_t>(dest.size())) == 0)
	{
		throw std::system_error{ static_cast<int32_t>(::GetLastError()), std::system_category() };
	}
	dest.resize(std::char_traits<wchar_t>::length(dest.data()));
	dest.shrink_to_fit();
	return std::wstring(dest.begin(), dest.end());
}

class Shader : public IShader
{
private:
	//シェーダオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	//シェーダバイトコード
	D3D12_SHADER_BYTECODE shaderBytecode{};

	//タイプ
	ShaderType type = ShaderType::VS;
	int32_t PADING;

public:
	/// <summary>
	/// シェーダオブジェクトを生成
	/// </summary>
	void Create(const std::string& fileName_, const std::string& entryPoint_, const std::string& target_, const ShaderType& type_) override;

	/// <summary>
	/// コピー
	/// </summary>
	void Copy(IShader* shader_);

	/// <summary>
	/// シェーダオブジェクトを取得
	/// </summary>
	D3D12_SHADER_BYTECODE* GetShader()override;

	/// <summary>
	/// タイプ
	/// </summary>
	const ShaderType& GetType()override;

	Shader() = default;
	virtual ~Shader() = default;

};

void Shader::Create(const std::string& fileName_, const std::string& entryPoint_, const std::string& target_, const ShaderType& type_)
{
	HRESULT lResult = S_OK;

	// エラーオブジェクト
	Microsoft::WRL::ComPtr<ID3DBlob> lErrorBlob;

	std::wstring lWFileName;
	lWFileName = StringToWstring(fileName_);

	// ピクセルシェーダの読み込みとコンパイル
	lResult = D3DCompileFromFile(
		lWFileName.c_str(), // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		entryPoint_.c_str(), target_.c_str(), // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&blob, &lErrorBlob);

	// エラーなら
	if (FAILED(lResult))
	{
		// errorBlobからエラー内容をstring型にコピー
		std::string lError;
		lError.resize(lErrorBlob->GetBufferSize());
		std::copy_n((char*)lErrorBlob->GetBufferPointer(),
			lErrorBlob->GetBufferSize(),
			lError.begin());
		lError += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(lError.c_str());
		assert(0);
	};

	shaderBytecode.pShaderBytecode = blob->GetBufferPointer();
	shaderBytecode.BytecodeLength = blob->GetBufferSize();

	type = type_;
}

void Shader::Copy(IShader* shader_)
{
	type = shader_->GetType();
	shaderBytecode = *shader_->GetShader();
}

D3D12_SHADER_BYTECODE* Shader::GetShader()
{
	return &shaderBytecode;
}

const IShader::ShaderType& Shader::GetType()
{
	return type;
}

std::unique_ptr<IShader> CreateUniqueShader(const std::string& fileName_, const std::string& entryPoint_, const std::string& target_, const IShader::ShaderType& type_)
{
	std::unique_ptr<IShader> lShader = std::make_unique<Shader>();
	lShader->Create(fileName_, entryPoint_, target_, type_);
	return std::move(lShader);
}

std::shared_ptr<IShader> CreateSharedShader(const std::string& fileName_, const std::string& entryPoint_, const std::string& target_, const IShader::ShaderType& type_)
{
	std::shared_ptr<IShader> lShader = std::make_shared<Shader>();
	lShader->Create(fileName_, entryPoint_, target_, type_);
	return lShader;
}

std::unique_ptr<IShader> CopyUniqueShader(IShader* shader_)
{
	std::unique_ptr<IShader> lShader = std::make_unique<Shader>();
	lShader->Copy(shader_);
	return std::move(lShader);
}

std::shared_ptr<IShader> CopySharedShader(IShader* shader_)
{
	std::shared_ptr<IShader> lShader = std::make_shared<Shader>();
	lShader->Copy(shader_);
	return lShader;
}
