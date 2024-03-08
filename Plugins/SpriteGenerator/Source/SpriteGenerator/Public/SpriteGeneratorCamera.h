// Copyright 2020 JWL, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "Engine/Texture2D.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "SpriteGeneratorHelper.h"
//#include "Engine/SceneCapture.h"
#include "Engine/Classes/Components/SceneCaptureComponent2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "SpriteGeneratorCamera.generated.h"

/* Status of the sprite generator */
enum class ESpriteGeneratorStatus : uint8
{
	Idle,
	ByFrame,
	ByPeriodTime
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEventWithoutParams);

/**
 * Generate sprites
 */
UCLASS(BlueprintType, Blueprintable, AutoExpandCategories = (SpriteGenerator), hidecategories=(CameraActor, DecalActor, Rendering, Replication, Input, LOD, Cooking))
class SPRITEGENERATOR_API ASpriteGeneratorCamera : public ASceneCapture2D
{
	GENERATED_UCLASS_BODY()

private:
	/* It's only used for camera view in viewport purpose. */
	UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	//UPROPERTY(Category = CameraActor, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//class USceneComponent* SceneComponent;
public:
	/* Count of cols in  sprite texture */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteGenerator)
	uint8 Columns = 8;

	/* Count of rows in  sprite texture */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteGenerator)
	uint8 Rows = 8;	

	/* Width of the final flipbook */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteGenerator)
	int32 Width = 1024;

	/* Height of the final flipbook */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteGenerator)
	int32 Height = 1024;

	/* Current sprite cell */
	UPROPERTY(BlueprintReadOnly, Category = SpriteGenerator)
	int32 CurrentSpriteFrame = 0;
	

	/* Flipbook render texture. */
	UPROPERTY(BlueprintReadOnly, Category = SpriteGenerator)
	UTextureRenderTarget2D* FlipbookCanvasTexture = nullptr;

	/* Used by a SceneCapture */
	UPROPERTY(BlueprintReadOnly, Category = SpriteGenerator)
	UTextureRenderTarget2D* SpriteFrameCanvasTexture = nullptr;

	/** EXPOSE USceneCaptureComponent2D PARAMETERS **/
	

	/** The only actors to be rendered by this scene capture, if PrimitiveRenderMode is set to UseShowOnlyList.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = SpriteCamera)
	TArray<AActor*> ShowOnlyActors;

	/** The actors to hide in the scene capture. */
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = SpriteCamera)
	TArray<AActor*> HiddenActors;

	/** Controls what primitives get rendered into the scene capture. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteCamera)
	ESceneCapturePrimitiveRenderMode RenderMode = ESceneCapturePrimitiveRenderMode::PRM_RenderScenePrimitives;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteCamera, meta = (DisplayName = "Projection Type"))
	TEnumAsByte<ECameraProjectionMode::Type> ProjectionType = ECameraProjectionMode::Perspective;

	/** Camera field of view (in degrees). */
	UPROPERTY(interp, EditAnywhere, BlueprintReadWrite, Category = SpriteCamera, meta = (DisplayName = "Field of View", UIMin = "5.0", UIMax = "170", ClampMin = "0.001", ClampMax = "360.0"))
	float FOVAngle = 90;

	/** The desired width (in world units) of the orthographic view (ignored in Perspective mode) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpriteCamera)
	float OrthoWidth = 256;
	
	/** END EXPOSE USceneCaptureComponent2D PARAMETERS **/	

	/**
	 * Creates an empty flipbook texture.
	 *
	 * @param imageWidth		Width of a sprite texture
	 * @param imageHeight		Height of a sprite texture
	 * @param startSpriteFrame	Start position in a sprite texture
	 *
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void setFlipbookEmpty();

	/**
	 * Creates a flopbook texture by the given texture (make copy of given texture).
	 *
	 * @param initialSpriteTexture	Texture
	 * @param startSpriteFrame		Start position in a sprite texture
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void setFlipbookTexture(UTexture2D* initialSpriteTexture, int32 startSpriteFrame = 0);


	/**
	 * Generate a sprite frame to the flipbook.
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void takeSpriteFrame();

	/**
	 * Generate a sprite frame every game frame.
	 *
	 * @param maxFPS			framerate
	 * @param spriteFrameLimit	limit of sprit frames taken, -1 - no limit
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void takeSpriteFrameEveryFrame(int32 maxFPS = 24, int32 spriteFrameLimit = -1);

	/**
	 * Generate a sprite frame every period time. 
	 *
	 * @param periodTimeInSec	period time in seconds
	 * @param spriteFrameLimit	limit sprits taken, -1 - no limit
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void takeSpriteFrameEveryPeriodTime(float periodTimeInSec, int32 spriteFrameLimit = -1);

	/**
	 * Pause of generating sprite frames every frame/period time.
	 */
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void pause();

	/**
	* Resume of generating sprite frames every frame/period time.
	*/
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void resume();

	/**
	* Stop of generating sprite frames every frame/period time.
	*/
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void stop();

	/**
	* Generate a flipbook texture.
	*/
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual UTexture2D* generateTexture();

	/**
	* Generate a *.PNG flipbook file.
	*/
	UFUNCTION(BlueprintCallable, Category = SpriteGenerator)
	virtual void generatePNGFile(FString directory, FString fileName);

	/**
	* Event called when the flipbook texture is full. 
	In this situation you should export a current flipbook (generateTexture|generatePNGFile) 
	and call (setFlipbookEmpty|setFlipbookTexture) to create new one.
	*/
	UPROPERTY(BlueprintAssignable, Category = SpriteGenerator)
	FEventWithoutParams OnFlipbookIsFull;

	/**
	* Event called when (takeSpriteFrameEveryFrame|takeSpriteFrameEveryPeriodTime) achieved limit
	*/
	UPROPERTY(BlueprintAssignable, Category = SpriteGenerator)
	FEventWithoutParams OnFinished;

	/**
	* Event called when a sprite frame is generated
	*/
	UPROPERTY(BlueprintAssignable, Category = SpriteGenerator)
	FEventWithoutParams OnSpriteFrameTaken;

	//~ Begin AActor Interface
	void BeginPlay() override;
	void Tick(float DeltaSeconds) override;	
	void PostInitializeComponents() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
	//~ End AActor Interface.		
