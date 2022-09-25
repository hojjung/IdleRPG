#include "Avatar/Widget/WidgetAvatarPanel.h"
#include "Entity.h"
#include "MyAssetManager.h"
#include "DataTableRow/AvatarData.h"

void UWidgetAvatarPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->SpawnPreviewActor(GetWorld());

	SetVisibility(ESlateVisibility::Collapsed);

	m_BtnClose->OnClicked.AddDynamic(this, &UWidgetAvatarPanel::OnClose);
	
	CreateAllElements();

	m_Preview->Init(FEntityModule::Get().GetAvatarManager()->m_AvatarInven->GetPreviewActor());
}

void UWidgetAvatarPanel::CreateAllElements()
{
	m_TotalCount = FEntityModule::Get().GetAvatarManager()->GetAvatarDatas().Num();
	
	for(const FAvatarRow* Row : FEntityModule::Get().GetAvatarManager()->GetAvatarDatas())
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

	PRINTF("Load:%d", m_AryEles.Num());
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
	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->ChangeAvatar(row->m_EntityAsset, FStreamableDelegate::CreateUObject(this, &UWidgetAvatarPanel::OnSelectLoaded, row));
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

	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->SetPreview(Asset);
}

void UWidgetAvatarPanel::OnOpen()
{
	SetVisibility(ESlateVisibility::SelfHitTestInvisible);

	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->ShowPreview();
}

void UWidgetAvatarPanel::OnClose()
{
	SetVisibility(ESlateVisibility::Collapsed);

	FEntityModule::Get().GetAvatarManager()->m_AvatarInven->HidePreview();
}
