// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ Begin Player Interface
	virtual void AddtoXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	//~ End Player Interface
	
	//~ Begin Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;
	//~ End Combat Interface
private:
	virtual void InitAbilityActorInfo() override;
};
