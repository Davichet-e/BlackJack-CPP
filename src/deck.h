#pragma once

#include <iostream>
#include <algorithm>
#include <random>
#include <string>
#include <vector>

enum Suit
{
    HEARTS,
    DIAMONDS,
    CLUBS,
    PIKES
};

class Card
{
private:
    std::string_view m_name;
    Suit m_suit;

public:
    Card();
    Card(std::string_view name, Suit suit);
    // Card(const Card &other) : m_name(other.m_name), m_suit(other.m_suit) {}

    Suit GetSuit() const;
    std::string_view GetName() const;

    int Value() const;

    std::string ToString() const;

    bool operator==(const Card &other);
};

std::ostream &operator<<(std::ostream &stream, const Card &card);

class Deck
{
private:
    std::vector<Card> m_cards;

public:
    Deck();
    Deck(int n_of_decks);

    std::vector<Card> GetCards() const;

    int Size() const;

    Card DealCard();

    std::vector<Card> InitialCards();
};
