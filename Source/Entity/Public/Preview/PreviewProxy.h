// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PreviewProxy.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPreviewProxy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTITY_API IPreviewProxy
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetIsTouched(bool b) = 0;

	virtual void RotatePawn(float deltaX) = 0;
};
