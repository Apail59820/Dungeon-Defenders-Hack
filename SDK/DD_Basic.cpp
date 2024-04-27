// Dungeon Defenders (9.3.0) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#pragma warning( disable : 4244 )

#include "../SDK.hpp"

namespace Classes
{
TArray<FNameEntry*>*	FName::GNames = nullptr;
TArray<UObject*>*		UObject::GObjects = nullptr;
uintptr_t				UObject::ProcessEventFuncAddr = 0;
}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
