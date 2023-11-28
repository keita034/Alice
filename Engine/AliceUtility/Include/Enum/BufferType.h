#pragma once

enum class BufferType
{
	MAIN,//メインアダプターで保持
	SUB,//サブアダプターで保持
	SHARED//アダプター同士で共有
};