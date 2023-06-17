#include<3DMesh.h>


std::unique_ptr<Mesh3D> Mesh3D::mesh = nullptr;
IWindowsApp* Mesh3D::sWindowsApp = nullptr;
ID3D12GraphicsCommandList* Mesh3D::sCmdList = nullptr;

#pragma region パブリック

//線を描画する
void Mesh3D::DrawLine(const AliceMathF::Vector3& start_,const AliceMathF::Vector3& end_,const AliceMathF::Vector4& color_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = lineBuff->vertexBuffer->GetView();

	assert(lineCount < lineMaxCount);

	// 頂点データ
	std::vector <PosColor > lVertices = {
	  {{start_.x, start_.y, start_.z}, {color_.x, color_.y, color_.z, color_.w}},
	  {{end_.x, end_.y, end_.z}, {color_.x, color_.y, color_.z, color_.w}},
	};

	uint32_t lIndxcount = lineCount * lineVertexCount;

	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//頂点バッファへのデータ転送
	copy(lVertices.begin(), lVertices.end(), &lineBuff->vertMap[lIndxcount]);
	lineBuff->vertexBuffer->Update(lVertices.data());

	switch (blendMode)
	{
	case 0:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineNoblend")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineNoblend")->rootSignature->GetRootSignature());
		break;
	case 1:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineAlpha")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineAlpha")->rootSignature->GetRootSignature());
		break;
	case 2:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineAdd")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineAdd")->rootSignature->GetRootSignature());
		break;
	case 3:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineSub")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineSub")->rootSignature->GetRootSignature());
		break;
	case 4:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineMula")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineMula")->rootSignature->GetRootSignature());
		break;
	case 5:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashLineInvsrc")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashLineInvsrc")->rootSignature->GetRootSignature());
		break;
	}
	// プリミティブ形状の設定コマンド
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &lVbView);

	//定数バッファビュー
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// 描画コマンド
	sCmdList->DrawInstanced(lineVertexCount, 1, lIndxcount, 0);

	lineCount++;
}

//カウント初期化
void Mesh3D::DrawReset()
{
	triangleCount = 0;
	lineCount = 0;
	boxCount = 0;
}

//ブレンドモードを設定する
void Mesh3D::SetBlendMode(BlendMode mode_)
{
	blendMode = static_cast<uint32_t>(mode_);
}

//色コードを取得する
AliceMathF::Vector4 Mesh3D::GetColor(int32_t red_, int32_t blue_, int32_t green_, int32_t alpha_)
{
	AliceMathF::Vector4 lResult{};

	lResult.x = static_cast<float>(red_) / 255.0f;
	lResult.y = static_cast<float>(blue_) / 255.0f;
	lResult.z = static_cast<float>(green_) / 255.0f;
	lResult.w = static_cast<float>(alpha_) / 255.0f;

	return lResult;
}

AliceMathF::Vector4 Mesh3D::GetColor(const AliceMathF::Vector4& color_)
{
	AliceMathF::Vector4 lResult{};

	lResult.x = static_cast<float>(color_.x) / 255.0f;
	lResult.y = static_cast<float>(color_.y) / 255.0f;
	lResult.z = static_cast<float>(color_.z) / 255.0f;
	lResult.w = static_cast<float>(color_.w) / 255.0f;

	return lResult;
}


//コンストラクタ
Mesh3D::Mesh3D()
{
	//三角形バッファ
	uint32_t lVertexCount = triangleVertexCount * triangleMaxCount;
	uint32_t lIndexCount = triangleIndexCount * triangleMaxCount;
	triangleBuff = PCreateBuff(lVertexCount, lIndexCount);

	//線バッファ
	lVertexCount = lineVertexCount * lineMaxCount;
	lIndexCount = lineIndexCount * lineMaxCount;
	lineBuff = PCreateBuff(lVertexCount, lIndexCount);

	//四角形バッファ
	lVertexCount = boxVertexCount * boxMaxCount;
	lIndexCount = boxIndexCount * boxMaxCount;
	boxBuff = PCreateBuff(lVertexCount, lIndexCount);

	PCreatConstBuff();

}

