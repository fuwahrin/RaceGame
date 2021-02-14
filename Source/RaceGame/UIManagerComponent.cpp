// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerComponent.h"
#include "Blueprint/UserWidget.h"

////
////ネットワーク同期時にWidgetの所有権周りで間違っていたのでパッケージファイルには使用していない。

// Sets default values for this component's properties
UUIManagerComponent::UUIManagerComponent()
{
	//Tickを走らせない用に変更
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UUIManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//UIが設定されているなら
	if (GameUiClass != nullptr)
	{
		//UIを作成
		GameUi = CreateWidget<UUserWidget>(GetWorld(), GameUiClass);

		//UIが無事に作成されたら表示する。
		if (GameUi != nullptr)
		{
			GameUi->AddToViewport();
		}
	}

	//GoalUIの作成
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


