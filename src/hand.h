#pragma once

#include <numeric>
#include "deck.h"

class Hand
{
private:
    std::vector<Card> m_cards;
    int m_points;
    int m_aces;
    static Deck &s_deck;

public:
    Hand();

    Hand(Deck &deck);

    Hand(std::vector<Card> &cards, Deck &deck);

    const std::vector<Card> &GetCards() const;

    int GetPoints() const;

    static Deck &GetDeck();

    void SetPoints(int value);

    Card PopCard();

    void CheckIfAce(const Card &card);

    void CheckAcePoints();

    bool HasBlackJack() const;

    void InitializeAtributes();

    void UpdatePoints();

    void DealCard();

    static int CalculatePoints(const std::vector<Card> &cards);
    static void SetDeck(Deck deck);
};

std::ostream &operator<<(std::ostream &stream, const Hand &hand);
