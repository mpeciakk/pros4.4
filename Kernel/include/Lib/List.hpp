#ifndef PROS_LIST_HPP
#define PROS_LIST_HPP

#include <MM/kmalloc.hpp>

struct ListNode {
    struct ListNode* next;
    void* value;
};

class List {
public:
    ListNode* head = nullptr;

    ListNode* insert(void* value) {
        auto* node = new ListNode();
        node->next = head;
        node->value = value;

        if (head == nullptr) {
            head = node;
        } else {
            node->next = head;
            head = node;
        }

        return node;
    }
};

#endif