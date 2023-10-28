//Uno
#include <iostream>
#include <vector>
#include <string>
#include "Player.h"
#include <ctime>
#include <math.h>
#include <windows.h> //for text color
                    
//vector <std::string> cards;
std::vector <std::string> deck = {
    "r0","r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9","rSkip", "rSkip", "rReverse", "rReverse", "r+2", "r+2",
    "b0","b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9", "b1", "b2", "b3", "b4", "b5", "b6", "b7", "b8", "b9","bSkip", "bSkip", "bReverse", "bReverse", "b+2", "b+2",
    "g0","g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8", "g9", "g1", "g2", "g3", "g4", "g5", "g6", "g7", "g8", "g9","gSkip", "gSkip", "gReverse", "gReverse", "g+2", "g+2",
    "y0","y1", "y2", "y3", "y4", "y5", "y6", "y7", "y8", "y9", "y1", "y2", "y3", "y4", "y5", "y6", "y7", "y8", "y9","ySkip", "ySkip", "yReverse", "yReverse", "y+2", "y+2",
    "Wild", "Wild", "Wild", "Wild", "Wild+4", "Wild+4", "Wild+4", "Wild+4"
};

std::vector <std::string> discardPile;

int currentPlayer = 0;

std::vector <Player> players;

//setup functions
void setup();
void initPlayerArr();
void initHands();

void shuffle(std::vector <std::string>& deckV);
void draw(std::vector <std::string>& handV);
void discard(std::string playedCard, std::vector <std::string>& handV);
void discard();
int canPlay(std::string card);
void playCard(std::string card, std::string* cardPTR);
void nextTurn(int revOrSkip);
void game();
int checkCardType(std::string card);
void chooseColor(std::string* currentWildCard);
void keepTurnInBounds();
int getCardIndex(std::string card);
void display();
std::string getInput();
int getColor(char currentCardColor);
bool checkIfInputValid(std::string input);
void printCardColor(std::string card);
void checkWin();
void checkDeckEmpty();

//for printing the game
void printBoard();
void printTwoPlayerGame();
void printThreePlayerGame();

//for debugging;
void debugDeck(std::vector <std::string>& deck);
void say(std::string str);

int main() {
    /*
    for (int i = 0; i < (sizeof(back)/sizeof(back[0])); i++) {
        std::cout << back[i] << " " << back[i] << std::endl;
    }
    */
    setup();
    game();
    return 0;
}

void shuffle(std::vector <std::string>& deckV) {
    srand(time(NULL));
    int randCard;
    std::string temp;

    /*
    std::cout << "before shuffle" << std::endl;
    for (int i = 0; i < deck.size(); i++) {
        std::cout << " " << deck[i];
    }
    std::cout << "size" << deck.size();

    std::cout << "\n\n\n";
    */

    /*
        //method 1 (array)
        for (int i = 0; i < 108; i++) {
            randCard = rand() % 108;
            temp = deck[i];
            deck[i] = deck[randCard];
            deck[randCard] = temp;
        }
    */
    /*
    //better for vectors because they change (method 2)
    for (int i = 0; i < deck.size(); i++) {
        randCard = rand() % deck.size();
        temp = deck[i];
        deck[i] = deck[randCard];
        deck[randCard] = temp;
        //std::cout << "size" << deck.size();
        //system("pause");
    }
    */

    //method 3 passing in a deck to be shuffled
    for (int i = 0; i < deckV.size(); i++) {
        randCard = rand() % deckV.size();
        temp = deckV[i];
        deckV[i] = deckV[randCard];
        deckV[randCard] = temp;
        //std::cout << "size" << deck.size();
        //system("pause");
    }


    //std::cout << "after shuffle" << std::endl;
    //debugDeck();
}

void draw(std::vector <std::string>& handV) {
    std::string pulledCard = deck.back();
    handV.push_back(pulledCard);

    deck.pop_back();

    //debugDeck();
}

void debugDeck(std::vector <std::string>& deck) {
    std::cout << "\n\n\n";
    for (int i = 0; i < deck.size(); i++) {
        std::cout << " " << deck[i];
    }

    std::cout << "\n size " << deck.size();
}

