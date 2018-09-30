typedef struct Node {
    int data;
    struct Node *next;
} Node;

// #include <stdlib.h>
// #include <time.h>
//
// Node* create(int len) {
//     Node dummy = {.next = NULL}, *h = &dummy;

//     while(len--) {
//         Node *p = malloc(sizeof(*p));
//         p->data = rand();
//         h->next = p;
//         h = p;
//     }

//     return dummy.next;
// }

#include <stddef.h>

Node* insertionSort(Node *head) {
    if(!head || !head->next) return head;

    Node *p = head->next;
    head->next = NULL;
    while(p) {
        Node *next = p->next;
        if(p->data < head->data) {
            p->next = head;
            head = p;
        } else {
            Node *tmp = head;
            while(tmp->next && tmp->next->data < p->data)
                tmp = tmp->next;
            p->next = tmp->next;
            tmp->next = p;
        }
        p = next;
    }
    return head;
}

// merge sort helper
Node* merge(Node *h1, Node *h2) {
    Node dummy, *p = &dummy;
    while(h1 && h2) {
        if(h1->data < h2->data) {
            p->next = h1;
            h1 = h1->next;
        } else {
            p->next = h2;
            h2 = h2->next;
        }
        p = p->next;
    }
    p->next = h1 ? h1 : h2;
    return dummy.next;
}

Node* mergeSort(Node *head) {
    if(!head || !head->next) return head;

    if(!head->next->next) {
        Node *p = head->next;
        if(p->data < head->data) {
            p->next = head;
            head->next = NULL;
            head = p;
        }
        return head;
    }

    Node *slow = head, *fast = head->next->next;
    while(fast) {
        slow = slow->next;
        fast = fast->next;
        if(fast) fast = fast->next;
    }
    // reuse fast
    fast = mergeSort(slow->next);
    slow->next = NULL;
    return merge(mergeSort(head), fast);
}
