#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include<time.h>

//value 1-13?

// Structure for card
typedef struct card {
    char value[3];
    char suit[2];
    bool hidden;
    struct card *next;
} Card;


void gameMove(Card **newColumn, struct card *foundation[4], char selectedColumn[2], char selectedSourceCardValue[3], char selectedSourceCardSuit[2], char destinationColumn[2]);

void endGame();

void startPlayPhase(Card* deck, Card *column[]);

/**
 * Creates a struct of type Card. Holds the suit and value.
 * TODO: The struct also needs to hold a boolean representing if the card is face up or face down.
 * @param value
 * @param suit
 * @return
 */
Card * createCard(char *value,char *suit){
    Card * newCard;

    newCard = (Card*)malloc(sizeof (Card));

    strcpy(newCard->value, value);
    strcpy(newCard->suit, suit);
    newCard->hidden=false;


    newCard->next = NULL;

    return newCard;
}


/**
 * Prints out the entire deck.
 * @param list
 */
void printCardList(Card *list) {
// list points to the head of a linked list of Flights
    int count = 1;
    printf("Deck---------------------\n");
    while (list != NULL) { // while not at the end
        char *value = list->value;
        char *suit = list->suit;
        printf("Card%d: %s%s\n",count,value,suit);

// etc. etc.
        count = count + 1;
        list = list->next; // move to next node
    }
    printf("\n\n");
}


/**
 * Creates a linked list representing a unshuffled deck.
 * @return
 */
Card * createDeck(){
    Card *deck= NULL;

    char suits[4][2] = {"S","H","D","C"};
    char values[13][3] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};

    for(int i=0;i<4;i++){
        Card *temp;



        for(int j=12;j>=0;j--){
            temp=createCard(values+j,suits+i);
            temp->next = deck;
            deck=temp;
        }
    }
    return deck;
}

/**
 * Creates a random number
 * @return
 */

int random(){


    srand(time(0));
    int l = 0;
    int u = 9999;

        int num = (rand() % (u - l + 1)) + l;
        return num;

}


/**
 * Shuffle a card deck into a new linked list which will be the new deck in a random order.
 * @return
 */

// NOT WORKING YET

Card * shuffleDeck(Card* head_ref){

    Card *shuffledeck = NULL;

    Card* current = head_ref;
    Card* next;
    Card* temp;

    shuffledeck = current;
    current=current->next;
    shuffledeck->next=shuffledeck;

    for (int i=0;i<51;i++){

        int ran = random();

        //current = current -> next;

        for (int k=0; k < ran; k++){
            shuffledeck = shuffledeck->next;
        }
        temp = current -> next;
        current -> next = shuffledeck -> next;
        shuffledeck -> next = current;
        current = temp;

    }

    temp = shuffledeck;

    for (int o = 0; o < 51; o++){
        temp = temp->next;
    }
    temp -> next = NULL;

    return shuffledeck;
}

/**
 * Reverses a single linked list
 * @param head_ref Pointer to head of linked list.
 */
