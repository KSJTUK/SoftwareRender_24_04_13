#include "stdafx.h"
#include "../Header/Enemy.h"

CEnemy::CEnemy()
{
}

CEnemy::~CEnemy()
{
}

void CEnemy::SetPosition(float x, float y, float z)
{
	m_xmf3Position = XMFLOAT3(x, y, z);

	CGameObject::SetPosition(x, y, z);
}

void CEnemy::SetRotation(float x, float y, float z)
{
}

void CEnemy::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
}

void CEnemy::Move(DWORD dwDirection, float fDistance)
{
}

void CEnemy::Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity)
{
}

void CEnemy::Move(float x, float y, float z)
{
}

void CEnemy::Rotate(float fPitch, float fYaw, float fRoll)
{
}

void CEnemy::SetCameraOffset(XMFLOAT3& xmf3CameraOffset)
{
}

void CEnemy::Update(float fTimeElapsed)
{
	Move(m_xmf3Velocity, false);
}

void CEnemy::OnUpdateTransform()
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;
}

void CEnemy::Animate(float fElapsedTime)
{
	OnUpdateTransform();

	CExplosiveObject::Animate(fElapsedTime);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAirplaneEnemy::CAirplaneEnemy()
{
}

CAirplaneEnemy::~CAirplaneEnemy()
{
}

void CAirplaneEnemy::OnUpdateTransform()
{
	CEnemy::OnUpdateTransform();

	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);
}

void CAirplaneEnemy::Animate(float fElapsedTime)
{
	CEnemy::Animate(fElapsedTime);
}
