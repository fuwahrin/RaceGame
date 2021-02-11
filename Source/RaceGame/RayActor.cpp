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

	RayTrace();

}

void ARayActor::RayTrace()
{
	if (RootComponent != nullptr)
	{
		FVector StartPoint = RootComponent->GetComponentLocation();
		FVector EndPoint = StartPoint - FVector(0.0f, 0.0f, 50.0f);

		FHitResult hit;
		FCollisionQueryParams query;
		query.AddIgnoredActor(this);
		DrawDebugLine(GetWorld(), StartPoint, EndPoint, FColor::Red, false, 1.0f, 0, 1);
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

