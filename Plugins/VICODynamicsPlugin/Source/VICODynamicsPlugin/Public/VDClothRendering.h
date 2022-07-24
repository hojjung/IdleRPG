// Copyright 2012-2017 VICO Game Studio LLC. Rights Reserved.

#pragma once

#include "VDProceduralClothComponent.h"
#include "VDMeshClothComponent.h"
#include "DynamicMeshBuilder.h"
#include "PrimitiveSceneProxy.h"
#include "Materials/Material.h"
#include "MeshBatch.h"
#include "Engine/Engine.h"
#include "SceneManagement.h"
#include "RHIDefinitions.h"

DECLARE_CYCLE_STAT(TEXT("ProcBuildClothMesh"), STAT_ProcBuildClothMesh, STATGROUP_VICODynamicsRendering);
DECLARE_CYCLE_STAT(TEXT("ProcClothSetDynamicDataRenderThread"), STAT_ProcClothSetDynamicDataRenderThread, STATGROUP_VICODynamicsRendering);
DECLARE_CYCLE_STAT(TEXT("ProcClothGetDynamicMeshElements"), STAT_ProcClothGetDynamicMeshElements, STATGROUP_VICODynamicsRendering);

/** Index Buffer */
class FVDClothIndexBuffer : public FIndexBuffer
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
struct FVDClothDynamicData
{
	/** Array of points */
	TArray<FVector> VDClothPoints;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	TArray<FVDDebugRenderData> DebugRenderData;
#endif

#if WITH_EDITORONLY_DATA
	TArray<FVector> PointsToHighlight;
#endif
};

//////////////////////////////////////////////////////////////////////////
// FVDProceduralClothSceneProxy

class FVDProcduralClothSceneProxy : public FPrimitiveSceneProxy
{
public:

