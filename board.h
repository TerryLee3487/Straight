#ifndef __BOARD_H__
#define __BOARD_H__
#include "observer.h"
#include "player.h"
#include "cardState.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class Board: public Observer {
    vector<Player*> allPlayers;
    int curPlayer;
    vector<string> deck;
    vector<char> clubs;
    vector<char> diamonds;
    vector<char> hearts;
    vector<char> spades;
    vector<string> legalMoves;
    vector<string> discarded;
  
    vector<string> produceLegalMoves(string card);

    // get whichever player has the 7 of sapde to start the game
    int get7Spades();

    template<typename T>
    void print( std::vector<T> const & v ) {
	    for ( auto i : v ) std::cout << i << ' ';
	    std::cout << std::endl;
    }

 public:
  Board(vector<Player*> allPlayers, vector<string> deck);
  void notify() override;
  ~Board();

  // get the player thats currently playing
  Player * getCurPlayer();
  // set the curPlayer with a player number
  void setCurPlayerNum(int num);
  // get the deck thats playing right now
  vector<string> getDeck();
  // set the deck thats playing right now
  void setDeck(vector<string> newDeck);
  // get the clubs currently on the board
  vector<char> getClubs();
  // get the diamonds currently on the board
  vector<char> getDiamonds();
  // get the hearts currenly on the board
  vector<char> getHearts();
  // get the spades currently on the board
  vector<char> getSpades();
  // get all the legal moves on the board
  vector<string> getAllLegalMoves();
  // gett all discarded
  vector<string> getAllDiscarded();
  // switch to the next player to play
  void switchPlayer();
  // reset the board
  void clear();
};

#endif
