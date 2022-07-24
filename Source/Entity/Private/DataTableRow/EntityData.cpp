#include "DataTableRow/EntityData.h"
#include "Actors/Pawns/MyBasePawn.h"

FNpcUnitEntityRow::FNpcUnitEntityRow()
{
	m_ClassActor = AMyBasePawn::StaticClass();
}

USkeletalMesh* FNpcUnitEntityRow::GetSkMesh()
{
	if(m_UnitDataAsset.ToSoftObjectPath().IsNull())
	{
		return nullptr;
	}
	
	return m_UnitDataAsset.LoadSynchronous()->GetSkMesh();
}

float FNpcUnitEntityRow::GetMeshScale()
{
	return m_fScale;
}
