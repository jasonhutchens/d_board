//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

class Node;
class Dist;

class Trie
{
public:
    Trie( unsigned int order, const char * alphabet, char escape );
    ~Trie();

    unsigned int getOrder();

    void clear();

    void reset();

    void walk( char symbol );

    void observe( char symbol );

    void forget( char symbol );

    const Dist * predict() const;

    void copyHand();

    void swapHand();

private:
    bool includes( char symbol );

    unsigned int m_order;
    const char * m_alphabet;
    char m_escape;
    Node ** m_hand;
    Node ** m_back;
    Dist * m_dist;
};
