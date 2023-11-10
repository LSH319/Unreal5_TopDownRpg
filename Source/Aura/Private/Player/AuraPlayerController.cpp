// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	CursorTrace();
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);
	
	//마우스 보임 및 커서 설정
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//input mode 설정
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	//바인딩 된 MoveAction 의 값 타입이 vector2d
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	//컨트롤러 기준 정면 값 추출
	const FRotator Rotaion = GetControlRotation();
	const FRotator YawRotaion(0.f, Rotaion.Yaw, 0.f);

	//정면 값을 기준으로 회전백터 생성 및 단위백터 추출
	const FVector FowardDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//입력된 값과 계산된 단위백터를 통해 움직임 추가
		ControlledPawn->AddMovementInput(FowardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
	
	/**
	* CursorHit 결과에 따른 case
	*  case A. LastActor == null && ThisActor == null -> nothing
	*  case B. LastActor == null && ThisActor != null -> ThisActor 하이라이트
	*  case C. LastActor != null && ThisActor == null -> LastActor 하이라이트 해제
	*  case D. (LastActor != null && ThisActor != null) && (LastActor != ThisActor) -> LastActor 하이라이트 해제, ThisActor 하이라이트
	*  case E. (LastActor != null && ThisActor != null) && (LastActor == ThisActor) -> nothing
	*/

	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr) // Case B
		{
			ThisActor->HighlightActor();
		}
		// Case A
	}
	else
	{
		if (ThisActor == nullptr) // Case C
		{
			LastActor->UnHighlightActor();
		}
		else
		{
			if (LastActor != ThisActor) // Case D
			{
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			// Case E
		}
	}
}
