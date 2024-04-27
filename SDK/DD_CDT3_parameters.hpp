#pragma once

// Dungeon Defenders (9.3.0) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x4)
#endif

#include "DD_CDT3_classes.hpp"

namespace Classes
{
//---------------------------------------------------------------------------
//Parameters
//---------------------------------------------------------------------------

// Function CDT3.DunDefProjectile_Meteor_HeroScaling.SpawnFires
struct ADunDefProjectile_Meteor_HeroScaling_SpawnFires_Params
{
	struct FVector                                     HitLocation;                                              // (Parm)
	struct FVector                                     HitNormal;                                                // (Parm)
};

}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
