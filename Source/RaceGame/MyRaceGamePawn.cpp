// Fill out your copyright notice in the Description page of Project Settings.


#include "MyRaceGamePawn.h"
#include "Blueprint/UserWidget.h"
#include "Components/InputComponent.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "RaceGameInstance.h"
#include "ItemSettingComponent.h"


// �R���X�g���N�^
AMyRaceGamePawn::AMyRaceGamePawn()
{
	USkeletalMeshComponent* OwnerMesh = GetMesh();
	OwnerMesh->SetCollisionProfileName(TEXT("MyRacePawn"));

	

	//�I�t���[�h�m�F�p�̃^�C���R���W�����R���|�[�l���g�̐ݒ�
	WheelColliderGroup = CreateDefaultSubobject<USceneComponent>(TEXT("WheelColiderGroup"));
	WheelColliderGroup->SetupAttachment(OwnerMesh);

	//�A�C�e���Ǘ��̃R���|�[�l���g�ݒ�
	ItemSettingComponent = CreateDefaultSubobject<UItemSettingComponent>(TEXT("ItemSettingComponent"));

	float radius = 15.0f;
	ForwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ForwardColision"));
	ForwardCollision->SetupAttachment(WheelColliderGroup);
	ForwardCollision->SetRelativeLocation(FVector(-50.0f, 0.0f, 10.0f));
	ForwardCollision->InitSphereRadius(radius);
	ForwardCollision->SetCollisionProfileName(TEXT("Wheel"));

	BackwardCollision = CreateDefaultSubobject<USphereComponent>(TEXT("BackwardColision"));
	BackwardCollision->SetupAttachment(WheelColliderGroup);
	BackwardCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 10.0f));
	BackwardCollision->InitSphereRadius(radius);
	BackwardCollision->SetCollisionProfileName(TEXT("Wheel"));




	//�ϐ��̏����l�ݒ�
	GamePlayerNum = 2;
	bisTimerStart = false;
	bIsCheckPoint = false;
	GameTimer = 0.0f;
	GoalLap = 3;

	
}

// BeginPlay
void AMyRaceGamePawn::BeginPlay()
{
	Super::BeginPlay();

	//UI���ݒ肳��Ă���Ȃ�
	if (GameUiClass != nullptr)
	{
		//UI���쐬
		GameUi = CreateWidget<UUserWidget>(GetWorld(), GameUiClass);

		//UI�������ɍ쐬���ꂽ��\������B
		if (GameUi != nullptr)
		{
			GameUi->AddToViewport();
		}
	}

	//GoalUI�̍쐬
	if (GoalWidgetClass != nullptr)
	{
		GoalWidget = CreateWidget<UUserWidget>(GetWorld(), GoalWidgetClass);
	}

	//�`�F�b�N�|�C���g�𐔂���
	TArray<AActor*>FindActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), CheckPointActorClass, FindActors);

	//������������ϐ��Ɋi�[����
	CheckPointCounter = FindActors.Num();
	UE_LOG(LogTemp, Log, TEXT("Checkpoint = %d"), CheckPointCounter);

	//�R���C�_�[�C�x���g�̓o�^
	ForwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionBeginOverlap);
	ForwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::ForwardCollisionEndOverlap);
	BackwardCollision->OnComponentBeginOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionBeginOverlap);
	BackwardCollision->OnComponentEndOverlap.AddDynamic(this, &AMyRaceGamePawn::BackwardCollisionEndOverlap);

	//�Q���l���̃J�E���g(GameInstance�ŊǗ����Ă���)
	RaceGameInstance = URaceGameInstance::GetInstance();

	if (RaceGameInstance)
	{
		RaceGameInstance->PlayerCounter++;
	}


}

// Tick
void AMyRaceGamePawn::Tick(float Delta)
{
	Super::Tick(Delta);

	if (RaceGameInstance)
	{
		//�Q���l�����������������
		if (RaceGameInstance->PlayerCounter == GamePlayerNum)
		{
			//���ԃJ�E���g���J�n���Ă��Ȃ����
			if (!bisTimerStart)
			{
				float TimeSpan = 1.0f;
				//���ԃJ�E���g���J�n
				GetWorld()->GetTimerManager().SetTimer(GameTimerCountHandler, this , &AMyRaceGamePawn::GameTimeCounter, TimeSpan, true);
				bisTimerStart = true;
			}

		}
	}
}

