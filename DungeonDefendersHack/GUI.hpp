#ifndef GUI_H
#define GUI_H

#include "stdafx.h"
#include "globals.hpp"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
using tEndScene = HRESULT(APIENTRY*)(LPDIRECT3DDEVICE9 pDevice);
using tReset = HRESULT(APIENTRY*)(D3DPRESENT_PARAMETERS* pPresentationParameters);


class Gui
{
public:
	static bool bDisplay;
	static BOOL bInit;
	static bool bSetPos;

	static HRESULT APIENTRY hkEndScene(LPDIRECT3DDEVICE9 D3D9Device);

	static IDirect3DDevice9* pDevice;
	static tEndScene oEndScene;
	static HWND window;
	static HMODULE hDDLModule;

	static void HookDirectX();
	static void UnHookDirectX();
	static void HookWindow();
	static void UnHookWindow();

	static void RenderMenu();

	static void SetupTheme();

private:
	static int windowHeight, windowWidth;
	static void* d3d9Device[119];
	static WNDPROC OWndProc;
	static tReset oReset;


	static BOOL CALLBACK enumWind(HWND handle, LPARAM lp);
	static HWND GetProcessWindow();
	static BOOL GetD3D9Device(void** pTable, size_t size);
	static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static HRESULT APIENTRY hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters);

	static ImVec2 vWindowPos;
	static ImVec2 vWindowSize;

	static void InitImGui(LPDIRECT3DDEVICE9 pDevice);
};

#endif