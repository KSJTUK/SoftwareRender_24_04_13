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
	m_xmf4x4World._41 = x; m_xmf4x4World._42 = y; m_xmf4x4World._43 = z;
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

void CEnemy::Animate(float fElapsedTime)
{
}
