#pragma once

#include "IConnection.h"

class Connection : IConnection {
public:
    void Start() override;

private:
    void DoRead() override;

    void HandleRead() override;

    void HandleWrite() override;

    void DoClose() override;
};