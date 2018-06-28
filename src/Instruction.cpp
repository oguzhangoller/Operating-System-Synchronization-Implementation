//
// Created by oguzhan on 12/4/17.
//

#include "Instruction.h"

Instruction::Instruction(const std::string &name, int duration) : m_name(name), m_duration(duration)
{
}


int Instruction::getDuration()const {
    return m_duration;
}

const std::string &Instruction::getName()const {
    return m_name;
}