#pragma once

template <class T>
class Singleton {
public:
    Singleton& operator = (const Singleton&) = delete;
    Singleton& operator = (Singleton&&)      = delete;

    static T& getInstance() {
        if(!instance)
            instance = new TInstance;
        return *instance;
    }

protected:
    Singleton() {}

private:
    struct TInstance : T {
        TInstance() : T() {}
    };

    static inline T* instance = nullptr;
};
