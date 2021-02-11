// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawnComponent.h"
#include "ItemRefActor.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UItemSpawnComponent::UItemSpawnComponent()
{
	//Tick�𖳌�
	PrimaryComponentTick.bCanEverTick = false;

	//�ϐ��̏�����
	bIsItemUse = false;

	//�f�[�^�e�[�u���̏�����
	static ConstructorHelpers::FObjectFinder<UDataTable>TableDataAsset(TEXT("DataTable'/Game/Developers/fuwarrin/Collections/BP/ImportData/RaceGameItemListCPP.RaceGameItemListCPP'"));
	if (TableDataAsset.Succeeded())
	{
		ItemDataTable = TableDataAsset.Object;
	}

}


// Called when the game starts
void UItemSpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	//���L�҂�Pawn���擾
	OwnerPawn = GetOwner()->GetInstigator<APawn>();

	//���̃R���|�[�l���g�̓�����L���ɂ���
	SetIsReplicatedByDefault(true);
	
}


// Called every frame
void UItemSpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemSpawnComponent::SpawnItemMulticast_Implementation()
{
	SpawnItem();
}


void UItemSpawnComponent::SpawnItemRunonServer_Implementation()
{
	SpawnItem();
}

// �A�C�e�����o�������郁�\�b�h
void UItemSpawnComponent::SpawnItem()
{
	if (OwnerPawn != nullptr)
	{
		//�A�C�e�����g�p�ł����Ԃ��m�F
		if (bIsItemUse)
		{
			//�A�C�e����Spawn������
			UWorld* const World = GetWorld();
			if (World)
			{
				//�X�|�[��������ׂ̃p�����[�^
				FActorSpawnParameters SpawnParams;

				//�I�[�i�[
				SpawnParams.Owner = this->GetOwner();

				//�X�|�[���ʒu�̐ݒ�(ItemSpawnPoint�̃��[���h���W���i�[)
				//FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
				//FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();
				FVector SpawnLocation = GetComponentLocation();
				FRotator SpawnRotation = GetComponentRotation();

				//�X�|�[������ۂ�Scale
				FVector SpawnScale = FVector(ItemScale);



				//�A�C�e�����X�|�[������B
				AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
				SpawnItem->SetActorScale3D(SpawnScale);

				//UE_LOG(LogTemp, Error, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);

				ItemUse();

			}
		}
	}
}


//RPC�Ăяo�����ؗp�̃��\�b�h
//��false��Ԃ���RPC�����s����Ȃ�
bool UItemSpawnComponent::SpawnItemRunonServer_Validate()
{
	return true;
}

// �A�C�e�����g�p���郁�\�b�h
void UItemSpawnComponent::ItemUse()
{
	//�A�C�e�����g�p�������Ƃ�ݒ�

	//UI�̉摜�������B
	DrawIcon = nullptr;

	//�A�C�e���g�p�\�ϐ���False
	bIsItemUse = false;

	//�A�C�e��ID��������
	ItemNumber = 0;
}

//�A�C�e���̐ݒ���s�����\�b�h
void UItemSpawnComponent::ItemSetting(int32 ItemNum)
{
	//�A�C�e����ID��ݒ�
	ItemNumber = ItemNum;


	//�o������ۂ̐ݒ�
	SpawnSetting();

	//�A�C�e�����g�p�ł����ԂɕύX
	bIsItemUse = true;
}

void UItemSpawnComponent::ItemSettingRunOnServer_Implementation(int32 ItemNum)
{
	ItemSetting(ItemNum);
}


//	�A�C�e�����o�������郁�\�b�h
void UItemSpawnComponent::SpawnSetting()
{
	UE_LOG(LogTemp, Error, TEXT("SpawnSetting"));
	//�l�b�g���[�N�̌����ɂ���ď�����ς���
	if (GetOwner()->HasAuthority())
	{
		//�T�[�o��

		//ItemClass��ID�ɂ���Đݒ肷��B
		//-----TOOD:�O���t�@�C������ݒ肩�����ŃA�Z�b�g��ǂݍ��ނ悤�ɕύX����B
		switch (ItemNumber)
		{
		case 1:
			ItemClass = BulletItem;
			break;
		case 2:
			ItemClass = SlipItem;
			break;
		case 3:
			ItemClass = SpeedUpItem;
			break;
		}

		FName RowName = FName(FString::FromInt(ItemNumber));
		FItemStructCpp* ItemStruct = ItemDataTable->FindRow<FItemStructCpp>(RowName, FString("Error"));

		//UE_LOG(LogTemp, Error, TEXT("ItemHasAu"));

		//�f�[�^�e�[�u������\���̂ɑ���o���Ă���Ίe�A�C�e���̏������s���B
		if (ItemStruct)
		{
			//UE_LOG(LogTemp, Error, TEXT("ItemStructSuccess"));
			//�A�C�e���̏o���ʒu��ݒ�
			FVector ItemSpawnLocation = FVector(ItemStruct->SpawnPosX, 0.0f, 0.0f);
			//ItemSpawnPoint->SetRelativeLocation(ItemSpawnLocation); // �G���[�����������Ă΂�Ȃ�
			SetRelativeLocation(ItemSpawnLocation);
			//UE_LOG(LogTemp, Error, TEXT("SpawnX = %f"), ItemStruct->SpawnPosX);

			//�A�C�e���̑傫����ݒ�
			ItemScale = ItemStruct->SpawnScale;

			//�A�C�R���ݒ�
			DrawIcon = ItemStruct->Icon;
		}

	}
	else
	{
		//�N���C�A���g��
		SpawnSettingRunOnServer();
	}

}


//	�A�C�e���擾���\�b�h
void UItemSpawnComponent::ItemPickup(int ItemNum)
{
	//�T�[�o�����N���C�A���g���ŏ����𕪂���
	if (GetOwner()->HasAuthority())
	{
		//�T�[�o�[���̏���

		//�A�C�e���̐ݒ���s���B
		ItemSetting(ItemNum);
	}
	else
	{
		//�N���C�A���g���̏���

		//�A�C�e���ݒ���T�[�o�Ŏ��s����B
		ItemSettingRunOnServer(ItemNum);
	}
}

void UItemSpawnComponent::SpawnSettingRunOnServer_Implementation()
{
	SpawnSetting();
}

void UItemSpawnComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemSpawnComponent, ItemNumber);
	DOREPLIFETIME(UItemSpawnComponent, ItemClass);
	DOREPLIFETIME(UItemSpawnComponent, DrawIcon);
	DOREPLIFETIME(UItemSpawnComponent, ItemScale);
	DOREPLIFETIME(UItemSpawnComponent, bIsItemUse);
	DOREPLIFETIME(UItemSpawnComponent, BulletItem);
	DOREPLIFETIME(UItemSpawnComponent, SlipItem);
	DOREPLIFETIME(UItemSpawnComponent, SpeedUpItem);

}
