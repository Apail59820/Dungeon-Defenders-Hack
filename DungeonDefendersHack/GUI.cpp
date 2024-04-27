#include "GUI.hpp"

BOOL Gui::bInit = FALSE; 
bool Gui::bDisplay = true; 
bool Gui::bSetPos = false; 
ImVec2 Gui::vWindowPos = { 0, 0 };
ImVec2 Gui::vWindowSize = { 0, 0 };

LPDIRECT3DDEVICE9 Gui::pDevice = nullptr; 
tEndScene Gui::oEndScene = nullptr;
tReset Gui::oReset = nullptr;
HWND Gui::window = nullptr;
HMODULE Gui::hDDLModule = nullptr;

int Gui::windowHeight = 0;
int Gui::windowWidth = 0;
void* Gui::d3d9Device[119];
WNDPROC Gui::OWndProc = nullptr;

HRESULT Gui::hkEndScene(const LPDIRECT3DDEVICE9 D3D9Device)
{
	if (!pDevice)
		pDevice = D3D9Device;

	if (!bInit)
		InitImGui(D3D9Device);

	if (GetAsyncKeyState(VK_INSERT) & 1)
		bDisplay = !bDisplay;

	if (globals::isDetaching)
	{
		UnHookDirectX();
		globals::bDx9UnHooked = true;
		return oEndScene(D3D9Device);
	}

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (bDisplay)
	{
		ImGui::Begin(_xor_("Dungeon Defenders Hacks"), &bDisplay);
		{
			ImGui::SetWindowSize({ 500, 300 }, ImGuiCond_Once);

			if (vWindowPos.x != 0.0f && vWindowPos.y != 0.0f && vWindowSize.x != 0.0f && vWindowSize.y != 0.0f && bSetPos)
			{
				ImGui::SetWindowPos(vWindowPos);
				ImGui::SetWindowSize(vWindowSize);
				bSetPos = false;
			}

			if (bSetPos == false)
			{
				vWindowPos = { ImGui::GetWindowPos().x, ImGui::GetWindowPos().y };
				vWindowSize = { ImGui::GetWindowSize().x, ImGui::GetWindowSize().y };
			}

			RenderMenu();

		}
		ImGui::End();
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return oEndScene(D3D9Device);
}


void Gui::InitImGui(const LPDIRECT3DDEVICE9 pDevice)
{
	D3DDEVICE_CREATION_PARAMETERS CP;
	pDevice->GetCreationParameters(&CP);
	window = CP.hFocusWindow;
	HookWindow();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.IniFilename = nullptr;
	io.Fonts->AddFontDefault();

	SetupTheme();

	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX9_Init(pDevice);

	bInit = TRUE;
}


void Gui::HookDirectX()
{
	if (GetD3D9Device(d3d9Device, sizeof(d3d9Device)))
	{
		oEndScene = (tEndScene)d3d9Device[42];
		oReset = (tReset)d3d9Device[16];
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)oEndScene, hkEndScene);
		DetourAttach(&(PVOID&)oReset, hkReset);
		DetourTransactionCommit();
	}
}

void Gui::UnHookDirectX()
{
	if (bInit)
	{
		UnHookWindow();
		ImGui_ImplDX9_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	bInit = FALSE;

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(&(PVOID&)oEndScene, hkEndScene);
	DetourDetach(&(PVOID&)oReset, hkReset);
	DetourTransactionCommit();
}


BOOL CALLBACK Gui::enumWind(const HWND handle, LPARAM lp)
{
	DWORD procID;
	GetWindowThreadProcessId(handle, &procID);
	if (GetCurrentProcessId() != procID)
		return TRUE;

	window = handle;
	return FALSE;
}

HWND Gui::GetProcessWindow()
{
	window = nullptr;

	EnumWindows(enumWind, NULL);

	RECT size;
	if (window == nullptr)
		return nullptr;

	GetWindowRect(window, &size);

	windowWidth = size.right - size.left;
	windowHeight = size.bottom - size.top;

	windowHeight -= 29;
	windowWidth -= 5;

	return window;
}

BOOL Gui::GetD3D9Device(void** pTable, const size_t size)
{
	if (!pTable)
		return FALSE;

	IDirect3D9* pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!pD3D)
		return FALSE;

	IDirect3DDevice9* pDummyDevice = nullptr;

	D3DPRESENT_PARAMETERS d3dpp = {};
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = GetProcessWindow();
	d3dpp.Windowed = (GetWindowLongPtr(d3dpp.hDeviceWindow, GWL_STYLE) & WS_POPUP) != 0 ? FALSE : TRUE;

	if (HRESULT dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice); dummyDevCreated != S_OK)
	{
		d3dpp.Windowed = !d3dpp.Windowed;
		dummyDevCreated = pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, d3dpp.hDeviceWindow, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &pDummyDevice);

		if (dummyDevCreated != S_OK)
		{
			pD3D->Release();
			return FALSE;
		}
	}

	memcpy(pTable, *(void***)(pDummyDevice), size);
	pDummyDevice->Release();
	pD3D->Release();
	return TRUE;
}

