#include<DefaultMaterial.h>

void MaterialManager::Initialize()
{
	CreateDefaultTexture();

	CreateDefaultTextureMaterial();

	CreateDefaultLambertMaterial();

	CreateDefaultPhongMaterial();

	CreateDefaultSprite2DMaterial();

	CreateDefaultSprite3DMaterial();

	CreateDefaultMeshMaterial();

	CreateDefaultFbxMaterial();
	CreateDefaultFbxAnimationMaterial();

	CreateDefaultToonModelMaterial();
	CreateDefaultToonModelAnimationMaterial();

	CreateDefaultPostEffectMaterial();

	CreateDefaultParticleMaterial();

	CreateDefaultRainParticleMaterial();

	CreateDefaultIcosahedronParticleMaterial();
}

MaterialManager* MaterialManager::GetInstance()
{
	static MaterialManager material;
	return &material;
}

Material* MaterialManager::GetMaterialData(const std::string name)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& p)
		{
			return p.second->name == name;
		});

	if (itr != materials.end())
	{
		return itr._Ptr->_Myval.second.get();
	}

	if (itr == materials.end())
	{

	}

	return nullptr;
}

void MaterialManager::AddMaterial(std::unique_ptr <Material>& material, const std::string name)
{
	material->name = name;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& p)
		{
			return p.second->name == name;
		});

	if (itr == materials.end())
	{
		materials[name] = std::move(material);
	}
}

void MaterialManager::AddMaterial(Material* material, const std::string name)
{
	material->name = name;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& p)
		{
			return p.second->name == name;
		});

	if (itr == materials.end())
	{

		std::unique_ptr<Material> mat;
		mat.reset(material);

		materials[name] = std::move(mat);
	}
}

Material* MaterialManager::GetMaterial(const std::string name)
{
	return MaterialManager::GetInstance()->GetMaterialData(name);
}

TextureData* MaterialManager::GetDefaultTextureData()
{
	return DEFAULT_TEXTURE;
}

TextureData* MaterialManager::GetDefaultTexture()
{
	return MaterialManager::GetInstance()->GetDefaultTextureData();
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::CreateDefaultTexture()
{
	uint32_t handle = TextureManager::Load("Resources/Default/white1x1.png");
	DEFAULT_TEXTURE = TextureManager::GetTextureData(handle);
}

void MaterialManager::CreateDefaultTextureMaterial()
{
	std::unique_ptr<Material> DEFAULT_TEXTURE_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_TEXTURE_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_TEXTURE_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_TEXTURE_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/ModelBasic/ModelBasicVS.hlsl");

	//ジオメトリシェーダの読み込み
	DEFAULT_TEXTURE_MATERIAL->geometryShader = std::make_unique<Shader>();
	DEFAULT_TEXTURE_MATERIAL->geometryShader->Create("Resources/Shaders/3D/Model/ModelBasic/PolygonEffectGS.hlsl", "main", "gs_5_0", Shader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	DEFAULT_TEXTURE_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_TEXTURE_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/ModelBasic/ModelBasicPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_TEXTURE_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	DEFAULT_TEXTURE_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_TEXTURE_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_TEXTURE_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_TEXTURE_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_TEXTURE_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_TEXTURE_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	DEFAULT_TEXTURE_MATERIAL->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	DEFAULT_TEXTURE_MATERIAL->Initialize();

	AddMaterial(DEFAULT_TEXTURE_MATERIAL, "DefaultTexture");
}

void MaterialManager::CreateDefaultLambertMaterial()
{
	std::unique_ptr<Material>DEFAULT_LAMBERT_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_LAMBERT_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_LAMBERT_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_LAMBERT_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/Lambert/LambertVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_LAMBERT_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_LAMBERT_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/Lambert/LambertPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_LAMBERT_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	DEFAULT_LAMBERT_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_LAMBERT_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_LAMBERT_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_LAMBERT_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_LAMBERT_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_LAMBERT_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//生成
	DEFAULT_LAMBERT_MATERIAL->Initialize();

	AddMaterial(DEFAULT_LAMBERT_MATERIAL, "DefaultLambert");
}

void MaterialManager::CreateDefaultPhongMaterial()
{
	std::unique_ptr<Material>DEFAULT_PHONG_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_PHONG_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_PHONG_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_PHONG_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/Phong/PhongVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_PHONG_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_PHONG_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/Phong/PhongPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_PHONG_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	DEFAULT_PHONG_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_PHONG_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_PHONG_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_PHONG_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_PHONG_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_PHONG_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_PHONG_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_PHONG_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_PHONG_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_PHONG_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_PHONG_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//生成
	DEFAULT_PHONG_MATERIAL->Initialize();

	AddMaterial(DEFAULT_PHONG_MATERIAL, "DefaultPhong");

}

void MaterialManager::CreateDefaultSprite2DMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<Shader>vertexShader = std::make_unique<Shader>();
	vertexShader->Create("Resources/Shaders/2D/Sprite/SpriteVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<Shader>pixelShader = std::make_unique<Shader>();
	pixelShader->Create("Resources/Shaders/2D/Sprite/SpritePS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_NOBLEND, vertexShader.get(), pixelShader.get()), "Sprite2DNoblend");
	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_ALPHA, vertexShader.get(), pixelShader.get()), "Sprite2DAlpha");
	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_ADD, vertexShader.get(), pixelShader.get()), "Sprite2DAdd");
	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_SUB, vertexShader.get(), pixelShader.get()), "Sprite2DSub");
	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_MULA, vertexShader.get(), pixelShader.get()), "Sprite2DMula");
	AddMaterial(CreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_INVSRC, vertexShader.get(), pixelShader.get()), "Sprite2DInvsrc");
}

