#pragma once

#include "stdafx.h"

typedef HRESULT(_stdcall* EndScene)(LPDIRECT3DDEVICE9 pDevice);

namespace globals
{
	extern inline bool isDetaching = false;
	extern inline bool bDx9UnHooked = false;

	static const inline unsigned char* PEventSig = reinterpret_cast<const unsigned char*>(_xor_("\x74\x00\x83\xC0\x07\x83\xE0\xF8\xE8\x00\x00\x00\x00\x8B\xC4"));
	static const inline char* PEventMask = _xor_("x?xxxxxxx????xx");

	static const inline unsigned char* GObjectSig = reinterpret_cast<const unsigned char*>(_xor_("\x8B\x00\x00\x00\x00\x00\x8B\x04\x00\x8B\x40\x00\x25\x00\x02\x00\x00"));
	static const inline char* GObjectMask = _xor_("x?????xx?xx?xxxxx");

	static const inline unsigned char* GNameSig = reinterpret_cast<const unsigned char*>(_xor_("\x8B\x0D\x00\x00\x00\x00\x83\x3C\x81\x00\x74"));
	static const inline char* GNameMask = _xor_("xx????xxxxx");

	static const inline unsigned char* StaticLoadObjectSign = 
		reinterpret_cast<const unsigned char*>
		(_xor_("\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x70\x53\x56\x57\xA1\x00\x00\x00\x00\x33\xC5\x50\x8D\x45\xF4\x64\xA3\x00\x00\x00\x00\x89\x65\xF0\x33"));

	static const inline char* StaticLoadObjectMask = _xor_("xxxxxx????xxxxxxxxxxxxxx????xxxxxxxxxxxxxxxx");

	extern inline AMain* UdkMain = nullptr;
	extern inline ADunDefPlayerController* ADunDefPC = nullptr;
	extern inline UEngine* Engine = nullptr;
	extern inline ADunDefPlayer* PreviousPlayerTick = nullptr;

	extern inline bool bEnableRainbow = true;
	extern inline bool bEnablePostRender = true;

	static LPDIRECT3DDEVICE9 PD3D_DEVICE = nullptr;
	static IDirect3D9* PD3D = nullptr;
	static HWND    WINDOW = nullptr;
	static WNDPROC WNDPROC_ORIGNAL = nullptr;
	static bool isMenuToggled = false;
	static bool isInit = false;
	static EndScene oEndScene;

	extern inline UObject* (*OriginalStaticLoadObject)(UClass* ObjectClass, UObject* Outer, const wchar_t* Name, const wchar_t* FileName, uint32_t LoadFlags, uintptr_t Sandbox, bool bAllowObjectReconciliation) = nullptr;

	inline UObject* HookedStaticLoadObject(UClass* ObjectClass, UObject* Outer, const wchar_t* Name, const wchar_t* FileName, uint32_t LoadFlags, uintptr_t Sandbox, bool bAllowObjectReconciliation) {

		#ifdef DEBUG
		printf(_xor_("################ LOADED OBJECT ############\n"));
		wprintf(_xor_(L"Name : %ls ("), Name);
		printf(_xor_("%s)\n"), ObjectClass->GetFullName().c_str());
		printf(_xor_("###########################################\n"));

		#endif // DEBUG
		
		return OriginalStaticLoadObject(ObjectClass, Outer, Name, FileName, LoadFlags, Sandbox, bAllowObjectReconciliation);
	}
}