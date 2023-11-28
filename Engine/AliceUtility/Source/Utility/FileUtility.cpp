#include "FileUtility.h"

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<filesystem>
#include<Windows.h>

ALICE_SUPPRESS_WARNINGS_END

	// ディレクトリからファイル名一覧を所得
std::vector<std::string> AliceUtility::Fille::GetFileNames(const std::string& folderPath_)
{
	std::filesystem::directory_iterator lDirectoryIterator(folderPath_),lIteratorEnd;
	std::error_code lErrorCode;
	std::vector<std::string> lFileNames;

	for ( ; lDirectoryIterator != lIteratorEnd && !lErrorCode; lDirectoryIterator.increment(lErrorCode) )
	{
		const std::filesystem::directory_entry lEntry = *lDirectoryIterator;

		lFileNames.push_back(lEntry.path().string());
	}

	return lFileNames;
}

std::string AliceUtility::Fille::GetFileName(std::string& path_)
{
	std::filesystem::path lFilePath = path_;

	return lFilePath.stem().string();
}

std::string AliceUtility::Fille::ExtractFileName(const std::string& path_)
{
	size_t lPos = 0;

	//区切り文字'\\'が出てくる一番最後の部分を検索
	lPos = path_.rfind('\\');
	if ( lPos != std::string::npos )
	{
		return path_.substr(lPos + 1,path_.size() - lPos - 1);
	}

	//区切り文字'\'が出てくる一番最後の部分を検索
	lPos = path_.rfind('/');
	if ( lPos != std::string::npos )
	{
		return path_.substr(lPos + 1,path_.size() - lPos - 1);
	}

	return path_;
}

std::string AliceUtility::Fille::GetDirectoryPath(const std::string& filePath_)
{
	std::filesystem::path lPath = filePath_;
	return lPath.remove_filename().string();
}

std::string AliceUtility::Fille::FileExtension(const std::string& path_)
{
	size_t lIndex = path_.rfind(L'.');
	return path_.substr(lIndex + 1,path_.length() - lIndex - 1);
}

std::string AliceUtility::Fille::ReplaceExtension(const std::string& path_,const char* ext_)
{
	std::filesystem::path lFilePath = path_.c_str();
	return lFilePath.replace_extension(ext_).string();
}

bool AliceUtility::Fille::FileIsExist(const std::string& path_)
{
	FILE* lFile;

	fopen_s(&lFile,path_.c_str(),"r");

	if ( lFile == NULL )
	{
		printf("%sdoes not exist\n",path_.c_str());

		return false;

	}

	fclose(lFile);

	return true;
}
