// Copyright 2020 JWL, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateBrush.h"
#include "Styling/SlateStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"

/**
 * 
 */

struct SSimpleTexture
{
	int32 Width;
	int32 Height;
	TArray<FColor> Colors;

public:
	static SSimpleTexture init(int32 width, int32 height,FColor color = FColor(255, 255, 255, 255));
	static SSimpleTexture init(UTexture2D* texture, bool removeFromRoot = true);
	static SSimpleTexture init(UTextureRenderTarget2D* texture);
	SSimpleTexture copy();	
	SSimpleTexture rotate90(uint8 count = 1);
	SSimpleTexture& flipAlpha();	
	SSimpleTexture& getMask(FColor color);
	SSimpleTexture& multiple(FColor color);
	SSimpleTexture& multiple(const SSimpleTexture& brush);
	SSimpleTexture& add(const SSimpleTexture& brush, int32 posX = 0, int32 posY = 0);
	SSimpleTexture& scale(int32 destWidth, int32 destHeight);
	UTexture2D* createTexture();
	void exportToFile(const FString& directory, const FString& textureName);
};
