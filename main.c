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

//Structure for the move stack
typedef struct moveStack {
    char source[3];
    char dest[3];
    char cardValue[3];
    char cardSuit[2];
    bool flippedCard;
    struct moveStack* next;
} MoveStack;


MoveStack *moveStack=NULL;
MoveStack *undoStack=NULL;

bool gameWon;

bool gameMove(Card **newColumn, struct card *foundation[4], char selectedColumn[2], char selectedSourceCardValue[3], char selectedSourceCardSuit[2], char destinationColumn[2], bool noChecks, bool *validGameCheck);

void endGame();

void startPlayPhase(Card* deck, Card *column[]);
void undoMove(Card ** column, Card **foundation);

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
 *  Creates a new Move element for the stack. This element contains all the information about the move.
 * @param source source column or foundation
 * @param dest destination column or foundation
 * @param cardValue card value
 * @param cardSuit card suit.
 * @return
 */
MoveStack * createMoveStack(char * source, char *dest, char *cardValue, char *cardSuit){
    MoveStack * newMoveStack;

    newMoveStack = (MoveStack*)malloc(sizeof (MoveStack));

    strcpy(newMoveStack->source,source);
    strcpy(newMoveStack->dest,dest);
    strcpy(newMoveStack->cardValue, cardValue);
    strcpy(newMoveStack->cardSuit, cardSuit);
    newMoveStack->next = NULL;

    return newMoveStack;
}

bool isEmpty(MoveStack** stackPointer){
    if(*stackPointer==NULL){
        return true;
    } else return false;
}

/**
 * Pushes information about a move onto a stack.
 * @param stackPointer This is the stackPointer. It points to the top of the stack.
 * @param source Source register
 * @param dest Destination register
 * @param cardValue Card value
 * @param cardSuit Card suit
 */
void push(MoveStack** stackPointer, char *source, char *dest, char * cardValue, char *cardSuit,bool cardFlipped){
    MoveStack * node = createMoveStack(source, dest, cardValue, cardSuit);
    node->flippedCard=cardFlipped;
    node->next = *stackPointer;
    *stackPointer = node;
    printf("%s:%s,%s%s pushed to stack\n",source,dest,cardValue,cardSuit);
}

/**
 * Pops the top element of the stack. Sets the stackpointer to next element.
 * @param stackPointer Pointer to top of stack
 */
