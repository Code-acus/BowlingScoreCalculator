#include <iostream>
#include <vector>
#include <sstream>
#include <regex>
#include <iomanip>

using namespace std;

class BowlingScoreCalculator {
private:
    vector<int> rolls;

public:
    void roll(int pins) {
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
                score += sumOfPinsInSet(frameIndex);
                frameIndex += 2;
            }
        }

        return score;
    }

    double calculatePercentage() {
        return static_cast<double>(calculateScore()) / 300.0 * 100;
    }

    int calculateHandicap() {
        return (200 - calculateScore()) / 10;
    }

    int compareNationalAverage() {
        int nationalAverage = 150;
        return calculateScore() - nationalAverage;
    }

private:
    bool isStrike(int frameIndex) {
        return rolls[frameIndex] == 10;
    }

    bool isSpare(int frameIndex) {
        return rolls[frameIndex] + rolls[frameIndex + 1] == 10;
    }

    int strikeBonus(int frameIndex) {
        return rolls[frameIndex + 1] + rolls[frameIndex + 2];
    }

    int spareBonus(int frameIndex) {
        return rolls[frameIndex + 2];
    }

    int sumOfPinsInSet(int frameIndex) {
        return rolls[frameIndex] + rolls[frameIndex + 1];
    }
};

string provideAdvice(int score) {
    if (score < 100) {
        return "Practice more on your aim and try to knock down more pins each time.";
    } else if (score < 200) {
        return "Work on your strikes and spares to get higher scores.";
    } else {
        return "You're doing great! Keep practicing to maintain your performance.";
    }
}

bool validateInput(const string& input, int frame) {
    regex r("(10)|([0-9], [0-9])|([0-9], 10)|([0-9]|10, [0-9]|10)");
    if (frame < 10) return regex_match(input, r);
    else {
        // In frame 10, we can have two or three inputs, separated by ','
        regex r10("(10, 10, [0-9]|10)|(10, [0-9], [0-9])|([0-9], [0-9])|([0-9], 10, [0-9])");
        return regex_match(input, r10);
    }
}

int main() {
    BowlingScoreCalculator calculator;

    cout << "Enter each set as 'x' or 'x, y' where x and y represent the number of pins knocked down." << endl;
    cout << "If you strike, just input '10'. For a spare, input two numbers like '5, 5'." << endl;
    cout << "Example game: 4, 3  5, 5  8, 1  10  3, 6  10  10  9, 0  0, 8  10  7, 1" << endl;
    cout << "Enter 'q' to quit the application without scoring your game." << endl;

    string input;
    int frame = 1;
    while (frame <= 10) {
        cout << "Frame " << frame << " - Set: ";
        getline(cin, input);

        if (input == "q") {
            cout << "You have chosen to quit the application. Your score will not be calculated. Goodbye." << endl;
            return 0;
        }

        if (!validateInput(input, frame)) {
            cout << "Invalid input. Please make sure you either input '10' or two numbers separated by a comma and a space, like '5, 5'. For 10th frame, possible entries can be 'x, y, z', '10, x, y', 'x, y'." << endl;
            continue;
        }

        stringstream ss(input);
        int pins1, pins2 = 0, pins3 = 0;

        ss >> pins1;

        if (ss.peek() == ',') {
            ss.ignore();  // Ignore the comma
            ss >> pins2;
        }

        if (ss.peek() == ',') {
            ss.ignore();  // Ignore the comma
            ss >> pins3;
        }

        if (pins1 + pins2 > 10 && frame < 10) {  // Not allowed more than 10 pins in a frame before the 10th
            cout << "Invalid input. Please make sure your roll totals do not exceed 10 unless it's the 10th frame and you struck or spared." << endl;
            continue;
        }

        calculator.roll(pins1);
        calculator.roll(pins2);
        if(frame == 10) {
            calculator.roll(pins3);
        }

        frame++;
    }

    int score = calculator.calculateScore();
    cout << "Your game score is: " << setfill('0') << setw(3) << score << " / 300" << endl;

    double percentage = calculator.calculatePercentage();
    int handicap = calculator.calculateHandicap();
    int comparedToAverage = calculator.compareNationalAverage();
    string advice = provideAdvice(score);

    cout << "Your score percentage is: " << percentage << "%" << endl;
    cout << "Your handicap is: " << handicap << endl;
    cout << "Compared to the national average (150), your score is: " << (comparedToAverage > 0 ? "+" : "") << comparedToAverage << endl;
    cout << "Advice for improvement: " << advice << endl;

    return 0;
}

