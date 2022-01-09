#include "player.h"
#include <iostream>
using namespace std;

// cstr for Player 
Player::Player(int id, char type, vector<string> cards) : id{id},
    type{type}, hands{cards} {
        oldScore = 0;
        newScore = 0;
    }

// dstr for Player
Player::~Player() {}

// get the old score for a player
int Player::getOldScore() {
    return oldScore;
}

// get the new score for a player
int Player::getNewScore() {
    return newScore;
}

// get the total score for a player
int Player::getTotalScore() {
    return oldScore + newScore;
}

// set the old score for a player
void Player::setOldScore(int score) {
    oldScore = score;
} 

// set the new score for a player
void Player::setNewScore(int score) {
    newScore = score;
}

// increment the newScore based on the first char of the card discarded
void Player::incrementScore(char num) {
    if (num == 'A') {
        newScore += 1;
    } else if (num == 'T') {
        newScore += 10;
    } else if (num == 'J') {
        newScore += 11;
    } else if (num == 'Q') {
        newScore += 12;
    } else if (num == 'K') {
        newScore += 13;
    } else {
        newScore += num - 48;
    }
}

// get the id of one player
int Player::getId() {
    return id;
}

// get the type of a player
char Player::getType() {
    return type;
}

// set the type of a player
void Player::setType(char type) {
    this->type = type;
}

// get a player's current hands
vector<string> Player::getHands() {
    return hands;
}

// set a player's current hands
void Player::setHands(vector<string> newHands) {
    hands = newHands;
}

// remove a card from hands if exists
void Player::removeFromHand(string card) {
    for (int i = 0; i < hands.size(); i++) {
        if (hands[i] == card) {
            hands.erase(hands.begin()+i);
        }
    }
}

// remove a card from legalMoves if exists
void Player::removeFromLegalMoves(string card) {
    for (int i = 0; i < legalMoves.size(); i++) {
        if (legalMoves[i] == card) {
            legalMoves.erase(legalMoves.begin()+i);
        }
    }
}

// get all the legal moves of a player
vector<string> Player::getLegalMoves() {
    return legalMoves;
}

// reset all legal moves
void Player::clearLegalMoves() {
    legalMoves.clear();
}

// get all the discarded cards of a player
vector<string> Player::getDiscarded() {
    return discarded;
}

// add a new card to the discarded pile 
void Player::addDiscarded(string card) {
    discarded.emplace_back(card);
}

// reset all discarded
void Player::clearDiscarded() {
    discarded.clear();
}

// get the most recent move, what action (play/discard) and what card
CardState Player::getState() {
    return move;
}

// set the last move
void Player::setState(StateType stateType, string card) {
    CardState state { stateType, card };
    move = state;
}

// check if a card exists in a vector of cards, if it does
// return true, else return false
bool Player::containCard(string card, vector<string> cards) {
    for (auto c : cards) {
        if (card == c) {
            return true;
        }
    }
    return false;
}

// check if current hands contains a new legal moves, if a card
// is a new legal move, add it to legalMoves and remove from hand 
void Player::addNewLegalMoves(vector<string> newMoves) {
    for (auto c : newMoves) {
        if (containCard(c , hands) && !containCard(c, legalMoves)) {
            legalMoves.emplace_back(c);
        } 
    }
}
