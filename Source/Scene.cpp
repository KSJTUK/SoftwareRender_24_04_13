#include "stdafx.h"
#include "Header/Scene.h"
#include "Header/GraphicsPipeline.h"
#include "Header/GameFramework.h"
#include "Alphabet5x3.h"

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene() { }

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	// Ŭ���� ���콺 ��ǥ(ȭ����ǥ��)�� ���ͼ� ���� ��ǥ���� ��ǥ�� ��ȯ�Ѵ�.
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	// ��ȯ�� ��ǥ�� �̿��Ͽ� ���� ���� ��� ������Ʈ�� ���ؼ� ������ ���� ���� ������(ȭ��� ���� ������ �ִ�) ������Ʈ�� ã�´�.
	for (int i = 0; i < m_nObjects; i++)
	{
		float fHitDistance = FLT_MAX;
		nIntersected = m_ppObjects[i]->PickObjectByRayIntersection(xmvPickPosition, xmmtxView, &fHitDistance);
		if ((nIntersected > 0) && (fHitDistance < fNearestHitDistance))
		{
			fNearestHitDistance = fHitDistance;
			pNearestObject = m_ppObjects[i];
		}
	}
	return(pNearestObject);
}

void CScene::CheckObjectByObjectCollisions(float fElapsedTime)
{
	// ������Ʈ�� ���� �浹�˻縦 �����Ѵ�.
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->m_pObjectCollided = NULL;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = (i + 1); j < m_nObjects; j++)
		{
			if (m_ppObjects[i]->m_xmOOBB.Intersects(m_ppObjects[j]->m_xmOOBB))
			{
				m_ppObjects[i]->m_pObjectCollided = m_ppObjects[j];
				m_ppObjects[j]->m_pObjectCollided = m_ppObjects[i];
			}
		}
	}
	// ������ �浹�� �Ͼ ��쿡 ������ ������Ʈ�鿡�� �ڽŰ� �浹�� ������Ʈ�� �ּҸ� ���������.
	for (int i = 0; i < m_nObjects; i++)
	{
		// �ڽŰ� �浹�� ������Ʈ�� �ִٸ�(�浹 �ߴٸ�)
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			// �ڽŰ� �浹�� ������Ʈ�� �ڽ��� �ӵ�, ������ ���� �ٲ۴� -> �Լ��� �ٲ��� (�� ������Ʈ ���� ������ ������ �Լ��� ����� ����)
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			
			// 2024 - 04 - 11
			m_ppObjects[i]->AfterCollision(m_ppObjects[i]->m_pObjectCollided, fElapsedTime);
			m_ppObjects[i]->m_pObjectCollided->AfterCollision(m_ppObjects[i], fElapsedTime);

			// �浹 ������ ����� -> �ߺ� ���� ����
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions(float fElapsedTime)
{
	for (int i = 0; i < m_nObjects; i++)
	{
		// ������Ʈ�� ���� BoundingBox ���� Contain Type�� ���´�.
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:  // ���� �ٿ�� �ڽ� �ȿ� ������Ʈ�� ���� ���� ���
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				// ��ü�� �� ���� �����ϴ� �� �˻��Ѵ�.
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				// ��ü�� �� ���� ���� �ϸ� �� ����� ���� ���͸� ���ؼ� ������ �ݻ��Ѵ�(��ü�� ƨ��� ��)
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				m_ppObjects[i]->AfterWallCollision(fElapsedTime);
			}
			break;
		}
		case INTERSECTS: // �� �ڽ��� ���� ������ ���
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == INTERSECTING)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
				m_ppObjects[i]->AfterWallCollision(fElapsedTime);
			}
			break;
		}
		case CONTAINS: // ���� �ٿ�� �ڽ� �ȿ� ������Ʈ�� ������ ���� �� ��쿡�� ���ٸ� ������ �ʿ� ����.
			break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	// ���� �÷��̾ ���� �����ϴ� ��� ���� ��ġ�� �÷��̾��� ��ġ�� �̵���Ų��
	// ��������δ� �÷��̾ ���� �߾ӿ� ��ġ�ϴ� ��ó�� ���̰� �ȴ�.
	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB))/* m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);*/
		if (!m_pPlayer->m_bBlowingUp) 
			m_pPlayer->CancelMove();
}

