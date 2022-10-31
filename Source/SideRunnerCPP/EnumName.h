#pragma once

UENUM()
enum  class EDamageType : uint8 
{
	Spike,
	Wall,
};

UENUM()
enum  class EPowerUpType : uint8
{
	Healing,
	Shield,
	Health,
	Speed,
	ScoreBoost,
};

UENUM()
enum  class EGameState : uint8
{
	PreGame,
	Active,
	PostGame,
};