#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

void wordleHelper(std::string in, std::string floating, const std::set<std::string>& dict, int idx, std::set<std::string>& results);
int blankCharLeft(std::string str);
std::string customCharRemove(std::string str, int idx);
// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here

    //ok i will outline here
    //look for first available letter(-) to fill in
    //put 'a' using loop, char to int conversion or something
    //go to next spot with recursion
    //put 'a' (literally the same loop)
    //once reached the final spot check if its valid(is legal, and has all the floating characters)
    //if its valid we insert, if not we don't
    //recursion goes back and does next char in loop until
    //all combos are done
    //keep track of floating chars to avoid combos that will never work(save time)
    //ok we will do recursion logic in a helper because i want to pass in the set by reference that we will return in this function
    std::set<std::string> results;
    wordleHelper(in,floating,dict,0,results);
    return results;
    

}

void wordleHelper(std::string in, std::string floating, const std::set<std::string>& dict, int idx, std::set<std::string>& results){
    if(idx == (int)in.size()){
        if(floating.empty() && dict.find(in)!= dict.end()){
            results.insert(in);
        }
        //ok good i didn't forget
        return;
    }

    //if it's already a character, then we don't want to change it; we skip it
    if(in[idx] != '-'){
        wordleHelper(in,floating,dict,idx+1,results);
    }else{
        //we need a way to check how many '-' chars are left, if '-' chars = floating chars, then we should only pick from those
        //this would make function more efficient, but will try without it for now

        //to make more efficient, once we know that the # of floating characters = remaining spots, we should only check those
        //this cuts off many useless combinations i think
        if(blankCharLeft(in) == (int)floating.size()){
            //loop 1
            for(int i = 0; i < (int)floating.size(); i++){
                in[idx]=floating[i];
                //used to be floating.erase(i,1) for the 2nd param
                wordleHelper(in,customCharRemove(floating,i), dict, idx+1, results);
            }
        }else{
            //otherwise, we keep looking for combinations
            //loop 2
            for(int i = 0; i < 26; i++){
                bool dontRepeat = true;
                in[idx]= (char)(97+i);
                //loop 3
                //
                for(size_t k = 0; k < floating.size(); ++k){
                    if(floating[k]==in[idx]){
                        //i think this is supposed to be floating.erase(k,1)
                        //for posterity's sake, because i'm not testing today
                        //if i'm wrong, the code used to say "floating.erase(i,1)"
                        wordleHelper(in, customCharRemove(floating, k), dict, idx+1, results);
                        dontRepeat = false;
                        //when we reach this point in the recursion, we don't want to do it again
                        //we still need to make sure for cases when we don't add a char in floating
                        //that it still recurses to the next level
                        //dontRepeat lets us do this
                        break;
                    }
                }
                if(dontRepeat){
                    wordleHelper(in,floating,dict,idx+1,results);
                }
            }
        }
    }
}
// Define any helper functions here

//special remove function that only works if position of char is known
std::string customCharRemove(std::string str, int idx){
    int strLen = str.size();
    if(idx == 0){
        return str.substr(1, strLen-1);
    }else if(idx == strLen-1){
        return str.substr(0,strLen-1);
    }else{
        return str.substr(0,idx) + str.substr(idx+1, strLen-1-idx);
    }
}
/*
example: str = "help", idx = 2
return "he" + "p"

example: str = "faker", idx = 2
return str.substr(0,2) ("fa") + str.substr(3, 2) ("er");


*/

int blankCharLeft(std::string str) {
    int count = 0;
    //loop 4
    for(size_t i = 0; i < str.size(); i++){
        if(str[i]=='-'){
            count++;
        }
    }
    return count;
}