void reverseList(Card** head_ref)
{
    Card* prev   = NULL;
    Card* current = *head_ref;
    Card* next;
    while (current != NULL)
    {
        next  = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    *head_ref = prev;
}


/**
 * Deals the cards from the deck into linked lists representing the columns.
 * @param deck The deck of cards represented as single linked list
 * @param column The array of linked lists.
 */
void dealCards(Card *deck,Card *column[]) {


    int columnValues[] = {1, 6, 7, 8, 9, 10, 11};

    Card *currentCard = (Card *) malloc(sizeof(Card));

    currentCard = deck;
    for (int i = 0; i < 52; i++) {
        for (int j = 0; j < 7; j++) {
            int allowedAmountOfCards = columnValues[j];
            int currentIndex = 0;


            Card *currentPlacement = column[j];


            while (currentPlacement != NULL && currentIndex < allowedAmountOfCards) {
                currentPlacement = currentPlacement->next;
                currentIndex++;
            }
            if (currentIndex < allowedAmountOfCards) {
                currentPlacement = createCard(currentCard->value, currentCard->suit);
                currentPlacement->next = column[j];
                column[j] = currentPlacement;


                currentCard = currentCard->next;
            }
        }
    }
}


// Doesn't work
Card * extractLast(Card card){
    /*
    Card returnCard;
    while(card.next!=NULL){
        card=*card.next;
    }
    returnCard=card;
    card=NULL;
    return returnCard;
     */
}


/**
 * Prints out the game state. Should be called after each input and action.
 * @param column is the pointer to the array of linked lists containing the 7 columns of the game.
 */
void printGameState(Card * column[]){
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n\n");

    for(int j=0;j<52;j++) {
        bool finished=true;                                     // @finished Helps us keep track of the columns.
                                                                // Should switch to false if there are still cards needing to be shown in the columns.
                                                                // Will remain true if all cards have been shown.
        for (int i = 0; i < 7; i++) {
            Card * currentCard=column[i];

            if(currentCard==NULL){
                printf("\t");
            } else {
                for(int l=0;l<j;l++) {
                    if(currentCard!=NULL) {
                        currentCard = currentCard->next;
                    }
                }
                if(currentCard==NULL){
                    printf("\t");
                } else{
                    if(!currentCard->hidden){
                        printf("%s%s\t", currentCard->value, currentCard->suit);
                    } else{printf("[]\t");}
                    finished=false;                             // @finished Will switch to false if there was a card
                }
            }
        }
        if(j%2==0 && j<7){
            printf("\t[ ]");
        }
        printf("\n");


        if(j>6&&finished){                                      // All the cards have been printed and we now stop printing.
            return;
        }
    }
}
void makeHidden(Card * column[]){
    int amountOfHidden[] = {0,1,2,3,4,5,6};

    for(int i=0;i<52;i++) {
        for (int j = 0; j < 7; j++) {
            int allowedAmountOfCards = amountOfHidden[j];
            int currentIndex = 0;
            Card *currentPlacement = column[j];


            while (currentPlacement != NULL && currentIndex < allowedAmountOfCards) {
                currentPlacement->hidden=true;
                currentPlacement = currentPlacement->next;
                currentIndex++;
            }
        }
    }
}

void startStartupPhase() {
    Card *deck;
    Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
    Card * foundation[] = {NULL,NULL,NULL,NULL};
    char str1[20];
    printf("You're in startup phase" "\n");
    while (true) {
        printf("Write input in console""\n");
        scanf("%20s",str1);

        switch(str1[0]+str1[1]) {
            // prints loaded deck
            case 'S'+'W':
                if (deck != NULL) {
                    dealCards(deck, column);
                    for (int i = 0; i <
                                    7; i++) {                                       // Reverses the 7 columns, so top card is at beginning of list.
                        reverseList(&column[i]);                       // Alternatively we could have made the linked lists double..
                    }
                    makeHidden(column);
                    printGameState(column);
                }else {
                    printf("No deck loaded");
                }
                break;
                case 'L'+'D':
                //loads saved deck not implemented
                if (str1[2]== '<') {
                    int i =3;
                    int j =0;
                    char str2[15];
                    while (str1[i]!='>'){
                        str2[j]= str1[i];
                        i++,j++;
                    }
                    printf("Not implemented yet""\n");

                }
                // loads standard deck
                else {
                    deck = createDeck();
                }
                break;
                // shuffles deck
            case 'S'+'R':
                if ( deck != NULL) {
                    deck = shuffleDeck(deck);
                } else
                    printf("No deck loaded");
                break;

            case 'Q'+'Q':
                endGame();
            case 'P':

                if (deck == NULL){
                    dealCards(deck, column);
                    for (int i = 0; i <
                                    7; i++) {                                       // Reverses the 7 columns, so top card is at beginning of list.
                        reverseList(
                                &column[i]);                       // Alternatively we could have made the linked lists double..
                    }
                    makeHidden(column);
                }
                startPlayPhase(deck, column);

                break;


            default:
                printf("Dette er ikke en kommando.""\n");

        }
    }
}
void startPlayPhase(Card* deck, Card *column[]) {
    Card *foundation[4];

    char input[20];
    char selectedColumn[2];
    char selectedSourceCardValue[3];
    char selectedSourceCardSuit[2];
    char destinationColumn[2];


    while (*column != NULL && *column + 1 != NULL) {
        printGameState(column);

        printf("Enter input: ");
        scanf("%19s", input);
        switch (input[0] + input[1] + input[2]) {
            case 'Q' + 'Q':
                endGame();
                break;
            case 'Q':
                break;
            case 'U':
                break;
            case 'R':
                break;
            case 'L':
                break;
            case 'S':
                break;
            default:
                if (input[3] == 0) {
                    selectedColumn[0] = input[0];
                    selectedColumn[1] = input[1];

                    selectedSourceCardValue[0] = input[2];
                    selectedSourceCardValue[1] = input[3];
                    selectedSourceCardValue[2] = '\0';

                    selectedSourceCardSuit[0] = input[4];
                    selectedSourceCardSuit[1] = '\0';

                    destinationColumn[0] = input[6];
                    destinationColumn[1] = input[7];
                } else {
                    selectedColumn[0] = input[0];
                    selectedColumn[1] = input[1];

                    selectedSourceCardValue[0] = input[2];
                    selectedSourceCardValue[1] = '\0';
                    selectedSourceCardValue[2] = '\0';

                    selectedSourceCardSuit[0] = input[3];
                    selectedSourceCardSuit[1] = '\0';
                    

                    destinationColumn[0] = input[5];
                    destinationColumn[1] = input[6];

                    // Example input "C210D : C3"
                    // Example input "C34C : F2"
                    // Example input "C5KH : C1"
                    // TODO: Handle wrong input here!

                }
                gameMove(column, foundation, selectedColumn, selectedSourceCardValue, selectedSourceCardSuit,
                         destinationColumn);
                break;
        }
    }
}

void endGame() {
    exit(0);
}

void gameMove(Card **newColumn, struct card *foundation[4], char selectedColumn[2], char selectedSourceCardValue[3], char selectedSourceCardSuit[2], char destinationColumn[2]) {
    int iselectedColumn = selectedColumn[1] - '0';
    int idestinationColumn = destinationColumn[1] - '0';
    Card *currentCard = newColumn[iselectedColumn - 1];
    Card *currentCard1 = newColumn[idestinationColumn - 1];
    Card *temp;
    //if(selectedColumn[0] == 'C' && selectedColumn[1] <8 && selectedColumn[1] >0 && selectedSourceCardValue[0] < 10 && selectedSourceCardValue[0] >0 || selectedSourceCardValue[0] == 'J' || selectedSourceCardValue[0] == 'Q' || selectedSourceCardValue[0] == 'K'|| selectedSourceCardValue[1]== 0  && selectedSourceCardSuit[0] == 'S' || selectedSourceCardSuit[0] == 'C' || selectedSourceCardSuit[0] == 'D' || selectedSourceCardSuit[0] == 'H' && destinationColumn[0] == 'C' || destinationColumn[0] == 'F'  ){
    if (destinationColumn[0] == 'C') {
        if (currentCard == NULL) {
            printf("there is no Card in the selected Column" "\n");
            return;
        } else
            while (currentCard->next->value[0] != selectedSourceCardValue[0] || currentCard->next->suit[0] != selectedSourceCardSuit[0] || currentCard->next->hidden!=false)  {
                currentCard = currentCard->next;
                // hvis kortet ikke er fundet efter en gennemgang
                //if( )
            }
        temp = currentCard->next;
        while (currentCard1->next != NULL) {
            currentCard1 = currentCard1->next;
        }
        switch (temp->value[0]) {
            case 'A': {
                if (currentCard1->value[0] == '2'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                    currentCard1->next = temp;
                    currentCard->next = NULL;
                    if (currentCard->hidden ==true){
                        currentCard->hidden=false;
                    }
                    break;
                }
                break;
            }
            case '1': {
                if (temp->value[1] == 0) {
                    if (currentCard1->value[0] == 'J'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '2': {
                    if (currentCard1->value[0] == '3'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '3': {
                    if (currentCard1->value[0] == '4'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '4': {
                    if (currentCard1->value[0] == '5'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '5': {
                    if (currentCard1->value[0] == '6'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '6': {
                    if (currentCard1->value[0] == '7'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '7': {
                    if (currentCard1->value[0] == '8'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '8': {
                    if (currentCard1->value[0] == '9'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case '9': {
                    if (currentCard1->value[0] == '1' && currentCard1->value[1] == 0&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case 'J': {
                    if (currentCard1->value[0] == 'Q'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case 'Q': {
                    if (currentCard1->value[0] == 'K'&& currentCard1->suit[0] != selectedSourceCardSuit[0]) {
                        currentCard1->next = temp;
                        currentCard->next = NULL;
                        if (currentCard->hidden ==true){
                            currentCard->hidden=false;
                        }
                        break;
                    }
                    break;
                }

                case 'K': {
                    if (newColumn[destinationColumn[1] - 1] == NULL) {
                        newColumn[destinationColumn[1] - 1] = temp;
                        break;
                    } else {
                        printf("Feltet er ikke tomt ");
                        break;
                    }

                }


            }


        }


    } else{

    }

    }


int main() {

    startStartupPhase();


    Card *deck = createDeck();                                  //Pointer to top card of deck

    Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};     // Pointer to array of linked lists representing the 7 columns in the game.

    Card * foundation[] = {NULL,NULL,NULL,NULL};                // Pointer to array of linked lists representing the 4 foundation columns.

    dealCards(deck,column);                                     // Deals the cards form the deck into the columns.

    printCardList(deck);                                    // Prints the entire deck starting from top card.

    for(int i=0;i<7;i++){                                       // Reverses the 7 columns, so top card is at beginning of list.
        reverseList(&column[i]);                       // Alternatively we could have made the linked lists double..
    }
    makeHidden(column);
    printGameState(column);                                     // Prints the game board


    return 0;


}
