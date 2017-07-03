// This is a .cpp file you will edit and turn in.
// We have provided a skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include "ArrayPriorityQueue.h"
#define PQINCRSIZE 100
ArrayPriorityQueue::ArrayPriorityQueue() {
    // create an array of size capacity, ssign it to the pointer
    ArrayPQcapacity = PQINCRSIZE;
    ArrayPQarr = new PQEntry[ArrayPQcapacity];
}

ArrayPriorityQueue::~ArrayPriorityQueue() {
    delete [] ArrayPQarr;
}
/*
 * In this function you will modify the priority of a given existing value in the queue.
 * The intent is to change the value's priority to be more urgent (smaller integer) than its current value.
 * If the given value is present in the queue and already has a more urgent priority to the given new priority,
 * or if the given value is not already in the queue, your function should throw a string exception.
 * If the given value occurs multiple times in the priority queue, you should alter the priority of the first occurrence you find
 * when searching your internal data from the start.
*/
void ArrayPriorityQueue::changePriority(string value, int newPriority) {
    bool elementPresent = false;
    for(int i=0; i<ArrayPQsize; i++){
        if(ArrayPQarr[i].value == value){
            ArrayPQarr[i].priority = newPriority;
            elementPresent = true;
            break;
        }
    }
    if(!elementPresent){
        // TODO: throw string exception
    }

}
/*
 * In this function you should remove all elements from the priority queue.
*/
void ArrayPriorityQueue::clear() {
    ArrayPQsize = 0;
}
/*
 * Search array for value w/ greatest priority, remove the element with the most urgent priority from your priority queue
*/
string ArrayPriorityQueue::dequeue() {
    // TODO: implement
    return "";   // remove this
}
/*
 * Add elements to end of array
 * what do we do when we run out of space (see vector class ex.
 * add the given string value into your priority queue with the given priority. Duplicates are allowed.
 * Any string is a legal value, and any integer is a legal priority; there are no invalid values that can be passed.
 */
void ArrayPriorityQueue::enqueue(string value, int priority) {
    // TODO: implement

}

bool ArrayPriorityQueue::isEmpty() const {

    return (ArrayPQsize == 0);
}
/*
 * return the string element with the most urgent priority from your priority queue
*/
string ArrayPriorityQueue::peek() const {
    // TODO: implement
    return "";   // remove this
}

int ArrayPriorityQueue::peekPriority() const {
    // TODO: implement
    return 0;   // remove this
}

int ArrayPriorityQueue::size() const {
    return ArrayPQcapacity;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    // TODO: implement
    return out;
}
