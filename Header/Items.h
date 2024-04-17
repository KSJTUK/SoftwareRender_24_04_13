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

class CTurret : public CItem
{
public:
	CTurret(CGameObject* pParent);
	virtual ~CTurret();

public:
	CGameObject*	m_pParent{ };

	float			m_fOrbitSpeed{ };
	
public:
	virtual void Animate(float fElapsedTime);
	void Orbit(float fElapsedTime);
};