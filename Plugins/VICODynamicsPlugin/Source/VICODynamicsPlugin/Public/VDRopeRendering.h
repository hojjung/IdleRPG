// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VDRopeComponent.h"

#include "DynamicMeshBuilder.h"
#include "Materials/Material.h"
#include "Engine/Engine.h"
#include "SceneManagement.h"
#include "PrimitiveSceneProxy.h"
#include "StaticMeshResources.h"

DECLARE_CYCLE_STAT(TEXT("BuildRopeMesh"), STAT_BuildRopeMesh, STATGROUP_VICODynamicsRendering);
DECLARE_CYCLE_STAT(TEXT("RopeSetDynamicDataRenderThread"), STAT_RopeSetDynamicDataRenderThread, STATGROUP_VICODynamicsRendering);
DECLARE_CYCLE_STAT(TEXT("RopeGetDynamicMeshElements"), STAT_RopeGetDynamicMeshElements, STATGROUP_VICODynamicsRendering);

/** Index Buffer */
class FVDRopeIndexBuffer : public FIndexBuffer
{
public:
	virtual void InitRHI() override
	{
		FRHIResourceCreateInfo CreateInfo;
		IndexBufferRHI = RHICreateIndexBuffer(sizeof(int32), NumIndices * sizeof(int32), BUF_Dynamic, CreateInfo);
	}

	int32 NumIndices;
};

/** Dynamic data sent to render thread */
struct FVDRopeDynamicData
{
	/** Array of points */
	TArray<FVector> VDRopePoints;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FVDDebugRenderData> DebugRenderData;
#endif
};

//////////////////////////////////////////////////////////////////////////
// FVDRopeSceneProxy

class FVDRopeSceneProxy : public FPrimitiveSceneProxy
{
public:

