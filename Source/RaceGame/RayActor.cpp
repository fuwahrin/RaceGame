// Fill out your copyright notice in the Description page of Project Settings.


#include "RayActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"


// Sets default values
ARayActor::ARayActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARayActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARayActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RayTrace(false);

}

void ARayActor::RayTrace(bool bIsDebugDraw)
{
	if (RootComponent != nullptr)
	{
		//レイの始点と終点の設定
		FVector StartPoint = RootComponent->GetComponentLocation();
		FVector EndPoint = StartPoint - FVector(0.0f, 0.0f, 50.0f);

		FHitResult hit;

		//あたり判定のパラメータ
		FCollisionQueryParams query;

		//自身に当たらない用にする。
		query.AddIgnoredActor(this);

		//デバッグ描画
		if (bIsDebugDraw)
		{
			DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 1.0f, 0, 1);
		}

		//レイトレース
		if (GetWorld()->LineTraceSingleByChannel(hit, StartPoint, EndPoint, ECC_Visibility, query))
		{
			if (IsValid(hit.GetActor()))
			{
				FString Name = hit.GetActor()->GetFName().ToString();
				//ヒットしたコンポーネントのタグを取得
				CurrentState = hit.GetComponent()->ComponentTags[0];
				//UE_LOG(LogTemp, Error, TEXT("HitName = %s") , *Name);
				//UE_LOG(LogTemp, Warning, TEXT("%s"), *CurrentState.ToString());
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("RootComponentError"));

	}


}

