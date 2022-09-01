#include "Avatar/Widget/WidgetAvatarEle.h"

#include "MyAssetManager.h"

void UWidgetAvatarEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

const FColorDataRow& UWidgetAvatarEle::GetColorData() const
{
	return *m_Row->m_ColorData.GetRow<FColorDataRow>("");
}

void UWidgetAvatarEle::Init(const FAvatarRow* row)
{
	m_Row = row;
	
	UAssetManager* Manager = UAssetManager::GetIfValid();
	
	UUnitAsset* MonsterData = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(m_Row->m_EntityAsset));
	
	m_ImgTier->SetBrushFromTexture(GetColorData().m_GlowTexture);

	m_ImgPortrait->SetBrushFromSoftTexture(MonsterData->m_Icon);
}

