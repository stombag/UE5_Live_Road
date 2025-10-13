// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Weapons/CWeaponComponent.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIVE_ROAD_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	// 블프가 자식이다. protected는 부모자식관계에서 접근가능하다
protected:
	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	 float Speed;

	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	 float Direction;

	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	 float Pitch;

protected:
	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	 EWeaponType WeaponType = EWeaponType::Max;

	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Weapons")
	 bool bUseIK;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UFUNCTION()
	void OnWeaponTypeChanged( EWeaponType InPrevType, EWeaponType InNewType);

private:
	class ACPlayer* OwnerCharacter;
	class UCWeaponComponent* Weapon;

	FRotator PrevRotation;
};
