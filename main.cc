#include "board.h"
#include "exception.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>

using namespace std;

// a print function for vectors
template<typename T>
void print( std::vector<T> const & v ) {
	for ( auto i : v ) std::cout << i << ' ';
	std::cout << std::endl;
}

// get a subsector of vectors
template<typename T>
std::vector<T> slice(std::vector<T> const &v, int m, int n) {
    auto first = v.cbegin() + m;
    auto last = v.cbegin() + n;
 
    std::vector<T> vec(first, last);
    return vec;
}

// check if a card exists in a series of cards
bool containCard(char card, vector<char> cards) {
    for (auto c : cards) {
        if (card == c) {
            return true;
        }
    }
    return false;
}

// check the number with four different suits and print out the number a card has been
// played on the board already, print out "■" if the card is available as a legal move
// for the current player and print out "X" if the card has been discarded by any player
void printLine(string num, Board board, Player* player) {
    string card = num;
    // check num of clubs
    card.push_back('C');
    cout << num << "   ";
    if (containCard(num.at(0), board.getClubs())) {
        cout << num << "  ";
    } else if (player->containCard(card, player->getLegalMoves())){
        cout << "■  ";
    } else if (player->containCard(card, board.getAllDiscarded())){
        cout << "X  ";
    } else {
        cout << "   ";
    }

    card = num;
    // check num of diamonds
    card.push_back('D');
    if (containCard(num.at(0), board.getDiamonds())) {
        cout << num << "  ";
    } else if (player->containCard(card, player->getLegalMoves())){
        cout << "■  ";
    } else if (player->containCard(card, board.getAllDiscarded())){
        cout << "X  ";
    } else {
        cout << "   ";
    }

    card = num;
    // check num of hearts
    card.push_back('H');
    if (containCard(num.at(0), board.getHearts())) {
        cout << num << "  ";
    } else if (player->containCard(card, player->getLegalMoves())){
        cout << "■  ";
    } else if (player->containCard(card, board.getAllDiscarded())){
        cout << "X  ";
    } else {
        cout << "   ";
    }

    card = num;
    // check num of spades
    card.push_back('S');
    if (containCard(num.at(0), board.getSpades())) {
        cout << num << "  " << endl;
    } else if (player->containCard(card, player->getLegalMoves())){
        cout << "■  " << endl;
    } else if (player->containCard(card, board.getAllDiscarded())){
        cout << "X  " << endl;
    } else {
        cout << "   " << endl;
    }
}

// print out the whole board with the states with call 52 cards showing
// rank by rank
void printBoardAndPlayer(Board board, Player* player) {
    cout << "    C  " << "D  " << "H  " << "S  " << endl;
    printLine("A", board, player);
    printLine("2", board, player);
    printLine("3", board, player);
    printLine("4", board, player);
    printLine("5", board, player);
    printLine("6", board, player);
    printLine("7", board, player);
    printLine("8", board, player);
    printLine("9", board, player);
    printLine("T", board, player);
    printLine("J", board, player);
    printLine("Q", board, player);
    printLine("K", board, player);
}

// get the card with lowest rank in a bunch of cards
string getLowestCard(vector<string> cards) {
    string lowest = cards.at(0);
    // if the card is a Ace, just return that card since its the 
    // lowest rank
    if (lowest.at(0) == 'A') {
        return lowest;
    } else {
        // go through all cards and compare to the known lowest one
        for (auto card : cards) {
            if (card.at(0) == 'A') {
                lowest = card;
                return lowest;
            } else {
                if (lowest.at(0) == 'K') {
                    lowest = card;
                } else if ((lowest.at(0) == 'Q') && (card.at(0) != 'K')) {
                    lowest = card;
                } else if ((lowest.at(0) == 'J') && (card.at(0) != 'Q') && (card.at(0) != 'K')) {
                    lowest = card;
                } else if ((lowest.at(0) == 'T') && (card.at(0) != 'J')
                             && (card.at(0) != 'Q' )&& (card.at(0) != 'K')) {
                    lowest = card;
                } else if (card.at(0) < lowest.at(0)) {
                    lowest = card;
                }
            }
        }
    } 
    return lowest;
}


