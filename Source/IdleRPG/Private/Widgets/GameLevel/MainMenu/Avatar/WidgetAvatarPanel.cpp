#include "Widgets/GameLevel/MainMenu/Avatar/WidgetAvatarPanel.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AvatarManager =TWeakPtr<AvatarManager>(UMyGameInstance::Get->m_AvatarManager);

	m_AvatarManager.Pin()->m_AvatarInven->SpawnPreviewActor(GetWorld());

	SetVisibility(ESlateVisibility::Collapsed);

	CreateAllElements();

	m_Preview->Init(m_AvatarManager.Pin()->m_AvatarInven->GetPreviewActor());
}

void UWidgetAvatarPanel::NativeDestruct()
{
	Super::NativeDestruct();

	m_AvatarManager.Reset();
}

void UWidgetAvatarPanel::CreateAllElements()
{
	m_TotalCount = UAvatarData::GetAvatarTable->GetRowMap().Num();
	
	UAvatarData::GetAvatarTable->ForeachRow<FAvatarRow>("",[&](const FName& key, const FAvatarRow& row)
	{
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnAvatarLoaded, &row);

		UMyAssetManager::Get()->LoadUnitAssetIconOnly(row.m_EntityAsset, Delegate);
	});
}

void UWidgetAvatarPanel::OnAvatarLoaded(const FAvatarRow* row)
{
	UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this, m_ClassEle);
		
	Ele->Init(*row);

	Ele->m_OnClick.BindUObject(this, &UWidgetAvatarPanel::OnSelect);

	m_AryEles.Add(Ele);

	if(m_AryEles.Num() >= m_TotalCount)
	{
		SortAvatar();
	}
}

void UWidgetAvatarPanel::SortAvatar()
{
	m_AryEles.Sort([](const UWidgetAvatarEle& LHS, const UWidgetAvatarEle& RHS)
	{
		int LhsLevel = LHS.GetColorData().m_fPriority;

		int RhsLevel = RHS.GetColorData().m_fPriority;

		return LhsLevel < RhsLevel;
	});

	for(UWidgetAvatarEle* Ele : m_AryEles)
	{
		m_Wrap->AddChildToWrapBox(Ele);
	}
}

void UWidgetAvatarPanel::OnSelect(const FAvatarRow& row)
{
	m_AvatarManager.Pin()->m_AvatarInven->ChangeAvatar(row.m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, &row));
}

void UWidgetAvatarPanel::OnSelectLoaded(const FAvatarRow* row)
{
	UUnitAsset* Asset = Cast<UUnitAsset>(UMyAssetManager::Get()->GetPrimaryAssetObject(row->m_EntityAsset));
	
	const FColorDataRow* ColorData = row->m_ColorData.GetRow<FColorDataRow>("");
	
	m_ImgGlow->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextName->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextTierName->SetColorAndOpacity(ColorData->m_Color.GetSpecifiedColor());

	m_TextName->SetText(Asset->m_ShowingName);
	
	m_TextTierName->SetText(ColorData->m_Name);

	m_AvatarManager.Pin()->m_AvatarInven->SetPreview(Asset);
}

void UWidgetAvatarPanel::OnOpen()
{
	Super::OnOpen();

	m_AvatarManager.Pin()->m_AvatarInven->ShowPreview();
}

void UWidgetAvatarPanel::OnClose()
{
	Super::OnClose();

	m_AvatarManager.Pin()->m_AvatarInven->HidePreview();
}
