#pragma once

// Dungeon Defenders (9.3.0) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x4)
#endif

#include "DD_Basic.hpp"
#include "DD_UDKGame_classes.hpp"
#include "DD_Core_classes.hpp"

namespace Classes
{
//---------------------------------------------------------------------------
//Script Structs
//---------------------------------------------------------------------------

// ScriptStruct DunDefPortal.DunDefPortalNode.RecentTeleport
// 0x0008
struct FRecentTeleport
{
	class AActor*                                      TeleportedActor;                                          // 0x0000(0x0004)
	float                                              TeleportedTime;                                           // 0x0004(0x0004)
};

}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
