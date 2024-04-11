#include <iostream>
#include "ChainOfResponsibilities.h"
#include "Handlers.h"

int main() {
    Tokenizer tokenizer;
    tokenizer.keywordList = { Keywords::create, Keywords::database };
    //tokenizer.keywordList = { Keywords::help };

    ChainOfResponsibilities::getInstance().handle(tokenizer);

    return 0;
}
