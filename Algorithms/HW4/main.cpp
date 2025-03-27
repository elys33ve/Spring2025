#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

/*  ASSIGNMENT 4 - Stable Matchings and the Gale-Shapely Algorithm

    - input: list of man and women preferences
    - calculates stable marriages using the Gale-Shapely Algorithm
    - output: table of male preferences, female preferences, and marriages result

make && main.exe
*/

// ===========================================================================

// --- Find Preferences - get index of a man in a woman's preferences
int findPreference(vector<int>& w_preferences, int m) {
    for (int i=0; i<w_preferences.size(); i++) {
        if (w_preferences[i] == m)
            return i;
    }
    return -1; // error
}

// --- Gale-Shapely Algorithm
vector<int> galeShapelyAlgorithm(vector<vector<int>> &preferences) {
    int n = preferences[0].size();

    vector<int> w_matches(n, -1);       // women matches 
    vector<int> m_matches(n, -1);       // men matches
    vector<int> proposals(n, 0);        // men proposals
    int unpaired = n;

    while (unpaired > 0) {
        for (int m=0; m<n; m++) {
            // for each unpaired man
            if (m_matches[m] == -1) {
                int w = preferences[m][proposals[m]];
                proposals[m]++;

                // if woman unpaired
                if (w_matches[w] == -1) {
                    w_matches[w] = m;
                    m_matches[m] = w;
                    unpaired--;
                } else {
                    int man = w_matches[w];
                    // if woman prefers current man, switch
                    if (findPreference(preferences[n+w], m) < findPreference(preferences[n+w], man)) {
                        w_matches[w] = m;
                        m_matches[m] = w;
                        m_matches[man] = -1;
                    }
                }
            }
        }
    }
    return m_matches;
}

// --- Print Preferences
void printPreferences(vector<vector<int>> &preferences) {
    int n = preferences[0].size();

    cout << " man preferences:\t\t| woman preferences:\n";
    cout << "------------------------------------------------------------\n";
    for (int i=0; i<n; i++) {
        // print man preferences
        cout << "m" << i << ": \t";
        for (int j=0; j<n; j++) {
            cout << "w" << preferences[i][j] << ", ";
        } cout << "\t|";
        // print woman preferences
        cout << "w" << i << ": \t";
        for (int j=0; j<n; j++) {
            cout << "m" << preferences[i+n][j] << ", ";
        } cout << "\n";
    }
}
// --- Print Preferences (separate lines)
void printPreferences2(vector<vector<int>> &preferences) {
    int n = preferences[0].size();

    // print man preferences
    cout << "    men preferences:\n";
    for (int i=0; i<n; i++) {
        cout << "m" << i << ": \t";
        for (int j=0; j<n; j++) {
            cout << "w" << preferences[i][j] << ", ";
        } cout << "\n";
    }
    // print woman preferences
    cout << "    women preferences:\n";
    for (int i=0; i<n; i++) {
        cout << "w" << i << ": \t";
        for (int j=0; j<n; j++) {
            cout << "m" << preferences[i+n][j] << ", ";
        } cout << "\n";
    }
}

// ===========================================================================

int main() {
    int n = 5;      // number of men (or women)
    //vector<vector<int>> preferences = { {0,1,2},{1,0,2},{0,1,2}, {1,0,2},{2,1,0},{0,1,2} };
    //vector<vector<int>> preferences = { {0,2,3,1,4},{2,3,1,4,0},{4,2,3,1,0},{2,1,4,3,0},{0,3,2,4,1}, {2,3,1,0,4},{0,1,2,4,3},{2,3,4,1,0},{2,3,4,0,1},{2,3,0,1,4} };
    vector<vector<int>> preferences = { {2,0,3,1,4},{1,2,4,3,0},{4,2,3,1,0},{1,3,2,4,0},{3,2,0,4,1}, {1,0,2,3,4},{0,3,4,2,1},{2,3,1,4,0},{3,4,0,1,2},{2,1,0,4,3} }; 

    // print preferences
    cout << endl;
    printPreferences(preferences);

    vector<int> marriages = galeShapelyAlgorithm(preferences);

    // print pairing results
    cout << "\npairing results:\n";
    for (int i=0; i<n; i++) {
        cout << "m" << i << "\t" << "w" << marriages[i] << "\n";
    }

    return 0;
}