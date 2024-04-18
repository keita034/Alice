#pragma once

#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>
#include<vector>

ALICE_SUPPRESS_WARNINGS_END

#include<Matrix4.h>

/// <summary>
/// ノード
/// </summary>
struct Node
{
	//名前
	std::string name;
	//ローカル変形行列
	AliceMathF::Matrix4 transform;
	//グローバル変形行列
	AliceMathF::Matrix4 globalTransform;
	//親ノード
	Node* parent = nullptr;
	//子ノード
	std::vector<Node*>childrens;

};

