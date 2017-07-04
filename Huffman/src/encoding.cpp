// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
// TODO: include any other headers you need

////////////// Priority queue implementation starts


/////
///
// #ifndef _pqueue_
// #define _pqueue_

#include "vector.h"
#include <string>


template <typename Type>
class PQueue {
public:

/**
 * Constructor: PQueue
 * Usage: PQueue<int> pq;
 * ----------------------
 * The constructor initializes a new priority queue containing
 * elements of the specified type.
 */
    PQueue();

/**
 * Destructor: ~PQueue
 * Usage: (usually implicit)
 * -------------------------
 * The destructor deallocates storage associated with this queue.
 */
    ~PQueue();

/**
 * Method: size
 * Usage: nElems = pq.size();
 * --------------------------
 * Returns the number of elements in this priority queue.
 */
    int size() const;

/**
 * Method: isEmpty
 * Usage: if (pq.isEmpty()) . . .
 * ------------------------------
 * Returns true if this priority queue contains no elements,
 * and false otherwise.
 */
    bool isEmpty() const;

/**
 * Method: enqueue
 * Usage: pq.enqueue(element);
 * ---------------------------------
 * This method adds element to the priority queue, using operator<
 * to decide which elements have higher priority.  Rather than relying
 * on operator< to determine priority, the interface just asks that the
 * client supply the priority.
 */
    void enqueue(const Type& elem, double priority);

/**
 * Method: extractMin
 * Usage: first = pq.extractMin();
 * ----------------------------
 * This method removes the highest-priority element from this queue
 * and returns it.
 */
    Type extractMin();

/**
 * Method: peek
 * Usage: first = pq.peek();
 * -------------------------
 * This method returns the value of highest-priority element in this
 * queue, without removing it.
 */
    const Type& peek() const;

/**
 * Method: getHead
 * Usage: first = pq.getHead();
 * -------------------------
 * This method returns the pointer to highest-priority element in this
 * queue, without removing it.
 */
    const Type* getHead() const;

private:
    struct entry {
        Type elem;
        double priority;
    };

    Vector<entry> entries;
};

// #include "pqueue-impl.h"

// #endif
///
template <typename Type>
PQueue<Type>::PQueue() {}

template <typename Type>
PQueue<Type>::~PQueue() {}

/**
 * Implementation notes: size, isEmpty, clear
 * ------------------------------------------
 * These implementations simply forward the request to the
 * underlying Vector object.
 */

template <typename Type>
int PQueue<Type>::size() const {
    return entries.size();
}

template <typename Type>
bool PQueue<Type>::isEmpty() const {
    return entries.isEmpty();
}

/**
 * Implementation notes: enqueue
 * -----------------------------
 * This function finds where to insert a new element into the
 * queue and then calls insertAt to put it there.  Because
 * items are removed from the end of the queue, the highest
 * priority elements must be stored at the end of the queue.
 * Moreover, to ensure that elements obey the first-in/first-out
 * discipline when they have the same priority, the function must
 * insert each new element before any with the same priority.
 * Keep in mind that the base type of the vector is pqEntryT,
 * which contains both the element and the priority.
 */

template <typename Type>
void PQueue<Type>::enqueue(const Type& elem, double priority) {
    int pos = 0;
    while (pos < entries.size() && entries[pos].priority > priority) {
        pos++;
    }
    entry ent = {elem, priority};
    entries.insert(pos, ent);
}

/**
 * Implementation notes: extractMin, peek
 * --------------------------------------
 * These functions must check for an empty queue and report an
 * error if there are no entries.
 */

template <typename Type>
Type PQueue<Type>::extractMin() {
    if (isEmpty()) error("extractMin: Attempting to extractMin from an empty queue.");
    int lastIndex = entries.size() - 1;
    Type result = entries[lastIndex].elem;
    entries.remove(lastIndex);
    return result;
}

