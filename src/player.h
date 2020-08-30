#pragma once

#include <utility>
#include <optional>
#include "hand.h"

class Player
{
private:
    std::pair<Hand, std::optional<Hand>> m_hands;
    std::string_view m_name;
    int m_initial_money, m_actual_money, m_bet;

public:
    Player(std::string_view name, int initial_money, Deck &deck);

    void ResetHands();

    void Bet(int money);

    void Hit(int hand_index);

    std::optional<std::string_view> Double();
    std::optional<std::string_view> Surrender();
    std::optional<std::string_view> Split();
    int Win(int hand_index);
    void Lose();
};
