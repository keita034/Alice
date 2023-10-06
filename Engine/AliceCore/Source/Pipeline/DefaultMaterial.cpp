#include<DefaultMaterial.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<directx/d3dx12.h>

ALICE_SUPPRESS_WARNINGS_END

ID3D12Device* MaterialManager::sDevice = nullptr;
std::unique_ptr<MaterialManager> MaterialManager::materialManager;

void MaterialManager::Initialize(ID3D12Device* device_)
{
	sDevice = device_;
	PCreateDefaultFbxMaterial();
	PCreateDefaultFbxAnimationMaterial();
}

MaterialManager* MaterialManager::SGetInstance()
{
	if ( !materialManager )
	{
		MaterialManager* lInstance = new MaterialManager();
		materialManager.reset(lInstance);
	}

	return materialManager.get();
}

Material* MaterialManager::GetMaterialData(const std::string& name_)
{
	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& p)
		{
			return p.second->name == name_;
		});

	if (itr != materials.end())
	{
		return itr._Ptr->_Myval.second.get();
	}

	if (itr == materials.end())
	{
		if (name_ == "DefaultTexture")
		{
			PCreateDefaultTextureMaterial();
		}
		else if (name_ == "DefaultLambert")
		{
			PCreateDefaultLambertMaterial();
		}
		else if (name_ == "DefaultPhong")
		{
			PCreateDefaultPhongMaterial();
		}
		else if (
			name_ == "Sprite2DNoblend" ||
			name_ == "Sprite2DAlpha" ||
			name_ == "Sprite2DAdd" ||
			name_ == "Sprite2DSub" ||
			name_ == "Sprite2DMula" ||
			name_ == "Sprite2DInvsrc")
		{
			PCreateDefaultSprite2DMaterial();
		}
		else if (
			name_ == "Sprite3DNoblend" ||
			name_ == "Sprite3DAlpha" ||
			name_ == "Sprite3DAdd" ||
			name_ == "Sprite3DSub" ||
			name_ == "Sprite3DMula" ||
			name_ == "Sprite3DInvsrc")
		{
			PCreateDefaultSprite3DMaterial();
		}
		else if (name_ == "DefaultParticle")
		{
			PCreateDefaultParticleMaterial();
		}
		else if (name_ == "DefaultParticle")
		{
			PCreateDefaultParticleMaterial();
		}
		else if (
			name_ == "MashTriangleNoblend" ||
			name_ == "MashTriangleAlpha" ||
			name_ == "MashTriangleAdd" ||
			name_ == "MashTriangleSub" ||
			name_ == "MashTriangleMula" ||
			name_ == "MashTriangleInvsrc" ||
			name_ == "MashLineNoblend" ||
			name_ == "MashLineAlpha" ||
			name_ == "MashLineAdd" ||
			name_ == "MashLineSub" ||
			name_ == "MashLineMula" ||
			name_ == "MashLineInvsrc")
		{
			PCreateDefaultMeshMaterial();
		}
		else if (name_ == "DefaultRainParticle")
		{
			PCreateDefaultRainParticleMaterial();
		}
		else if (name_ == "DefaultPostEffect")
		{
			PCreateDefaultPostEffectMaterial();
		}
		else if (name_ == "IcosahedronParticle")
		{
			PCreateDefaultIcosahedronParticleMaterial();
		}
		else if (name_ == "DefaultToonModel")
		{
			PCreateDefaultToonModelMaterial();
		}
		else if (name_ == "DefaultToonModelAnimation")
		{
			PCreateDefaultToonModelAnimationMaterial();
		}
		else if (name_ == "DefaultToonModelOutLine")
		{
			PCreateDefaultToonModelOutLineMaterial();
		}
		else if (name_ == "DefaultToonModelAnimationOutLine")
		{
			PCreateDefaultToonModelOutLineAnimationMaterial();
		}
		else if (name_ == "DefaultZeldaToonModel")
		{
			PCreateDefaultZeldaToonModelMaterial();
		}
		else if (name_ == "DefaultZeldaToonModelAnimation")
		{
			PCreateDefaultZeldaToonModelAnimationMaterial();
		}

		return materials[name_].get();
	}

	return nullptr;
}

