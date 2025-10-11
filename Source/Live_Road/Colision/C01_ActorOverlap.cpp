// Fill out your copyright notice in the Description page of Project Settings.


#include "../Colision/C01_ActorOverlap.h"
#include "../Global.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
// Sets default values
AC01_ActorOverlap::AC01_ActorOverlap()
{
//<> 붙이는 이유
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UBoxComponent>(this, &Box, "Box");

	CreateTextRenderComponent();

	Box->bHiddenInGame = false;
	Box->SetRelativeScale3D(FVector(3));

}

// Called when the game starts or when spawned
void AC01_ActorOverlap::BeginPlay()
{
	Super::BeginPlay();
	


}
