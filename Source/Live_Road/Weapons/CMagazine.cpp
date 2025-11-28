// Fill out your copyright notice in the Description page of Project Settings.


#include "../Weapons/CMagazine.h"
#include "../Global.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACMagazine::ACMagazine()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Full, "Full",Root);
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Empty, "Empty",Root);

	Full->SetCollisionProfileName("Magazine");
	Empty->SetCollisionProfileName("Magazine");

}

// Called when the game starts or when spawned
void ACMagazine::BeginPlay()
{
	Super::BeginPlay();

	if (bEject == false)
		Full->SetVisibility(true);
	
}

void ACMagazine::SetEject()
{
	bEject = true;

	Full->SetVisibility(false);
	Empty->SetSimulatePhysics(true);
}
