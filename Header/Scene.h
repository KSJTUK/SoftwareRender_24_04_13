#pragma once

#include "GameObject.h"
#include "Camera.h"
#include "Player.h"
#include "Enemy.h"

class CScene
{
public:
	CScene(CPlayer *pPlayer);
	virtual ~CScene();

protected:
	int							m_nObjects = 0;
	CGameObject**				m_ppObjects = NULL;

	CWallsObject*				m_pWallsObject = NULL;

	CPlayer*					m_pPlayer = NULL;

	CGameObject*				m_pStartButton = NULL;

#ifdef _WITH_DRAW_AXIS
	CGameObject*				m_pWorldAxis = NULL;
#endif

public:
	virtual void BuildObjects() { }
	virtual void ReleaseObjects() { }

	void CheckObjectByObjectCollisions(float fElapsedTime);
	void CheckObjectByWallCollisions(float fElapsedTime);
	void CheckPlayerByWallCollision();
	void CheckObjectByBulletCollisions();

	virtual void Animate(float fElapsedTime) { }
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera) { }

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) { }
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) { }

	CGameObject* PickObjectPointedByCursor(int xClient, int yClient, CCamera* pCamera);
};

class CStartScene : public CScene
{
public:
	CStartScene(CPlayer* pPlayer) : CScene(pPlayer) { }
	virtual ~CStartScene() { }

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};

class CPlayScene : public CScene
{
public:
	CPlayScene(CPlayer* pPlayer) : CScene(pPlayer) { }
	virtual ~CPlayScene() { } 

public:
	virtual void BuildObjects();
	virtual void ReleaseObjects();

	virtual void Animate(float fElapsedTime);
	virtual void Render(HDC hDCFrameBuffer, CCamera* pCamera);

	virtual void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
};
