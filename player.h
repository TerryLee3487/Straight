#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <vector>
#include <string>
#include "subject.h"
#include "cardState.h"
using namespace std;

class Player: public Subject {
  // score carried over from last round
  int oldScore;
  // score gained in the current round
  int newScore;
  // 1, 2, 3, or 4
  int id;
  // type of a player, 'h' for human and 'c' for player
  char type;
  // all cards currently in the hand
  vector<string> hands;
  // all avaiable legal plays
  vector<string> legalMoves;
  // all discarded cards by player
  vector<string> discarded;
  // the move player made at this turn
  CardState move;

 public:
  Player( int id, char type, vector<string> hands);
  ~Player();

  // get the old score for a player
  int getOldScore();
  // get the new score for a player
  int getNewScore();
  // get the total score for a player
  int getTotalScore();
  // set the old score for a player
  void setOldScore(int score);
  // set the new score for a player
  void setNewScore(int score);
  // increment the newScore based on the first char of the card discarded
  void incrementScore(char num);
  // get the id of one player
  int getId();
  // get the type of a player
  char getType();
  // set the type of a player
  void setType(char type);
  // get a player's current hands
  vector<string> getHands();
  // set a player's current hands
  void setHands(vector<string> newHands);
  // remove a card from hands if exists
  void removeFromHand(string card);
  // remove a card from legalMoves if exists
  void removeFromLegalMoves(string card);
  // get all the legal moves of a player
  vector<string> getLegalMoves();
  // reset all legal moves
  void clearLegalMoves();
  // get all the discarded cards of a player
  vector<string> getDiscarded();
  // add a new card to the discarded pile 
  void addDiscarded(string card);
  // reset all discarded
  void clearDiscarded();
  // get the most recent move, what action (play/discard) and what card
  CardState getState();
  // set the last move
  void setState(StateType cardType, string card);

  // check if a card exists in a vector of cards, if it does
  // return true, else return false
  bool containCard(string card, vector<string> cards);
  // check if current hands contains a new legal moves, if a card
  // is a new legal move, add it to legalMoves and remove from hand 
  void addNewLegalMoves(vector<string> newMoves);
};

#endif
