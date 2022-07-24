// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "BaseVDComponent.h"
#include "VDSimulation.h"
#include "VDSimulatedParticle.h"
#include "VDSimulatedObject.h"
#include "Engine/StaticMesh.h"
#include "VDMeshClothComponent.generated.h"

class FVDMeshClothSceneProxy;

UCLASS(hidecategories = (Object, Physics, Collision, Activation, "Components|Activation"), editinlinenew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering)
class VICODYNAMICSPLUGIN_API UVDMeshClothComponent : public UBaseVDComponent
{
	GENERATED_BODY()

public:
	UVDMeshClothComponent(const FObjectInitializer& ObjectInitializer);
	
	// Begin UActorComponent interface.
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SendRenderDynamicData_Concurrent() override;
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

	/**
	 * Creates the points that make up the cloth. Used during preview and as the initial start info for the particles.
	 */
	virtual void CreateClothGeometry(TArray<FVDParticleInfo>& ClothPoints);

	/**
	 * Sends the updated particle info to the Scene Proxy.
	 */
	virtual void UpdateSceneProxy(FVDMeshClothSceneProxy* Proxy);

	/**
	 * Build the actual simulated cloth using VICODynamics
	 */
	virtual void BuildSimulatedCloth();

	const TArray<UVDParticleSpringConstraint*>& GetStretchConstraintsArray() const { return StretchConstraints; }
	const TArray<UVDParticleSpringConstraint*>& GetBendConstraintsArray() const { return BendConstraints; }

protected:
	void ResolvePreBuildParticleAttachments();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloth")
	UStaticMesh* HullMesh;

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
	
private:
	// NOT READY FOR PRIMETIME YET
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cloth", meta = (DevelopmentStatus = Experimental))
	bool TearingEnabled;

public:
	/** If true, uses the Vertex Colors to setup Particle properties. 
	 * Red channel   - used to enable/disable Particle-RigidBody collision (0=Disabled, 1+=Enabled)
	 * Green channel - used to enable/disable Particle-Particle collision (0=Disabled, 1+=Enabled)
	 * Blue channel  - unused currently
	 * Alpha channel - unused currently
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cloth")
	uint32 UseVertexColorDataForSetup : 1;

	/** The maximum distance a Bend Constraint is allowed to span. Helps avoid attaching points that are too far away to have any reasonable connection created. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VDynamics", meta = (ClampMin = "0.01", UIMin = "0.01"))
	float MaxBendConstraintDistance;

protected:
	TArray<FVDParticleInfo> ParticleBuildInfo;
	bool bBuildOnNextSync;
	int32 NumParticles;
	TArray<FVDParticleAttachment> ParticleAttachmentsToResolveOnBuild;

	UPROPERTY(BlueprintReadOnly, Category = "Cloth")
	TArray<UVDParticleSpringConstraint*> StretchConstraints;
	UPROPERTY(BlueprintReadOnly, Category = "Cloth")
	TArray<UVDParticleSpringConstraint*> BendConstraints;
	
	TArray<TTuple<int32, int32>> Tears;

	TArray<FStaticMeshBuildVertex> SourceVertices;
	TArray<uint32> SourceIndices;
	TArray<FStaticMeshBuildVertex> SimulatedVertices;
	TArray<uint32> SimulatedIndices;
	TArray<int32> SourceToSimulatedVertexMappings;
	
	friend class FVDMeshClothSceneProxy;
};