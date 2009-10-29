//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

#include <trie.hpp>
#include <node.hpp>
#include <dist.hpp>

#include <cassert>

//------------------------------------------------------------------------------
Trie::Trie( unsigned int order, const char * alphabet, char escape )
    :
    m_order( order ),
    m_alphabet( alphabet ),
    m_escape( escape ),
    m_hand( 0 ),
    m_back( 0 ),
    m_dist( 0 )
{
    m_hand = new Node * [order];
    m_hand[0] = new Node( escape );
    m_back = new Node * [order];
    m_dist = new Dist( alphabet );
    reset();
}

//------------------------------------------------------------------------------
Trie::~Trie()
{
    delete m_hand[0];
    delete [] m_hand;
    delete [] m_back;
    delete m_dist;
}

//------------------------------------------------------------------------------
unsigned int Trie::getOrder()
{
    return m_order;
}

//------------------------------------------------------------------------------
void Trie::clear()
{
    m_hand[0]->clear();
    reset();
}

//------------------------------------------------------------------------------
void Trie::reset()
{
    for ( unsigned int i = 1; i < m_order; ++i )
    {
        m_hand[i] = 0;
    }
}

//------------------------------------------------------------------------------
void Trie::walk( char symbol )
{
    assert( includes( symbol ) );
    for ( unsigned int i = m_order - 1; i > 0; --i )
    {
        if ( m_hand[i - 1] )
        {
            m_hand[i] = m_hand[i - 1]->find( symbol );
        }
    }
}

//------------------------------------------------------------------------------
void Trie::observe( char symbol )
{
    assert( includes( symbol ) );
    for ( unsigned int i = m_order - 1; i > 0; --i )
    {
        if ( m_hand[i - 1] )
        {
            m_hand[i] = m_hand[i - 1]->observe( symbol, m_escape );
        }
    }
}

//------------------------------------------------------------------------------
void Trie::forget( char symbol )
{
    assert( includes( symbol ) );
    for ( unsigned int i = m_order; i > 0; --i )
    {
        if ( m_hand[i - 1] )
        {
            m_hand[i - 1]->forget( symbol, m_escape );
        }
    }
}

//------------------------------------------------------------------------------
const Dist * Trie::predict() const
{
    double attenuation( 1.0f );
    m_dist->clear();
    for ( unsigned int i = m_order; i > 0; --i )
    {
        if ( ! m_hand[i - 1] )
        {
            continue;
        }
        attenuation = m_hand[i - 1]->predict( m_dist, m_escape, attenuation );
    }
    unsigned int size( 0 );
    while ( m_alphabet[size++] != '\0' );
    --size;
    double probability( attenuation / static_cast<double>( size ) );
    size = 0;
    while ( m_alphabet[size] != '\0' )
    {
        const_cast<Dist *>(m_dist)->add( m_alphabet[size++], probability );
    }
    assert( m_dist->normalised() );
    return m_dist;
}

//------------------------------------------------------------------------------
void Trie::copyHand()
{
    for ( unsigned int i = 0; i < m_order; ++i )
    {
        m_back[i] = m_hand[i];
    }
}

//------------------------------------------------------------------------------
void Trie::swapHand()
{
    Node ** tmp = m_hand;
    m_hand = m_back;
    m_back = tmp;
}

//------------------------------------------------------------------------------
//private
//------------------------------------------------------------------------------
bool Trie::includes( char symbol )
{
    assert( m_alphabet );
    unsigned int index( 0 );
    while ( m_alphabet[index] != '\0' )
    {
        if ( m_alphabet[index++] == symbol )
        {
            return true;
        }
    }
    return symbol == m_escape;
}