int canPlay(std::string card) {//return 0 if card cannot be played and an int 1-6 indicating a matching cards type
    int canPlay = 0;
    std::string lastPlayed = discardPile[discardPile.size() - 1];

    //check color
    if (lastPlayed[0] == card[0]) {
        say("colors match!");

        //we know the colors match but what type of card is it?
        canPlay = checkCardType(card);
    }

    //check numbers
    if (lastPlayed[1] == card[1]) {
        say("numbers match!");
        canPlay = 1;
    }
    //checking action cards
    if (lastPlayed[1] == 'S' && card[1] == 'S') {
        say("skip match!");
        canPlay = 2;
    }
    if (lastPlayed[1] == 'R' && card[1] == 'R') {
        say("reverse match!");
        canPlay = 3;
    }
    if (lastPlayed[1] == '+' && card[1] == '+') {
        say("plus 2 match!");
        canPlay = 4;
    }

    //check wild cards
    if (card[0] == 'W') {//if it's a wild you don't need to know if it was a match with the last

        if (card.length() > 4) {
            //say("Wild+4!");
            canPlay = 5;
        }
        else {
            //say("Wild!");
            canPlay = 6;
        }

    }

    return canPlay;
}

void say(std::string str) {
    std::cout << str << std::endl;
}

void playCard(std::string card, std::string* cardPTR) {
    enum cardTypes { NUM = 1, SKIP, REV, PLUS2, WILD4, WILD, PLUS4 };

    int cardType = canPlay(card);

    if (cardType) {
        switch (cardType) {
        case NUM:
            discard(card, players[currentPlayer].hand);
            checkWin();
            nextTurn(NUM); //next player
            break;
        case SKIP:
            discard(card, players[currentPlayer].hand);
            checkWin();
            nextTurn(SKIP);
            break;
        case REV:
            discard(card, players[currentPlayer].hand);
            checkWin();
            nextTurn(REV);
            break;
        case PLUS2:
            discard(card, players[currentPlayer].hand);
            checkWin();
            nextTurn(PLUS2);
            break;
        case WILD4:
            chooseColor(cardPTR);
            //debugDeck(players[currentPlayer].hand);
            discard(*cardPTR, players[currentPlayer].hand); //discard modified card
            checkWin();
            nextTurn(PLUS4); //just treat it as a number card because nothing more happens
            break;
        case WILD:
            chooseColor(cardPTR);
            //debugDeck(players[currentPlayer].hand);
            discard(*cardPTR, players[currentPlayer].hand); //discard modified card
            checkWin();
            nextTurn(NUM); //just treat it as a number card because nothing more happens
            break;
        }
    }
    else {
        say("not a match!");
    }

}

//int currentPlayer = 0;
void nextTurn(int actionType) {//takes in a 1 for a non turn modifing card, 3 for a reverse and a 2 for a skip card
    enum actions { REV = 3, SKIP = 2, PLUS2 = 4, PLUS4 = 7 };

    //static int currentPlayer = 0;
    static bool clockwise = true;
    bool skip = false;
    bool plus2 = false;
    bool plus4 = false;

    //handle the card that was played
    switch (actionType) {
    case REV:
        clockwise = !clockwise;
        break;
    case SKIP:
        skip = true;
        break;
    case PLUS2:
        plus2 = true;
        break;
    case PLUS4:
        plus4 = true;
        break;
    }
    /*
        //wrapping around the number of players
        if (currentPlayer < 0) {
            currentPlayer = players.size() + currentPlayer;
        }
        if (currentPlayer > (players.size() - 1)) {
            currentPlayer = -1 + (currentPlayer - (players.size() - 1));
        }
    */
    //std::cout << currentPlayer << std::endl;
    if (clockwise) {
        if (skip) {
            currentPlayer += 2;
        }
        else {
            currentPlayer++;
        }
    }
    else {
        if (skip) {
            currentPlayer -= 2;
        }
        else {
            currentPlayer--;
        }
    }

    keepTurnInBounds();//go to the correct next index of player array

    //std::cout << currentPlayer;

    //handling cards that make the next players draw after the next player is determined
    if (plus2) {
        checkDeckEmpty();
        for (int i = 0; i < 2; i++)
            draw(players[currentPlayer].hand);
    }

    if (plus4) {
        checkDeckEmpty();
        for (int i = 0; i < 4; i++)
            draw(players[currentPlayer].hand);
    }
}

void discard(std::string playedCard, std::vector <std::string>& handV) {//takes in the card being discarded and the hand it's in
    std::string discarded;

    for (int i = 0; i < handV.size(); i++) {
        if (handV[i] == playedCard) {
            discarded = handV[i];
            handV.erase(handV.begin() + i);
            discardPile.push_back(discarded);
            break;
        }
    }
}

void discard() {//discards from top of deck
    std::string discarded;

    discarded = deck[deck.size() - 1];
    //std::cout << "end of deck = " << *(deck.end() - 1) << std::endl;
    deck.erase((deck.end() - 1));
    discardPile.push_back(discarded);
}

void setup() {
    initPlayerArr();
    shuffle(deck);
    initHands();

    //say("discard pile before");
    //debugDeck(discardPile);


    //say("deck before");
    //debugDeck(deck);

    discard();
    //say("discard pile after");
    //debugDeck(discardPile);

    //say("deck after");
    //debugDeck(deck);
}

