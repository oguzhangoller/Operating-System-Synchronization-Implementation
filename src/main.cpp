#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <algorithm>
#include "Process.h"
#include "Semaphore.h"


#define quantum 100

//convert char to corresponding integer value
inline int toDigit(char c) {return c-'0';}

using namespace std;

void startExecution();
void executeProcess(Process *);
void processQueue();
void checkProcessArrivals();
void printProcessInfo();


//this vector contains all processes read from file
vector<Process *> waitingVec;
//this queue holds processes whose arrival times are past and waiting to be executed
queue<Process *> readyQueue;
//holds current time in units, currently set to 0
int currentTime = 0;

ofstream out("output.txt");


//contains all semaphors, used to check whether corresponding semaphor is free
vector<Semaphore> semaVec(10);

/*
 * holds all processes waiting for a semaphor as keys and the semaphor they wait as values
 * each time checkProcessArrivals is called, this map is iterated to check whether some of
 * the semaphors this processes wait are set to free
 */
map<Process *,int> processMap ;

/*
 * contains processes that are waiting for a semaphor to become free,
 * in the order that process with highest priority is at front of the list and
 * index number being corresponding semaphor number
 */

vector<vector<string>> waiting(10);

int main() {
    ifstream reader("definition.txt");

    //read values from file and create processes according to those values

    if(reader) {

        string processName, codeNumber;
        int arrivalTime;

        while (reader >> processName >> codeNumber >> arrivalTime) {
            Process *p = new Process(processName,codeNumber, arrivalTime);
            waitingVec.push_back(p);
        }
        startExecution();
    }
    else{
        cerr<<"Can't open file!"<<endl;
    }
    return 0;
}

/* this function iterates over processes waiting for semaphors, and if that semaphor is
 * available, it removes the process with highest priority from processmap and adds it to
 * the readqueue to be executed
 *
 * it also checks this function check whether there are process arrivals to the cpu -
 * waiting to be executed for current time, and adds them to ready queue if so, removing
 * them from waiting queue
 */
void checkProcessArrivals(){

      for (auto it = processMap.cbegin(); it != processMap.cend();) {
          if (!semaVec[it->second].isBusy()  && !(find(waiting[it->second].begin(), waiting[it->second].end(), it->first->getName()) != waiting[it->second].end())) {
             readyQueue.push(it->first);
              it = processMap.erase(it);
              printProcessInfo();
          } else {
              ++it;
          }
      }

    for (auto it = waitingVec.begin(); it != waitingVec.end();) {
        if ((*it)->getArrival() <= currentTime) {
            readyQueue.push(*it);
            it = waitingVec.erase(it);
        }
        else {
            it++;
        }
    }

}

/*
 * starts the execution of cpu
 * while there are processes that are not arrived yet,
 * calls checkProcessArrivals to update and check processes
 * and calls processQueue to process contents of queue according to that time and
 * update currentTime with quantum if there are no processes,
 *
 * after while loop breaks, calls processQueue one more time to make sure
 * all of readyQueue is processed.
 */

void startExecution(){

    while(waitingVec.size()>0) {
        checkProcessArrivals();
        processQueue();
    }

    processQueue();

}

/*
 * this method executes process at the front of queue ,
 * if the queue is empty updates time by quantum units
 */
void processQueue(){

    while(!readyQueue.empty()) {
        printProcessInfo();
        executeProcess((readyQueue.front()));
        checkProcessArrivals();

    }

    printProcessInfo();
    currentTime += quantum;
}

/*
 * executes a specific process instructions until a time greater than quantum
 * has passed, modifies currentTime value according to sum of instruction times
 */
void executeProcess(Process * p){
    int timeSum = 0 ;


    while(timeSum<quantum){
        if(p->processFinished()) {
            readyQueue.pop();
            return;
        }

        Instruction temp = p->popInstruction();
        //check if the currrent instruction is wait or sign instruction
        if(temp.getDuration() == 0){
            //get which semaphor this instruction is related to
            int semapNo = toDigit(temp.getName().back());

            if(temp.getName().find("wait") == 0) {
                /*
                 * if related semaphor is busy, add this process to queue
                 * of corresponding semaphor and also to processmap so that
                 * each time checkProcessArrival is called, all processes are checked
                 * whether semaphors they wait are free
                */
                if (semaVec[semapNo].isBusy()) {

                        semaVec[semapNo].processWaiting.push(p->getName());
                        p->pushInstructionFront(temp);
                        readyQueue.pop();
                        processMap.insert(make_pair(p,semapNo));
                        return;
                    }

                    /*
                     * if corresponding semaphor is free, checks whether there are other
                     * processes waiting to use that semaphor, if so; gives them ability to use
                     * that semaphore, adding current process to waiting queue for semaphors
                     *
                     * if no process is waiting for this semaphor, enables current process to
                     * use this semaphor
                     */
                    else {
                        if (semaVec[semapNo].processWaiting.empty())
                            {
                                semaVec[semapNo].setBusy();
                            }
                        else {
                            if(semaVec[semapNo].processWaiting.front() == p->getName()){
                                semaVec[semapNo].processWaiting.pop();

                                semaVec[semapNo].setBusy();
                            }
                            else {

                                waiting[semapNo].push_back(p->getName());
                                semaVec[semapNo].processWaiting.push(p->getName());
                                p->pushInstructionFront(temp);
                                readyQueue.pop();


                                processMap.insert(make_pair(p,semapNo));

                                return;
                            }
                        }
                    }
            }
            /*
             * if the instruction with duration 0 is a sign instruction,
             * set corresponding semaphor to free and remove the first process
             * from waiting queue because it will be processed next
             */
            else{
                semaVec[semapNo].setFree();
                if(!waiting[semapNo].empty())
                    waiting[semapNo].erase(waiting[semapNo].begin());
                checkProcessArrivals();
            }
        }
        //update time according to instruction
        timeSum += temp.getDuration();
        currentTime += temp.getDuration();
        if(temp.getName()== "exit"){
            readyQueue.pop();
            return ; }
    }

    //add process that is processed from head of queue to tail of queue
    checkProcessArrivals();
    readyQueue.pop();
    readyQueue.push(p);

}

/*
 * prints the time and queue info to output by creating a copy of
 * readyQueue and iterating over it
 */
void printProcessInfo(){

    out<<currentTime<<"::";
    queue<Process *> tmp_q = readyQueue;
    out<<"HEAD-";

    while (!tmp_q.empty())
    {

        Process * q_element = tmp_q.front();
        out << q_element->getName() <<"-";
        tmp_q.pop();

    }

    out<<"TAIL"<<endl;

}
