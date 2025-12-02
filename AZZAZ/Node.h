#pragma once
#include <afx.h>

// --------------------------------------------------
// Classe de base Node
// --------------------------------------------------
class Node
{
public:
    virtual bool evaluate() = 0;
    virtual ~Node() {}
};

// --------------------------------------------------
// Noeud d'entrée (X, Y, Z)
// --------------------------------------------------
class InputNode : public Node
{
public:
    CString name;
    bool value;

    InputNode(const CString& n, bool v)
        : name(n), value(v) {
    }

    bool evaluate() override { return value; }
};

// --------------------------------------------------
// Noeud porte logique (AND, OR, XOR, NOT)
// --------------------------------------------------
class GateNode : public Node
{
public:
    CString type;
    Node* left;
    Node* right;

    GateNode(const CString& t, Node* l, Node* r = nullptr)
        : type(t), left(l), right(r) {
    }

    bool evaluate() override
    {
        if (type == _T("AND")) return left->evaluate() && right->evaluate();
        if (type == _T("OR"))  return left->evaluate() || right->evaluate();
        if (type == _T("XOR")) return left->evaluate() ^ right->evaluate();
        if (type == _T("NOT")) return !left->evaluate();
        return false;
    }
};

// --------------------------------------------------
// Noeud de sortie S
// --------------------------------------------------
class OutputNode : public Node
{
public:
    CString name;
    Node* input;

    OutputNode(const CString& n, Node* in)
        : name(n), input(in) {
    }

    bool evaluate() override
    {
        return input ? input->evaluate() : false;
    }
};