	FVDProcduralClothSceneProxy(UVDProceduralClothComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, Material(NULL)
		, VertexFactory(GetScene().GetFeatureLevel(), "VDClothVertexFactory")
		, DynamicData(NULL)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, Width(Component->NumParticlesWide)
		, Height(Component->NumParticlesHigh)
		, TileMaterialAlongWidth(Component->TileMaterialAlongWidth)
		, TileMaterialAlongHeight(Component->TileMaterialAlongHeight)
		, TornPoints(Component->Tears)
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		, ShowParticles(Component->ShowParticles)
#endif
	{
		VertexBuffers.InitWithDummyData(&VertexFactory, GetRequiredVertexCount());

		IndexBuffer.NumIndices = GetRequiredIndexCount();
		checkf(IndexBuffer.NumIndices, TEXT("Index Num is zero!"));

		// Enqueue initialization of render resource
		BeginInitResource(&IndexBuffer);

		// Grab material
		Material = Component->GetMaterial(0);
		if (Material == NULL)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FVDProcduralClothSceneProxy()
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
		return Width * Height;
	}

	int32 GetRequiredIndexCount() const
	{
		return (((Width - 1) * (Height - 1)) * 3 * 2) - TornPoints.Num() * 3 * 2;
	}

	int32 GetVertIndex(int32 x, int32 y) const
	{
		return y + (x * Height);
	}

	void BuildClothMesh(const TArray<FVector>& InPoints, TArray<FDynamicMeshVertex>& OutVertices, TArray<int32>& OutIndices)
	{
		SCOPE_CYCLE_COUNTER(STAT_ProcBuildClothMesh);

		const FColor VertexColor(255, 255, 255);
		const int32 NumPoints = InPoints.Num();

		// Build vertices

		if (NumPoints < 2)
			return;
		
		for (int32 y = 0; y < Height; y++)
		{
			const float AlongY = (float)y / (float)Height; // Distance along VDCloth Height

			for (int32 x = 0; x < Width; x++)
			{
				const auto index = x + y * Width;
				const float AlongX = (float)x / (float)Width;

				FDynamicMeshVertex Vert;
				Vert.Position = InPoints[index];
				Vert.TextureCoordinate[0] = FVector2D(AlongX * TileMaterialAlongWidth, AlongY * TileMaterialAlongHeight);
				Vert.Color = VertexColor;
				OutVertices.Add(Vert);
			}
		}

		Normals.Reset(OutVertices.Num());
		TangentX.Reset(OutVertices.Num());
		Normals.AddZeroed(OutVertices.Num());
		TangentX.AddZeroed(OutVertices.Num());

		// Build triangles
		for (int32 y = 0; y < Height - 1; y++)
		{
			for (int32 x = 0; x < Width - 1; x++)
			{
				const auto index = GetVertIndex(x, y);

				const auto TL = index;
				const auto BL = GetVertIndex(x, y + 1);
				const auto TR = GetVertIndex(x + 1, y);
				const auto BR = GetVertIndex(x + 1, y + 1);

				OutIndices.Add(TL);
				OutIndices.Add(BL);
				OutIndices.Add(TR);

				OutIndices.Add(TR);
				OutIndices.Add(BL);
				OutIndices.Add(BR);

				const auto TangentX1 = OutVertices[BL].Position - OutVertices[TL].Position;
				// Generate normals
				const auto TriNormal1 = TangentX1 ^ (OutVertices[TL].Position - OutVertices[TR].Position);

				Normals[TR] += TriNormal1;
				Normals[BL] += TriNormal1;
				Normals[TL] += TriNormal1;
				Normals[BR] += TriNormal1;

				TangentX[TR] += TangentX1;
				TangentX[BL] += TangentX1;
				TangentX[TL] += TangentX1;
				TangentX[BR] += TangentX1;
			}
		}

		for (int32 i = 0; i < Normals.Num(); i++)
		{
			OutVertices[i].SetTangents(TangentX[i].GetSafeNormal(), FVector::ZeroVector, Normals[i].GetSafeNormal());
		}

		if (TornPoints.Num())
		{
			TArray<int32> indecisToRemove;
			for (int32 triIdx = 0; triIdx < OutIndices.Num(); triIdx += 3)
			{
				const auto vert1 = OutIndices[triIdx];
				const auto vert2 = OutIndices[triIdx + 1];
				const auto vert3 = OutIndices[triIdx + 2];

				for (int32 tearIdx = 0; tearIdx < TornPoints.Num(); tearIdx++)
				{
					if ((TornPoints[tearIdx].Get<0>() == vert1 && (TornPoints[tearIdx].Get<1>() == vert2 || TornPoints[tearIdx].Get<1>() == vert3)) ||
						(TornPoints[tearIdx].Get<0>() == vert2 && (TornPoints[tearIdx].Get<1>() == vert1 || TornPoints[tearIdx].Get<1>() == vert3)) ||
						(TornPoints[tearIdx].Get<0>() == vert3 && (TornPoints[tearIdx].Get<1>() == vert1 || TornPoints[tearIdx].Get<1>() == vert2)))
					{
						indecisToRemove.Add(triIdx);
						indecisToRemove.Add(triIdx + 1);
						indecisToRemove.Add(triIdx + 2);
					}
				}
			}

			for (int32 idxToRemove = indecisToRemove.Num() - 1; idxToRemove >= 0; idxToRemove--)
			{
				OutIndices.RemoveAt(indecisToRemove[idxToRemove], 1, false);
			}

			OutIndices.Shrink();
		}
	}

	/** Called on render thread to assign new dynamic data */
	void SetDynamicData_RenderThread(FVDClothDynamicData* NewDynamicData)
	{
		SCOPE_CYCLE_COUNTER(STAT_ProcClothSetDynamicDataRenderThread);
		check(IsInRenderingThread());

		// Free existing data if present
		if (DynamicData)
		{
			delete DynamicData;
			DynamicData = NULL;
		}
		DynamicData = NewDynamicData;

		if (!DynamicData) return;

		// Build mesh from VDCloth points
		TArray<FDynamicMeshVertex> Vertices;
		TArray<int32> Indices;
		BuildClothMesh(NewDynamicData->VDClothPoints, Vertices, Indices);

		if (Indices.Num() != GetRequiredIndexCount())
		{
			IndexBuffer.NumIndices = Indices.Num();
			IndexBuffer.UpdateRHI();
		}

		check(Vertices.Num() == GetRequiredVertexCount());
		check(Indices.Num() == IndexBuffer.NumIndices/*GetRequiredIndexCount()*/);

		for (int i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
			VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}

		{
			auto& VertexBuffer = VertexBuffers.PositionVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.ColorVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTangentSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTangentData(), VertexBuffer.GetTangentSize());
			RHIUnlockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTexCoordSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTexCoordData(), VertexBuffer.GetTexCoordSize());
			RHIUnlockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
		}