void MaterialManager::AddMaterial(std::unique_ptr <Material>& material_, const std::string& name_)
{
	material_->name = name_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMaterialItr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& material)
		{
			return material.second->name == name_;
		});

	if (lMaterialItr == materials.end())
	{
		materials[name_] = std::move(material_);
	}
}

void MaterialManager::AddMaterial(Material* material, const std::string& name_)
{
	material->name = name_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMaterialItr = find_if(materials.begin(), materials.end(), [&](std::pair<const std::string, std::unique_ptr<Material, std::default_delete<Material>>>& material)
		{
			return material.second->name == name_;
		});

	if (lMaterialItr == materials.end())
	{
		std::unique_ptr<Material> lMaterial;
		lMaterial.reset(material);
		materials[name_] = std::move(lMaterial);
	}
}

void MaterialManager::Finalize()
{
	sDevice = nullptr;
	materials.clear();
	materialManager.release();
}

Material* MaterialManager::SGetMaterial(const std::string& name_)
{
	return MaterialManager::SGetInstance()->GetMaterialData(name_);
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::PCreateDefaultTextureMaterial()
{
	std::unique_ptr<Material> lDefaultTextureMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultTextureMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/ModelBasicVS.hlsl");

	//ジオメトリシェーダの読み込み
	lDefaultTextureMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/PolygonEffectGS.hlsl", "main", "gs_5_0", IShader::ShaderType::GS);

	//ピクセルシェーダの読み込み
	lDefaultTextureMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ModelBasic/ModelBasicPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultTextureMaterial->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	lDefaultTextureMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultTextureMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultTextureMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultTextureMaterial->rootSignature->Create(sDevice);

	//ブレンド設定
	lDefaultTextureMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultTextureMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultTextureMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultTextureMaterial->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	lDefaultTextureMaterial->Initialize();

	AddMaterial(lDefaultTextureMaterial, "DefaultTexture");
}

void MaterialManager::PCreateDefaultLambertMaterial()
{
	std::unique_ptr<Material>lDefaultLambertMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultLambertMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/Lambert/LambertVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultLambertMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/Lambert/LambertPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultLambertMaterial->inputLayouts = {
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//法線
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	//ルートシグネチャ設定
	lDefaultLambertMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultLambertMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultLambertMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultLambertMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultLambertMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultLambertMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultLambertMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultLambertMaterial->rootSignature->Create(sDevice);

	//ブレンド設定
	lDefaultLambertMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultLambertMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultLambertMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//生成
	lDefaultLambertMaterial->Initialize();

	AddMaterial(lDefaultLambertMaterial, "DefaultLambert");
}

void MaterialManager::PCreateDefaultPhongMaterial()
{
	std::unique_ptr<Material>lDefaultPhongMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultPhongMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/Phong/PhongVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultPhongMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/Phong/PhongPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultPhongMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultPhongMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultPhongMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultPhongMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultPhongMaterial->rootSignature->Create(sDevice);


	lDefaultPhongMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultPhongMaterial->cullMode = D3D12_CULL_MODE_BACK;
	lDefaultPhongMaterial->depthFlag = true;

	//生成
	lDefaultPhongMaterial->Initialize();

	AddMaterial(lDefaultPhongMaterial, "DefaultPhong");

}

void MaterialManager::PCreateDefaultSprite2DMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpriteVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpritePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	std::unique_ptr<Material>lMaterial;

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_NOBLEND, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DNoblend");

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_ALPHA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DAlpha");

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_ADD, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DAdd");

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_SUB, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DSub");

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_MULA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DMula");

	lMaterial = PCreateDefaultSprite2DBlend(BlendMode::AX_BLENDMODE_INVSRC, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite2DInvsrc");
}

void MaterialManager::PCreateDefaultSprite3DMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpriteVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/2D/Sprite/SpritePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	std::unique_ptr<Material>lMaterial;

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_NOBLEND, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DNoblend");

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_ALPHA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DAlpha");

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_ADD, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DAdd");

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_SUB, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DSub");

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_MULA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DMula");

	lMaterial = PCreateDefaultSprite3DBlend(BlendMode::AX_BLENDMODE_INVSRC, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "Sprite3DInvsrc");
}

void MaterialManager::PCreateDefaultFbxMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/DefaultModel/ModelVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/DefaultModel/ModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	lDefaultModelMaterial->depthFlag = true;

	//生成
	lDefaultModelMaterial->Initialize();


	AddMaterial(lDefaultModelMaterial, "DefaultFbx");
}

