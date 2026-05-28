#include <iostream>
#include <random>
#include <utility>



struct Card
{
	const char* face;
	const char* suit;
};

void fillDeck(Card(&deck)[52])
{
	const char* faces[13] = { "2","3","4","5","6","7","8","9","10","j","q","k","a" };
	const char* suits[4] = { "spades","clubs","diamonds","hearts" };
	for (int Index = 0; Index < 52 ; Index++)
	{
		deck[Index].suit = suits[Index / 13];
		deck[Index].face = faces[Index % 13];
	}
}

using namespace std;
template<class It>
void shuffle(It low, It hi)
{
	decltype(hi) current = hi;
	auto remaining = hi - low;
	while (remaining)
		std::iter_swap(--current, low + (std::random_device{}() % remaining--));
}


void shuffleDeck(Card(&deck)[52])
{
	int current = 52; int remaining = 52;
	while (remaining)[](Card& left,Card& right) {
		Card temp = left;
		left = right;
		right = temp;
	}(deck[--current], deck[std::random_device{}()%remaining--]);
}

int main()
{
	Card deck[52];
	fillDeck(deck);
	for (auto i : deck)
	{
		std::cout << i.face << " of " << i.suit << std::endl;
	}
	std::cout << std::endl << std::endl;
// 	shuffleDeck(deck);
	shuffle(deck, deck + 52);
	for (auto i : deck)
	{
		std::cout << i.face << " of " << i.suit << std::endl;
	}
}
