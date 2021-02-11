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
	
	///�R���|�[�l���g	
	//�ړ��R���|�[�l���g
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
		class UMyCartMoveComponent* Movement;

	//�����ړ��R���|�[�l���g
	UPROPERTY(VisibleAnywhere , BlueprintReadOnly)
		class UMyCartMoveComponentReplicator* MovementReplicator;

	//�A�C�e���ݒ�̃R���|�[�l���g
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UItemSettingComponent* ItemSettingComponent;
	

	//�R���C�_�[
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* BoxCollision;

	//���b�V���̃I�t�Z�b�g
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* MeshOffSetRoot;

	//���b�V��
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USkeletalMeshComponent* CarMesh;

	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	//���C�g���[�X�̋N�_�ƂȂ�SceneComponent
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *ForwardRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *BackwardRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *LeftRayPoint;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent *RightRayPoint;







protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	//����
	ENetRole Role;

	//�����̊m�F���\�b�h
	FString GetEnumText(ENetRole Role);

	UFUNCTION()
	void ItemUseMultiCast();

	UFUNCTION()
	void ItemUseRunonServer();

	UFUNCTION()
	void ItemUse();
	//���H����
	UFUNCTION()
	float RoadSpeedCalcFunction(class ARayActor *rayActor);

	UFUNCTION()
	ARayActor* SpawnRayActor(USceneComponent *parent);

	//���C�g���[�X�p��BP�Q�Ɨp
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARayActor>RayActorClass;

	//�S�[���ɕK�v�ȃ��b�v��
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 GoalLap;

	//���݉����b�v���������J�E���g����ϐ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Race")
		int32 LapCounter;

	//UI�̃C���X�^���X
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> GameUiClass;

	//UI�̎������i�[����ϐ�
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;


	//�ϐ��Q�Ɨp
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
