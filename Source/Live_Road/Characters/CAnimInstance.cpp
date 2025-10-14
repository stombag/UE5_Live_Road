// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CAnimInstance.h"
#include "../Global.h"
#include "CPlayer.h"
#include "../Weapons/CWeaponComponent.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> (TryGetPawnOwner());
	CheckNull(OwnerCharacter); 

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	CheckNull(Weapon);


	Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 만약 캐릭터가 없어도 실행이 잘동은 안하지만 실행은 되지 않는다 그걸 방지 

	CheckNull(OwnerCharacter);
	// 평면상의 이동속도
	Speed = OwnerCharacter->GetVelocity().Size2D();

// 현재 캐릭터의 이동 방향(속도 벡터)을 회전값(FRotator)으로 변환하여 저장
// 예: 앞(+X)으로 이동 → Yaw=0°, 오른쪽(+Y) → Yaw=90°
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator target = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation,target, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	CheckNull(Weapon);

	bUseIK = Weapon->IsUnarmedMode() == false;
	LeftHandLocation = Weapon->GetLeftHandLocation();
} 

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	WeaponType = InNewType;

}
