// Copyright 2020 JWL, Inc. All Rights Reserved.


#include "SpriteGeneratorHelper.h"
#include "ImageUtils.h"
#include "Engine/Texture2D.h"
//#include "FileHelper.h"
#include "Misc/FileHelper.h"

SSimpleTexture SSimpleTexture::init(int32 width, int32 height, FColor color)
{
	SSimpleTexture _simple;
	_simple.Height = height;
	_simple.Width = width;
	_simple.Colors.SetNumUninitialized(_simple.Height * _simple.Width);

	for (int32 X = 0; X < _simple.Width; X++)
	{
		for (int32 Y = 0; Y < _simple.Height; Y++)
		{
			int32 curPixelIndex = ((Y * _simple.Width) + X);
			_simple.Colors[curPixelIndex] = color;
		}
	}

	return _simple;	
}

SSimpleTexture SSimpleTexture::init(UTexture2D* texture, bool removeFromRoot)
{
	SSimpleTexture _result;
	_result.Width = 0;
	_result.Height = 0;

	TextureCompressionSettings OldCompressionSettings = texture->CompressionSettings;
#if WITH_EDITORONLY_DATA
	TextureMipGenSettings OldMipGenSettings = texture->MipGenSettings;
#endif
	bool OldSRGB = texture->SRGB;    

	texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
#if WITH_EDITORONLY_DATA
	texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
#endif    
	texture->SRGB = false;
	texture->UpdateResource();

	const FColor* FormatedImageData = reinterpret_cast<const FColor*>(texture->PlatformData->Mips[0].BulkData.LockReadOnly());


	_result.Height = texture->GetSizeY();
	_result.Width = texture->GetSizeX();
	_result.Colors.SetNumUninitialized(_result.Height * _result.Width);

	for (int32 X = 0; X < texture->GetSizeX(); X++)
	{
		for (int32 Y = 0; Y < texture->GetSizeY(); Y++)
		{
			int32 curPixelIndex = ((Y * _result.Width) + X);
			_result.Colors[curPixelIndex] = FormatedImageData[curPixelIndex];
		}
	}

	texture->PlatformData->Mips[0].BulkData.Unlock();

	texture->CompressionSettings = OldCompressionSettings;
#if WITH_EDITORONLY_DATA
	texture->MipGenSettings = OldMipGenSettings;
#endif
	texture->SRGB = OldSRGB;
	texture->UpdateResource();

	if(removeFromRoot)
		texture->RemoveFromRoot();

	return _result;
}

SSimpleTexture SSimpleTexture::init(UTextureRenderTarget2D* texture)
{
	SSimpleTexture _result;
	_result.Width = 0;
	_result.Height = 0;

	if (texture)
	{
		FTextureRenderTargetResource* render = texture->GameThread_GetRenderTargetResource();

		const EPixelFormat PixelFormat = texture->GetFormat();
		ETextureSourceFormat TextureFormat = TSF_Invalid;
		switch (PixelFormat)
		{
		case PF_B8G8R8A8:
			TextureFormat = TSF_BGRA8;
			break;
		case PF_FloatRGBA:
			TextureFormat = TSF_RGBA16F;
			break;
		}


		if (render == nullptr || TextureFormat == TSF_Invalid)
		{
			return _result;
		}

		FColor background = FColor::Black;

		_result.Height = render->GetSizeY();
		_result.Width = render->GetSizeX();
		_result.Colors.SetNumUninitialized(_result.Height*_result.Width);

		int32 width = _result.Width;
		int32 height = _result.Height;

		if (TextureFormat == TSF_RGBA16F)
		{
			TArray<FFloat16Color> SurfData;
			render->ReadFloat16Pixels(SurfData);

			TArray<FLinearColor> SurfData2;
			render->ReadLinearColorPixels(SurfData2);

			for (int32 y = 0; y < height; y++)
			{
				for (int32 x = 0; x < width; x++)
				{
					int32 curPixelIndex = ((y * width) + x);
					
					_result.Colors[curPixelIndex] = FLinearColor(SurfData[curPixelIndex]).ToFColor(true);
				}
			}
		}
		else if (TextureFormat == TSF_BGRA8)
		{
			render->ReadPixels(_result.Colors);
		}
	}
	
	return _result;
}

SSimpleTexture& SSimpleTexture::getMask(FColor color)
{	
	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{
			int32 curPixelIndex = ((Y * Width) + X);
			if (Colors[curPixelIndex] == color)
			{
				Colors[curPixelIndex] = FColor::White;
			}
			else
			{
				Colors[curPixelIndex] = FColor::Black.WithAlpha(0);
			}

		}
	}	

	return *this;
}

SSimpleTexture& SSimpleTexture::flipAlpha()
{
	int32 _count = Colors.Num();
	for (int32 _index = 0; _index < _count; _index++)
	{
		Colors[_index].A = 255 - Colors[_index].A;
	}	

	return *this;
}

