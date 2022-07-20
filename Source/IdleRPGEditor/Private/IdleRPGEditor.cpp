#include "IdleRPGEditor.h"
#include "IdleRPGEditor/Public/DataTableRowHandleCustom.h"
#include "DataTableEditorUtils.h"

class OnDataTableChanged :public INotifyOnChanged<UDataTable,FDataTableEditorUtils::EDataTableChangeInfo>//,public FDataTableEditorUtils::INotifyOnDataTableChanged
{
public:
	virtual ~OnDataTableChanged() 
	{
		Module = nullptr;
	}
	
	FIdleRPGEditorModule* Module;
	
	virtual void PreChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		//Module->OnAllPlaceableAssetsChanged();
	}
	virtual void PostChange(const UDataTable* Changed, FDataTableEditorUtils::EDataTableChangeInfo ChangedType)override
	{
		// if(!Changed->RowStruct->IsChildOf(FSkillDataRow::StaticStruct()) && !Changed->RowStruct->IsChildOf(FPlayerUnitEntityRow::StaticStruct()))
		// {
		// 	return;//스킬테이블이 바뀌면
		// }
		// Module->RefreshPlayerSkillData();//플레이어 테이블 변경
	}
	
	virtual void SelectionChange(const UDataTable* DataTable, FName RowName)
	{
		
	}
};

void FIdleRPGEditorModule::StartupModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	//PropertyModule.RegisterCustomPropertyTypeLayout("ItemDataHandle", FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataTableRowHandleCustom::MakeInstance));


	m_OnDataTableChanged = MakeShared<OnDataTableChanged>();

	m_OnDataTableChanged->Module = this;
	
	FDataTableEditorUtils::FDataTableEditorManager::Get().AddListener(m_OnDataTableChanged.Get());
	
	RefreshPlayerSkillData();
	
	PropertyModule.NotifyCustomizationModuleChanged();
}//FCurrencyDataHandle

void FIdleRPGEditorModule::ShutdownModule()
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	
	//PropertyModule.UnregisterCustomPropertyTypeLayout("ItemDataHandle");

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FIdleRPGEditorModule::RefreshPlayerSkillData()
{
	//const TArray<FName> ArySkillNames = USkillData::GetSkillTable->GetRowNames();
	
	// for(TObjectIterator<UDataTable> classIt(EObjectFlags::RF_ClassDefaultObject,false,EInternalObjectFlags::None);classIt;++classIt)
	// {
	// 	if(!classIt->RowStruct->IsChildOf(FPlayerUnitEntityRow::StaticStruct()))
	// 	{
	// 		continue;
	// 	}
	// 	TArray<FPlayerUnitEntityRow*> AryUnits;
	// 	
	// 	classIt->GetAllRows(TEXT(""),AryUnits);
	//
	// 	for(FPlayerUnitEntityRow* Pl : AryUnits)
	// 	{
	// 		int Diff = ArySkillNames.Num() - Pl->m_ArySkillAnims.Num();
	//
	// 		while (Diff<0)
	// 		{
	// 			Pl->m_ArySkillAnims.Pop();
	// 			Diff++;
	// 		}
	// 		while (Diff>0)
	// 		{
	// 			Pl->m_ArySkillAnims.Add(FPlayerSkillAnim());
	// 			Diff--;
	// 		}
	// 		for(int i=0; i < ArySkillNames.Num(); i++)
	// 		{
	// 			Pl->m_ArySkillAnims[i].m_SkillID = ArySkillNames[i];
	// 		}
	// 	}
	// }
}

IMPLEMENT_GAME_MODULE(FIdleRPGEditorModule,IdleRPGEditor);