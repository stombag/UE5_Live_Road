// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	AR4,AK47, Pistol,Max,
};

UCLASS()
class LIVE_ROAD_API ACWeapon : public AActor
{
	GENERATED_BODY()

private: // 설정
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	FName HolsterSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	class UAnimMontage* EquipMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	float EquipMontage_PlayRate = 1;

private: // 컴퍼넌트
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

private:// 메쉬	
	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

public:
	FORCEINLINE const EWeaponType GetType() { return Type; }

public:	
	ACWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	bool CanEquip();
	void Equip();

private:
	EWeaponType Type = EWeaponType::Max;
private:
	class ACPlayer* Owner;

};
