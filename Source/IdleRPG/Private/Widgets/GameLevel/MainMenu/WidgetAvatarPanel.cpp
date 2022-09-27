#include "Widgets/GameLevel/MainMenu/WidgetAvatarPanel.h"
#include "MyAssetManager.h"
#include "MyGameInstance.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AvatarManager = UMyGameInstance::Get->m_AvatarManager.Get();
	
	m_AvatarManager->m_AvatarInven->SpawnPreviewActor(GetWorld());

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClose);
	
	CreateAllElements();

	m_Preview->Init(m_AvatarManager->m_AvatarInven->GetPreviewActor());
}

void UWidgetAvatarPanel::CreateAllElements()
{
	m_TotalCount = m_AvatarManager->GetAvatarDatas().Num();
	
	for(const FAvatarRow* Row : m_AvatarManager->GetAvatarDatas())
	{
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnAvatarLoaded, Row);

		UMyAssetManager::Get()->LoadUnitAssetIconOnly(Row->m_EntityAsset, Delegate);
	}
}

void UWidgetAvatarPanel::OnAvatarLoaded(const FAvatarRow* row)
{
	UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this, m_ClassEle);
		
	Ele->Init(row);

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

void UWidgetAvatarPanel::OnSelect(const FAvatarRow* row)
{
	m_AvatarManager->m_AvatarInven->ChangeAvatar(row->m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, row));
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

	m_AvatarManager->m_AvatarInven->SetPreview(Asset);
}

void UWidgetAvatarPanel::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	m_AvatarManager->m_AvatarInven->ShowPreview();
}

void UWidgetAvatarPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	m_AvatarManager->m_AvatarInven->HidePreview();
}
