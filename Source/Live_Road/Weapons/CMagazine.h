#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CMagazine.generated.h"

UCLASS()
class LIVE_ROAD_API ACMagazine : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Full;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Empty;
	
public:	
	// Sets default values for this actor's properties
	ACMagazine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void SetEject();

private:
	bool bEject;
};
