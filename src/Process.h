
#ifndef INC_322PROJECT_PROCESS_H
#define INC_322PROJECT_PROCESS_H

#include <string>
#include <queue>
#include <deque>
#include "Instruction.h"


class Process{

private:
    std::string m_name;
    int m_startTime;
    //holds all instructions this process contains
    std::deque<Instruction> instructions;

public:
    Process(std::string name,std::string fileName,int startTime);
    void pushInstructionBack(Instruction);
    void pushInstructionFront(Instruction);
    Instruction popInstruction();
    bool processFinished() const;
    int getArrival() const;
    std::string getName() const;


};


#endif //INC_322PROJECT_PROCESS_H