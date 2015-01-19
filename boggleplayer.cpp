#include "boggleplayer.h"

/**
 * Destructs a BogglePlayer.
 */
BogglePlayer::~BogglePlayer() {
    if ( board != nullptr ) {
        for ( int i = 0; i < (int)rows; i++ ) {
            for ( int j = 0; j < (int)cols; j++ ) {
                delete board[i][j];
            }
        }

        for ( int i = 0; i < (int)rows; i++ ) {
            delete[] board[i];
        }

        delete[] board;

        board = nullptr;
    }
}

/**
 * Builds the lexicon.
 *
 * Takes as argument a set containing the words specifying the
 * official lexicon to be used for the game. Each word in the
 * set will be a string consisting of lowercase letters a-z only.
 */
void BogglePlayer::buildLexicon( const set<string>& word_list ) {

    set<string>::const_iterator it;
    vector<string*> v;
    string *str, *temp;
    int rdIndex, sz;
    random_device rd;

    for ( it = word_list.begin(); it != word_list.end(); ++it ) {
        str = new string( *it );
        v.push_back( str );
    }

    sz = (int)( v.size() );

    for ( int i = 0; i < sz; i++ ) {
        rdIndex = rd() % ( sz - i ) + i;
        temp = v[rdIndex];
        v[rdIndex] = v[i];
        v[i] = temp;
    }

    tst.clear();

    for ( int i = 0; i < sz; i++ ) {
        tst.insert( *( v[i] ) );
    }

    for ( int i = 0; i < sz; i++ ) {
        delete v[i];
    }
}

/**
 * Sets the board.
 *
 * Takes two arguments: the number of rows and columns in the
 * board, and an array of arrays of strings representing what
 * is shown on the face of dice on a Boggle board.
 */
void BogglePlayer::setBoard( unsigned int rows,
                             unsigned int cols,
                             string** diceArray ) {
    if ( diceArray == nullptr ) return;

    if ( board != nullptr ) {
        for ( int i = 0; i < (int)rows; i++ ) {
            for ( int j = 0; j < (int)cols; j++ ) {
                delete board[i][j];
            }
        }

        for ( int i = 0; i < (int)rows; i++ ) {
            delete[] board[i];
        }

        delete[] board;

        board = nullptr;
    }

    this->rows = rows;
    this->cols = cols;

    board = new Node**[rows];
    for ( int i = 0; i < (int)rows; i++ ) {
        board[i] = new Node*[cols];
    }

    for ( int i = 0; i < (int)rows; i++ ) {
        for ( int j = 0; j < (int)cols; j++ ) {
            board[i][j] = new Node( diceArray[i][j], i * cols + j );
        }
    }

    for ( int i = 0; i < (int)rows; i++ ) {
        for ( int j = 0; j < (int)cols; j++ ) {
            if ( i - 1 >= 0 && j - 1 >= 0 )
                board[i][j]->addNeighbour( board[i-1][j-1] );
            if ( i - 1 >= 0 )
                board[i][j]->addNeighbour( board[i-1][j] );
            if ( i - 1 >= 0 && j + 1 < (int)cols )
                board[i][j]->addNeighbour( board[i-1][j+1] );
            if ( j - 1 >= 0 )
                board[i][j]->addNeighbour( board[i][j-1] );
            if ( j + 1 < (int)cols )
                board[i][j]->addNeighbour( board[i][j+1] );
            if ( i + 1 < (int)rows && j - 1 >= 0 )
                board[i][j]->addNeighbour( board[i+1][j-1] );
            if ( i + 1 < (int)rows )
                board[i][j]->addNeighbour( board[i+1][j] );
            if ( i + 1 < (int)rows && j + 1 < (int)cols )
                board[i][j]->addNeighbour( board[i+1][j+1] );
        }
    }
}

/**
 * Gets all the words which both board and lexicon contain.
 *
 * Takes two arguments: an int specifying a minimum word length,
 * and a pointer to a set of strings.
 */
bool BogglePlayer::getAllValidWords( unsigned int minimum_word_length,
                                                   set<string>* words ) {
    if ( board == nullptr || tst.isEmpty() ) {
        return 0;
    }

    for ( int i = 0; i < (int)rows; i++ ) {
        for ( int j = 0; j < (int)cols; j++ ) {
            for ( int m = 0; m < (int)rows; m++ ) {
                for ( int n = 0; n < (int)cols; n++ ) {
                    board[m][n]->setVisited( 0 );
                }
            }
            board[i][j]->searchValid( minimum_word_length, words, tst );
        }
    }
    return 1;
}

/**
 * Checks whether a specified word is in the lexicon.
 *
 * Takes as argument a const string passed by reference, and
 * determines whether it can be found in the lexicon specified
 * by the most recent call to buildLexicon().
 *
 * Returns true if the word is in the lexicon, and returns
 * false if it is not in the lexicon or if buildLexicon()
 * has not yet been called.
 */
bool BogglePlayer::isInLexicon( const string& word_to_check ) {
    return tst.find( word_to_check );
}

/**
 * Checks whether a specified word is on the board.
 *
 * Takes as argument a string passed by reference. It determines
 * whether the string can be found by following an acyclic simple
 * path on the board specified by the most recent call to setBoard().
 */
vector<int> BogglePlayer::isOnBoard( const string& word_to_check ) {

    string s;
    for ( int i = 0; i < (int)( word_to_check.length() ); i++ ) {
        s += tolower( word_to_check[i] );
    }

    for ( int i = 0; i < (int)rows; i++ ) {
        for ( int j = 0; j < (int)cols; j++ ) {
            vector<int> v;

            if ( !s.compare( 0, board[i][j]->getString().length(),
                                         board[i][j]->getString() ) ) {
                v.push_back( board[i][j]->getIndex() );

                if ( !s.compare( board[i][j]->getString() ) ) {
                    return v;
                }

                else if ( board[i][j]->getString().length() < s.length() ) {

                    pair<vector<int>, bool> p = board[i][j]->search
                      ( s, v, board[i][j]->getString().length() );

                    if ( p.second == 1 ) return p.first;
                }
            }
        }
    }
    vector<int> v;
    return v;
}

/**
 * Used for testing.
 */
void BogglePlayer::getCustomBoard( string** &new_board,
                                   unsigned int *rows,
                                   unsigned int * cols ) {}
