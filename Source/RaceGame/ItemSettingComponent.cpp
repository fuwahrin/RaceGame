// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSettingComponent.h"
#include "Net/UnrealNetwork.h"
#include "ItemRefActor.h"
#include "MyCartMoveComponentReplicator.h"

// Sets default values for this component's properties
UItemSettingComponent::UItemSettingComponent()
{
	//Tick�𖳌�
	PrimaryComponentTick.bCanEverTick = true;


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

	MoveReplicateComponent = GetOwner()->FindComponentByClass<UMyCartMoveComponentReplicator>();


	
	
}


// Called every frame
void UItemSettingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//UE_LOG(LogTemp, Warning, TEXT("%s:ServerItemState:%d") , *GetRole(GetOwner()->GetRemoteRole()) , ServerItemState.IsUse);
	if (ServerItemState.IsUse)
	{
		if(GetOwner()->GetRemoteRole() == ROLE_Authority)
		//if(GetOwnerRole() == ROLE_Authority)
		{
			Server_SendItemSpawn(ServerItemState.ItemNum);
			UE_LOG(LogTemp, Warning, TEXT("%s:ItemUse:%d"), *GetRole(GetOwnerRole()) , ServerItemState.ItemNum);

		}
		ServerItemState.IsUse = false;
	}

	
}

// �A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnItem()
{
	if (OwnerPawn != nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("bIsItemUse = %d"), bIsItemUse);
		//�A�C�e�����g�p�ł����Ԃ��m�F
		if(LocalItemState.IsUse)
		{
			
			ServerItemState = LocalItemState;
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

				////�T�[�o���̑���҂̂�
				if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					UE_LOG(LogTemp, Warning, TEXT("Server"));
					
				}

				//�N���C�A���g���̑���҂̂�
				if (GetOwnerRole() == ROLE_AutonomousProxy)
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					//UE_LOG(LogTemp, Warning, TEXT("Cliant"));
				}
				

				//UE_LOG(LogTemp, Warning, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
				//UE_LOG(LogTemp, Warning, TEXT("ItemSpawnPoint%f%f%f"), ItemSpawnPoint->GetRelativeLocation().X, ItemSpawnPoint->GetRelativeLocation().Y, ItemSpawnPoint->GetRelativeLocation().Z);

				ItemUse();

			}
		}
	}
}


void UItemSettingComponent::Server_SendItemSpawn_Implementation(int itemnum)
{
	
	ItemCreate(itemnum);

	FActorSpawnParameters SpawnParams;

	//�I�[�i�[
	SpawnParams.Owner = this->GetOwner();


	//�X�|�[���ʒu�̐ݒ�(ItemSpawnPoint�̃��[���h���W���i�[)
	FVector SpawnLocation = ItemSpawnPoint->GetComponentLocation();
	FRotator SpawnRotation = ItemSpawnPoint->GetComponentRotation();

	//�X�|�[������ۂ�Scale
	FVector SpawnScale = FVector(ItemScale);

	AActor* const SpawnItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
	SpawnItem->SetActorScale3D(SpawnScale);

	UE_LOG(LogTemp, Warning, TEXT("SpawnServerSend"));
}

bool UItemSettingComponent::Server_SendItemSpawn_Validate(int itemnum)
{
	return true;
}



void UItemSettingComponent::ItemCreate(int32 ItemNum)
{
	switch (ItemNum)
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

	//FName RowName = FName(FString::FromInt(ItemNumber));
	FName RowName = FName(FString::FromInt(ItemNum));

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
		//�A�C�e���̑傫����ݒ�
		ItemScale = ItemStruct->SpawnScale;

		//�A�C�R���ݒ�
		//�R���g���[�����Ă���Pawn�Ȃ�A�C�R���̐ݒ���s���B
		if (LocalItemState.IsUse)
		{
			DrawIcon = ItemStruct->Icon;
		}
	}
}



// �A�C�e�����g�p���郁�\�b�h
void UItemSettingComponent::ItemUse()
{
	//�A�C�e�����g�p�������Ƃ�ݒ�

	//UI�̉摜�������B
	DrawIcon = nullptr;

	
	//�A�C�e���g�p�o���Ȃ���ԂɕύX
	LocalItemState.IsUse = false;

	//�A�C�e��ID��������
	LocalItemState.ItemNum = 0;
	
}



//	�A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnSetting()
{

	//�A�C�e��No��
	int itemnum = LocalItemState.ItemNum;
	ItemCreate(itemnum);

}


//	�A�C�e���擾���\�b�h
void UItemSettingComponent::ItemPickup(int ItemNum)
{
	ItemSetting(ItemNum);
}


//�A�C�e���̐ݒ���s�����\�b�h
void UItemSettingComponent::ItemSetting(int32 ItemNum)
{
	//���[�J���̃X�e�[�^�X�ϐ���ID
	LocalItemState.ItemNum = ItemNum;

	//�A�C�e���g�p�\��
	LocalItemState.IsUse = true;


	//UE_LOG(LogTemp, Warning, TEXT("%s:Itemnum = %d"), *GetRole(GetOwnerRole()), LocalItemState.ItemNum);

	//�A�C�e���o���̐ݒ���s���B
	SpawnSetting();

}

void UItemSettingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UItemSettingComponent, ServerItemState);

}


FString UItemSettingComponent::GetRole(ENetRole role)
{
	switch (role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	case ROLE_MAX:
		return "ROLE_MAX";
	default:
		return "ERROR";
	}

}