void pop(MoveStack** stackPointer){
    if(isEmpty(stackPointer)){                                                                              // Check if stack is empty. Should never happen in our game,
        return;                                                                                             // but still a good guard to include.
    }
    MoveStack* temp = *stackPointer;
    *stackPointer = (*stackPointer)->next;
    printf("%s:%s,%s%s popped from stack\n",temp->source,temp->dest,temp->cardValue,temp->cardSuit);
    free(temp);
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
 * Creates a random number
 * @return
 */

int randomNumber(){
    srand(time(0));
    int l = 0;
    int u = 9999;

    int num = (rand() % (u - l + 1)) + l;
    return num;
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
 * Shuffle a card deck into a new linked list which will be the new deck in a random order.
 * @return
 */
Card * shuffleDeck(Card* head_ref){

    Card *shuffledeck = NULL;

    Card* current = head_ref;
    Card* next;
    Card* temp;

    shuffledeck = current;
    current=current->next;
    shuffledeck->next=shuffledeck;

    for (int i=0;i<51;i++){

        int ran = randomNumber();

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
void printGameState(Card ** column,Card ** foundation){
    printf("C1\tC2\tC3\tC4\tC5\tC6\tC7\t\t\t\n\n");

    int foundationIndex=0;

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
            Card * currentFoundation=foundation[foundationIndex];

            if(currentFoundation!=NULL) {
                Card * currentFoundation=foundation[foundationIndex];
                printf("\t[%s%s]", foundation[foundationIndex]->value, foundation[foundationIndex]->suit);
            } else {
                printf("\t[]");
            }
            foundationIndex++;
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
                    for (int i = 0; i <7; i++) {                                       // Reverses the 7 columns, so top card is at beginning of list.
                        reverseList(&column[i]);                           // Alternatively we could have made the linked lists double..
                    }
                    makeHidden(column);
                    printGameState(&column,&foundation);
                    for (int i = 0; i <7; i++) {                                       // Reverses the 7 columns, so top card is at beginning of list.
                        column[i]=NULL;                          // Alternatively we could have made the linked lists double..
                    }
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
                return;
            case 'P':

                if (*column == NULL){
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

void startPlayPhase(Card* deck, Card ** column) {
    Card *foundation[]={NULL,NULL,NULL,NULL};

    char input[20];
    char selectedColumn[2];
    char selectedSourceCardValue[3];
    char selectedSourceCardSuit[2];
    char destinationColumn[2];


    while (true) {
        printGameState(column,foundation);
        if (gameWon){
            return;
            break;
        }
        printf("Enter input: ");
        scanf("%19s", input);
        switch (input[0] + input[1]) {
            case 'Q' + 'Q':
                endGame();
                return;
                break;
                //TODO: Check how return to startup works
            case 'Q':
                return;
                break;
            case 'U':
                undoMove(column,foundation);
                break;
            case 'R':
                break;
            case 'L':
                break;
            case 'S':
                break;
            default:
                if (input[3] == '0') {
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
                bool validGameMove=false;
                bool cardFlipped=gameMove(column, foundation, selectedColumn, selectedSourceCardValue, selectedSourceCardSuit,
                         destinationColumn,false,&validGameMove);

                // Push move to the move stack.
                // TODO: Have a check somehow, so that only valid moves, that go through are pushed to the stack.

                if(validGameMove) {
                    char source[3];
                    source[0] = selectedColumn[0];
                    source[1] = selectedColumn[1];
                    source[2] = '\0';
                    char dest[3];
                    dest[0] = destinationColumn[0];
                    dest[1] = destinationColumn[1];
                    dest[2] = '\0';
                    push(&moveStack, source, dest, selectedSourceCardValue, selectedSourceCardSuit, cardFlipped);

                    // Empty undo stack after every succesful move.
                    undoStack = NULL;
                }

                break;
        }
    }
}

void endGame() {
    exit(0);
}

bool gameMove(Card **newColumn, Card *foundation[4], char selectedColumn[2], char selectedSourceCardValue[3], char selectedSourceCardSuit[2], char destinationColumn[2], bool noChecks, bool *validGameCheck) {
    // laver chars i columns om til ints
    int iselectedColumn = selectedColumn[1] - '0';
    int idestinationColumn = destinationColumn[1] - '0';
    // finder det øverste tal i en af vores columns linked list
    Card *currentCard = newColumn[iselectedColumn - 1];
    //laver en temp fil til at holde stur på hvad der skal overføres
    Card *temp;

    // boolean to check if the move flipped a card in the source column.
    bool flippedCard = false;
    //if(selectedColumn[0] == 'C' && selectedColumn[1] <8 && selectedColumn[1] >0 && selectedSourceCardValue[0] < 10 && selectedSourceCardValue[0] >0 || selectedSourceCardValue[0] == 'J' || selectedSourceCardValue[0] == 'Q' || selectedSourceCardValue[0] == 'K'|| selectedSourceCardValue[1]== 0  && selectedSourceCardSuit[0] == 'S' || selectedSourceCardSuit[0] == 'C' || selectedSourceCardSuit[0] == 'D' || selectedSourceCardSuit[0] == 'H' && destinationColumn[0] == 'C' || destinationColumn[0] == 'F'  ){
    //chcks if it is a column or foundation
    if (destinationColumn[0] == 'C' && selectedColumn[0] == 'C') {
        Card *currentCard1 = newColumn[idestinationColumn - 1];
        //checks if it is possible to take cards from the column by checking if the column is empty
        if (currentCard == NULL) {
            printf("there is no Card in the selected Column" "\n");
            return flippedCard;
        } else if (currentCard->value[0] == selectedSourceCardValue[0] &&
                   currentCard->suit[0] == selectedSourceCardSuit[0] && currentCard->hidden == false &&
                   currentCard->next == NULL) {
            temp = currentCard;
            currentCard = NULL;

        } else {
            //checks if the currentCard is in the linkedlist by checking value suit and if it is hidden
            while (currentCard->next->value[0] != selectedSourceCardValue[0] ||
                   currentCard->next->suit[0] != selectedSourceCardSuit[0] || currentCard->next->hidden != false) {
                currentCard = currentCard->next;
                // hvis kortet ikke er fundet efter en gennemgang
                //if( )
            }
            //temp is given a pointer
            temp = currentCard->next;
        }
        if (currentCard1 == NULL) {

        } else {
            //finds the last card in the column we want to move the cards to
            while (currentCard1->next != NULL) {
                currentCard1 = currentCard1->next;
            }
        }
        // switch for each value that the cards can have
        switch (temp->value[0]) {
            case 'A': {
                //checks if the currentCard1 value is bigger than currentCard
                if ((currentCard1->value[0] == '2' && currentCard1->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                    //sets currentCard1 pointer to temp
                    currentCard1->next = temp;
                    if (currentCard != NULL) {
                        //sets currentcard pointer to null
                        currentCard->next = NULL;
                        //makes a card visible if it removes the last visible card
                        if (currentCard->hidden == true) {
                            currentCard->hidden = false;
                            flippedCard = true;
                        }


                    } else {
                        newColumn[iselectedColumn - 1] = NULL;
                    }
                    *validGameCheck = true;
                    break;
                }
                break;
            }
            case '1': {
                if (temp->value[1] == '0') {
                    if ((currentCard1->value[0] == 'J' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '2': {
                    if ((currentCard1->value[0] == '3' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '3': {
                    if ((currentCard1->value[0] == '4' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '4': {
                    if ((currentCard1->value[0] == '5' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '5': {
                    if ((currentCard1->value[0] == '6' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '6': {
                    if ((currentCard1->value[0] == '7' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '7': {
                    if ((currentCard1->value[0] == '8' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '8': {
                    if ((currentCard1->value[0] == '9' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '9': {
                    if ((currentCard1->value[0] == '1' && currentCard1->value[1] == '0' &&
                         currentCard1->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case 'J': {
                    if ((currentCard1->value[0] == 'Q' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case 'Q': {
                    if ((currentCard1->value[0] == 'K' && currentCard1->suit[0] != selectedSourceCardSuit[0]) ||
                        noChecks) {
                        //sets currentCard1 pointer to temp
                        currentCard1->next = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                //todo make sure it works as intended
                case 'K': {
                    if (newColumn[idestinationColumn - 1] == NULL || noChecks) {
                        newColumn[idestinationColumn - 1] = temp;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;

                    } else {
                        printf("Feltet er ikke tomt ");
                        break;
                    }
                }
            }
        }
    } else if (selectedColumn[0] == 'C') {
        Card *currentFoundationCard1 = foundation[idestinationColumn - 1];
        if (currentCard == NULL) {
            printf("there is no Cards in the selected Column" "\n");
            return flippedCard;
        }
        if (currentCard == NULL) {

        } else if (currentCard->next == NULL) {
            temp = currentCard;
            currentCard = NULL;
        } else {
            while (currentCard->next->next != NULL) {
                currentCard = currentCard->next;

            }
            temp = currentCard->next;
        }
        switch (temp->value[0]) {
            case 'A': {
                if ((currentFoundationCard1 == NULL) || noChecks) {

                    foundation[idestinationColumn - 1] = temp;
                    if (currentCard != NULL) {
                        //sets currentcard pointer to null
                        currentCard->next = NULL;
                        //makes a card visible if it removes the last visible card
                        if (currentCard->hidden == true) {
                            currentCard->hidden = false;
                            flippedCard = true;
                        }


                    } else {
                        newColumn[iselectedColumn - 1] = NULL;
                    }
                    *validGameCheck = true;
                    break;
                }
                break;
            }
            case '1': {
                if (temp->value[1] == '0') {
                    if ((currentFoundationCard1->value[0] == '9' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '2': {
                    if ((currentFoundationCard1->value[0] == 'A' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '3': {
                    if ((currentFoundationCard1->value[0] == '2' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '4': {
                    if ((currentFoundationCard1->value[0] == '3' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '5': {
                    if ((currentFoundationCard1->value[0] == '4' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '6': {
                    if ((currentFoundationCard1->value[0] == '5' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '7': {
                    if ((currentFoundationCard1->value[0] == '6' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '8': {
                    if ((currentFoundationCard1->value[0] == '7' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case '9': {
                    if ((currentFoundationCard1->value[0] == '8' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case 'J': {
                    if ((currentFoundationCard1->value[0] == '1' && currentFoundationCard1->value[1] == '0' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }

                case 'Q': {
                    if ((currentFoundationCard1->value[0] == 'J' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;
                }
                //todo make sure it works as intended
                case 'K': {
                    if ((currentFoundationCard1->value[0] == 'Q' &&
                         currentFoundationCard1->suit[0] == selectedSourceCardSuit[0]) ||
                        noChecks) {
                        temp->next = currentFoundationCard1;
                        currentFoundationCard1 = temp;
                        foundation[idestinationColumn - 1] = currentFoundationCard1;
                        if (currentCard != NULL) {
                            //sets currentcard pointer to null
                            currentCard->next = NULL;
                            //makes a card visible if it removes the last visible card
                            if (currentCard->hidden == true) {
                                currentCard->hidden = false;
                                flippedCard = true;
                            }


                        } else {
                            newColumn[iselectedColumn - 1] = NULL;
                        }
                        int checkForWin;
                        *validGameCheck = true;
                        for (int i = 0; i < 4; i++) {
                            if (foundation[i]->value[0] == 'K') {
                                checkForWin++;
                            }
                        }
                        if (checkForWin == 4) {
                            printf("Du har vundet");
                            gameWon = true;
                            return true;
                        }
                        break;
                    }
                    break;
                }

            }
        }
    } else {
        Card *currentColumnCard = newColumn[idestinationColumn - 1];
        Card *currentFoundationCard = foundation[iselectedColumn-1];
        Card *tempFoundation;

        if (currentFoundationCard == NULL){
            printf("there is no Card in the selected foundation" "\n");
            return flippedCard;
        } else if(currentFoundationCard->value[0] == selectedSourceCardValue[0] && currentFoundationCard->suit[0] == selectedSourceCardSuit[0]){
            tempFoundation = currentFoundationCard;
        }
        if (currentColumnCard == NULL) {

        } else {
            //finds the last card in the column we want to move the cards to
            while (currentColumnCard->next != NULL) {
                currentColumnCard = currentColumnCard->next;
            }
            switch (tempFoundation->value[0]) {
                case 'A': {
                    //checks if the currentCard1 value is bigger than currentCard
                    if ((currentColumnCard->value[0] == '2' &&
                         currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                        //sets currentCard1 pointer to temp
                        currentColumnCard->next = tempFoundation;
                        foundation[iselectedColumn - 1] = currentFoundationCard->next;
                        currentFoundationCard = NULL;
                    }
                    *validGameCheck = true;
                    break;
                }
                    break;
                case '1': {
                    if (temp->value[1] == '0') {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == 'j' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '2': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '3' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '3': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '4' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '4': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '5' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '5': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '6' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '6': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '7' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '7': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '8' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '8': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '9' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case '9': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == '1' && currentColumnCard->value[1] == '0' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case 'J': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == 'Q' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    case 'Q': {
                        //checks if the currentCard1 value is bigger than currentCard
                        if ((currentColumnCard->value[0] == 'K' &&
                             currentColumnCard->suit[0] != selectedSourceCardSuit[0]) || noChecks) {
                            //sets currentCard1 pointer to temp
                            currentColumnCard->next = tempFoundation;
                            foundation[iselectedColumn - 1] = currentFoundationCard->next;
                            currentFoundationCard = NULL;
                        }
                        *validGameCheck = true;
                        break;
                    }
                    break;

                    //todo make sure it works as intended
                    case 'K': {
                        if (newColumn[idestinationColumn - 1] == NULL || noChecks) {
                            newColumn[idestinationColumn - 1] = temp;

                            *validGameCheck = true;
                            break;

                        } else {
                            printf("Feltet er ikke tomt ");
                            break;
                        }
                    }
                }
            }

        }
    }



        return flippedCard;
    }


    void undoMove(Card **column, Card **foundation) {
        char sourceColumn[2];
        sourceColumn[0] = moveStack->dest[0];
        sourceColumn[1] = moveStack->dest[1];
        char *value = moveStack->cardValue;
        char *suit = moveStack->cardSuit;
        char destColumn[2];
        destColumn[0] = moveStack->source[0];
        destColumn[1] = moveStack->source[1];

        // Checks if the move needs to make a card hidden in the destination column
        if (destColumn[0] == 'C' && moveStack->flippedCard == true) {
            int destColumnIndex = destColumn[1] - '0' - 1;

            //currentCard is the top card of the destination column
            Card *currentCard = column[destColumnIndex];

            //Should skip everything if the destination column is empty
            if (currentCard != NULL) {

                //Traverses the linked list until reaching a card that is not hidden.
                while (currentCard->hidden == true) {
                    currentCard = currentCard->next;
                }

                //If the card is the first and last not hidden of that column, it should be flipped.
                if (currentCard->next == NULL) {
                    currentCard->hidden = true;
                }
            }
        }
        bool validGameCheck;
        gameMove(column, foundation, sourceColumn, value, suit, destColumn, true, &validGameCheck);


        push(&undoStack, moveStack->dest, moveStack->source, value, suit, false);
        pop(&moveStack);
    }


    int main() {
        startStartupPhase();
        return 0;
    }
