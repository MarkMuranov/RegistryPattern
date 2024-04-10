//
// Created by Mark Muranov on 4/9/24.
//

#pragma once

#include <vector>

enum class Keywords {
    help, about, version, create, drop, database, table, row, insert // ...
};

class Tokenizer {
public:
    Tokenizer() = default;

    bool more() { return currentIndex + 1 < keywordList.size(); }
    Keywords current() { return keywordList[currentIndex]; }
    bool peekEquals(const Keywords keyword) {
        if (!more())
            return false;

        return keywordList[currentIndex + 1] == keyword;
    }

    std::vector<Keywords> keywordList;

private:
    size_t currentIndex = 0;

};