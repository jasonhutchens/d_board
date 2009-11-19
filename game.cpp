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
#include <cmath>

//==============================================================================
Game::Game()
    :
    Context(),
    m_brain( 7 ),
    m_lines(),
    m_dial(),
    m_current(),
    m_row( 0 ),
    m_col( 0 )
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
    m_lines.clear();
    m_dial.clear();

    _clearCurrent();

    m_row = 0;
    m_col = 0;
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
    hgeResourceManager * rm( Engine::rm() );

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
        _clearCurrent();
        if ( byte == ' ' )
        {
            hge->Effect_PlayEx( rm->GetEffect( "space" ), 15 );
        }
        else
        {
            hge->Effect_Play( rm->GetEffect( "letter" ) );
        }
        m_row += 1;
    }
    // Backspace
    if ( hge->Input_GetKey() == HGEK_BACKSPACE &&
         m_brain.getBuffer()[0] != '\0' )
    {
        m_brain.remove();
        _clearCurrent();
        m_row -= 1;
    }
    // Return
    if ( hge->Input_GetKey() == HGEK_ENTER )
    {
        m_lines.push_back( m_brain.getBuffer() );
        m_brain.accept();
        _clearCurrent();
        hge->Effect_Play( rm->GetEffect( "return" ) );
        m_row = 0;
        m_col += 1;
    }
    if ( m_row == 40 )
    {
        hge->Effect_Play( rm->GetEffect( "bell" ) );
    }

    float threshold( 1.0f / strlen( m_brain.getAlphabet() ) );

    // Populate the dial without repeating the same symbol in a sliding window
    m_dial.clear();
    const char * choice( m_brain.predictChoice( threshold ) );
    for ( unsigned int i = 0; i < strlen( choice ) && m_dial.size() < 4; ++i )
    {
        m_dial.push_back( choice[i] );
    }
    const char * alphabet( m_brain.getAlphabet() );
    for ( unsigned int i = 0; i < strlen( alphabet ); ++i )
    {
        // Don't add if it exists in the previous 5.
        if ( _inPrevious( alphabet[i], 5 ) )
        {
            continue;
        }
        m_dial.push_back( alphabet[i] );
    }
    // And now pop off elements that appear in the next 5.
    for ( int offset = 1; offset <= 5; ++offset )
    {
        int index( m_dial.size() - offset );
        if ( _inNext( index, 5 ) )
        {
            m_dial.erase( m_dial.begin() + index );
        }
    }

    // Implement cursor keys
    if ( hge->Input_GetKey() == HGEK_UP )
    {
        m_current[m_brain.getMode()] -= 1;
        if ( m_current[m_brain.getMode()] < 0 )
        {
            m_current[m_brain.getMode()] = m_dial.size() - 1;
        }
    }
    if ( hge->Input_GetKey() == HGEK_DOWN )
    {
        m_current[m_brain.getMode()] += 1;
        if ( m_current[m_brain.getMode()] >= m_dial.size() )
        {
            m_current[m_brain.getMode()] = 0;
        }
    }
    if ( hge->Input_GetKey() == HGEK_RIGHT )
    {
        char byte( m_dial[m_current[m_brain.getMode()]] );
        m_brain.select( byte );
        _clearCurrent();
        m_row += 1;
        if ( byte == ' ' )
        {
            hge->Effect_PlayEx( rm->GetEffect( "space" ), 15 );
        }
        else if ( byte >= 'A' && byte <= 'Z' )
        {
            hge->Effect_Play( rm->GetEffect( "shift_letter" ) );
        }
        else
        {
            hge->Effect_Play( rm->GetEffect( "letter" ) );
        }
    }
    if ( hge->Input_GetKey() == HGEK_LEFT &&
         m_brain.getBuffer()[0] != '\0' )
    {
        m_brain.remove();
        _clearCurrent();
        m_row -= 1;
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

    unsigned int row( 0 );
    unsigned int col( 0 );

    // draw the lines that we've entered so far
    for( std::vector< std::string >::iterator i = m_lines.begin();
         i != m_lines.end(); ++i )
    {
        const char * line( i->c_str() );
        for ( unsigned int j = 0; j < strlen( line ); ++j )
        {
            font->printf( -50.0f + 2.0f * row,
                          -9.5f + 4.75f * col,
                          HGETEXT_LEFT, "%c", line[j] );
            row += 1;
            if ( row > 50 ) row = 50;
        }
        col += 1;
        row = 0;
    }

    // draw what we've written this far: m_brain.getBuffer();
    const char * past( m_brain.getBuffer() );
    for ( unsigned int i = 0; i < strlen( past ); ++i )
    {
        font->printf( -50.0f + 2.0f * row,
                      -9.5f + 4.75f * col,
                      HGETEXT_LEFT, "%c", past[i] );
        row += 1;
        if ( row > 50 ) row = 50;
    }

    // draw the cursor, showing a dial of characters
    hgeSprite * cursor( rm->GetSprite( "cursor" ) );
    cursor->RenderEx( -50.0f + 2.0f * row + 1.0f, -9.5f + 4.75f * col + 5.5f,
                      0.0f, 0.06f, 0.13f );
    if ( m_dial.size() < 5 )
    {
        return;
    }
    int current( m_current[m_brain.getMode()] );
    for ( int offset = -1; offset <= 4; ++offset )
    {
        int index( current + offset );
        if ( index < 0 )
        {
            index += m_dial.size();
        }
        else if ( index >= m_dial.size() )
        {
            index -= m_dial.size();
        }
        hgeSprite * sprite( font->GetSprite( m_dial[index] ) );
        hgeColorRGB color( 1.0f, 1.0f, 1.0f, ( offset == 0 ) ? 1.0f : 0.8f );
        sprite->SetColor( color.GetHWColor() );
        float scale( 1.0f );
        sprite->RenderEx( -50.0f + 2.0f * row,
                          -9.5f + 4.75f * col + 2.5f * offset,
                          0.0f, 0.1f, 0.1f * scale );
    }
    row += 1;

    // draw a prediction of what we're likely to write next
    const char * future( m_brain.predictFuture( m_dial[current] ) );
    const double * probs( m_brain.getProbs() );
    for ( unsigned int i = 0; i < strlen( future ); ++i )
    {
        hgeColorRGB color( 0.0f, 0.0f, 0.0f, static_cast< float >( probs[i] ) );
        font->SetColor( color.GetHWColor() );
        font->printf( -50.0f + 2.0f * row,
                      -9.5f + 4.75f * col,
                      HGETEXT_LEFT, "%c", future[i] );
        row += 1;
        if ( row > 50 ) row = 50;
    }

    // TODO: Toggle between cursor and d-board - make cursor flash
    // TODO: Forward predictions for all items in the dial
    // TODO: Train model
    // TODO: Cursor-relative versus paper-relative
    // TODO: Feed paper in
    // TODO: Move paper when shift pressed
    // TODO: Gamepad controls
    // TODO: Save content to a file
    // TODO: Hook up Wiimote
    // TODO: Introduction tutorial
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------
void Game::_clearCurrent()
{
    m_current[Brain::MODE_NORMAL] = 0;
    m_current[Brain::MODE_SHIFT] = 0;
    m_current[Brain::MODE_ALT] = 0;
    m_current[Brain::MODE_BOTH] = 0;
}

//------------------------------------------------------------------------------
bool Game::_inPrevious( char item, int num )
{
    for ( int i = m_dial.size() - 1; i >= 0 && num > 0; --i )
    {
        num -= 1;
        if ( m_dial[i] == item )
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------
bool Game::_inNext( int index, int num )
{
    for ( int i = 0; i <= num; ++i )
    {
        if ( m_dial[index] == m_dial[( index + i + 1 ) % m_dial.size()] )
        {
            return true;
        }
    }
    return false;
}

//==============================================================================
