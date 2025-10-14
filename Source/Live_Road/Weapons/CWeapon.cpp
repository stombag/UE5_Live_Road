

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
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Equip()
{
	bEquipping = true;
	if (!!EquipMontage) {
		Owner->PlayAnimMontage(EquipMontage, EquipMontage_PlayRate);
	}
}

void ACWeapon::Begin_Equip()
{
	if (RightHandSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), RightHandSocketName);
}

void ACWeapon::End_Equip()
{
	bEquipping = false;

}

bool ACWeapon::CanUnequip()
{	
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Unequip()
{
	if (HolsterSocketName.IsValid())
		CHelpers::AttachTo(this, Owner->GetMesh(), HolsterSocketName);
}

bool ACWeapon::CanFire()
{
	bool b = false;
	b |= bEquipping;
	return b == false;
}

void ACWeapon::Begin_Fire()
{

}

void ACWeapon::End_Fire()
{

}

