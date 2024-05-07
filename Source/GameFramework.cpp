//-----------------------------------------------------------------------------
// File: CGameFramework.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Header/GameFramework.h"

void CGameFramework::ChangeScene()
{
	extern CGameFramework gGameFramework;
	gGameFramework.NextScene();
}

void CGameFramework::OnCreate(HINSTANCE hInstance, HWND hMainWnd)
{
	::srand(timeGetTime());

	m_hInstance = hInstance;
	m_hWnd = hMainWnd;

	PrepareFunctions();

	BuildFrameBuffer(); 

	BuildObjects(); 

	m_ppScenes[m_nCurSceneIdx]->BuildObjects();

	_tcscpy_s(m_pszFrameRate, _T("LabProject ("));
}

void CGameFramework::OnDestroy()
{
	if (CExplosiveObject::m_pExplosionMesh) CExplosiveObject::m_pExplosionMesh->Release();
	ReleaseObjects();

	if (m_hBitmapFrameBuffer) ::DeleteObject(m_hBitmapFrameBuffer);
	if (m_hDCFrameBuffer) ::DeleteDC(m_hDCFrameBuffer);
}

void CGameFramework::PrepareFunctions()
{
	m_mapSceneFunctions.insert(std::make_pair(static_cast<UINT32>(SceneState::SCENE_START), &CScene::SceneStart));
	m_mapSceneFunctions.insert(std::make_pair(static_cast<UINT32>(SceneState::RUNNING), &CScene::Animate));
	m_mapSceneFunctions.insert(std::make_pair(static_cast<UINT32>(SceneState::PREPARE_CHANGE), &CScene::PrepareChange));
	m_mapSceneFunctions.insert(std::make_pair(static_cast<UINT32>(SceneState::CHANGING), &CScene::ChangeAnimation));
	m_mapSceneFunctions.insert(std::make_pair(static_cast<UINT32>(SceneState::END), &CScene::SceneEnd));
}

void CGameFramework::NextScene()
{
	m_ppScenes[m_nCurSceneIdx]->ReleaseObjects();

	++m_nCurSceneIdx;
	assert(m_nCurSceneIdx < m_nScenes);
	
	m_ppScenes[m_nCurSceneIdx]->BuildObjects();
}

void CGameFramework::BuildFrameBuffer()
{
	// 현재 윈도우의 화면 영역을 얻어온다
	::GetClientRect(m_hWnd, &m_rcClient);

	// 현재 윈도우의 Device Context를 얻어온다.
	HDC hDC = ::GetDC(m_hWnd);

	// 현재 DC와 호환되는 MemoryDC를 생성한다.
    m_hDCFrameBuffer = ::CreateCompatibleDC(hDC);
	// 현재 DC와 호환되는 비트맵을 새로 생성한다.
	m_hBitmapFrameBuffer = ::CreateCompatibleBitmap(hDC, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top);
	// 새로 만든 MemoryDC에 새로 만든 비트맵을 세팅해준다.
    ::SelectObject(m_hDCFrameBuffer, m_hBitmapFrameBuffer);

	::ReleaseDC(m_hWnd, hDC);
	// 새로만든 DC의 배경색 모드를 설정해준다.
    ::SetBkMode(m_hDCFrameBuffer, TRANSPARENT);
}

void CGameFramework::ClearFrameBuffer(DWORD dwColor)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 0, dwColor);
	HPEN hOldPen = (HPEN)::SelectObject(m_hDCFrameBuffer, hPen);
	HBRUSH hBrush = ::CreateSolidBrush(dwColor);
	HBRUSH hOldBrush = (HBRUSH)::SelectObject(m_hDCFrameBuffer, hBrush);
	// 화면 버퍼를 모두 덮는 사각형을 그려 프레임 버퍼를 같은 색으로 초기화한다.
	::Rectangle(m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);

	::SelectObject(m_hDCFrameBuffer, hOldBrush);
	::SelectObject(m_hDCFrameBuffer, hOldPen);
	::DeleteObject(hPen);
	::DeleteObject(hBrush);
}

