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
	
	//���콺 ���� �� Ŀ�� ����
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//input mode ����
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
	//���ε� �� MoveAction �� �� Ÿ���� vector2d
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();

	//��Ʈ�ѷ� ���� ���� �� ����
	const FRotator Rotaion = GetControlRotation();
	const FRotator YawRotaion(0.f, Rotaion.Yaw, 0.f);

	//���� ���� �������� ȸ������ ���� �� �������� ����
	const FVector FowardDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotaion).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		//�Էµ� ���� ���� �������͸� ���� ������ �߰�
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
	* CursorHit ����� ���� case
	*  case A. LastActor == null && ThisActor == null -> nothing
	*  case B. LastActor == null && ThisActor != null -> ThisActor ���̶���Ʈ
	*  case C. LastActor != null && ThisActor == null -> LastActor ���̶���Ʈ ����
	*  case D. (LastActor != null && ThisActor != null) && (LastActor != ThisActor) -> LastActor ���̶���Ʈ ����, ThisActor ���̶���Ʈ
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
