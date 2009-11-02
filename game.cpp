//==============================================================================

#include <game.hpp>
#include <engine.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>
#include <viewport.hpp>

#include <hgeresource.h>
#include <hgefont.h>

#include <algorithm>
#include <set>

//==============================================================================
Game::Game()
    :
    Context()
{
}

//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Game::init()
{
    HGE * hge( Engine::hge() );
    b2World * b2d( Engine::b2d() );
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    Engine::em()->init();

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 1280.0f;
    vp->bounds().y = 720.0f;
    vp->setAngle( 0.0f );
    vp->setScale( 10.0f );
    
}

//------------------------------------------------------------------------------
void
Game::fini()
{
    Engine::em()->fini();
}

//------------------------------------------------------------------------------
bool
Game::update( float dt )
{
    const Controller & pad( Engine::instance()->getController() );
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );

    if ( Engine::instance()->isPaused() )
    {
        return false;
    }

    return false;
}

//------------------------------------------------------------------------------
void
Game::render()
{
    hgeResourceManager * rm( Engine::rm() );
    hgeFont * font( rm->GetFont( "love" ) );
    ViewPort * vp( Engine::vp() );

    hgeSprite * paper( rm->GetSprite( "paper" ) );

    vp->setTransform();

    paper->RenderEx( 0, 50, 0.0f, 0.15f );

    font->SetColor( 0xF0000000 );
    font->SetScale( 0.1f );
    for ( int i = 0; i < 10; ++i )
    {
        font->printf( -50.0f, -9.5f + 4.75f * i, HGETEXT_LEFT,
            "All *WORK* and no _PLAY_ makes 'Jack' a dull boy!?!");
    }
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------

//==============================================================================
