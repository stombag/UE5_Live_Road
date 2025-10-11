// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CAnimInstance.h"
#include "../Global.h"
#include "CPlayer.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> (TryGetPawnOwner());
	CheckNull(OwnerCharacter); 
//테스트　테스트

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// 만약 캐릭터가 없어도 실행이 잘동은 안하지만 실행은 되지 않는다 그걸 방지 

	CheckNull(OwnerCharacter);
	// 평면상의 이동속도
	Speed = OwnerCharacter->GetVelocity().Size2D();

	WeaponType = OwnerCharacter->GetWeapon()->GetType();
}