void MaterialManager::CreateDefaultSprite3DMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<Shader>vertexShader = std::make_unique<Shader>();
	vertexShader->Create("Resources/Shaders/2D/Sprite/SpriteVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<Shader>pixelShader = std::make_unique<Shader>();
	pixelShader->Create("Resources/Shaders/2D/Sprite/SpritePS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_NOBLEND, vertexShader.get(), pixelShader.get()), "Sprite3DNoblend");
	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_ALPHA, vertexShader.get(), pixelShader.get()), "Sprite3DAlpha");
	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_ADD, vertexShader.get(), pixelShader.get()), "Sprite3DAdd");
	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_SUB, vertexShader.get(), pixelShader.get()), "Sprite3DSub");
	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_MULA, vertexShader.get(), pixelShader.get()), "Sprite3DMula");
	AddMaterial(CreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_INVSRC, vertexShader.get(), pixelShader.get()), "Sprite3DInvsrc");
}

void MaterialManager::CreateDefaultFbxMaterial()
{
	std::unique_ptr<Material>DEFAULT_FBX_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_FBX_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_FBX_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/DefaultModel/ModelVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_FBX_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/DefaultModel/ModelPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_FBX_MATERIAL->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	DEFAULT_FBX_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_FBX_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());


	DEFAULT_FBX_MATERIAL->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	DEFAULT_FBX_MATERIAL->cullMode = D3D12_CULL_MODE_NONE;
	DEFAULT_FBX_MATERIAL->depthFlag = true;

	//生成
	DEFAULT_FBX_MATERIAL->Initialize();


	AddMaterial(DEFAULT_FBX_MATERIAL, "DefaultFbx");
}

void MaterialManager::CreateDefaultFbxAnimationMaterial()
{
	std::unique_ptr<Material>DEFAULT_FBX_MATERIAL = std::make_unique<Material>();

//テクスチャデータ設定
	DEFAULT_FBX_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_FBX_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/DefaultModel/ModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_FBX_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/DefaultModel/ModelPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_FBX_MATERIAL->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	DEFAULT_FBX_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_FBX_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());


	DEFAULT_FBX_MATERIAL->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	DEFAULT_FBX_MATERIAL->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	DEFAULT_FBX_MATERIAL->Initialize();

	AddMaterial(DEFAULT_FBX_MATERIAL, "DefaultFbxAnimation");
}