// �C���v�b�g�o�^
void AMyRaceGamePawn::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//�A�C�e���o���{�^��
	//�l�b�g���[�N�̌������m�F
	if (this->HasAuthority())
	{
		//�T�[�o�[���Ȃ�}���`�L���X�g
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUseMultiCast);
		
	}
	else
	{
		//�N���C�A���g���Ȃ�T�[�o�Ŏ��s������B
		PlayerInputComponent->BindAction("UseItem", IE_Pressed, this, &AMyRaceGamePawn::ItemUseRunonServer);
	}

	//���X�|�[��
	PlayerInputComponent->BindAction("PlayerRespawn", IE_Pressed, this, &AMyRaceGamePawn::Reapawn);
	
	//�Q�[���I��
	PlayerInputComponent->BindAction("EndGame", IE_Pressed, this, &AMyRaceGamePawn::GameEnd);

	//�^�C�g���ɖ߂�
	PlayerInputComponent->BindAction("GameQuit", IE_Pressed, this, &AMyRaceGamePawn::MoveTitle);
}



//	�X�s�[�h�𒲐�����C�x���g
void AMyRaceGamePawn::SpeedCalcFunction(float SpeedMultipication)
{
	//���ʂ̃x�N�g���Ɉ����Ŏw�肵���l����Z����
	FVector NewVelocity = GetMesh()->GetForwardVector() * SpeedMultipication;
	GetMesh()->SetPhysicsLinearVelocity(NewVelocity);

}

//	�S�[��������s�����\�b�h
void AMyRaceGamePawn::GoalCheck()
{
	//�`�F�b�N�|�C���g�ɓ��B������
	if (bIsCheckPoint)
	{
		//���B���Ă����烉�b�v�����Z
		LapCounter++;

		//���b�v�����S�[�����b�v���ɓ��B���Ă���΃S�[������
		if (LapCounter == GoalLap)
		{
			GoalWidget->AddToViewport();
			bIsGoal = true;
			CurrentCheckPoint = 0;
			APlayerController *PC = Cast<APlayerController>(GetController());
			if (PC)
			{
				PC->bShowMouseCursor = true;
			}

		}
		else
		{
			//���b�v�����S�[�����b�v�ɓ��B���Ă��Ȃ������ꍇ
			//���b�v���B�`�F�b�N��OFF�ɂ���B
			bIsCheckPoint = false;
			
			//�`�F�b�N�|�C���g�����Z�b�g
			CurrentCheckPoint = 0;
		}
		UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
	}
}

//	�`�F�b�N�|�C���g�ʉߎ��ɌĂ΂�郁�\�b�h
void AMyRaceGamePawn::CheckPointRun()
{
	//�ʉ߂����`�F�b�N�|�C���g�𐔂���
	CurrentCheckPoint++;

	//�ʉ߂��������P���ɕK�v�ȃ`�F�b�N�|�C���g���������Ă�����
	if (CurrentCheckPoint >= CheckPointCounter)
	{
		//�P�������L���ɂ���B
		bIsCheckPoint = true;

		//�ʉ߂����`�F�b�N�|�C���g����������
		CurrentCheckPoint = 0;
	}

	UE_LOG(LogTemp, Log, TEXT("CurrentCheckPoint = %d"), CurrentCheckPoint);
}

//	�v���C���[���X�|�[���i�~�ϑ[�u�j
void AMyRaceGamePawn::Reapawn()
{
	//�T�[�o�[���ŌĂ΂�Ă���Ȃ珈�����s��
	if (this->HasAuthority())
	{
		//���X�|�[���ʒu�̒����l
		FVector VectorOffset = FVector(0.0f, 0.0f, 20.0f);
		//���X�|�[���ʒu�̐ݒ�
		FVector RespawnLocation = GetMesh()->GetComponentLocation() + VectorOffset;
		FTransform RespawnTransform = FTransform(RespawnLocation);
		GetMesh()->SetWorldTransform(RespawnTransform, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		//�N���C�A���g���Ȃ�T�[�o�Ŏ��s����悤�ɏ�������
		RespawnRunonServer();
	}
	
}

void AMyRaceGamePawn::RespawnRunonServer_Implementation()
{
	Reapawn();
}

//	�^�C�g����ʂɖ߂�
void AMyRaceGamePawn::MoveTitle()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("MainMenu")), true, TEXT("listen"));
}

