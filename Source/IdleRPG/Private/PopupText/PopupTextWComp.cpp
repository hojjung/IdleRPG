#include "PopupText/PopupTextWComp.h"

#include "BUITween.h"
#include "Engine/Font.h"
#include "PopupText/PopupTextWidget.h"
#include "PopupText/WidgetPopupText.h"

UPopupTextWComp::UPopupTextWComp()//
{
	static ConstructorHelpers::FClassFinder<UUserWidget> FoundW(
	TEXT("WidgetBlueprint'/Game/Blueprints/Widgets/MainGame/WorldText/WB_PopupText.WB_PopupText_C'"));
	m_ClassTextWidget = FoundW.Class;

	SetDrawAtDesiredSize(true);
	SetPivot(FVector2D (0.5f, 0.5f));
	SetWidgetSpace(EWidgetSpace::Screen);
	SetCanEverAffectNavigation(false);
	SetManuallyRedraw(true);
}

void UPopupTextWComp::BeginPlay()
{
	Super::BeginPlay();
	
	SetWidgetClass(m_ClassTextWidget);

	m_TextWidget =  Cast<UWidgetPopupText>( GetUserWidgetObject());

	m_TextWidget->m_ParentComponent = this;
	
	EndAnimation();
}

void UPopupTextWComp::SetTextWant(const FVector& loc, const FText& text, int index)
{
	SetHiddenInGame(false);
	
	SetWorldLocation(loc);

	m_TextWidget->SetTextWant(text,index);
	
	RequestRedraw();
}
void UPopupTextWComp::EndAnimation()
{
	SetHiddenInGame(true);
	//SetRelativeLocation(FVector(0,0,0));
}