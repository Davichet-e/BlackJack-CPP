#include "hand.h"

Deck &Hand::s_deck = Deck();

Hand::Hand(Deck &deck)
{
    InitializeAtributes();
}

Hand::Hand()
{
    InitializeAtributes();
}

Hand::Hand(std::vector<Card> &cards, Deck &deck)
    : m_cards(cards), m_aces(0), m_points(CalculatePoints(cards))
{
    int points = CalculatePoints(cards);

    for (Card card : cards)
    {
        CheckIfAce(card);
    }
    CheckAcePoints();
}

int Hand::GetPoints() const
{
    return m_points;
}

const std::vector<Card> &Hand::GetCards() const
{
    return m_cards;
}

Deck &Hand::GetDeck()
{
    return Hand::s_deck;
}

void Hand::SetPoints(int value)
{
    m_points = value;
}

Card Hand::PopCard()
{
    Card last_card_copy(m_cards.back());
    m_cards.pop_back();
    return last_card_copy;
}

void Hand::CheckIfAce(const Card &card)
{
    if (card.GetName() == "ACE")
    {
        m_aces++;
    }
}

void Hand::CheckAcePoints()
{
    while (m_aces > 0 && m_points > 21)
    {
        m_points -= 10;
        m_aces--;
    }
}

bool Hand::HasBlackJack() const
{
    return m_cards.size() == 2 && m_points == 21;
}

void Hand::InitializeAtributes()
{
    m_cards = s_deck.InitialCards();
    m_points = CalculatePoints(m_cards);
    m_aces = 0;

    for (auto card : m_cards)
    {
        CheckIfAce(card);
    }

    CheckAcePoints();
}

void Hand::UpdatePoints()
{
    m_points = CalculatePoints(m_cards);
    CheckAcePoints();
}

void Hand::DealCard()
{
    auto card = s_deck.DealCard();
    CheckIfAce(card);

    m_cards.push_back(card);
    UpdatePoints();

    if (m_points > 21)
    {
        m_points = 0;
    }
}

int Hand::CalculatePoints(const std::vector<Card> &cards)
{
    return std::accumulate(
        cards.begin(), cards.end(), 0, [](int res, Card c) {
            return res + c.Value();
        });
}

void Hand::SetDeck(Deck deck)
{
    s_deck = deck;
}

std::vector<std::string> split(char *phrase, std::string delimiter)
{
    std::vector<std::string> list;
    std::string s(phrase);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        list.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    list.push_back(s);
    return list;
}

std::string join(const std::vector<Card> &range, const char *separator)
{
    if (range.empty())
        return std::string();

    return std::accumulate(
        std::next(range.begin()),
        range.end(),
        range[0].ToString(),
        [&separator](std::string acc, Card b) {
            return acc + separator + b.ToString();
        });
};

// Maybe FIXME??
std::ostream &operator<<(std::ostream &stream, const Hand &hand)
{
    std::vector<Card> cards = hand.GetCards();
    int points = hand.GetPoints();
    Card lastCard = cards.back();
    cards.resize(cards.size() - 1);
    auto a = join(cards, ", ");

    stream << a << " and " << lastCard << " (" << points << " points)";
    return stream;
}