#pragma once
#include<Defined.h>

ALICE_SUPPRESS_WARNINGS_BEGIN

#include<string>
#include<vector>

ALICE_SUPPRESS_WARNINGS_END

namespace AliceUtility
{
	namespace Fille
	{
		/// <summary>
		/// ディレクトリからファイル名一覧を所得
		/// </summary>
		/// <param name="folderPath">ディレクトリパス</param>
		/// <returns>ファイル名一覧</returns>
		std::vector<std::string> GetFileNames(const std::string& folderPath_);

		/// <summary>
		/// ファイルパスからファイル名を取り出す
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>ファイル名</returns>
		std::string GetFileName(std::string& path_);

		/// <summary>
		/// ファイルパスからファイル名を取り出す(拡張子あり)
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>ファイル名</returns>
		std::string ExtractFileName(const std::string& path_);

		/// <summary>
		/// ファイルパスからディレクトリパスを取り出す
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>ディレクトリパス</returns>
		std::string GetDirectoryPath(const std::string& filePath_);

		/// <summary>
		/// ファイルパスから拡張子を取り出す
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <returns>拡張子</returns>
		std::string FileExtension(const std::string& path_);

		/// <summary>
		/// ファイルの拡張子を置き換える
		/// </summary>
		/// <param name="path">ファイルパス</param>
		/// <param name="ext">置き換える拡張子</param>
		std::string ReplaceExtension(const std::string& path_,const char* ext_);

		/// <summary>
		/// ファイルが存在するかどうか
		/// </summary>
		/// <param name="path">ファイルパス</param>
		bool FileIsExist(const std::string& path_);
	}
}