	FVDRopeSceneProxy(UVDRopeComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, Material(NULL)
		, VertexFactory(GetScene().GetFeatureLevel(), "VDRopeVertexFactory")
		, DynamicData(NULL)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, NumSegments(Component->NumSegments)
		, Width(Component->Width)
		, NumSides(Component->NumSides)
		, TileMaterial(Component->TileMaterial)
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		, ShowParticles(Component->ShowParticles)
		, ParticleRadius(Component->ParticleRadius)
#endif
	{
		for (int32 i = 0; i < Component->Pieces.Num(); i++)
		{
			const auto Piece = Component->Pieces[i];
			if (Piece.StartParticleIndex > 0)
				BrokenSegments.Add(Piece.StartParticleIndex - 1);
		}

		VertexBuffers.InitWithDummyData(&VertexFactory, GetRequiredVertexCount());

		IndexBuffer.NumIndices = GetRequiredIndexCount();

		// Enqueue initialization of render resource
		BeginInitResource(&IndexBuffer);

		// Grab material
		Material = Component->GetMaterial(0);
		if (Material == NULL)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FVDRopeSceneProxy()
	{
		VertexBuffers.PositionVertexBuffer.ReleaseResource();
		VertexBuffers.StaticMeshVertexBuffer.ReleaseResource();
		VertexBuffers.ColorVertexBuffer.ReleaseResource();
		IndexBuffer.ReleaseResource();
		VertexFactory.ReleaseResource();

		if (DynamicData != NULL)
		{
			delete DynamicData;
		}
	}

	SIZE_T GetTypeHash() const override
	{
		static size_t UniquePointer;
		return reinterpret_cast<size_t>(&UniquePointer);
	}

	int32 GetRequiredVertexCount() const
	{
		return (NumSegments + 1) * (NumSides + 1);
	}

	int32 GetRequiredIndexCount() const
	{
		return ((NumSegments - BrokenSegments.Num()) * NumSides * 2) * 3;
	}

	int32 GetVertIndex(int32 AlongIdx, int32 AroundIdx) const
	{
		return (AlongIdx * (NumSides + 1)) + AroundIdx;
	}

	void BuildRopeMesh(const TArray<FVector>& InPoints)
	{
		SCOPE_CYCLE_COUNTER(STAT_BuildRopeMesh);

//		const FColor VertexColor(255, 255, 255);
		const int32 NumPoints = InPoints.Num();
		const int32 SegmentCount = NumPoints - 1;

		// Build vertices
			   
		// We double up the first and last vert of the ring, because the UVs are different
		int32 NumRingVerts = NumSides + 1;

		if (NumPoints < 2)
			return;

		Vertices.Reset(GetRequiredVertexCount());
		Indices.Reset(GetRequiredIndexCount());

		FVector BasisX = (InPoints[1] - InPoints[0]).GetSafeNormal();
		FVector BasisY, BasisZ;
		BasisX.FindBestAxisVectors(BasisY, BasisZ);

		// For each point along spline..
		for (int32 PointIdx = 0; PointIdx < NumPoints; PointIdx++)
		{
			const float AlongFrac = (float)PointIdx / (float)SegmentCount; // Distance along VDRope

			const auto PartOfBrokenSegmnentAtStart = BrokenSegments.Contains(PointIdx);
			const auto PartOfBrokenSegmnentAtEnd = BrokenSegments.Contains(PointIdx - 1);

			// Find direction of VDRope at this point, by averaging previous and next points. Handle differently if part of a broken segment to avoid strange looking end points.
			const int32 PrevIndex = PartOfBrokenSegmnentAtEnd ? FMath::Max(0, PointIdx) : FMath::Max(0, PointIdx - 1);
			const int32 NextIndex = PartOfBrokenSegmnentAtStart ? FMath::Min(PointIdx, NumPoints - 1) : FMath::Min(PointIdx + 1, NumPoints - 1);
			const FVector ForwardDir = (InPoints[NextIndex] - InPoints[PrevIndex]).GetSafeNormal();

			FVector RotationAxis = (BasisX ^ ForwardDir).GetSafeNormal();
			float CosTheta = ForwardDir | BasisX;

			// Use a parallel transport frame to create a smooth basis along the rope
			if (FMath::Abs(CosTheta - 1.0f) > KINDA_SMALL_NUMBER)
			{
				BasisX = ForwardDir;

				// TODO: trigonometric functions totally unnecessary here
				float Theta = FMath::RadiansToDegrees(FMath::Acos(CosTheta));

				BasisY = BasisY.RotateAngleAxis(Theta, RotationAxis);
				BasisZ = BasisZ.RotateAngleAxis(Theta, RotationAxis);
			}

			// Generate a ring of verts
			for (int32 VertIdx = 0; VertIdx < NumRingVerts; VertIdx++)
			{
				const float AroundFrac = float(VertIdx) / float(NumSides);
				// Find angle around the ring
				const float RadAngle = 2.f * PI * AroundFrac;
				// Find direction from center of VDRope to this vertex
				const FVector OutDir = (FMath::Cos(RadAngle) * BasisY) + (FMath::Sin(RadAngle) * BasisZ);

				FDynamicMeshVertex Vert;
				Vert.Position = InPoints[PointIdx] + (OutDir * 0.5f * Width);
				Vert.TextureCoordinate[0] = FVector2D(AlongFrac * TileMaterial, AroundFrac);
//				Vert.Color = VertexColor;
				Vert.SetTangents(ForwardDir, OutDir ^ ForwardDir, OutDir);
				Vertices.Add(MoveTemp(Vert));
			}
		}

		// Build triangles
		for (int32 SegIdx = 0; SegIdx < SegmentCount; SegIdx++)
		{
			if (BrokenSegments.Contains(SegIdx)) continue;

			for (int32 SideIdx = 0; SideIdx < NumSides; SideIdx++)
			{
				int32 TL = GetVertIndex(SegIdx, SideIdx);
				int32 BL = GetVertIndex(SegIdx, SideIdx + 1);
				int32 TR = GetVertIndex(SegIdx + 1, SideIdx);
				int32 BR = GetVertIndex(SegIdx + 1, SideIdx + 1);

				Indices.Add(TL);
				Indices.Add(BL);
				Indices.Add(TR);

				Indices.Add(TR);
				Indices.Add(BL);
				Indices.Add(BR);
			}
		}
	}

	/** Called on render thread to assign new dynamic data */
	void SetDynamicData_RenderThread(FVDRopeDynamicData* NewDynamicData)
	{
		SCOPE_CYCLE_COUNTER(STAT_RopeSetDynamicDataRenderThread);

		check(IsInRenderingThread());

		// Free existing data if present
		if (DynamicData)
			delete DynamicData;
		DynamicData = NewDynamicData;

		// Build mesh from VDRope points		
		BuildRopeMesh(NewDynamicData->VDRopePoints);

		check(Vertices.Num() == GetRequiredVertexCount());
		check(Indices.Num() == GetRequiredIndexCount());

		for (int i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
//			VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}

		{
			auto& VertexBuffer = VertexBuffers.PositionVertexBuffer;
			const auto BufferSize = VertexBuffer.GetNumVertices() * VertexBuffer.GetStride();
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, BufferSize, RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), BufferSize);
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

// 		{
// 			auto& VertexBuffer = VertexBuffers.ColorVertexBuffer;
// 			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
// 			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
// 			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
// 		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			const auto BufferSize = VertexBuffer.GetTangentSize();
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI, 0, BufferSize, RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTangentData(), BufferSize);
			RHIUnlockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			const auto BufferSize = VertexBuffer.GetTexCoordSize();
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, 0, BufferSize, RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTexCoordData(), BufferSize);
			RHIUnlockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
		}

