#include "pch.h"
#include <iostream>
#include <array>
#include <random> //для rand(),srand()
#include <ctime> //для time()
#include <array>
#include <cassert>

using namespace std;

class Card{
public:
	enum CardSuit {
		SUIT_CLUB,
		SUIT_DIAMOND,
		SUIT_HEART,
		SUIT_SPADE,
		MAX_SUITS
	};
	enum CardRank {
		RANK_2,
		RANK_3,
		RANK_4,
		RANK_5,
		RANK_6,
		RANK_7,
		RANK_8,
		RANK_9,
		RANK_10,
		RANK_JACK,
		RANK_QUEEN,
		RANK_KING,
		RANK_ACE,
		MAX_RANKS
	};
private:
	CardSuit m_suit;	//масть карты
	CardRank m_rank;	//ранк карты
public:
	Card(CardRank rank=MAX_RANKS,CardSuit suit=MAX_SUITS) 
		: m_rank{ rank }, m_suit{ suit } {}
	void printCard() const
	{	//вывод ранка и масти карты
		switch (m_rank)
		{
		case RANK_2:		cout << "2"; break;
		case RANK_3:		cout << "3"; break;
		case RANK_4:		cout << "4"; break;
		case RANK_5:		cout << "5"; break;
		case RANK_6:		cout << "6"; break;
		case RANK_7:		cout << "7"; break;
		case RANK_8:		cout << "8"; break;
		case RANK_9:		cout << "9"; break;
		case RANK_10:		cout << "10"; break;
		case RANK_JACK:		std::cout << 'J'; break;
		case RANK_QUEEN:	std::cout << 'Q'; break;
		case RANK_KING:		std::cout << 'K'; break;
		case RANK_ACE:		std::cout << 'A'; break;
		}
		switch (m_suit)
		{
		case SUIT_CLUB:		std::cout << 'C'; break;
		case SUIT_DIAMOND:	std::cout << 'D'; break;
		case SUIT_HEART:	std::cout << 'H'; break;
		case SUIT_SPADE:	std::cout << 'S'; break;
		}
	}
	int getCardValue() const
	{
		switch (m_rank)
		{
		case RANK_2:		return 2;
		case RANK_3:		return 3;
		case RANK_4:		return 4;
		case RANK_5:		return 5;
		case RANK_6:		return 6;
		case RANK_7:		return 7;
		case RANK_8:		return 8;
		case RANK_9:		return 9;
		case RANK_10:		return 10;
		case RANK_JACK:		return 10;
		case RANK_QUEEN:	return 10;
		case RANK_KING:		return 10;
		case RANK_ACE:		return 11;
		}
	}
};
class Deck {
private:
	array<Card, 52> m_deck;
	int m_cardIndex;
	static int getRandomNumber(int min, int max){
		static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);		
		return static_cast<int>(rand() * fraction * (max - min + 1) + min);
	}
	static void swapCard(Card &a, Card &b){
		Card temp = a;
		a = b;
		b = temp;
	}
	
