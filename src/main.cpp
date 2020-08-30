#include <charconv>
#include <array>
#include "player.h"

std::optional<int> AskUserNumber(std::string_view prompt)
{
    auto input = AskUser("How many decks do you wanna use? (4-8)");
    try
    {
        int val = std::stoi(input);
        if (val > 0)
        {

            std::cout << "The number must be greater than 0.\n"
                      << std::endl;
            return {};
        }
        else
            return val;
        return val;
    }
    catch (std::invalid_argument const &e)
    {
        std::cout << "Expected integer input\n"
                  << std::endl;
    }
    return {};
}

std::optional<std::string_view> AskUserNumer(std::string_view);
void StartGame(std::vector<Player> &players, Deck &deck)
{
    int nOfPeople = AskNumberOfPeople();
    AskAndSetPlayerAttributes(nOfPeople, players, deck);
}
int AskNumberOfPeople()
{
    int nOfPeople;
    while (true)
    {
        std::optional<int> maybeNumber =
            AskUserNumber("\nHow many people are going to play? (1-7)");
        if (!maybeNumber.has_value())
        {
            continue;
        }
        int number = maybeNumber.value();
        if (!(0 < number && number <= 7))
        {
            std::cout << "The number of people must be between 1 and 7"
                      << std::endl;
        }
        else
        {
            nOfPeople = number;
            break;
        }
    }
    return nOfPeople;
}
void AskAndSetPlayerAttributes(
    int nOfPeople,
    std::vector<Player> &players,
    Deck &deck)
{
    for (int i = 0; i < nOfPeople; i++)
    {
        auto name = AskUser("\nPlease, enter your name player " + std::to_string(i));
        while (true)
        {
        }
    }
}
int AskPlayerBet(Player &player);
bool HandWinOrLose(Hand &hand);
bool CheckIfYes(std::string_view user_decision)
{
    return user_decision == "yes" || user_decision == "y" || user_decision == "Yes" || user_decision == "Y" || user_decision == "1";
}
// GetPlayerHand (?)
void PlayerTurn(Player &player, Deck &deck);
bool DealerLost(Hand &dealer_hand);
void DealerTurn(Hand &dealer_hand, Deck &deck);
void EndGame(std::vector<Player> &players, Hand &dealer_hand);
bool AskIfNextGame(Player &player);
bool NextGame(std::vector<Player> &players, Hand &dealer_hand, Deck &deck);
std::string AskUser(std::string_view prompt)
{
    std::cout << prompt << " ";
    std::string input;
    std::getline(std::cin, input);
    return input;
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
        for (auto player : players)
        {
            PlayerTurn(player, deck);
        }

        DealerTurn(dealer_hand, deck);
        EndGame(players, dealer_hand);
        if (!NextGame(players, dealer_hand, deck))
            break;
    }
}