template <typename Type>
const Type& PQueue<Type>::peek() const {
    if (isEmpty()) error("peek: Attempting to peek at an empty queue.");
    return entries[entries.size() - 1].elem;
}

template <typename Type>
const Type* PQueue<Type>::getHead() const {
    if (isEmpty()) return NULL;
    return &(entries[0].elem);
}


////////////// Priority queue implementation ends

Map<int, int> buildFrequencyTable(istream& input) {
    // TODO: implement this function
    Map<int, int> freqTable; // this is just a placeholder so it will compile


    int ch = input.get();

    while(ch!=EOF){
        if( freqTable.containsKey(ch) ){
            freqTable[ch] = (freqTable[ch] + 1);
        }else{
            freqTable.add(ch, 1);
        }
        ch = input.get();
    }
     freqTable.add(256, 1); // add one EOF
    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    // TODO: implement this function
    PQueue<HuffmanNode*> pq;
    Vector<int> letters = freqTable.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        pq.enqueue( new HuffmanNode(*i, freqTable[*i], NULL, NULL) , freqTable[*i]);
    }
//    while(!pq.isEmpty()){
//        HuffmanNode* highPri = pq.extractMin();
//        cout<< highPri->toString()<<" ";
//    }
    HuffmanNode* head = NULL;
    while(pq.size() > 1){
        HuffmanNode* minEle1 = pq.extractMin();
        HuffmanNode* minEle2 = pq.extractMin();
        int newPriority = (minEle1->count + minEle2->count);
        HuffmanNode* jointEle = new HuffmanNode(NOT_A_CHAR, newPriority, minEle1, minEle2);
        pq.enqueue( jointEle , newPriority);
    }
    head = pq.extractMin();
    return head;
    // return const_cast<HuffmanNode*>(pq.getHead());   // TODO stupid, PQ of HuffmanNode* was to be made
}

string getCodeFromVector(Vector<int> pathTillNow){
    string retVal = "";
    for(Vector<int>::iterator i=pathTillNow.begin(); i!= pathTillNow.end(); i++)
        retVal+=to_string(*i);
    return retVal;
}

void traverseAndPopulateMap(HuffmanNode* encodingTree, Vector<int>& pathTillNow, Map<int, string>& encodingMap){
    // cout<<"Called with vector sequence "<<getCodeFromVector(pathTillNow);
    if(encodingTree == NULL){
        return;
    }
    if(encodingTree->isLeaf()){
        // update encoding map and return
        string huffCodeForLetter = getCodeFromVector(pathTillNow);
        encodingMap.add(encodingTree->character, huffCodeForLetter);
        // cout<<"\nCode found at leaf: "<< encodingTree->character<<" "<< huffCodeForLetter<<endl;
        return;
    }
    else{
        // letf
        pathTillNow.push_back(0);
        traverseAndPopulateMap(encodingTree->zero, pathTillNow, encodingMap);
        int lastIndex = pathTillNow.size() - 1;
        pathTillNow.remove(lastIndex);
        // right
        pathTillNow.push_back(1);
        traverseAndPopulateMap(encodingTree->one, pathTillNow, encodingMap);
        lastIndex = pathTillNow.size() - 1;
        pathTillNow.remove(lastIndex);
        return;
    }
}

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    Vector<int> pathTillNow;
    traverseAndPopulateMap(encodingTree, pathTillNow, encodingMap);
    return encodingMap;
}

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    int ch = input.get();
    while(ch!=-1){
        // cout<<(char)ch;
        string bitString = encodingMap[ch];
        for(int i=0; i<bitString.length(); i++){
            (bitString[i] == '1') ? output.writeBit(1): output.writeBit(0);
        }
        ch = input.get();
    }
    string bitString = encodingMap[256];
    for(int i=0; i<bitString.length(); i++){
        (bitString[i] == '1') ? output.writeBit(1): output.writeBit(0);
    }
}

