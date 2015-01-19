#ifndef BOGGLEUTIL_H
#define BOGGLEUTIL_H

#include <algorithm>
#include <vector>
#include <stack>
#include <set>
#include <string>
#include <locale>

using std::pair;
using std::vector;
using std::stack;
using std::set;
using std::string;
using std::locale;

class TST;

/**
 * Represents a dice in a board.
 *
 * Node contains a string to represent single or multiple
 * letters on the die face, contains an index to locate
 * dice's position, contains a flag to represent whether
 * the dice has been visited, contains a vector of pointers
 * to store the address of its adjacent dices.
 */
class Node {

private:
    string str;
    int index;
    bool visited;
    vector<Node*> neighbour;

    /**
     * Judges whether the dice has been visited and compares the
     * letters on the dice is same to part of the specified string.
     *
     * Returns true if both of above are satisfied and false otherwise.
     */
    bool isMatch(const string& str, int start, vector<int> v, Node* n);

public:
    /**
     * Constructs a Node from the given parameters.
     */
    Node(const string& str, int index);

    /**
     * Destructs the Node.
     */
    ~Node() {}

    /**
     * Returns a string which represents the letters on the die face.
     */
    const string& getString();

    /**
     * Returns an index which represents the position of the dice.
     */
    int getIndex();

    /**
     * Sets a flag to indicate whether the dice has been visited.
     */
    void setVisited(bool visited);

    /**
     * Adds a pointer to an adjacent dice.
     */
    void addNeighbour(Node* n);

    /**
     * Searches for a specified string on the board from this dice.
     *
     * Returns a pair, first of which is a vector to store the index
     * information and second of which is a flag to indicate whether
     * the string has been found.
     */
    pair<vector<int>, bool> search(const string& str,
                                       vector<int> v,
                                           int start);

    /**
     * Searches from this dice for all the strings which both the board
     * and lexicon contain and inserts all valid strings to a set.
     */
    void searchValid(unsigned int min,
                   set<string>* words,
                             TST& tst);

};

/**
 * Represents a Ternary Search Tree Node.
 */
template<typename T>
class TSTNode {

public:
    TSTNode<T> *left;
    TSTNode<T> *middle;
    TSTNode<T> *right;
    const T digit;
    bool end;

    /**
     * Constructs a TSTNode.
     */
    TSTNode( const T& d ) : digit(d), end(0) {
        left = middle = right = nullptr;
    }

};

/**
 * Represents a Ternary Search Tree.
 */
class TST {

private:
    TSTNode<char> *root;

public:
    friend class Node;

    /**
     * Constructs a TST.
     */
    TST() : root(nullptr) {}

    /**
     * Destructs a TST.
     *
     * Implemented simply by calling the clear() method.
     */
    ~TST();

    /**
     * Clears the TST.
     */
    void clear();

    /**
     * Inserts a string to TST.
     */
    void insert(const string& str);

    /**
     * Finds whether a specified string is in the TST.
     *
     * Returns true if the string is in the TST and false otherwise.
     */
    bool find(const string& str);

    /**
     * Judges whether a specified string is a prefix in the TST.
     *
     * Returns true if the string is a prefix and false otherwise.
     */
    bool isPrefix(const string& str);

    /**
     * Judges whether the TST is empty.
     *
     * If there is no string in the TST, returns true; otherwise,
     * returns false.
     */
    bool isEmpty();

};

#endif // BOGGLEUTIL_H
