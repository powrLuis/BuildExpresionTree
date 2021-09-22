// BuildExpresionTree.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <cassert>
#include <stack>
#include <queue>
#include <map>

struct ExprNode
{
    ExprNode* left{ nullptr };
    ExprNode* right{ nullptr };
    virtual double compute() = 0;
    virtual std::string to_string() = 0;
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
    std::string to_string() override { return std::string() + operator_; }
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
    std::string to_string() override { return std::to_string(value_); }
};


void tree_dfs(ExprNode* root) {
    std::stack<ExprNode*> stack;
    stack.push(root);

    while (!stack.empty()) {
        auto current = stack.top();
        stack.pop();
        if (!current)
            continue;
        std::cout << current->to_string() << std::endl;
        stack.push(current->right);
        stack.push(current->left);
    }
}

struct Item {
    ExprNode* node;
    int level;
};
void tree_bfs(ExprNode* root) {
    std::queue<Item> queue;

    queue.push(Item{.node= root,.level= 0 });
    std::map<int, std::vector<ExprNode*>> container;

    // std::map<ExprNode *> visited;
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();

        if (!current.node)
            continue;
        auto level = current.level;
        container[level].push_back(current.node);
        queue.push(Item{ .node = current.node->left, .level = level + 1 });
        queue.push(Item{ .node = current.node->right, .level = level + 1 });
    }

    for (auto iter = container.begin(); iter != container.end(); iter++) {
        auto arr = iter->second;
        std::cout << "level> " << iter->first << ": ";
        for (auto v : arr) {
            std::cout << v->to_string() << ",";
        }
        std::cout << "\n";
    }
}


//TAREA 1
void tree_dfs_in_order(ExprNode* root)
{
    std::stack<ExprNode*> stack;
    auto current = root;
    while (!stack.empty() || current!=nullptr) 
    {
        if (current)
        {
            stack.push(current);
            current = current->left;
        }
        else
        {
            current = stack.top();
            stack.pop();
            std::cout << current->to_string() << std::endl;
            current = current->right;
        }
    }
}


//TAREA 2
void tree_print_ITERATIVO(ExprNode* root,int level =0)
{
    std::stack<Item> pila;

    auto current = Item(root,level);
    while (!pila.empty()||current.node!=nullptr)
    {
        if (current.node!=nullptr)
        {
            pila.push(current);
            current = Item(current.node->left,current.level+1);
        }
        else
        {
            current = pila.top();
            pila.pop();
            for (auto i = 0; i < current.level; i++) {
                std::cout << "     ";
            }
            std::cout << current.node->to_string() << std::endl;
            current = Item(current.node->right,current.level+1);
        }
        
    }
}

void tree_print(ExprNode* root, int level = 0) {
    if (root) {
        tree_print(root->right, level + 1);
        for (auto i = 0; i < level; i++) {
            std::cout << "     ";
        }
        std::cout << root->to_string() << std::endl;
        tree_print(root->left, level + 1);
    }
}

struct ExpressionTree
{
    ExprNode* root_;
    ExpressionTree(ExprNode* root):root_(root){}
    double compute() { return root_->compute(); }
    void dfs() { tree_dfs_in_order(root_); }
    void bfs() { tree_bfs(root_); }
    void print() { return tree_print_ITERATIVO(root_); }
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
    std::string expr = "10-2*5+5/2+2";//4.5
   // std::string expr = "10-2";//8
    //std::string expr = "2*2*2/2";//4
    ExpressionTree tree = BuildExpressionTree(expr);
    tree.dfs();
    std::cout <<tree.compute()<< "\n";
}