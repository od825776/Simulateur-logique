#pragma once
#include "Node.h"
#include "ExpressionTokenizer.h"
#include <vector>
#include <string>
#include <map>
#include <stdexcept>

class ExpressionParser {
public:
    ExpressionParser(const std::vector<Token>& t, const std::map<CString, bool>& vars)
        : tokens(t), pos(0), variables(vars) {
    }

    Node* parse() { return parseExpression(); }

private:
    std::vector<Token> tokens;
    size_t pos;
    std::map<CString, bool> variables;

    std::string currentToken() { return pos < tokens.size() ? tokens[pos].value : ""; }
    void nextToken() { if (pos < tokens.size()) pos++; }

    Node* parseExpression() {
        Node* left = parseTerm();
        while (currentToken() == "OR" || currentToken() == "XOR") {
            std::string op = currentToken();
            nextToken();
            Node* right = parseTerm();
            left = new GateNode(CString(op.c_str()), left, right);
        }
        return left;
    }

    Node* parseTerm() {
        Node* left = parseFactor();
        while (currentToken() == "AND") {
            nextToken();
            Node* right = parseFactor();
            left = new GateNode(_T("AND"), left, right);
        }
        return left;
    }

    Node* parseFactor() {
        if (currentToken() == "NOT") {
            nextToken();
            Node* child = parseFactor();
            return new GateNode(_T("NOT"), child);
        }
        else if (currentToken() == "(") {
            nextToken();
            Node* node = parseExpression();
            if (currentToken() != ")") throw std::runtime_error("Parenthèse fermante attendue");
            nextToken();
            return node;
        }
        else {
            CString var(currentToken().c_str());
            if (variables.find(var) == variables.end()) throw std::runtime_error("Variable non définie");
            Node* leaf = new InputNode(var, variables[var]);
            nextToken();
            return leaf;
        }
    }
};
#pragma once
#pragma once
