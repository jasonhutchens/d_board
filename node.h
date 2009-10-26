//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

class Dist;

class Node
{
public:
    explicit Node( char symbol );
    ~Node();

    void clear();
    Node * find( char symbol );
    Node * observe( char symbol, char escape );
    void forget( char symbol, char escape );
    double predict( Dist * dist, char escape, double attenuation );

private:
    char m_symbol;
    unsigned char m_num;
    unsigned short m_count;
    Node ** m_child;
};
