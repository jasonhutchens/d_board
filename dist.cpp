//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

#include <dist.hpp>

#include <cassert>

//------------------------------------------------------------------------------
Dist::Dist( const char * alphabet )
    :
    m_item( 0 ),
    m_size( 0 ),
    m_max( 0 )
{
    while ( alphabet[m_max++] != '\0' );
    --m_max;
    m_item = new Item[m_max];
    clear();
}

//------------------------------------------------------------------------------
Dist::~Dist()
{
    delete [] m_item;
}

//------------------------------------------------------------------------------
void Dist::clear()
{
    m_size = 0;
    for ( unsigned int i = 0; i < m_max; ++i )
    {
        m_item[i].m_symbol = '-';
        m_item[i].m_value = 0.0f;
    }
}

//------------------------------------------------------------------------------
void Dist::set( char symbol, double value )
{
    assert( zero( symbol ) );
    unsigned int bot( 0 ), mid( 0 ), top( m_size );
    while ( bot < top )
    {
        mid = ( top + bot ) / 2;
        if ( value < m_item[mid].m_value )
        {
            bot = mid + 1;
        }
        else if ( value > m_item[mid].m_value )
        {
            top = mid;
        }
        else
        {
            break;
        }
    }
    assert( m_size <= m_max );
    for ( unsigned int i = m_size; i > top; --i )
    {
        m_item[i] = m_item[i - 1];
    }
    m_item[top].m_symbol = symbol;
    m_item[top].m_value = value;
    m_size += 1;
}

//------------------------------------------------------------------------------
void Dist::add( char symbol, double value )
{
    int index( -1 );
    for ( unsigned int i = 0; i < m_size; ++ i )
    {
        if ( m_item[i].m_symbol == symbol )
        {
            index = static_cast<int>(i);
            break;
        }
    }
    if ( index < 0 )
    {
        set( symbol, value );
    }
    else
    {
        m_item[index].m_value += value;
        Item tmp;
        while ( index > 0 && m_item[index].m_value > m_item[index-1].m_value )
        {
            tmp = m_item[index-1];
            m_item[index-1] = m_item[index];
            m_item[index--] = tmp;
        }
    }
}

//------------------------------------------------------------------------------
void Dist::normalise()
{
    double total( 0.0f );

    for ( unsigned int i = 0; i < m_size; ++i )
    {
        total += m_item[i].m_value;
    }
    if ( total == 0.0f )
    {
        return;
    }
    for ( unsigned int i = 0; i < m_size; ++i )
    {
        m_item[i].m_value /= total;
    }
}

//------------------------------------------------------------------------------
bool Dist::normalised() const
{
    double total( 0.0f );

    for ( unsigned int i = 0; i < m_size; ++i )
    {
        total += m_item[i].m_value;
    }

    return total < 1.000001f && total > 0.999999f;
}

//------------------------------------------------------------------------------
const Item & Dist::get( unsigned int index ) const
{
    assert( index < m_size );
    return m_item[index];
}

//------------------------------------------------------------------------------
bool Dist::zero( char symbol )
{
    for ( unsigned int i = 0; i < m_max; ++ i )
    {
        if ( m_item[i].m_symbol == symbol &&
             m_item[i].m_value > 0.0f )
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
unsigned int Dist::size() const
{
    return m_size;
}
