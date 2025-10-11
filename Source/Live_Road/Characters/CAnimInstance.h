// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Weapons/CWeapon.h"
#include "CAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LIVE_ROAD_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	// ������ �ڽ��̴�. protected�� �θ��ڽİ��迡�� ���ٰ����ϴ�
protected:
	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	 float Speed;

	 UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Settings")
	 EWeaponType WeaponType = EWeaponType::Max;


public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	class ACPlayer* OwnerCharacter;
};
