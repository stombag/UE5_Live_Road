#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CWeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR4,AK47, Pistol,Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponTypeChanged, EWeaponType, InPrevType, EWeaponType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LIVE_ROAD_API UCWeaponComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
	TArray<TSubclassOf<class ACWeapon>> WeaponClasses;

public:

	FORCEINLINE EWeaponType GetType() { return CurrentType; }
public:
	FORCEINLINE bool IsUnarmedMode() { return CurrentType == EWeaponType::Max; }
	FORCEINLINE bool IsAR4Mode() { return CurrentType == EWeaponType::AR4; }
	FORCEINLINE bool IsAK47Mode() { return CurrentType == EWeaponType::AK47; }
	FORCEINLINE bool InPistolMode() { return CurrentType == EWeaponType::Pistol; }

public:	
	UCWeaponComponent();

protected:
	virtual void BeginPlay() override;
private:
	class ACWeapon* GetCurrentWeapon();
public:
	FVector GetLeftHandLocation();
public:
	void SetUnarmdMode();
	void SetAR4Mode();

private:
	void SetMode(EWeaponType InType);
	void ChangeType(EWeaponType InType);
public:
	void Begin_Equip();
	void End_Equip();
public:
	void Begin_Fire();
	void End_Fire();

public:
	void Begin_Aim();
	void End_Aim();

public:
	FWeaponTypeChanged OnWeaponTypeChanged;

private:
	EWeaponType CurrentType = EWeaponType::Max;
private:
	class ACPlayer* OwnerCharacter;
	TArray<class ACWeapon*> Weapons;
};
