#pragma once

#include "IConnection.h"

class Connection : IConnection {
public:
    int Start() override;

private:
    void DoRead() override;

    void HandleRead() override;

    void HandleWrite() override;

    void DoClose() override;
};