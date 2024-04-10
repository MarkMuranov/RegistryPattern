//
// Created by Mark Muranov on 4/9/24.
//

#pragma once

#include "Tokenizer.h"
#include <vector>
#include <memory>

class Handler {
public:
    virtual ~Handler() = default;

    bool find(Tokenizer& tokenizer) {
        if (canHandle(tokenizer))
            return handle();

        if (!next)
            return false;

        return next->find(tokenizer);
    }

    void setNext(const std::shared_ptr<Handler>& handler) { next = handler; }

    virtual bool canHandle(Tokenizer&) = 0;
    virtual bool handle(/* params */) = 0;

private:
    std::shared_ptr<Handler> next;

};

class ChainOfResponsibilities {
private:
    ChainOfResponsibilities() = default;

public:
    ChainOfResponsibilities(const ChainOfResponsibilities&) = delete;
    ChainOfResponsibilities& operator=(const ChainOfResponsibilities&) = delete;

    static ChainOfResponsibilities& getInstance() {
        static ChainOfResponsibilities instance;
        return instance;
    }

    template<class HandlerType>
    void registerHandler() {
        auto newHandler = std::shared_ptr<Handler>(new HandlerType);
        if (!firstHandler) {
            firstHandler = newHandler;
            lastHandler = newHandler;
        }
        else {
            lastHandler->setNext(newHandler);
            lastHandler = newHandler;
        }
    }

    bool handle(Tokenizer& tokenizer) {
        if (!firstHandler)
            return false;

        return firstHandler->find(tokenizer);
    }

private:
    std::shared_ptr<Handler> firstHandler, lastHandler;

};

template<class HandlerType>
struct RegisterHandler {
    RegisterHandler() {
        ChainOfResponsibilities::getInstance().registerHandler<HandlerType>();
    }
};