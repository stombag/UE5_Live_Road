#include "../Weapons/CWeapon_AK47.h"
#include "../Global.h"
#include "CMagazine.h"
#include "../Characters/CPlayer.h"
#include "../Widgets/CUserWidget_CrossHair.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Animation/AnimMontage.h"


ACWeapon_AK47::ACWeapon_AK47() {
	
	Type = EWeaponType::AK47;
	
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Meshs/FPS_Weapon_Bundle/Weapons/Meshes/Ka47/SK_KA47.SK_KA47'");
	Mesh->SetSkeletalMesh(mesh);

	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Sight, "Sight", Mesh, "DotSight");


	UStaticMesh* staticMesh;
	CHelpers::GetAsset<UStaticMesh>(&staticMesh, "/Script/Engine.StaticMesh'/Game/Meshs/FPS_Weapon_Bundle/Weapons/Meshes/Accessories/SM_T4_Sight.SM_T4_Sight' ");
	Sight->SetStaticMesh(staticMesh);
	Sight->SetCollisionProfileName("NoCollision");


	LeftHandSocketName = "Rifle_AK47_LeftHand";


	//Equip
	{
		HolsterSocketName = "Rifle_AK47_Holster";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/MM_GrabRifleFromBackLeft_Montage.MM_GrabRifleFromBackLeft_Montage'");
		EquipMontage_PlayRate = 2;
		RightHandSocketName = "Rifle_AK47_RightHand";
		LeftHandLocation = FVector(-35.0f, 15.5f, 4.0f);
	}
	//Aim
	{
		BaseData.TargetArmLength = 200;
		BaseData.SoketOffset = FVector(0, 55, 10);
		BaseData.FielOfView = 90;
		BaseData.bEnableCameraLag = true;

		AimData.TargetArmLength = 30;
		AimData.SoketOffset = FVector(-55, 0, 10);
		AimData.FielOfView = 55;
		AimData.bEnableCameraLag = false;

	}

	//Fire
	{
		RecoilAnlgle = 0.75f;
		CHelpers::GetClass<UCameraShakeBase>(&CameraShakeClass, "/Script/Engine.Blueprint'/Game/Blueprints/Weapons/BP_CamaraShake_AK47.BP_CamaraShake_AK47_C'");
		AutoFireInterval = 0.1f;
		RecoilPitch = 0.05f;
		SpreadSpeed = 2.0f;
		MaxSpreadRadius = 2.0f;

	}


	//Magazine
	{
		MaxMagazineCount = 30;
		CHelpers::GetAsset<UAnimMontage>(&ReloadMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/MM_Rifle_Reload_Montage.MM_Rifle_Reload_Montage'");
		ReloadMontage_PlayRate = 1.5f;
		HideMagazinBoneName = "b_gun_mag";
		CHelpers::GetClass<ACMagazine>(&MagazineClass, "/Script/Engine.Blueprint'/Game/Blueprints/Weapons/BP_CMagazine_AK47.BP_CMagazine_AK47_C'");
		SpawnMagazineSocketName = "Rifle_Magazine";

	}



}

void ACWeapon_AK47::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), LeftHandSocketName);

}

void ACWeapon_AK47::End_Equip()
{
	Super::Begin_Equip();
	Super::End_Equip();
}

void ACWeapon_AK47::Begin_Aim()
{
	Super::Begin_Aim();

	Owner->SetFirstPersonMode();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Hidden);



}

void ACWeapon_AK47::End_Aim()
{
	Super::End_Aim();

	Owner->SetThirdPersonMode();

	if (!!CrossHair)
		CrossHair->SetVisibility(ESlateVisibility::Visible);

}
