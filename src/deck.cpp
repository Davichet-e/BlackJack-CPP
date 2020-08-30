#include "deck.h"

Card::Card() {}
Card::Card(std::string_view name, Suit suit) : m_name(name), m_suit(suit) {}

Suit Card::GetSuit() const { return m_suit; }
std::string_view Card::GetName() const { return m_name; }

int Card::Value() const
{
    if (m_name == "ACE")
        return 11;
    else if (m_name == "TWO")
        return 2;
    else if (m_name == "THREE")
        return 3;
    else if (m_name == "FOUR")
        return 4;
    else if (m_name == "FIVE")
        return 5;
    else if (m_name == "SIX")
        return 6;
    else if (m_name == "SEVEN")
        return 7;
    else if (m_name == "EIGHT")
        return 8;
    else if (m_name == "NINE")
        return 9;
    else if (m_name == "TEN" || m_name == "JACK" || m_name == "QUEEN" || m_name == "KING")
        return 10;
    throw std::invalid_argument("Invalid parameter");
}

bool Card::operator==(const Card &other)
{
    return m_name == other.m_name && m_suit == other.m_suit;
}

std::ostream &operator<<(std::ostream &stream, const Card &card)
{
    std::string_view suit;
    switch (card.GetSuit())
    {
    case Suit::HEARTS:
        suit = "HEARTS";
        break;
    case Suit::DIAMONDS:
        suit = "DIAMONS";
        break;
    case Suit::CLUBS:
        suit = "CLUBS";
        break;
    case Suit::PIKES:
        suit = "PIKES";
        break;
    }
    stream << card.GetName() << " of " << suit;
    return stream;
}

Deck::Deck(int n_of_decks)
{
    std::string_view cardNames[] = {
        "ACE",
        "TWO",
        "THREE",
        "FOUR",
        "FIVE",
        "SIX",
        "SEVEN",
        "EIGHT",
        "NINE",
        "TEN",
        "JACK",
        "QUEEN",
        "KING",
    };
    for (int i = 0; i < n_of_decks; i++)
    {
        for (int j = 0; j < 4 /* The four Suits */; j++)
        {
            for (const std::string_view cardName : cardNames)
            {
                m_cards.push_back(Card(cardName, static_cast<Suit>(j)));
            }
        }
    }
    auto rd = std::random_device{};
    auto rng = std::default_random_engine{rd()};
    std::shuffle(std::begin(m_cards), std::end(m_cards), rng);
}

std::vector<Card> Deck::GetCards() const { return m_cards; }

int Deck::Size() const
{
    return m_cards.size();
}

Card Deck::DealCard()
{
    Card last_card_copy(m_cards.back());
    m_cards.pop_back();
    return last_card_copy;
}

std::vector<Card> Deck::InitialCards()
{
    return {this->DealCard(), this->DealCard()};
}
