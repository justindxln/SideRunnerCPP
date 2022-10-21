#pragma once

UENUM()
enum  class EDamageType : uint8 
{
	Spike,
	Wall,
};


UENUM()
enum  class EGameState : uint8
{
	PreGame,
	Active,
	PostGame,
};