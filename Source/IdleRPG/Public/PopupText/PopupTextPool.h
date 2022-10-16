// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupTextWComp.h"
#include "UObject/StrongObjectPtr.h"

/**
 * 
 */
class IDLERPG_API PopupTextPool
{
public:
	PopupTextPool(int count, USceneComponent* root);
	~PopupTextPool();
	
protected:
	TArray<TStrongObjectPtr<UPopupTextWComp>> m_AryWidgets;

	int m_CompUseIndex;
	
public:
	void ShowInGameWorldText(const FString& stringWant, FVector loc, int index);

	void ShowInGameWorldText(float number, FVector loc, int index);
	
protected:
	UPopupTextWComp* GetIGWC();
	
};
