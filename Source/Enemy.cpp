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
	m_qxmf3RotateDests.push(Random::RandomFloat3(-1.0f, 1.0f, true));

	CGameObject::SetPosition(x, y, z);
}

void CEnemy::SetRotation(float x, float y, float z)
{
}

void CEnemy::LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up)
{
	XMFLOAT4X4 xmf4x4View = Matrix4x4::LookAtLH(m_xmf3Position, xmf3LookAt, xmf3Up);
	m_xmf3Right = Vector3::Normalize(XMFLOAT3(xmf4x4View._11, xmf4x4View._21, xmf4x4View._31));
	m_xmf3Up = Vector3::Normalize(XMFLOAT3(xmf4x4View._12, xmf4x4View._22, xmf4x4View._32));
	m_xmf3Look = Vector3::Normalize(XMFLOAT3(xmf4x4View._13, xmf4x4View._23, xmf4x4View._33));
}

void CEnemy::Rotate(float fPitch, float fYaw, float fRoll)
{
	if (fPitch != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Right), XMConvertToRadians(fPitch));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
	}
	if (fYaw != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Up), XMConvertToRadians(fYaw));
		m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}
	if (fRoll != 0.0f)
	{
		XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&m_xmf3Look), XMConvertToRadians(fRoll));
		m_xmf3Up = Vector3::TransformNormal(m_xmf3Up, mtxRotate);
		m_xmf3Right = Vector3::TransformNormal(m_xmf3Right, mtxRotate);
	}

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Up, m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
	m_xmf3MovingDirection = m_xmf3Look;
}

void CEnemy::RotateLook(XMFLOAT3& xmf3Axis, float fAngle)
{
	XMMATRIX mtxRotate = XMMatrixRotationAxis(XMLoadFloat3(&xmf3Axis), XMConvertToRadians(fAngle));
	m_xmf3Look = Vector3::TransformNormal(m_xmf3Look, mtxRotate);

	m_xmf3Look = Vector3::Normalize(m_xmf3Look);
	m_xmf3Right = Vector3::Normalize(Vector3::CrossProduct(XMFLOAT3(0.f, 1.f, 0.f), m_xmf3Look));
	m_xmf3Up = Vector3::Normalize(Vector3::CrossProduct(m_xmf3Look, m_xmf3Right));
	m_xmf3MovingDirection = m_xmf3Look;
}

void CEnemy::RotateToSmoothly(XMFLOAT3& xmf3Dest, float fSpeed)
{
	static float fEqualAngleDegree = 5.0f;
	float fAngle = XMConvertToDegrees(Vector3::Angle(m_xmf3Look, xmf3Dest));
	if (fAngle < fEqualAngleDegree) {
		m_qxmf3RotateDests.pop();
		return;
	}

	XMFLOAT3 xmf3RotationAxis = Vector3::CrossProduct(m_xmf3Look, xmf3Dest);
	if (IsZero(xmf3RotationAxis)) xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	RotateLook(xmf3RotationAxis, fSpeed);
}

void CEnemy::CancelLastMove(float fElapsedTime)
{
	Move(m_xmf3Look, -m_fMovingSpeed * fElapsedTime);
}

void CEnemy::Update(float fTimeElapsed)
{
	m_xmf3Position.x = m_xmf4x4World._41;
	m_xmf3Position.y = m_xmf4x4World._42;
	m_xmf3Position.z = m_xmf4x4World._43;
}

void CEnemy::OnUpdateTransform()
{
	m_xmf4x4World._11 = m_xmf3Right.x; m_xmf4x4World._12 = m_xmf3Right.y; m_xmf4x4World._13 = m_xmf3Right.z;
	m_xmf4x4World._21 = m_xmf3Up.x; m_xmf4x4World._22 = m_xmf3Up.y; m_xmf4x4World._23 = m_xmf3Up.z;
	m_xmf4x4World._31 = m_xmf3Look.x; m_xmf4x4World._32 = m_xmf3Look.y; m_xmf4x4World._33 = m_xmf3Look.z;
	//m_xmf4x4World._41 = m_xmf3Position.x; m_xmf4x4World._42 = m_xmf3Position.y; m_xmf4x4World._43 = m_xmf3Position.z;
}

void CEnemy::Animate(float fElapsedTime)
{
	if (!m_qxmf3RotateDests.empty())
		RotateToSmoothly(m_qxmf3RotateDests.front(), 100.0f * fElapsedTime);

	OnUpdateTransform();
	CExplosiveObject::Animate(fElapsedTime);
	Update(fElapsedTime);
}

