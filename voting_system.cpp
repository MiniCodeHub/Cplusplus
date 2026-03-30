#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <stdexcept>
#include <set>
#include <numeric>
using namespace std;

// -- Candidate -------------------------------------------------------------
struct Candidate {
    int    id;
    string name;
    string party;
    int    votes;
    Candidate(int i, const string& n, const string& p) : id(i), name(n), party(p), votes(0) {}
};

// -- VotingSystem ----------------------------------------------------------
class VotingSystem {
private:
    vector<Candidate>       candidates;
    set<string>             voterRegistry;    // registered voter IDs
    set<string>             votedVoters;      // who has already voted
    unordered_map<string,int> voteLog;        // voterId -> candidateId
    bool                    pollsOpen;
    string                  electionName;
    int                     nextCandId;

public:
    explicit VotingSystem(const string& name)
        : pollsOpen(false), electionName(name), nextCandId(1) {}

    // -- Setup --
    void registerCandidate(const string& name, const string& party) {
        if (pollsOpen) throw runtime_error("Cannot register candidates after polls open.");
        candidates.emplace_back(nextCandId++, name, party);
        cout << "[+] Candidate registered: " << name << " (" << party << ")\n";
    }

    void registerVoter(const string& voterId) {
        if (voterRegistry.count(voterId))
            throw invalid_argument("Voter already registered: " + voterId);
        voterRegistry.insert(voterId);
    }

    void registerVoters(const vector<string>& ids) {
        for (const auto& id : ids) registerVoter(id);
        cout << "[+] Registered " << ids.size() << " voters. Total: " << voterRegistry.size() << "\n";
    }

    void openPolls() {
        if (candidates.empty()) throw runtime_error("No candidates registered.");
        if (voterRegistry.empty()) throw runtime_error("No voters registered.");
        pollsOpen = true;
        cout << "\n[OK] Polls are now OPEN for \"" << electionName << "\"\n";
    }

    void closePolls() {
        pollsOpen = false;
        cout << "\n[OK] Polls are now CLOSED.\n";
    }

    // -- Voting --
    void castVote(const string& voterId, int candidateId) {
        if (!pollsOpen)
            throw runtime_error("Polls are closed.");
        if (!voterRegistry.count(voterId))
            throw invalid_argument("Voter not registered: " + voterId);
        if (votedVoters.count(voterId))
            throw runtime_error("Voter has already voted: " + voterId);
        Candidate* c = findCandidate(candidateId);
        if (!c) throw invalid_argument("Candidate ID not found: " + to_string(candidateId));

        c->votes++;
        votedVoters.insert(voterId);
        voteLog[voterId] = candidateId;
    }

    // -- Query --
    Candidate* findCandidate(int id) {
        for (auto& c : candidates) if (c.id == id) return &c;
        return nullptr;
    }

    int totalVotes() const {
        return accumulate(candidates.begin(), candidates.end(), 0,
            [](int s, const Candidate& c){ return s + c.votes; });
    }

    // -- Results --
    void printCandidates() const {
        cout << "\n-- Registered Candidates --\n";
        cout << left << setw(4) << "ID" << setw(22) << "Name" << setw(20) << "Party" << "\n";
        cout << string(46, '-') << "\n";
        for (const auto& c : candidates)
            cout << setw(4) << c.id << setw(22) << c.name << setw(20) << c.party << "\n";
    }

    void printTally() const {
        int total = totalVotes();
        int reg   = (int)voterRegistry.size();
        cout << "\n" << string(60, '=') << "\n";
        cout << "  ELECTION RESULTS: " << electionName << "\n";
        cout << string(60, '=') << "\n\n";

        cout << left << setw(4) << "#" << setw(22) << "Candidate"
             << setw(18) << "Party" << right << setw(7) << "Votes"
             << setw(9) << "Share" << "  Bar\n";
        cout << string(68, '-') << "\n";

        // Sort by votes descending
        vector<const Candidate*> sorted;
        for (const auto& c : candidates) sorted.push_back(&c);
        sort(sorted.begin(), sorted.end(), [](const Candidate* a, const Candidate* b){
            return a->votes > b->votes;
        });

        int rank = 1;
        int maxVotes = sorted.empty() ? 1 : sorted[0]->votes;
        for (const auto* c : sorted) {
            double pct = total > 0 ? (double)c->votes / total * 100 : 0;
            int    barLen = maxVotes > 0 ? (int)((double)c->votes / maxVotes * 20) : 0;

            cout << left << setw(4) << rank++
                 << setw(22) << c->name
                 << setw(18) << c->party
                 << right << setw(7) << c->votes
                 << setw(8) << fixed << setprecision(1) << pct << "%"
                 << "  [" << string(barLen, '#') << string(20 - barLen, '.') << "]\n";
        }

        cout << string(68, '-') << "\n";
        cout << right << setw(46) << "Total votes: " << total << "\n\n";

        // Winner
        if (!sorted.empty() && total > 0) {
            const Candidate* winner = sorted[0];
            bool tie = sorted.size() > 1 && sorted[1]->votes == winner->votes;
            if (tie) {
                cout << "  *** RESULT: TIE between " << sorted[0]->name
                     << " and " << sorted[1]->name << " ***\n";
            } else {
                cout << "  *** WINNER: " << winner->name
                     << " (" << winner->party << ") with "
                     << winner->votes << " votes ("
                     << fixed << setprecision(1) << (double)winner->votes/total*100
                     << "%) ***\n";
            }
        }

        // Participation
        cout << "\n  Registered voters : " << reg << "\n";
        cout << "  Votes cast        : " << total << "\n";
        cout << "  Voter turnout     : "
             << (reg > 0 ? (double)total/reg*100 : 0.0)
             << "%\n";
        cout << "  Abstentions       : " << reg - total << "\n";
        cout << string(60, '=') << "\n";
    }

