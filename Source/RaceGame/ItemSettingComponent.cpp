// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSettingComponent.h"
#include "Net/UnrealNetwork.h"
#include "ItemRefActor.h"


// Sets default values for this component's properties
UItemSettingComponent::UItemSettingComponent()
{
	//Tick�𖳌�
	PrimaryComponentTick.bCanEverTick = false;


	//�ϐ��̏�����
	bIsItemUse = false;

	//���̃R���|�[�l���g�̓�����L���ɂ���
	SetIsReplicatedByDefault(true);


	//�f�[�^�e�[�u���̏�����
	static ConstructorHelpers::FObjectFinder<UDataTable>TableDataAsset(TEXT("DataTable'/Game/Developers/fuwarrin/Collections/BP/ImportData/RaceGameItemListCPP.RaceGameItemListCPP'"));
	if (TableDataAsset.Succeeded())
	{
		ItemDataTable = TableDataAsset.Object;
	}

	ItemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ItemSpawnPoint"));
	
}


// Called when the game starts
void UItemSettingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//���L�҂�Pawn���擾
	OwnerPawn = GetOwner()->GetInstigator<APawn>();

	
	
}


// Called every frame
void UItemSettingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UItemSettingComponent::SpawnItemMulticast_Implementation()
{
	SpawnItem();
}


void UItemSettingComponent::SpawnItemRunonServer_Implementation()
{
	SpawnItem();
}

// �A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnItem()
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
				FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
				FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();
				
				//�X�|�[������ۂ�Scale
				FVector SpawnScale = FVector(ItemScale);

				//�A�C�e�����X�|�[������B
				AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
				SpawnItem->SetActorScale3D(SpawnScale);

				//UE_LOG(LogTemp, Error, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
				UE_LOG(LogTemp, Warning, TEXT("ItemSpawnPoint%f%f%f"), ItemSpawnPoint->GetRelativeLocation().X, ItemSpawnPoint->GetRelativeLocation().Y, ItemSpawnPoint->GetRelativeLocation().Z);

				ItemUse();

			}
		}
	}
}


//RPC�Ăяo�����ؗp�̃��\�b�h
//��false��Ԃ���RPC�����s����Ȃ�
bool UItemSettingComponent::SpawnItemRunonServer_Validate()
{
	return true;
}

// �A�C�e�����g�p���郁�\�b�h
void UItemSettingComponent::ItemUse()
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
void UItemSettingComponent::ItemSetting(int32 ItemNum)
{
	//�A�C�e����ID��ݒ�
	ItemNumber = ItemNum;


	//�o������ۂ̐ݒ�
	SpawnSetting();

	//�A�C�e�����g�p�ł����ԂɕύX
	bIsItemUse = true;
}

void UItemSettingComponent::ItemSettingRunOnServer_Implementation(int32 ItemNum)
{
	ItemSetting(ItemNum);
}


//	�A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnSetting()
{
	//UE_LOG(LogTemp, Error, TEXT("SpawnSetting"));
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

		
		//�f�[�^�e�[�u������\���̂ɑ���o���Ă���Ίe�A�C�e���̏������s���B
		if (ItemStruct)
		{
			//�A�C�e���̏o���ʒu��ݒ�
			FVector ItemSpawnLocation = FVector(ItemStruct->SpawnPosX, 0.0f, 0.0f);
			if (ItemSpawnPoint->GetAttachParent() != nullptr)
			{
				ItemSpawnPoint->SetRelativeLocation(ItemSpawnLocation);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ItemSpawnNotParent"));
			}
			//UE_LOG(LogTemp, Error, TEXT("SpawnX = %f") , ItemStruct->SpawnPosX);

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
void UItemSettingComponent::ItemPickup(int ItemNum)
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

void UItemSettingComponent::SpawnSettingRunOnServer_Implementation()
{
	SpawnSetting();
}

void UItemSettingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UItemSettingComponent, ItemNumber);
	DOREPLIFETIME(UItemSettingComponent, ItemClass);
	DOREPLIFETIME(UItemSettingComponent, DrawIcon);
	DOREPLIFETIME(UItemSettingComponent, ItemScale);
	DOREPLIFETIME(UItemSettingComponent, bIsItemUse);
	DOREPLIFETIME(UItemSettingComponent, BulletItem);
	DOREPLIFETIME(UItemSettingComponent, SlipItem);
	DOREPLIFETIME(UItemSettingComponent, SpeedUpItem);

}

