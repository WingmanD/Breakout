#pragma once

class GameMode {
public:
    virtual void start() = 0;
    virtual void end() = 0;

    virtual ~GameMode() = default;
};
