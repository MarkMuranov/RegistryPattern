//
// Created by Mark Muranov on 4/9/24.
//

#pragma once

#include "Tokenizer.h"
#include <vector>

class Handler {
public:
    virtual ~Handler() {}

    bool handle(Tokenizer& tokenizer) {
        if (canHandle(tokenizer))
            return execute();

        if (!next)
            return false;

        return next->handle(tokenizer);
    }

    void setNext(Handler* handler) { next = handler; }

    virtual bool canHandle(Tokenizer&) = 0;
    virtual bool execute(/* params... */) = 0;

private:
    Handler* next = nullptr;

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

        auto* newHandler = new HandlerType;
        if (!handlerList.empty())
            handlerList.back()->setNext(newHandler);

        handlerList.emplace_back(newHandler);
    }

    bool handle(Tokenizer& tokenizer) {
        if (handlerList.empty())
            return false;

        return handlerList[0]->handle(tokenizer);
    }

private:
    std::vector<std::unique_ptr<Handler>> handlerList;

};

template<class HandlerType>
struct RegisterHandler {
    RegisterHandler() {
        ChainOfResponsibilities::getInstance().registerHandler<HandlerType>();
    }
};