void initPlayerArr() {
    int playerCount;

    std::cout << "How many players? (2-4)" << std::endl;
    std::cin >> playerCount;

    switch (playerCount) {
    case 2: {
        Player p1;
        Player p2;

        players.push_back(p1);
        players.push_back(p2);


        //doesn't work
        //p1.hand.push_back("test1");
        //p2.hand.push_back("test2");

        //works
        //players[0].hand.push_back("test1");
        //players[1].hand.push_back("test2");


        //for (int i = 0; i < 2; i++)
        //  std::cout << players[i].hand[(players[i].hand.size() - 1)] << std::endl;
    }
          break;
    case 3: {
        Player p1;
        Player p2;
        Player p3;

        players.push_back(p1);
        players.push_back(p2);
        players.push_back(p3);
    }
          break;
    case 4: {
        Player p1;
        Player p2;
        Player p3;
        Player p4;

        players.push_back(p1);
        players.push_back(p2);
        players.push_back(p3);
        players.push_back(p4);
    }
          break;
    }
}

void initHands() {
    
    for (int i = 0; i < players.size(); i++) {//give every player 7 cards
        for (int j = 0; j < 7; j++)
        {
            draw(players[i].hand);
        }
    }

    //for (int i = 0; i < players.size(); i++) 
    //debugDeck(players[i].hand);

}

void game() {
    std::string card;

    /*for debugging checkDeckEmpty()
    int deckSize = deck.size();

    for (int i = 0; i < deckSize; i++)
        draw(discardPile);

    say("deck after");
    debugDeck(deck);
    */

    /*
    players[currentPlayer].hand.push_back("Wild");
    players[currentPlayer].hand.push_back("Wild+4");

    players[currentPlayer].hand.push_back("rSkip");
    players[currentPlayer].hand.push_back("bSkip");
    players[currentPlayer].hand.push_back("gSkip");
    players[currentPlayer].hand.push_back("ySkip");

    players[currentPlayer].hand.push_back("rReverse");
    players[currentPlayer].hand.push_back("gReverse");
    players[currentPlayer].hand.push_back("bReverse");
    players[currentPlayer].hand.push_back("yReverse");

    players[currentPlayer].hand.push_back("r+2");
    players[currentPlayer].hand.push_back("g+2");
    players[currentPlayer].hand.push_back("b+2");
    players[currentPlayer].hand.push_back("y+2");
    */

    while (1) {
        display();

        //tried to do ascii art lol
        //std::cout << "\n";
        //displayHand();

        card = getInput();
        playCard(players[currentPlayer].hand[getCardIndex(card)], &players[currentPlayer].hand[getCardIndex(card)]);
        //checkWin();
        system("pause");
        system("cls");
    }
}

void display() {
    
    //debugging stuff
    /*
    std::cout << "current player = " << currentPlayer << std::endl;

    say("discarded ");
    debugDeck(discardPile);

    say("Your hand ");
    debugDeck(players[currentPlayer].hand);
    */

    printBoard();
}

std::string getInput() {
    std::string card;

    do {
        say("\nplease enter a card");
        std::cin >> card;
        
        //checking for commands
        if (card == "help") {
            std::cout << "\n\nList of Commands\n----------------------\n1.) help: brings up this list \n2.) draw: draws a card from the deck into your hand\n\nHow to Play\n----------------------\nType the name of a card to play it." << std::endl;
        }
        if (card == "draw") {
            system("cls");
            checkDeckEmpty();
            draw(players[currentPlayer].hand);
            display();
            
            //more ascii art stuff
            //std::cout << "\n";
            //displayHand();
        }
        if (!checkIfInputValid(card) && card != "help" && card != "draw"){
            std::cout << "Invalid input!" << std::endl;
        }

    } while (card == "help" || card == "draw" || !checkIfInputValid(card)); //loop if input is a command or invalid
 
    return card;
}

int checkCardType(std::string card) {
    int cardType = 0;

    //what type of card is it?
    if (isdigit(card[1])) {
        //can play, number type
        cardType = 1;
    }
    else if (card[1] == 'S') {
        //can play, of skip type
        cardType = 2;
    }
    else if (card[1] == 'R') {
        //can play, of reverse type
        cardType = 3;
    }
    else if (card[1] == '+') {
        //can play, of plus 2 type
        cardType = 4;
    }

    return cardType;
}

void chooseColor(std::string* currentWildCard) {
    char color;

    say("please pick a color");
    std::cin >> color;

    (*currentWildCard).insert(0, 1, color);

    //std::cout << currentWildCard << std::endl;
    //std::cout << *currentWildCard << std::endl;

    //say("current players hand ");
    //debugDeck(players[currentPlayer].hand);
}

