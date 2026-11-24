#include <iostream>
#include <string>

using namespace std;

struct Student {
    string name;
    int category;
    int priority;
    int minTok;
    int maxTok;
};

const int NUM_STUDENTS = 6;
const int NUM_CATS = 3;

bool checkConstraints(int idx, int N, Student students[], int alloc[],
                      int highLowThreshold, bool conflict[][NUM_STUDENTS]) {
    if (idx == N) {
        int highSum = 0, lowSum = 0;
        bool hasMin = false, hasMax = false;

        for (int i = 0; i < N; i++) {
            if (alloc[i] == students[i].minTok) hasMin = true;
            if (alloc[i] == students[i].maxTok) hasMax = true;
            if (students[i].priority == 1) highSum += alloc[i];
            else lowSum += alloc[i];
        }
        return hasMin && hasMax && (highSum - lowSum > highLowThreshold);
    }
    return true;
}

bool isValidSoFar(int idx, int val, Student students[], int alloc[],
                  bool conflict[][NUM_STUDENTS]) {
    for (int j = 0; j < idx; j++) {
        if (students[j].category == students[idx].category && alloc[j] == val) {
            return false;
        }
        if (students[j].priority > students[idx].priority && alloc[j] < val) {
            return false;
        }
        if (students[j].priority < students[idx].priority && alloc[j] > val) {
            return false;
        }
        if (conflict[j][idx] && alloc[j] == val) {
            return false;
        }
    }
    return true;
}

bool solveMess(int idx, int N, Student students[], int alloc[],
               int catUsed[], int catBase[], int catLimit[],
               int emergencyAllowed, int emergencyUsed,
               int highLowThreshold, bool conflict[][NUM_STUDENTS],
               int& finalEmergencyUsed);

bool tryTokenValues(int val, int idx, int N, Student students[], int alloc[],
                    int catUsed[], int catBase[], int catLimit[],
                    int emergencyAllowed, int emergencyUsed,
                    int highLowThreshold, bool conflict[][NUM_STUDENTS],
                    int& finalEmergencyUsed) {
    if (val > students[idx].maxTok) {
        return false;
    }

    int cat = students[idx].category;
    if (catUsed[cat] + val <= catLimit[cat] && isValidSoFar(idx, val, students, alloc, conflict)) {
        int extraNeeded = 0;
        if (catUsed[cat] + val > catBase[cat]) {
            int prevExtra = (catUsed[cat] > catBase[cat]) ? (catUsed[cat] - catBase[cat]) : 0;
            extraNeeded = (catUsed[cat] + val - catBase[cat]) - prevExtra;
        }

        if (emergencyUsed + extraNeeded <= emergencyAllowed) {
            alloc[idx] = val;
            catUsed[cat] += val;

            if (solveMess(idx + 1, N, students, alloc, catUsed, catBase, catLimit,
                          emergencyAllowed, emergencyUsed + extraNeeded,
                          highLowThreshold, conflict, finalEmergencyUsed)) {
                return true;
            }

            catUsed[cat] -= val;
        }
    }

    return tryTokenValues(val + 1, idx, N, students, alloc, catUsed, catBase, catLimit,
                          emergencyAllowed, emergencyUsed, highLowThreshold, conflict, finalEmergencyUsed);
}

bool solveMess(int idx, int N, Student students[], int alloc[],
               int catUsed[], int catBase[], int catLimit[],
               int emergencyAllowed, int emergencyUsed,
               int highLowThreshold, bool conflict[][NUM_STUDENTS],
               int& finalEmergencyUsed) {
    if (idx == N) {
        if (checkConstraints(idx, N, students, alloc, highLowThreshold, conflict)) {
            finalEmergencyUsed = emergencyUsed;
            return true;
        }
        return false;
    }

    return tryTokenValues(students[idx].minTok, idx, N, students, alloc, catUsed, catBase, catLimit,
                          emergencyAllowed, emergencyUsed, highLowThreshold, conflict, finalEmergencyUsed);
}

bool findMinEmergency(int emergencyAllowed, int maxEmergency, int N, Student students[],
                      int alloc[], int catUsed[], int catBase[], int catLimit[],
                      int highLowThreshold, bool conflict[][NUM_STUDENTS],
                      int& finalEmergencyUsed) {
    if (emergencyAllowed > maxEmergency) {
        return false;
    }

    if (solveMess(0, N, students, alloc, catUsed, catBase, catLimit,
                  emergencyAllowed, 0, highLowThreshold, conflict, finalEmergencyUsed)) {
        return true;
    }

    return findMinEmergency(emergencyAllowed + 1, maxEmergency, N, students, alloc,
                            catUsed, catBase, catLimit, highLowThreshold, conflict, finalEmergencyUsed);
}

void printResults(int idx, int N, Student students[], int alloc[], string catNames[]) {
    if (idx == N) return;
    cout << students[idx].name << " (" << catNames[students[idx].category]
         << ", Priority " << students[idx].priority << "): " << alloc[idx] << " tokens" << endl;
    printResults(idx + 1, N, students, alloc, catNames);
}

int main() {
    Student students[NUM_STUDENTS] = {
        {"Ali",    0, 1, 3, 6},
        {"Sara",   0, 0, 1, 3},
        {"Bilal",  1, 1, 4, 7},
        {"Hina",   1, 0, 1, 4},
        {"Usman",  2, 1, 3, 6},
        {"Ayesha", 2, 0, 1, 3}
    };

    string catNames[NUM_CATS] = {"Breakfast", "Lunch", "Dinner"};
    int catBase[NUM_CATS] = {7, 8, 7};
    int catLimit[NUM_CATS] = {10, 11, 10};
    int catUsed[NUM_CATS] = {0, 0, 0};
    int alloc[NUM_STUDENTS] = {0};

    bool conflict[NUM_STUDENTS][NUM_STUDENTS] = {false};
    conflict[0][2] = conflict[2][0] = true;

    int highLowThreshold = 5;
    int maxEmergency = 6;
    int finalEmergencyUsed = 0;

    if (findMinEmergency(0, maxEmergency, NUM_STUDENTS, students, alloc, catUsed,
                         catBase, catLimit, highLowThreshold, conflict, finalEmergencyUsed)) {
        cout << "Final Allocation:" << endl;
        printResults(0, NUM_STUDENTS, students, alloc, catNames);

        int totalBase = catBase[0] + catBase[1] + catBase[2];
        int totalUsed = catUsed[0] + catUsed[1] + catUsed[2];
        int unusedBase = (totalBase + finalEmergencyUsed) - totalUsed;

        cout << "\nCategory Totals:" << endl;
        cout << "Breakfast: " << catUsed[0] << " / " << catBase[0] << endl;
        cout << "Lunch:     " << catUsed[1] << " / " << catBase[1] << endl;
        cout << "Dinner:    " << catUsed[2] << " / " << catBase[2] << endl;
        cout << "\nEmergency Tokens Used: " << finalEmergencyUsed << endl;
        cout << "Unused Base Tokens:    " << unusedBase << endl;
    } else {
        cout << "No valid token allocation exists." << endl;
    }

    return 0;
}
