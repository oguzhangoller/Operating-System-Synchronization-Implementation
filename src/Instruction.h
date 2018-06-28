//
// Created by oguzhan on 12/4/17.
//

#ifndef INC_322PROJECT2_INSTRUCTION_H
#define INC_322PROJECT2_INSTRUCTION_H

#include <string>

class Instruction{

private:
    std::string m_name;
    int m_duration;

public:
    Instruction(const std::string &name,int duration);
    int getDuration()const;
    const std::string &getName()const;


};



#endif //INC_322PROJECT2_INSTRUCTION_H
