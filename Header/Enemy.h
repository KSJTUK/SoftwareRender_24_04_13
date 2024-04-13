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

	void Move(DWORD dwDirection, float fDistance);
	void Move(XMFLOAT3& xmf3Shift, bool bUpdateVelocity);
	void Move(float x, float y, float z);

	void Rotate(float fPitch = 0.0f, float fYaw = 0.0f, float fRoll = 0.0f);

	void SetCameraOffset(XMFLOAT3& xmf3CameraOffset);

	virtual void Update(float fTimeElapsed = 0.016f);

	virtual void OnUpdateTransform() { }
	virtual void Animate(float fElapsedTime);
};

class CAirplaneEnemy : public CEnemy
{
public:
	CAirplaneEnemy();
	virtual ~CAirplaneEnemy();

public:
	float m_fElapsedFromLastFire{ };

	inline const static float m_fFireDelay{ 3.0f };
};