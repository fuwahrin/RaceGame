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
		//if(GetOwner()->GetRemoteRole() == ROLE_Authority)
		//{
			Server_SendItemSpawn(ServerItemState.ItemNum);
			UE_LOG(LogTemp, Warning, TEXT("%s:ItemUse:%d"), *GetRole(GetOwnerRole()) , ServerItemState.ItemNum);

		//}
		ServerItemState.IsUse = false;
	}

	
}



void UItemSettingComponent::SpawnItemMulticast_Implementation()
{
	SpawnItem();
	//UE_LOG(LogTemp, Warning, TEXT("Server: %s") ,*GetRole(GetOwnerRole()));
	
}


void UItemSettingComponent::SpawnItemRunonServer_Implementation()
{
	SpawnItem();
	//UE_LOG(LogTemp, Warning, TEXT("Cliant: %s"), *GetRole(GetOwner()->GetRemoteRole()));
}

// �A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnItem()
{
	if (OwnerPawn != nullptr && MoveReplicateComponent != nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("bIsItemUse = %d"), bIsItemUse);
		//�A�C�e�����g�p�ł����Ԃ��m�F
		//if (bIsItemUse)
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

				//AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
				//SpawnItem->SetActorScale3D(SpawnScale);

				//�T�[�o���̑���҂̂�
				if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					//UE_LOG(LogTemp, Warning, TEXT("Server"));
					
				}
				
				//�N���C�A���g���̑���҂̂�
				//if (GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
				if(GetOwnerRole() == ROLE_AutonomousProxy)
				{
					AActor* const SpawnItem = World->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, SpawnParams);
					SpawnItem->SetActorScale3D(SpawnScale);
					//UE_LOG(LogTemp, Warning, TEXT("Cliant"));
				}

				//UE_LOG(LogTemp, Error, TEXT("SpawnLocation%f%f%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
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

	//�A�C�e���g�p�\�ϐ���False
	//bIsItemUse = false;

	//�A�C�e��ID��������
	//ItemNumber = 0;

	LocalItemState.IsUse = false;
	LocalItemState.ItemNum = 0;

	//ServerItemState = LocalItemState;
	
}



//	�A�C�e�����o�������郁�\�b�h
void UItemSettingComponent::SpawnSetting()
{
	
	//�l�b�g���[�N�̌����ɂ���ď�����ς���
	//if (GetOwner()->HasAuthority())
	//{
		//UE_LOG(LogTemp, Error, TEXT("%s:SpawnSetting:ItemNumber = %d"),GetOwner()->GetRemoteRole(), ItemNumber);

		//�T�[�o��

		//ItemClass��ID�ɂ���Đݒ肷��B
		//-----TOOD:�O���t�@�C������ݒ肩�����ŃA�Z�b�g��ǂݍ��ނ悤�ɕύX����B
		//switch (ItemNumber)
		//{
		//case 1:
		//	ItemClass = BulletItem;
		//	break;
		//case 2:
		//	ItemClass = SlipItem;
		//	break;
		//case 3:
		//	ItemClass = SpeedUpItem;
		//	
		//	break;
		//}
		//
		////FName RowName = FName(FString::FromInt(ItemNumber));
		//FName RowName = FName(FString::FromInt(ServerItemState.ItemNum));
		//
		//FItemStructCpp* ItemStruct = ItemDataTable->FindRow<FItemStructCpp>(RowName, FString("Error"));
		//
		//
		////�f�[�^�e�[�u������\���̂ɑ���o���Ă���Ίe�A�C�e���̏������s���B
		//if (ItemStruct)
		//{
		//	//�A�C�e���̏o���ʒu��ݒ�
		//	FVector ItemSpawnLocation = FVector(ItemStruct->SpawnPosX, 0.0f, 0.0f);
		//	if (ItemSpawnPoint->GetAttachParent() != nullptr)
		//	{
		//		ItemSpawnPoint->SetRelativeLocation(ItemSpawnLocation);
		//	}
		//	else
		//	{
		//		UE_LOG(LogTemp, Error, TEXT("ItemSpawnNotParent"));
		//	}
		//	//�A�C�e���̑傫����ݒ�
		//	ItemScale = ItemStruct->SpawnScale;
		//
		//	//�A�C�R���ݒ�
		//	//�R���g���[�����Ă���Pawn�Ȃ�A�C�R���̐ݒ���s���B
		//	if (GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
		//	{
		//		DrawIcon = ItemStruct->Icon;
		//	}
		//	//UE_LOG(LogTemp, Error, TEXT("Icon = %s") , *ItemStruct->Icon->GetName());
		//
		//}
	//}
	//else
	//{
	//	//�N���C�A���g��
	//	SpawnSettingRunOnServer();
	//}

	//TODO:test
	//�N���C�A���g���ő��삵�Ă���Pawn�̓������T�[�o�[�ɕ���������
		UE_LOG(LogTemp, Error, TEXT("Role=%s"), *GetRole(GetOwnerRole()));

		if (GetOwnerRole() == ROLE_AutonomousProxy)
		{
			UE_LOG(LogTemp, Error, TEXT("ROLE_AutonomousProxy"));
			int itemnum = LocalItemState.ItemNum;
			ItemCreate(itemnum);
		
		}

		//�T�[�o�[���Ŏ��g�����삷��Pawn�̓������N���C�A���g���ɕ���������
		//��RemoteRole�̓}�b�v�J�ڂ���Ɛ��������L�҂��擾�ł��Ȃ��׎g�p����
		//�Q�l�Fhttps://docs.unrealengine.com/en-US/InteractiveExperiences/Networking/Actors/Roles/index.html
		if (GetOwner()->GetLocalRole() == ROLE_Authority && GetOwner()->GetInstigator<APawn>()->IsLocallyControlled())
		{
			UE_LOG(LogTemp, Error, TEXT("ROLE_Authority"));
			int itemnum = LocalItemState.ItemNum;
			ItemCreate(itemnum);

		}

		//�N���C�A���g���ŕ�������Ă���Pawn�Ȃ�
		if (GetOwnerRole() == ROLE_SimulatedProxy)
		{
			UE_LOG(LogTemp, Error, TEXT("ROLE_SimulatedProxy"));
			int itemnum = LocalItemState.ItemNum;
			ItemCreate(itemnum);
		}

		//if (GetOwnerRole() == ROLE_Authority)
		//{
		//	UE_LOG(LogTemp, Error, TEXT("ROLE_Authority"));
		//	int itemnum = ServerItemState.ItemNum;
		//	ItemCreate(itemnum);
		//}
}


