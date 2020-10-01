#include "player.h"

std::string AskUser(std::string_view prompt)
{
    std::cout << prompt << "\n> ";
    std::string input;
    std::getline(std::cin, input);
    std::cout << std::endl;
    return input;
}

std::optional<int> AskUserNumber(std::string_view prompt)
{
    std::cout << prompt << "\n> ";
    int val;
    std::cin >> val;
    std::cin.ignore();

    std::cout << std::endl;
    if (val <= 0)
    {

        std::cout << "The number must be a number greater than 0.\n"
                  << std::endl;
        return {};
    }
    else
        return val;
}

void AskAndSetPlayerAttributes(
    int n_of_people,
    std::vector<Player> &players,
    Deck &deck)
{
    for (int i = 0; i < n_of_people; i++)
    {
        auto name = AskUser("\nPlease, enter your name player " + std::to_string(i));
        while (true)
        {
            std::optional<int> maybe_number =
                AskUserNumber("How much money do you have? (Use only integer values)");
            if (!maybe_number.has_value())
                continue;
            int initial_money = maybe_number.value();
            if (initial_money < 50)
            {
                std::cout << "The initial money must be greater or equal than 50\n"
                          << std::endl;
            }
            else
            {
                players.push_back(Player(name, initial_money, deck));
                break;
            }
        }
    }
}

int AskNumberOfPeople()
{
    int n_of_people;
    while (true)
    {
        std::optional<int> maybe_number =
            AskUserNumber("\nHow many people are going to play? (1-7)");
        if (!maybe_number.has_value())
            continue;
        int number = maybe_number.value();
        if (!(0 < number && number <= 7))
        {
            std::cout << "The number of people must be between 1 and 7"
                      << std::endl;
        }
        else
        {
            n_of_people = number;
            break;
        }
    }
    return n_of_people;
}

void StartGame(std::vector<Player> &players, Deck &deck)
{
    int n_of_people = AskNumberOfPeople();
    AskAndSetPlayerAttributes(n_of_people, players, deck);
}

int AskPlayerBet(Player &player)
{
    int bet;
    while (true)
    {
        std::optional<int> maybe_number = AskUserNumber("What bet do you wanna make? (Use only integral values)");
        if (!maybe_number.has_value())
            continue;

        int number = maybe_number.value();
        if (number > player.GetActualMoney())
        {
            std::cout << "Your bet cannot be greater than your actual money.\n"
                      << std::endl;
        }
        else
        {
            bet = number;
            break;
        }
    }
    return bet;
}
bool HandWinOrLose(Hand &hand)
{
    if (hand.HasBlackJack())
    {
        std::cout << "BLACKJACK!\n"
                  << std::endl;
        return true;
    }
    else
    {
        switch (hand.GetPoints())
        {
        case 21:
            std::cout << "YOU GOT 21 POINTS!\n"
                      << std::endl;
            return true;

        case 0:
            std::cout << "BUST.\nI'm afraid you lose this game :(\n"
                      << std::endl;
            return true;

        default:
            return false;
        }
    }
}

bool CheckIfYes(std::string_view user_decision)
{
    return user_decision == "yes" || user_decision == "y" || user_decision == "Yes" || user_decision == "Y" || user_decision == "1";
}

const Hand &GetPlayerHand(Player &player, int hand_index)
{
    return hand_index == 0 ? player.GetHands().first
                           : player.GetHands().second.value();
}
void PlayerTurn(Player &player, Deck &deck)
{
    std::cout << "\n"
              << player << ", your actual money is "
              << player.GetActualMoney() << " €\n"
              << std::endl;
    int bet = AskPlayerBet(player);
    player.Bet(bet);
    Hand hand = player.GetHands().first;
    std::cout << "\nYour cards are:\n"
              << hand.GetCards()[0]
              << " and " << hand.GetCards()[1]
              << " (" << hand.GetPoints() << " points)\n"
              << std::endl;

    bool has_doubled;
    for (int i = 0; i < 2; i++)
    {
        has_doubled = false;
        while (!HandWinOrLose(hand) && (!has_doubled || hand.GetCards().size() < 3))
        {
            hand = GetPlayerHand(player, i);
            if (player.GetHands().second.has_value())
            {
                std::cout << "\n(Hand #" << (i + 1) << ")" << std::endl;
            }
            auto user_decision = AskUser("What do you want to do?\nAvailable Commands: (h)it, (s)tand, (sp)lit, (d)ouble, (surr)ender");
            if (user_decision == "h" || user_decision == "hit")
            {
                player.Hit(i);
                hand = GetPlayerHand(player, i);

                std::cout << "Now, the cards are: " << hand << std::endl;
            }
            else if (user_decision == "s" || user_decision == "stand")
            {
                std::cout << player << " stood" << std::endl;
                break;
            }
            else if (user_decision == "sp" || user_decision == "split")
            {
                if (!has_doubled)
                {
                    auto error_message = player.Split();
                    if (error_message.has_value())
                    {
                        std::cout << error_message.value() << std::endl;
                    }
                    else
                    {
                        std::cout << "You have splitted the hand!\n"
                                  << std::endl;
                    }
                }
                else
                {
                    std::cout << "Cannot split because you have already doubled\n"
                              << std::endl;
                }
            }
            else if (user_decision == "d" || user_decision == "double")
            {
                if (!has_doubled)
                {
                    auto error_message = player.Double();
                    if (error_message.has_value())
                    {
                        std::cout << error_message.value() << std::endl;
                    }
                    else
                    {
                        has_doubled = true;
                        std::cout << "You have doubled the hand!\n"
                                  << std::endl;
                    }
                }
                else
                {
                    std::cout << "Cannot double more than once!\n"
                              << std::endl;
                }
            }
            else if (user_decision == "surr" || user_decision == "surrender")
            {
                if (!has_doubled)
                {
                    auto error_message = player.Surrender();
                    if (error_message.has_value())
                    {
                        std::cout << error_message.value() << std::endl;
                    }
                    else
                    {
                        std::cout << "You have surrendered!\n"
                                  << std::endl;
                    }
                }
                else
                {
                    std::cout << "Cannot surrender because you have already doubled\n"
                              << std::endl;
                }
            }
            else
            {
                std::cout << "Invalid command!\nAvailable Commands: (h)it, (s)tand, (sp)lit, (d)ouble, (surr)ender" << std::endl;
            }
        }
        if (!player.GetHands().second.has_value())
            break;
    }
}
bool DealerLost(Hand &dealer_hand)
{
    if (dealer_hand.GetPoints() == 0)
    {
        std::cout << "The dealer busted. The game ended :)\n"
                  << std::endl;
        return true;
    }
    return false;
}

