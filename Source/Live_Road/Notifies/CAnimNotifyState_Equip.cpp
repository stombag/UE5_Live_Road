// Fill out your copyright notice in the Description page of Project Settings.


#include "../Notifies/CAnimNotifyState_Equip.h"
#include "../Global.h"
#include "../Weapons/CWeaponComponent.h"
FString UCAnimNotifyState_Equip::GetNotifyName_Implementation() const
{

	return "Equip";
}

void UCAnimNotifyState_Equip::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	// 게임 모드가 작동을 안하면 이것도 작동 안함
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	weapon->Begin_Equip();


}

void UCAnimNotifyState_Equip::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

    UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	
	weapon->End_Equip();

}