protected:	
	/* Initialize a sprite frame render texture */
	virtual void initSpriteFrameTexture();
	/* Initialize a flipbook texture and prepare to draw */
	virtual void initFlipbookTexture();	
	/* End draw of flipbook and release his */
	virtual void releaseFlipbookTexture();
	/* Load needed material. Call only in constructor */
	virtual void loadMaterial();
	
	/* Used to generate a texture or a png file in thread */
	FGraphEventRef RunLambdaOnGameThread(TFunction< void()> InFunction);
	
	/* Material and dynamic instance of material, used to flip alpha channel*/
	UPROPERTY()
	UMaterial* Material;
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialDynamicOneMinus = nullptr;

	/* Context and canvas of fliobook texture, used to draw on flipbook */
	FDrawToRenderTargetContext FlipbookContext;
	UCanvas* FlipbookCanvas = nullptr;	
	
	/* Size of the cell sprite */
	int32 CellWidth;
	int32 CellHeight;

	/* Used to check if sprite frame limit is achieved in (takeSpriteFrameEveryFrame|takeSpriteFrameEveryPeriodTime) */
	int32 loopLimitPhoto;
	int32 loopCurrentPhoto;

	/* Used for period time loop */
	float currentTime;
	float periodTime;	

	/* Check if a flipbook texture is set */
	bool isSetTexture = false;

	/* Check if loop is paused */
	bool isPaused = false;

	/* Type of processing of sprite frame capturing */
	ESpriteGeneratorStatus Status = ESpriteGeneratorStatus::Idle;
};
