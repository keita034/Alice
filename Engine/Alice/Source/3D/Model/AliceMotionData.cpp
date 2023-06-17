#include<AliceMotionData.h>
#include<AliceFunctionUtility.h>
#include<AliceFileStream.h>


std::vector<std::string>AliceMotionData::sFilePaths;
std::unordered_map<std::string, std::unique_ptr<MotionData>> AliceMotionData::sMotionDatas;
uint32_t AliceMotionData::sModelCount;

float AliceMotionData::GetAnimeMaxflame()const
{
	return motionData->duration;
}

float AliceMotionData::GetTickTimes(float frame_)
{
	float lTicksPerSecond_ = motionData->ticksPerSecond != 0 ? motionData->ticksPerSecond : 25.0f;

	return  frame_ * lTicksPerSecond_;
}

void AliceMotionData::SetMotion(uint32_t motionHandle_)
{
	motionData = sMotionDatas[sFilePaths[motionHandle_]].get();
}

uint32_t AliceMotionData::SCreateMotion(const std::string& fileDirectoryPath_)
{
	std::string lDirectoryPath = fileDirectoryPath_;

	//一回読み込んだことがあるファイルはそのまま返す
	auto lMotionItr = find_if(sMotionDatas.begin(), sMotionDatas.end(), [&](std::pair<const std::string, std::unique_ptr<MotionData, std::default_delete<MotionData>>>& motion)
		{
			return motion.second->filePath == lDirectoryPath;
		});

	if (lMotionItr == sMotionDatas.end())
	{
		uint32_t lModelHandle = sModelCount;

		std::unique_ptr<MotionData> lMotionData;
		lMotionData = std::make_unique<MotionData>();

		{
			std::vector<std::string> lFiles;

			std::string lFilePath;

			lFiles = AliceFunctionUtility::getFileNames(fileDirectoryPath_);

			//ディレクトリからFBXファイルを探す
			for (std::string file : lFiles)
			{
				if (file.find(".almb") != std::string::npos || file.find(".alm") != std::string::npos)
				{
					lFilePath = file;
				}
			}

			std::string lFileExtension = AliceFunctionUtility::FileExtension(lFilePath);

			if (lFileExtension == "almb")
			{
				if (!AliceFileStream::SLoadAliceMotionBinaryData(lFilePath, lMotionData.get()))
				{
					assert(0);
				}
			}
			else if (lFileExtension == "alm")
			{
				if (!AliceFileStream::SLoadAliceMotionData(lFilePath, lMotionData.get()))
				{
					assert(0);
				}
			}
			else
			{
				assert(0);
			}
		}

		lMotionData->modelHandle = sModelCount;

		lMotionData->filePath = lDirectoryPath;

		sMotionDatas[lDirectoryPath] = std::move(lMotionData);

		sFilePaths[sModelCount] = lDirectoryPath;

		sModelCount++;

		return lModelHandle;
	}
	else
	{

		uint32_t lModelHandle = sMotionDatas[lDirectoryPath]->modelHandle;

		return lModelHandle;
	}

	return 0;
}

void AliceMotionData::SCommonInitialize()
{
	sFilePaths.resize(sMAX_MOTION);
}
