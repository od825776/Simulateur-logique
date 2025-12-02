#pragma once
#include <vector>
#include <string>

struct Token {
    std::string value;
};

class ExpressionTokenizer {
public:
    std::vector<Token> Tokenize(const std::string& expr) {
        std::vector<Token> tokens;
        std::string current;

        for (size_t i = 0; i < expr.length(); ++i) {
            char c = expr[i];
            if (c == ' ' || c == '\t') { if (!current.empty()) { tokens.push_back({ current }); current = ""; } continue; }
            if (c == '(' || c == ')') { if (!current.empty()) { tokens.push_back({ current }); current = ""; } tokens.push_back({ std::string(1,c) }); continue; }
            if (isalpha(c) || isdigit(c)) current += c;
            else { if (!current.empty()) { tokens.push_back({ current }); current = ""; } tokens.push_back({ std::string(1,c) }); }
        }
        if (!current.empty()) tokens.push_back({ current });
        return tokens;
    }
};
#pragma once
#pragma once
