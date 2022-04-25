#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//value 1-13?


typedef struct card {
    char value[3];
    char suit[2];
    struct card *next;
} Card;

Card * createCard(char *value,char *suit){
    Card * newCard;

    newCard = (Card*)malloc(sizeof (Card));

    strcpy(newCard->value, value);
    strcpy(newCard->suit, suit);


    newCard->next = NULL;

    return newCard;
}

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



void dealCards(Card *deck,Card *column[7]){


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



int main() {


Card *deck = createDeck();
Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
dealCards(deck,column);




/*
Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};


int columnValues[] = {1,6,7,8,9,10,11};

Card * currentCard = (Card*)malloc(sizeof (Card));

currentCard = deck;
for(int i=0;i<11;i++) {
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
*/








    printCardList(deck);






/*
printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n\n");
for(int i=0;i<13;i++){
    Card * currentCard = column[i];
    for(int j=13-i;j<13;){

    }

    printf("%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t\t\t[]\tF1\n",
           column[0]->value,column[0]->suit,
           column[1]->value,column[1]->suit,
           column[2]->value,column[2]->suit,
           column[3]->value,column[3]->suit,
           column[4]->value,column[4]->suit,
           column[5]->value,column[5]->suit,
           column[6]->value,column[6]->suit);
}
*/
    // LAYOUT
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n");
    printf("\n");
    /*
    printf("%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t\t\t[]\tF1\n",
           column[0].value,column[0].suit,
           column[1].value,column[1].suit,
           column[2].value,column[2].suit,
           column[3]->value,column[3].suit,
           column[4]->value,column[4]->suit,
           column[5]->value,column[5]->suit,
           column[6]->value,column[6]->suit);
    printf("%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t%s%s\t\t\t\t\n",
           column[0]->value,column[0]->suit,
           (column[1]->next)->value,(column[1]->next)->suit,
           column[2]->value,column[2]->suit,
           column[3]->value,column[3]->suit,
           column[4]->value,column[4]->suit,
           column[5]->value,column[5]->suit,
           column[6]->value,column[6]->suit);
           */
    printf("\t\t\t\t\t\t\t\t\t[]\tF2\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t[]\tF3\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t[]\tF4\n");




    return 0;
}
