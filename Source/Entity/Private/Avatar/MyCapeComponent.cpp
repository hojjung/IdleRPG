// Fill out your copyright notice in the Description page of Project Settings.


#include "Avatar/MyCapeComponent.h"

void UMyCapeComponent::Init(UStaticMesh* stm)
{
	SetStaticMesh(stm);
	
	m_NameWindParam = TEXT("VertexPushStrength");
	
	m_MatInst=UMaterialInstanceDynamic::Create(GetMaterial(0), this);

	SetMaterial(0, m_MatInst);

	SetWindPower(40);
}

void UMyCapeComponent::SetWindPower(float v)
{
	if(FMath::IsNaN(v))
	{
		m_MatInst->SetScalarParameterValue(m_NameWindParam,0);
		
		return;
	}
	m_MatInst->SetScalarParameterValue(m_NameWindParam,v);
}