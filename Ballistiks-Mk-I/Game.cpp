/****************************************************************************************** 
 *	Chili DirectX Framework Version 14.03.22											  *	
 *	Game.cpp																			  *
 *	Copyright 2014 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "Game.h"

Game::Game( HWND hWnd,KeyboardServer& kServer,MouseServer& mServer )
:	gfx( hWnd ),
	audio( hWnd ),
	kbd( kServer ),
	mouse( mServer ),
	vp( gfx,gfx.GetScreenRect() ),
	pWorld( std::make_unique< World >( kbd,vp,obs ) ),
	cam( vp,vp.GetWidth(),vp.GetHeight(),{ vp.GetWidth() / 2.0f,vp.GetHeight() / 2.0f } ),
	dick(TriangleStrip::ExtractSolidStripCollection(PolyClosed("shipd.dxf"))),
	whistle(audio.CreateSound("whistle.wav")),
	batman(audio.CreateSound("batman.wav")),
	obs( whistle )
{
}

Game::~Game()
{
}

void Game::Go()
{
	UpdateModel();
	gfx.BeginFrame();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel( )
{
	const float dt = 1.0f / 60.0f;
	if( !transitioning )
	{
		pWorld->Step( dt );
		obs.Step( dt );

		if( obs.GoalScored() )
		{
			transitioning = true;
			batman.Play();
		}
	}
	else
	{
		transitionTime += dt;
		if( transitionTime >= transitionDuration )
		{
			transitionTime = 0.0f;
			transitioning = false;
			pWorld = std::make_unique< World >( kbd,vp,obs );
			obs.Reset();
			cam.SetZoom( 1.0f );
		}
	}
}

void Game::ComposeFrame()
{
	if( !transitioning )
	{
		pWorld->Render( cam );
	}
	else
	{
		const float theta = ( transitionTime / transitionDuration ) * 2.0f * PI * 4.0f;
		cam.SetZoom( 0.7f + 0.4f * -sin( theta ) );
		pWorld->Render( cam );
		for( const TriangleStrip& s : dick )
		{
			vp.Draw( s.GetDrawable(
				Mat3::Translation( { vp.GetWidth() / 2.0f,vp.GetHeight() / 2.0f } ) *
				Mat3::Rotation( theta * 0.5f + PI / 2.3f ) *
				Mat3::Scaling( 5.0f ),GREEN ) );
		}
		for( const TriangleStrip& s : dick )
		{
			vp.Draw( s.GetDrawable(
				Mat3::Translation( { vp.GetWidth() / 2.0f,vp.GetHeight() / 2.0f } ) *
				Mat3::Rotation( -theta * 1.0f + PI / 2.3f ) *
				Mat3::Scaling( 5.0f ),BROWN ) );
		}
		for( const TriangleStrip& s : dick )
		{
			vp.Draw( s.GetDrawable(
				Mat3::Translation( { vp.GetWidth() / 2.0f,vp.GetHeight() / 2.0f } ) *
				Mat3::Rotation( theta * 1.5f ) *
				Mat3::Scaling( 5.0f ),PURPLE ) );
		}
	}
}
