#include "stdafx.h"
#include "Header/Scene.h"
#include "Header/GraphicsPipeline.h"

CScene::CScene(CPlayer* pPlayer)
{
	m_pPlayer = pPlayer;
}

CScene::~CScene() { }

void CScene::BuildObjects()
{
	// 폭발하는 오브젝트의 기본적인 설정을 준비한다.
	CExplosiveObject::PrepareExplosion();

	float fHalfWidth = 45.0f, fHalfHeight = 45.0f, fHalfDepth = 200.0f;
	CWallMesh* pWallCubeMesh = new CWallMesh(fHalfWidth * 2.0f, fHalfHeight * 2.0f, fHalfDepth * 2.0f, 30);

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
	m_pWallsObject->m_xmOOBBPlayerMoveCheck = BoundingOrientedBox(XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(fHalfWidth, fHalfHeight, fHalfDepth * 0.05f), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f));

	CAirplaneMesh* pAirPlaneMesh = new CAirplaneMesh(15.f, 15.f, 3.f);

	m_nObjects = 10;
	m_ppObjects = new CGameObject * [m_nObjects];
	XMFLOAT4X4 xmf4x4RotationTransform = Matrix4x4::RotationYawPitchRoll(0.f, 0.f, XMConvertToRadians(90.f));
	
	
	CAirplaneEnemy* pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 0));
	pExplosiveObject->SetPosition(-13.5f, 0.0f, -14.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationTransform(&xmf4x4RotationTransform);
	//pExplosiveObject->SetRotationSpeed(90.0f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 0.0f));
	pExplosiveObject->SetMovingSpeed(10.5f);
	m_ppObjects[0] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(0, 0, 255));
	pExplosiveObject->SetPosition(+13.5f, 0.0f, -14.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(180.0f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(-1.0f, 0.0f, 0.0f));
	pExplosiveObject->SetMovingSpeed(8.8f);
	m_ppObjects[1] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(0, 255, 0));
	pExplosiveObject->SetPosition(0.0f, +5.0f, 20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(30.15f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(1.0f, -1.0f, 0.0f));
	pExplosiveObject->SetMovingSpeed(5.2f);
	m_ppObjects[2] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(0, 255, 255));
	pExplosiveObject->SetPosition(0.0f, 0.0f, 0.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	pExplosiveObject->SetRotationSpeed(40.6f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, 1.0f));
	pExplosiveObject->SetMovingSpeed(20.4f);
	m_ppObjects[3] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(128, 0, 255));
	pExplosiveObject->SetPosition(10.0f, 0.0f, 0.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(50.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(0.0f, 1.0f, 1.0f));
	pExplosiveObject->SetMovingSpeed(6.4f);
	m_ppObjects[4] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 255));
	pExplosiveObject->SetPosition(-10.0f, 0.0f, -10.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(60.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(1.0f, 0.0f, 1.0f));
	pExplosiveObject->SetMovingSpeed(8.9f);
	m_ppObjects[5] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 255));
	pExplosiveObject->SetPosition(-10.0f, 10.0f, -10.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(60.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(1.0f, 1.0f, 1.0f));
	pExplosiveObject->SetMovingSpeed(9.7f);
	m_ppObjects[6] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(255, 0, 128));
	pExplosiveObject->SetPosition(-10.0f, 10.0f, -20.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(70.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(-1.0f, 1.0f, 1.0f));
	pExplosiveObject->SetMovingSpeed(15.6f);
	m_ppObjects[7] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(128, 0, 255));
	pExplosiveObject->SetPosition(-15.0f, 10.0f, -30.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(90.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(0.0f, 0.0f, -1.0f));
	pExplosiveObject->SetMovingSpeed(15.0f);
	m_ppObjects[8] = pExplosiveObject;

	pExplosiveObject = new CAirplaneEnemy();
	pExplosiveObject->SetMesh(pAirPlaneMesh);
	pExplosiveObject->SetColor(RGB(255, 64, 64));
	pExplosiveObject->SetPosition(+15.0f, 10.0f, 0.0f);
	pExplosiveObject->SetRotationAxis(XMFLOAT3(0.0f, 1.0f, 0.0f));
	//pExplosiveObject->SetRotationSpeed(90.06f);
	pExplosiveObject->SetMovingDirection(XMFLOAT3(-0.0f, 0.0f, -1.0f));
	pExplosiveObject->SetMovingSpeed(15.0f);
	m_ppObjects[9] = pExplosiveObject;

#ifdef _WITH_DRAW_AXIS
	m_pWorldAxis = new CGameObject();
	CAxisMesh* pAxisMesh = new CAxisMesh(0.5f, 0.5f, 0.5f);
	m_pWorldAxis->SetMesh(pAxisMesh);
#endif
}

