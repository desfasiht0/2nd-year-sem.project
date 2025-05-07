#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <queue>
#include <stack>

using namespace std;

// Structure for Team using pointers extensively
struct Team {
    string* name;
    int points;
    int goalsScored;
    int goalsConceded;
    
    Team(string n) : name(new string(n)), points(0), goalsScored(0), goalsConceded(0) {}
    ~Team() { delete name; }
    
    int getGoalDifference() const { return goalsScored - goalsConceded; }
    
    void display() const {
        cout << setw(15) << left << *name 
             << setw(6) << points 
             << setw(6) << goalsScored 
             << setw(6) << goalsConceded 
             << setw(6) << getGoalDifference() << endl;
    }
};

// Node structure for linked list implementation
struct TeamNode {
    Team* team;
    TeamNode* next;
    
    TeamNode(Team* t) : team(t), next(nullptr) {}
    ~TeamNode() { delete team; }
};

// Linked list implementation of Team collection
class TeamList {
private:
    TeamNode* head;
    
    // Recursive function for counting teams
    int countTeamsRecursive(TeamNode* current) const {
        if (!current) return 0;
        return 1 + countTeamsRecursive(current->next);
    }
    
    // Recursive function for displaying teams
    void displayRecursive(TeamNode* current) const {
        if (!current) return;
        current->team->display();
        displayRecursive(current->next);
    }
    
public:
    TeamList() : head(nullptr) {}
    ~TeamList() {
        while (head) {
            TeamNode* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void addTeam(Team* t) {
        TeamNode* newNode = new TeamNode(t);
        newNode->next = head;
        head = newNode;
    }
    
    Team* findTeam(const string& name) const {
        TeamNode* current = head;
        while (current) {
            if (*(current->team->name) == name) {
                return current->team;
            }
            current = current->next;
        }
        return nullptr;
    }
    
    int countTeams() const {
        return countTeamsRecursive(head);
    }
    
    void displayAll() const {
        displayRecursive(head);
    }
    
    // Get all teams as vector (for sorting)
    vector<Team*> getAllTeams() const {
        vector<Team*> teams;
        TeamNode* current = head;
        while (current) {
            teams.push_back(current->team);
            current = current->next;
        }
        return teams;
    }
};

// Match structure with pointers
struct Match {
    string* date;
    Team* team1;
    Team* team2;
    int score1;
    int score2;
    
    Match(string d, Team* t1, Team* t2, int s1, int s2)
        : date(new string(d)), team1(t1), team2(t2), score1(s1), score2(s2) {}
    ~Match() { delete date; }
    
    void display() const {
        cout << *date << ": " << *(team1->name) << " " << score1 << " - " 
             << score2 << " " << *(team2->name) << endl;
    }
};

// Binary Search Tree implementation for matches
class MatchBST {
private:
    struct MatchNode {
        Match* match;
        MatchNode* left;
        MatchNode* right;
        
        MatchNode(Match* m) : match(m), left(nullptr), right(nullptr) {}
        ~MatchNode() { delete match; }
    };
    
    MatchNode* root;
    
    // Recursive insert
    void insert(MatchNode* &node, Match* m) {
        if (!node) {
            node = new MatchNode(m);
        } else if (*m->date < *node->match->date) {
            insert(node->left, m);
        } else {
            insert(node->right, m);
        }
    }
    
    // Recursive in-order traversal
    void inOrder(MatchNode* node, vector<Match*>& matches, 
                const string& start, const string& end) const {
        if (!node) return;
        
        inOrder(node->left, matches, start, end);
        if (*node->match->date >= start && *node->match->date <= end) {
            matches.push_back(node->match);
        }
        inOrder(node->right, matches, start, end);
    }
    
    // Recursive destructor helper
    void clear(MatchNode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }
    
public:
    MatchBST() : root(nullptr) {}
    ~MatchBST() { clear(root); }
    
    void addMatch(Match* m) {
        insert(root, m);
    }
    
    vector<Match*> getMatchesInRange(const string& start, const string& end) const {
        vector<Match*> result;
        inOrder(root, result, start, end);
        return result;
    }
};

// Stack implementation for match history
class MatchHistory {
private:
    stack<Match*> history;
    
public:
    void addMatch(Match* m) {
        history.push(m);
    }
    
    void undoLastMatch() {
        if (!history.empty()) {
            Match* last = history.top();
            history.pop();
            delete last;
        }
    }
    
    bool isEmpty() const {
        return history.empty();
    }
};

// Queue implementation for match scheduling
class MatchSchedule {
private:
    queue<Match*> schedule;
    
public:
    void scheduleMatch(Match* m) {
        schedule.push(m);
    }
    
    Match* playNextMatch() {
        if (!schedule.empty()) {
            Match* next = schedule.front();
            schedule.pop();
            return next;
        }
        return nullptr;
    }
    
    bool isEmpty() const {
        return schedule.empty();
    }
};

// Sorting functions
class Sorter {
public:
    // Bubble sort implementation
    static void bubbleSortTeams(vector<Team*>& teams) {
        bool swapped;
        for (size_t i = 0; i < teams.size() - 1; i++) {
            swapped = false;
            for (size_t j = 0; j < teams.size() - i - 1; j++) {
                if (compareTeams(teams[j], teams[j+1]) > 0) {
                    swap(teams[j], teams[j+1]);
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }
    
    // Quick sort implementation
    static void quickSortTeams(vector<Team*>& teams, int low, int high) {
        if (low < high) {
            int pi = partition(teams, low, high);
            quickSortTeams(teams, low, pi - 1);
            quickSortTeams(teams, pi + 1, high);
        }
    }
    
private:
    // Comparison function for teams (returns >0 if a should come after b)
    static int compareTeams(Team* a, Team* b) {
        if (a->points != b->points) return b->points - a->points;
        if (a->getGoalDifference() != b->getGoalDifference()) 
            return b->getGoalDifference() - a->getGoalDifference();
        return b->goalsScored - a->goalsScored;
    }
    
    // Partition function for quick sort
    static int partition(vector<Team*>& teams, int low, int high) {
        Team* pivot = teams[high];
        int i = low - 1;
        
        for (int j = low; j <= high - 1; j++) {
            if (compareTeams(teams[j], pivot) < 0) {
                i++;
                swap(teams[i], teams[j]);
            }
        }
        swap(teams[i + 1], teams[high]);
        return i + 1;
    }
};

// Main score manager class
class ScoreManager {
private:
    TeamList teams;
    MatchBST matches;
    MatchHistory history;
    MatchSchedule schedule;
    
    void updateStandings(Team* t1, Team* t2, int s1, int s2) {
        // Update goals
        t1->goalsScored += s1;
        t1->goalsConceded += s2;
        t2->goalsScored += s2;
        t2->goalsConceded += s1;
        
        // Update points
        if (s1 > s2) {
            t1->points += 3;
        } else if (s2 > s1) {
            t2->points += 3;
        } else {
            t1->points += 1;
            t2->points += 1;
        }
    }
    
public:
    void addTeam(const string& name) {
        if (teams.findTeam(name)) {
            cout << "Team already exists!" << endl;
            return;
        }
        teams.addTeam(new Team(name));
        cout << "Team added successfully!" << endl;
    }
    
    void recordMatch(const string& date, const string& t1, 
                    const string& t2, int s1, int s2) {
        Team* team1 = teams.findTeam(t1);
        Team* team2 = teams.findTeam(t2);
        
        if (!team1 || !team2) {
            cout << "Error: One or both teams not found!" << endl;
            return;
        }
        
        Match* m = new Match(date, team1, team2, s1, s2);
        matches.addMatch(m);
        history.addMatch(m);
        updateStandings(team1, team2, s1, s2);
        cout << "Match recorded successfully!" << endl;
    }
    
    void scheduleMatch(const string& date, const string& t1, const string& t2) {
        Team* team1 = teams.findTeam(t1);
        Team* team2 = teams.findTeam(t2);
        
        if (!team1 || !team2) {
            cout << "Error: One or both teams not found!" << endl;
            return;
        }
        
        // Score will be determined when match is played
        Match* m = new Match(date, team1, team2, 0, 0);
        schedule.scheduleMatch(m);
        cout << "Match scheduled successfully!" << endl;
    }
    
    void playScheduledMatch() {
        Match* next = schedule.playNextMatch();
        if (next) {
            cout << "Playing scheduled match: " << *(next->team1->name) << " vs " 
                 << *(next->team2->name) << endl;
            cout << "Enter score for " << *(next->team1->name) << ": ";
            cin >> next->score1;
            cout << "Enter score for " << *(next->team2->name) << ": ";
            cin >> next->score2;
            
            matches.addMatch(next);
            history.addMatch(next);
            updateStandings(next->team1, next->team2, next->score1, next->score2);
            cout << "Match played and recorded successfully!" << endl;
        } else {
            cout << "No scheduled matches to play." << endl;
        }
    }
    
    void undoLastMatch() {
        if (history.isEmpty()) {
            cout << "No matches to undo." << endl;
            return;
        }
        cout << "Undoing last match..." << endl;
        history.undoLastMatch();
        // Note: In a real implementation, we would need to reverse the standings updates
        cout << "Last match undone. Note: Standings may be inconsistent." << endl;
    }
    
    void displayStandings() {
        vector<Team*> teamList = teams.getAllTeams();
        
        // Using different sorting algorithms
        if (teamList.size() < 10) {
            cout << "\nUsing Bubble Sort for small dataset..." << endl;
            Sorter::bubbleSortTeams(teamList);
        } else {
            cout << "\nUsing Quick Sort for large dataset..." << endl;
            Sorter::quickSortTeams(teamList, 0, teamList.size() - 1);
        }
        
        cout << "\nLeague Standings:\n";
        cout << "-------------------------------------------------\n";
        cout << setw(15) << left << "Team" 
             << setw(6) << "Pts" 
             << setw(6) << "GS" 
             << setw(6) << "GC" 
             << setw(6) << "GD" << endl;
        cout << "-------------------------------------------------\n";
        for (auto t : teamList) {
            t->display();
        }
        cout << "-------------------------------------------------\n";
    }
    
    void searchMatches(const string& start, const string& end) {
        vector<Match*> results = matches.getMatchesInRange(start, end);
        cout << "\nMatches between " << start << " and " << end << ":\n";
        cout << "-----------------------------------------\n";
        for (auto m : results) {
            m->display();
        }
        cout << "Total matches: " << results.size() << endl;
    }
    
    void generateReport() {
        vector<Team*> teamList = teams.getAllTeams();
        vector<Match*> allMatches = matches.getMatchesInRange("0000-00-00", "9999-99-99");
        
        cout << "\nFootball League Analysis Report\n";
        cout << "==============================\n";
        cout << "Total Teams: " << teamList.size() << endl;
        cout << "Total Matches Played: " << allMatches.size() << endl;
        
        int totalGoals = 0;
        for (auto m : allMatches) {
            totalGoals += m->score1 + m->score2;
        }
        cout << "Total Goals Scored: " << totalGoals << endl;
        if (!allMatches.empty()) {
            cout << "Average Goals per Match: " << fixed << setprecision(2) 
                 << (double)totalGoals / allMatches.size() << endl;
        }
        
        // Using queue to process teams
        queue<Team*> teamQueue;
        for (auto t : teamList) {
            teamQueue.push(t);
        }
        
        cout << "\nTeam Processing Queue:\n";
        while (!teamQueue.empty()) {
            Team* t = teamQueue.front();
            teamQueue.pop();
            cout << "Processed: " << *(t->name) << endl;
        }
    }
};

void displayMenu() {
    cout << "\nFootball Score Management System\n";
    cout << "1. Add New Team\n";
    cout << "2. Record Match Result\n";
    cout << "3. Schedule Future Match\n";
    cout << "4. Play Scheduled Match\n";
    cout << "5. Display League Standings\n";
    cout << "6. Search Matches by Date Range\n";
    cout << "7. Generate Statistical Report\n";
    cout << "8. Undo Last Match\n";
    cout << "9. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    ScoreManager sm;
    int choice;
    string date, t1, t2, start, end;
    int s1, s2;

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                cout << "Enter team name: ";
                getline(cin, t1);
                sm.addTeam(t1);
                break;
                
            case 2:
                cout << "Enter match date (YYYY-MM-DD): ";
                getline(cin, date);
                cout << "Enter team 1: ";
                getline(cin, t1);
                cout << "Enter team 2: ";
                getline(cin, t2);
                cout << "Enter " << t1 << " score: ";
                cin >> s1;
                cout << "Enter " << t2 << " score: ";
                cin >> s2;
                sm.recordMatch(date, t1, t2, s1, s2);
                break;
                
            case 3:
                cout << "Enter match date (YYYY-MM-DD): ";
                getline(cin, date);
                cout << "Enter team 1: ";
                getline(cin, t1);
                cout << "Enter team 2: ";
                getline(cin, t2);
                sm.scheduleMatch(date, t1, t2);
                break;
                
            case 4:
                sm.playScheduledMatch();
                break;
                
            case 5:
                sm.displayStandings();
                break;
                
            case 6:
                cout << "Enter start date (YYYY-MM-DD): ";
                getline(cin, start);
                cout << "Enter end date (YYYY-MM-DD): ";
                getline(cin, end);
                sm.searchMatches(start, end);
                break;
                
            case 7:
                sm.generateReport();
                break;
                
            case 8:
                sm.undoLastMatch();
                break;
                
            case 9:
                cout << "Exiting system...\n";
                return 0;
                
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
}
