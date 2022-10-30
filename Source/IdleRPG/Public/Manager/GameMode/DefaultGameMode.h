#pragma once

#include "CoreMinimal.h"
#include "Manager/GameMode/IdleRPGGameModeBase.h"
#include "DefaultGameMode.generated.h"

/**
 * 
 */
UCLASS()
class IDLERPG_API ADefaultGameMode : public AIdleRPGGameModeBase
{
	GENERATED_BODY()
	
protected:
	BigInt m_Gold;

private:
	virtual void StartPlay() override;
	
	virtual void SetLevel(int l) override;
	
	const FStageRow& GetStage(int stageLevel) const;

	TArray<FPrimaryAssetId> GetZone(int stageLevel);

	const FPrimaryAssetId& GetBossMonster(int stageLevel);

	void OnPlayerDead();

public:
	virtual FText GetStageName(int lv) override;
	
	void SpawnMobs();
	
	virtual void OnMonsterDead(AMonsterPawn* target) override;
	
	virtual void OnMonsterAnimEnd(AMonsterPawn* target) override;

	virtual void OnPlayerDead(AMyPlayerPawn* target) override;

	virtual void OnPlayerAnimEnd(AMyPlayerPawn* target) override;
};
