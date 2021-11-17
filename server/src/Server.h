#pragma once

#include "IServer.h"

class Server : public IServer {
public:
    void Run() override;

private:
    void StartAccept() override;

    void HandleAccept() override;

    void HandleStop() override;
};