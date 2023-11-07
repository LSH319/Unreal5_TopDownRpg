// Copyright Druid Mechanics


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
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
