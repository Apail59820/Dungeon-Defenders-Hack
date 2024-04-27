// Dungeon Defenders (9.3.0) SDK

#ifdef _MSC_VER
	#pragma pack(push, 0x8)
#endif

#include "DD_CDT3_parameters.hpp"

#pragma warning( disable : 4244 )

namespace Classes
{
//---------------------------------------------------------------------------
//Functions
//---------------------------------------------------------------------------

// Function CDT3.DunDefProjectile_Meteor_HeroScaling.SpawnFires
// (Defined, Public, HasDefaults)
// Parameters:
// struct FVector                 HitLocation                    (Parm)
// struct FVector                 HitNormal                      (Parm)

void ADunDefProjectile_Meteor_HeroScaling::SpawnFires(const struct FVector& HitLocation, const struct FVector& HitNormal)
{
	static auto fn = UObject::FindObject<UFunction>("Function CDT3.DunDefProjectile_Meteor_HeroScaling.SpawnFires");

	ADunDefProjectile_Meteor_HeroScaling_SpawnFires_Params params;
	params.HitLocation = HitLocation;
	params.HitNormal = HitNormal;

	auto flags = fn->FunctionFlags;

	UObject::ProcessEvent(fn, &params);

	fn->FunctionFlags = flags;
}


}

#ifdef _MSC_VER
	#pragma pack(pop)
#endif