void UItemSettingComponent::SpawnSettingRunOnServer_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Cliant:SpawnSetting"));
	SpawnSetting();
}



//	�A�C�e���擾���\�b�h
void UItemSettingComponent::ItemPickup(int ItemNum)
{
	//�T�[�o�����N���C�A���g���ŏ����𕪂���
	//if (GetOwner()->HasAuthority())
	//{
	//	//�T�[�o�[���̏���
	//
	//	//�A�C�e���̐ݒ���s���B
	//	ItemSetting(ItemNum);
	//}
	//else
	//{
	//	//�N���C�A���g���̏���
	//	UE_LOG(LogTemp, Warning, TEXT("PickUp"));
	//	//�A�C�e���ݒ���T�[�o�Ŏ��s����B
	//	ItemSettingRunOnServer(ItemNum);
	//}

	ItemSetting(ItemNum);
}


//�A�C�e���̐ݒ���s�����\�b�h
void UItemSettingComponent::ItemSetting(int32 ItemNum)
{
	//�A�C�e����ID��ݒ�
	//ItemNumber = ItemNum;
	
	//UE_LOG(LogTemp, Warning, TEXT("ItemSetting"));
	
	//�o������ۂ̐ݒ�
	//SpawnSetting();
	
	//�A�C�e�����g�p�ł����ԂɕύX
	//bIsItemUse = true;

	//UE_LOG(LogTemp, Warning, TEXT("bIsItemUse = %d") , bIsItemUse);

	//TODO:test
	LocalItemState.ItemNum = ItemNum;
	LocalItemState.IsUse = true;

	UE_LOG(LogTemp, Warning, TEXT("%s:Itemnum = %d"), *GetRole(GetOwnerRole()), LocalItemState.ItemNum);
	//�o������ۂ̐ݒ�
	SpawnSetting();



}

void UItemSettingComponent::ItemSettingRunOnServer_Implementation(int32 ItemNum)
{
	ItemSetting(ItemNum);
	UE_LOG(LogTemp, Warning, TEXT("ItemSettingRunOnServer"));

}


void UItemSettingComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//DOREPLIFETIME(UItemSettingComponent, ItemNumber);
	//DOREPLIFETIME(UItemSettingComponent, ItemClass);
	//DOREPLIFETIME(UItemSettingComponent, DrawIcon);
	//DOREPLIFETIME(UItemSettingComponent, ItemScale);
	//DOREPLIFETIME(UItemSettingComponent, bIsItemUse);


	//DOREPLIFETIME(UItemSettingComponent, BulletItem);
	//DOREPLIFETIME(UItemSettingComponent, SlipItem);
	//DOREPLIFETIME(UItemSettingComponent, SpeedUpItem);

	//TODO:test
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