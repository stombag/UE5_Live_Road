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
	// ���� ĳ���Ͱ� ��� ������ �ߵ��� �������� ������ ���� �ʴ´� �װ� ���� 

	CheckNull(OwnerCharacter);
	// ������ �̵��ӵ�
	Speed = OwnerCharacter->GetVelocity().Size2D();

}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	WeaponType = InNewType;

}
