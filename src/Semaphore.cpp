//
// Created by oguzhan on 12/7/17.
//

#include "Semaphore.h"

Semaphore::Semaphore()
{
}

bool Semaphore::isBusy() const {
    return Busy;
}

void Semaphore::setBusy() {
    Semaphore::Busy = true;
}

void Semaphore::setFree() {
    Semaphore::Busy = false;
}


