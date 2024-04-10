//
// Rick's COR example
//

#pragma once

#include <memory>
#include <string>
#include <iostream>

struct CORHandler : public std::enable_shared_from_this<CORHandler>
{
    using SharedHandler = std::shared_ptr<CORHandler>;
    CORHandler(SharedHandler aHandler = nullptr) : next(aHandler) {}

    virtual ~CORHandler() {}

    virtual SharedHandler find(const std::string &aStr) {
        if(canHandle(aStr))
            return shared_from_this();

        else if(next && next->find(aStr))
            return next;

        return SharedHandler();
    }

    virtual bool        canHandle(const std::string &aStr)=0;
    virtual CORHandler& handle(const std::string &aStr)=0;

    SharedHandler next;

};

struct BasicCmdHandler : public CORHandler
{
    BasicCmdHandler(SharedHandler aHandler  = nullptr) : CORHandler(aHandler) {}

    bool canHandle(const std::string &aStr) override {
        return aStr=="basic";
    }

    CORHandler& handle(const std::string &aStr) override {
        std::cout << "BasicController handle\n";
        return *this;

    }

};

struct AltCmdHandler : public CORHandler
{
    AltCmdHandler(SharedHandler aHandler = nullptr) : CORHandler(aHandler) {}

    bool canHandle(const std::string &aStr) override {
        return aStr=="alt";
    }

    CORHandler& handle(const std::string &aStr) override {
        std::cout << "AltController handle\n";
        return *this;
    }

};

int mainExample() {
    auto theBasic = std::make_shared<BasicCmdHandler>();
    auto theAlt = std::make_shared<AltCmdHandler>(theBasic);

    std::string temp("alt");

    if (auto theHandler = theAlt->find(temp))
        theHandler->handle(temp);

    return 0;
}