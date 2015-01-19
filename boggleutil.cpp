#include "boggleutil.h"

/**
 * Constructs a Node from the given parameters.
 */
Node::Node( const string& str, int index ) {
    // converts to lower case
    for ( int i = 0; i < (int)( str.length() ); i++ ) {
        this->str += tolower( str[i] );
    }
    this->index = index;
}

/**
 * Returns a string which represents the letters on the die face.
 */
const string& Node::getString() {
    return str;
}

/**
 * Returns an index which represents the position of the dice.
 */
int Node::getIndex() {
    return index;
}

/**
 * Sets a flag to indicate whether the dice has been visited.
 */
void Node::setVisited( bool visited ) {
    this->visited = visited;
}

/**
 * Adds a pointer to an adjacent dice.
 */
void Node::addNeighbour( Node* n ) {
    neighbour.push_back( n );
}

/**
 * Judges whether the dice has been visited and compares the
 * letters on the dice is same to part of the specified string.
 *
 * Returns true if both of above are satisfied and false otherwise.
 */
bool Node::isMatch( const string& str,
                            int start,
                        vector<int> v,
                              Node* n ) {
    vector<int>::iterator it;
    bool isVisited = 0;

    for ( it = v.begin(); it != v.end(); ++it ) {
        if ( *it == n->getIndex() ) {
            isVisited = 1;
        }
    }

    if ( !str.compare( start, n->str.length(), n->str )
                                        && !isVisited ) {
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * Searches for a specified string on the board from this dice.
 *
 * Returns a pair, first of which is a vector to store the index
 * information and second of which is a flag to indicate whether
 * the string has been found.
 */
pair<vector<int>, bool> Node::search( const string& str,
                                          vector<int> v,
                                              int start ) {
    vector<Node*>::iterator it;
    for ( it = neighbour.begin(); it != neighbour.end(); ++it ) {

        vector<int> temp = v;

        if ( isMatch( str, start, temp, *it ) ) {
            temp.push_back( (*it)->getIndex() );

            if ( start + (*it)->str.length() == str.length() ) {
                return make_pair( temp, 1 );
            }

            else if ( start + (*it)->str.length() < str.length() ) {
                pair<vector<int>, bool> p = (*it)->search( str,
                                                          temp,
                                     start + (*it)->str.length() );
                if ( p.second == 1 )  return p;
            }
        }
    }
    return make_pair( v, 0 );
}

/**
 * Searches from this dice for all the strings which both the board
 * and lexicon contain and inserts all valid strings to a set.
 */
void Node::searchValid( unsigned int min,
                      set<string>* words,
                                TST& tst ) {
    stack<Node*> s;
    vector<Node*>::iterator it;
    Node *temp;
    string str;

    s.push( this );

    while ( s.size() > 0 ) {
        temp = s.top();
        s.pop();

        if ( temp->visited ) {
            temp->setVisited( 0 );
            str = str.substr( 0, str.length() - temp->getString().length() );
        }
        else if ( tst.isPrefix( str + temp->getString() ) ) {
            temp->setVisited( 1 );
            str = str + temp->getString();

            if ( tst.find( str ) && str.length() >= min ) {
                words->insert( str );
            }

            s.push( temp );

            for ( it = temp->neighbour.begin();
                   it != temp->neighbour.end();
                                          ++it ) {
                if ( !( (*it)->visited ) ) {
                    s.push( *it );
                }
            }
        }
    }
}

/**
 * Destructs a TST.
 *
 * Implemented simply by calling the clear() method.
 */
TST::~TST() {
    clear();
}

/**
 * Clears the TST.
 */
void TST::clear() {
    stack< TSTNode<char>* > s;
    TSTNode<char> *temp;

    if ( root != nullptr ) {
        s.push( root );
    }

    while ( s.size() > 0 ) {
        temp = s.top();
        s.pop();
        if ( temp->left != nullptr ) {
            s.push( temp->left );
        }
        if ( temp->middle != nullptr ) {
            s.push( temp->middle );
        }
        if ( temp->right != nullptr ) {
            s.push( temp->right );
        }
        delete temp;
    }

    root = nullptr;
}

/**
 * Inserts a string to TST.
 */
void TST::insert( const string& str ) {
    TSTNode<char> *seek = root, *plug = nullptr;
    int pos = 0, child = 0;

    // if TST is empty
    if ( root == nullptr && str.length() != 0 ) {
        root = new TSTNode<char>( str[pos] );
        plug = root;
        seek = root->middle;
        pos = pos + 1;
    }

    while ( pos < (int)( str.length() ) && str.length() != 0 ) {
        if ( seek == nullptr ) {
            if ( child == -1 ) {
                plug->left = new TSTNode<char>( str[pos] );
                plug = plug->left;
            }
            else if ( child == 0 ) {
                plug->middle = new TSTNode<char>( str[pos] );
                plug = plug->middle;
            }
            else {
                plug->right = new TSTNode<char>( str[pos] );
                plug = plug->right;
            }
            pos = pos + 1;
            while ( pos < (int)( str.length() ) ) {
                plug->middle = new TSTNode<char>( str[pos] );
                plug = plug->middle;
                pos = pos + 1;
            }
        }
        else if ( str[pos] < seek->digit ) {
            plug = seek;
            seek = seek->left;
            child = -1;
        }
        else if ( str[pos] == seek->digit ) {
            plug = seek;
            seek = seek->middle;
            pos = pos + 1;
            child = 0;
        }
        else {
            plug = seek;
            seek = seek->right;
            child = 1;
        }
    }

    // sets the end bit
    if ( plug != nullptr ) {
        plug->end = 1;
    }

}

/**
 * Finds whether a specified string is in the TST.
 *
 * Returns true if the string is in the TST and false otherwise.
 */
bool TST::find( const string& str ) {
    TSTNode<char> *curr = root, *prev = nullptr;
    int pos = 0;
    while ( pos < (int)( str.length() ) ) {
        if ( curr == nullptr ) {
            return 0;
        }
        else if ( str[pos] < curr->digit ) {
            prev = curr;
            curr = curr->left;
        }
        else if ( str[pos] == curr->digit ) {
            prev = curr;
            curr = curr->middle;
            pos = pos + 1;
        }
        else {
            prev = curr;
            curr = curr->right;
        }
    }

    if ( prev != nullptr && prev->end == 1 ) {
        return 1;
    }
    else {
        return 0;
    }
}

/**
 * Judges whether a specified string is a prefix in the TST.
 *
 * Returns true if the string is a prefix and false otherwise.
 */
bool TST::isPrefix( const string& str ) {
    TSTNode<char> *curr = root;
    int pos = 0;
    while ( pos < (int)( str.length() ) ) {
        if ( curr == nullptr ) {
            return 0;
        }
        else if ( str[pos] < curr->digit ) {
            curr = curr->left;
        }
        else if ( str[pos] == curr->digit ) {
            curr = curr->middle;
            pos = pos + 1;
        }
        else {
            curr = curr->right;
        }
    }

    return 1;
}

/**
 * Judges whether the TST is empty.
 *
 * If there is no string in the TST, returns true; otherwise,
 * returns false.
 */
bool TST::isEmpty() {
    return root == nullptr;
}