void MaterialManager::CreateDefaultParticleMaterial()
{
	std::unique_ptr<Material>DEFAULT_PARTICLE_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_PARTICLE_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_PARTICLE_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->vertexShader->Create("Resources/Shaders/2D/Particle/ParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_PARTICLE_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->pixelShader->Create("Resources/Shaders/2D/Particle/ParticlePS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	DEFAULT_PARTICLE_MATERIAL->geometryShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->geometryShader->Create("Resources/Shaders/2D/Particle/ParticleGS.hlsl", "main", "gs_5_0", Shader::ShaderType::GS);

	//頂点レイアウト設定
	DEFAULT_PARTICLE_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//スケール
		{ "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//回転角
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

	};

	//ルートシグネチャ設定
	DEFAULT_PARTICLE_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	DEFAULT_PARTICLE_MATERIAL->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	DEFAULT_PARTICLE_MATERIAL->depthFlag = false;

	//生成
	DEFAULT_PARTICLE_MATERIAL->Initialize();

	AddMaterial(DEFAULT_PARTICLE_MATERIAL, "DefaultParticle");
}

void MaterialManager::CreateDefaultMeshMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<Shader>vertexShader = std::make_unique<Shader>();
	vertexShader->Create("Resources/Shaders/2D/Mesh/MeshVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<Shader>pixelShader = std::make_unique<Shader>();
	pixelShader->Create("Resources/Shaders/2D/Mesh/MeshPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//三角形形状用パイプラインセット
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_NOBLEND, vertexShader.get(), pixelShader.get()), "MashTriangleNoblend");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ALPHA, vertexShader.get(), pixelShader.get()), "MashTriangleAlpha");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ADD, vertexShader.get(), pixelShader.get()), "MashTriangleAdd");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_SUB, vertexShader.get(), pixelShader.get()), "MashTriangleSub");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_MULA, vertexShader.get(), pixelShader.get()), "MashTriangleMula");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_INVSRC, vertexShader.get(), pixelShader.get()), "MashTriangleInvsrc");

	//ライン形状用パイプラインセット
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_NOBLEND, vertexShader.get(), pixelShader.get()), "MashLineNoblend");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ALPHA, vertexShader.get(), pixelShader.get()), "MashLineAlpha");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ADD, vertexShader.get(), pixelShader.get()), "MashLineAdd");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_SUB, vertexShader.get(), pixelShader.get()), "MashLineSub");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_MULA, vertexShader.get(), pixelShader.get()), "MashLineMula");
	AddMaterial(CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_INVSRC, vertexShader.get(), pixelShader.get()), "MashLineInvsrc");

}

