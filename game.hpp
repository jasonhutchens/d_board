//==============================================================================

#ifndef ArseGame
#define ArseGame

#pragma once

#include <vector>
#include <string>

#include <brain.hpp>
#include <context.hpp>

class hgeSprite;

//------------------------------------------------------------------------------
class Game : public Context
{
  public:
    Game();
    virtual ~Game();

  private:
    Game( const Game & );
    Game & operator=( const Game & );

  public:
    virtual void init();
    virtual void fini();
    virtual bool update( float dt );
    virtual void render();

  private:
    void _clearCurrent( int index = 0 );
    bool _inPrevious( char item, int num );
    bool _inNext( int index, int num );

    Brain m_brain;
    std::vector< std::string > m_lines;
    std::vector< char > m_dial;
    int m_current[4];
    int m_row;
    int m_col;
	int m_dialSize;
	int m_coolDown;
};

#endif

//==============================================================================
