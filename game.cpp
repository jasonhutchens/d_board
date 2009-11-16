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
    Context(),
    m_brain( 5 )
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
    
    m_brain.clear();
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

    // Call m_brain.setMode based on modifier keys / buttons
    Brain::Mode ctrl( hge->Input_GetKeyState( HGEK_CTRL )
                      ? Brain::MODE_ALT : Brain::MODE_NORMAL );
    Brain::Mode shift( hge->Input_GetKeyState( HGEK_SHIFT )
                      ? Brain::MODE_SHIFT : Brain::MODE_NORMAL );
    m_brain.setMode( static_cast< Brain::Mode >( ctrl + shift ) );

    // Implement normal typing
    char byte( hge->Input_GetChar() );
    if ( m_brain.isValid( byte ) )
    {
        m_brain.select( byte );
    }
    // Backspace
    if ( hge->Input_GetKey() == HGEK_BACKSPACE &&
         m_brain.getBuffer()[0] != '\0' )
    {
        m_brain.remove();
    }
    // Return
    if ( hge->Input_GetKey() == HGEK_ENTER )
    {
        // TODO: store previous buffer so we can render it, etc.
        m_brain.accept();
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

    // draw what we've written this far: m_brain.getBuffer();

    // draw the cursor. show a dial of characters.
    const char * alphabet( m_brain.getAlphabet() );
    for ( unsigned int i = 0; i < strlen( alphabet ); ++i )
    {
        char byte( alphabet[i] );
        if ( byte == ' ' )
        {
            byte = '^';
        }
        font->printf( -50.0f, -9.5f + 2.5f * i, HGETEXT_LEFT,
                      "%c", byte );
//      font->printf( -50.0f, -9.5f + 4.75f * i, HGETEXT_LEFT,
//                    "%c", byte );
    }
    font->printf( -50.0f, -9.5f + 4.75f, HGETEXT_LEFT,
                  m_brain.getBuffer() );

    // draw what we predict we'll write next
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------

//==============================================================================
