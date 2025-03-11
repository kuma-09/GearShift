#pragma once

/// <summary>
/// Jsonファイルから情報を取得する関数
/// </summary>
namespace Json
{
	// オブジェクトの名前と座標を取得する
	void LoadJsonFile(const std::wstring& fileName,std::vector<std::string>& object, std::vector<DirectX::SimpleMath::Vector3>& pos);
}