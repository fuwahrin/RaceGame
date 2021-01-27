// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyCartMoveComponent.h"
#include "MyCartMoveComponentReplicator.h"
#include "MyPawn.generated.h"



UCLASS()
class RACEGAME_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();
	
	///コンポーネント	
	//移動コンポーネント
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	UMyCartMoveComponent* MovementComponent;

	//同期移動コンポーネント
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	UMyCartMoveComponentReplicator* MovementComponentReplicator;
private:
///////変数
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);


	//権限
	ENetRole Role;

	//権限の確認メソッド
	FString GetEnumText(ENetRole Role);

public:

	UPROPERTY(EditAnyWhere , BlueprintReadWrite)
	float SpeedRate = 1.0f;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
