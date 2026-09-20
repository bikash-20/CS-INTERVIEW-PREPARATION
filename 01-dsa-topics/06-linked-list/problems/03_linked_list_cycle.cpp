// Problem: Linked List Cycle (LeetCode #141)
// Difficulty: Easy
// Approach: Floyd's cycle-finding — slow moves 1 step, fast moves 2 steps.
//           If they ever meet, there's a cycle. If fast hits null, no cycle.
// Time:  O(n), Space: O(1)

#include <iostream>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int x) : val(x), next(nullptr) {}
};

bool hasCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast) return true;
    }
    return false;
}

int main() {
    // Build 3 -> 2 -> 0 -> -4 -> (back to 2, cycle)
    ListNode* n1 = new ListNode(3);
    ListNode* n2 = new ListNode(2);
    ListNode* n3 = new ListNode(0);
    ListNode* n4 = new ListNode(-4);
    n1->next = n2; n2->next = n3; n3->next = n4; n4->next = n2;

    cout << (hasCycle(n1) ? "Cycle" : "No cycle") << endl;
    return 0;
}
