#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "Components/EditableText.h"
#include "Components/ScrollBox.h"
#include "CUserWidget_MainChat.generated.h"


class UCUserWidget_PlayerChat;
class UCUserWidget_NPCChat;

UCLASS()
class LIVE_ROAD_API UCUserWidget_MainChat : public UUserWidget
{
	GENERATED_BODY()
protected:

    virtual void NativeConstruct() override;

    UFUNCTION()
    void OnInputCommitted(const FText& Text,ETextCommit::Type CommitMethod);

    UPROPERTY(meta = (BindWidget))
    UEditableText* InputChatText;

    UPROPERTY(meta = (BindWidget))
    UScrollBox* ChatScroll;

public:

    UPROPERTY(EditAnywhere)
    TSubclassOf<UCUserWidget_PlayerChat> PlayerChatClass;
    UPROPERTY(EditAnywhere)
    TSubclassOf<UCUserWidget_NPCChat> NPCChatClass;
	
	// Variable
private:
	bool bRequesting = false;

void SendRequest(const FString& Message);

void OnResponseReceived(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful
);

};
