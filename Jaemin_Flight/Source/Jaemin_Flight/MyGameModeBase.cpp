// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPlayerController.h"
#include "MyPawn.h"

AMyGameModeBase::AMyGameModeBase()
{
	//CDO 초기화 할때 쓰는 함수
	UE_LOG(LogTemp, Warning, TEXT("Hello World"));

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, TEXT("Hello World"));
	}


	//클래스 이름으로 저장하고 싶어요.
	//Class Default Object
	DefaultPawnClass = AMyPawn::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();

}
