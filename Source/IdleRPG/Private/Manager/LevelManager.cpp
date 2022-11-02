#include "Manager/LevelManager.h"
#include "Kismet/GameplayStatics.h"

LevelManager::LevelManager(UObject* outer)
{
	m_Outer = outer;//
	m_CurrentLevel = TEXT("Plane");
}

LevelManager::~LevelManager()
{
	m_Outer.Reset();
}

bool LevelManager::IsNeedChangeMap(const FName& levelName)
{
	return m_CurrentLevel != levelName;
}

void LevelManager::OpenLevel(const FName& levelName)
{
	m_CurrentLevel = levelName;
	
	UGameplayStatics::OpenLevel(m_Outer.Get(), m_CurrentLevel);
}