void traverseAndDecode(Vector<int> bitSequence, HuffmanNode* encodingTree, ostream& output){
    int i=-1;
    int bitSequenceLen = bitSequence.size();
    if(bitSequenceLen == 0) // empty input
        return;
    bool stop = false; // flag to see if EOF was reached

    HuffmanNode* curr = encodingTree;
    while( ! (i+1>=bitSequenceLen || stop) ){
        i++;
        if(bitSequence[i] == 0){
            // go left
            curr = curr->zero;
        }
        else if(bitSequence[i] == 1){
            // go right
            curr = curr->one;
            }
        // if leaf, update output and set curr to head
        if(curr->isLeaf()){
            if(curr->character == EOF)
                stop = true;
            output.put(curr->character);
            curr = encodingTree;
        }
    }
    // TODO kay: correct answer but 2 bytes extra, don't know why
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if(encodingTree == NULL){
        return;
    }
    Vector<int> bitSequence;
    int ch = input.readBit();;
    while(ch!=EOF){
        bitSequence.push_back(ch);
        ch = input.readBit();
    }
    cout<<endl;
    traverseAndDecode(bitSequence, encodingTree, output);
}

void compress(istream& input, obitstream& output) {
    // Build Frequency Table, Build Tree, Build Map, Print Frequency Table to output, rewind, encode the file to output
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);
    // print freqtable to output
    input.seekg(0); // rewind
    encodeData(input, encodingMap, output);

}

///* Function: readFileHeader
// * Usage: Map<int, int> freq = writeFileHeader(input);
// * --------------------------------------------------------
// * Reads a table to the front of the specified input file
// * that contains information about the frequencies of all of
// * the letters in the input text.  This information can then
// * be used to reconstruct the encoding tree for that file.
// *
// * This function is provided for you.  You are free to modify
// * it if you see fit, but if you do you must also update the
// * writeFileHeader function defined before this one so that it
// * can properly write the data.
// */
Map<int, int> readFileHeader(ibitstream& infile) {
    /* This function inverts the mapping we wrote out in the
     * writeFileHeader function before.  If you make any
     * changes to that function, be sure to change this one
     * too!
     */
    Map<int, int> result;

    /* Read how many values we're going to read in. */
    int numValues;
    infile >> numValues;

    /* Skip trailing whitespace. */
    infile.get();

    /* Read those values in. */
    for (int i = 0; i < numValues; i++) {
        /* Get the character we're going to read. */
        int ch = infile.get();

        /* Get the frequency. */
        int frequency;
        infile >> frequency;

        /* Skip the space character. */
        infile.get();

        /* Add this to the encoding table. */
        result[ch] = frequency;
    }

    /* Add in 1 for PSEUDO_EOF. */
    result[PSEUDO_EOF] = 1;
    return result;
}

void decompress(ibitstream& input, ostream& output) {
    // implementation 1 starts || mine
                // !!!!! read the freq table from wheeeere?
                // Read the Frequency Table, Build Tree, Decode File
                Map<int, int> freqTable = buildFrequencyTable(input); // MUST BE WRONG!, mandatory for next line
                HuffmanNode* encodingTree = buildEncodingTree(freqTable); // mandatory for next line
                decodeData(input, encodingTree, output); // mandatory line
                freeTree(encodingTree);
    // implementation 1 ends

    // implementation 2 starts || copied
//                Map<int, int> freqTable = readFileHeader(input);
//                HuffmanNode* encodingTree = buildEncodingTree(freqTable);
//                decodeData(input, encodingTree, output); // mandatory line
//                freeTree(encodingTree);
    // implementation 2 ends



}

void freeTree(HuffmanNode* node) {
    if(!node) // empty tree, return
        return;
    if(node->zero) // delete left subtree if exists
        freeTree(node->zero);
    if(node->one) // delete right subtree if exists
        freeTree(node->one);
    delete node; // free(node); // delete this node
    return;
}
