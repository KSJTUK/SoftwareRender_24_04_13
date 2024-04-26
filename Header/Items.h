#pragma once
#include "GameObject.h"

enum ITEM_TYPE {
	ITEM_BLOCK,
	ITEM_BOOM,
};

class CItem : public CRotatingObject
{
public:
	CItem();
	virtual ~CItem();

public:
	ITEM_TYPE	m_eType{ };
	CMesh*		m_typeMesh{ };
};

#define TURRET_BULLETS 10
class CTurret : public CItem
{
public:
	CTurret(CGameObject* pParent);
	virtual ~CTurret();

public:
	CGameObject*	m_pParent{ };
	XMFLOAT3		m_xmf3Offset{ 0.0f, 0.0f, 5.0f };

	float			m_fOrbitSpeed{ };
	
public:
	virtual void Animate(float fElapsedTime);
	void Orbit(float fElapsedTime);
};