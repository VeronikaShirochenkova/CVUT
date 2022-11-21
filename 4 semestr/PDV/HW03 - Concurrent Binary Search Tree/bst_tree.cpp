#include <functional>
#include "bst_tree.h"
#include <atomic>

void bst_tree::insert(long long data) {
    node * new_node = new node(data);
    node * current_node = nullptr;
    node * null_pointer = nullptr;

    if (!root.compare_exchange_strong(current_node, new_node)) {
        while(true) {
            null_pointer = nullptr;

            // left
            if (data < current_node->data) {
                if (current_node->left == null_pointer) {
                    if (current_node->left.compare_exchange_strong(null_pointer, new_node)) { break; }
                }
                else {
                    current_node = current_node->left;
                }
            }
            // right
            else {
                if (current_node->right == null_pointer) {
                    if (current_node->right.compare_exchange_strong(null_pointer, new_node)) { break; }
                }
                else {
                    current_node = current_node->right;

                }
            }
        }
    }

}

bst_tree::~bst_tree() {
    // Rekurzivni funkce pro pruchod stromu a dealokaci pameti prirazene jednotlivym
    // uzlum
    std::function<void(node*)> cleanup = [&](node * n) {
        if(n != nullptr) {
            cleanup(n->left);
            cleanup(n->right);

            delete n;
        }
    };
    cleanup(root);
}
