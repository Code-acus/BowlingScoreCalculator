#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>

using namespace std;

class BowlingScoreCalculator {
private:
    vector<int> rolls;

public:
    void addSet(int pins) {
        rolls.push_back(pins);
    }

    int calculateScore() {
        int score = 0;
        int frameIndex = 0;

        for (int frame = 0; frame < 10; ++frame) {
            if (isStrike(frameIndex)) {
                score += 10 + strikeBonus(frameIndex);
                frameIndex++;
            } else if (isSpare(frameIndex)) {
                score += 10 + spareBonus(frameIndex);
                frameIndex += 2;
            } else {
                score += sumOfPinsInFrame(frameIndex);
                frameIndex += 2;
            }
        }
        return score;
    }

private:
    bool isStrike(int frameIndex) {
        return rolls[frameIndex] == 10;
    }

    bool isSpare(int frameIndex) {
        return sumOfPinsInFrame(frameIndex) == 10;
    }

    int strikeBonus(int frameIndex) {
        return rolls[frameIndex + 1] + rolls[frameIndex + 2];
    }

    int spareBonus(int frameIndex) {
        return rolls[frameIndex + 2];
    }

    int sumOfPinsInFrame(int frameIndex) {
        return rolls[frameIndex] + rolls[frameIndex + 1];
    }
};

void promptEnter() {
    cout << "Press Enter to continue..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int main() {
    BowlingScoreCalculator calculator;

    cout << "Enter each set as 'x, y' where x and y represent the number of pins knocked down in two attempts." << endl;
    cout << "If you knock down all 10 pins in the first attempt, enter it as '10'." << endl;
    cout << "Example game: 4, 3  5, 5  8, 1  10  3, 6  10  10  9, 0  0, 8  10, 7, 1" << endl;
    cout << "Enter 'q' to finish entering sets." << endl;

    promptEnter();

    string input;
    int frame = 1;
    while (frame <= 10) {
        cout << "Frame " << frame << " - Set: ";
        getline(cin, input);

        if (input == "q")
            break;

        stringstream ss(input);
        int pins1, pins2 = 0;
        char delimiter;

        if (ss >> pins1) {
            calculator.addSet(pins1);
            if (pins1 != 10) {
                if (!(ss >> delimiter >> pins2)) {
                    cout << "Invalid input. You need to enter two numbers for this set." << endl;
                    continue;
                } else if (pins1 + pins2 > 10) {
                    cout << "Invalid input. The sum of two attempts cannot exceed 10." << endl;
                    continue;
                }
                calculator.addSet(pins2);
            }
        } else {
            cout << "Invalid input. Please enter sets in the format described above." << endl;
            continue;
        }

        frame++;
    }

    int score = calculator.calculateScore();
    cout << "Final Score: " << score << endl;

    // Define and initialize variables for perfect score and national average

    const int perfectScore = 300;
    const double nationalAverage = 150.0; // National average score (example value)

    // Calculate the percentage difference compared to a perfect game
    double percentageDifference = (score / static_cast<double>(perfectScore)) * 100.0;

    // Calculate the handicap percentage based on the national average
    double handicapPercentage = abs((percentageDifference - nationalAverage) / nationalAverage) * 100.0;

    // Display the information
    cout << "In contrast to a perfect game (300 points), your score is: " << percentageDifference << "%" << endl;
    cout << "Compared to the national average score of " << nationalAverage << ", your handicap percentage is: " << handicapPercentage << "%" << endl;

    return 0;
}
