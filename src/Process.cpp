

#include "Process.h"
#include <fstream>
#include <iostream>

/*
 * this constructor creates the process with given m_name and arrival time,
 * adds instructions to this process reading from the file given
 */

Process::Process(std::string name,std::string fileName, int startTime) :m_name(name),m_startTime(startTime)
{

    std::ifstream reader(fileName);


    int duration;
    if(reader) {
        std::string instrName;
        while(reader>>instrName>>duration){
            pushInstructionBack(Instruction(instrName,duration));
        }

    }

    else{
        std::cerr<<"Not a valid file m_name"<<std::endl;
    }

}

/*
 * pushes the instruction given as parameter to back of queue
 */
void Process::pushInstructionBack(Instruction instr) {
    instructions.push_back(instr);
}
/*
 * pushes the instruction given as parameter to front of queue
 */
void Process::pushInstructionFront(Instruction instr) {
    instructions.push_front(instr);
}

/*
 * gets runtime of next instruction and returns it,
 * removing it from instructions queue
 */

Instruction Process::popInstruction() {
    Instruction instr = instructions.front();
    instructions.pop_front();
    return instr;
}

//checks whether all instructions of this process are executed
bool Process::processFinished() const{
    return instructions.empty();
}

//returns arrival time of this process to cpu

int Process::getArrival() const{
    return m_startTime;
}

//returns m_name of this process

std::string Process::getName() const{
    return m_name;
}

