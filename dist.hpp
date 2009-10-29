//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

struct Item
{
    char m_symbol;
    double m_value;
};

class Dist
{
public:
    explicit Dist( const char * alphabet );
    ~Dist();

    void clear();
    void set( char symbol, double value );
    void add( char symbol, double value );
    void normalise();
    bool normalised() const;
    const Item & get( unsigned int index ) const;
    bool zero( char symbol );
    unsigned int size() const;

private:
    Item * m_item;
    unsigned int m_size;
    unsigned int m_max;
};
