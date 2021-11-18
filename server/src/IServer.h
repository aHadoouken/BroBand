#pragma once

class IServer {
public:
    virtual int Run() = 0;

private:
    virtual void StartAccept() = 0;

    virtual void HandleAccept() = 0;

    virtual void HandleStop() = 0;
};
