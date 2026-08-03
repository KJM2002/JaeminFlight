// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyRocket.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;

UCLASS()
class JAEMIN_FLIGHT_API AMyRocket : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyRocket();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void ProcessActorBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void ProcessActorHit(AActor* SelfActor, AActor* OtherActor,
		FVector NormalImpulse, const FHitResult& Hit);

	// BP_Rocket의 ProjectileMovement에 적용할 직선 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
	float RocketSpeed = 1500.0f;

	// 발사 위치로부터 이 거리만큼 날아가면 사라진다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
	float MaxDistance = 5000.0f;

	// 충돌 크기는 유지하고 로켓 메시만 보기 좋게 키운다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rocket")
	float RocketVisualScale = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<UNiagaraSystem> ExplosionTemplate;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TObjectPtr<USoundBase> ExplosionSound;

	UFUNCTION(BlueprintImplementableEvent)
	void CallCPPExecuteBP();

	UFUNCTION(BlueprintNativeEvent)
	void CallCPPExecuteBPDefault();
	void CallCPPExecuteBPDefault_Implementation();

private:
	void Explode(AActor* OtherActor, const FVector& ExplosionLocation);

	FVector StartLocation = FVector::ZeroVector;
	bool bExploded = false;
};
