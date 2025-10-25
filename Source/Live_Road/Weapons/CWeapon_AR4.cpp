

#include "../Weapons/CWeapon_AR4.h"
#include "../Global.h"
#include "Components/SkeletalMeshComponent.h"
ACWeapon_AR4::ACWeapon_AR4()
{
	Type = EWeaponType::AR4;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Meshs/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4' ");
	Mesh->SetSkeletalMesh(mesh);


	//Equip
	{
		HolsterSocketName = "Rifle_AR4_Holster";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'");
		RightHandSocketName = "Rifle_AR4_RightHand";
		LeftHandLocation = FVector(-30.0f, 11.0f, 9.0f);
	}
	//Aim
	{
		BaseData.TargetArmLength = 200;
		BaseData.SoketOffset = FVector(0, 50, 15);
		BaseData.bEnableCameraLag = true;
		BaseData.FielOfView = 90;

		AimData.TargetArmLength = 80;
		AimData.SoketOffset = FVector(0, 55, 10);
		AimData.FielOfView = 65;
		AimData.bEnableCameraLag = false;

	}


	//Fire
	{
		RecoilAnlgle = 0.75f;
	}


}
