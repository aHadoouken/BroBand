#pragma once

#include "IServer.h"

class Server : public IServer {
public:
    int Run() override;

private:
    void StartAccept() override;

    void HandleAccept() override;

    void HandleStop() override;
};