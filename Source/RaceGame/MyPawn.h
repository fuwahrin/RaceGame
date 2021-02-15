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

	//���[�X��Ԃ��Ǘ�����R���|�[�l���g
	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class URaceSettingComponent* RaceSettingComponent;

	//AudioComponent
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UAudioComponent* EngineSoundComponent;

	//�ϐ��Q�Ɨp
	ARayActor* RayForward;
	ARayActor* RayBackward;
	ARayActor* RayLeft;
	ARayActor* RayRight;


	//����
	ENetRole Role;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	//�O�i�E���
	void MoveForward(float value);

	//���E
	void MoveRight(float value);

	//�A�C�e�����d�l������Ԃɂ��郁�\�b�h
	UFUNCTION()
		void ItemUse();
	
	//���H�̑��x�l���擾���郁�\�b�h
	UFUNCTION()
		float RoadSpeedCalcFunction(class ARayActor *rayActor);

	//���C�g���[�X�̃A�N�^�[���o��������B
	UFUNCTION()
		ARayActor* SpawnRayActor(USceneComponent *parent);

	//���C�g���[�X�p��BP�Q�Ɨp
	UPROPERTY(EditAnywhere)
		TSubclassOf<class ARayActor>RayActorClass;

	//�����̊m�F���\�b�h
	FString GetEnumText(ENetRole Role);


};
