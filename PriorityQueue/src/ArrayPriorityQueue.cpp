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
        // Q: memory leak?
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
    if(isEmpty()){
        // TODO: suppress stacktrace
        throw string("Error: priority queue is empty");
        return "";
    }else{
        int indexToRemove = 0;
        string retVal="";
        for(int i=1; i<ArrayPQsize; i++){
            if(ArrayPQarr[i] < ArrayPQarr[indexToRemove]){
                indexToRemove = i;
            }
        }

        retVal = ArrayPQarr[indexToRemove].value;

        for(int i = indexToRemove; i<ArrayPQsize-1; i++){
            ArrayPQarr[i] = ArrayPQarr[i+1];
        }
        ArrayPQsize--;

        return retVal;
    }

}
/*
 * Add elements to end of array
 * what do we do when we run out of space (see vector class ex.
 * add the given string value into your priority queue with the given priority. Duplicates are allowed.
 * Any string is a legal value, and any integer is a legal priority; there are no invalid values that can be passed.
 */
void ArrayPriorityQueue::enqueue(string value, int priority) {
    // TODO: check for overflow here, resize if reqd
    ArrayPQarr[ArrayPQsize].priority = priority;
    ArrayPQarr[ArrayPQsize].value = value;
    ArrayPQsize++;
}

bool ArrayPriorityQueue::isEmpty() const {
    return (ArrayPQsize == 0);
}
/*
 * return the string element with the most urgent priority from your priority queue, without removing it or altering the state of the queue.
 * You should throw a string exception if the queue does not contain any elements.
*/
string ArrayPriorityQueue::peek() const {
    if(isEmpty()){
        throw new string("Error: priority queue is empty");
    }
    else{
        int indexToRemove = 0;
        for(int i=1; i<ArrayPQsize; i++){
            if(ArrayPQarr[i] < ArrayPQarr[indexToRemove]){
                indexToRemove = i;
            }
        }
        return ArrayPQarr[indexToRemove].value;
    }
}
/*
 * return the integer priority that is most urgent from your priority queue (the priority associated with the string that would be
 * returned by a call to peek),
 * without removing it or altering the state of the queue.
 * You should throw a string exception if the queue does not contain any elements.
*/
int ArrayPriorityQueue::peekPriority() const {
    int indexToRemove = 0;
    for(int i=1; i<ArrayPQsize; i++){
        if(ArrayPQarr[i] < ArrayPQarr[indexToRemove]){
            indexToRemove = i;
        }
    }
    return ArrayPQarr[indexToRemove].priority;
}

int ArrayPriorityQueue::size() const {
    return ArrayPQcapacity;
}

int ArrayPriorityQueue::len() const {
    return ArrayPQsize;
}

string ArrayPriorityQueue::stringifyArrPQ() const{
    string str = "{";
    for(int i=0; i<ArrayPQsize; i++){
        str += "\"" + ArrayPQarr[i].value + "\":" + to_string(ArrayPQarr[i].priority) + ", ";
    }
    // remove last ", "
    int len = str.length();
    if(len != 1)
        str = str.substr (0,len-2);
    str += "}";
    return str;
}

ostream& operator<<(ostream& out, const ArrayPriorityQueue& queue) {
    // TODO: print to OUT and not COUT
    cout<<queue.stringifyArrPQ();
    return out;
}
