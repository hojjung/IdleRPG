// Fill out your copyright notice in the Description page of Project Settings.


#include "Pet/PetManager.h"

PetManager::PetManager()
{
	m_PetInven = new PetInven();
}

PetManager::~PetManager()
{
	delete m_PetInven;
}
