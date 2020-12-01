#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <time.h>
#include <algorithm>
#include <pthread.h>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include "minisat/core/Solver.h"
#include "minisat/core/SolverTypes.h"

using namespace std;
using namespace Minisat;

void *GraphInit(void *arg);

void *cnfSat(void *arg);

void *approxVC1(void *arg);

void *approxVC2(void *arg);

struct Graph {
    unsigned ver;
    vector <vector<unsigned>> ed;
    vector<unsigned> cnfsat;
    vector<unsigned> approxVC1;
    vector<unsigned> approxVC2;
};

void *threadInputOutput(void *arg) {
    auto *G = (Graph *) arg;

    while (!cin.eof()) {
        string BufferInput;
        getline(cin, BufferInput);
        stringstream input(BufferInput);

        char Buffer_Input = '0';
        input >> Buffer_Input;
        if (Buffer_Input == 'V') {
            input >> G->ver;
        } else if (Buffer_Input == 'E') {
            while (!input.eof()) {
                char symbol;
                input >> symbol;
                if (symbol == '<') {
                    unsigned n1, n2;
                    char comma;
                    input >> n1 >> comma >> n2;
                    G->ed.push_back({n1, n2});
                }
            }

            pthread_t thread_algorithm1, thread_algorithm2, thread_algorithm3;

            pthread_create(&thread_algorithm1, nullptr, &cnfSat, G);
            pthread_create(&thread_algorithm2, nullptr, &approxVC1, G);
            pthread_create(&thread_algorithm3, nullptr, &approxVC2, G);

            pthread_join(thread_algorithm1, nullptr);
            pthread_join(thread_algorithm2, nullptr);
            pthread_join(thread_algorithm3, nullptr);

            cout << "CNF-SAT-VC: ";

            unsigned p = 0;
            while (p < G->cnfsat.size()) {
                if (p != 0) {
                    cout << "," << G->cnfsat[p];
                } else {
                    cout << G->cnfsat[p];
                }
                ++p;
            }

            cout << endl;
            G->cnfsat.clear();

            cout << "APPROX-VC-1: ";
            p = 0;
            while (p < G->approxVC1.size()) {
                if (p != 0) {
                    cout << "," << G->approxVC1[p];
                } else {
                    cout << G->approxVC1[p];
                }
                ++p;
            }
            cout << endl;
            G->approxVC1.clear();

            cout << "APPROX-VC-2: ";
            p = 0;
            while (p < G->approxVC2.size()) {
                if (p != 0) {
                    cout << "," << G->approxVC2[p];
                } else {
                    cout << G->approxVC2[p];
                }
                ++p;
            }
            cout << endl;
            G->approxVC2.clear();
            G->ed.clear();
        }
    }
    return nullptr;
}

void *approxVC1(void *arg) {
    auto *G = (Graph *) arg;

    if (G->ed.empty()) {
        return nullptr;
    } else {
        vector<bool> visit(G->ed.size(), false);

        while (true) {
            vector<unsigned> count(G->ver, 0);
            unsigned p = 0;
            while (p < G->ed.size()) {
                if (visit[p] == false) {
                    count[G->ed[p][0]]++;
                    count[G->ed[p][1]]++;
                }
                ++p;
            }
            unsigned verMax = distance(count.begin(), max_element(count.begin(), count.end()));
            G->approxVC1.push_back(verMax);

            p = 0;
            while (p < G->ed.size()) {
                if (G->ed[p][0] == verMax or G->ed[p][1] == verMax) {
                    visit[p] = true;
                }
                ++p;
            }
            if (find(visit.begin(), visit.end(), false) == visit.end()) {
                sort(G->approxVC1.begin(), G->approxVC1.end());
                return nullptr;
            }
        }
    }


}

void *approxVC2(void *arg) {
    auto *G = (Graph *) arg;

    vector <vector<unsigned>> tmporaryE = G->ed;
    ifstream urandom("/dev/urandom");
    while (!tmporaryE.empty()) {
        int num = 21;
        urandom.read((char *) &num, sizeof(int));
        unsigned index = abs(num) % (tmporaryE.size());
        unsigned y = tmporaryE[index][0], z = tmporaryE[index][1];
        G->approxVC2.push_back(y);
        G->approxVC2.push_back(z);

        unsigned p = 0;
        while (p < tmporaryE.size()) {
            if (tmporaryE[p][0] == y or tmporaryE[p][0] == z or tmporaryE[p][1] == y or tmporaryE[p][1] == z) {
                tmporaryE.erase(tmporaryE.begin() + p);
            } else {
                ++p;
            }
        }
    }
    sort(G->approxVC2.begin(), G->approxVC2.end());
    return nullptr;
}

void *cnfSat(void *arg) {
    auto *G = (Graph *) arg;

    if (G->ed.empty())
        return nullptr;

    unsigned h = 1;
    while (h < G->ver) {

        unique_ptr <Solver> solver(new Solver());
        vector <vector<Lit>> lit(G->ver);
        unsigned p = 0;
        while (p < G->ver) {

            unsigned q = 0;
            while (q < h) {

                Lit l = mkLit(solver->newVar());
                lit[p].push_back(l);
                ++q;
            }
            ++p;
        }

        p = 0;
        while (p < h) {
            vec <Lit> ps;
            unsigned q = 0;
            while (q < G->ver) {
                ps.push(lit[q][p]);
                ++q;
            }
            solver->addClause(ps);
            ++p;
        }
        p = 0;
        while (p < G->ver) {
            unsigned q = 0;
            while (q < h - 1) {
                unsigned r = q + 1;
                while (r < h) {
                    solver->addClause(~lit[p][q], ~lit[p][r]);
                    ++r;
                }
                ++q;
            }
            ++p;
        }
        p = 0;
        while (p < h) {
            unsigned q = 0;
            while (q < G->ver - 1) {
                unsigned r = q + 1;
                while (r < G->ver) {
                    solver->addClause(~lit[q][p], ~lit[r][p]);
                    ++r;
                }
                ++q;
            }
            ++p;
        }
        p = 0;
        while (p < G->ed.size()) {
            vec <Lit> ps;
            unsigned q = 0;
            while (q < h) {
                ps.push(lit[G->ed[p][0]][q]);
                ps.push(lit[G->ed[p][1]][q]);
                ++q;
            }
            solver->addClause(ps);
            ++p;
        }

        if (solver->solve()) {
            unsigned p = 0;
            while (p < G->ver) {
                unsigned q = 0;
                while (q < h) {
                    if (solver->modelValue(lit[p][q]) == l_True)
                        G->cnfsat.push_back(p);
                    ++q;
                }
                ++p;
            }
            solver.reset(new Solver());
            return nullptr;
        }
        solver.reset(new Solver());
        ++h;
    }
    return nullptr;
}

int main() {
    Graph G;
    pthread_t thread_in_out;
    pthread_create(&thread_in_out, nullptr, &threadInputOutput, &G);
    pthread_join(thread_in_out, nullptr);
    return 0;
}
