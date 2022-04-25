#include <stdio.h>
#include <stdlib.h>

//value 1-13?
enum value{
    value_Ace,value_2,value_3,value_4,value_5,value_6,value_7,value_8,value_9,value_10,value_Jack,value_Queen,value_King
};

enum suit {
    S,H,D,C
};

typedef struct card {
    int value;
    int suit;
    struct card *next;
} Card;

Card * createCard(int value,int suit){
    Card * newCard;

    newCard = (Card*)malloc(sizeof (Card));

    newCard->suit=suit;
    newCard->value=value;

    newCard->next = NULL;

    return newCard;
}

int main() {

Card *deck= NULL;
Card *column1 = NULL;
Card *column2 = NULL;
Card *column3 = NULL;
Card *column4 = NULL;
Card *column5 = NULL;
Card *column6 = NULL;
Card *column7 = NULL;

for(int i=0;i<4;i++){
    Card *temp;

    for(int j=12;j>=0;j--){
        temp=createCard(i,j);
        temp->next = deck;
        deck=temp;
    }
}

Card * column[] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};


int columnValues = {1,6,7,8,9,10,11};
    Card *currentCard = deck;

for(int i=0;i<52;i++){
    for(int j=0;j<7;i++){


    }
}











    // LAYOUT
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n");
    printf("\n");
    printf("K2\t[]\t\t\t\t\t\t\t\t[]\tF1\n");
    printf("\n");
    printf("\tK2\t\t\t\t\t\t\t\t[]\tF2\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t[]\tF3\n");
    printf("\n");
    printf("\t\t\t\t\t\t\t\t\t[]\tF4\n");




    return 0;
}
