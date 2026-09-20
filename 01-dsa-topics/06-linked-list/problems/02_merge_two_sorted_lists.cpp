// Problem: Merge Two Sorted Lists (LeetCode #21)
// Difficulty: Easy
// Approach: Dummy head + tail pointer. Pick the smaller of the two heads,
//           advance that pointer, append to tail.
// Time:  O(n + m), Space: O(1)

#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (l1 && l2) {
        if (l1->val <= l2->val) { tail->next = l1; l1 = l1->next; }
        else                    { tail->next = l2; l2 = l2->next; }
        tail = tail->next;
    }
    tail->next = l1 ? l1 : l2;
    return dummy.next;
}

void printList(ListNode* head) {
    while (head) { cout << head->val << " "; head = head->next; }
    cout << endl;
}

int main() {
    // 1->2->4, 1->3->4
    ListNode* l1 = new ListNode(1); l1->next = new ListNode(2); l1->next->next = new ListNode(4);
    ListNode* l2 = new ListNode(1); l2->next = new ListNode(3); l2->next->next = new ListNode(4);
    printList(mergeTwoLists(l1, l2)); // 1 1 2 3 4 4
    return 0;
}
