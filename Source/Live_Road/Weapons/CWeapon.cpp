

#include "../Weapons/CWeapon.h"
#include "../Characters/CPlayer.h"
#include "../Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimMontage.h"

ACWeapon::ACWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Meshs/FPS_Weapon_Bundle/Weapons/Meshes/AR4/SK_AR4.SK_AR4' ");
	Mesh->SetSkeletalMesh(mesh);

	//Equip
	{
		HolsterSocketName = "Rifle_AR4_Holster";
		CHelpers::GetAsset<UAnimMontage>(&EquipMontage, "/Script/Engine.AnimMontage'/Game/Characters/Animations/Equip_Rifle_Standing_Montage.Equip_Rifle_Standing_Montage'");

	}
}

void ACWeapon::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACPlayer>(GetOwner());
	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
	}

void ACWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ACWeapon::CanEquip()
{
	return false;
}

void ACWeapon::Equip()
{
	if (!!EquipMontage)
		Owner->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
}

