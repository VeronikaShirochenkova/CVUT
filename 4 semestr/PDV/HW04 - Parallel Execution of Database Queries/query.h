
#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda pro VSECHNY dilci dotazy (obsazene ve
    // vektoru 'predicates') existuje alespon jeden zaznam v tabulce (reprezentovane vektorem
    // 'data_table'), pro ktery je dany predikat splneny.

    // Pro inspiraci si prostudujte kod, kterym muzete zjistit, zda prvni dilci dotaz plati,
    // tj., zda existuje alespon jeden radek v tabulce, pro ktery predikat reprezentovany
    // funkci predicates[i] plati:

//    throw "Not implemented yet";

//    auto &first_predicate = predicates[0];        // Funkce reprezentujici predikat prvniho poddotazu
//
//    unsigned int row_count = data_table.size();    // Kazdy radek tabulky je reprezentovany jednim prvkem
//                                                   // vektoru 'data_table'. Velikost vektoru tedy odpovida
//                                                   // poctu zaznamu
//
//    for(unsigned int i = 0 ; i < row_count ; i++) {
//        auto & row = data_table[i];                // i-ty radek tabulky ...
//        bool is_satisfied = first_predicate(row);  // ... splnuje prvni predikat, pokud funkce first_predicate
//                                                   //     vrati true
//
//        if(is_satisfied) {
//            // Nalezli jsme radek, pro ktery je predikat splneny.
//            // Dilci poddotaz tedy plati
//
//            return true;
//        } else {
//            // V opacnem pripade hledame dal - stale muzeme najit radek, pro ktery predikat plati
//        }
//    }


    unsigned int row_count = data_table.size();
    unsigned int predicates_count = predicates.size();
    std::atomic<bool> res {true};

#pragma omp parallel for
    for(unsigned int j = 0; j < predicates_count; j++) {
        bool plati = false;
        auto & predicate = predicates[j];
        for (unsigned int i = 0; i < row_count; i++) {
            auto & row = data_table[i];
            bool is_satisfied = predicate(row);

            if (is_satisfied) {
                plati = true;
                break;
            }
        }
        if (!plati) {
            res.store(false);
#pragma omp cancel for
        }
    }

    return res.load();
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda je ALESPON JEDEN dilci dotaz pravdivy.
    // To znamena, ze mate zjistit, zda existuje alespon jeden predikat 'p' a jeden zaznam
    // v tabulce 'r' takovy, ze p(r) vraci true.

    // Zamyslete se nad tim, pro ktery druh dotazu je vhodny jaky druh paralelizace. Vas
    // kod optimalizujte na situaci, kdy si myslite, ze navratova hodnota funkce bude true.
    // Je pro Vas dulezitejsi rychle najit splnujici radek pro jeden vybrany predikat, nebo
    // je dulezitejsi zkouset najit takovy radek pro vice predikatu paralelne?

    //throw "Not implemented yet";

    unsigned int row_count = data_table.size();
    unsigned int predicates_count = predicates.size();
    bool res = false;

#pragma omp parallel for
    for (unsigned int i = 0; i < row_count; i++){
        auto & row = data_table[i];
        for (unsigned int j = 0; j < predicates_count; j++) {
            auto & predicate = predicates[j];
            bool is_satisfied = predicate(row);

            if (is_satisfied) {
                res = true;
                break;
            }
        }
        if (res) {
#pragma omp cancel for
        }
    }

    return res;
}


#endif //DATABASEQUERIES_QUERY_H