void Mesh3D::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}

void Mesh3D::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sCmdList = directX12Core_->GetCommandList();
}

#pragma endregion

#pragma region プライベート

Mesh3D* Mesh3D::GetInstance()
{
	if (!mesh)
	{
		Mesh3D* lMesh3D = new Mesh3D();
		mesh.reset(lMesh3D);
	}

	return mesh.get();
}

void Mesh3D::SetCamera(GameCamera* camera_)
{
	camera = camera_;
}

//三角形を描画する(中身塗りつぶし)
void Mesh3D::PPrawTriangleFill(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_,const AliceMathF::Vector4& color_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = triangleBuff->vertexBuffer->GetView();

	assert(triangleCount < triangleMaxCount);

	// 頂点データ
	std::vector <PosColor > lVertices = {
	  {{x1_, y1_, 0.0f}, {color_.x, color_.y, color_.z, color_.w}},
	  {{x2_, y2_, 0.0f}, {color_.x, color_.y, color_.z, color_.w}},
	  {{x3_, y3_, 0.0f}, {color_.x, color_.y, color_.z, color_.w}},
	};
	uint32_t lVertexCount = triangleCount * triangleVertexCount;
	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//頂点バッファへのデータ転送
	copy(lVertices.begin(), lVertices.end(), &triangleBuff->vertMap[lVertexCount]);
	triangleBuff->vertexBuffer->Update(triangleBuff->vertMap);

	switch (blendMode)
	{
	case 0:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleNoblend")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleNoblend")->rootSignature->GetRootSignature());
		break;
	case 1:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleAlpha")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleAlpha")->rootSignature->GetRootSignature());
		break;
	case 2:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleAdd")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleAdd")->rootSignature->GetRootSignature());
		break;
	case 3:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleSub")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleSub")->rootSignature->GetRootSignature());
		break;
	case 4:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleMula")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleMula")->rootSignature->GetRootSignature());
		break;
	case 5:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleInvsrc")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleInvsrc")->rootSignature->GetRootSignature());
		break;
	}

	// プリミティブ形状の設定コマンド
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	// 頂点バッファビューの設定コマンド
	sCmdList->IASetVertexBuffers(0, 1, &lVbView);

	//定数バッファビュー
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	// 描画コマンド
	sCmdList->DrawInstanced(triangleVertexCount, 1, lVertexCount, 0);

	// 使用カウント上昇
	triangleCount++;
}

