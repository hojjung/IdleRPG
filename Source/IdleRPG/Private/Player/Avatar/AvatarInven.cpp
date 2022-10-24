#include "Player/Avatar/AvatarInven.h"
#include "MyAssetManager.h"
#include "Manager/MyGameInstance.h"


AvatarInven::AvatarInven()
{
	SetEquip(TEXT("Avatar_39"));
	SetSkin(TEXT("Avatar_39"));
}

AvatarInven::~AvatarInven()
{
	
}

void AvatarInven::SetEquip(const FName& key)
{
	if(key == NAME_None)
	{
		m_RowEquipAvatar = nullptr;
		m_EquippedAvatar = NAME_None;
		return;
	}
	const FAvatarRow* AvatarEquip = UAvatarData::GetAvatarTable->FindRow<FAvatarRow>(key, "");

	m_RowEquipAvatar = AvatarEquip;

	m_EquippedAvatar = key;
}

void AvatarInven::SetSkin(const FName& key)
{
	if(key == NAME_None)
	{
		m_RowSkinAvatar = nullptr;
		m_EquipSkinAvatar = NAME_None;
		return;
	}
	const FAvatarRow* AvatarEquip = UAvatarData::GetAvatarTable->FindRow<FAvatarRow>(key, "");

	m_RowSkinAvatar = AvatarEquip;

	m_EquipSkinAvatar = key;
}

void AvatarInven::EquipAvatar()
{
	if(m_PreviewID == NAME_None)
	{
		return;
	}

	SetEquip(m_PreviewID);

	UpdateEquipAvatar();
}

void AvatarInven::EquipSkinAvatar()
{
	if(m_PreviewID == NAME_None)
	{
		return;
	}
	SetSkin(m_PreviewID);

	UpdateEquipAvatar();
}

void AvatarInven::UpdateEquipAvatar()
{
	if(!UMyGameInstance::Get->GetPlayerPawn())
	{
		return;
	}

	FName Skin = m_EquippedAvatar;
	
	if(m_EquipSkinAvatar != NAME_None)
	{
		Skin = m_EquipSkinAvatar;
	}

	const FAvatarRow& Avatar = *UAvatarData::GetAvatarTable->FindRow<FAvatarRow>(Skin, "");

	LoadAvatar(Avatar.m_EntityAsset, FStreamableDelegate::CreateLambda(
		[=]
		{
			UAssetManager* Manager = UAssetManager::GetIfValid();
	
			UUnitAsset* Asset = Cast<UUnitAsset>(Manager->GetPrimaryAssetObject(Avatar.m_EntityAsset));
			
			UMyGameInstance::Get->GetPlayerPawn()->SetEntity(Asset);		
		}
		));
}
