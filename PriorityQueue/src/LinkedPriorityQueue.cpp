// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "LinkedPriorityQueue.h"

LinkedPriorityQueue::LinkedPriorityQueue() {
    LinkedPQfront = NULL;
}

LinkedPriorityQueue::~LinkedPriorityQueue() {
    clear();
    free(LinkedPQfront);
}

void LinkedPriorityQueue::changePriority(string value, int newPriority) {
    ListNode* current = LinkedPQfront;
    while(current != NULL && current->value != value){
        current = current->next;
    }

    if(current == NULL){
        // item not found in list
    }else{
        current->priority = newPriority;
    }
}

void LinkedPriorityQueue::clear() {
    ListNode* current = LinkedPQfront;
    ListNode* next;
    while(current != NULL){
        next = current->next;
        free(current);
        current = next;
    }
    LinkedPQfront = NULL;
}

string LinkedPriorityQueue::dequeue() {
    ListNode* current = LinkedPQfront;
    List* prev = NULL;
    string retVal;
    while(current!=NULL){

    }
    return "";   // remove this
}

void LinkedPriorityQueue::enqueue(string value, int priority) {
    // TODO: implement

}

bool LinkedPriorityQueue::isEmpty() const {
    // TODO: implement
    return false;   // remove this
}

string LinkedPriorityQueue::peek() const {
    // TODO: implement
    return "";   // remove this
}

int LinkedPriorityQueue::peekPriority() const {
    // TODO: implement
    return 0;   // remove this
}

int LinkedPriorityQueue::size() const {
    // TODO: implement
    return 0;   // remove this
}

ostream& operator<<(ostream& out, const LinkedPriorityQueue& queue) {
    // TODO: implement
    return out;
}