void CGameFramework::PresentFrameBuffer()
{    
	// 게임 내에 존재하는 모든 게임 오브젝트를 메모리 DC에 다 그렸다면
	// 그 메모리 DC의 내용을 현재 화면 DC에 BitBlt함수를 호출하여 고속 복사한다. -> 화면 플리커링 방지용 더블 버퍼링
    HDC hDC = ::GetDC(m_hWnd);
    ::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right - m_rcClient.left, m_rcClient.bottom - m_rcClient.top, m_hDCFrameBuffer, m_rcClient.left, m_rcClient.top, SRCCOPY);
    ::ReleaseDC(m_hWnd, hDC);
}

void CGameFramework::BuildObjects()
{
	// 폭발하는 오브젝트의 기본적인 설정을 준비한다.
	CExplosiveObject::PrepareExplosion();

	CCamera* pCamera = new CCamera();
	// 카메라에 현재 기본
	pCamera->SetViewport(0, 0, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);
	pCamera->GeneratePerspectiveProjectionMatrix(1.01f, 500.0f, 60.0f);
	pCamera->SetFOVAngle(60.0f);

	pCamera->GenerateOrthographicProjectionMatrix(1.01f, 50.0f, FRAMEBUFFER_WIDTH, FRAMEBUFFER_HEIGHT);

	CAirplaneMesh* pAirplaneMesh = new CAirplaneMesh(6.0f, 6.0f, 1.0f);

	m_pPlayer = new CAirplanePlayer();
	m_pPlayer->SetPosition(0.0f, 0.0f, 0.0f);
	m_pPlayer->SetMesh(pAirplaneMesh);
	m_pPlayer->SetColor(RGB(0, 0, 255));
	m_pPlayer->SetCamera(pCamera);
	m_pPlayer->SetCameraOffset(XMFLOAT3(0.0f, 5.0f, -20.0f));

	m_nScenes = 2;
	m_ppScenes = new CScene * [m_nScenes];
	CScene* pScene = new CStartScene(m_pPlayer);
	m_ppScenes[0] = pScene;
	pScene = new CPlayScene(m_pPlayer);
	m_ppScenes[1] = pScene;
}

void CGameFramework::ReleaseObjects()
{
	m_ppScenes[m_nCurSceneIdx]->ReleaseObjects();

	if (m_ppScenes) {
		for (UINT32 i = 0; i < m_nScenes; ++i)
			if (m_ppScenes[i]) delete m_ppScenes[i];
	}
	
	delete[] m_ppScenes;

	if (m_pPlayer) delete m_pPlayer;
}

void CGameFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene* currentScene = m_ppScenes[m_nCurSceneIdx];
	if (currentScene) currentScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
	case WM_LBUTTONDOWN:
		::SetCapture(hWnd);
		::GetCursorPos(&m_ptOldCursorPos);
		if (nMessageID == WM_RBUTTONDOWN)
		{
			// 마우스 입력이 들어 오면 물체를 피킹하는 함수를 먼저 호출하고
			// 피킹된 오브젝트가 있다면 lockedObject로 설정한다. -> 나중에 유도탄 연산에 사용한다.
			m_pLockedObject = currentScene->PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam), m_pPlayer->m_pCamera);
			if (m_pLockedObject) m_pLockedObject->SetColor(RGB(0, 0, 0));
		}
		break;
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
		::ReleaseCapture();
		break;
	case WM_MOUSEMOVE:
		break;
	default:
		break;
	}
}

void CGameFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CScene* currentScene = m_ppScenes[m_nCurSceneIdx];
	if (currentScene) currentScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		case VK_RETURN:
			break;
		case VK_CONTROL:
			if (!m_pPlayer->m_bBlowingUp) ((CAirplanePlayer*)m_pPlayer)->FireBullet(m_pLockedObject);
			m_pLockedObject = NULL;
			break;

		// 2024 - 04 - 11
		case VK_SPACE:
			if (!m_pPlayer->m_bBlowingUp) ((CAirplanePlayer*)m_pPlayer)->ActiveShield();
			break;
		default:
			currentScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
			break;
		}
		break;
	default:
		break;
	}
}

