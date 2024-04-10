//
// Created by Mark Muranov on 4/9/24.
//

#pragma once

#include "ChainOfResponsibilities.h"
#include <iostream>

class BasicCommandHandler : public Handler {
public:
    bool canHandle(Tokenizer& tokenizer) override {
        return tokenizer.current() == Keywords::help
            || tokenizer.current() == Keywords::version
            || tokenizer.current() == Keywords::about;
    }

    bool handle() override {
        std::cout << "Basic command!\n";
        return true;
    }

};

class DatabaseCommandHandler : public Handler {
public:
    bool canHandle(Tokenizer& tokenizer) override {
        return tokenizer.peekEquals(Keywords::database);
    }

    bool handle() override {
        std::cout << "Database command!\n";
        return true;
    }

};

//inline const RegisterHandler<BasicCommandHandler> registerBasicHandler;
//inline const RegisterHandler<DatabaseCommandHandler> registerDatabaseHandler;
