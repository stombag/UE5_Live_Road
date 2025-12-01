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
	

private:
	class ACPlayer* Owner;

public:
	ACTargetActor();

public:
	void HitTarget();

protected:
	virtual void BeginPlay() override;
public :
	UPROPERTY(VisibleAnywhere)
	int value= 100; 

};
