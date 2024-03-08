// Copyright 2020 JWL, Inc. All Rights Reserved.

#include "SpriteGeneratorCamera.h"
#include "UObject/ConstructorHelpers.h"
#include "CanvasTypes.h"
#include "Engine/Canvas.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/World.h"
#include "Engine/Classes/Camera/CameraComponent.h"
#include "UnrealEngine.h"

ASpriteGeneratorCamera::ASpriteGeneratorCamera(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	loadMaterial();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;

	//SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));

	// Make the scene component the root component
	//RootComponent = SceneComponent;

	// Setup camera defaults
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	//CameraComponent->RelativeLocation = FVector(0);
	CameraComponent->FieldOfView = FOVAngle;
	CameraComponent->bConstrainAspectRatio = true;
	CameraComponent->AspectRatio = 1.0f;
	CameraComponent->OrthoWidth = OrthoWidth;
	CameraComponent->ProjectionMode = ProjectionType;
	CameraComponent->PostProcessBlendWeight = 1.0f;

	//CameraComponent->SetupAttachment(GetMeshComp());
	CameraComponent->SetupAttachment(RootComponent);
	
	//CameraComponent->SetupAttachment(SceneComponent);

	USceneCaptureComponent2D* _capture = GetCaptureComponent2D();
	if (_capture)
	{
		//_capture->RelativeLocation = FVector(0);
		_capture->OrthoWidth = OrthoWidth;
		_capture->FOVAngle = FOVAngle;
		_capture->ProjectionType = ProjectionType;
	}

	//_capture->SetupAttachment(SceneComponent);
}

void ASpriteGeneratorCamera::BeginPlay()
{
	Super::BeginPlay();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = true;
}

void ASpriteGeneratorCamera::setFlipbookEmpty()
{	
	// Release data if they aren't released
	if(isSetTexture)
		releaseFlipbookTexture();

	// Set flipbook parameters
	CurrentSpriteFrame = 0;

	// Initialize flipbook render texture
	initFlipbookTexture();	
	isSetTexture = true;

	// Set capture component
	CellWidth = Width / Columns;
	CellHeight = Height / Rows;
}

void ASpriteGeneratorCamera::setFlipbookTexture(UTexture2D* spriteTexture, int32 startSpriteFrame)
{
	// Validation
	check(spriteTexture != nullptr);
	check(startSpriteFrame >= 0);
	check(startSpriteFrame < Columns * Rows);

	// Release data if they aren't released
	if (isSetTexture)
		releaseFlipbookTexture();

	// Set flipbook parameters	
	CurrentSpriteFrame = startSpriteFrame;

	// Initialize flipbook render texture
	initFlipbookTexture();
	isSetTexture = true;

	// Draw given texture on flipbook render texture
	if (FlipbookCanvas)
	{
		FlipbookCanvas->K2_DrawTexture(
			spriteTexture,
			FVector2D(0,0),
			FVector2D(Width, Height),
			FVector2D(0, 0));

		FlipbookCanvas->Canvas->Flush_GameThread();
	}
	
	// Calc sprite frame size
	CellWidth = Width / Columns;
	CellHeight = Height / Rows;	
}

void ASpriteGeneratorCamera::loadMaterial()
{	
	// Load material (call only in constructor)
	static ConstructorHelpers::FObjectFinder<UMaterial> _m_OneMinus(TEXT("Material'/SpriteGenerator/M_flipAlpha.M_FlipAlpha'"));
	if (_m_OneMinus.Object)
	{
		Material = _m_OneMinus.Object;			
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find the Material'/SpriteGenerator/M_flipAlpha.M_FlipAlpha' material in the plugin folder."))
	}	
}

void ASpriteGeneratorCamera::initFlipbookTexture()
{
	// Create target texture to generate flipbook
	FlipbookCanvasTexture = NewObject<UTextureRenderTarget2D>();
	FlipbookCanvasTexture->Filter = TextureFilter::TF_Default;
	FlipbookCanvasTexture->ClearColor = FLinearColor(0,0,0,1);
	FlipbookCanvasTexture->SRGB = 1;
	FlipbookCanvasTexture->TargetGamma = 1;
	FlipbookCanvasTexture->InitCustomFormat(Width, Height, PF_FloatRGBA, 1);
	FlipbookCanvasTexture->UpdateResourceImmediate(true);
	
	// Begin draw on flipbook texture
	FVector2D _size;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(this, FlipbookCanvasTexture, FlipbookCanvas, _size, FlipbookContext);	
}

