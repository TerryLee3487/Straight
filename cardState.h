#ifndef __CARDSTATE_H__
#define __CARDSTATE_H__

#include <string>
using namespace std;
/* State types are:
    Legal -- a legal move was made by a player and the card is now placed on
             the board, more legal moves are produced if possible

    Discard -- The player had no legal moves and were forced to discard a 
               card 
*/
enum class StateType { Legal, Discard };

struct CardState {
    StateType type; // See above
    string card; // What card was played
};

#endif