void CScene::CheckObjectByBulletCollisions()
{
	// �÷��̾ �߻��� �Ѿ��� ����Ʈ�� �����ͼ� ���� ������Ʈ����� �浹�� �˻��Ѵ�.
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	CSheildObject* pSheild = ((CAirplanePlayer*)m_pPlayer)->m_pSheild;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && !((CExplosiveObject*)m_ppObjects[i])->m_bBlowingUp && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
			}
		}

		CAirplaneEnemy* pEnemy = (CAirplaneEnemy*)m_ppObjects[i];
		CBulletObject** ppEnemyBullets = pEnemy->m_ppBullets;
		for (int j = 0; j < ENEMY_BULLETS; ++j) {
			if (!ppEnemyBullets[j]->m_bActive)
				continue;

			if (pSheild->m_bActive && pSheild->m_xmOOBB.Intersects(ppEnemyBullets[j]->m_xmOOBB)) {
				ppEnemyBullets[j]->Reset();
				continue;
			}

			if (m_pPlayer->m_xmOOBB.Intersects(ppEnemyBullets[j]->m_xmOOBB)) {
				((CExplosiveObject*)m_pPlayer)->m_bBlowingUp = true;
				ppEnemyBullets[j]->Reset();
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CStartScene::BuildObjects()
{
	int nAlphas[5] = { S, T, A, R, T };
	m_nObjects = 1 + CNT_S + CNT_T + CNT_A + CNT_R + CNT_T;
	m_ppObjects = new CGameObject*[m_nObjects];
	m_pPlayer->m_bActive = false;

	int nObjects = 0;
	CCubeMesh* pButtonMesh = new CCubeMesh(25.f, 15.f, 0.0f);
	CGameObject* pButton = new CGameObject();
	pButton->SetMesh(pButtonMesh);
	pButton->SetColor(RGB(0, 0, 0));
	pButton->SetPosition(0.0f, 0.0f, 0.0f);
	m_ppObjects[nObjects++] = pButton;

	float fLeft = -18.f;
	float fTop = 6.0f;
	float fXSize = 2.0f;
	float fYSize = 3.0f;
	float fZ = 14.0f;
	CCubeMesh* pCubeMesh = new CCubeMesh(fXSize, fYSize, fXSize);

	int nWidth = 0;
	int nHeight = 0;
	int nColumn = 3;
	int nRow = 5;
	int nLoopInterval = nRow * nColumn;
	for (int alpha = 0; alpha < _countof(nAlphas); ++alpha) {
		for (int i = 0; i < nLoopInterval; ++i) {
			int alphabet = nAlphas[alpha];
			nWidth = i % nColumn;
			nHeight = i / nColumn;

			if (fAlphabet[alphabet][nHeight][nWidth] > 0.0f) {
				float x = fLeft + nWidth * fXSize;
				float y = fTop - nHeight * fYSize;
				CRotatingObject* pTextObject = new CRotatingObject();
				pTextObject->SetMesh(pCubeMesh);
				pTextObject->SetColor(RGB(0, 0, 0));
				pTextObject->SetPosition(x, y, fZ);
				pTextObject->SetRotationAxis(XMFLOAT3(0.f, 1.f, 0.f));
				m_ppObjects[nObjects++] = pTextObject;
			}
		}
		fLeft += fXSize * 4.0f;
	}
}

void CStartScene::ReleaseObjects()
{
	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i])
			delete m_ppObjects[i];

	if (m_ppObjects) delete[] m_ppObjects;
}

void CStartScene::Animate(float fElapsedTime)
{
	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i]->m_bActive)
			m_ppObjects[i]->Animate(fElapsedTime);
}

void CStartScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	// �׷��� ���������ο� ����Ʈ������ ī�޶�, ������� ������ �����Ѵ�.
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	for (int i = 0; i < m_nObjects; ++i)
		if (m_ppObjects[i])
			m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
}

void CStartScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		if (nMessageID == WM_LBUTTONDOWN)
		{
			CGameFramework::ChangeScene();
		}
		break;
	case WM_LBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		if (m_ppObjects[0] == PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera))
		{
			m_ppObjects[0]->SetColor(RGB(255, 0, 255));

			int nObjects = 1;
			for (int i = nObjects; i < nObjects + CNT_S; ++i) {
				m_ppObjects[i]->SetColor(RGB(255, 0, 0));
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(30.0f);
			}
			nObjects += CNT_S;

			for (int i = nObjects; i < nObjects + CNT_T; ++i) {
				m_ppObjects[i]->SetColor(RGB(0, 255, 0));
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(30.0f);
			}
			nObjects += CNT_T;

			for (int i = nObjects; i < nObjects + CNT_A; ++i) {
				m_ppObjects[i]->SetColor(RGB(0, 0, 255));
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(30.0f);
			}
			nObjects += CNT_A;

			for (int i = nObjects; i < nObjects + CNT_R; ++i) {
				m_ppObjects[i]->SetColor(RGB(255, 255, 0));
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(30.0f);
			}
			nObjects += CNT_R;

			for (int i = nObjects; i < nObjects + CNT_T; ++i) {
				m_ppObjects[i]->SetColor(RGB(255, 0, 255));
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(30.0f);
			}
			nObjects += CNT_T;

		}
		else
		{
			for (int i = 0; i < m_nObjects; ++i)
				m_ppObjects[i]->SetColor(RGB(0, 0, 0));

			for (int i = 1; i < m_nObjects; ++i) {
				((CRotatingObject*)m_ppObjects[i])->SetRotationSpeed(0.0f);
				((CRotatingObject*)m_ppObjects[i])->SetRotationTransform(&Matrix4x4::Identity());
			}
		}
		break;
	default:
		break;
	}
}

void CStartScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CPlayScene::BuildObjects()
{
	// �����ϴ� ������Ʈ�� �⺻���� ������ �غ��Ѵ�.
	CExplosiveObject::PrepareExplosion();
	m_pPlayer->m_bActive = true;

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 100.f;
	CWallMesh* pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 5);

	m_pWallsObject = new CWallsObject();
	m_pWallsObject->SetPosition(0.0f, 0.0f, 0.0f);
	m_pWallsObject->SetMesh(pWallCubeMesh);
	m_pWallsObject->SetColor(RGB(0, 0, 0));
	m_pWallsObject->m_pxmf4WallPlanes[0] = XMFLOAT4(+1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[1] = XMFLOAT4(-1.0f, 0.0f, 0.0f, fHalfWidth);
	m_pWallsObject->m_pxmf4WallPlanes[2] = XMFLOAT4(0.0f, +1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[3] = XMFLOAT4(0.0f, -1.0f, 0.0f, fHalfHeight);
	m_pWallsObject->m_pxmf4WallPlanes[4] = XMFLOAT4(0.0f, 0.0f, +1.0f, fHalfDepth);
	m_pWallsObject->m_pxmf4WallPlanes[5] = XMFLOAT4(0.0f, 0.0f, -1.0f, fHalfDepth);
	m_pWallsObject->m_xmOOBBPlayerMoveCheck = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.5f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CAirplaneMesh* pAirPlaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);

	m_nObjects = 10;
	m_ppObjects = new CGameObject * [m_nObjects];
	XMFLOAT3 xmf3Positions[10] {
		XMFLOAT3(-13.5f, 0.0f, -24.0f), XMFLOAT3(0.0f, 10.0f, 30.0f),
		XMFLOAT3(0.0f, 0.0f, 15.0f), XMFLOAT3(-20.0f, 0.0f, 15.0f),
		XMFLOAT3(15.0f, 0.0f, 0.0f), XMFLOAT3(-10.0f, 0.0f, -10.0f),
		XMFLOAT3(-10.0f, 10.0f, -5.0f), XMFLOAT3(-40.0f, 10.0f, -25.0f),
		XMFLOAT3(-15.0f, 10.0f, -30.0f), XMFLOAT3(+15.0f, 10.0f, 30.0f),
	};
	
	for (int i = 0; i < m_nObjects; ++i) {
		CAirplaneEnemy* pExplosiveObject = new CAirplaneEnemy();
		pExplosiveObject->SetMesh(pAirPlaneMesh);
		pExplosiveObject->SetColor(Random::RandomColor());
		pExplosiveObject->SetPosition(xmf3Positions[i].x, xmf3Positions[i].y, xmf3Positions[i].z);
		m_ppObjects[i] = pExplosiveObject;
	}

	m_pPlayer->SetPosition(0.0f, 0.0f, -2000.0f);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -m_fPlayerSpeedPerSec * 0.1f));

	
	float offset = 5.f;
	XMFLOAT3 xmf3Start = m_pPlayer->GetPosition();
	CAirplaneMesh* pSmallAirPlaneMesh = new CAirplaneMesh(3.0f, 3.0f, 0.5f);
	m_ppFriendTurrets = new CGameObject*[4];
	XMFLOAT3 xmf3MoveDirs[4]{
		Vector3::Normalize(XMFLOAT3(-0.5f, 1.0f, 1.0f)), Vector3::Normalize(XMFLOAT3(-0.5f, 1.0f, 1.0f)),
		Vector3::Normalize(XMFLOAT3(0.5f, 1.0f, 1.0f)), Vector3::Normalize(XMFLOAT3(0.5f, 1.0f, 1.0f)),
	};

	xmf3Start.x -= offset * 2;
	for (int i = 0; i < 4; ++i) {
		CEnemy* pExplosiveObject = new CEnemy();
		pExplosiveObject->SetMesh(pSmallAirPlaneMesh);
		pExplosiveObject->SetColor(RGB(0, 255, 0));
		pExplosiveObject->SetPosition(xmf3Start.x, xmf3Start.y, xmf3Start.z - 10.f);
		pExplosiveObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
		pExplosiveObject->SetRotateLookSpeed(0.0f);
		pExplosiveObject->m_qxmf3RotateDests.push(xmf3MoveDirs[i]);
		xmf3Start.x += offset;
		if (i == 1)
			xmf3Start.x += offset;
		m_ppFriendTurrets[i] = pExplosiveObject;
	}

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CPlayScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CPlayScene::SceneStart(float fElapsedTime)
{
	static float fFriendMoveTime = 0.0f;
	fFriendMoveTime += fElapsedTime;

	m_pPlayer->Move(XMFLOAT3(0.0f, 0.0f, m_fPlayerSpeedPerSec * fElapsedTime), false);
	m_fPlayerSpeedPerSec -= m_fDeccelSpeedPerSec * fElapsedTime;
	if (m_fPlayerSpeedPerSec > 200.f)
		m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -m_fPlayerSpeedPerSec * 0.1f));

	for (int i = 0; i < 4; ++i) {
		if (fFriendMoveTime > 1.5f) {
			((CEnemy*)m_ppFriendTurrets[i])->SetRotateLookSpeed(5.0f);
		}
		m_ppFriendTurrets[i]->SetMovingSpeed(m_fPlayerSpeedPerSec);
		m_ppFriendTurrets[i]->Animate(fElapsedTime);
	}

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	if (m_pPlayer->GetPosition().z < 0.f) {
		return;
	}

	CAirplaneEnemy::SetTargetObject(m_pPlayer);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -20.0f));
	m_eSceneState = SceneState::RUNNING;
}

void CPlayScene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	CheckPlayerByWallCollision();

	CheckObjectByWallCollisions(fElapsedTime);

	CheckObjectByObjectCollisions(fElapsedTime);

	CheckObjectByBulletCollisions();
}

void CPlayScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	// �׷��� ���������ο� ����Ʈ������ ī�޶�, ������� ������ �����Ѵ�.
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	// ��� ������Ʈ�� MemoryDC�� �׸���.
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < 4; ++i) m_ppFriendTurrets[i]->Render(hDCFrameBuffer, pCamera);

	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

	//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}

void CPlayScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CPlayScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		{
			CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[int(wParam - '1')];
			pExplosiveObject->m_bBlowingUp = true;
			break;
		}
		case '0':
			((CAirplanePlayer*)m_pPlayer)->FireBulletToAll(m_ppObjects, m_nObjects);
			break;
		case 'A':
			for (int i = 0; i < m_nObjects; i++)
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
