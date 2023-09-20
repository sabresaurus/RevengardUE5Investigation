// Copyright Sabresaurus Ltd


#include "ScoreSubsystem.h"

void UScoreSubsystem::AddValor(int Delta)
{
	ValorCollected += Delta;
}

void UScoreSubsystem::SetValor(int NewValor)
{
	ValorCollected = NewValor;
}
