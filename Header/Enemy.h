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

public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void LookAt(XMFLOAT3& xmf3LookAt, XMFLOAT3& xmf3Up);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	virtual void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);
};

#define ENEMY_BULLETS 10

class CAirplaneEnemy : public CEnemy
{
public:
	CAirplaneEnemy();
	virtual ~CAirplaneEnemy();

public:
	float						m_fElapsedFromLastFire{ };
	CPlayer*					m_pPlayer{ };
	CBulletObject*				m_ppBullets[ENEMY_BULLETS];
	float						m_fBulletEffectiveRange = 150.0f;
	bool						m_bDetectedPOlayer{ false };
	inline const static float	m_fFireDelay{ 3.0f };
	inline const static float	m_fDetectRange{ 10.f };

public:
	virtual void OnUpdateTransform();
	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	bool DetectPlayer(XMFLOAT3& xmf3PlayerPosition);
	void FireBullet();
};