#include<Mesh.h>

Mesh* Mesh::mesh = nullptr;
IWindowsApp* Mesh::sWindowsApp = nullptr;
ID3D12GraphicsCommandList* Mesh::sCmdList = nullptr;

#pragma region パブリック

//線を描画する
void Mesh::DrawLine(float x1_, float y1_, float x2_, float y2_, const AliceMathF::Vector4& color_)
{
	D3D12_VERTEX_BUFFER_VIEW lVbView = lineBuff->vertexBuffer->GetView();

	assert(lineCount < lineMaxCount);

	// 頂点データ
	std::vector <PosColor > lVertices = {
	  {{x1_, y1_, 0.0f}, {color_.x, color_.y, color_.z, color_.w}},
	  {{x2_, y2_, 0.0f}, {color_.x, color_.y, color_.z, color_.w}},
	};

	uint32_t lIndxcount = lineCount * lineVertexCount;

	//頂点バッファへのデータ転送
	copy(lVertices.begin(), lVertices.end(), &lineBuff->vertMap[lIndxcount]);
	lineBuff->vertexBuffer->Update(lineBuff->vertMap);

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

//三角形を描画する
void Mesh::DrawTriangle(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_,const AliceMathF::Vector4& color_, int32_t fillFlag_)
{
	if (fillFlag_)
	{
		PDrawTriangleFill(x1_, y1_, x2_, y2_, x3_, y3_, color_);
	}
	else
	{
		DrawLine(x1_, y1_, x2_, y2_, color_);
		DrawLine(x2_, y2_, x3_, y3_, color_);
		DrawLine(x3_, y3_, x1_, y1_, color_);

	}
}

//四角形を描画する
void Mesh::DrawBox(float x_, float y_, float radiusX_, float radiusY_, float angle_,const AliceMathF::Vector4& color_, int32_t fillFlag_)
{
	if (fillFlag_)
	{
		PDrawBoxFill(x_, y_, radiusX_, radiusY_, angle_, color_);

	}
	else
	{

		DrawLine(x_ - radiusX_, y_ - radiusY_, x_ + radiusX_, y_ - radiusY_, color_);//左上から右上
		DrawLine(x_ - radiusX_, y_ - radiusY_, x_ - radiusX_, y_ + radiusY_, color_);//左上から左下
		DrawLine(x_ + radiusX_, y_ - radiusY_, x_ + radiusX_, y_ + radiusY_, color_);//右上から右下
		DrawLine(x_ - radiusX_, y_ + radiusY_, x_ + radiusX_, y_ + radiusY_, color_);//左下から右下
	}
}

//楕円を描画する
void Mesh::DrawEllipse(float x_, float y_, float radiusX_, float radiusY_, float angle_,const AliceMathF::Vector4& color_, int32_t fillMode_)
{
	const uint32_t DIV_NUM = 64; // 分割数
	float lRadianPerDivision = AliceMathF::AX_2PI / static_cast<float>(DIV_NUM); // 分割1単位の角度

	//分割点分割数より1つ多くしておいて1周回るようにする
	std::array<float, DIV_NUM + 1> lPointsX{};
	std::array<float, DIV_NUM + 1> lPointsY{};
	float lAngleCos = cos(angle_);
	float lAngleSin = sin(angle_);
	for (size_t i = 0; i < DIV_NUM; ++i)
	{
		float lBaseX = cos(lRadianPerDivision * static_cast<float>(i)) * radiusX_;
		float lBaseY = sin(lRadianPerDivision * static_cast<float>(i)) * radiusY_;
		lPointsX[i] = static_cast<float>(lBaseY * lAngleCos - lBaseX * lAngleSin + x_);
		lPointsY[i] = static_cast<float>(lBaseY * lAngleCos + lBaseX * lAngleSin + y_);
	}

	//最後と最初は一緒
	(*lPointsX.rbegin()) = (*lPointsX.begin());
	(*lPointsY.rbegin()) = (*lPointsY.begin());

	if (fillMode_)
	{
		for (size_t index = 0; index < DIV_NUM; ++index)
		{
			PDrawTriangleFill(x_, y_, lPointsX[index], lPointsY[index], lPointsX[index + 1], lPointsY[index + 1], color_);
		}
	}
	else
	{
		for (size_t index = 0; index < DIV_NUM; ++index)
		{
			DrawLine(lPointsX[index], lPointsY[index], lPointsX[index + 1], lPointsY[index + 1], color_);
		}
	}
}

//カウント初期化
void Mesh::DrawReset()
{
	triangleCount = 0;
	lineCount = 0;
	boxCount = 0;
}

//ブレンドモードを設定する
void Mesh::SetBlendMode(BlendMode mode)
{
	blendMode = static_cast<uint32_t>(mode);
}

//色コードを取得する
AliceMathF::Vector4 Mesh::GetColor(int32_t red, int32_t blue, int32_t green, int32_t alpha)
{
	AliceMathF::Vector4 lColor{};

	lColor.x = static_cast<float>(red) / 255.0f;
	lColor.y = static_cast<float>(blue) / 255.0f;
	lColor.z = static_cast<float>(green) / 255.0f;
	lColor.w = static_cast<float>(alpha) / 255.0f;

	return lColor;
}

AliceMathF::Vector4 Mesh::GetColor(const AliceMathF::Vector4& color_)
{
	AliceMathF::Vector4 lColor{};

	lColor.x = static_cast<float>(color_.x) / 255.0f;
	lColor.y = static_cast<float>(color_.y) / 255.0f;
	lColor.z = static_cast<float>(color_.z) / 255.0f;
	lColor.w = static_cast<float>(color_.w) / 255.0f;

	return lColor;
}


//コンストラクタ
Mesh::Mesh()
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
	lVertexCount = boxIndexCount * boxMaxCount;
	boxBuff = PCreateBuff(lVertexCount, lIndexCount);

	PCreatConstBuff();

}

