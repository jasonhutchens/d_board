//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

#include <cassert>
#include "node.h"
#include "dist.h"

//------------------------------------------------------------------------------
Node::Node( char symbol )
    :
    m_symbol( symbol ),
    m_num( 0 ),
    m_count( 0 ),
    m_child( 0 )
{
}

//------------------------------------------------------------------------------
Node::~Node()
{
    clear();
}

//------------------------------------------------------------------------------
void Node::clear()
{
    m_count = 0;
    for ( unsigned char i = 0; i < m_num; ++i )
    {
        assert( m_child[i] );
        delete m_child[i];
    }
    delete [] m_child;
    m_child = 0;
    m_num = 0;
}

//------------------------------------------------------------------------------
Node * Node::find( char symbol )
{
    unsigned char bot( 0 ), mid( 0 ), top( m_num );
    while ( bot < top )
    {
        mid = ( top + bot ) / 2;
        if ( symbol > m_child[mid]->m_symbol )
        {
            bot = mid + 1;
        }
        else if ( symbol < m_child[mid]->m_symbol )
        {
            top = mid;
        }
        else
        {
            return m_child[mid];
        }
    }
    return 0;
}

//------------------------------------------------------------------------------
Node * Node::observe( char symbol, char escape )
{
    Node * child( find( symbol ) );

    if ( ! child )
    {
        if ( symbol != escape )
        {
            observe( escape, escape );
        }

        child = new Node( symbol );
        Node ** tmp = new Node * [m_num + 1];
        unsigned char i( 0 ), j( 0 );
        bool inserted( false );
        while ( j < m_num )
        {
            if ( ! inserted && child->m_symbol < m_child[j]->m_symbol )
            {
                tmp[i++] = child;
                inserted = true;
            }
            else
            {
                tmp[i++] = m_child[j++];
            }
        }
        if ( ! inserted )
        {
            tmp[i] = child;
        }
        m_num += 1;
        delete [] m_child;
        m_child = tmp;
    }

    assert( child->m_count < 65535 );

    child->m_count += 1;

    return child;
}

//------------------------------------------------------------------------------
void Node::forget( char symbol, char escape )
{
    Node * child( find( symbol ) );

    if ( ! child )
    {
        return;
    }

    assert( child->m_count > 0 );

    child->m_count -= 1;

    if ( child->m_count == 0 )
    {
        if ( symbol != escape )
        {
            forget( escape, escape );
        }
        --m_num;
        bool found( false );
        for ( unsigned char i = 0; i < m_num; ++i )
        {
            if ( m_child[i]->m_symbol == symbol )
            {
                found = true;
            }
            if ( found )
            {
                m_child[i] = m_child[i + 1];
            }
        }
        delete child;
    }
}

//------------------------------------------------------------------------------
double Node::predict( Dist * dist, char escape, double attenuation )
{
    if ( m_num == 0 )
    {
        return attenuation;
    }

    double new_attenuation( 0.0f );

    unsigned int total( 0 );
    for ( unsigned char i = 0; i < m_num; ++i )
    {
        total += m_child[i]->m_count;
    }

    for ( unsigned char i = 0; i < m_num; ++i )
    {
        double probability( static_cast<double>( m_child[i]->m_count ) /
                            static_cast<double>( total ) );
        if ( m_child[i]->m_symbol == escape )
        {
            new_attenuation = attenuation * probability;
        }
        else
        {
            dist->add( m_child[i]->m_symbol, probability * attenuation );
        }
    }

    assert( new_attenuation > 0.0f );

    return new_attenuation;
}