//	�Q�[���I��
void AMyRaceGamePawn::GameEnd()
{
	APlayerController *MyController = UGameplayStatics::GetPlayerController(this, 0);
	UKismetSystemLibrary::QuitGame(this ,MyController , EQuitPreference::Quit, false);
}

//�@�^�C���̃R���C�_�[����i�O�ցj��BeginOverlap
void AMyRaceGamePawn::ForwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//�@�^�C���̃R���C�_�[����(�O�ցj��Endlap
void AMyRaceGamePawn::ForwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//�@�^�C���̃R���C�_�[����i��ցj��BeginOverlap
void AMyRaceGamePawn::BackwardCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, true);
	}
}

//�@�^�C���̃R���C�_�[����(��ցj��Endlap
void AMyRaceGamePawn::BackwardCollisionEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag("OffRoad"))
	{
		AccelSetting(OtherComp, false);
	}

}

//�Q�[�����ԃJ�E���g
void AMyRaceGamePawn::GameTimeCounter()
{
	//�T�[�o�݂̂Ŏ��s����B
	if (this->HasAuthority())
	{
		//�S�[�����Ă��Ȃ����
		if (!bIsGoal)
		{
			//���Ԃ��J�E���g
			GameTimer += 1.0f;
		}
	}
}

void AMyRaceGamePawn::ItemUseMultiCast()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemMulticast();
	}
}

void AMyRaceGamePawn::ItemUseRunonServer()
{
	if (ItemSettingComponent != nullptr)
	{
		ItemSettingComponent->SpawnItemRunonServer();
	}
}


void AMyRaceGamePawn::CrashEvent()
{
	
	const float SpeedMultipication = 0.0f;

	//0����Z���邱�Ƃɂ���ăX�s�[�h���Ȃ����B
	SpeedCalcFunction(SpeedMultipication);

	//���͒l�Ɋ|���Ă���l��0�ɂ���B�i���͂��Ă��i�܂��Ȃ��ׁj
	InputCalc = 0.0f;

	//�A�C�e�����g�p�ł��Ȃ��悤�ɂ���A
	//bIsItemUse = false;
	ItemSettingComponent->SetItemUse(true);


	//��莞�Ԋ�������͒l�Ɋ|���Ă���l�����Ƃɖ߂�
	float DelayTime = 1.0f;
	GetWorld()->GetTimerManager().SetTimer(BrakeTimeHandler, this, &AMyRaceGamePawn::InputReset, DelayTime ,  false);
	

}

void AMyRaceGamePawn::InputReset()
{
	InputCalc = 1.0f;
	//bIsItemUse = true;
	ItemSettingComponent->SetItemUse(true);
}


void AMyRaceGamePawn::AccelSetting(UPrimitiveComponent* HitComponent, bool bIsSpeedDown)
{
	//UE_LOG(LogTemp, Log, TEXT("AccleSettingOverlap"));

	//�u���[�L�l�ɉ��Z���Z������l
	const float BrakeValue = 0.25f;

	//�ő�̃C���v�b�g�l
	const float InputCalcMax = 1.0f;


	//�������邩�ǂ����H
	if (bIsSpeedDown)
	{
		//�������s���ꍇ�͌����l�Ƃ��ău���[�L�l�������l�ɉ�����
		DownSpeedOffset += BrakeValue;
	}
	else
	{
		//��������߂�ꍇ�̓u���[�L�l�������l�������
		DownSpeedOffset += (BrakeValue * -1);
	}

	//�C���v�b�g�l�Ƀu���[�L�l�̌v�Z��������
	InputCalc = InputCalcMax - DownSpeedOffset;

}


void AMyRaceGamePawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyRaceGamePawn, GameTimer);

}


