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
	m_xmf4x4World = Matrix4x4::Multiply(XMMatrixRotationRollPitchYaw(XMConvertToRadians(90.0f), 0.0f, 0.0f), m_xmf4x4World);
	OnUpdateTransform();
}

CTurret::~CTurret()
{
}

void CTurret::Animate(float fElapsedTime)
{
	m_xmf4x4World = m_pParent->m_xmf4x4World;
	SetPosition(Vector3::Add(m_xmf3Offset, m_pParent->GetPosition()));
	CRotatingObject::Animate(fElapsedTime);
	Orbit(fElapsedTime);
}

void CTurret::Orbit(float fElapsedTime)
{
	XMMATRIX rotation = XMMatrixRotationAxis(XMLoadFloat3(&m_pParent->GetLook()), XMConvertToRadians(m_fOrbitSpeed) * fElapsedTime);
	m_xmf3Offset = Vector3::TransformCoord(m_xmf3Offset, rotation);
}
