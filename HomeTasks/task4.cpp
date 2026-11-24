#include <iostream>

using namespace std;

const int N = 4;
const int M = 4;

bool rowHasAffected(int r, int c, int people[][M]) {
    if (c == M) return false;
    if (people[r][c] > 0) return true;
    return rowHasAffected(r, c + 1, people);
}

void copySelection(int r, int c, bool src[][M], bool dst[][M]) {
    if (r == N) return;
    dst[r][c] = src[r][c];
    if (c + 1 < M) copySelection(r, c + 1, src, dst);
    else copySelection(r + 1, 0, src, dst);
}

void solveRescue(int r, int c, int people[][M], int units[][M],
                 int capacity, int usedUnits, int rescuedPeople,
                 int highRiskSelected, bool rowSelected,
                 bool currentSel[][M], bool bestSel[][M],
                 int& maxPeople, int& bestUnitsUsed, bool& foundValid) {
    if (r == N) {
        if (rescuedPeople > maxPeople || (rescuedPeople == maxPeople && usedUnits < bestUnitsUsed)) {
            maxPeople = rescuedPeople;
            bestUnitsUsed = usedUnits;
            foundValid = true;
            copySelection(0, 0, currentSel, bestSel);
        }
        return;
    }

    int nextR = (c + 1 == M) ? r + 1 : r;
    int nextC = (c + 1 == M) ? 0 : c + 1;
    bool endOfRow = (c + 1 == M);

    if (people[r][c] != -1 && units[r][c] != -1) {
        bool isHighRisk = (people[r][c] >= 10);
        int extraUnit = (isHighRisk && highRiskSelected >= 1) ? 1 : 0;
        int newUnits = usedUnits + units[r][c] + extraUnit;

        if (newUnits <= capacity) {
            currentSel[r][c] = true;
            solveRescue(nextR, nextC, people, units, capacity,
                        newUnits, rescuedPeople + people[r][c],
                        highRiskSelected + (isHighRisk ? 1 : 0),
                        endOfRow ? false : true,
                        currentSel, bestSel, maxPeople, bestUnitsUsed, foundValid);
            currentSel[r][c] = false;
        }
    }

    bool mustSelectRow = rowHasAffected(r, 0, people);
    if (!endOfRow || !mustSelectRow || rowSelected) {
        solveRescue(nextR, nextC, people, units, capacity,
                    usedUnits, rescuedPeople, highRiskSelected,
                    endOfRow ? false : rowSelected,
                    currentSel, bestSel, maxPeople, bestUnitsUsed, foundValid);
    }
}

void printSelected(int r, int c, bool bestSel[][M], int people[][M], int units[][M]) {
    if (r == N) return;
    if (bestSel[r][c]) {
        cout << "(" << r << ", " << c << ") -> People: " << people[r][c]
             << ", Base Units: " << units[r][c] << endl;
    }
    if (c + 1 < M) printSelected(r, c + 1, bestSel, people, units);
    else printSelected(r + 1, 0, bestSel, people, units);
}

int main() {
    int people[N][M] = {
        { 8, 12, -1,  6},
        { 5, 10,  7, -1},
        {-1,  9, 15,  4},
        { 6, -1,  8, 11}
    };

    int units[N][M] = {
        { 2,  3, -1,  2},
        { 1,  3,  2, -1},
        {-1,  2,  4,  1},
        { 2, -1,  2,  3}
    };

    int capacity = 12;
    bool currentSel[N][M] = {false};
    bool bestSel[N][M] = {false};
    int maxPeople = -1;
    int bestUnitsUsed = 0;
    bool foundValid = false;

    solveRescue(0, 0, people, units, capacity, 0, 0, 0, false,
                currentSel, bestSel, maxPeople, bestUnitsUsed, foundValid);

    if (foundValid) {
        cout << "Selected Matrix Positions:" << endl;
        printSelected(0, 0, bestSel, people, units);
        cout << "\nTotal People Rescued: " << maxPeople << endl;
        cout << "Total Rescue Units Used: " << bestUnitsUsed << endl;
        cout << "Remaining Units: " << (capacity - bestUnitsUsed) << endl;
    } else {
        cout << "No valid allocation exists." << endl;
    }

    return 0;
}