//四角形を描画する(中身塗りつぶし)
void Mesh3D::PDrawBoxFill(float x_, float y_, float radiusX_, float radiusY_, float angle_, const AliceMathF::Vector4& color_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = boxBuff->vertexBuffer->GetView();
	D3D12_INDEX_BUFFER_VIEW lIbView = boxBuff->indexBuffer->GetView();

	assert(boxCount < boxMaxCount);

	float lLeft = -radiusX_;
	float lTop = -radiusY_;
	float lRight = radiusX_;
	float lBottom = radiusY_;

	// 頂点データ
	std::vector <PosColor > lVertices = {
	  {{lLeft, lBottom, 0.0f},{color_.x, color_.y, color_.z, color_.w}},//左下
	  {{lLeft, lTop, 0.0f},{color_.x, color_.y, color_.z, color_.w}},//左上
	  {{lRight, lTop, 0.0f},{color_.x, color_.y, color_.z, color_.w}},//右上
	  {{lRight, lBottom, 0.0f},{color_.x, color_.y, color_.z, color_.w}},//右下
	};

	//インデックス
	std::vector<uint16_t> lIndices = { 0, 1, 2, 2, 3, 0 };

	constMapTransform->mat = camera->GetViewMatrixInv() * camera->GetProjectionMatrix();

	//回転
	for (auto& vertex : lVertices)
	{
		//回転
		vertex.pos = {
		  vertex.pos.x * cosf(angle_) + vertex.pos.y * -sinf(angle_),
		  vertex.pos.x * sinf(angle_) + vertex.pos.y * cosf(angle_), vertex.pos.z };

		//平行移動
		vertex.pos.x += x_;
		vertex.pos.y += y_;
	}

	uint32_t lVertexCount = boxCount * boxVertexCount;
	uint32_t lIndxCount = boxCount * boxIndexCount;

	//頂点バッファへのデータ転送
	std::copy(lVertices.begin(), lVertices.end(), &boxBuff->vertMap[lVertexCount]);
	boxBuff->vertexBuffer->Update(boxBuff->vertMap);

	//インデックスバッファへのデータ転送
	std::copy(lIndices.begin(), lIndices.end(), &boxBuff->indexMap[lIndxCount]);
	boxBuff->indexBuffer->Update(boxBuff->indexMap);

	switch (blendMode)
	{
	case 0:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleNoblend")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleNoblend")->rootSignature->GetRootSignature());
		break;
	case 1:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleAlpha")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleAlpha")->rootSignature->GetRootSignature());
		break;
	case 2:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleAdd")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleAdd")->rootSignature->GetRootSignature());
		break;
	case 3:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleSub")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleSub")->rootSignature->GetRootSignature());
		break;
	case 4:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleMula")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleMula")->rootSignature->GetRootSignature());
		break;
	case 5:
		// パイプラインステートの設定
		sCmdList->SetPipelineState(MaterialManager::SGetMaterial("MashTriangleInvsrc")->pipelineState->GetPipelineState());
		// ルートシグネチャの設定
		sCmdList->SetGraphicsRootSignature(MaterialManager::SGetMaterial("MashTriangleInvsrc")->rootSignature->GetRootSignature());
		break;
	}

	//プリミティブ形状の設定コマンド
	sCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //三角形リスト

	//頂点バッファの設定
	sCmdList->IASetVertexBuffers(0, 1, &lVbView);

	//インデックスバッファの設定
	sCmdList->IASetIndexBuffer(&lIbView);

	//定数バッファビュー
	sCmdList->SetGraphicsRootConstantBufferView(0, constBuffTransform->GetAddress());

	//描画コマンド
	sCmdList->DrawIndexedInstanced(boxIndexCount, 1, static_cast<uint32_t>(lIndxCount), static_cast<INT>(lVertexCount), 0);

	//使用カウント上昇
	boxCount++;
}

//定数バッファ生成(2D座標変換行列)
void Mesh3D::PCreatConstBuff()
{
	//定数バッファ生成
	constBuffTransform = CreateUniqueConstantBuffer(sizeof(ConstBufferDataTransform));

	AliceMathF::MakeOrthogonalLOffCenter(
		0.0f, static_cast<float>(sWindowsApp->GetWindowSize().width),
		static_cast<float>(sWindowsApp->GetWindowSize().height), 0.0f,
		0.0f, 1.0f, projectionMat);

	constBuffTransform->Update(&projectionMat);

}

///<summary>
///バッファ作成
///</summary>
///<param name="vertexCount">頂点数</param>
///<param name="indexCount">インデックス数</param>
///<returns>バッファ</returns>
std::unique_ptr <Buff> Mesh3D::PCreateBuff(uint32_t vertexCount_, uint32_t indexCount_)
{
	std::unique_ptr <Buff> lBuff = std::make_unique<Buff>();

	if (vertexCount_ > 0)
	{

		lBuff->vertexBuffer = CreateUniqueVertexBuffer(vertexCount_, sizeof(PosColor));

		lBuff->vertMap = static_cast<PosColor*>(malloc(vertexCount_ * sizeof(PosColor)));

	}

	if (indexCount_ > 0)
	{
		lBuff->indexBuffer = CreateUniqueIndexBuffer(indexCount_);

		lBuff->indexMap = static_cast<uint32_t*>(malloc(indexCount_ * sizeof(uint32_t)));
	}

	return std::move(lBuff);
}

#pragma endregion