void MaterialManager::CreateDefaultRainParticleMaterial()
{
	std::unique_ptr<Material>DEFAULT_RAIN_PARTICLE_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_RAIN_PARTICLE_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_RAIN_PARTICLE_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_RAIN_PARTICLE_MATERIAL->vertexShader->Create("Resources/Shaders/2D/Particle/RainParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_RAIN_PARTICLE_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_RAIN_PARTICLE_MATERIAL->pixelShader->Create("Resources/Shaders/2D/Particle/RainParticlePS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	DEFAULT_RAIN_PARTICLE_MATERIAL->geometryShader = std::make_unique<Shader>();
	DEFAULT_RAIN_PARTICLE_MATERIAL->geometryShader->Create("Resources/Shaders/2D/Particle/RainParticleGS.hlsl", "main", "gs_5_0", Shader::ShaderType::GS);

	//頂点レイアウト設定
	DEFAULT_RAIN_PARTICLE_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//スケール
		{ "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//回転角
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

	};

	//ルートシグネチャ設定
	DEFAULT_RAIN_PARTICLE_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_RAIN_PARTICLE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_RAIN_PARTICLE_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_RAIN_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_RAIN_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_RAIN_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	DEFAULT_RAIN_PARTICLE_MATERIAL->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	DEFAULT_RAIN_PARTICLE_MATERIAL->depthFlag = false;

	//生成
	DEFAULT_RAIN_PARTICLE_MATERIAL->Initialize();

	AddMaterial(DEFAULT_RAIN_PARTICLE_MATERIAL, "DefaultRainParticle");
}

void MaterialManager::CreateDefaultPostEffectMaterial()
{
	std::unique_ptr<Material>DEFAULT_POST_EFFECT_MATERIAL = std::make_unique<Material>();

	//頂点シェーダの読み込み
	DEFAULT_POST_EFFECT_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_POST_EFFECT_MATERIAL->vertexShader->Create("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_POST_EFFECT_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_POST_EFFECT_MATERIAL->pixelShader->Create("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestPS.hlsl", "main", "ps_5_0");

	//頂点レイアウト設定
	DEFAULT_POST_EFFECT_MATERIAL->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	DEFAULT_POST_EFFECT_MATERIAL->depthFlag = false;

	DEFAULT_POST_EFFECT_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_POST_EFFECT_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_POST_EFFECT_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//ルートシグネチャ設定
	DEFAULT_POST_EFFECT_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_POST_EFFECT_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_POST_EFFECT_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_POST_EFFECT_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//生成
	DEFAULT_POST_EFFECT_MATERIAL->Initialize();

	AddMaterial(DEFAULT_POST_EFFECT_MATERIAL, "DefaultPostEffect");
}

void MaterialManager::CreateDefaultIcosahedronParticleMaterial()
{
	std::unique_ptr<Material>DEFAULT_PARTICLE_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_PARTICLE_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_PARTICLE_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->vertexShader->Create("Resources/Shaders/2D/Particle/ParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_PARTICLE_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->pixelShader->Create("Resources/Shaders/2D/Particle/ParticlePS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	DEFAULT_PARTICLE_MATERIAL->geometryShader = std::make_unique<Shader>();
	DEFAULT_PARTICLE_MATERIAL->geometryShader->Create("Resources/Shaders/2D/Particle/IcosahedronGS.hlsl", "main", "gs_5_0", Shader::ShaderType::GS);

	//頂点レイアウト設定
	DEFAULT_PARTICLE_MATERIAL->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//スケール
		{ "SCALE", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//回転角
		{ "ROTATION", 0, DXGI_FORMAT_R32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},

	};

	//ルートシグネチャ設定
	DEFAULT_PARTICLE_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_PARTICLE_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//ブレンド設定
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	DEFAULT_PARTICLE_MATERIAL->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	DEFAULT_PARTICLE_MATERIAL->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	DEFAULT_PARTICLE_MATERIAL->depthFlag = false;

	DEFAULT_PARTICLE_MATERIAL->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	DEFAULT_PARTICLE_MATERIAL->Initialize();

	AddMaterial(DEFAULT_PARTICLE_MATERIAL, "IcosahedronParticle");
}

void MaterialManager::CreateDefaultToonModelMaterial()
{
	std::unique_ptr<Material>DEFAULT_FBX_MATERIAL = std::make_unique<Material>();

	//テクスチャデータ設定
	DEFAULT_FBX_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_FBX_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/ToonModel/ToonModelVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_FBX_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/ToonModel/ToonModelPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_FBX_MATERIAL->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	DEFAULT_FBX_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 1);//t1
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(1);//s1
	DEFAULT_FBX_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());


	DEFAULT_FBX_MATERIAL->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	DEFAULT_FBX_MATERIAL->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	DEFAULT_FBX_MATERIAL->Initialize();

	AddMaterial(DEFAULT_FBX_MATERIAL, "DefaultToonModel");
}

void MaterialManager::CreateDefaultToonModelAnimationMaterial()
{
	std::unique_ptr<Material>DEFAULT_FBX_MATERIAL = std::make_unique<Material>();

//テクスチャデータ設定
	DEFAULT_FBX_MATERIAL->textureData = DEFAULT_TEXTURE;

	//頂点シェーダの読み込み
	DEFAULT_FBX_MATERIAL->vertexShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->vertexShader->Create("Resources/Shaders/3D/Model/ToonModel/ToonModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	DEFAULT_FBX_MATERIAL->pixelShader = std::make_unique<Shader>();
	DEFAULT_FBX_MATERIAL->pixelShader->Create("Resources/Shaders/3D/Model/ToonModel/ToonModelPS.hlsl", "main", "ps_5_0", Shader::ShaderType::PS);

	//頂点レイアウト設定
	DEFAULT_FBX_MATERIAL->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	DEFAULT_FBX_MATERIAL->rootSignature = std::make_unique<RootSignature>();
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 1);//b1
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 2);//b2
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RootType::CBV, 3);//b3
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(0);//s0
	DEFAULT_FBX_MATERIAL->rootSignature->Add(RootSignature::RangeType::SRV, 1);//t1
	DEFAULT_FBX_MATERIAL->rootSignature->AddStaticSampler(1);//s1
	DEFAULT_FBX_MATERIAL->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());


	DEFAULT_FBX_MATERIAL->blenddesc = CreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	DEFAULT_FBX_MATERIAL->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	DEFAULT_FBX_MATERIAL->Initialize();

	AddMaterial(DEFAULT_FBX_MATERIAL, "DefaultToonModelAnimation");
}

