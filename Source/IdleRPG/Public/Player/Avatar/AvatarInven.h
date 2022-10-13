// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvatarData.h"
#include "Actors/PreviewActor.h"
#include "Engine/StreamableManager.h"
#include "Preview/PreviewInven.h"


class IDLERPG_API AvatarInven : public PreviewInven
{
public:
	AvatarInven();
	
	virtual  ~AvatarInven() override;

protected:
	FName m_EquippedAvatar;

	FName m_EquipSkinAvatar;

	const FAvatarRow* m_RowEquipAvatar;

	const FAvatarRow* m_RowSkinAvatar;
	
protected:
	void SetEquip(const FName& key);

	void SetSkin(const FName& key);

public:
	void EquipAvatar();

	void EquipSkinAvatar();

	void UpdateEquipAvatar();

	void InitEquipAvatar();

public:
	FORCEINLINE const FName& GetKeyEquip() const
	{
		return  m_EquippedAvatar;
	}
	FORCEINLINE const FName& GetKeySkin() const
	{
		return  m_EquipSkinAvatar;
	}

	FORCEINLINE const FAvatarRow& GetRowEquip() const
	{
		return  *m_RowEquipAvatar;
	}
	FORCEINLINE const FAvatarRow& GetRowSkin() const
	{
		return  *m_RowSkinAvatar;
	}
};