void CScene::ReleaseObjects()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();

	for (int i = 0; i < m_nObjects; i++) if (m_ppObjects[i]) delete m_ppObjects[i];
	if (m_ppObjects) delete[] m_ppObjects;

	if (m_pWallsObject) delete m_pWallsObject;

#ifdef _WITH_DRAW_AXIS
	if (m_pWorldAxis) delete m_pWorldAxis;
#endif
}

void CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

CGameObject* CScene::PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera)
{
	// 클릭한 마우스 좌표(화면좌표계)를 얻어와서 월드 좌표계의 좌표로 변환한다.
	XMFLOAT3 xmf3PickPosition;
	xmf3PickPosition.x = (((2.0f * xClient) / (float)pCamera->m_Viewport.m_nWidth) - 1) / pCamera->m_xmf4x4PerspectiveProject._11;
	xmf3PickPosition.y = -(((2.0f * yClient) / (float)pCamera->m_Viewport.m_nHeight) - 1) / pCamera->m_xmf4x4PerspectiveProject._22;
	xmf3PickPosition.z = 1.0f;

	XMVECTOR xmvPickPosition = XMLoadFloat3(&xmf3PickPosition);
	XMMATRIX xmmtxView = XMLoadFloat4x4(&pCamera->m_xmf4x4View);

	int nIntersected = 0;
	float fNearestHitDistance = FLT_MAX;
	CGameObject* pNearestObject = NULL;
	// 변환된 좌표를 이용하여 게임 내의 모든 오브젝트에 대해서 광선과 가장 먼저 만나는(화면과 가장 가까이 있는) 오브젝트를 찾는다.
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

void CScene::CheckObjectByObjectCollisions()
{
	// 오브젝트들 간의 충돌검사를 진행한다.
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
	// 위에서 충돌이 일어난 경우에 각각의 오브젝트들에게 자신과 충돌한 오브젝트의 주소를 세팅해줬다.
	for (int i = 0; i < m_nObjects; i++)
	{
		// 자신과 충돌한 오브젝트가 있다면(충돌 했다면)
		if (m_ppObjects[i]->m_pObjectCollided)
		{
			XMFLOAT3 xmf3MovingDirection = m_ppObjects[i]->m_xmf3MovingDirection;
			float fMovingSpeed = m_ppObjects[i]->m_fMovingSpeed;
			// 자신과 충돌한 오브젝트와 자신의 속도, 방향을 서로 바꾼다 -> 함수로 바꾸자 (각 오브젝트 마다 수행할 연산을 함수로 만들어 수행)
			m_ppObjects[i]->m_xmf3MovingDirection = m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection;
			m_ppObjects[i]->m_fMovingSpeed = m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed;
			m_ppObjects[i]->m_pObjectCollided->m_xmf3MovingDirection = xmf3MovingDirection;
			m_ppObjects[i]->m_pObjectCollided->m_fMovingSpeed = fMovingSpeed;
			
			// 2024 - 04 - 11
			m_ppObjects[i]->AfterCollision(m_ppObjects[i]->m_pObjectCollided);
			m_ppObjects[i]->m_pObjectCollided->AfterCollision(m_ppObjects[i]);

			// 충돌 내용을 지운다 -> 중복 연산 방지
			m_ppObjects[i]->m_pObjectCollided->m_pObjectCollided = NULL;
			m_ppObjects[i]->m_pObjectCollided = NULL;
		}
	}
}

void CScene::CheckObjectByWallCollisions()
{
	for (int i = 0; i < m_nObjects; i++)
	{
		// 오브젝트와 벽의 BoundingBox 간의 Contain Type을 얻어온다.
		ContainmentType containType = m_pWallsObject->m_xmOOBB.Contains(m_ppObjects[i]->m_xmOOBB);
		switch (containType)
		{
		case DISJOINT:  // 벽의 바운딩 박스 안에 오브젝트가 있지 않은 경우
		{
			int nPlaneIndex = -1;
			for (int j = 0; j < 6; j++)
			{
				// 물체가 벽 평면과 교차하는 지 검사한다.
				PlaneIntersectionType intersectType = m_ppObjects[i]->m_xmOOBB.Intersects(XMLoadFloat4(&m_pWallsObject->m_pxmf4WallPlanes[j]));
				if (intersectType == BACK)
				{
					nPlaneIndex = j;
					break;
				}
			}
			if (nPlaneIndex != -1)
			{
				// 물체가 벽 평면과 교차 하면 그 평면의 법선 벡터를 통해서 방향을 반사한다(물체를 튕기게 함)
				XMVECTOR xmvNormal = XMVectorSet(m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].x, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].y, m_pWallsObject->m_pxmf4WallPlanes[nPlaneIndex].z, 0.0f);
				XMVECTOR xmvReflect = XMVector3Reflect(XMLoadFloat3(&m_ppObjects[i]->m_xmf3MovingDirection), xmvNormal);
				XMStoreFloat3(&m_ppObjects[i]->m_xmf3MovingDirection, xmvReflect);
			}
			break;
		}
		case INTERSECTS: // 두 박스가 서로 교차한 경우
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
			}
			break;
		}
		case CONTAINS: // 벽의 바운딩 박스 안에 오브젝트가 완전히 포함 된 경우에는 별다른 연산이 필요 없다.
			break;
		}
	}
}