Material* MaterialManager::CreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode mode, Shader* vex, Shader* pix)
{
	Material* material = new Material;

	//頂点シェーダの読み込み
	material->vertexShader = std::make_unique<Shader>();
	material->vertexShader->Copy(vex);

	//ピクセルシェーダの読み込み
	material->pixelShader = std::make_unique<Shader>();
	material->pixelShader->Copy(pix);

	//頂点レイアウト設定
	material->inputLayouts = {
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//uv座標 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	material->depthFlag = false;

	material->primitiveType = type;

	switch (mode)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:
		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値
		break;
	case BlendMode::AX_BLENDMODE_ADD:
		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// デストからソースを減算
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		material->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		material->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		material->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;// 使わない
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		break;
	default:
		break;
	}

	//ルートシグネチャ設定
	material->rootSignature = std::make_unique<RootSignature>();
	material->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());
	//生成
	material->Initialize();

	return material;
}

Material* MaterialManager::CreateDefaultSprite2DBlend(BlendMode mode, Shader* vex, Shader* pix)
{
	Material* material = new Material;

	//頂点シェーダの読み込み
	material->vertexShader = std::make_unique<Shader>();
	material->vertexShader->Copy(vex);

	//ピクセルシェーダの読み込み
	material->pixelShader = std::make_unique<Shader>();
	material->pixelShader->Copy(pix);

	//頂点レイアウト設定
	material->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	material->depthFlag = false;

	material->blenddesc = CreateBlend(mode);

	//ルートシグネチャ設定
	material->rootSignature = std::make_unique<RootSignature>();
	material->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	material->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	material->rootSignature->AddStaticSampler(0);//s0
	material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//生成
	material->Initialize();

	return material;
}

Material* MaterialManager::CreateDefaultSprite3DBlend(BlendMode mode, Shader* vex, Shader* pix)
{
	Material* material = new Material;

//頂点シェーダの読み込み
	material->vertexShader = std::make_unique<Shader>();
	material->vertexShader->Copy(vex);

	//ピクセルシェーダの読み込み
	material->pixelShader = std::make_unique<Shader>();
	material->pixelShader->Copy(pix);


	//頂点レイアウト設定
	material->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	material->blenddesc.AlphaToCoverageEnable = true;

	material->blenddesc = CreateBlend(mode);

	//ルートシグネチャ設定
	material->rootSignature = std::make_unique<RootSignature>();
	material->rootSignature->Add(RootSignature::RootType::CBV, 0);//b0
	material->rootSignature->Add(RootSignature::RangeType::SRV, 0);//t0
	material->rootSignature->AddStaticSampler(0);//s0
	material->rootSignature->Create(DirectX12Core::GetInstance()->GetDevice().Get());

	//生成
	material->Initialize();

	return material;
}

D3D12_BLEND_DESC MaterialManager::CreateBlend(BlendMode mode)
{
	D3D12_BLEND_DESC blend = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	switch (mode)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:

		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		blend.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		blend.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
		blend.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
		blend.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
		// 半透明合成
		blend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;// 加算
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

		break;
	case BlendMode::AX_BLENDMODE_ADD:
		blend.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		blend.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		blend.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// デストからソースを減算
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		blend.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		blend.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		blend.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		blend.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;// 使わない
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		break;
	default:
		break;
	}

	return blend;
}
