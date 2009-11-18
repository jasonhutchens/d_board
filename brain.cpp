//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

#include <brain.hpp>
#include <trie.hpp>
#include <dist.hpp>

#include <cassert>
#include <cstring>

namespace
{
    const unsigned int MAX_SIZE( 65536 );
    const char * ALPHABET[] = {
        "abcdefghijklmnopqrstuvwxyz ",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ ",
        "0123456789 ",
        "?!.,@#$/-_:;'\"()%&* ",
    };
    const char * UNIVERSE = "abcdefghijklmnopqrstuvwxyz"
                            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                            "0123456789"
                            " ,.?!;:/-_()'\"$@#%&*";
    const char ESCAPE( '~' );
}

//------------------------------------------------------------------------------
Brain::Brain( unsigned int order = 5 )
    :
    m_mode( MODE_NORMAL ),
    m_buffer( 0 ),
    m_size( 0 ),
    m_trie( 0 ),
    m_tmp( 0 ),
    m_probs( 0 )
{
    m_buffer = new char[ MAX_SIZE ];
    m_trie = new Trie( order, UNIVERSE, ESCAPE );
    m_tmp = new char[ 256 ];
    m_probs = new double[ 256 ];
    assert( m_buffer );
    m_buffer[0] = '\0';
    for ( unsigned int i = 0; i < m_trie->getOrder(); ++i )
    {
        select( ESCAPE );
    }
}

//------------------------------------------------------------------------------
Brain::~Brain()
{
    delete [] m_buffer;
    delete m_trie;
    delete [] m_tmp;
    delete [] m_probs;
}

//------------------------------------------------------------------------------
void Brain::setMode( Mode mode )
{
    assert( mode <= MODE_BOTH );
    m_mode = mode;
}

//------------------------------------------------------------------------------
Brain::Mode Brain::getMode()
{
    return m_mode;
}

//------------------------------------------------------------------------------
const char * Brain::getAlphabet()
{
    return ALPHABET[static_cast<int>(m_mode)];
}

//------------------------------------------------------------------------------
void Brain::clear()
{
    m_buffer[0] = '\0';
    m_size = 0;
    m_trie->clear();
    for ( unsigned int i = 0; i < m_trie->getOrder(); ++i )
    {
        select( ESCAPE );
    }
}

//------------------------------------------------------------------------------
void Brain::learn( const char * blob )
{
    accept();
    for ( unsigned int i = 0; blob[i] != '\0'; ++i )
    {
        if ( blob[i] == '\r' || blob[i] == '\n' )
        {
            accept();
        }
        else
        {
            m_trie->observe( blob[i] );
        }
    }
    accept();
}

//------------------------------------------------------------------------------
const char * Brain::predictChoice( double probability )
{
    const Dist * dist = m_trie->predict();
    unsigned int index( 0 );
    for ( unsigned int i = 0; i < dist->size(); ++i )
    {
        if ( dist->get(i).m_value < probability )
        {
            break;
        }
        m_probs[index] = dist->get(i).m_value;
        m_tmp[index++] = dist->get(i).m_symbol;
    }
    m_tmp[index] = '\0';
    return m_tmp;
}

//------------------------------------------------------------------------------
const char * Brain::predictFuture( double probability )
{
    return predictFuture( ESCAPE, probability );
}

//------------------------------------------------------------------------------
const char * Brain::predictFuture( char selected, double probability )
{
    unsigned int index( 0 );
    double total( 1.0f );
    m_trie->copyHand();
    m_trie->swapHand();
    if ( selected != ESCAPE )
    {
        m_trie->walk( selected );
    }
    while ( total >= probability && index < 255 )
    {
        const Dist * dist = m_trie->predict();
        if ( dist->size() == 0 )
        {
            break;
        }
        total *= dist->get(0).m_value;
        if ( total >= probability )
        {
            m_probs[index] = total;
            m_tmp[index] = dist->get(0).m_symbol;
            m_trie->walk( m_tmp[index++] );
        }
    }
    m_trie->swapHand();
    m_tmp[index] = '\0';
    return m_tmp;
}

//------------------------------------------------------------------------------
const double * Brain::getProbs()
{
    return m_probs;
}

//------------------------------------------------------------------------------
const Dist * Brain::predict()
{
    return m_trie->predict();
}

//------------------------------------------------------------------------------
void Brain::select( char symbol )
{
    m_trie->observe( symbol );
    if ( symbol == ESCAPE )
    {
        return;
    }
    assert( m_size < MAX_SIZE - 1 );
    m_buffer[m_size] = symbol;
    ++m_size;
    m_buffer[m_size] = '\0';
}

//------------------------------------------------------------------------------
void Brain::walk( char symbol )
{
    m_trie->walk( symbol );
    if ( symbol == ESCAPE )
    {
        return;
    }
    assert( m_size < MAX_SIZE - 1 );
    m_buffer[m_size] = symbol;
    ++m_size;
    m_buffer[m_size] = '\0';
}

//------------------------------------------------------------------------------
char Brain::remove()
{
    assert( m_size > 0 );
    --m_size;
    char symbol = m_buffer[m_size];
    m_buffer[m_size] = '\0';
    m_trie->reset();
    for ( unsigned int i = 0; i < m_trie->getOrder(); ++i )
    {
        m_trie->walk( ESCAPE );
    }
    for ( unsigned int i = 0; i < m_size; ++ i )
    {
        m_trie->walk( m_buffer[i] );
    }
    m_trie->forget( symbol );
    return symbol;
}

//------------------------------------------------------------------------------
const char * Brain::getBuffer()
{
    return m_buffer;
}

//------------------------------------------------------------------------------
bool Brain::isValid( char symbol )
{
    return symbol != '\0' && strchr( UNIVERSE, symbol ) != 0;
}

//------------------------------------------------------------------------------
void Brain::accept()
{
    m_buffer[0] = '\0';
    m_size = 0;
    m_trie->reset();
    for ( unsigned int i = 0; i < m_trie->getOrder(); ++i )
    {
        walk( ESCAPE );
    }
}
