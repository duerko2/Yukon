#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//value 1-13?


typedef struct card {
    char value[3];
    char suit[2];
    struct card *next;
} Card;

//Works
// Creates a struct of type Card. Holds the suit and value.
// TODO: The struct also needs to hold a boolean representing if the card is face up or face down.
Card * createCard(char *value,char *suit){
    Card * newCard;

    newCard = (Card*)malloc(sizeof (Card));

    strcpy(newCard->value, value);
    strcpy(newCard->suit, suit);


    newCard->next = NULL;

    return newCard;
}

// Works
// Prints out the entire deck.
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

// Works.
// Creates a linked list representing a unshuffled deck.
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



// Works.
// Deals the cards from the deck into linked lists representing the columns.
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

// Doesn't work
// Prints out the game state. Should be called after each input and action.
void printGameState(Card * column[]){
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n\n");

    Card * previousCard;

    for(int j=0;j<13;j++) {
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
                    printf("%s%s\t", currentCard->value, currentCard->suit);
                }
            }
        }
        printf("\n");
    }
}



int main() {


    Card *deck = createDeck(); //Pointer to top card of deck
    Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL}; // Pointer to array of linked lists representing the 7 columns in the game.


    dealCards(deck,column); // Deals the cards form the deck into the columns.


    printCardList(deck); // Prints the entire deck starting from top card.




    for(int i=0;i<7;i++){
        reverseList(&column[i]);
    } // Reverses the columns, for easier use in printGameState function.

    printGameState(column);


    return 0;
}
