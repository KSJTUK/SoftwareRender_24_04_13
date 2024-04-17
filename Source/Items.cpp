#include "stdafx.h"
#include "../Header/Items.h"

CItem::CItem()
{
}

CItem::~CItem()
{
}


/////////////////////////////////////////////////////////////////////

CTurret::CTurret(CGameObject* pParent) : m_pParent{ pParent }
{
	if (m_pMesh) m_pMesh->Release();
	m_pMesh = new CAirplaneMesh(2.0f, 2.0f, 0.4f);
	XMFLOAT3 xmf3Position = m_pParent->GetPosition();
	xmf3Position.x += 2.f;
	SetPosition(xmf3Position);
}

CTurret::~CTurret()
{
}

void CTurret::Animate(float fElapsedTime)
{
	//m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);
	CRotatingObject::Animate(fElapsedTime);
	Orbit(fElapsedTime);
}

void CTurret::Orbit(float fElapsedTime)
{
	XMFLOAT3 xmf3Position = Vector3::Subtract(GetPosition(), m_pParent->GetPosition());
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&m_pParent->GetLook()), XMConvertToRadians(m_fOrbitSpeed) * fElapsedTime);
	xmf3Position = Vector3::TransformCoord(xmf3Position, rotation);
	SetPosition(Vector3::Add(xmf3Position, m_pParent->GetPosition()));
}
