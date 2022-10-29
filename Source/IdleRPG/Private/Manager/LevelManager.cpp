#include "Manager/LevelManager.h"
#include "Kismet/GameplayStatics.h"

LevelManager::LevelManager(UObject* outer)
{
	m_Outer = outer;//
	m_CurrentLevel = TEXT("Map01");
}

LevelManager::~LevelManager()
{
	m_Outer.Reset();
}

bool LevelManager::OpenLevel(const FName& levelName)
{
	if(m_CurrentLevel == levelName)
	{
		return false;
	}
	
	m_CurrentLevel = levelName;
	
	UGameplayStatics::OpenLevel(m_Outer.Get(), m_CurrentLevel);

	return true;
}

void LevelManager::OnLoadComplete(const FString& mapName)
{
	m_OnLevelLoadComplete.ExecuteIfBound();
	m_OnLevelLoadComplete.Unbind();
}
