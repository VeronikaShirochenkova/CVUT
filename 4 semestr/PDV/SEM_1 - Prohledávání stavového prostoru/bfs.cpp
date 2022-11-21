#include "bfs.h"
#include <queue>
#include <atomic>
#include <utility>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

// Naimplementujte efektivni algoritmus pro nalezeni nejkratsi cesty v grafu.
// V teto metode nemusite prilis optimalizovat pametove naroky, a vhodnym algo-
// ritmem tak muze byt napriklad pouziti prohledavani do sirky (breadth-first
// search.
//
// Metoda ma za ukol vratit ukazatel na cilovy stav, ktery je dosazitelny pomoci
// nejkratsi cesty.
std::shared_ptr<const state> bfs(std::shared_ptr<const state> root) {

    std::vector<std::shared_ptr<const state>> opened_states;
    std::vector<std::shared_ptr<const state>> solution_states;
    std::unordered_set<unsigned long long> visited_states;
    //std::shared_ptr<const state> solution;
    bool finish = false;

    // check if root is solution
    if (root->is_goal()) {
        return root;
    }

    // check childrens of root
    std::vector<std::shared_ptr<const state>> next_states = root->next_states();
    auto next_states_size = next_states.size();
    for (auto i = 0; i < next_states_size; ++i) {
        if (next_states[i]->is_goal()) {
            return next_states[i];
        }
        opened_states.push_back(next_states[i]);
    }

    // MAIN LOOP
    while (!opened_states.empty() && !finish) {

        auto size = opened_states.size();
        std::vector<std::shared_ptr<const state>> tmp;

#pragma omp parallel for
        for (auto i = 0; i < size; i++) {
            std::vector<std::shared_ptr<const state>> neighbors = opened_states[i]->next_states();

            for (auto j = 0; j < neighbors.size(); j++) {
                if (visited_states.find(neighbors[j]->get_identifier()) == visited_states.end()) {
#pragma omp critical
                    {

                        if (neighbors[j]->is_goal()) {
                            finish = true;
                            solution_states.push_back(neighbors[j]);
                        }

                        visited_states.insert(neighbors[j]->get_identifier());
                        tmp.push_back(neighbors[j]);

                    }
                }
            }
        }
        opened_states = tmp;
    }
    //std::cout << opened_states.size() << " " << finish << std::endl;

    std::shared_ptr<const state> best_solution = solution_states[0];


    for (auto i = 0; i < solution_states.size(); ++i) {
        //std::cout << solution_states[i]->current_cost() << solution_states[i]->get_identifier() << std::endl;
        if (best_solution->get_identifier() > solution_states[i]->get_identifier()) {
            best_solution = solution_states[i];
        }
    }

    return best_solution;
}