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
    auto op_suma = expr.find('+');
    auto op_resta = expr.find('-');
    auto op_mult = expr.find('*');
    auto op_div = expr.find('/');
    auto pos = std::min(op_suma, op_resta);
    if (pos!=std::string::npos)
    {
        return new OpNode(expr[pos],
            Parse(expr.substr(0, pos)),
            Parse(expr.substr(pos + 1, expr.size())));
    }

    pos = std::min(op_mult, op_div);
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
    std::string expr = "10/2";
    ExpressionTree tree = BuildExpressionTree(expr);
    std::cout <<tree.compute()<< "\n";
}

// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

// Sugerencias para primeros pasos: 1. Use la ventana del Explorador de soluciones para agregar y administrar archivos
//   2. Use la ventana de Team Explorer para conectar con el control de código fuente
//   3. Use la ventana de salida para ver la salida de compilación y otros mensajes
//   4. Use la ventana Lista de errores para ver los errores
//   5. Vaya a Proyecto > Agregar nuevo elemento para crear nuevos archivos de código, o a Proyecto > Agregar elemento existente para agregar archivos de código existentes al proyecto
//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln
