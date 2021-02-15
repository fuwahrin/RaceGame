// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "ItemBox.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//�R���|�[�l���g�ݒ�
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	RootComponent = Arrow;

	//�_�~�[���b�V���̐ݒ�
	DummyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DummyMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshSet(TEXT("StaticMesh'/Engine/EditorMeshes/EditorHelp.EditorHelp'"));
	UStaticMesh* DummyItemMeshAsset = MeshSet.Object;
	DummyItemMesh->SetStaticMesh(DummyItemMeshAsset);
	DummyItemMesh->SetHiddenInGame(true);
	DummyItemMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -50.0f));
	DummyItemMesh->SetupAttachment(Arrow);
	
	//�ϐ��̏����l�ݒ�
	ItemSpawnNum = 4;
	ItemMeshSize = 100;
	Offset = 0.0f;

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	//�A�C�e������ׂ���W�ɖ�����������
	//�A�C�e�����o��������
	for (int i = 0; i < ItemSpawnNum; i++)
	{
		//�A�C�e������ʒu�̐ݒ�
		FVector AlignmentLocation;
		float LocationScaler = ItemMeshSize * i + Offset;

		//���g�̌����ɍ��킹�ăA�C�e�����o��������
		AlignmentLocation = LocationScaler * GetActorForwardVector();

		//�o��������
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParam;

			//�I�[�i�[
			SpawnParam.Owner = this;
			
			//�o���ʒu�̐ݒ�
			FVector SpawnLocation = GetActorLocation() + AlignmentLocation;
			FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

			//�X�|�[������
			AActor* const SpawnItems = World->SpawnActor<AActor>(ItemBoxClass, SpawnLocation , SpawnRotation);
		}
	}
	
}