		void* IndexBufferData = RHILockIndexBuffer(IndexBuffer.IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &Indices[0], Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBuffer.IndexBufferRHI);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		SCOPE_CYCLE_COUNTER(STAT_ProcClothGetDynamicMeshElements);

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
//				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());

				bool bHasPrecomputedVolumetricLightmap;
				FMatrix PreviousLocalToWorld;
				int32 SingleCaptureIndex;
				GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex);

				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, UseEditorDepthTest());
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = /*GetRequiredIndexCount()*/IndexBuffer.NumIndices / 3;
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

				if (ShowParticles && (IsSelected() || ViewFamily.EngineShowFlags.Game) && DynamicData)
				{
					const auto LocalToWorldMat = GetLocalToWorld();
					for (int32 i = 0; i < DynamicData->DebugRenderData.Num(); i++)
					{
						DrawWireSphere(Collector.GetPDI(ViewIndex), LocalToWorldMat.TransformPosition(DynamicData->VDClothPoints[i]), DynamicData->DebugRenderData[i].IsFree ? FLinearColor::Green : FLinearColor::Red, DynamicData->DebugRenderData[i].Radius, 6, SDPG_World);
					}
				}
#endif

#if WITH_EDITORONLY_DATA
				if (IsSelected() && DynamicData)
				{
					for (int32 i = 0; i < DynamicData->PointsToHighlight.Num(); i++)
						DrawWireSphereAutoSides(Collector.GetPDI(ViewIndex), DynamicData->PointsToHighlight[i], FLinearColor::Red, 5.f, SDPG_World);
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

	virtual uint32 GetMemoryFootprint(void) const { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface* Material;

	FStaticMeshVertexBuffers VertexBuffers;
	FVDClothIndexBuffer IndexBuffer;
	FLocalVertexFactory VertexFactory;

	FVDClothDynamicData* DynamicData;

	FMaterialRelevance MaterialRelevance;

	int32 Width;
	int32 Height;

	float TileMaterialAlongWidth;
	float TileMaterialAlongHeight;

	TArray<FVector> Normals;
	TArray<FVector> TangentX;

	TArray<TTuple<int32, int32>> TornPoints;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	uint32 ShowParticles : 1;
#endif
};

//////////////////////////////////////////////////////////////////////////
// FVDMeshClothSceneProxy

class FVDMeshClothSceneProxy : public FPrimitiveSceneProxy
{
public:

	FVDMeshClothSceneProxy(UVDMeshClothComponent* Component)
		: FPrimitiveSceneProxy(Component)
		, Material(NULL)
		, VertexFactory(GetScene().GetFeatureLevel(), "VDMeshClothVertexFactory")
		, DynamicData(NULL)
		, MaterialRelevance(Component->GetMaterialRelevance(GetScene().GetFeatureLevel()))
		, VertexData(Component->SourceVertices)
		, Indices(Component->SourceIndices)
		, SourceToSimulatedVertexMappings(Component->SourceToSimulatedVertexMappings)
		, TornPoints(Component->Tears)
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		, ShowParticles(Component->ShowParticles)
#endif
	{
		VertexBuffers.InitWithDummyData(&VertexFactory, GetRequiredVertexCount());

		IndexBuffer.NumIndices = GetRequiredIndexCount();
		checkf(IndexBuffer.NumIndices, TEXT("Index Num is zero!"));

		// Enqueue initialization of render resource
		BeginInitResource(&IndexBuffer);

		// Grab material
		Material = Component->GetMaterial(0);
		if (Material == NULL)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
	}

	virtual ~FVDMeshClothSceneProxy()
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
		return VertexData.Num();
	}

	int32 GetRequiredIndexCount() const
	{
		return Indices.Num();
	}

	void BuildClothMesh(const TArray<FVector>& InPoints, TArray<FDynamicMeshVertex>& OutVertices)
	{
		const int32 NumPoints = InPoints.Num();

		// Build vertices

		if (NumPoints < 2)
			return;

		for (int32 i = 0; i < VertexData.Num(); i++)
		{
			// Make sure we grab the correct Particle data in case this is a duplicate vertex
			const auto PIdx = SourceToSimulatedVertexMappings[i];

			FDynamicMeshVertex Vertex;
			Vertex.Position = InPoints[PIdx];
			Vertex.TextureCoordinate[0] = VertexData[i].UVs[0];
			Vertex.Color = VertexData[i].Color;
			OutVertices.Add(Vertex);
		}

		Normals.Reset(OutVertices.Num());
		TangentXs.Reset(OutVertices.Num());
		Normals.AddZeroed(OutVertices.Num());
		TangentXs.AddZeroed(OutVertices.Num());

		// Build normals
		for (int32 i = 0; i < Indices.Num(); i += 3)
		{
			const auto TL = Indices[i];
			const auto TR = Indices[i + 1];
			const auto BL = Indices[i + 2];
			
			const auto Edge01 = OutVertices[TR].Position - OutVertices[TL].Position;
			const auto Edge02 = OutVertices[BL].Position - OutVertices[TR].Position;
			
			const auto TangentX = -Edge01;
			const auto TangentZ = Edge02 ^ Edge01;

			Normals[TR] += TangentZ;
			Normals[BL] += TangentZ;
			Normals[TL] += TangentZ;

			TangentXs[TR] += TangentX;
			TangentXs[BL] += TangentX;
			TangentXs[TL] += TangentX;
		}

		for (int32 i = 0; i < Normals.Num(); i++)
		{
			OutVertices[i].SetTangents(TangentXs[i].GetSafeNormal(), FVector::ZeroVector, Normals[i].GetSafeNormal());
		}

		if (TornPoints.Num())
		{
			TArray<int32> indecisToRemove;
			for (int32 triIdx = 0; triIdx < Indices.Num(); triIdx += 3)
			{
				const auto vert1 = Indices[triIdx];
				const auto vert2 = Indices[triIdx + 1];
				const auto vert3 = Indices[triIdx + 2];

				for (int32 tearIdx = 0; tearIdx < TornPoints.Num(); tearIdx++)
				{
					if ((TornPoints[tearIdx].Get<0>() == vert1 && (TornPoints[tearIdx].Get<1>() == vert2 || TornPoints[tearIdx].Get<1>() == vert3)) ||
						(TornPoints[tearIdx].Get<0>() == vert2 && (TornPoints[tearIdx].Get<1>() == vert1 || TornPoints[tearIdx].Get<1>() == vert3)) ||
						(TornPoints[tearIdx].Get<0>() == vert3 && (TornPoints[tearIdx].Get<1>() == vert1 || TornPoints[tearIdx].Get<1>() == vert2)))
					{
						indecisToRemove.Add(triIdx);
						indecisToRemove.Add(triIdx + 1);
						indecisToRemove.Add(triIdx + 2);
					}
				}
			}

			for (int32 idxToRemove = indecisToRemove.Num() - 1; idxToRemove >= 0; idxToRemove--)
			{
				Indices.RemoveAt(indecisToRemove[idxToRemove], 1, false);
			}

			Indices.Shrink();
		}
	}

	/** Called on render thread to assign new dynamic data */
	void SetDynamicData_RenderThread(FVDClothDynamicData* NewDynamicData)
	{
		check(IsInRenderingThread());

		// Free existing data if present
		if (DynamicData)
		{
			delete DynamicData;
			DynamicData = NULL;
		}
		DynamicData = NewDynamicData;

		// Build mesh from VDCloth points
		TArray<FDynamicMeshVertex> Vertices;
		BuildClothMesh(NewDynamicData->VDClothPoints, Vertices);

		if (Indices.Num() != GetRequiredIndexCount())
		{
			IndexBuffer.NumIndices = Indices.Num();
			IndexBuffer.UpdateRHI();
		}

		check(Vertices.Num() == GetRequiredVertexCount());
		check(Indices.Num() == IndexBuffer.NumIndices);

		for (int i = 0; i < Vertices.Num(); i++)
		{
			const FDynamicMeshVertex& Vertex = Vertices[i];

			VertexBuffers.PositionVertexBuffer.VertexPosition(i) = Vertex.Position;
			VertexBuffers.StaticMeshVertexBuffer.SetVertexTangents(i, Vertex.TangentX.ToFVector(), Vertex.GetTangentY(), Vertex.TangentZ.ToFVector());
			VertexBuffers.StaticMeshVertexBuffer.SetVertexUV(i, 0, Vertex.TextureCoordinate[0]);
			VertexBuffers.ColorVertexBuffer.VertexColor(i) = Vertex.Color;
		}

		{
			auto& VertexBuffer = VertexBuffers.PositionVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.ColorVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetNumVertices() * VertexBuffer.GetStride(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetVertexData(), VertexBuffer.GetNumVertices() * VertexBuffer.GetStride());
			RHIUnlockVertexBuffer(VertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTangentSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTangentData(), VertexBuffer.GetTangentSize());
			RHIUnlockVertexBuffer(VertexBuffer.TangentsVertexBuffer.VertexBufferRHI);
		}

		{
			auto& VertexBuffer = VertexBuffers.StaticMeshVertexBuffer;
			void* VertexBufferData = RHILockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI, 0, VertexBuffer.GetTexCoordSize(), RLM_WriteOnly);
			FMemory::Memcpy(VertexBufferData, VertexBuffer.GetTexCoordData(), VertexBuffer.GetTexCoordSize());
			RHIUnlockVertexBuffer(VertexBuffer.TexCoordVertexBuffer.VertexBufferRHI);
		}

		void* IndexBufferData = RHILockIndexBuffer(IndexBuffer.IndexBufferRHI, 0, Indices.Num() * sizeof(int32), RLM_WriteOnly);
		FMemory::Memcpy(IndexBufferData, &Indices[0], Indices.Num() * sizeof(int32));
		RHIUnlockIndexBuffer(IndexBuffer.IndexBufferRHI);
	}

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override
	{
		QUICK_SCOPE_CYCLE_COUNTER(STAT_VDClothSceneProxy_GetDynamicMeshElements);

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
//				BatchElement.PrimitiveUniformBuffer = CreatePrimitiveUniformBufferImmediate(GetLocalToWorld(), GetBounds(), GetLocalBounds(), true, UseEditorDepthTest());

				bool bHasPrecomputedVolumetricLightmap;
				FMatrix PreviousLocalToWorld;
				int32 SingleCaptureIndex;
				GetScene().GetPrimitiveUniformShaderParameters_RenderThread(GetPrimitiveSceneInfo(), bHasPrecomputedVolumetricLightmap, PreviousLocalToWorld, SingleCaptureIndex);

				FDynamicPrimitiveUniformBuffer& DynamicPrimitiveUniformBuffer = Collector.AllocateOneFrameResource<FDynamicPrimitiveUniformBuffer>();
				DynamicPrimitiveUniformBuffer.Set(GetLocalToWorld(), PreviousLocalToWorld, GetBounds(), GetLocalBounds(), true, bHasPrecomputedVolumetricLightmap, UseEditorDepthTest());
				BatchElement.PrimitiveUniformBufferResource = &DynamicPrimitiveUniformBuffer.UniformBuffer;

				BatchElement.FirstIndex = 0;
				BatchElement.NumPrimitives = IndexBuffer.NumIndices / 3;
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

				if (ShowParticles && (IsSelected() || ViewFamily.EngineShowFlags.Game))
				{
					const auto LocalToWorldMat = GetLocalToWorld();

					for (int32 i = 0; i < DynamicData->DebugRenderData.Num(); i++)
					{
						DrawWireSphere(Collector.GetPDI(ViewIndex), LocalToWorldMat.TransformPosition(DynamicData->VDClothPoints[i]), DynamicData->DebugRenderData[i].IsFree ? FLinearColor::Green : FLinearColor::Red, 6, 6, SDPG_World);
					}
				}
#endif

#if WITH_EDITORONLY_DATA
				if (IsSelected())
				{
					for (int32 i = 0; i < DynamicData->PointsToHighlight.Num(); i++)
						DrawWireSphereAutoSides(Collector.GetPDI(ViewIndex), DynamicData->PointsToHighlight[i], FLinearColor::Red, 5.f, SDPG_World);
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

	virtual uint32 GetMemoryFootprint(void) const { return(sizeof(*this) + GetAllocatedSize()); }

	uint32 GetAllocatedSize(void) const { return(FPrimitiveSceneProxy::GetAllocatedSize()); }

private:

	UMaterialInterface* Material;

	FStaticMeshVertexBuffers VertexBuffers;
	FVDClothIndexBuffer IndexBuffer;
	FLocalVertexFactory VertexFactory;

	FVDClothDynamicData* DynamicData;

	FMaterialRelevance MaterialRelevance;
	TArray<FStaticMeshBuildVertex> VertexData;
	TArray<uint32> Indices;
	TArray<int32> SourceToSimulatedVertexMappings;

	TArray<FVector> Normals;
	TArray<FVector> TangentXs;

	TArray<TTuple<int32, int32>> TornPoints;

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	uint32 ShowParticles : 1;
#endif
};