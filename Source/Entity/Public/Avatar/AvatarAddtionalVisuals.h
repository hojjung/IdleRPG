// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCapeComponent.h"
#include "UObject/NoExportTypes.h"
#include "AvatarAddtionalVisuals.generated.h"

/**
 * 
 */
UCLASS()
class ENTITY_API UAvatarAddtionalVisuals : public UObject
{
	GENERATED_BODY()
protected:
	TWeakObjectPtr<USkeletalMeshComponent> m_Owner;
	
	UPROPERTY(EditAnywhere)
	TArray<UMyCapeComponent*> m_AryCapes;

	float m_fAttackDur;
	
public:
	void Init(USkeletalMeshComponent* owner);

	void Hide();

	void SpawnAttachment(FName id, UStaticMesh* mesh);
	
	void TickWind(bool is_moving, float deltaTime);

	void SetAttacking(float dur);
};