void MaterialManager::PCreateDefaultFbxAnimationMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/DefaultModel/ModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/DefaultModel/ModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultFbxAnimation");
}

void MaterialManager::PCreateDefaultParticleMaterial()
{
	std::unique_ptr<Material>lDefaultParticleMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultParticleMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultParticleMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticlePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	lDefaultParticleMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticleGS.hlsl", "main", "gs_5_0", IShader::ShaderType::GS);

	//頂点レイアウト設定
	lDefaultParticleMaterial->inputLayouts = {
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
	lDefaultParticleMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultParticleMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultParticleMaterial->rootSignature->Create(sDevice);

	//ブレンド設定
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultParticleMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lDefaultParticleMaterial->depthFlag = false;

	//生成
	lDefaultParticleMaterial->Initialize();

	AddMaterial(lDefaultParticleMaterial, "DefaultParticle");
}

void MaterialManager::PCreateDefaultMeshMaterial()
{
	//頂点シェーダの読み込み
	std::unique_ptr<IShader>lVertexShader = CreateUniqueShader("Resources/Shaders/2D/Mesh/MeshVS.hlsl");

	//ピクセルシェーダの読み込み
	std::unique_ptr<IShader>lPixelShader = CreateUniqueShader("Resources/Shaders/2D/Mesh/MeshPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	std::unique_ptr<Material>lMaterial;

	//三角形形状用パイプラインセット
	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_NOBLEND, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleNoblend");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ALPHA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleAlpha");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_ADD, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleAdd");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_SUB, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleSub");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_MULA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleMula");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, BlendMode::AX_BLENDMODE_INVSRC, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashTriangleInvsrc");

	//ライン形状用パイプラインセット
	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_NOBLEND, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineNoblend");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ALPHA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineAlpha");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_ADD, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineAdd");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_SUB, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineSub");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_MULA, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineMula");

	lMaterial = PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, BlendMode::AX_BLENDMODE_INVSRC, lVertexShader.get(), lPixelShader.get());
	AddMaterial(lMaterial, "MashLineInvsrc");

}

