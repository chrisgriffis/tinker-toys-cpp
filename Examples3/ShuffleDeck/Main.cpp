#include <iostream>
#include <utility>
#include <cstdlib>
#include <string>
using namespace std;

template<class T,unsigned int N>
void shuffle(T (&deck)[N]);
template<class T,unsigned int N>
void print(T (&deck)[N]);
struct card
{
    string val;
    string suit;
};
int main()
{
    //shuffle a deck of cards
    card deck[52];
    const char* suits[4] = {"h","d","c","s"};
    const char* vals[13] = {"1","2","3","4","5","6","7","8","9","10","j","q","k"};
    for (int i = 0; i < 52 ; i++)
    {
        deck[i]=*(new card);
        deck[i].suit = string(suits[i/13]);
        deck[i].val = string(vals[i%13]);
    }
    shuffle(deck);
    cin >> deck[0].val;
    return 0;
}

template<class T,unsigned int N>
void shuffle(T (&deck)[N])
{
    for (int remaining = N - 1; remaining >= 1 ; remaining--)
    {
    	swap(deck[remaining],deck[rand()%remaining]);
        print(deck);
    }
}

template<class T,unsigned int N>
void print( T (&deck)[N] )
{
    using namespace std;
    for (int i = 0; i < N ; i++)
    {
        cout << deck[i].val << deck[i].suit << " ";
    }
    cout << endl;
}