// how a computer will play the game
void computerPlay(Board board) {
    // get current player
    Player* player = board.getCurPlayer();
    // first print out the board
    cout << "Card on the table: " << endl;
    printBoardAndPlayer(board, player);
    // print out current player's hand
    cout << "Your hand: ";
    print(player->getHands());
    // print out current player's legal plays
    cout << "Legal plays: ";
    print(player->getLegalMoves());

    // if the computer player has no legal plays, then the player will automatically
    // discards the card with the lowest rank in its hand
    if (player->getLegalMoves().size() == 0) {
        string cardPlayed = getLowestCard(player->getHands());
        // set the player state to be discard with the card about to be discarded
        player->setState(StateType::Discard, cardPlayed);
        // add cardPlayed to the discarded vector
        player->addDiscarded(cardPlayed);
        // increment current player's score 
        player->incrementScore(cardPlayed.at(0));
        // print out player's action
        cout << "Player<" << player->getId() << "> " << "discards<" << cardPlayed << ">." << endl;
        // remove the discarded from hand
        player->removeFromHand(cardPlayed);
    } else {
        // if there is at least one legal play, play the card with the lowest rank
        string cardPlayed = getLowestCard(player->getLegalMoves());
        // set player state to be Legal with the lowest legal play
        player->setState(StateType::Legal, cardPlayed);
        // print out player's action
        cout << "Player<" << player->getId() << "> " << "plays<" << cardPlayed << ">." << endl;
        // remove the card played from hand
        player->removeFromHand(cardPlayed);
        // remove the card played from legalMoves
        player->removeFromLegalMoves(cardPlayed);
    }
}

void humanPlay(Board board) {
    // get current player
    Player* player = board.getCurPlayer();
    // record the command given by player
    string cmd;
    // record the card player input
    string card;
    // double check if player is a human player
    if (player->getType() == 'h') {
        // print out the whole board
        cout << "Card on the table: " << endl;
        printBoardAndPlayer(board, player);
        // print out player's hand
        cout << "Your hand: ";
        print(player->getHands());
        // print out player's legal moves
        cout << "Legal plays: ";
        print(player->getLegalMoves());
        
        // use a infinitie loop to catch ant invalid command 
        while (true) {
            // read in player's command
            cin >> cmd;
            // players input play
            if (cmd == "play") {
                // read in the card the player wants to play 
                cin >> card;
                // if the card is a legal move
                if (player->containCard(card, player->getLegalMoves())) {
                    // set the player state
                    player->setState(StateType::Legal, card);
                    // remove the card from hand
                    player->removeFromLegalMoves(card);
                    // remove the card from legal moves 
                    player->removeFromHand(card);
                    // print out player's action 
                    cout << "Player<" << player->getId() << "> " << "plays<" << card << ">." << endl;
                    // since its a valid command, breaks out the loop.
                    break;
                } else {
                    // if the card player trying to play is not a legal move,
                    // show warning and block that action
                    cout << "This is not a legal play." << endl;
                }
                // if player is discarding
            } else if (cmd == "discard") {
                // read in the card player is trying to diacard
                cin >> card;
                // if a player has legal moves, he may not discard
                if (player->getLegalMoves().size() != 0) {
                    cout << "You have a legal play. You may not discard." << endl;
                } else {
                    // if player has the card in his hand
                    if (player->containCard(card, player->getHands())) {
                        // set player's state
                        player->setState(StateType::Discard, card);
                        // add card to the discarded pile
                        player->addDiscarded(card);
                        // remove card from hand
                        player->removeFromHand(card);
                        // increment player's score
                        player->incrementScore(card.at(0));
                        // print out action
                        cout << "Player<" << player->getId() << "> " << "discards<" << card << ">." << endl;
                        break;
                    } else {
                        // print warning if a player is discarding a card he doesnt have
                        cout << "You do not have this card." << endl;
                    
                    }
                }
            } else if (cmd == "deck") {
                // print out the whole deck with 13 cards on each line 
                print(slice(board.getDeck(), 0, 13));
                print(slice(board.getDeck(), 13, 26));
                print(slice(board.getDeck(), 26, 39));
                print(slice(board.getDeck(), 39, 52));
            } else if (cmd == "quit") {
                // by throwing an exception to break out of the whole program
                throw Exception{};
            } else if (cmd == "ragequit") {
                // print out player has rage quited
                cout << "Player<" << player->getId() << "> ragequits. A computer will now take over." << endl;
                // update player's type to be a computer
                player->setType('c');
                // let a computer player finish current action
                computerPlay(board);
                break;
            } else {
                // if the command is not recognized, print out this error
                cout << "Invalid command" << endl;
            }
        }
    } else {
        cout << "Not a human player" << endl;
    }
}

