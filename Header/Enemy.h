#pragma once
#include "GameObject.h"

class CEnemy : public CExplosiveObject
{
public:
	CEnemy();
	virtual ~CEnemy();

public:
	XMFLOAT3					m_xmf3Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	XMFLOAT3					m_xmf3Look = XMFLOAT3(0.0f, 0.0f, 1.0f);

	XMFLOAT3					m_xmf3CameraOffset = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3					m_xmf3Velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	float           			m_fPitch = 0.0f;
	float           			m_fYaw = 0.0f;
	float           			m_fRoll = 0.0f;

	float						m_fRotateToSpeed{ 100.0f };
	std::queue<XMFLOAT3>		m_qxmf3RotateDests{ };

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);
	void SetRotateLookSpeed(float fSpeed);

	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);
	void RotateLook(XMFLOAT3& xmf3Axis, float fAngle);
	void RotateToSmoothly(XMFLOAT3& xmf3Dest, float fSpeed);
	
	void CancelLastMove(float fElapsedTime);

	virtual void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

#define ENEMY_BULLETS 10

class CPlayer;
class CAirplaneEnemy : public CEnemy
{
public:
	CAirplaneEnemy();
	virtual ~CAirplaneEnemy();

public:
	static void SetTargetObject(CPlayer* pTargetObject) { m_pTargetPlayer = pTargetObject; }

public:
	float							m_fElapsedFromLastFire{ };
	float							m_fElapsedChangingDirection{ };
	CBulletObject*					m_ppBullets[ENEMY_BULLETS];
	float							m_fBulletEffectiveRange = 150.0f;
	bool							m_bDetectedPlayer{ false };
	bool							m_bDebug{ false };
	XMFLOAT3						m_xmf3ChaseRotateAxis{ };

	inline static CPlayer*			m_pTargetPlayer{ nullptr };
	inline const static float		m_fFireDelay{ 2.5f };
	inline const static float		m_fDetectRange{ 25.0f };
	inline const static float		m_fChangeDirectionTime{ 5.0f };
	inline const static float		m_fChaseRotateSpeed{ 100.f };

public:
	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
	virtual void AfterCollision(const CGameObject* pCollObject, float fElapsedTime);
	virtual void AfterWallCollision(float fElapsedTime);

	bool DetectTarget();
	void ChaseTarget(float fElapsedTime);
	void FireBullet();
};