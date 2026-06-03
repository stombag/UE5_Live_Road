#include "../Widgets/CUserWidget_MainChat.h"
#include "../Global.h"
#include "../Widgets/CUserWidget_PlayerChat.h"
#include "../Widgets/CUserWidget_NPCChat.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
void UCUserWidget_MainChat::NativeConstruct()
{
    Super::NativeConstruct();

    if (InputChatText)
    {
        InputChatText->OnTextCommitted.AddDynamic(
            this,
            &UCUserWidget_MainChat::OnInputCommitted
        );

    }		
}

void UCUserWidget_MainChat::OnInputCommitted(const FText& InputText, ETextCommit::Type CommitMethod)
{

    if (CommitMethod == ETextCommit::OnEnter && !bRequesting)
        return;
    bRequesting = true;
    if (PlayerChatClass)
    {
        UCUserWidget_PlayerChat* Playerchat = CreateWidget<UCUserWidget_PlayerChat>(GetWorld(), PlayerChatClass);
        CheckNull(Playerchat);
        Playerchat->SetPlayerMessage(InputText);
        ChatScroll->AddChild(Playerchat);
        ChatScroll->ScrollToEnd();
    };
    SendRequest(InputText.ToString());	
}

void UCUserWidget_MainChat::SendRequest(
    const FString& Message)
{
   auto Request =
        FHttpModule::Get().CreateRequest();

    Request->SetURL(
        TEXT("http://127.0.0.1:8000/chat")
    );

    Request->SetVerb(TEXT("POST"));

    Request->SetHeader(
        TEXT("Content-Type"),
        TEXT("application/json")
    );

    FString Json =
        FString::Printf(
            TEXT("{\"message\":\"%s\"}"),
            *Message
        );

    Request->SetContentAsString(Json);

    Request->OnProcessRequestComplete().
        BindUObject(
            this,
            &UCUserWidget_MainChat::OnResponseReceived
        );

    Request->ProcessRequest();
}

    void UCUserWidget_MainChat::OnResponseReceived(
    FHttpRequestPtr Request,
    FHttpResponsePtr Response,
    bool bWasSuccessful)
{
    bRequesting = false;
        if (!bWasSuccessful)
        return;

    FString Result =
        Response->GetContentAsString();

    if (NPCChatClass)
    {
        UCUserWidget_NPCChat* NPCChat = CreateWidget<UCUserWidget_NPCChat>(GetWorld(), NPCChatClass);
        NPCChat->SetNPCMessage(Result);
        ChatScroll->AddChild(NPCChat);
        ChatScroll->ScrollToEnd();
    }
 }
