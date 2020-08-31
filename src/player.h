#pragma once

#include <utility>
#include <optional>
#include "hand.h"

class Player
{
private:
    std::pair<Hand, std::optional<Hand>> m_hands;
    std::string m_name;
    int m_initial_money, m_actual_money, m_bet;

public:
    Player(std::string name, int initial_money, Deck &deck);

    int GetActualMoney() const;
    int GetInitialMoney() const;
    std::string_view GetName() const;
    const std::pair<Hand, std::optional<Hand>> &GetHands() const;

    void ResetHands();

    void Bet(int money);

    void Hit(int hand_index);

    std::optional<std::string_view> Double();
    std::optional<std::string_view> Surrender();
    std::optional<std::string_view> Split();
    int Win(int hand_index);
    void Lose();
};

std::ostream &operator<<(std::ostream &stream, const Player &player);
