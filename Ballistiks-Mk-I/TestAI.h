#pragma once
#include "AI.h"

class TestAI : public AI
{
public:
	TestAI( ControllablePlayer& player,const ViewableWorld& view )
		:
		AI( player,view )
	{}
protected:
	virtual void _Process() override
	{
		player.SetThrustVector(
			( view.GetBalls().front().GetCenter() - player.PlayerConst().GetCenter() ).Normalize() );
	}
};

class TestFactory : public AI::Factory
{
public:
	TestFactory( const ViewableWorld& view )
		:
		Factory( view )
	{}
	virtual std::unique_ptr< AI > Make( ControllablePlayer& player ) override
	{
		return std::make_unique< TestAI >( player,view );
	}
};