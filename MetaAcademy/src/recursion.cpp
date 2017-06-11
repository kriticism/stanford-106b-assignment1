/*
* Author: Kriti Singh | ks2259@cornell.edu
* Date: June 6, 2017
* Towards assignment for Stanford's CS106B as a preparatory course.
*/

#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
#include "random.h"
using namespace std;
int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};
// TODO : add exception to milestone 2
int gcd(int a, int b) {
    // your code here
    if(b==0){
        cout<< "gcd("<<a<<","<<b<<") = "<<a<<endl;
        return a;
    }
    else{
        // cout << "[recursion gcd called]" << endl;
        cout<< "gcd("<<a<<","<<b<<") = gcd("<<b<<","<<a%b<<")\n";
        return gcd(b, a%b);
    }

    return 0;
}

void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    // your code here
    if(order == 0)
        return;
    else if(order < 0){
        cout<<"Negative order not allowed.\n";
        return;
    }
    int rightX = leftX+size, rightY = leftY;
    int baseX = leftX+size/2, baseY = leftY+size*pow(3, 0.5)/2;

    w.drawLine(leftX, leftY, rightX, rightY);
    w.drawLine(baseX, baseY, leftX, leftY);
    w.drawLine(baseX, baseY, rightX, rightY);

    serpinskii(w, leftX, leftY, size/2, order-1);
    serpinskii(w, (leftX+rightX)/2, leftY, size/2, order-1);
    serpinskii(w, (3*leftX+rightX)/4, leftY+size*pow(3, 0.5)/4, size/2, order-1);
    //cout << "[recursion serpinskii called]" << endl;
}


bool isBounded(GBufferedImage& image, int x, int y){
    return image.inBounds(x,y);
}
int recursiveFillAndCount(GBufferedImage& image, int x, int y, int newColor, int segmentColor){
    if(!isBounded(image,x,y))
        return 0;
    if(newColor == image.getRGB(x,y) || (image.getRGB(x,y)!=segmentColor))
        return 0;
    else{
        image.setRGB(x,y,newColor);
        return recursiveFillAndCount(image,x+1,y,newColor,segmentColor) + recursiveFillAndCount(image,x-1,y,newColor,segmentColor) +recursiveFillAndCount(image,x,y+1,newColor,segmentColor) +recursiveFillAndCount(image,x,y-1,newColor,segmentColor) +1;
    }
}
int floodFill(GBufferedImage& image, int x, int y, int newColor) {
    if(!isBounded(image,x,y))
        return 0;
    return recursiveFillAndCount(image,x+1,y,newColor,image.getRGB(x,y));
}

void _personalCurriculum(Map<string, Vector<string>> & prereqMap, string goal, Vector<string> &finalSeq) {

    Vector<string> thisPreReq= prereqMap.get(goal);
    int l = thisPreReq.size(); // number of pre requisites for goal
    int startIdx = finalSeq.size();
    int count = 0;
    for(int i=0; i<l; i++){ // for each pre requisite of this goal
        // add if not already there, increment count
        bool isAlready = false;
        //cout<<"Checking "<<thisPreReq[i]<<"...\n";
        for(int j=0; j<startIdx; j++){ // TODO: poor method, use a set to keep track
            if(finalSeq[j] == thisPreReq[i]){
                isAlready = true;
                // cout<<thisPreReq[i]<<" is already there!\n";
                break;
            }
        }
        if(!isAlready){
            // cout<<thisPreReq[i]<<" is NOT there! Adding...\n";
            finalSeq.push_back(thisPreReq[i]); count++;
        }
    }
    for(int i=startIdx; i<startIdx+ count; i++){
        string newGoal = finalSeq[i];
        // cout<<"Checking "<<newGoal<<"...\n";
        _personalCurriculum(prereqMap, newGoal, finalSeq);
    }
    // cout << "[recursion personal curriculum called]" << endl;
}

void personalCurriculum(Map<string, Vector<string>> & prereqMap, string goal) {
    // your code here
    Vector<string> finalSeq;
    _personalCurriculum(prereqMap, goal, finalSeq); // helper recursive fn
    for(int i=finalSeq.size()-1; i>=0; i--)
        cout<<finalSeq[i]<<endl;
    cout<<goal<<endl;
}



string generate(Map<string, Vector<string> > & grammar, string symbol) {
    string upto = "";
    if(grammar.containsKey(symbol)){
        Vector<string> thisExpansion = grammar.get(symbol);
        string subExp = thisExpansion.get(randomInteger(0,thisExpansion.size()-1));
        TokenScanner scanner(subExp);
        while (scanner.hasMoreTokens()) {
           string token = scanner.nextToken();
           upto += generate(grammar,token);
        }
    }else{
        return symbol;
    }
    return upto;
}