#pragma endregion

#pragma region プライベート

Mesh* Mesh::GetInstance()
{
	if (mesh == nullptr)
	{
		mesh = new Mesh();
	}

	return mesh;
}

void Mesh::Destroy()
{
	free(triangleBuff->vertMap);
	free(triangleBuff->indexMap);
	free(lineBuff->vertMap);
	free(lineBuff->indexMap);
	free(boxBuff->vertMap);
	free(boxBuff->indexMap);

	delete mesh;
}

void Mesh::SSetWindowsApp(IWindowsApp* windowsApp_)
{
	sWindowsApp = windowsApp_;
}

void Mesh::SSetDirectX12Core(DirectX12Core* directX12Core_)
{
	sCmdList = directX12Core_->GetCommandList();
}

//三角形を描画する(中身塗りつぶし)
void Mesh::PDrawTriangleFill(float x1_, float y1_, float x2_, float y2_, float x3_, float y3_, const AliceMathF::Vector4& color_)
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
void Mesh::PDrawBoxFill(float x_, float y_, float radiusX_, float radiusY_, float angle_, AliceMathF::Vector4 color_)
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
	sCmdList->DrawIndexedInstanced(boxIndexCount, 1, static_cast<UINT>(lIndxCount), static_cast<INT>(lVertexCount), 0);

	//使用カウント上昇
	boxCount++;
}


//定数バッファ生成(2D座標変換行列)
void Mesh::PCreatConstBuff()
{
	//定数バッファ生成
	constBuffTransform = CreateUniqueConstantBuffer(sizeof(ConstBufferDataTransform));

	AliceMathF::MakeOrthogonalLOffCenter(
		0.0f, static_cast<float>(sWindowsApp->GetWindowSize().width),
		static_cast<float>(sWindowsApp->GetWindowSize().height), 0.0f,
		0.0f, 1.0f, constMapTransform.mat);

	constBuffTransform->Update(&constMapTransform);

}


///<summary>
///バッファ作成
///</summary>
///<param name="vertexCount">頂点数</param>
///<param name="indexCount">インデックス数</param>
///<returns>バッファ</returns>
std::unique_ptr <Buff> Mesh::PCreateBuff(uint32_t vertexCount, uint32_t indexCount)
{
	std::unique_ptr <Buff> lBuff = std::make_unique<Buff>();

	if (vertexCount > 0)
	{

		lBuff->vertexBuffer = CreateUniqueVertexBuffer(vertexCount, sizeof(PosColor));

		lBuff->vertMap = static_cast<PosColor*>(malloc(vertexCount * sizeof(PosColor)));

	}

	if (indexCount > 0)
	{
		lBuff->indexBuffer = CreateUniqueIndexBuffer(indexCount);

		lBuff->indexMap = static_cast<uint32_t*>(malloc(indexCount * sizeof(uint32_t)));
	}

	return std::move(lBuff);
}

#pragma endregion
