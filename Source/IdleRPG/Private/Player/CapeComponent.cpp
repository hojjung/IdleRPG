// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CapeComponent.h"

void UCapeComponent::BeginPlay()
{
	Super::BeginPlay();

	m_NameWindParam = TEXT("VertexPushStrength");
	
	m_MatInst=UMaterialInstanceDynamic::Create(GetMaterial(0), this);

	SetMaterial(0, m_MatInst);
}

void UCapeComponent::SetWindPower(float v)
{
	if(FMath::IsNaN(v))
	{
		m_MatInst->SetScalarParameterValue(m_NameWindParam,0);
		
		return;
	}
	m_MatInst->SetScalarParameterValue(m_NameWindParam,v);
}