SSimpleTexture& SSimpleTexture::multiple(const SSimpleTexture& brush)
{
	FColor _color = FColor::White;

	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{
			int32 curPixelIndex = ((Y * Width) + X);

			if(X< brush.Width && Y<brush.Height)
			{
				int32 curBrushPixelIndex = ((Y * brush.Width) + X);
				_color = brush.Colors[curBrushPixelIndex];
			}
			else
				_color = FColor::White;				

			FColor _source = Colors[curPixelIndex];
			float R = ((float)_source.R * (float)_color.R) / 255.0;
			float G = ((float)_source.G * (float)_color.G) / 255.0;
			float B = ((float)_source.B * (float)_color.B) / 255.0;
			float A = ((float)_source.A * (float)_color.A) / 255.0;


			Colors[curPixelIndex] = FColor(
				(uint8)FMath::RoundToInt(R),
				(uint8)FMath::RoundToInt(G),
				(uint8)FMath::RoundToInt(B),
				(uint8)FMath::RoundToInt(A)
			);

			/*
			FLinearColor _col1 = FLinearColor(Colors[curPixelIndex]);
			FLinearColor _col2 = FLinearColor(color);

			Colors[curPixelIndex] = (_col1 * _col2).ToFColor(true);*/
		}
	}

	return *this;
}

SSimpleTexture& SSimpleTexture::multiple(FColor color)
{
	for (int32 X = 0; X < Width; X++)
	{
		for (int32 Y = 0; Y < Height; Y++)
		{
			int32 curPixelIndex = ((Y * Width) + X);

			
			FColor _source = Colors[curPixelIndex];
			float R = ((float)_source.R * (float)color.R) / 255.0;
			float G = ((float)_source.G * (float)color.G) / 255.0;
			float B = ((float)_source.B * (float)color.B) / 255.0;
			float A = ((float)_source.A * (float)color.A) / 255.0;
			

			Colors[curPixelIndex] = FColor(
				(uint8)FMath::RoundToInt(R),
				(uint8)FMath::RoundToInt(G),
				(uint8)FMath::RoundToInt(B),
				(uint8)FMath::RoundToInt(A)
			);

			/*
			FLinearColor _col1 = FLinearColor(Colors[curPixelIndex]);
			FLinearColor _col2 = FLinearColor(color);						

			Colors[curPixelIndex] = (_col1 * _col2).ToFColor(true);*/
		}
	}

	return *this;
}

SSimpleTexture& SSimpleTexture::add(const SSimpleTexture& brush, int32 posX, int32 posY)
{	
	int32 limitX = FMath::Min<int32>(Width,posX+brush.Width);
	int32 limitY = FMath::Min<int32>(Height, posY + brush.Height);

	for (int32 X = posX; X < limitX; X++)
	{
		for (int32 Y = posY; Y < limitY; Y++)
		{
			int32 _canvasPixelIndex = ((Y * Width) + X);

			FColor _res = Colors[_canvasPixelIndex];
			if (Y - posY >= 0
				&& Y - posY < brush.Height
				&& X - posX >= 0
				&& X - posX < brush.Width)
			{
				int32 _index = ((Y - posY) * brush.Width) + X - posX;
				FColor _brush = brush.Colors[_index];

				if (_brush.A > 0)
				{

					if(_res.A >0 )
					{

					//float _A = ((float)_brush.A + 255 - (float)_res.A) / 255.0;

						float _A = ((float)_brush.A + 255 - (float)_res.A) / 255.0;
					if(_A>1)
						_A = 1;

					FVector _vres	= FVector(_res.R,_res.G,_res.B);
					FVector _vbrush	= FVector(_brush.R, _brush.G, _brush.B);

					FVector _vresult = FMath::Lerp(_vres, _vbrush, _A);

					_res = FColor(
						_vresult.X, 
						_vresult.Y, 
						_vresult.Z,
						FMath::Max<uint8>(_brush.A, _res.A)
					);
					//_res.A = FMath::Max<uint8>(_brush.A, _res.A);
						/*
					float R = ((float)_res.R * (float)color.R) / 255.0;
					float G = ((float)_res.G * (float)color.G) / 255.0;
					float B = ((float)_res.B * (float)color.B) / 255.0;
					float A = ((float)_res.A * (float)color.A) / 255.0;*/


					/*
					FLinearColor _licurr = FLinearColor(_res);
					FLinearColor _librus = FLinearColor(_brush);

					float _A = ((float)_brush.A)/255;

					FLinearColor _lires = _A * _librus + (1 - _A) * _licurr;
					_res = _lires.ToFColor(true);
					*/
					}
					else
					{
						_res = _brush;
					}
				}
			}

			Colors[_canvasPixelIndex] = _res;
		}
	}
	
	return *this;
}


