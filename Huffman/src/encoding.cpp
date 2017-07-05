#include "encoding.h"
#include "vector.h"
#include <string>
#include "pqueue.h"

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
    PriorityQueue<HuffmanNode*> pq;
    Vector<int> letters = freqTable.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        pq.enqueue( new HuffmanNode(*i, freqTable[*i], NULL, NULL) , freqTable[*i]);
    }

    HuffmanNode* head = NULL;
    while(pq.size() > 1){
        HuffmanNode* minEle1 = pq.dequeue();
        HuffmanNode* minEle2 = pq.dequeue();
        int newPriority = (minEle1->count + minEle2->count);
        HuffmanNode* jointEle = new HuffmanNode(NOT_A_CHAR, newPriority, minEle1, minEle2);
        pq.enqueue( jointEle , newPriority);
    }
    head = pq.dequeue(); // dequeue

    // todo: delete the pq to avoid mem leak
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

void writeFileHeader(obitstream& outfile, Map<int, int>& frequencies) {
    /* The format we will use is the following:
     *
     * First number: Total number of characters whose frequency is being
     *               encoded.
     * An appropriate number of pairs of the form [char][frequency][space],
     * encoding the number of occurrences.
     *
     * No information about PSEUDO_EOF is written, since the frequency is
     * always 1.
     */

    /* Verify that we have PSEUDO_EOF somewhere in this mapping. */
    if (!frequencies.containsKey(PSEUDO_EOF)) {
        error("No PSEUDO_EOF defined.");
    }

    /* Write how many encodings we're going to have.  Note the space after
     * this number to ensure that we can read it back correctly.
     */
    outfile << frequencies.size() - 1 << ' ';

    /* Now, write the letter/frequency pairs. */

//    foreach (int ch in frequencies) {
//        // Skip PSEUDO_EOF if we see it.
//        if (ch == PSEUDO_EOF) continue;

//        // Write out the letter and its frequency.
//        outfile << char(ch) << frequencies[ch] << ' ';
//    }

    Vector<int> letters = frequencies.keys();
    for(Vector<int>::iterator i=letters.begin(); i!=letters.end(); i++){
        if(*i == PSEUDO_EOF) // Skip EOF if we see it
            continue;
        outfile << (char) *i << frequencies[*i] << ' '; // Write out the letter and its frequency.
    }
}

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

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);               // Build Frequency Table
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);           // Build Tree
    Map<int, string> encodingMap = buildEncodingMap(encodingTree);      // Build Map

//    output << freqTable;                                               // Print Frequency Table to output
    writeFileHeader(output, freqTable);                                 // Print Frequency Table to output
    cout<<"moving input pos from "<<input.tellg();
    input.clear();
    input.seekg(0);                                                      // rewind
    cout<<"to "<<input.tellg();
    encodeData(input, encodingMap, output);                              // encode the file to output
    freeTree(encodingTree);
}

void decompress(ibitstream& input, ostream& output) {
    Map<int, int> freqTable = readFileHeader(input);
//    Map<int, int> freqTable; input >> freqTable;

    cout<<"\n freq noted while decompressing: \n";
    cout<<freqTable.toString()<<endl;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    printSideways(encodingTree, false, "   ");
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
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
