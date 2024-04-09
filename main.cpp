#include <iostream>
#include "ChainOfResponsibilities.h"
#include "Handlers.h"

int main() {
    Tokenizer tokenizer;
    //tokenizer.keywordList = { Keywords::create, Keywords::database };
    tokenizer.keywordList = { Keywords::help };

    auto& CORInstance = ChainOfResponsibilities::getInstance();
    //CORInstance.registerHandler<BasicCommandHandler>();
    //CORInstance.registerHandler<DatabaseCommandHandler>();
    CORInstance.handle(tokenizer);

    return 0;
}
