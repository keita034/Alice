#include "AliceMotionData.h"
#include"AliceFunctionUtility.h"
#include "AliceFileStream.h"


std::vector<std::string>AliceMotionData::filePaths;
std::unordered_map<std::string, std::unique_ptr<MotionData>> AliceMotionData::motionDatas;
uint32_t AliceMotionData::modelCount;

float AliceMotionData::GetAnimeMaxflame()
{
	return motionData->duration;
}

float AliceMotionData::GetTickTimes(float frame)
{
	float TicksPerSecond = motionData->ticksPerSecond != 0 ? motionData->ticksPerSecond : 25.0f;

	return  frame * TicksPerSecond;
}

void AliceMotionData::SetMotion(uint32_t motionHandle)
{
	motionData = motionDatas[filePaths[motionHandle]].get();
}

uint32_t AliceMotionData::CreateMotion(const std::string& fileDirectoryPath)
{
	std::string path = fileDirectoryPath;

	//一回読み込んだことがあるファイルはそのまま返す
	auto itr = find_if(motionDatas.begin(), motionDatas.end(), [&](std::pair<const std::string, std::unique_ptr<MotionData, std::default_delete<MotionData>>>& p)
		{
			return p.second->filePath == path;
		});

	if (itr == motionDatas.end())
	{

		uint32_t modelHandle = modelCount;

		std::unique_ptr<MotionData> data;
		data = std::make_unique<MotionData>();

		{
			std::vector<std::string> files;

			std::string filePath;

			files = AliceFunctionUtility::getFileNames(fileDirectoryPath);

			//ディレクトリからFBXファイルを探す
			for (std::string file : files)
			{
				if (file.find(".almb") != std::string::npos || file.find(".alm") != std::string::npos)
				{
					filePath = file;
				}
			}

			std::string fileExtension = AliceFunctionUtility::FileExtension(filePath);

			if (fileExtension == "almb")
			{
				if (!AliceFileStream::LoadAliceMotionBinaryData(filePath, data.get()))
				{
					assert(0);
				}
			}
			else if (fileExtension == "alm")
			{
				if (!AliceFileStream::LoadAliceMotionData(filePath, data.get()))
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}

		data->modelHandle = modelCount;

		data->filePath = path;

		motionDatas[path] = std::move(data);

		filePaths[modelCount] = path;

		modelCount++;

		return modelHandle;
	}
	else
	{

		uint32_t modelHandle = motionDatas[path]->modelHandle;

		return modelHandle;
	}

	return 0;
}

void AliceMotionData::CommonInitialize()
{
	filePaths.resize(maxMotion);
}
