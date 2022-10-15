// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 모든 상품 획득은 이 매니저를 통해서 전달
 * 아이디를 분석해 올바르게 획득 시켜주거나
 * 위젯으로 보상을 띄워줌
 */
class IDLERPG_API PrizeReceiveManager
{
public:
	PrizeReceiveManager();
	~PrizeReceiveManager();
};
