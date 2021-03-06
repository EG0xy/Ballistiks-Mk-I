#include "Presentator.h"


#pragma region GameOnState
void Presentator::GameOnState::Observe()
{
	if( parent.perObs->IsSet() )
	{
		if( parent.manager->GetPeriod() < 1 )
		{
			parent.Transition( std::make_unique<PeriodEndState>( parent ) );
		}
		else
		{
			parent.Transition( std::make_unique<GameEndState>( parent ) );
		}
	}
	else if( parent.ptObs->IsSet() )
	{
		parent.ptObs->Reset();
		parent.Transition( std::make_unique<GoalScoredState>( parent ) );
	}
}

void Presentator::GameOnState::Step( float dt )
{
	parent.manager->Step( dt );
}

void Presentator::GameOnState::Draw( DrawTarget& tgt )
{
	parent.manager->RenderWorld( tgt );
}
#pragma endregion


#pragma region BeginTitleState
void Presentator::BeginTitleState::OnCountdownFinished()
{
	parent.Transition( std::make_unique< GameOnState >( parent ) );
}

void Presentator::BeginTitleState::Draw( DrawTarget& tgt )
{
	parent.manager->RenderWorld( tgt );
	tgt.Draw( txtA );
	tgt.Draw( txtB );
	tgt.Draw( vs );
}
#pragma endregion


#pragma region GoalScoredState
void Presentator::GoalScoredState::Draw( DrawTarget& tgt )
{
	parent.manager->RenderWorld( tgt );
	tgt.Draw( scoreText );
}

void Presentator::GoalScoredState::OnCountdownFinished()
{
	parent.manager->StartNewPoint();
	parent.Transition( std::make_unique< GameOnState >( parent ) );
}

void Presentator::GoalScoredState::_Step( float dt )
{
	parent.manager->Step( dt );
}
#pragma endregion


#pragma region PeriodEndState
void Presentator::PeriodEndState::Draw( DrawTarget& tgt )
{
	parent.manager->RenderWorld( tgt );
	tgt.Draw( endText );
}

void Presentator::PeriodEndState::OnCountdownFinished()
{
	parent.manager->StartNewPeriod();
	parent.perObs->Reset();
	parent.Transition( std::make_unique< GameOnState >( parent ) );
}

void Presentator::PeriodEndState::_Step( float dt )
{
	parent.manager->Step( dt );
}
#pragma endregion


#pragma region GameEndState
void Presentator::GameEndState::Draw( DrawTarget& tgt )
{
	parent.manager->RenderWorld( tgt );
	tgt.Draw( endText );
}

void Presentator::GameEndState::Step( float dt )
{
	parent.manager->Step( dt );
}
#pragma endregion


#pragma region GameSimulatingState
void Presentator::GameSimulatingState::Observe()
{
	if( parent.perObs->IsSet() )
	{
		parent.perObs->Reset();
		if( parent.manager->GetPeriod() < 1 )
		{
			parent.manager->StartNewPeriod();
			parent.Transition( std::make_unique<GameSimulatingState>( parent ) );
		}
		else
		{
			parent.overEvent.OnGameOver();
			// startsimulation already transitions this bs for us...
			// maybe separate into createsimulation and start simulation...?
			//parent.Transition( std::make_unique<NullState>( parent ) );
		}
	}
	else if( parent.ptObs->IsSet() )
	{
		parent.ptObs->Reset();
		parent.manager->StartNewPoint();
		parent.Transition( std::make_unique<GameSimulatingState>( parent ) );
	}
}

void Presentator::GameSimulatingState::Step( float dt )
{
	parent.manager->Step( dt );
}
#pragma endregion