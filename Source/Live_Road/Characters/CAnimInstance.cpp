// Fill out your copyright notice in the Description page of Project Settings.


#include "../Characters/CAnimInstance.h"
#include "../Global.h"
#include "CPlayer.h"


void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACPlayer> (TryGetPawnOwner());
	CheckNull(OwnerCharacter); 
//�׽�Ʈ���׽�Ʈ

}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	// ���� ĳ���Ͱ� ��� ������ �ߵ��� �������� ������ ���� �ʴ´� �װ� ���� 

	CheckNull(OwnerCharacter);
	// ������ �̵��ӵ�
	Speed = OwnerCharacter->GetVelocity().Size2D();

	WeaponType = OwnerCharacter->GetWeapon()->GetType();
}
