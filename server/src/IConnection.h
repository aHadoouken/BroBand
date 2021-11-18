#pragma once

class IConnection {
public:
    virtual int Start() = 0;

private:
    virtual void DoRead() = 0;

    virtual void HandleRead() = 0;

    virtual void HandleWrite() = 0;

    virtual void DoClose() = 0;
};
