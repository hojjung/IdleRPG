// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAsset/EntityBaseAsset.h"

USkeletalMesh* UUnitEntityAsset::GetSkMesh() const
{
	return m_BodyMesh;
}

UAnimMontage* UUnitEntityAsset::GetBaseAtkAnim() const
{
	return m_BaseAttackAnim;
}
