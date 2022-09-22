#include "Avatar/Widget/WidgetAvatarEle.h"

#include "Entity.h"
#include "MyAssetManager.h"

void UWidgetAvatarEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_Row = nullptr;
}

const FColorDataRow& UWidgetAvatarEle::GetColorData() const
{
	return *m_Row->m_ColorData.GetRow<FColorDataRow>("");
}

void UWidgetAvatarEle::Init(const FAvatarRow* row)
{
	m_Row = row;
	
	m_ImgTier->SetBrushFromTexture(GetColorData().m_GlowTexture);
	//
	UMyAssetManager::Get()->LoadUnitAssetIconOnly(m_Row->m_EntityAsset,FStreamableDelegate::CreateUObject(this, &UWidgetAvatarEle::OnLoaded, m_Row->m_EntityAsset));
}

void UWidgetAvatarEle::OnLoaded(FPrimaryAssetId id)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(id));

	if(!Asset)
	{
		PRINTF("UWidgetAvatarEle:: No Avatar Data in Table");
		return;
	}

	m_ImgPortrait->SetBrushFromSoftTexture(Asset->m_Icon);
}

FReply UWidgetAvatarEle::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);

	if(!m_Row)
	{
		return FReply::Handled(); 
	}
	m_OnClick.ExecuteIfBound(m_Row);

	return FReply::Handled();
}
