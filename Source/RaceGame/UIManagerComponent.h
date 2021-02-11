// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UIManagerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RACEGAME_API UUIManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUIManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	//----変数
	//ゴール用のUI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
		TSubclassOf<class UUserWidget> GoalWidgetClass;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
		class UUserWidget* GoalWidget;

	//==========================変数
	//UIのインスタンス
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class UUserWidget> GameUiClass;

	//UIの実数を格納する変数
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Display, meta = (AllowPrivateAccess = "true"))
		class UUserWidget *GameUi;

public:
	UFUNCTION()
		void ShowGoalWidget();




		
};
