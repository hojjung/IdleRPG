// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PetInven.h"

/**
 * 
 */
class IDLERPG_API PetManager
{
public:
	PetManager();
	~PetManager();

public:
	PetInven* m_PetInven;
};
