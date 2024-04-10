#include <iostream>
#include "ChainOfResponsibilities.h"
#include "Handlers.h"

int main() {
    Tokenizer tokenizer;
    tokenizer.keywordList = { Keywords::create, Keywords::database };
    //tokenizer.keywordList = { Keywords::help };

    // OLD WAY
    //auto& CORInstance = ChainOfResponsibilities::getInstance();
    //CORInstance.registerHandler<BasicCommandHandler>();
    //CORInstance.registerHandler<DatabaseCommandHandler>();

    // NEW WAY
    ChainOfResponsibilities::getInstance().handle(tokenizer);

    return 0;
}
