#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    DailySchedule& sched,
    std::vector<int>& shiftsLeft,
    int kIdx, int jIdx);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    //this vector keeps track of how many shifts
    //each worker has left
    std::vector<int> shiftsLeft;
    for(size_t k = 0; k < avail[0].size(); k++){
      shiftsLeft.push_back(maxShifts);
    }

    //creates a 2d array for sched that is empty
    for(size_t i = 0; i < avail.size(); i++){
      //try pushing a filled vector into sched instead
      sched.push_back(std::vector<Worker_T>(dailyNeed, INVALID_ID));
    }

    //on to actual recursion
    return scheduleHelper(avail, dailyNeed, sched, shiftsLeft, 0, 0);
    


}

bool scheduleHelper(const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    DailySchedule& sched, std::vector<int>& shiftsLeft,
    int kIdx, int jIdx)
{
  //kIdx is row number, jIdx is col number

  //this is to make recursion simpler, if we finish a days shift allocations
  //we can move on to the next day
  if(jIdx == (int)dailyNeed){
    //std::cout << "are equal?" << std::endl;
    kIdx++;
    jIdx = 0;
  }
  //std::cout << "jIdx = " << jIdx << " and kIdx = " << kIdx << std::endl;
  
  
  //once we reach the "end", we successfully allocated a shift for each day we return true
  if(kIdx == (int)avail.size()){
    return true;
  }

  //we will loop over each employee available for the day
  for(size_t i = 0; i < avail[kIdx].size(); i++){
    //if they're available and they haven't exceeded their max shifts
    //and they can't do more than one shift a day

    if(avail[kIdx][i] == 1 && shiftsLeft[i] != 0 &&
      std::find(sched[kIdx].begin(), sched[kIdx].end(), i) == sched[kIdx].end()){
      sched[kIdx][jIdx] = i;
      shiftsLeft[i]--;
      //we will allocate them to the current part of the 2d array, lower their remaining shifts
      //then move on to next level
      bool fine = scheduleHelper(avail,dailyNeed,sched, shiftsLeft, kIdx, jIdx+1);
      if(fine){
        return true;
      }
      //if we made it to this point, that means a later level of recursion coudldn't find a solution
      //we should undo the work we did in this level, and try the next available employee(if there is one)
      sched[kIdx][jIdx] = INVALID_ID;
      shiftsLeft[i]++;
    }
    
  }
  //if we couldn't find an available employee to fill a remaining shift for the day, then that means
  //we must return to a previous level and try a different combination
  return false;
    
    
}
