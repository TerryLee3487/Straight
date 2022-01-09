#include "board.h"
#include <string>
#include <vector>

// depends on the card that was played, determine what new legal moves was created
// for instance, 7 enables 6 and 8 of the same suit to play, 6 or less enables one smaller
// and 8 or more enable one bigger
vector<string> Board::produceLegalMoves(string card) {
    vector<string> newLegalMoves;
    char num = card.at(0);
    if (num != 'A' && num != 'K') {
        if (num == '2') {
            string legalMove = "A";
            legalMove.push_back(card.at(1));
            newLegalMoves.emplace_back(legalMove);
        } else if (num == '9') {
            string legalMove = "T";
            legalMove.push_back(card.at(1));
            newLegalMoves.emplace_back(legalMove);
        } else if (num == 'T') {
            string legalMove = "J";
            legalMove.push_back(card.at(1));
            newLegalMoves.emplace_back(legalMove);
        } else if (num == 'J') {
            string legalMove = "Q";
            legalMove.push_back(card.at(1));
            newLegalMoves.emplace_back(legalMove);
        } else if (num == 'Q') {
            string legalMove = "K";
            legalMove.push_back(card.at(1));
            newLegalMoves.emplace_back(legalMove);
        } else {
            if (num == '7') {
                string legalMove = "6";
                legalMove.push_back(card.at(1));
                newLegalMoves.emplace_back(legalMove);
                legalMove.clear();
                legalMove = "8";
                legalMove.push_back(card.at(1));
                newLegalMoves.emplace_back(legalMove);
            } else if (num < '7') {
                string legalMove;
                legalMove.push_back(char(num-1));
                legalMove.push_back(card.at(1));
                newLegalMoves.emplace_back(legalMove);
            } else if (num > '7') {
                string legalMove;
                legalMove.push_back(char(num+1));
                legalMove.push_back(card.at(1));
                newLegalMoves.emplace_back(legalMove);
            }
        }
    }
    return newLegalMoves;
}

// get the number of player who has the 7 of spade to start the game
int Board::get7Spades() {
    int playerNum = 0;
      for (int i = 0; i < allPlayers.size(); i++) {
        if (allPlayers[i]->containCard("7S", allPlayers[i]->getHands())) {
          playerNum = i;
        }
      }
    return playerNum;
}

// cstr for board
Board::Board(vector<Player*> allPlayers, vector<string> deck) :
    allPlayers{allPlayers}, deck{deck} {
        // attach all players to be the subject
        for (auto player : allPlayers) {
            player->attach(this);
        }
        // curPlayer is initialized to who has the 7 of spades 
        curPlayer = this->get7Spades();
        // first group of legal plays will be all 7s
        legalMoves = {"7C", "7D", "7H", "7S"};
    }

// dstr for board
Board::~Board() {
    // detach this board from all players
    for (auto player : allPlayers) {
        player->detach(this);
    }
}

// set the rules for observing all players 
void Board::notify() {
    CardState move = allPlayers[curPlayer]->getState();
    // if it was a discard
    if (move.type == StateType::Discard) {
        discarded.emplace_back(move.card);
    } 
    // if a card was played
    else if (move.type == StateType::Legal) {
        // check if suit of the card played, if its less than 7, insert it at the 
        // front of the corresponding pile, otherwise, put it in the back of the pile
        if (move.card.at(1) == 'C') {
            if (move.card.at(0) <= '7' || move.card.at(0) == 'A') {
                clubs.insert(clubs.begin(), move.card.at(0));
            } else {
                clubs.emplace_back(move.card.at(0));
            }
        } else if (move.card.at(1) == 'D') {
            if (move.card.at(0) <= '7' || move.card.at(0) == 'A') {
                diamonds.insert(diamonds.begin(), move.card.at(0));
            } else {
                diamonds.emplace_back(move.card.at(0));
            }
        } else if (move.card.at(1) == 'H') {
            if (move.card.at(0) <= '7' || move.card.at(0) == 'A') {
                hearts.insert(hearts.begin(), move.card.at(0));
            } else {
                hearts.emplace_back(move.card.at(0));
            }
        } else if (move.card.at(1) == 'S') {
            if (move.card.at(0) <= '7' || move.card.at(0) == 'A') {
                spades.insert(spades.begin(), move.card.at(0));
            } else {
                spades.emplace_back(move.card.at(0));
            }
        }

        // produce more legal moves according to the card played
        vector<string> newLegalMoves = produceLegalMoves(move.card);
        // add all the new legal plays
        for (auto move : newLegalMoves) {
            legalMoves.emplace_back(move);
        }
    }
}

// get the player playing right now
Player * Board::getCurPlayer() {
    return allPlayers[curPlayer];
}

// set the current player number to the given number 
void Board::setCurPlayerNum(int num) {
    curPlayer = num;
}

// get the deck thats playing right now
vector<string> Board::getDeck() {
    return deck;
}

// set the deck thats playing right now
void Board::setDeck(vector<string> newDeck) {
    deck = newDeck;
}

// get the clubs currently on the board
vector<char> Board::getClubs() {
    return clubs;
}

// get the diamonds currently on the board
vector<char> Board::getDiamonds() {
    return diamonds;
}

// get the hearts currenly on the board
vector<char> Board::getHearts() {
    return hearts;
}

// get the spades currently on the board
vector<char> Board::getSpades() {
    return spades;
}

// get all the legal moves on the board
vector<string> Board::getAllLegalMoves() {
    return legalMoves;
}

// get all discarded on the board
vector<string> Board::getAllDiscarded() {
    return discarded;
}

// depends on the player now, switch to the next player
void Board::switchPlayer() {
    if (curPlayer == 0) {
        curPlayer = 1;
    } else if (curPlayer == 1) {
        curPlayer = 2;
    } else if (curPlayer == 2) {
        curPlayer = 3;
    } else if (curPlayer == 3) {
        curPlayer = 0;
    }
}

// reset the board
void Board::clear() {
    // the first player is who has the 7S now
    curPlayer = this->get7Spades();
    // clear out all piles
    clubs.clear();
    diamonds.clear();
    hearts.clear();
    spades.clear();
    // reset all legal moves
    legalMoves = {"7C", "7D", "7H", "7S"};
}