SSimpleTexture SSimpleTexture::copy()
{
	SSimpleTexture _texture = init(Width,Height);
	int32 _size = Colors.Num();
	for (int32 _index = 0; _index < _size; _index++)
	{
		_texture.Colors[_index] = Colors[_index];
	}

	return _texture;
}

SSimpleTexture SSimpleTexture::rotate90(uint8 count)
{
	SSimpleTexture _res;

	count = count % 4;

	if(count == 0 || count == 2)
		_res  = init(Width,Height);
	else 
		_res = init(Height,Width);

	for(int32 _x = 0; _x < Width; _x++)
		for (int32 _y = 0; _y < Height; _y++)
		{
			int32 _sourcePos = _x + _y* Width;
			int32 _targetPos = _sourcePos;
			switch (count)
			{
			case 0:break;
			case 1: 
				_targetPos = _x * Height + (Height - _y - 1);		
				break;
			case 2:
				_targetPos = (Width-_x-1) + (Height - _y -1) * Width;
				break;
			case 3:
				_targetPos = (Width - _x -1) * Height + _y;
				break;
			}

			_res.Colors[_targetPos] = Colors[_sourcePos];
		}

	return _res;
}

SSimpleTexture& SSimpleTexture::scale(int32 destWidth, int32 destHeight)
{
	SSimpleTexture _alphaTexture = SSimpleTexture::init(Width, Height, FColor(1, 1, 1, 1));
	for (int32 _index = 0; _index < Colors.Num(); _index++)
	{
		uint8 _alpha = Colors[_index].A;
		_alphaTexture.Colors[_index] = FColor(_alpha, _alpha, _alpha, 1);
	}

	TArray<FColor> _alphaOutput;
	FImageUtils::ImageResize(_alphaTexture.Width, _alphaTexture.Height, _alphaTexture.Colors, destWidth, destHeight, _alphaOutput, true);

	TArray<FColor> _output;
	FImageUtils::ImageResize(Width, Height, Colors, destWidth, destHeight, _output, true);
	
	Width = destWidth;
	Height = destHeight;
	Colors.Empty();
	Colors.Append(_output);

	//UE_LOG(LogTemp,Warning,TEXT("War %d %d"), _simple.Colors.Num(), _alphaOutput.Num())

	for (int32 _index = 0; _index < _alphaOutput.Num(); _index++)
	{
		FColor _dest = FColor(
			Colors[_index].R,
			Colors[_index].G,
			Colors[_index].B,
			_alphaOutput[_index].R);
		Colors[_index] = _dest;
	}

	return *this;
}


UTexture2D* SSimpleTexture::createTexture()
{
	UTexture2D* _texture = UTexture2D::CreateTransient(Width, Height);

	_texture->AddToRoot();
	_texture->PlatformData = new FTexturePlatformData();
	_texture->PlatformData->SizeX = Width;
	_texture->PlatformData->SizeY = Height;
	//_texture->PlatformData->SetNumSlices(1);
	_texture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;

	int32 _bytesPerPixel = 4; // r g b a	
	int32 _bufferSize = Width * Height * _bytesPerPixel;
	uint8* _pixels = new uint8[_bufferSize];

	for (int32 y = 0; y < Height; y++)
	{
		for (int32 x = 0; x < Width; x++)
		{

			int32 _curPixelIndex = ((y * Width) + x);
			_pixels[4 * _curPixelIndex + 0] = Colors[_curPixelIndex].B;
			_pixels[4 * _curPixelIndex + 1] = Colors[_curPixelIndex].G;
			_pixels[4 * _curPixelIndex + 2] = Colors[_curPixelIndex].R;
			_pixels[4 * _curPixelIndex + 3] = Colors[_curPixelIndex].A;
		}
	}

    FTexture2DMipMap* _mip = new FTexture2DMipMap();    
    _texture->PlatformData->Mips.Add(_mip);
	
	_mip->SizeX = Width;
	_mip->SizeY = Height;
	_mip->BulkData.Lock(LOCK_READ_WRITE);
	uint8* _textureData = (uint8*)_mip->BulkData.Realloc(_bufferSize);
	FMemory::Memcpy(_textureData, _pixels, sizeof(uint8)* _bufferSize);
	_mip->BulkData.Unlock();

#if WITH_EDITORONLY_DATA
	_texture->Source.Init(Width, Height, 1, 1, ETextureSourceFormat::TSF_BGRA8, _pixels);
#endif
	_texture->UpdateResource();

	delete[] _pixels;

	return _texture;
}


void SSimpleTexture::exportToFile(const FString& directory, const FString& textureName)
{
	FString fullPath = directory + "/" + textureName;

	if (!textureName.EndsWith(TEXT(".png")))
	{
		fullPath += TEXT(".png");
	}

	TArray<uint8> CompressedBitmap;
	FImageUtils::CompressImageArray(Width, Height, Colors, CompressedBitmap);
	FFileHelper::SaveArrayToFile(CompressedBitmap, *fullPath);
}