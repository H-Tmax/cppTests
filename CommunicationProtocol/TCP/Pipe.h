#include <zconf.h>

#ifndef MESSAGEPROTOCOL_PIPE_H
#define MESSAGEPROTOCOL_PIPE_H

//TODO: OPEN PIPE WITH PIPE_BUF_IPARAM

class Pipe {
public:
    Pipe() {
        pipe(this->fd);
        this->readFD = fd[0];
        this->writeFD = fd[1];
    }

    int getReadFd() const{
        return this->readFD;
    }

    int getWriteFd() const {
        return this->writeFD;
    }

private:
    int fd[2];
    int readFD;
    int writeFD;
};


#endif //MESSAGEPROTOCOL_PIPE_H

