//
// Created by oguzhan on 12/7/17.
//

#ifndef INC_322PROJECT2_SEMAPHORE_H
#define INC_322PROJECT2_SEMAPHORE_H

#include <string>
#include <queue>

class Semaphore{

private:

    bool Busy = false;


public:
    //this queue holds the processes waiting for this semaphor
    std::queue<std::string> processWaiting;
    Semaphore();



    bool isBusy() const;


    void setBusy();
    void setFree();


};


#endif //INC_322PROJECT2_SEMAPHORE_H