		const auto BufferSize = Indices.Num() * sizeof(int32);
		void* IndexBufferData = RHILockIndexBuffer(IndexBuffer.IndexBufferRHI, 0, BufferSize, RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &Indices[0], BufferSize);
		RHIUnlockIndexBuffer(IndexBuffer.IndexBufferRHI);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
//		SCOPE_CYCLE_COUNTER(STAT_RopeGetDynamicMeshElements);

		const bool bWireframe = AllowDebugViewmodes() && ViewFamily.EngineShowFlags.Wireframe;

		auto WireframeMaterialInstance = new FColoredMaterialRenderProxy(
			GEngine->WireframeMaterial ? GEngine->WireframeMaterial->GetRenderProxy() : NULL,
			FLinearColor(0, 0.5f, 1.f)
		);

		Collector.RegisterOneFrameMaterialProxy(WireframeMaterialInstance);

		FMaterialRenderProxy* MaterialProxy = NULL;
		if (bWireframe)
		{
			MaterialProxy = WireframeMaterialInstance;
		}
		else
		{
			MaterialProxy = Material->GetRenderProxy();
		}

		for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
		{
			if (VisibilityMap & (1 << ViewIndex))
			{
				const FSceneView* View = Views[ViewIndex];
				// Draw the mesh.
				FMeshBatch& Mesh = Collector.AllocateMesh();
				FMeshBatchElement& BatchElement = Mesh.Elements[0];
				BatchElement.IndexBuffer = &IndexBuffer;
				Mesh.bWireframe = bWireframe;
				Mesh.VertexFactory = &VertexFactory;
				Mesh.MaterialRenderProxy = MaterialProxy;				
				//BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());

				bool bHasPrecomputedVolumetricLightmap;
				FMatrix PreviousLocalToWorld;
				int32 SingleCaptureIndex;
				GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex);

				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, UseEditorDepthTest());
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = GetRequiredIndexCount() / 3;
				BatchElement.MinVertexIndex = 0;
				BatchElement.MaxVertexIndex = GetRequiredVertexCount();
				Mesh.ReverseCulling = IsLocalToWorldDeterminantNegative();
				Mesh.Type = PT_TriangleList;
				Mesh.DepthPriorityGroup = SDPG_World;
				Mesh.bCanApplyViewModeOverrides = false;
				Collector.AddMesh(ViewIndex, Mesh);

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
				// Render bounds
				RenderBounds(Collector.GetPDI(ViewIndex), ViewFamily.EngineShowFlags, GetBounds(), IsSelected());

				if (DynamicData && ShowParticles && (IsSelected() || ViewFamily.EngineShowFlags.Game))
				{
					const auto LocalToWorldMat = GetLocalToWorld();

					for (int32 i = 0; i < DynamicData->DebugRenderData.Num(); i++)
					{
						DrawWireSphere(Collector.GetPDI(ViewIndex), LocalToWorldMat.TransformPosition(DynamicData->VDRopePoints[i]), 
									   DynamicData->DebugRenderData[i].IsFree ? FLinearColor::Green : FLinearColor::Red, ParticleRadius, 6, SDPG_World);
					}
				}
#endif
			}
		}
	}

	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override
	{
		FPrimitiveViewRelevance Result;
		Result.bDrawRelevance = IsShown(View);
		Result.bShadowRelevance = IsShadowCast(View);
		Result.bDynamicRelevance = true;
		Result.bRenderCustomDepth = ShouldRenderCustomDepth();
		MaterialRelevance.SetPrimitiveViewRelevance(Result);
		return Result;
	}

	virtual uint32 GetMemoryFootprint() const { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize() const { return (FPrimitiveSceneProxy::GetAllocatedSize()); }

	int32 GetNumSegments() const { return NumSegments; }

protected:

	UMaterialInterface* Material;

	FStaticMeshVertexBuffers VertexBuffers;
	FVDRopeIndexBuffer IndexBuffer;
	FLocalVertexFactory VertexFactory;

	TArray<FDynamicMeshVertex> Vertices;
	TArray<int32> Indices;

	FVDRopeDynamicData* DynamicData;

	FMaterialRelevance MaterialRelevance;

	int32 NumSegments;

	float Width;

	int32 NumSides;

	float TileMaterial;

	TArray<int32> BrokenSegments;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	int32 ShowParticles : 1;
	float ParticleRadius;
#endif
};