void ASpriteGeneratorCamera::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	initSpriteFrameTexture();

	// Create dynamic material to recalc alpha channel of sprite frame
	if (!MaterialDynamicOneMinus)
	{
		MaterialDynamicOneMinus = UMaterialInstanceDynamic::Create(Material, this);
	}

	check(MaterialDynamicOneMinus != nullptr);

	USceneCaptureComponent2D* _capturer = GetCaptureComponent2D();
	MaterialDynamicOneMinus->SetTextureParameterValue(FName(TEXT("input")), _capturer->TextureTarget);
}

void ASpriteGeneratorCamera::initSpriteFrameTexture()
{			
	// Set capture component
	CellWidth = Width / Columns;
	CellHeight = Height / Rows;

	// Create texture for capture sprite frames
	SpriteFrameCanvasTexture = NewObject<UTextureRenderTarget2D>();
	SpriteFrameCanvasTexture->Filter = TextureFilter::TF_Default;
	SpriteFrameCanvasTexture->ClearColor = FLinearColor(0, 0, 0, 0);
	SpriteFrameCanvasTexture->SRGB = 1;
	SpriteFrameCanvasTexture->TargetGamma = 1;
	SpriteFrameCanvasTexture->InitCustomFormat(CellWidth, CellHeight, PF_FloatRGBA, 1);
	SpriteFrameCanvasTexture->UpdateResourceImmediate(true);


	// Set parameters in SceneCaptureComponent
	USceneCaptureComponent2D* _capturer = GetCaptureComponent2D();
	_capturer->TextureTarget = SpriteFrameCanvasTexture;
	_capturer->CaptureSource = ESceneCaptureSource::SCS_SceneColorHDR;
	_capturer->CompositeMode = ESceneCaptureCompositeMode::SCCM_Overwrite;
	_capturer->bCaptureEveryFrame = true;
}

void ASpriteGeneratorCamera::takeSpriteFrame()
{
	// Validation
	if (CurrentSpriteFrame >= Columns * Rows)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpriteGenerator: Texture is full, please call generateTexture/generatePNGFile to take texture and then call setEmptyTexture/setExistTexture to create new one"));
		return;
	}

	if (isSetTexture == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpriteGenerator: Please set texture by call setEmptyTexture/setExistTexture"));
		return;
	}
	
	// Calculate the current frame position
	int32 _rowIndex = CurrentSpriteFrame / Columns;
	int32 _collIndex = CurrentSpriteFrame % Columns;
		
	check(FlipbookCanvas != nullptr);
	if (!MaterialDynamicOneMinus)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpriteGenerator: No MaterialDynamicOneMinus!"));
	}
	else
	{
		FlipbookCanvas->K2_DrawMaterial(
			MaterialDynamicOneMinus, 
			FVector2D(_collIndex * CellWidth, _rowIndex * CellHeight),
			FVector2D(CellWidth, CellHeight),
			FVector2D(0,0));
	}
	// Draw sprite frame on flipbook render texture
		
	FlipbookCanvas->Canvas->Flush_GameThread();		

	// Send event "OnSpriteFrameTaken"
	if(OnSpriteFrameTaken.IsBound())
		OnSpriteFrameTaken.Broadcast();

	// Increase sprite frame number
	CurrentSpriteFrame++;

	// If it is last sprite frame on flipbook then send event "OnTextureIsFull"
	if (CurrentSpriteFrame >= Columns * Rows)
		if(OnFlipbookIsFull.IsBound())
			OnFlipbookIsFull.Broadcast();		
}


void ASpriteGeneratorCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// Only for loop processing likes "every frame"/"every period time" 
	if (!isPaused && (Status == ESpriteGeneratorStatus::ByFrame || Status == ESpriteGeneratorStatus::ByPeriodTime))
	{
		// Check if is achieved limit of sprite frames 
		if (loopLimitPhoto > 0 && loopCurrentPhoto >= loopLimitPhoto)
		{
			stop();
			if(OnFinished.IsBound())
				OnFinished.Broadcast();
			return;
		}

		// Every frame
		if (Status == ESpriteGeneratorStatus::ByFrame)
		{
			takeSpriteFrame();
			loopCurrentPhoto++;
		}
		// Every period time 
		else if (Status == ESpriteGeneratorStatus::ByPeriodTime)
		{
			// It is a little smart solution, if some wants to have a sprite frame every 0.01s in 25FPS (that means a game frame is every 0.04s)
			// this loop will create four of the same sprite frames in one game frame
			currentTime += DeltaSeconds;
			while (currentTime >= periodTime)
			{
				takeSpriteFrame();
				currentTime -= periodTime;
				loopCurrentPhoto++;

				if (loopLimitPhoto > 0 && loopCurrentPhoto >= loopLimitPhoto)
					break;
			}
		}

		// Check
		if (loopLimitPhoto > 0 && loopCurrentPhoto >= loopLimitPhoto)
		{
			stop();
			if (OnFinished.IsBound())
				OnFinished.Broadcast();
			return;
		}
	}
}