// check if any player still have some card, aka round is not over
bool stillHaveCards(vector<Player*> allPlayers) {
    for (auto player : allPlayers) {
        if (player->getHands().size() != 0) {
            return true;
        }
    }
    return false;
}

// check if the game is over, aka someone has a score higher than 80
bool checkWinner(vector<Player*> allPlayers) {
    for (auto player : allPlayers) {
        if (player->getTotalScore() >= 80) {
            return true;
        }
    }
    return false;
}

// get the indexes for winners (including ties)
vector<int> getWinners(vector<Player*> allPlayers) {
    int lowest = 9999;
    vector<int> indexes;
    for (int i = 0; i < allPlayers.size(); i++) {
        if (allPlayers[i]->getTotalScore() < lowest) {
            indexes.clear();
            lowest = allPlayers[i]->getTotalScore();
            indexes.emplace_back(i);
        } else if (allPlayers[i]->getTotalScore() == lowest) {
            indexes.emplace_back(i);
        }
    } 
    return indexes;
}

// reset the players and the board
void clearEverything(vector<Player*> &allPlayers, Board &board) {
    for (auto player : allPlayers) {
        // make their current to be their old score
        player->setOldScore(player->getNewScore());
        // reset their newScore
        player->setNewScore(0);
        // set their new hand with a new deck
        player->setHands(slice(board.getDeck(), ((player->getId() - 1) * 13), (player->getId() * 13)));
        // empty discarded and legal moves
        player->clearDiscarded();
        player->clearLegalMoves();
        // clear the board
        board.clear();
    }
}


