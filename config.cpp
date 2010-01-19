//==============================================================================

#include <config.hpp>
#include <engine.hpp>

#include <hge.h>

namespace
{
    static const char * FILENAME( "dboard" );
}

//------------------------------------------------------------------------------
Config::Config()
    :
    screenWidth( 800 ),
    screenHeight( 600 ),
    bpp( 32 ),
#ifdef _DEBUG
    fullScreen( false ),
#else
    fullScreen( false ),
#endif
    sound( true )
{
}

//------------------------------------------------------------------------------
Config::~Config()
{
}

//------------------------------------------------------------------------------
void
Config::init()
{
    screenWidth = Engine::hge()->Ini_GetInt( FILENAME, "width",
                                             screenWidth );
    screenHeight = Engine::hge()->Ini_GetInt( FILENAME,"height",
                                              screenHeight );
    bpp = Engine::hge()->Ini_GetInt( FILENAME,"bpp", bpp );
    fullScreen = Engine::hge()->Ini_GetInt( FILENAME, "fullscreen",
                                            fullScreen ? 1 : 0 ) == 1;
    sound = Engine::hge()->Ini_GetInt( FILENAME, "sound", sound ? 1 : 0 ) == 1;
}

//------------------------------------------------------------------------------
void
Config::fini()
{
    Engine::hge()->Ini_SetInt( FILENAME, "width", screenWidth );
    Engine::hge()->Ini_SetInt( FILENAME,"height", screenHeight );
    Engine::hge()->Ini_SetInt( FILENAME,"bpp", bpp );
    Engine::hge()->Ini_SetInt( FILENAME, "fullscreen", fullScreen ? 1 : 0 );
    Engine::hge()->Ini_SetInt( FILENAME, "sound", sound ? 1 : 0 );
}

//==============================================================================