void DealerTurn(Hand &dealer_hand, Deck &deck)
{
    auto dealer_cards = dealer_hand.GetCards();
    std::cout << "\nThe dealer's cards are "
              << dealer_cards[0] << " and "
              << dealer_cards[1] << "\n"
              << std::endl;
    while (!DealerLost(dealer_hand) && dealer_hand.GetPoints() < 17)
    {
        std::cout << "The dealer is going to hit a card\n"
                  << std::endl;
        dealer_hand.DealCard();
        std::cout << "Now, the cards of the dealer are: " << dealer_hand << std::endl;
    }
}
void EndGame(std::vector<Player> &players, Hand &dealer_hand)
{
    std::cout << "####### Game Finished #######\n"
              << std::endl;
    int dealer_points = dealer_hand.GetPoints();
    Hand hand;
    int hand_points;
    for (Player &player : players)
    {
        for (int i = 0; i < 2; i++)
        {
            // Possibly avoid copy
            hand = i == 0 ? player.GetHands().first : player.GetHands().second.value();
            hand_points = hand.GetPoints();

            if (hand_points > dealer_points || hand.HasBlackJack() && !dealer_hand.HasBlackJack())
            {
                int moneyEarned = player.Win(i);
                std::cout << player << (player.GetHands().second.has_value() ? " (#" + std::to_string(i + 1) + " hand)" : "")
                          << " won " << moneyEarned << "€! :)\n"
                          << std::endl;
            }
            else if (hand_points == 0 || hand_points < dealer_points)
            {
                player.Lose();
                std::cout << player << (player.GetHands().second.has_value() ? " (#" + std::to_string(i + 1) + " hand)" : "")
                          << " lost! :(\n"
                          << std::endl;
            }
            else
            {
                std::cout << player << (player.GetHands().second.has_value() ? " (#" + std::to_string(i + 1) + " hand)" : "")
                          << " tied! :|\n"
                          << std::endl;
            }
            if (!player.GetHands().second.has_value())
                break;
        }
    }
}
bool AskIfNotNextGame(Player &player)
{
    bool player_next_game;
    int final_balance = player.GetActualMoney() - player.GetInitialMoney(); // TODO
    std::string finalBalanceString = (final_balance >= 0 ? "+" : "") + std::to_string(final_balance);
    if (player.GetActualMoney() > 0)
    {
        std::string decision = AskUser("\n" + std::string(player.GetName()) + ", do you want to play again? (y/n)\n");

        if (CheckIfYes(decision))
            player_next_game = true;
        else
        {
            player_next_game = false;
            std::cout << "Thanks for playing, " << player
                      << ", your final balance is " << final_balance
                      << "\n"
                      << std::endl;
        }
    }
    else
    {
        player_next_game = false;
        std::cout << player << ", you have lost all your money. Thanks for playing\n"
                  << std::endl;
    }
    return !player_next_game;
}

bool NextGame(std::vector<Player> &players, Hand &dealer_hand, Deck &deck)
{
    players.erase(std::remove_if(players.begin(), players.end(), AskIfNotNextGame), players.end());
    for (Player &player : players)
    {
        player.ResetHands();
    }

    std::cout << "\n\n\n"
              << std::endl;

    if (!players.empty())
    {
        dealer_hand.InitializeAtributes();
        return true;
    }
    return false;
}

int AskUserNOfDecks()
{
    int n_of_decks;
    while (true)
    {
        std::optional<int> number =
            AskUserNumber("How many decks do you wanna use? (4-8)");
        if (number.has_value())
        {
            if (number >= 4 && number <= 8)
            {
                n_of_decks = number.value();
                break;
            }
            else
            {
                std::cout << "The number of decks must be between 4 and 8" << std::endl;
            }
        }
        else
        {
            std::cout << "Expected integral input" << std::endl;
        }
    }
    return n_of_decks;
}

int main()
{
    std::cout << "Welcome to BlackJack!\n"
              << std::endl;

    std::vector<Player> players;
    int n_of_decks = AskUserNOfDecks();

    Deck deck(n_of_decks);
    Hand::SetDeck(deck);

    Hand dealer_hand;

    std::cout << "####### Game Started #######" << std::endl;

    StartGame(players, deck);
    while (true)
    {
        std::cout << "\nThe first card of the dealer is "
                  << dealer_hand.GetCards()[0] << "\n"
                  << std::endl;
        for (Player &player : players)
        {
            PlayerTurn(player, deck);
        }

        DealerTurn(dealer_hand, deck);
        EndGame(players, dealer_hand);
        if (!NextGame(players, dealer_hand, deck))
            break;
    }
}
