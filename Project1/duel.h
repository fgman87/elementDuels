#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <map>

using namespace std;

// ANSI escape codes for text color
const string GREEN_TEXT = "\033[1;32m";
const string GOLD_TEXT = "\033[1;33m";
const string RED_TEXT = "\033[1;31m";
const string LIGHT_BLUE_TEXT = "\033[1;36m";
const string RESET_COLOR = "\033[0m";

class Game {
private:
    int userHealth;
    int aiHealth;
    int userMana;
    int aiMana;
    int roundCount;
    bool userAttacker;
    map<string, int> userElementCount;

public:
    Game() {
        userHealth = 3;
        aiHealth = 3;
        userMana = 4;
        aiMana = 4;
        roundCount = 0;
        userAttacker = (rand() % 2 == 0);
        userElementCount["Fire"] = 0;
        userElementCount["Grass"] = 0;
        userElementCount["Water"] = 0;
    }

    int getRandomValue(int max) {
        return rand() % (max + 1) + 1;
    }

    int aiDecision() {
        int playStyle = rand() % 3;
        int maxDecision;

        if (playStyle == 0) {
            maxDecision = min(aiMana, 3); // Defensive play
        }
        else if (playStyle == 1) {
            maxDecision = min(aiMana, 6); // Aggressive play
        }
        else {
            maxDecision = aiMana; // Random play
        }

        // Generate a random decision within the allowed range
        int decision = rand() % (maxDecision + 1);

        // Ensure the AI's decision is always above 1
        if (decision < 2) {
            decision = 2;
        }

        return decision;
    }


    void determineElementInteraction(string userElement, string aiElement, int& attackerValue, int& defenderValue) {
        if (userElement == aiElement) {
            return; // Elements are the same, no effect
        }

        if (userAttacker) {
            if ((userElement == "Fire" && aiElement == "Grass") ||
                (userElement == "Grass" && aiElement == "Water") ||
                (userElement == "Water" && aiElement == "Fire")) {
                attackerValue = static_cast<int>(attackerValue * 1.5);
            }
            else {
                defenderValue = static_cast<int>(defenderValue * 1.5);
            }
        }
        else {
            if ((userElement == "Fire" && aiElement == "Grass") ||
                (userElement == "Grass" && aiElement == "Water") ||
                (userElement == "Water" && aiElement == "Fire")) {
                defenderValue = static_cast<int>(defenderValue * 1.5);
            }
            else {
                attackerValue = static_cast<int>(attackerValue * 1.5);
            }
        }
    }

    void playRound() {
        userMana += 2;
        aiMana += 2;
        string attackElement, defenseElement;
        int attackerValue, defenderValue;

        if (userAttacker) {
            cout << GREEN_TEXT << "Round " << (roundCount + 1) << " - User is the Attacker!" << RESET_COLOR << endl;
            cout << RED_TEXT << "Current Health - User: " << userHealth << " AI: " << aiHealth << endl;
            cout << LIGHT_BLUE_TEXT << "Current Mana - User: " << userMana << " AI: " << aiMana << endl;

            attackerValue = getUserInput(userMana);
            defenderValue = aiDecision();
            userMana -= attackerValue;
            aiMana -= defenderValue;
        }
        else {
            cout << GOLD_TEXT << "Round " << (roundCount + 1) << " - AI is the Attacker!" << RESET_COLOR << endl;
            cout << RED_TEXT << "Current Health - User: " << userHealth << " AI: " << aiHealth << endl;
            cout << LIGHT_BLUE_TEXT << "Current Mana - User: " << userMana << " AI: " << aiMana << endl;

            attackerValue = aiDecision();
            defenderValue = getUserInput(userMana);
            aiMana -= attackerValue;
            userMana -= defenderValue;
        }

        // Ask the user to choose an element
        string elements[] = { "Fire", "Water", "Grass" };
        cout << "Choose an element (Fire, Water, Grass): ";
        string userElement;
        cin >> userElement;

        userElementCount[userElement]++;

        // Randomly choose an element for the AI
        string aiElement = elements[rand() % 3];

        // Determine element interaction and modify values
        determineElementInteraction(userElement, aiElement, attackerValue, defenderValue);

        if (userAttacker) {
            cout << "Attacker's element: " << userElement << " Defender's element: " << aiElement << endl;
        }
        else {
            cout << "Attacker's element: " << aiElement << " Defender's element: " << userElement << endl;
        }

        cout << "Attacker's value: " << attackerValue << " Defender's value: " << defenderValue << endl;
        cout << "-------------------------------------------------------" << endl;
        cout << LIGHT_BLUE_TEXT << "---------------------- +2 MANA ------------------------" << RESET_COLOR << endl;

        if (attackerValue > defenderValue) {
            if (userAttacker) {
                aiHealth--;
            }
            else {
                userHealth--;
            }
        }

        userAttacker = !userAttacker;
        roundCount++;
    }

    bool isGameOver() {
        return userHealth <= 0 || aiHealth <= 0;
    }

    int getUserInput(int maxInput) {
        int input;
        do {
            int maxValidInput = min(min(maxInput, 6), userMana);
            cout << RESET_COLOR << "Enter a value (1-" << maxValidInput << "): ";
            cin >> input;
        } while (input < 1 || input > min(maxInput, 6) || input > userMana);
        return input;
    }

    void start() {
        srand(time(0));

        while (!isGameOver()) {
            playRound();
        }

        if (userHealth <= 0) {
            cout << RED_TEXT << "Game over. AI wins!" << RESET_COLOR << endl;
        }
        else {
            cout << GREEN_TEXT << "Game over. User wins!" << RESET_COLOR << endl;
        }

        char playAgain;
        cout << "Play again? (Y/N): ";
        cin >> playAgain;
        if (playAgain == 'Y' || playAgain == 'y') {
            Game newGame;
            newGame.start();
        }
    }
};

int main() {
    cout << "Welcome to the Element Duel Game!" << endl;
    Game game;
    game.start();
    return 0;
}