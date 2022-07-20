// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pawns/MyCombatPawn.h"

// Sets default values
AMyCombatPawn::AMyCombatPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyCombatPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyCombatPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCombatPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

