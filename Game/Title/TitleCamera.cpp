#include "pch.h"
#include "TitleCamera.h"
#include "Game/Components/Look.h"

TitleCamera::TitleCamera()
{
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Initialize(GameObject* target)
{
	m_target = target;
}

void TitleCamera::Update(float elapsedTime)
{
	_Unreferenced_parameter_(elapsedTime);
	using namespace DirectX::SimpleMath;

	SetQuaternion(GetQuaternion() * Quaternion::CreateFromYawPitchRoll(3.14 /180, 0, 0));

	Matrix world = Matrix::CreateTranslation(GetPosition());
	world *= Matrix::CreateFromQuaternion(GetQuaternion());
	
	Vector3 position = { world._41, world._42, world._43 };

	Matrix view = Matrix::CreateLookAt(position, Vector3::Zero, Vector3::Up);

	Graphics::GetInstance()->SetViewMatrix(view);
}
