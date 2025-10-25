#include "../Weapons/CWeaponComponent.h"
#include "../Global.h"
#include "CWeapon.h"
#include "../Characters/CPlayer.h"

UCWeaponComponent::UCWeaponComponent()
{

}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACPlayer>(GetOwner());
	CheckNull(OwnerCharacter);


	FActorSpawnParameters params;
	params.Owner = OwnerCharacter;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (TSubclassOf<ACWeapon> weaponClass : WeaponClasses)
	{
		if (!!weaponClass) {
			ACWeapon* weapon = OwnerCharacter->GetWorld()->SpawnActor<ACWeapon>(weaponClass, params);
			Weapons.Add(weapon);

		}
	}

}

ACWeapon* UCWeaponComponent::GetCurrentWeapon()
{

	CheckTrueResult(IsUnarmedMode(), nullptr);


	return Weapons[(int32)CurrentType];
}

FVector UCWeaponComponent::GetLeftHandLocation()
{
	CheckNullResult(GetCurrentWeapon(), FVector::ZeroVector);

	return GetCurrentWeapon()->GetLeftHandLocation();
}

void UCWeaponComponent::SetUnarmdMode()
{
	CheckFalse(GetCurrentWeapon()->CanUnequip());
	GetCurrentWeapon()->Unequip();	
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetAR4Mode()
{
	SetMode(EWeaponType::AR4);
	
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (CurrentType == InType)
	{
		SetUnarmdMode();
		return;
	}
	else if (IsUnarmedMode() == false) {
		// 현재 장착되어 있는 무기 해제

	}
	CheckNull(Weapons[(int32)InType]);
	CheckFalse(Weapons[(int32)InType]->CanEquip());

	Weapons[(int32)InType]->Equip();
	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType type = CurrentType;
	CurrentType = InType;
	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(type, InType);
}

void UCWeaponComponent::Begin_Equip()
{
	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->Begin_Equip();

}

void UCWeaponComponent::End_Equip()
{
	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->End_Equip();
}

void UCWeaponComponent::Begin_Fire()
{
	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanFire());

	GetCurrentWeapon()->Begin_Fire();
}

void UCWeaponComponent::End_Fire()
{

	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->End_Fire();

}


void UCWeaponComponent::Begin_Aim()
{
	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanAim());
	GetCurrentWeapon()->Begin_Aim();

}

void UCWeaponComponent::End_Aim()
{
	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->End_Aim();
}


