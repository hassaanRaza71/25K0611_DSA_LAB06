#include <iostream>
#include <string>

using namespace std;

void fillPosition(int pos, int N, int currentSum, int targetSum,
                  string currentCode, bool used[10],
                  const string& targetCode, bool& targetFound);

void tryDigits(int digit, int pos, int N, int currentSum, int targetSum,
               string currentCode, bool used[10],
               const string& targetCode, bool& targetFound) {
    if (digit > 9) {
        return;
    }

    bool isLeadingZero = (pos == 0 && digit == 0);
    if (!isLeadingZero && !used[digit] && (currentSum + digit <= targetSum)) {
        used[digit] = true;

        fillPosition(pos + 1, N, currentSum + digit, targetSum,
                     currentCode + to_string(digit), used, targetCode, targetFound);

        used[digit] = false;
    }

    tryDigits(digit + 1, pos, N, currentSum, targetSum,
              currentCode, used, targetCode, targetFound);
}

void fillPosition(int pos, int N, int currentSum, int targetSum,
                  string currentCode, bool used[10],
                  const string& targetCode, bool& targetFound) {
    if (pos == N) {
        if (currentSum == targetSum) {
            cout << currentCode << endl;
            if (currentCode == targetCode) {
                targetFound = true;
            }
        }
        return;
    }

    tryDigits(0, pos, N, currentSum, targetSum,
              currentCode, used, targetCode, targetFound);
}

int main() {
    int N = 3;
    int targetSum = 6;
    string targetCode = "123";

    bool used[10] = {false};
    bool targetFound = false;

    cout << "Valid lock codes (N = " << N << ", Sum = " << targetSum << "):" << endl;

    if (N >= 1 && N <= 10) {
        fillPosition(0, N, 0, targetSum, "", used, targetCode, targetFound);
    }

    cout << "\nTarget code \"" << targetCode << "\" found? "
         << (targetFound ? "Yes" : "No") << endl;

    return 0;
}