    // -- Party aggregation --
    void printPartyResults() const {
        unordered_map<string, int> partyVotes;
        for (const auto& c : candidates) partyVotes[c.party] += c.votes;
        int total = totalVotes();

        cout << "\n-- Party Results --\n";
        cout << left << setw(22) << "Party" << right << setw(8) << "Votes" << setw(10) << "Share\n";
        cout << string(40, '-') << "\n";

        vector<pair<string,int>> pv(partyVotes.begin(), partyVotes.end());
        sort(pv.begin(), pv.end(), [](auto& a, auto& b){ return a.second > b.second; });
        for (auto& [party, votes] : pv) {
            double pct = total > 0 ? (double)votes/total*100 : 0;
            cout << left << setw(22) << party
                 << right << setw(8) << votes
                 << setw(9) << fixed << setprecision(1) << pct << "%\n";
        }
    }

    // -- Invalid vote attempts --
    int rejectedVotes = 0;
    void tryVote(const string& voter, int candidateId, bool expect_fail = false) {
        try {
            castVote(voter, candidateId);
            if (!expect_fail) cout << "  Vote cast: " << voter << " -> Candidate #" << candidateId << "\n";
        } catch (const exception& e) {
            rejectedVotes++;
            cout << "  [Rejected] " << voter << ": " << e.what() << "\n";
        }
    }
};

int main() {
    cout << "=== Mini Voting System ===\n\n";

    VotingSystem vs("2025 City Council Election");

    // -- Register candidates --
    cout << "-- Registering Candidates --\n";
    vs.registerCandidate("Alice Mercer",   "Progressive Party");
    vs.registerCandidate("Bob Harrington", "Conservative Party");
    vs.registerCandidate("Carol Singh",    "Green Alliance");
    vs.registerCandidate("Dan Okafor",     "Progressive Party");
    vs.printCandidates();

    // -- Register voters --
    cout << "\n-- Registering Voters --\n";
    vs.registerVoters({"V001","V002","V003","V004","V005","V006","V007","V008","V009","V010",
                        "V011","V012","V013","V014","V015","V016","V017","V018","V019","V020"});

    // -- Open polls --
    vs.openPolls();

    // -- Cast votes --
    cout << "\n-- Casting Votes --\n";
    vs.tryVote("V001", 1); vs.tryVote("V002", 1); vs.tryVote("V003", 2);
    vs.tryVote("V004", 3); vs.tryVote("V005", 1); vs.tryVote("V006", 2);
    vs.tryVote("V007", 1); vs.tryVote("V008", 3); vs.tryVote("V009", 4);
    vs.tryVote("V010", 1); vs.tryVote("V011", 2); vs.tryVote("V012", 1);
    vs.tryVote("V013", 3); vs.tryVote("V014", 4); vs.tryVote("V015", 1);
    vs.tryVote("V016", 2); vs.tryVote("V017", 1);
    // V018, V019, V020 abstain

    // -- Error cases --
    cout << "\n-- Error Handling --\n";
    vs.tryVote("V001",   1, true); // double vote
    vs.tryVote("V999",   2, true); // unregistered
    vs.tryVote("V002",  99, true); // invalid candidate

    // -- Close polls --
    vs.closePolls();

    // -- Vote after close --
    cout << "\n-- Vote after polls close --\n";
    vs.tryVote("V018", 1, true);

    // -- Results --
    vs.printTally();
    vs.printPartyResults();

    return 0;
}