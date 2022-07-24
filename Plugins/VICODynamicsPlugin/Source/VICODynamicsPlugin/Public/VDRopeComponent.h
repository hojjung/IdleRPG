// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "BaseVDComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"
#include "VDSimulatedObject.h"
#include "VDRopeComponent.generated.h"

class FVDRopeSceneProxy;

USTRUCT(BlueprintType)
struct FRopePiece
{
	GENERATED_BODY();

	FRopePiece() : StartParticleIndex(0), EndParticleIndex(0)
	{}
	FRopePiece(int32 StartIndex, int32 EndIndex) : StartParticleIndex(StartIndex), EndParticleIndex(EndIndex)
	{}

	bool Split(int32 SplitAtIndex, FRopePiece& Piece1, FRopePiece& Piece2)
	{
		if (SplitAtIndex <= StartParticleIndex || SplitAtIndex >= EndParticleIndex)
			return false;

		Piece1.StartParticleIndex = StartParticleIndex;
		Piece1.EndParticleIndex = SplitAtIndex;

		Piece2.StartParticleIndex = SplitAtIndex + 1;
		Piece2.EndParticleIndex = EndParticleIndex;

		return true;
	}

	bool Contains(int32 ParticleIndex) const
	{
		return ParticleIndex >= StartParticleIndex && ParticleIndex <= EndParticleIndex;
	}

	bool operator=(const FRopePiece& Other) const
	{
		return StartParticleIndex == Other.StartParticleIndex && EndParticleIndex == Other.EndParticleIndex;
	}
	bool operator<(const FRopePiece& Other) const
	{
		return StartParticleIndex < Other.EndParticleIndex;
	}

	UPROPERTY(BlueprintReadWrite, Category = "Rope")
	int32 StartParticleIndex;
	UPROPERTY(BlueprintReadWrite, Category = "Rope")
	int32 EndParticleIndex;
};

UCLASS(hidecategories = (Object, Physics, Collision, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDRopeComponent : public UBaseVDComponent
{
	GENERATED_BODY()

public:
	UVDRopeComponent(const FObjectInitializer& ObjectInitializer);
	
	// Begin UActorComponent interface.
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
	virtual void CreateRenderState_Concurrent() override;
	// End UActorComponent interface.

	// Begin USceneComponent interface.
	virtual bool HasAnySockets() const override;
	virtual FTransform GetSocketTransform(FName InSocketName, ERelativeTransformSpace TransformSpace = RTS_World) const override;
	virtual void QuerySupportedSockets(TArray<FComponentSocketDescription>& OutSockets) const override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// End USceneComponent interface.

	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual float GetMass() const override { return Mass; }
	virtual FVector GetSocketLocation(FName InSocketName) const override;
	virtual FQuat GetSocketQuaternion(FName InSocketName) const override;
	virtual FRotator GetSocketRotation(FName InSocketName) const override;
	// End UPrimitiveComponent interface.
	
	// Begin UMeshComponent interface.
	virtual int32 GetNumMaterials() const override;
	// End UMeshComponent interface.

	// Begin IVICODynamicsSimulated Interface
	virtual void Sync() override; // Signals that a simulate tick has finished and it is safe to access/modify Particle and/or Constraint data	
	virtual void Release() override; // Perform any Simulation related clean up
	// End IVICODynamicsSimulated Interface

	// Begin UBaseVDComponent interface.
	virtual void RegisterWithSimulation() override;
	virtual void UnregisterWithSimulation() override;
	virtual void ApproximateWindDrag(const FVector& Direction, float Speed) override;
	virtual void CreateTensionConstraintsFromAttachment(FVDParticleAttachment& Attachment) override;
	virtual const TArray<FVDParticleInfo>* GetParticleBuildInfo() const override;
	// End UBaseVDComponent interface.

	virtual void GetEndPositions(FVector& OutStartPosition, FVector& OutEndPosition);

	/**
	 * Creates the points that make up the rope. Used during preview and as the initial start info for the particles.
	 */
	virtual void CreateRopeGeometry(TArray<FVDParticleInfo>& RopePoints);

	/**
	 * Sends the updated particle info to the Scene Proxy.
	 */
	virtual void UpdateSceneProxy(FVDRopeSceneProxy* Proxy);

	/**
	 * Build the actual simulated rope using VICODynamics
	 */
	virtual void BuildSimulatedRope();

	/**
	 * Helper to changes the rest length of the entire rope at runtime. NewLength must be greater than 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rope")
	void SetNewRestLength(float NewLength, int32 PieceIndex = -1);

	/**
	 * Helper to changes the rest length of the entire rope at runtime. NewLength must be greater than 0.
	 */
	UFUNCTION(BlueprintCallable, Category = "Rope")
	void SetNewRestLengthStartingAtParticle(int32 ParticleIndexStart, float NewLength);
	
	/** Returns the ropes true length at runtime, accounts any stretching. */
	UFUNCTION(BlueprintCallable, Category = "Rope")
	float GetTrueLength(int32 PieceIndex = 0) const;

	UFUNCTION(BlueprintCallable, Category = "Rope")
	void BreakAtSegment(int32 SegmentIndex);

	UFUNCTION(BlueprintCallable, Category = "Rope")
	TArray<FRopePiece> GetPieces() const { return Pieces; }

	const TArray<UVDParticleSpringConstraint*>& GetStretchConstraintsArray() const { return StretchConstraints; }
	const TArray<UVDParticleSpringConstraint*>& GetBendConstraintsArray() const { return BendConstraints; }

protected:
	void ResolvePreBuildParticleAttachments();
	void CorrectConstraintsForAttachedParticles();
	void SetNewLengthsForPiece(const FRopePiece& Piece, const float StretchLength, const float BendLength);

public:
	/** How strongly the rope resists stretching. [0-1] */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StretchStiffness;

	/** How strongly the rope resists bending. [0-1] */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BendStiffness;

	/** The ropes total Mass */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Mass;
	
	/** The ropes Drag coefficient, how much drag is experienced by the rope. Simulation wide settings affect this as well. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.0", UIMin = "0.01"))
	float DragCoefficient;
	
	/** Rest length of the Rope, NOT the actual current length. Only used during Construction. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Length;

	/** How wide the Rope geometry is, does not affect the particles' collision radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Width;

	/** How many segments the Rope has */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope", meta = (ClampMin = "1", UIMin = "1"))
	int32 NumSegments;

	/** Number of sides of the Rope geometry */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rope Rendering", meta = (ClampMin = "1", ClampMax = "16"))
	int32 NumSides;

	/** How many times to repeat the material along the length of the Rope */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rope Rendering", meta = (UIMin = "0.1", UIMax = "8"))
	float TileMaterial;

protected:
	TArray<FVDParticleInfo> ParticleBuildInfo;
	TArray<FVDParticleAttachment> ParticleAttachmentsToResolveOnBuild;

	UPROPERTY(BlueprintReadOnly, Category = "Rope")
	TArray<UVDParticleSpringConstraint*> StretchConstraints;
	UPROPERTY(BlueprintReadOnly, Category = "Rope")
	TArray<UVDParticleSpringConstraint*> BendConstraints;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Rope")
	TArray<FRopePiece> Pieces;

	uint32 bBuildRopeOnNextSync : 1;

	friend class FVDRopeSceneProxy;
};