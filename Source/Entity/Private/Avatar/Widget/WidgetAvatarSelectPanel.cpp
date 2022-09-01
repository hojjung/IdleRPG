#include "Avatar/Widget/WidgetAvatarSelectPanel.h"

#include "Entity.h"
#include "MyAssetManager.h"
#include "Avatar/AvatarManager.h"

void UWidgetAvatarSelectPanel::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_AryEle.Reserve(200);

	AvatarManager* AvatarMan = FEntityModule::Get().GetAvatarManager();

	for(const FAvatarRow* Row :AvatarMan->GetAvatarDatas())
	{
		FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &UWidgetAvatarSelectPanel::OnAvatarLoaded, Row);

		UMyAssetManager::Get()->LoadUnitAssetIconOnly(Row->m_EntityAsset, Delegate);
	}
}

void UWidgetAvatarSelectPanel::OnAvatarLoaded(const FAvatarRow* row)
{
	UWidgetAvatarEle* Ele = CreateWidget<UWidgetAvatarEle>(this,m_ClassAvatarEle);

	Ele->Init(row);

	m_AryEle.Add(Ele);

	m_WrapParent->AddChildToWrapBox(Ele);
}
