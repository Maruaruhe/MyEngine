#include "TextureConverter.h"

void TextureConverter::ConvertTextureWIC_To_DDS(const std::string& filePath)
{
	//テクスチャを読み込む
	LoadWICTextureFromFile(filePath);

	//DDS形式に変換して書き出す
	SaveDDSTextureToFile();
}

void TextureConverter::LoadWICTextureFromFile(const std::string& filePath)
{
	//ファイルパスをワイド文字列に変換
	std::wstring wFilePath = ConvertMultiByteStringToWideString(filePath);

	//テクスチャを読み込む
	HRESULT hr = DirectX::LoadFromWICFile(wFilePath.c_str(), DirectX::WIC_FLAGS_NONE, &metaData_, scratchImage_);
	assert(SUCCEEDED(hr));

	//フォルダパスとファイル名を分離する
	SeparateFilePath(wFilePath);

	//DDSテクスチャとしてファイル書き出し
	SaveDDSTextureToFile();
}

std::wstring TextureConverter::ConvertMultiByteStringToWideString(const std::string& mString)
{
	// ワイド文字列に変換した際の文字数を計算
	int filePathBufferSize = MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, nullptr, 0);

	// ワイド文字
	std::wstring wString;
	wString.resize(filePathBufferSize);

	// ワイド文字列に変換
	MultiByteToWideChar(CP_ACP, 0, mString.c_str(), -1, &wString[0], filePathBufferSize);

	return wString;
}

void TextureConverter::SeparateFilePath(const std::wstring& filePath)
{
	size_t pos1 = 0;
	std::wstring exceptExt;

	// 区切り文字 '.' が出てくる一番最後の部分を検索
	pos1 = filePath.rfind('.');
	// 検索がヒットしたら
	if (pos1 != std::wstring::npos) {
		// 区切り文字の後ろをファイル拡張子として保存
		fileExt_ = filePath.substr(pos1 + 1, filePath.size() - pos1 - 1);
		// 区切り文字の前までを抜き出す
		exceptExt = filePath.substr(0, pos1);
	}
	else {
		fileExt_ = L"";
		exceptExt = filePath;
	}
	// 区切り文字 '\\' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('\\');
	if (pos1 != std::wstring::npos) {
		// 区切り文字の前までをディレクトリパスとして保存
		directoryPath_ = exceptExt.substr(0, pos1 + 1);
		// 区切り文字の後ろをファイル名として保存
		fileName_ = exceptExt.substr(pos1 + 1, exceptExt.size() - pos1 - 1);
		return;
	}

	// 区切り文字 '/' が出てくる一番最後の部分を検索
	pos1 = exceptExt.rfind('/');
	if (pos1 != std::wstring::npos) {
		// 区切り文字がないのでファイル名のみとして扱う
		directoryPath_ = L"";
		fileName_ = exceptExt;
	}
}

void TextureConverter::SaveDDSTextureToFile()
{
	// 読み込んだテクスチャをSRGBとして扱う
	metaData_.format = DirectX::MakeSRGB(metaData_.format);

	HRESULT hr{};

	// 出力ファイル名を設定する
	std::wstring filePath = directoryPath_ + fileName_ + L".dds";

	// DDSファイル書き出し
	hr = DirectX::SaveToDDSFile(
		scratchImage_.GetImages(), scratchImage_.GetImageCount(),
		metaData_, DirectX::DDS_FLAGS_NONE, filePath.c_str());
	assert(SUCCEEDED(hr));
}