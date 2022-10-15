// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 골드 다이아, 보석 정령석 등,
 * 골드만 빅인트를 지원해야하니까 골드는 여기 못쓸듯?
 * 숫자 자원인데 가상화폐를 지원하고 빅인트 지원은 안하는 자원들을
 * 스트링 키값으로 관리
 */
class IDLERPG_API ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();
};
