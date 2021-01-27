// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class RACEGAME_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	//�R���|�[�l���g�ݒ�
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* Arrow;

	UPROPERTY(Category = Display, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* DummyItemMesh;



protected:
	//�ϐ��̐ݒ�

	//�A�C�e���̏o����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		int32 ItemSpawnNum;


	//�A�C�e�����b�V���̃T�C�Y
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		int32 ItemMeshSize;


	//�A�C�e���̃X�y�[�X
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		float Offset;

	//Spanw������A�C�e��
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AActor> ItemBoxClass;




};
