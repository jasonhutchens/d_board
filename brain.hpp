//==============================================================================
//         Copyright (c) 2006 - 2007 Jason Hutchens and Paul Geerts.          //
//==============================================================================

class Trie;
class Dist;

/*
 * The brain is the back-end for the d-board prototype.
 */
class Brain
{
public:
    // Higher order = more memory but better predictions.
    explicit Brain( unsigned int order );
    ~Brain();

    enum Mode { MODE_NORMAL = 0, MODE_SHIFT, MODE_ALT, MODE_BOTH };

    void setMode( Mode mode );
    Mode getMode();

    // Get the alphabet for the current mode.
    const char * getAlphabet();

    // Clear everything so we start with a blank slate.
    void clear();

    // Learn from the stuff in the blob. Asserts if the blob is null, or if it
    // contains bytes other than those in the four alphabets, '\n', '\r', '\0'
    void learn( const char * blob );

    // Return a '\0' terminated blob of the characters that could be selected by
    // the user, only showing those that are at least as likely as probability.
    const char * predictChoice( double probability = 0.1f );

    // Return a '\0' terminated blob of the sequence of characters likely to
    // come next, including the one at the insertion point.
    const char * predictFuture( double probability = 0.1f );
    const char * predictFuture( char selected, double probability = 0.1f );
    const double * getProbs();

    // Just return the prediction from the trie.
    const Dist * predict();

    // Select the specified symbol, learning by observing it, and adding it to
    // the buffer.
    void select( char symbol );

    // As above, without learning.
    void walk( char symbol );

    // Delete the most recent symbol from the buffer, and return it. Asserts if
    // there are no symbols in the buffer.
    char remove();

    // Return the buffer of text the user has entered. Won't be NULL ever.
    const char * getBuffer();

    // Return a boolean indicating whether the specified symbol can be used.
    bool isValid( char symbol);

    // Eat the current buffer, as if the user had entered a newline. This clears
    // the contents of the buffer and does other stuff too.
    void accept();

private:
    Mode m_mode;
    char * m_buffer;
    unsigned int m_size;
    Trie * m_trie;
    char * m_tmp;
    double * m_probs;
};