public:
	Deck() : m_cardIndex{ 0 } {
		int card = 0;
		for (int suit = 0; suit < Card::MAX_SUITS; ++suit)
			for (int rank = 0; rank < Card::MAX_RANKS; ++rank)
			{
				m_deck[card] = Card(static_cast<Card::CardRank>(rank), static_cast<Card::CardSuit>(suit));
				++card;
			}
	}
	void printDeck() const
	{	//вывод всей колоды карт
		for (const auto &card : m_deck)
		{
			card.printCard();
			cout << " ";
		}
		cout << endl;
	}
	void shuffleDeck(){
		m_cardIndex = 0;
		for (int i = 0; i < 52; ++i)
			swapCard(m_deck[i], m_deck[getRandomNumber(0, 51)]);
	}
	const Card& dealCard() { assert(m_cardIndex < 52); return m_deck[m_cardIndex++]; }
	const Card& getCurrentCard() { return m_deck[m_cardIndex]; }
	const int getCardIndex() { return m_cardIndex; }

};
enum BlackjackResult
{
	BLACKJACK_PLAYER_WIN,
	BLACKJACK_DEALER_WIN,
	BLACKJACK_EVEN
};
BlackjackResult playBlackjack(Deck &deck)
{
	deck.shuffleDeck();
	//вывод информации о карте дилера и его результате
	int totalDealer = deck.dealCard().getCardValue();
	cout << "Dealer has ";
	deck.getCurrentCard().printCard();
	cout << ", total of " << totalDealer;

	//вывод информации о картах игрока и его результате
	int totalPlayer = deck.dealCard().getCardValue();
	cout << "\nPlayer has ";
	deck.getCurrentCard().printCard();
	cout << " and ";
	totalPlayer += deck.dealCard().getCardValue();
	deck.getCurrentCard().printCard();
	cout << ", total of " << totalPlayer;

	// при раздаче игроку попалось 2 туза
	if (totalPlayer > 21)
	{
		cout << "\nBUST!";
		return BLACKJACK_DEALER_WIN;
	}
	//прираздаче игрок сразу получил 21 очко
	if (totalPlayer == 21)
	{
		cout << "\nLUCKY!";
		return BLACKJACK_PLAYER_WIN;
	}
	//игрок берет доп. карты или оставляет то, что уже есть
	char answer = 'h';
	while ((answer == 'h') && (totalPlayer < 21))
	{	//вопрос и ответ брать или нет карты
		while (true)
		{
			cout << "\nWould you like to hit or stand? (h/s) ";
			cin >> answer;
			if ((cin.fail()) || ((answer != 'h') && (answer != 's')))
			{
				cin.clear();
				cin.ignore(32767, '\n');
				cout << "You entered something wrong, please answer again.";
			}
			else break;
		}
		//не брать карты
		if (answer == 's') break;
		else
		{	//игрок берет карты и вывод информации о новой карте и общем счете
			cout << "Your card is ";
			deck.getCurrentCard().printCard();
			totalPlayer += deck.dealCard().getCardValue();			
			cout << " and your total now is " << totalPlayer;
		}
		//игрок получил больше 21, когда взял новую карту
		if (totalPlayer > 21)
		{
			cout << "\nBUST!";
			return BLACKJACK_DEALER_WIN;
		}
		//игрок получил ровно 21 очко, когда взял новую карту
		if (totalPlayer == 21) return BLACKJACK_PLAYER_WIN;
	}
	//дилер берет доп. карты, если необходимо
	while (totalDealer < 17)
	{
		cout << "Dealer's next card is ";
		totalDealer += deck.dealCard().getCardValue();
		deck.getCurrentCard().printCard();
		cout << " and his total is " << totalDealer;
		//дилер получил ровно 21 очко
		if (totalDealer == 21)	return BLACKJACK_DEALER_WIN;
		//дилер, взяв новую карту, получил больше 21 очка
		if (totalDealer > 21)	return BLACKJACK_PLAYER_WIN;
		if (totalDealer < 21)	cout << "\n";
	}
	//вывод в main() результата игры
	if (totalDealer > totalPlayer)	return BLACKJACK_DEALER_WIN;
	if (totalDealer < totalPlayer)	return BLACKJACK_PLAYER_WIN;
	if (totalDealer == totalPlayer)	return BLACKJACK_EVEN;
}

int main()
{
	// создание колоды
	srand(static_cast<unsigned int>(time(0)));
	rand();
	Deck deck;
	
	char playGame = 'y';
	while (playGame == 'y')
	{	//игра и запоминание результата в resultOfTheGame
		BlackjackResult resultOfTheGame = playBlackjack(deck);

		//вывод результата игры игроку
		if (resultOfTheGame == BLACKJACK_PLAYER_WIN)
			cout << "\nCongatulations, you win!\n";
		if (resultOfTheGame == BLACKJACK_DEALER_WIN)
			cout << "\nSorry, you lose.\n";
		if (resultOfTheGame == BLACKJACK_EVEN)
			cout << "\nEven, no one wins.\n";

		//хочет ли игрок сыграть еще
		while (true)
		{
			cout << "Do you want to play again? (y/n) ";
			cin >> playGame;
			if ((cin.fail()) || ((playGame != 'y') && (playGame != 'n')))
			{
				cin.clear();
				cin.ignore(32767, '\n');
				cout << "You entered something wrong, please answer again.\n";
			}
			else break;
		}
		if (playGame == 'y') cout << '\n';
	}

	cout << "\nThanks for playing!\n";
	
	return 0;
}