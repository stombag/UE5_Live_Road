// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CTargetActor.generated.h"

UCLASS()
class LIVE_ROAD_API ACTargetActor : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	
public:	
	// Sets default values for this actor's properties
	ACTargetActor();
public:
	void HitTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