void CEnemy::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CExplosiveObject::Render(hDCFrameBuffer, pCamera);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "Header/Player.h"
CAirplaneEnemy::CAirplaneEnemy()
{
	CCubeMesh* pBulletMesh = new CCubeMesh(1.0f, 4.0f, 1.0f);
	for (int i = 0; i < ENEMY_BULLETS; i++)
	{
		m_ppBullets[i] = new CBulletObject(m_fBulletEffectiveRange);
		m_ppBullets[i]->SetMesh(pBulletMesh);
		m_ppBullets[i]->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_ppBullets[i]->SetRotationSpeed(360.0f);
		m_ppBullets[i]->SetMovingSpeed(120.0f);
		m_ppBullets[i]->SetActive(false);
	}
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
	m_fElapsedChangingDirection += fElapsedTime;
	if (DetectTarget()) {
		ChaseTarget(fElapsedTime);
		m_fElapsedFromLastFire += fElapsedTime;
		if (m_fElapsedFromLastFire > m_fFireDelay) {
			FireBullet();
			m_fElapsedFromLastFire = 0.f;
		}
	}
	else {
		SetRotationSpeed(0.0f);
		m_fMovingSpeed = 10.0f;
		if (m_fChangeDirectionTime < m_fElapsedChangingDirection && m_qxmf3RotateDests.empty()) {
			m_qxmf3RotateDests.push(Random::RandomFloat3(-1.0f, 1.0f, true));
			m_fMovingSpeed = Random::RandomFloat(0.0f, 20.0f);
			m_fElapsedChangingDirection = 0.0f;
		}
	}

	CEnemy::Animate(fElapsedTime);

	for (int i = 0; i < ENEMY_BULLETS; ++i)
		if (m_ppBullets[i]->m_bActive)
			m_ppBullets[i]->Animate(fElapsedTime);
}

void CAirplaneEnemy::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	CEnemy::Render(hDCFrameBuffer, pCamera);

	for (int i = 0; i < ENEMY_BULLETS; ++i)
		if (m_ppBullets[i]->m_bActive)
			m_ppBullets[i]->Render(hDCFrameBuffer, pCamera);
}

void CAirplaneEnemy::AfterCollision(const CGameObject* pCollObject, float fElapsedTime)
{
	CancelLastMove(fElapsedTime * 1.5f);
	while (!m_qxmf3RotateDests.empty()) m_qxmf3RotateDests.pop();
	m_qxmf3RotateDests.push(m_xmf3MovingDirection);
}

void CAirplaneEnemy::AfterWallCollision(float fElapsedTime)
{
	CancelLastMove(fElapsedTime * 1.5f);
	while (!m_qxmf3RotateDests.empty()) m_qxmf3RotateDests.pop();
	m_qxmf3RotateDests.push(m_xmf3MovingDirection);
}

bool CAirplaneEnemy::DetectTarget()
{
	if (!m_pTargetPlayer || m_pTargetPlayer->m_bBlowingUp) return false;

	XMFLOAT3 targetPosition = m_pTargetPlayer->m_xmf3Position;
	if (Vector3::Distance(m_xmf3Position, targetPosition) > m_fDetectRange) return false;

	return true;
}

void CAirplaneEnemy::ChaseTarget(float fElapsedTime)
{
	XMFLOAT3 targetPosition = m_pTargetPlayer->m_xmf3Position;

	m_fMovingSpeed = 0.0f;
	XMFLOAT3 xmf3ToTarget = Vector3::Subtract(targetPosition, m_xmf3Position);
	XMFLOAT3 xmf3RotationAxis = Vector3::CrossProduct(m_xmf3Look, xmf3ToTarget);
	float fAngle = XMConvertToDegrees(Vector3::Angle(m_xmf3Look, xmf3ToTarget));

	if (IsZero(xmf3RotationAxis)) {
		xmf3RotationAxis = XMFLOAT3(0.0f, 1.0f, 0.0f);
	}

	RotateLook(
		xmf3RotationAxis, fAngle * fElapsedTime
	);
}

void CAirplaneEnemy::FireBullet()
{
	CBulletObject* pBulletObject = NULL;
	// 활성화 되징 않은 총알을 선택한다.
	for (int i = 0; i < ENEMY_BULLETS; i++)
	{
		if (!m_ppBullets[i]->m_bActive)
		{
			pBulletObject = m_ppBullets[i];
			break;
		}
	}

	// 선택된 총알이 없다는것은 이미 오브젝트 풀에 있는 모든 총알들이 활성화 상태인것이다.
	if (pBulletObject)
	{
		XMFLOAT3 xmf3Position = GetPosition();
		XMFLOAT3 xmf3Direction = GetUp();
		XMFLOAT3 xmf3FirePosition = Vector3::Add(xmf3Position, Vector3::ScalarProduct(xmf3Direction, 6.0f, false));

		pBulletObject->m_xmf4x4World = m_xmf4x4World;

		pBulletObject->SetFirePosition(xmf3FirePosition);
		pBulletObject->SetMovingDirection(xmf3Direction);
		pBulletObject->SetColor(RGB(255, 0, 0));
		pBulletObject->SetActive(true);
	}
}
