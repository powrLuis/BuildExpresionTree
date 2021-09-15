// BuildExpresionTree.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <cassert>

struct ExprNode
{
    ExprNode* left{ nullptr };
    ExprNode* right{ nullptr };
    virtual double compute() = 0;
};

struct OpNode:ExprNode
{
    char operator_;
    OpNode(char op, ExprNode* left, ExprNode* right) :ExprNode(), operator_(op)
    {
        this->left = left;
        this->right = right;
    }
    double compute() override
    {
        auto lhs = this->left->compute();
        auto rhs = this->right->compute();
        switch (operator_)
        {
        case '+':
            return lhs + rhs;
        case'-':
            return lhs - rhs;
        case '*':
            return lhs * rhs;
        case '/':
            return lhs / rhs;
        default:
            assert(false);
            break;
        }
    }
};
//// DFS -> stack -> LIFO
//post-order = [travel(travel.left) + travel(travel.right) + root] 
//pre-order = [root + travel(travel.left) + travel(travel.right)] 
//in-order = [travel(travel.left) + root + travel(travel.right)]

// BFS -> queue -> FIFO
// level transversal
// => +, 
// * -
// 5, 3, 2, /
// 6 3

struct NumberNode:ExprNode
{
    double value_;
    NumberNode(double value) :ExprNode(), value_(value){}
    double compute() override { return value_; }
};

struct ExpressionTree
{
    ExprNode* root_;
    ExpressionTree(ExprNode* root):root_(root){}
    double compute() { return root_->compute(); }
};

ExprNode* Parse(std::string expr)
{
    auto pos = expr.find_last_of("+-");
    if (pos!=std::string::npos)
    {
        return new OpNode(expr[pos],
            Parse(expr.substr(0, pos)),
            Parse(expr.substr(pos + 1, expr.size())));
    }

    pos = expr.find_last_of("*/");
    if (pos != std::string::npos)
    {
        return new OpNode(expr[pos],
            Parse(expr.substr(0, pos)),
            Parse(expr.substr(pos + 1, expr.size())));
    }

    return new NumberNode(std::stod(expr));
}


ExpressionTree BuildExpressionTree(std::string expr)
{
    ExprNode* root = Parse(expr);
    return ExpressionTree(root);
}

int main()
{
    //std::string expr = "10-2*5+5/2+2";//4.5
    //std::string expr = "10-2";//8
    std::string expr = "2*2*2/2";//4
    ExpressionTree tree = BuildExpressionTree(expr);
    std::cout <<tree.compute()<< "\n";
}