// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameLevel/MainMenu/Map/WidgetMapEle.h"

#include "Entity.h"
#include "Components/CanvasPanelSlot.h"
#include "ConquerMap/ContentData.h"

void UWidgetMapEle::NativePreConstruct()
{
	Super::NativePreConstruct();

	UpdateIcon();
}
void UWidgetMapEle::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	m_BtnIcon->OnClicked.AddDynamic(this, &UWidgetMapEle::OnClick);
	
	UpdateIcon();
}

void UWidgetMapEle::UpdateIcon()
{
	const FContentDataRow* ZoneData = UContentData::GetContentData->FindRow<FContentDataRow>(m_ZoneID, "");

	if(!ZoneData)
	{
		PRINTF("UWidgetMapEle::UpdateIcon, No Data, ID Name is :%s", *m_ZoneID.ToString());
		return;	
	}
	FButtonStyle Style;
	UTexture2D* t = ZoneData->m_Icon;
	Style.Normal.SetResourceObject(t);
	Style.Hovered.SetResourceObject(t);
	Style.Pressed.SetResourceObject(t);
	Style.Disabled.SetResourceObject(t);
	m_BtnIcon->SetStyle(Style);

	m_TextName->SetText(ZoneData->m_Name);
}



void UWidgetMapEle::OnClick()
{
	m_OnClick.ExecuteIfBound(m_ZoneID);
}

FName UWidgetMapEle::GetZoneID()
{
	return  m_ZoneID;
}

void UWidgetMapEle::SetZoneID(FName id, float dur)
{
	m_ZoneID = id;

	UpdateIcon();//시간 제한, 시간제한이 이동시간보다 크다면, 이동 불가능임
}

FVector2D UWidgetMapEle::GetPos()
{
	return Cast<UCanvasPanelSlot>( Slot)->GetPosition();
}
