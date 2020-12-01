#include <iostream>
#include <vector>
#include <memory>
#include <sstream>
#include "minisat/core/SolverTypes.h"
#include "minisat/core/Solver.h"

using namespace Minisat;
using namespace std;


void VerCov(vector<vector<int>> E, int v);
void IPData(string &IP, int &v);

void VerCov(vector<vector<int>> E, int v) {
    if (E.empty())
        return;

    for (int t = 1; t < v; ++t) {
        unique_ptr<Solver> solver(new Solver());
        vector<vector<Lit>> lit(v);


        for (int l = 0; l < v; ++l)
            for (int m = 0; m < t; ++m) {
                Lit k = mkLit(solver->newVar());
                lit[l].push_back(k);
            }

        for (int l = 0; l < t; ++l) {
            vec<Lit> ps;
            for (int m = 0; m < v; ++m) {
                ps.push(lit[m][l]);
            }
            solver->addClause(ps);
        }

        for (int l = 0; l < v; ++l)
            for (int m = 0; m < t - 1; ++m)
                for (int n = m + 1; n < t; ++n)
                    solver->addClause(~lit[l][m], ~lit[l][n]);

        for (int l = 0; l < t; ++l)
            for (int m = 0; m < v - 1; ++m)
                for (int n = m + 1; n < v; ++n)
                    solver->addClause(~lit[m][l], ~lit[n][l]);

        for (unsigned l = 0; l < E.size(); ++l) {
            vec<Lit> ps;
            for (int m = 0; m < t; ++m) {
                ps.push(lit[E[l][0]][m]);
                ps.push(lit[E[l][1]][m]);
            }
            solver->addClause(ps);
        }

        if (solver->solve()) {
            vector<int> res;
            for (int l = 0; l < v; ++l)
                for (int m = 0; m < t; ++m)
                    if (solver->modelValue(lit[l][m]) == l_True)
                        res.push_back(l);
            for (unsigned l = 0; l < res.size(); ++l) {
                if (l == 0)
                    cout << res[l];
                else
                    cout << " " << res[l];
            }
            cout << endl;
            solver.reset(new Solver());
            return;
        }
        solver.reset(new Solver());
    }
}

void IPData(string &IP, int &v) {
    stringstream input(IP);
    char CheckCommand;
    input >> CheckCommand;

    if (input.fail())
        return;

    if (CheckCommand == 'V') {
        input >> v;
    } else if (CheckCommand == 'E') {
        vector<vector<int>> E;
        while (!input.eof()) {
            char EdgeSign;
            input >> EdgeSign;
            if (EdgeSign == '<') {
                int V1, V2;
                char comma;
                input >> V1 >> comma >> V2;
                E.push_back({V1, V2});
            }
        }
        VerCov(E, v);
    }
}


int main() {
    int v;
    while (!cin.eof()) {
        string IP;
        getline(cin, IP);
        IPData(IP, v);
    }
    return 0;
}


