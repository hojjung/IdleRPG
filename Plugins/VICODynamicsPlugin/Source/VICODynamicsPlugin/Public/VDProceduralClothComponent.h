// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "BaseVDComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"
#include "VDSimulatedObject.h"
#include "VDProceduralClothComponent.generated.h"

class FVDProcduralClothSceneProxy;

UCLASS(hidecategories = (Object, Physics, Collision, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDProceduralClothComponent : public UBaseVDComponent
{
	GENERATED_BODY()

public:
	UVDProceduralClothComponent(const FObjectInitializer& ObjectInitializer);
	
	// Begin UActorComponent interface.
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
	// End UActorComponent interface.

	// Begin USceneComponent interface.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
	// End USceneComponent interface.

	// Begin UPrimitiveComponent interface.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual float GetMass() const override { return Mass; }
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
	// End UBaseVDComponent interface.

	/**
	 * Creates the points that make up the cloth. Used during preview and as the initial start info for the particles.
	 */
	virtual void CreateClothGeometry(TArray<FVDParticleInfo>& ClothPoints);

	/**
	 * Sends the updated particle info to the Scene Proxy.
	 */
	virtual void UpdateSceneProxy(FVDProcduralClothSceneProxy* Proxy);

	/**
	 * Build the actual simulated cloth using VICODynamics
	 */
	virtual void BuildSimulatedCloth();

	const TArray<UVDParticleSpringConstraint*>& GetStretchConstraintsArray() const { return StretchConstraints; }
	const TArray<UVDParticleSpringConstraint*>& GetBendConstraintsArray() const { return BendConstraints; }

public:
	/** How strongly the cloth resists stretching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float StretchStiffness;

	/** How strongly the cloth resists bending */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "0.0", UIMin = "0.0", UIMax = "1.0"))
	float BendStiffness;

	/** The cloths total Mass */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Mass;
	
	/** The cloths Drag coefficient, how much drag is experienced by the cloth. Simulation wide settings affect this as well. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.0", UIMin = "0.01"))
	float DragCoefficient;

	/** How wide the Cloth is, does not affect the particles' collision radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Width;

	/** How high/long the Cloth is, does not affect the particles' collision radius */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float Height;
	
	/** Number of particles to use for the wide */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "2", UIMin = "2"))
	int32 NumParticlesWide;
	
	/** Number of particles to use for the height */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (ClampMin = "2", UIMin = "2"))
	int32 NumParticlesHigh;

	/** How many times to repeat the material along the width of the Cloth */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth Rendering", meta = (UIMin = "0.1", UIMax = "100"))
	float TileMaterialAlongWidth;

	/** How many times to repeat the material along the height/length of the Cloth */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth Rendering", meta = (UIMin = "0.1", UIMax = "100"))
	float TileMaterialAlongHeight;

private:
	// NOT READY FOR PRIMETIME YET
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (DevelopmentStatus = Experimental))
	bool TearingEnabled;

protected:
	TArray<FVDParticleInfo> ParticleBuildInfo;
	int32 NumParticles;

	bool bBuildClothOnNextSync;
	
	UPROPERTY(BlueprintReadOnly, Category = "Cloth")
	TArray<UVDParticleSpringConstraint*> StretchConstraints;
	UPROPERTY(BlueprintReadOnly, Category = "Cloth")
	TArray<UVDParticleSpringConstraint*> BendConstraints;

	TArray<TTuple<int32, int32>> Tears;
	
	friend class FVDProcduralClothSceneProxy;
};