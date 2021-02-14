// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerComponent.h"
#include "Blueprint/UserWidget.h"

////
////�l�b�g���[�N��������Widget�̏��L������ŊԈ���Ă����̂Ńp�b�P�[�W�t�@�C���ɂ͎g�p���Ă��Ȃ��B

// Sets default values for this component's properties
UUIManagerComponent::UUIManagerComponent()
{
	//Tick�𑖂点�Ȃ��p�ɕύX
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

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
	
}


// Called every frame
void UUIManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UUIManagerComponent::ShowGoalWidget()
{
	GoalWidget->AddToViewport();
}


