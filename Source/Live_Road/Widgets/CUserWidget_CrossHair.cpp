#include "../Widgets/CUserWidget_CrossHair.h"
#include "../Global.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Border.h"

// 만약 밑에 부분 안쓸때 주석처리되면 해당 부분도 컴파일에서 빠진다.
//#define LOG_UCUserWidget_CrossHair

void UCUserWidget_CrossHair::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UCanvasPanel* canvas = Cast<UCanvasPanel>(WidgetTree->RootWidget);
	CheckNull(canvas);

	TArray<UWidget*> widgets = canvas->GetAllChildren();

	// widgets에 있는 값을 만큼 반복한다.
	for (UWidget* widget : widgets) {
		UBorder* border = Cast<UBorder>(widget);
		// 널이 들어오면 넘긴다.
		if (border == nullptr) continue;

		//private에 있는 borders에 넣기 
		Borders.Add(border);

		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(border->Slot);
		Alignments.Add(slot->GetAlignment());
	}
	//CHelpers::Log("UI Creation");


// 색이 빠진다는건 컴파일 안된다는것이다. 
#ifdef LOG_UCUserWidget_CrossHair
	for (int32 i = 0; i < Borders.Num(); i++) {
		UBorder* Border = Borders[i];

		CHelpers::Log(Border->GetName());
		CHelpers::Log(Alignments[i].ToString());

	}
#endif

}

void UCUserWidget_CrossHair::NativeConstruct()
{
	Super::NativeConstruct();


}

void UCUserWidget_CrossHair::NativeDestruct()
{	
	Super::NativeDestruct();


}

void UCUserWidget_CrossHair::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	float minimum = 0.0f, maximum = 0.0f;
	for (int32 i = 0; i < (int32)EDirection::Max; i++)
	{
		switch ((EDirection)i)
		{
		case EDirection::Top:
		{
			minimum = Alignments[i].Y;
			maximum = Alignments[i].Y + MaxRadus;

		}
		break;
		case EDirection::Botten:
		{
			minimum = Alignments[i].Y;
			maximum = Alignments[i].Y - MaxRadus;

		}
		break;
		case EDirection::Left:
		{
			minimum = Alignments[i].X;
			maximum = Alignments[i].X + MaxRadus;

		}
		break;
		case EDirection::Right:
		{
			minimum = Alignments[i].X;
			maximum = Alignments[i].X - MaxRadus;

		}
		break;

		}
		float value = FMath::Lerp<float>(minimum, maximum, Radius);

		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Borders[i]->Slot);
		switch ((EDirection)i)
		{
		case EDirection::Top:
		case EDirection::Botten:
			slot->SetAlignment(FVector2D(Alignments[i].X , value));
			break;

		case EDirection::Left:
		case EDirection::Right:
			slot->SetAlignment(FVector2D(value, Alignments[i]. Y));
			break;

		}
	}//for(i)

}

void UCUserWidget_CrossHair::UpdateSpreadRange(float InRadius)
{
	Radius = InRadius;
}
