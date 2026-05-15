
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_CrossHair.generated.h"

UCLASS()
class LIVE_ROAD_API UCUserWidget_CrossHair : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetMaxSpreadRadius(float InValue) { MaxRadus = InValue; }

protected: 
	void NativeOnInitialized() override;
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	void UpdateSpreadRange(float InRadius);

private:
	//캐스팅 된 border를 Borders에 넣기 
	TArray<class UBorder*> Borders;
	TArray<FVector2D> Alignments;

private:
	enum class EDirection 
	{
		Top,Botten,Left, Right, Max,
	};

private:
	float Radius;
	float MaxRadus;

};
