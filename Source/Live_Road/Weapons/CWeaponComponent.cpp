#include "../Weapons/CWeaponComponent.h"
#include "../Global.h"
#include "CWeapon.h"
#include "../Characters/CPlayer.h"
#include "../Widgets/CUserWidget_HUD.h"	
#include "GameFramework/PlayerController.h"
UCWeaponComponent::UCWeaponComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true; 
	CHelpers::GetClass<UCUserWidget_HUD>(&HudClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_HUD.WB_HUD_C'");

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

	if (!!HudClass) {

		 Hud = CreateWidget<UCUserWidget_HUD, APlayerController*>(OwnerCharacter->GetController<APlayerController>(), HudClass);
		 Hud->AddToViewport();
		 Hud->SetVisibility(ESlateVisibility::Hidden);

	}

}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {


	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(GetCurrentWeapon());

	if (!!Hud) {
		uint8 currCount = GetCurrentWeapon()->GetCurrentMagazineCount();
		uint8 maxCount = GetCurrentWeapon()->GetMaxMagazineCount();

		Hud->UpdateMagazineCount(currCount, maxCount);

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

	if (!!Hud)
		Hud->SetVisibility(ESlateVisibility::Hidden);



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

	if (!!Hud) {

		Hud->SetVisibility(ESlateVisibility::Visible);
		Hud->DrawAutoFire(Weapons[(int32)InType]->IsAutoFire());		
		Hud->UpdateWeaponType(CurrentType);
	}

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

bool UCWeaponComponent::InAim()
{
	CheckNullResult(GetCurrentWeapon(), false);
	return GetCurrentWeapon()->InAim();

}
void UCWeaponComponent::ToggleAutoFire() {

	CheckNull(GetCurrentWeapon());

	GetCurrentWeapon()->ToggleAutoFire();

	if (!!Hud)
		Hud->DrawAutoFire(GetCurrentWeapon()->IsAutoFire());

}
void UCWeaponComponent::Reload()
{

	CheckNull(GetCurrentWeapon());
	CheckFalse(GetCurrentWeapon()->CanReload());
	GetCurrentWeapon()->Reload();
}
void UCWeaponComponent::Eject_Magazine()
{

	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->Eject_Magazine();

}

void UCWeaponComponent::Spawn_Magazine()
{
	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->Spawn_Magazine();
}

void UCWeaponComponent::Load_Magazine()
{
	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->Load_Magazine();
}

void UCWeaponComponent::End_Reload()
{
	CheckNull(GetCurrentWeapon());
	GetCurrentWeapon()->End_Reload();
}