void Gui::HookWindow()
{
	OWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
}


void Gui::UnHookWindow()
{
	SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)OWndProc);
}

LRESULT WINAPI Gui::WndProc(const HWND hWnd, const UINT msg, const WPARAM wParam, const LPARAM lParam)
{
	if (bDisplay && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	{
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);
		return true;
	}

	if (bInit)
		ImGui::GetIO().MouseDrawCursor = ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow);

	if (msg == WM_CLOSE)
	{
		UnHookDirectX();
		UnHookWindow();
		TerminateProcess(GetCurrentProcess(), 0);
	}

	if (ImGui::GetIO().WantCaptureMouse)
	{
		if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AnyWindow))
			return true;
		return false;
	}

	return CallWindowProc(OWndProc, hWnd, msg, wParam, lParam);
}

HRESULT Gui::hkReset(D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	bSetPos = true;
	UnHookWindow();
	bInit = FALSE;
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
	pDevice = nullptr;

	return oReset(pPresentationParameters);
}


void Gui::RenderMenu()
{

	if (ImGui::BeginTabBar(_xor_("#tabs")))
	{
		#ifdef DEBUG
				if (ImGui::BeginTabItem(_xor_("Debug")))
				{
					ImGui::Text(_xor_("GObjects:\t[0x%p]\tObjects count: %d"), UObject::GObjects, UObject::GObjects->Num());
					ImGui::Text("GNames:\t[0x%p]\tNames count: %d", FName::GNames, FName::GNames->Num());
					ImGui::Text("UdkMain:\t[0x%p]\n", globals::UdkMain);
					ImGui::Text("UEngine:\t[0x%p]\n", globals::Engine);
					ImGui::Text("StaticLoadObject:\t[0x%p]\n", globals::OriginalStaticLoadObject);
					ImGui::Text("ADunDefLocalPlayerController:\t[0x%p]\n", globals::ADunDefPC);

					ImGui::EndTabItem();
				}
		#endif // DEBUG

		if (ImGui::BeginTabItem(_xor_("Player")))
		{
			if (globals::ADunDefPC != nullptr) {
				// Player hacks
			}
			else {
				ImGui::Text(_xor_("Loading player info..."));
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem(_xor_("Misc")))
		{
			// Test
			if (ImGui::Button(_xor_("Spawn Random Item"), ImVec2(130, 40))) {
				globals::ADunDefPC->SpawnRandomItem(5.0f, 6.0f);
			}
		}
		
		ImGui::EndTabBar();
	}
}

void Gui::SetupTheme()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	style->Colors[ImGuiCol_Text] = ImVec4(0.30f, 0.34f, 0.42f, 1.00f);
	style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.64f, 0.67f, 1.00f);
	style->Colors[ImGuiCol_WindowBg] = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
	style->Colors[ImGuiCol_Border] = ImVec4(0.85f, 0.85f, 0.88f, 0.30f);
	style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_FrameBg] = ImVec4(0.90f, 0.92f, 0.94f, 1.00f);
	style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.00f, 0.98f, 0.95f, 1.00f);
	style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_TitleBg] = ImVec4(0.98f, 0.92f, 0.84f, 1.00f);
	style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 1.00f, 1.00f, 0.75f);
	style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.94f, 0.88f, 0.80f, 1.00f);
	style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.86f, 0.86f, 0.90f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.88f, 0.88f, 0.92f, 1.00f);
	style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.78f, 0.82f, 0.88f, 0.31f);
	style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.74f, 0.78f, 0.84f, 0.78f);
	style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.69f, 0.73f, 0.79f, 1.00f);
	style->Colors[ImGuiCol_CheckMark] = ImVec4(0.25f, 0.47f, 0.71f, 1.00f);
	style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.28f, 0.56f, 0.77f, 0.78f);
	style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.25f, 0.45f, 0.66f, 1.00f);
	style->Colors[ImGuiCol_Button] = ImVec4(0.82f, 0.82f, 0.86f, 1.00f);
	style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.85f, 0.85f, 0.89f, 1.00f);
	style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.92f, 0.84f, 1.00f);
	style->Colors[ImGuiCol_Header] = ImVec4(0.76f, 0.76f, 0.80f, 1.00f);
	style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.95f, 0.89f, 0.81f, 1.00f);
	style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.96f, 0.96f, 0.98f, 1.00f);
	style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.28f, 0.56f, 0.77f, 1.00f);
	style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.25f, 0.45f, 0.66f, 1.00f);
	style->Colors[ImGuiCol_PlotLines] = ImVec4(0.47f, 0.47f, 0.69f, 0.63f);
	style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.36f, 0.61f, 0.81f, 1.00f);
	style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.36f, 0.61f, 0.81f, 1.00f);
	style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.73f, 0.84f, 0.95f, 0.35f);
}