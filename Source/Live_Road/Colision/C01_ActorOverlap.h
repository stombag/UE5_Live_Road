// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C01_ActorOverlap.generated.h"

UCLASS()
class LIVE_ROAD_API AC01_ActorOverlap : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnyWhere)
	class UBoxComponent* Box;
	UPROPERTY(VisibleAnywhere)
	class UTextRenderComponent* Text;


public:	
	AC01_ActorOverlap();

protected:
	virtual void BeginPlay() override;

public:	

};
