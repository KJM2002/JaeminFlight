// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

UCLASS()
class JAEMIN_FLIGHT_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMyPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UStaticMeshComponent* Body;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UStaticMeshComponent* Left;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UStaticMeshComponent* Right;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Components", BlueprintReadOnly)
	UFloatingPawnMovement* Movement;

	// 비행 속도는 블루프린트에서도 조절할 수 있다.
	UPROPERTY(EditAnywhere, Category = "Flight", BlueprintReadWrite)
	float NormalSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Flight", BlueprintReadWrite)
	float BoostSpeed = 2000.0f;


	UFUNCTION(BlueprintCallable, meta=(DisplayName="이건 호출 하는거"))
	void CallBlueprint(int Money, FString Name);


	void Fire();
	void Pitch(float Value);
	void Yaw(float Value);
	void Roll(float Value);
	void BoostOn();
	void BoostOff();

	UPROPERTY(EditAnywhere, Category = "Data", BlueprintReadWrite)
	TSubclassOf<class AMyRocket> RocketTemplate;
};