LRESULT CALLBACK CGameFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_ACTIVATE:
	{
		if (LOWORD(wParam) == WA_INACTIVE)
			m_GameTimer.Stop();
		else
			m_GameTimer.Start();
		break;
	}
	case WM_SIZE:
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONUP:
	case WM_MOUSEMOVE:
		if (m_ppScenes[m_nCurSceneIdx]->GetSceneState() == SceneState::SCENE_START)  break;
		OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		if (m_ppScenes[m_nCurSceneIdx]->GetSceneState() == SceneState::SCENE_START) break;
		OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;
	}
	return(0);
}

void CGameFramework::ProcessInput()
{
	if (m_ppScenes[m_nCurSceneIdx]->GetSceneState() == SceneState::SCENE_START) {
		m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
		return;
	}

	static UCHAR pKeyBuffer[256];
	if (GetKeyboardState(pKeyBuffer))
	{
		DWORD dwDirection = 0;
		if (pKeyBuffer[VK_UP] & 0xF0) dwDirection |= DIR_FORWARD;
		if (pKeyBuffer[VK_DOWN] & 0xF0) dwDirection |= DIR_BACKWARD;
		if (pKeyBuffer[VK_LEFT] & 0xF0) dwDirection |= DIR_LEFT;
		if (pKeyBuffer[VK_RIGHT] & 0xF0) dwDirection |= DIR_RIGHT;
		if (pKeyBuffer[VK_PRIOR] & 0xF0) dwDirection |= DIR_UP;
		if (pKeyBuffer[VK_NEXT] & 0xF0) dwDirection |= DIR_DOWN;

		if (dwDirection && !m_pPlayer->m_bBlowingUp) m_pPlayer->Move(dwDirection, 0.15f);
	}

	// GetCaoture 가 현재 윈도우 핸들이 아닌 경우 사용자는 다른 윈도우를 메인으로 사용하고 있는 중이다.
	if (GetCapture() == m_hWnd)
	{
		SetCursor(NULL);
		POINT ptCursorPos;
		GetCursorPos(&ptCursorPos);
		float cxMouseDelta = (float)(ptCursorPos.x - m_ptOldCursorPos.x) / 2.2f;
		float cyMouseDelta = (float)(ptCursorPos.y - m_ptOldCursorPos.y) / 2.2f;
		SetCursorPos(m_ptOldCursorPos.x, m_ptOldCursorPos.y);
		if (cxMouseDelta || cyMouseDelta)
		{
			if (pKeyBuffer[VK_RBUTTON] & 0xF0)
				m_pPlayer->Rotate(cyMouseDelta, 0.0f, -cxMouseDelta);
			else
				m_pPlayer->Rotate(cyMouseDelta, cxMouseDelta, 0.0f);
		}
	}

	m_pPlayer->Update(m_GameTimer.GetTimeElapsed());
}

void CGameFramework::AnimateObjects()
{
	// 게임 월드 내의 모든 오브젝트에 대해서 Animate함수를 호출하여 움직임을 업데이트 한다.
	float fTimeElapsed = m_GameTimer.GetTimeElapsed();

	CScene* currentScene = m_ppScenes[m_nCurSceneIdx];
	if (currentScene)
		m_mapSceneFunctions[currentScene->GetSceneState()](*currentScene, fTimeElapsed);
	if (m_pPlayer) m_pPlayer->Animate(fTimeElapsed);
}

void CGameFramework::FrameAdvance()
{    
	// 타이머 업데이트 -> 사용자 입력처리 -> 오브젝트의 움직임 처리 -> 렌더링 -> Present
	m_GameTimer.Tick(60.0f);

	ProcessInput();

	AnimateObjects();

    ClearFrameBuffer(RGB(255, 255, 255));

	CCamera* pCamera = m_pPlayer->GetCamera();
	CScene* currentScene = m_ppScenes[m_nCurSceneIdx];
	if (currentScene) currentScene->Render(m_hDCFrameBuffer, pCamera);

	PresentFrameBuffer();

	m_GameTimer.GetFrameRate(m_pszFrameRate + 12, 37);
	::SetWindowText(m_hWnd, m_pszFrameRate);
}