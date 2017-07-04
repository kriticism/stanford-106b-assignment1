#include "encoding.h"

////////////// Priority queue implementation starts
#include "vector.h"
#include <string>


template <typename Type>
class PQueue {
public:

    PQueue();
    ~PQueue();
    int size() const;
    bool isEmpty() const;
    void enqueue(const Type& elem, double priority);
    Type extractMin();
    const Type& peek() const;
    const Type* getHead() const;

private:
    struct entry {
        Type elem;
        double priority;
    };

    Vector<entry> entries;
};

///
template <typename Type>
PQueue<Type>::PQueue() {}
template <typename Type>
PQueue<Type>::~PQueue() {}

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
    Map<int, int> freqTable;
    int ch = input.get();
    while(ch!=EOF){
        if( freqTable.containsKey(ch) ){
            freqTable[ch] = (freqTable[ch] + 1);
        }else{
            freqTable.add(ch, 1);
        }
        ch = input.get();
    }
     freqTable.add(PSEUDO_EOF, 1); // add one EOF

    return freqTable;
}

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PQueue<HuffmanNode*> pq;
    Vector<int> letters = freqTable.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        pq.enqueue( new HuffmanNode(*i, freqTable[*i], NULL, NULL) , freqTable[*i]);
    }

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
}

string getCodeFromVector(Vector<int> pathTillNow){
    string retVal = "";
    for(Vector<int>::iterator i=pathTillNow.begin(); i!= pathTillNow.end(); i++)
        retVal+=to_string(*i);
    return retVal;
}

void traverseAndPopulateMap(HuffmanNode* encodingTree, Vector<int>& pathTillNow, Map<int, string>& encodingMap){
    if(encodingTree == NULL){
        cout<<"traversal called on null node!";
        return;
    }
    if(encodingTree->isLeaf()){
        string huffCodeForLetter = getCodeFromVector(pathTillNow);
        encodingMap.add(encodingTree->character, huffCodeForLetter);
        return;
    }
    else{
        if(encodingTree->zero){
            // letf
            pathTillNow.add(0);
            traverseAndPopulateMap(encodingTree->zero, pathTillNow, encodingMap);
            int lastIndex = pathTillNow.size() - 1;
            pathTillNow.remove(lastIndex);
        }
        if(encodingTree->one){
            // right
            pathTillNow.add(1);
            traverseAndPopulateMap(encodingTree->one, pathTillNow, encodingMap);
            int lastIndex = pathTillNow.size() - 1;
            pathTillNow.remove(lastIndex);
        }
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
    while(ch!=EOF){
        string bitString = encodingMap[ch];
        for(int i=0; i<bitString.length(); i++){
            (bitString[i] == '1') ? output.writeBit(1): output.writeBit(0);
        }
        ch = input.get();
    }
    string bitString = encodingMap[PSEUDO_EOF];
    for(int i=0; i<bitString.length(); i++){
        (bitString[i] == '1') ? output.writeBit(1): output.writeBit(0);
    }
}

void traverseAndDecode(Vector<int> bitSequence, HuffmanNode* encodingTree, ostream& output){
    int i=0, bitSequenceLen = bitSequence.size();

    if(bitSequenceLen == 0) // empty input
        return;
    bool stop = false; // flag to see if EOF was reached

    HuffmanNode* curr = encodingTree;
    while(  i<bitSequenceLen && !stop ){
        if(bitSequence[i] == 0){
            // go left
            if(curr->zero)
                curr = curr->zero;
            else
                cout<<"No left to this!\n";
        }
        else{  // if(bitSequence[i] == 1)
            // go right
            if(curr->one)
                curr = curr->one;
            else
                cout<<"No right to this!\n";
        }
        // if leaf, update output and set curr to head
        if( curr->isLeaf() ){
            // cout<<"leaf here\n";
            if(curr->character == PSEUDO_EOF){
                // cout<<"EOF found";
                output.put( (char) PSEUDO_EOF );
                stop = true;
                break;
            }

            output.put( (char) curr->character );
            curr = encodingTree;
        }
        i++;
    }
    // TODO kay: correct answer but 2 bytes extra, don't know why
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    if(encodingTree == NULL){
        return;
    }
    Vector<int> bitSequence;
    // TODO: wrong bit stream received here when using decompress!!!
    int ch = input.readBit();
    while(ch!=EOF){
        bitSequence.add(ch);
        ch = input.readBit();
    }

    traverseAndDecode(bitSequence, encodingTree, output);
}

/* Function: writeFileHeader
 * Usage: writeFileHeader(output, frequencies);
 * --------------------------------------------------------
 * Writes a table to the front of the specified output file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to decompress input files once they've been
 * compressed.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * readFileHeader function defined below this one so that it
 * can properly read the data back.
 */
void writeFileHeader(obitstream& outfile, Map<int, int>& frequencies) {
    /* The format we will use is the following:
     * num_of_chars<_>num_of_elements<_>{[char][freq]<_>}<bits>
     * First number: Total number of characters whose frequency is being
     *               encoded.
     * An appropriate number of pairs of the form [char][frequency][space],
     * encoding the number of occurrences.
     *
     * No information about EOF is written, since the frequency is
     * always 1.
     * TODO: try to reposition bit after-
     * sizeof(int) * (1 + number_of_elements) + sizeof(char) * (2+number_of_elements*2) bytes
     */

    /* Verify that we have EOF somewhere in this mapping. */
    if (!frequencies.containsKey(PSEUDO_EOF)) {
        error("No PSEUDO_EOF defined.");
    }

    /* Write how many encodings we're going to have.  Note the space after
     * this number to ensure that we can read it back correctly.
     */
    outfile << frequencies.size() - 1<< ' ';

    // Now, write the letter/frequency pairs.
    Vector<int> letters = frequencies.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        if(*i == PSEUDO_EOF) // Skip EOF if we see it
            continue;
        // cout << (char) *i << frequencies[*i] << ' ';
        outfile << (char) *i << frequencies[*i] << ' '; // Write out the letter and its frequency.
    }
}

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);               // Build Frequency Table
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);           // Build Tree
    printSideways(encodingTree, false, "   ");
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);      // Build Map
    writeFileHeader(output, freqTable);                                 // Print Frequency Table to output
    input.seekg(0);                                                      // rewind
    encodeData(input, encodingMap, output);                              // encode the file to output
}

/* Function: readFileHeader
 * Usage: Map<int, int> freq = writeFileHeader(input);
 * --------------------------------------------------------
 * Reads a table to the front of the specified input file
 * that contains information about the frequencies of all of
 * the letters in the input text.  This information can then
 * be used to reconstruct the encoding tree for that file.
 *
 * This function is provided for you.  You are free to modify
 * it if you see fit, but if you do you must also update the
 * writeFileHeader function defined before this one so that it
 * can properly write the data.
 */
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
        result.add(ch,frequency);
    }

    /* Add in 1 for EOF. */
    result.add(PSEUDO_EOF, 1);
    return result;
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freq = readFileHeader(input);
    cout<<"\n freq noted while decompressing: \n";
    cout<<freq.toString()<<endl;
    HuffmanNode* tree = buildEncodingTree(freq);
    printSideways(tree, false, "   ");
    decodeData(input, tree, output);
    // freeTree(tree);
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
