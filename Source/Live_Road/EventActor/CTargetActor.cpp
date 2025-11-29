// Fill out your copyright notice in the Description page of Project Settings.


#include "../EventActor/CTargetActor.h"
#include "../Global.h"
#include "../Weapons/CWeapon.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
ACTargetActor::ACTargetActor()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &Mesh, "Target",Root);
	UStaticMesh* mesh;
	CHelpers::GetAsset<UStaticMesh>(&mesh, "/Script/Engine.StaticMesh'/Game/Meshs/Wooden_target_01v1/wooden_target1v1.wooden_target1v1'");

	Mesh->SetStaticMesh(mesh);
	Mesh->SetRelativeScale3D(FVector(5, 5, 5));
}

void ACTargetActor::HitTarget()
{ 
	CHelpers::Log();
	Mesh->SetVisibility(false);
	Destroy();
}

// Called when the game starts or when spawned
void ACTargetActor::BeginPlay()
{
	Super::BeginPlay();
	
}




// hit -> Event-> Num->UI
