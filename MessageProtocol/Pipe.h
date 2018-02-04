#include <zconf.h>

#ifndef MESSAGEPROTOCOL_PIPE_H
#define MESSAGEPROTOCOL_PIPE_H


class Pipe {
public:
    Pipe() {
        pipe(this->fd);
        this->readFD = fd[0];
        this->writeFD = fd[1];
    }

    int getReadFd(){
        return this->readFD;
    }

    int getWriteFd(){
        return this->writeFD;
    }

private:
    int fd[2];
    int readFD;
    int writeFD;
};


#endif //MESSAGEPROTOCOL_PIPE_H