void MaterialManager::PCreateDefaultRainParticleMaterial()
{
	std::unique_ptr<Material>lDefaultRainParticleMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultRainParticleMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultRainParticleMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticlePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	lDefaultRainParticleMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/2D/Particle/RainParticleGS.hlsl", "main", "gs_5_0", IShader::ShaderType::GS);

	//頂点レイアウト設定
	lDefaultRainParticleMaterial->inputLayouts = {
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
	lDefaultRainParticleMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultRainParticleMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultRainParticleMaterial->rootSignature->Create(sDevice);

	//ブレンド設定
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultRainParticleMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultRainParticleMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lDefaultRainParticleMaterial->depthFlag = false;

	//生成
	lDefaultRainParticleMaterial->Initialize();

	AddMaterial(lDefaultRainParticleMaterial, "DefaultRainParticle");
}

void MaterialManager::PCreateDefaultPostEffectMaterial()
{
	std::unique_ptr<Material>lDefaultPostEffectMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultPostEffectMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultPostEffectMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/PostEffect/PostEffectTest/PostEffectTestPS.hlsl", "main", "ps_5_0");

	//頂点レイアウト設定
	lDefaultPostEffectMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lDefaultPostEffectMaterial->depthFlag = false;

	lDefaultPostEffectMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultPostEffectMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultPostEffectMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	//ルートシグネチャ設定
	lDefaultPostEffectMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultPostEffectMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultPostEffectMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultPostEffectMaterial->rootSignature->Create(sDevice);

	//生成
	lDefaultPostEffectMaterial->Initialize();

	AddMaterial(lDefaultPostEffectMaterial, "DefaultPostEffect");
}

void MaterialManager::PCreateDefaultIcosahedronParticleMaterial()
{
	std::unique_ptr<Material>lDefaultParticleMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultParticleMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticleVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultParticleMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/2D/Particle/ParticlePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	lDefaultParticleMaterial->geometryShader = CreateUniqueShader("Resources/Shaders/2D/Particle/IcosahedronGS.hlsl", "main", "gs_5_0", IShader::ShaderType::GS);

	//頂点レイアウト設定
	lDefaultParticleMaterial->inputLayouts = {
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
	lDefaultParticleMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultParticleMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultParticleMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultParticleMaterial->rootSignature->Create(sDevice);

	//ブレンド設定
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
	lDefaultParticleMaterial->blenddesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;// 1.0f-ソースのアルファ値

	lDefaultParticleMaterial->primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	lDefaultParticleMaterial->depthFlag = false;

	lDefaultParticleMaterial->cullMode = D3D12_CULL_MODE_NONE;

	//生成
	lDefaultParticleMaterial->Initialize();

	AddMaterial(lDefaultParticleMaterial, "IcosahedronParticle");
}

void MaterialManager::PCreateDefaultToonModelMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 1);//t1
	lDefaultModelMaterial->rootSignature->AddStaticSampler(1);//s1
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultToonModel");
}

void MaterialManager::PCreateDefaultToonModelAnimationMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 1);//t1
	lDefaultModelMaterial->rootSignature->AddStaticSampler(1);//s1
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultToonModelAnimation");
}

void MaterialManager::PCreateDefaultToonModelOutLineAnimationMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelAnimationOutLineVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelOutLinePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultToonModelAnimationOutLine");
}

void MaterialManager::PCreateDefaultToonModelOutLineMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelOutLineVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ToonModelOutLinePS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_FRONT;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultToonModelOutLine");
}

void MaterialManager::PCreateDefaultZeldaToonModelMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ZeldaToonModelVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ZeldaToonModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	lDefaultModelMaterial->depthFlag = true;

	//生成
	lDefaultModelMaterial->Initialize();


	AddMaterial(lDefaultModelMaterial, "DefaultZeldaToonModel");
}

void MaterialManager::PCreateDefaultZeldaToonModelAnimationMaterial()
{
	std::unique_ptr<Material>lDefaultModelMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lDefaultModelMaterial->vertexShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ZeldaToonModelAnimationVS.hlsl");

	//ピクセルシェーダの読み込み
	lDefaultModelMaterial->pixelShader = CreateUniqueShader("Resources/Shaders/3D/Model/ToonModel/ZeldaToonModelPS.hlsl", "main", "ps_5_0", IShader::ShaderType::PS);

	//頂点レイアウト設定
	lDefaultModelMaterial->inputLayouts = {
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float3のPOSITION
	{ "NORMAL",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0   }, // float3のNORMAL
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0  }, // float2のTEXCOORD
	{ "TANGENT",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float3のTANGENT
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float4のCOLOR
	{ "INDEX",		0, DXGI_FORMAT_R32G32B32A32_UINT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // UINT[4]のINDEX
	{ "WEIGHT",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }, // float[4]のWEIGHT
	};

	//ルートシグネチャ設定
	lDefaultModelMaterial->rootSignature = CreateUniqueRootSignature();
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 1);//b1
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 2);//b2
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 3);//b3
	lDefaultModelMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lDefaultModelMaterial->rootSignature->AddStaticSampler(0);//s0
	lDefaultModelMaterial->rootSignature->Create(sDevice);


	lDefaultModelMaterial->blenddesc = PCreateBlend(BlendMode::AX_BLENDMODE_ALPHA);

	lDefaultModelMaterial->cullMode = D3D12_CULL_MODE_BACK;
	//生成
	lDefaultModelMaterial->Initialize();

	AddMaterial(lDefaultModelMaterial, "DefaultZeldaToonModelAnimation");
}