void keepTurnInBounds() {

    //wrapping around the number of players
    if (currentPlayer < 0) {
        currentPlayer = players.size() + currentPlayer;
    }
    if (currentPlayer > (players.size() - 1)) {
        currentPlayer = -1 + (currentPlayer - (players.size() - 1));
    }

}

int getCardIndex(std::string card) {

    for (int i = 0; i < players[currentPlayer].hand.size(); i++) {
        if (players[currentPlayer].hand[i] == card) {
            return i;
        }
    }

}

int getColor(char currentCardColor) {
    if (currentCardColor == 'r')
        return 12;
    
    if (currentCardColor == 'g')
        return 10;

    if (currentCardColor == 'b')
        return 9;

    if (currentCardColor == 'y')
        return 14;
    
    if (currentCardColor == 'W' || currentCardColor == ' ');
        return 7;
}

bool checkIfInputValid(std::string input) {//checks to see if a card is in hand

    for (int i = 0; i < players[currentPlayer].hand.size(); i++) {
        if (input == players[currentPlayer].hand[i]) {
            return true;//valid card
        }
    }

    return false;
}

void printBoard() {
   //determine how many players are in the game
    switch (players.size()) {
    case 2:
        printTwoPlayerGame();
    break;
    case 3:
        printThreePlayerGame();
    break;
    };
}

void printTwoPlayerGame() {
std::string lastPlayed = discardPile[discardPile.size() - 1];

//first print the other players hidden hand

//formatting
    std::cout << "  ";
    printf("Player %d's hand:", !currentPlayer + 1);
    for (int i = 0; i < players[!currentPlayer].hand.size(); i++) {
        std::cout << "X ";
    }

    //print the discard pile
    std::cout << "\n\n\n      " << "X ";
    printCardColor(lastPlayed); //add some wow
    std::cout << "\n\n\n";

    //show the current players hand
    printCardColor(" ");
    printf("Your hand (Player: %d):", currentPlayer + 1);
    for (int i = 0; i < players[currentPlayer].hand.size(); i++) {
        //std::cout << players[currentPlayer].hand[i] << " ";
        printCardColor(players[currentPlayer].hand[i]);
        std::cout << " ";
    }
    //go back to white text after printing hand
    printCardColor(" ");
    
}

void printThreePlayerGame() {
    std::string lastPlayed = discardPile[discardPile.size() - 1];
    
    //determining to orientation to print out
    switch (currentPlayer) {
    case 0:
        //print the third player
        std::cout << "  ";
        for (int i = 0; i < players[2].hand.size(); i++) {
            std::cout << "X ";
        }
        std::cout << "\n";

        //print the discard pile and second player
        for (int i = 0; i < players[1].hand.size(); i++) {

            //print the discard pile 3 cards down
            if (i == 2)
                std::cout << "   " << "X " << lastPlayed;

            //print after the end of the top players hand
            for (int j = 0; j < ((players[1].hand.size() * 2) + 2); j++) {
                std::cout << " ";

                if (i == 2 && j == (players[1].hand.size() * 2) + 1) {
                    std::cout << "\b\b\b\b\b\b\b";
                }
            }

            std::cout << "X" << std::endl;
        }

        //print the first players hand
        for (int i = 0; i < players[currentPlayer].hand.size(); i++) {
            std::cout << players[currentPlayer].hand[i] << " ";
        }

    break;
    case 1:
        //print the discard pile, first, and second players
        for (int i = 0; i < players[1].hand.size(); i++) {

            //print the discard pile 3 cards down
            if (i == 2)
                std::cout << "   " << "X " << lastPlayed;

            //print after the end of the top players hand
            for (int j = 0; j < ((players[1].hand.size() * 2) + 2); j++) {
                std::cout << " ";

                if (i == 2 && j == (players[1].hand.size() * 2) + 1) {
                    std::cout << "\b\b\b\b\b\b\b";
                }
            }

            std::cout << "X" << std::endl;
        }

        //print the first players hand
        for (int i = 0; i < players[currentPlayer].hand.size(); i++) {
            std::cout << players[currentPlayer].hand[i] << " ";
        }
        break;
    }
}

//prints a card to the console with the correct color
void printCardColor(std::string card) {
    HANDLE  hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //handling the color of the cards
    SetConsoleTextAttribute(hConsole, getColor(card[0]));//getting the correct color to print
    
    std::cout << card;
}

void checkWin() {
    if (players[currentPlayer].hand.size() == 0)
        std::cout << "PLAYER " << currentPlayer + 1 << " WINS!!";
}

void checkDeckEmpty() {

    if (deck.size() == 0) {
        shuffle(discardPile);
        //put it back into the deck
        for (int i = 0; i < discardPile.size(); i++)
            deck.push_back(discardPile[i]);
    }

}