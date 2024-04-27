#include "stdafx.h"

#include <detours/detours.h>
#include <cwchar>
#include "PatternFinder.hpp"
#include "GUI.hpp"

#include "globals.hpp"

#include "PostRender.hpp"


typedef void(__thiscall* tProcessEvent)(class Classes::UObject*, class UFunction*, void*, void*);
tProcessEvent ProcessEvent;

HMODULE gModule;

void __fastcall HookedPE(UObject* pObject, void* edx, UFunction* pFunction, void* pParams, void* pResult)
{
    if (!globals::isDetaching) {

        if (globals::bEnableRainbow) {
            utils::rainbow::tick();
        }

        auto FunctionName = pFunction->GetFullName();

        if (!Gui::bInit) {
            return ProcessEvent(pObject, pFunction, pParams, pResult);
        }

        if (FunctionName == _xor_("Function UDKGame.DunDefViewportClient.PostRender") && globals::bEnablePostRender) {
            Render::PostRender(((UDunDefViewportClient_PostRender_Params*)(pParams))->Canvas);
        }

        if (FunctionName == _xor_("Function UDKGame.Main.Tick")) {
            if (globals::UdkMain != (AMain*)pObject) {
                globals::UdkMain = reinterpret_cast<AMain*>(pObject);
                globals::Engine = globals::UdkMain->GetEngine();
                globals::ADunDefPC = globals::UdkMain->STATIC_GetPrimaryLocalPlayerController();
            }
        }

        // UDunDefLocalPlayerController addy changed
        // Is this function called by other player's ticks ?
        if (FunctionName == _xor_("Function UDKGame.DunDefPlayer.Tick")) {
            if (globals::PreviousPlayerTick != pObject && globals::UdkMain != nullptr) {
                globals::ADunDefPC = globals::UdkMain->STATIC_GetPrimaryLocalPlayerController();
            }
        }
    }

    ProcessEvent(pObject, pFunction, pParams, pResult);
}


void DllUnload()
{
    while (!globals::bDx9UnHooked) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(&(PVOID&)ProcessEvent, HookedPE);
    DetourTransactionCommit();

    if (globals::HookedStaticLoadObject != nullptr) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)globals::OriginalStaticLoadObject, globals::HookedStaticLoadObject);
        DetourTransactionCommit();
    }

    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, Gui::hDDLModule, 0, nullptr);
    FreeLibraryAndExitThread(gModule, 0);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
    #ifdef DEBUG
        AllocConsole();
        freopen(_xor_("conin$"), _xor_("r"), stdin);
        freopen(_xor_("conout$"), _xor_("w"), stdout);
        freopen(_xor_("conout$"), _xor_("w"), stderr);
    #endif // DEBUG

    ProcessEvent = reinterpret_cast<tProcessEvent>(memory::FindPattern(GetModuleHandleW(_xor_(L"DunDefGame.exe")), reinterpret_cast<const unsigned char*>(globals::PEventSig), globals::PEventMask) - 0xE8);
    UObject::ProcessEventFuncAddr = reinterpret_cast<uintptr_t>(ProcessEvent);

    auto GObjectAddy = memory::FindPattern(GetModuleHandleW(_xor_(L"DunDefGame.exe")), reinterpret_cast<const unsigned char*>(globals::GObjectSig), globals::GObjectMask);
    UObject::GObjects = reinterpret_cast<decltype(UObject::GObjects)>(*reinterpret_cast<uint32_t*>(GObjectAddy + 2));

    auto FNameAddy = memory::FindPattern(GetModuleHandleW(_xor_(L"DunDefGame.exe")), reinterpret_cast<const unsigned char*>(globals::GNameSig), globals::GNameMask);
    FName::GNames = reinterpret_cast<decltype(FName::GNames)>(*reinterpret_cast<uint32_t*>(FNameAddy + 2));

    auto StaticLoadObjectAddy = memory::FindPattern(GetModuleHandleW(_xor_(L"DunDefGame.exe")), reinterpret_cast<const unsigned char*>(globals::StaticLoadObjectSign), globals::StaticLoadObjectMask);
    globals::OriginalStaticLoadObject = reinterpret_cast<decltype(globals::OriginalStaticLoadObject)>(StaticLoadObjectAddy);

    bool attached = false;

    if (globals::OriginalStaticLoadObject != nullptr) {
        DetourTransactionBegin();
        DetourUpdateThread(GetCurrentThread());
        DetourAttach(&(PVOID&)globals::OriginalStaticLoadObject, globals::HookedStaticLoadObject);
        DetourTransactionCommit();
    }

    Gui::hDDLModule = gModule;
    CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Gui::HookDirectX, nullptr, 0, nullptr);

    Sleep(1000);

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread()); 
    DetourAttach(&(PVOID&)ProcessEvent, HookedPE); 
    DetourTransactionCommit();

    while (true)
    {

        if (GetAsyncKeyState(VK_END) & 1)
        {
            globals::isDetaching = true;
            DllUnload();
            return TRUE;
        }
    }

    return TRUE;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        gModule = hModule;
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
    }
    return TRUE;
}

