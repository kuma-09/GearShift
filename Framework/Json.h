#pragma once

namespace Json
{
	void LoadJsonFile(const std::wstring& fileName,std::vector<std::string>& object, std::vector<DirectX::SimpleMath::Vector3>& pos);
}