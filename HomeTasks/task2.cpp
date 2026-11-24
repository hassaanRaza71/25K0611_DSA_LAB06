#include <iostream>
#include <string>

using namespace std;

struct Runner {
    string name;
    double stamina;
    double speed;
};

bool canHandover(const Runner& curr, const Runner& next) {
    return (curr.speed - next.speed) >= (next.stamina - curr.stamina);
}

bool isSwapValid(int pos, int candidateIdx, Runner runners[], int order[], double avgStamina, int N) {
    int prevIdx = order[pos - 1];
    const Runner& A = runners[prevIdx];
    const Runner& B = runners[candidateIdx];

    if (!canHandover(B, A)) {
        return false;
    }

    if (pos - 2 >= 0) {
        const Runner& P = runners[order[pos - 2]];
        if (!canHandover(P, B)) {
            return false;
        }
    }

    if (pos == N - 2 && A.stamina < avgStamina) {
        return false;
    }

    return true;
}

void printOrder(int idx, int N, Runner runners[], int order[]) {
    if (idx == N) {
        cout << endl;
        return;
    }
    cout << runners[order[idx]].name << (idx < N - 1 ? " -> " : "");
    printOrder(idx + 1, N, runners, order);
}

void backtrack(int pos, int N, Runner runners[], bool used[], int order[], double avgStamina, int& validCount);

void tryRunners(int i, int pos, int N, Runner runners[], bool used[], int order[], double avgStamina, int& validCount) {
    if (i == N) {
        return;
    }

    if (!used[i]) {
        bool feasible = true;

        if ((pos == N - 2 || pos == N - 1) && runners[i].stamina < avgStamina) {
            feasible = false;
        }

        if (feasible && pos > 0) {
            if (!canHandover(runners[order[pos - 1]], runners[i])) {
                feasible = false;
            } else if (isSwapValid(pos, i, runners, order, avgStamina, N)) {
                feasible = false;
            }
        }

        if (feasible) {
            used[i] = true;
            order[pos] = i;

            backtrack(pos + 1, N, runners, used, order, avgStamina, validCount);

            used[i] = false;
        }
    }

    tryRunners(i + 1, pos, N, runners, used, order, avgStamina, validCount);
}

void backtrack(int pos, int N, Runner runners[], bool used[], int order[], double avgStamina, int& validCount) {
    if (pos == N) {
        validCount++;
        cout << "Order " << validCount << ": ";
        printOrder(0, N, runners, order);
        return;
    }

    tryRunners(0, pos, N, runners, used, order, avgStamina, validCount);
}

double sumStamina(int idx, int N, Runner runners[]) {
    if (idx == N) return 0.0;
    return runners[idx].stamina + sumStamina(idx + 1, N, runners);
}

int main() {
    const int N = 4;
    Runner runners[N] = {
        {"R1", 80, 90},
        {"R2", 75, 85},
        {"R3", 90, 70},
        {"R4", 85, 80}
    };

    bool used[N] = {false};
    int order[N];
    int validCount = 0;

    double avgStamina = sumStamina(0, N, runners) / N;

    cout << "Average Team Stamina: " << avgStamina << "\n\n";
    backtrack(0, N, runners, used, order, avgStamina, validCount);

    if (validCount == 0) {
        cout << "No valid running orders found." << endl;
    }

    return 0;
}
