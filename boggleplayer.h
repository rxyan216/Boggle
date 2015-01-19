#ifndef BOGGLEPLAYER_H
#define BOGGLEPLAYER_H

#include <algorithm>
#include <vector>
#include <string>
#include <locale>
#include <random>

#include "baseboggleplayer.h"
#include "boggleutil.h"

using std::pair;
using std::vector;
using std::string;
using std::locale;
using std::random_device;

/**
 * Represents a boggle.
 *
 * BogglePlayer contains a pointer to the board and a TST.
 */
class BogglePlayer: public BaseBogglePlayer {

private:
    unsigned int rows;
    unsigned int cols;
    Node*** board;
    TST tst;

public:
    /**
     * Constructs a BogglePlayer.
     */
    BogglePlayer() : rows(0), cols(0), board(nullptr) {}

    /**
     * Destructs a BogglePlayer.
     */
    ~BogglePlayer();

    /**
     * Builds the lexicon.
     *
     * Takes as argument a set containing the words specifying the
     * official lexicon to be used for the game. Each word in the
     * set will be a string consisting of lowercase letters a-z only.
     */
    void buildLexicon(const set<string>& word_list);

    /**
     * Sets the board.
     *
     * Takes two arguments: the number of rows and columns in the
     * board, and an array of arrays of strings representing what
     * is shown on the face of dice on a Boggle board.
     */
    void setBoard(unsigned int rows, unsigned int cols, string** diceArray);

    /**
     * Gets all the words which both board and lexicon contain.
     *
     * Takes two arguments: an int specifying a minimum word length,
     * and a pointer to a set of strings.
     */
    bool getAllValidWords(unsigned int minimum_word_length,
                                        set<string>* words);

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
    bool isInLexicon(const string& word_to_check);

    /**
     * Checks whether a specified word is on the board.
     *
     * Takes as argument a string passed by reference. It determines
     * whether the string can be found by following an acyclic simple
     * path on the board specified by the most recent call to setBoard().
     */
    vector<int> isOnBoard(const string& word_to_check);

    /**
     * Used for testing.
     */
    void getCustomBoard(string** &new_board, unsigned int *rows,
                                             unsigned int *cols);

};

#endif // BOGGLEPLAYER_H
