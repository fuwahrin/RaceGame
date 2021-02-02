// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
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
	class UMyCartMoveComponent* MovementComponent;

	//同期移動コンポーネント
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
	class UMyCartMoveComponentReplicator* MovementComponentReplicator;

	//アイテム設定のコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UItemSettingComponent* ItemSettingComponent;
	

	//コライダー
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoxCollision;

	//メッシュのオフセット
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MeshOffSetRoot;

	//メッシュ
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* CarMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	//権限
	ENetRole Role;

	//権限の確認メソッド
	FString GetEnumText(ENetRole Role);

	UFUNCTION()
	void ItemUseMultiCast();

	UFUNCTION()
	void ItemUseRunonServer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;



};
