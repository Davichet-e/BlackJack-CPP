#include "player.h"

Player::Player(std::string name, int initial_money, Deck &deck)
    : m_hands({Hand(deck), std::nullopt}), m_name(name), m_initial_money(initial_money),
      m_actual_money(initial_money), m_bet(0) {}

int Player::GetActualMoney() const { return m_actual_money; }
int Player::GetInitialMoney() const { return m_initial_money; }
std::string_view Player::GetName() const { return m_name; }
const std::pair<Hand, std::optional<Hand>> &Player::GetHands() const { return m_hands; }

void Player::ResetHands()
{
    m_hands.first.InitializeAtributes();
    auto splitted_hand = m_hands.second;
    if (splitted_hand.has_value())
        splitted_hand->InitializeAtributes();
}

void Player::Bet(int money)
{
    m_bet = money;
}

void Player::Hit(int hand_index)
{
    if (hand_index == 0)
    {
        m_hands.first.DealCard();
    }
    else
    {
        m_hands.second->DealCard();
    }
}

std::optional<std::string_view> Player::Double()
{
    if (m_bet * 2 > m_actual_money)
        return "Cannot double because you have not enough money!";
    else if (m_hands.first.GetCards().size() > 2)
        return "Cannot double because you have already hit!";
    else
    {
        if (m_hands.second.has_value())
            m_bet += m_bet / 2;
        else
            m_bet *= 2;
        return {};
    }
}

std::optional<std::string_view> Player::Surrender()
{
    if (m_hands.first.GetCards().size() != 2)
    {
        return "Cannot surrender because you have already hit!";
    }
    else if (m_hands.second.has_value())
    {
        return "Cannot surrender because you have already splitted!";
    }
    else
    {
        m_bet /= 2;
        m_hands.first.SetPoints(0);
        return {};
    }
}

std::optional<std::string_view> Player::Split()
{
    const std::vector<Card> &first_hand_cards = m_hands.first.GetCards();
    Deck &deck = m_hands.first.GetDeck();

    if (m_bet * 2 > m_actual_money)
        return "Cannot split because you have not enough money!";
    else if (m_hands.second.has_value())
        return "Cannot split because you have already splitted!";
    else if (first_hand_cards.size() != 2)
        return "Cannot split because you have already hit!";
    else if (first_hand_cards[0].GetName() != first_hand_cards[1].GetName())
        return "Cannot split because your cards are not equal!";
    else
    {
        m_bet *= 2;
        std::vector<Card> cards{
            m_hands
                .second->PopCard(),
            deck.DealCard(),
        };
        m_hands.second = Hand(cards, deck);

        m_hands.first.DealCard();
        return {};
    }
}

int Player::Win(int hand_index)
{
    int money_before = m_actual_money;
    if (!m_hands.second.has_value())
    {
        m_actual_money += m_bet;
        if (m_hands.first.HasBlackJack())
            m_actual_money += m_bet / 2;
    }
    else
    {
        // If the player has splitted, the money earned
        // by each winning hand should be the half,
        // since when player splitted, bet got doubled,
        // and each half of the bet represents one hand
        m_actual_money += m_bet / 2;
        if (hand_index == 0)
        {
            if (m_hands.first.HasBlackJack())
                m_actual_money += m_bet / 4;
        }
        else
        {
            if (m_hands.second.has_value() && m_hands.second->HasBlackJack())
                m_actual_money += m_bet / 4;
        }
    }
    return m_actual_money - money_before;
}

void Player::Lose()
{
    if (m_hands.second.has_value())
        m_actual_money -= m_bet / 2;
    else
        m_actual_money -= m_bet;
}

std::ostream &operator<<(std::ostream &stream, const Player &player)
{
    stream << player.GetName();
    return stream;
}