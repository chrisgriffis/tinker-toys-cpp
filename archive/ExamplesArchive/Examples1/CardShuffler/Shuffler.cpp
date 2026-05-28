#include <cstdio>
#include "cstdlib"
#include "time.h"

struct Card
{
    char* suit;
    char* face;
};


void fillDeck(Card (&)[52]);
void shuffleDeck(Card (&)[52]);
void printDeck(Card (&)[52]);

int main (int argc, char **argv)
{
    srand(time(NULL));
    Card deck [52] = {0};
    fillDeck(deck);
    shuffleDeck(deck);
    printDeck(deck);
    return 0;

}

void fillDeck( Card (&inDeck)[52] )
{
    char* suits [4]  = {"spades","clubs","hearts","diamonds"};
    char* faces [13] = {"two","three","four","five","six","seven","eight","nine","ten","jack","queen","king","ace"};

    for (int i = 0; i < 52 ; i++)
    {
        inDeck[i].suit = suits [i%4];
        inDeck[i].face = faces [i%13];

    }
}

void shuffleDeck( Card (&inDeck)[52] )
{
    for (int card_index = 0; card_index < 52 ; card_index++)
    {
        int rand_position = rand() % 52;
        Card temp =  inDeck[card_index];
        inDeck[card_index] =  inDeck[rand_position];
        inDeck[rand_position] = temp;
    }
}

void printDeck( Card (&inDeck)[52] )
{
    printf ("\nthe shuffled deck is as follows:\n");
    for (int card_index = 0; card_index < 52 ; card_index++)
    {
        printf("%s \tof\t %s\n",inDeck[card_index].face,inDeck[card_index].suit);
    }
}




