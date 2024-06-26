#pragma once

// Dungeon Defenders (9.3.0) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x4)
#endif

#include "DD_CDT8_structs.hpp"

namespace Classes
{
//---------------------------------------------------------------------------
//Classes
//---------------------------------------------------------------------------

// Class CDT8.DunDefWeapon_MagicStaff_Channeling
// 0x0030 (0x0648 - 0x0618)
class ADunDefWeapon_MagicStaff_Channeling : public ADunDefWeapon_MagicStaff
{
public:
	class ADunDefProjectile*                           ChannelingProjectileTemplate;                             // 0x0618(0x0004) (Edit)
	float                                              ChannelingProjectileFireSpeed;                            // 0x061C(0x0004) (Edit)
	float                                              ChannelingProjectileDamageMultiplier;                     // 0x0620(0x0004) (Edit)
	struct FVector                                     ChannelingProjectileLocationOffset;                       // 0x0624(0x000C) (Edit)
	float                                              ChannelingRangeMultiplier;                                // 0x0630(0x0004) (Edit)
	TScriptInterface<class UDunDefTargetableInterface> act;                                                      // 0x0634(0x0008) (Transient)
	TArray<TScriptInterface<class UDunDefTargetableInterface>> Enemies;                                                  // 0x063C(0x000C) (Transient, NeedCtorLink)

	static UClass* StaticClass()
	{
		static auto ptr = UObject::FindClass("Class CDT8.DunDefWeapon_MagicStaff_Channeling");
		return ptr;
	}

};


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