int main( int argc, char * argv[] ) {
    // initialize 52 cards used to play where each card is in the form <rank><suit> (e.g. 7S).
    vector<string> deck = { "AC", "2C", "3C", "4C", "5C", "6C", "7C", "8C", "9C", "TC",
        "JC", "QC", "KC", "AD", "2D", "3D", "4D", "5D", "6D", "7D", "8D", "9D", "TD",
        "JD", "QD", "KD", "AH", "2H", "3H", "4H", "5H", "6H", "7H", "8H", "9H", "TH",
        "JH", "QH", "KH", "AS", "2S", "3S", "4S", "5S", "6S", "7S", "8S", "9S", "TS",
        "JS", "QS", "KS" };

    // use a time-based seed for the default seed value
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    if ( argc > 1 ) {
		try {
			seed = std::stoi( std::string{ argv[1] } );
		} catch( std::invalid_argument & e ) {
			std::cerr << e.what() << std::endl;
			return 1;
		} catch( std::out_of_range & e ) {
			std::cerr << e.what() << std::endl;
			return -1;
		} // catch
	} // if

    std::default_random_engine rng{seed};

    // shuffle the deck for game play
    std::shuffle( deck.begin(), deck.end(), std::default_random_engine(seed) );
    
    // type of new player
    char type;
    // tmp variables to hold each new player's hand
    vector<string> hands;
    vector<Player*> allPlayers;
    try {
        // invite all 4 plays, set their types and assign their hands from
        // the deck depending on their player ID
        cout << "Is Player<1> a human (h) or a computer (c)" << endl;
        cin >> type;
        hands = slice(deck, 0, 13);
        Player player1 = { 1, type, hands };
        cout << "Is Player<2> a human (h) or a computer (c)" << endl;
        cin >> type;
        hands = slice(deck, 13, 26);
        Player player2 = { 2, type, hands };
        cout << "Is Player<3> a human (h) or a computer (c)" << endl;
        cin >> type;
        hands = slice(deck, 26, 39);
        Player player3 = { 3, type, hands };
        cout << "Is Player<4> a human (h) or a computer (c)" << endl;
        cin >> type;
        hands = slice(deck, 39, 52);
        Player player4 = { 4, type, hands };

        // add all 4 plays to the vector "allPlayers"
        allPlayers.emplace_back(&player1);
        allPlayers.emplace_back(&player2);
        allPlayers.emplace_back(&player3);
        allPlayers.emplace_back(&player4);

        // construct the board with the 4 players just added and the shuffled deck+
        Board board = {allPlayers, deck};
        // player who has the 7 of spades plays first and 7S is placed on the board,
        // move on to the next player online 
        cout << "A new round begins. It's Player" << board.getCurPlayer()->getId() 
                << "'s turn to play." << endl;
        // the first ever player has to play 7S
        board.getCurPlayer()->setState(StateType::Legal, "7S");
        // remove 7S from legal moves
        board.getCurPlayer()->removeFromLegalMoves("7S");
        // remove 7S from player's hand
        board.getCurPlayer()->removeFromHand("7S");
        // print out the action for playing 7S
        cout << "Player<" << board.getCurPlayer()->getId() << "> " << "plays<7S>." << endl;
        // notify the board that 7S has been played
        board.getCurPlayer()->notifyObservers();
        // switch to the next player
        board.switchPlayer();
        // keeps playing unntil we have a winner
        while (true) {
            // the current round does not end until all players are out of cards
            while(stillHaveCards(allPlayers)) {
                // get the current player
                Player* cur = board.getCurPlayer();
                // update player's legal moves
                cur->addNewLegalMoves(board.getAllLegalMoves());
                // check the type of current player
                if (cur->getType() == 'h') {
                    // run human helper if a human player
                    humanPlay(board);
                    // after playing, notify the board 
                    cur->notifyObservers();
                    // switch to the next player
                    board.switchPlayer();
            } else { 
                    // run cmoputer helper if a human player
                    computerPlay(board);
                    // after playing, notify the board 
                    cur->notifyObservers();
                    // switch to the next player
                    board.switchPlayer();
                } 
            }
            
            // when a round ends, check if there is any winner
            if (checkWinner(allPlayers)) {
                // print out each players discarded and score detail
                for (auto player : allPlayers) {
                    cout << "Player" << player->getId() << "'s discards: "; 
                    print(player->getDiscarded());
                    cout << "Player" << player->getId() << "'s score: " << player->getOldScore()
                         << " + " << player->getNewScore() << " = " << player->getTotalScore() << endl;
                }
                vector<int> winners = getWinners(allPlayers);
                // print out all winners
                for (auto index : winners) {
                    cout << "Player" << allPlayers[index]->getId() << " wins!" << endl;
                }
                // break out of the loop if there is winner
                break;
            } else {
                // if a round ends and there is no winner
                seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine rng{seed};
                // re-shuffle the deck
                shuffle( deck.begin(), deck.end(), default_random_engine(seed) );
                // set the deck for the board
                board.setDeck(deck);
                // reset the board and all players
                clearEverything(allPlayers, board);
                // a new round begins
                cout << "A new round begins. It's Player" << board.getCurPlayer()->getId() 
                << "'s turn to play." << endl;
            }
        }
    }// catch the quit command 
    catch (Exception) {
    }
     catch (ios::failure &) {
    } 
	return 0;
}

