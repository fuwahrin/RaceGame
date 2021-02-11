// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RayActor.generated.h"

UCLASS()
class RACEGAME_API ARayActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARayActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "RayActor", meta = (AllowPrivateAccess = "true"))
	FName CurrentState;


	UFUNCTION(BlueprintCallable)
	void RayTrace();

public:
	UFUNCTION(BlueprintCallable)
		FName GetRayHitState() { return CurrentState; }

	UFUNCTION(BlueprintCallable)
		bool IsStateChange() { return CurrentState != FName("None"); }



};
