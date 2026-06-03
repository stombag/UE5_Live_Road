#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_PlayerChat.generated.h"
UCLASS()
class LIVE_ROAD_API UCUserWidget_PlayerChat : public UUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerMessage(const FText& Message);

protected:

    UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerMessage;

};
