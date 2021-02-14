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
		class UMyCartMoveComponent* Movement;

	//同期移動コンポーネント
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
		class UMyCartMoveComponentReplicator* MovementReplicator;

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

	//レイトレースの起点となるSceneComponent
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *ForwardRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *BackwardRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *LeftRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *RightRayPoint;

	//レース状態を管理するコンポーネント
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class URaceSettingComponent* RaceSettingComponent;








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
	void ItemUse();
	//道路判定
	UFUNCTION()
	float RoadSpeedCalcFunction(class ARayActor *rayActor);

	UFUNCTION()
	ARayActor* SpawnRayActor(USceneComponent *parent);

	//レイトレース用のBP参照用
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARayActor>RayActorClass;

	//ゴールに必要なラップ数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//現在何ラップ走ったかカウントする変数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//UIの実数を格納する変数
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;

	//AudioComponent
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* EngineSoundComponent;


	//変数参照用
	ARayActor* RayForward;
	ARayActor* RayBackward;
	ARayActor* RayLeft;
	ARayActor* RayRight;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	


};