void ASpriteGeneratorCamera::takeSpriteFrameEveryFrame(int32 maxFPS, int32 spriteLimit)
{
	// Set loop parameters
	Status				= ESpriteGeneratorStatus::ByFrame;
	loopCurrentPhoto	= 0;
	loopLimitPhoto		= spriteLimit;
	currentTime			= 0;
	periodTime			= 1;
	isPaused = false;

	// Change FPS
	GEngine->Exec(GetWorld(), *FString::Format(TEXT("t.MaxFPS {0}"), { maxFPS }));		
}

void ASpriteGeneratorCamera::takeSpriteFrameEveryPeriodTime(float periodTimeInSec, int32 spriteLimit)
{
	check(periodTimeInSec > 0);

	// Set loop parameters
	Status = ESpriteGeneratorStatus::ByPeriodTime;
	loopCurrentPhoto	= 0;
	loopLimitPhoto		= spriteLimit;
	currentTime			= 0;
	periodTime			= periodTimeInSec;
	isPaused			= false;
}

void ASpriteGeneratorCamera::pause()
{
	isPaused = true;
}

void ASpriteGeneratorCamera::resume()
{
	isPaused = false;
}

void ASpriteGeneratorCamera::stop()
{
	// Set loop parameters
	Status				= ESpriteGeneratorStatus::Idle;
	loopCurrentPhoto	= 0;
	loopLimitPhoto		= 0;
	currentTime			= 0;
	periodTime			= 0;	

	// Change FPS
	GEngine->Exec(GetWorld(), *FString::Format(TEXT("t.MaxFPS {0}"), { 0 }));
}

void ASpriteGeneratorCamera::releaseFlipbookTexture()
{
	isSetTexture = false;
	if (FlipbookCanvas)
	{
		FlipbookCanvas->Canvas = NULL;
		FlipbookCanvas = nullptr;
	}
	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, FlipbookContext);
}

UTexture2D* ASpriteGeneratorCamera::generateTexture()
{		
	return SSimpleTexture::init(FlipbookCanvasTexture)
		.flipAlpha()
		.createTexture();
}

void ASpriteGeneratorCamera::generatePNGFile(FString directory, FString fileName)
{
	SSimpleTexture::init(FlipbookCanvasTexture)
		.flipAlpha()
		.exportToFile(directory, fileName);
}

#if WITH_EDITOR
void ASpriteGeneratorCamera::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);

	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	if (   PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, ShowOnlyActors)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, HiddenActors)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, RenderMode)
		)
	{
		USceneCaptureComponent2D* _capturer = GetCaptureComponent2D();
		_capturer->ShowOnlyActors = ShowOnlyActors;
		_capturer->HiddenActors = HiddenActors;
		_capturer->PrimitiveRenderMode = RenderMode;
	}

	if (PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, ProjectionType)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, FOVAngle)
		|| PropertyName == GET_MEMBER_NAME_CHECKED(ASpriteGeneratorCamera, OrthoWidth)
		)
	{
		USceneCaptureComponent2D* _capturer = GetCaptureComponent2D();
		_capturer->ProjectionType = ProjectionType;
		_capturer->FOVAngle = FOVAngle;
		_capturer->OrthoWidth = OrthoWidth;

		if (CameraComponent)
		{
			CameraComponent->ProjectionMode = ProjectionType;
			CameraComponent->FieldOfView = FOVAngle;
			CameraComponent->OrthoWidth = OrthoWidth;
		}
	}
}
#endif

FGraphEventRef ASpriteGeneratorCamera::RunLambdaOnGameThread(TFunction< void()> InFunction)
{
	return FFunctionGraphTask::CreateAndDispatchWhenReady(InFunction, TStatId(), nullptr, ENamedThreads::GameThread);
}