#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//value 1-13?

// Structure for card
typedef struct card {
    char value[3];
    char suit[2];
    bool hidden;
    struct card *next;
} Card;


void endGame();

void gameMove(Card **pCard, struct card *pCard1[4], char column[2], char value[3], char suit[2], char column1[2]);

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
void dealCards(Card *deck,Card *column[]){


    int columnValues[] = {1,6,7,8,9,10,11};

    Card * currentCard = (Card*)malloc(sizeof (Card));

    currentCard = deck;
    for(int i=0;i<52;i++) {
        for (int j = 0; j < 7; j++) {
            int allowedAmountOfCards = columnValues[j];
            int currentIndex = 0;


            Card *currentPlacement = column[j];


            while (currentPlacement != NULL && currentIndex < allowedAmountOfCards) {
                currentPlacement = currentPlacement->next;
                currentIndex++;
            }
            if(currentIndex<allowedAmountOfCards) {
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

void startPlayPhase(Card* deck, Card *column[]){
    Card * foundation[4];

    char input[20];
    char selectedColumn[2];
    char selectedSourceCardValue[3];
    char selectedSourceCardSuit[2];
    char destinationColumn[2];


    while(*column!=NULL && *column+1!=NULL){
        printGameState(column);

        printf("Enter input: ");
        scanf("%s",input);
        switch(input[0]+input[1]+input[2]){
            case 'Q'+'Q':
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
                selectedColumn[0]=input[0];
                selectedColumn[1]=input[1];

                selectedSourceCardValue[0]=input[2];
                selectedSourceCardValue[1]=input[3];
                selectedSourceCardValue[2]='\0';

                selectedSourceCardSuit[0]=input[4];
                selectedSourceCardSuit[1]='\0';

                destinationColumn[0]=input[8];
                destinationColumn[1]=input[9];

                       // Example input "C210D : C3"
                       // Example input "C34C : F2"
                       // Example input "C5KH : C1"
                       // TODO: Be able to handle both double digit value and single digit value...
                       // TODO: Handle wrong input here!

                gameMove(column,foundation,selectedColumn,selectedSourceCardValue,selectedSourceCardSuit,destinationColumn);
                break;
        }
    }
}

void gameMove(Card **pCard, struct card *pCard1[4], char column[2], char value[3], char suit[2], char column1[2]) {

}

void endGame() {

}

int main() {
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

    startPlayPhase(deck,column);


    return 0;
}