std::unique_ptr<Material> MaterialManager::PCreateDefaultMeshBlend(D3D12_PRIMITIVE_TOPOLOGY_TYPE type_, BlendMode mode_, IShader* vex_, IShader* pix_)
{
	std::unique_ptr<Material> lMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CopyUniqueShader(vex_);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CopyUniqueShader(pix_);

	//頂点レイアウト設定
	lMaterial->inputLayouts = {
		//座標
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//uv座標 
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lMaterial->depthFlag = false;

	lMaterial->primitiveType = type_;

	lMaterial->blenddesc = PCreateBlend(mode_);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lMaterial->rootSignature->Create(sDevice);
	//生成
	lMaterial->Initialize();

	return std::move(lMaterial);
}

std::unique_ptr<Material> MaterialManager::PCreateDefaultSprite2DBlend(BlendMode mode_, IShader* vex_, IShader* pix_)
{
	std::unique_ptr<Material> lMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CopyUniqueShader(vex_);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CopyUniqueShader(pix_);

	//頂点レイアウト設定
	lMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lMaterial->depthFlag = false;

	lMaterial->blenddesc = PCreateBlend(mode_);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0
	lMaterial->rootSignature->Create(sDevice);

	//生成
	lMaterial->Initialize();

	return std::move(lMaterial);
}

std::unique_ptr<Material> MaterialManager::PCreateDefaultSprite3DBlend(BlendMode mode_, IShader* vex_, IShader* pix_)
{
	std::unique_ptr<Material> lMaterial = std::make_unique<Material>();

	//頂点シェーダの読み込み
	lMaterial->vertexShader = CopyUniqueShader(vex_);

	//ピクセルシェーダの読み込み
	lMaterial->pixelShader = CopyUniqueShader(pix_);


	//頂点レイアウト設定
	lMaterial->inputLayouts =
	{
		// xyz座標
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		// uv座標
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
		//カラー
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0},
	};

	lMaterial->blenddesc.AlphaToCoverageEnable = true;

	lMaterial->blenddesc = PCreateBlend(mode_);

	//ルートシグネチャ設定
	lMaterial->rootSignature = CreateUniqueRootSignature();
	lMaterial->rootSignature->Add(IRootSignature::RootType::CBV, 0);//b0
	lMaterial->rootSignature->Add(IRootSignature::RangeType::SRV, 0);//t0
	lMaterial->rootSignature->AddStaticSampler(0);//s0
	lMaterial->rootSignature->Create(sDevice);

	//生成
	lMaterial->Initialize();

	return std::move(lMaterial);
}

D3D12_BLEND_DESC MaterialManager::PCreateBlend(BlendMode mode_)
{

	D3D12_BLEND_DESC lBlendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	switch (mode_)
	{
	case BlendMode::AX_BLENDMODE_NOBLEND:

		break;
	case BlendMode::AX_BLENDMODE_ALPHA:
		lBlendDesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;//加算
		lBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;//ソースの値を100%使う
		lBlendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;//テストの値を0%使う
		// 半透明合成
		lBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;// 加算
		lBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースのアルファ値
		lBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;;// 1.0f-ソースのアルファ値

		break;
	case BlendMode::AX_BLENDMODE_ADD:
		lBlendDesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		lBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_SUB:
		lBlendDesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;// デストからソースを減算
		lBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;// ソースの値を100% 使う
		lBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;// デストの値を100% 使う
		break;
	case BlendMode::AX_BLENDMODE_MULA:
		lBlendDesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_ZERO;
		lBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_SRC_COLOR;
		break;
	case BlendMode::AX_BLENDMODE_INVSRC:
		lBlendDesc.RenderTarget[0].BlendEnable = true;// ブレンドを有効
		lBlendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_INV_DEST_COLOR;// 1.0f-デストカラーの値
		lBlendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;// 使わない
		break;
	case BlendMode::AX_BLENDMODE_MAX:
		break;
	case BlendMode::AX_BLENDMODE_CUSTOM:
		break;
	default:
		break;
	}

	return lBlendDesc;
}