void CScene::CheckPlayerByWallCollision()
{
	BoundingOrientedBox xmOOBBPlayerMoveCheck;
	m_pWallsObject->m_xmOOBBPlayerMoveCheck.Transform(xmOOBBPlayerMoveCheck, XMLoadFloat4x4(&m_pWallsObject->m_xmf4x4World));
	XMStoreFloat4(&xmOOBBPlayerMoveCheck.Orientation, XMQuaternionNormalize(XMLoadFloat4(&xmOOBBPlayerMoveCheck.Orientation)));

	// 평면과 플레이어가 서로 교차하는 경우 벽의 위치를 플레이어의 위치로 이동시킨다
	// 결과적으로는 플레이어가 벽의 중앙에 위치하는 것처럼 보이게 된다.
	if (!xmOOBBPlayerMoveCheck.Intersects(m_pPlayer->m_xmOOBB)) m_pWallsObject->SetPosition(m_pPlayer->m_xmf3Position);
}

void CScene::CheckObjectByBulletCollisions()
{
	// 플레이어가 발사한 총알의 리스트를 가져와서 게임 오브젝트들과의 충돌을 검사한다.
	CBulletObject** ppBullets = ((CAirplanePlayer*)m_pPlayer)->m_ppBullets;
	for (int i = 0; i < m_nObjects; i++)
	{
		for (int j = 0; j < BULLETS; j++)
		{
			if (ppBullets[j]->m_bActive && m_ppObjects[i]->m_xmOOBB.Intersects(ppBullets[j]->m_xmOOBB))
			{
				CExplosiveObject* pExplosiveObject = (CExplosiveObject*)m_ppObjects[i];
				pExplosiveObject->m_bBlowingUp = true;
				ppBullets[j]->Reset();
			}
		}
	}
}

void CScene::Animate(float fElapsedTime)
{
	m_pWallsObject->Animate(fElapsedTime);

	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Animate(fElapsedTime);

	CheckPlayerByWallCollision();

	CheckObjectByWallCollisions();

	CheckObjectByObjectCollisions();

	CheckObjectByBulletCollisions();
}

void CScene::Render(HDC hDCFrameBuffer, CCamera* pCamera)
{
	// 그래픽 파이프라인에 뷰포트정보와 카메라, 투영행렬 정보를 설정한다.
	CGraphicsPipeline::SetViewport(&pCamera->m_Viewport);
	CGraphicsPipeline::SetViewPerspectiveProjectTransform(&pCamera->m_xmf4x4ViewPerspectiveProject);

	// 모든 오브젝트를 MemoryDC에 그린다.
	m_pWallsObject->Render(hDCFrameBuffer, pCamera);
	for (int i = 0; i < m_nObjects; i++) m_ppObjects[i]->Render(hDCFrameBuffer, pCamera);

	if (m_pPlayer) m_pPlayer->Render(hDCFrameBuffer, pCamera);

//UI
#ifdef _WITH_DRAW_AXIS
	CGraphicsPipeline::SetViewOrthographicProjectTransform(&pCamera->m_xmf4x4ViewOrthographicProject);
	m_pWorldAxis->SetRotationTransform(&m_pPlayer->m_xmf4x4World);
	m_pWorldAxis->Render(hDCFrameBuffer, pCamera);
#endif
}
