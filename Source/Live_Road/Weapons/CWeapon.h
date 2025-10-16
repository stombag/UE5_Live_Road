// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponComponent.h"
#include "CWeapon.generated.h"


UCLASS(Abstract, NotBlueprintable)
class LIVE_ROAD_API ACWeapon : public AActor
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	EWeaponType Type = EWeaponType::Max;

protected: // 설정
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	class UAnimMontage* EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float EquipMontage_PlayRate = 1;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName RightHandSocketName;


	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FVector LeftHandLocation;
	//IA 적용할때 넣을값

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	float HitDistance = 3000;
	// 발사되는 거리 

	UPROPERTY(EditDefaultsOnly, Category = "Hit")
	class UMaterialInstanceConstant* HitDecal;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float RecoilAnlgle;
	// 에임 흔들리는거


private: // 컴퍼넌트
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

protected:// 메쉬	
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

public:
	FORCEINLINE const EWeaponType GetType() { return Type; }
	FORCEINLINE FVector GetLeftHandLocation() { return LeftHandLocation; }

public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();
	void Begin_Equip();
	void End_Equip();

	bool CanUnequip();
	void Unequip();

public:
	bool CanFire();
	void Begin_Fire();
	void End_Fire();

private:
	class ACPlayer* Owner;
private:
	bool bEquipping;

};
