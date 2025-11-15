#include <iostream>

using namespace std;

enum Color {RED, BLACK};

class rbTree{

public:
    struct rbNode
    {
        Color color;
        int key;
        rbNode* left;
        rbNode* right;
        rbNode* parent;

        rbNode(int k) 
            : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {} 
        
        std::string colorName() const
        {
            return color == RED ? "RED" : "BLACK";
        }    
    };

    rbNode* rbRoot = nullptr;
    
    bool hasRedRed(rbNode* node) const
    {
        if(!node) return false;

        if(node->color == RED)
        {
            if((node->left && node->left->color == RED)  ||
                (node->right && node->right->color == RED))
                return true;
        }
        return hasRedRed(node->left) || hasRedRed(node->right);
    }

    void leftRotate(rbNode* x)
    {
        rbNode* y = x->right;
        x->right = y->left;
        if(y->left) { y->left->parent = x;}
        y->parent = x->parent;
        if(x->parent == nullptr) { rbRoot = y;}
        else if(x->parent->left == x) { x->parent->left = y;}
        else { x->parent->right = y;}
        y->left = x;
        x->parent = y;
    }

    void rightRotate(rbNode* x)
    {
        rbNode* y = x->left;
        x->left = y->right;
        if(y->right) { y->right->parent = x;}
        y->parent = x->parent;
        if(x->parent == nullptr) { rbRoot = y;}
        else if(x == x->parent->left) { x->parent->left = y;}
        else { x->parent->right = y;}
        y->right = x;
        x->parent = y;    
    }

    void fixViolation(rbNode*& root, rbNode* z)
    {
        while(z != root && z->parent->color == RED)
        {
            rbNode* p = z->parent;
            rbNode* g = p->parent;

            if(p == g->left)
            {
                rbNode* u = g->right;
                if(u && u->color == RED)
                {     
                    p->color = BLACK;
                    u->color = BLACK;
                    g->color = RED;
                    z = g;
                }
                else
                {
                    if(z == p->right)
                    {
                        z = p;
                        leftRotate(p);
                    }
                    p->color = BLACK;
                    g->color = RED;
                    rightRotate(g);
                }
            }
            else
            {
                rbNode* u = g->left;
                if(u && u->color == RED)
                {
                    u->color = BLACK;
                    p->color = BLACK;
                    g->color = RED;
                    z = g;
                }
                else
                {
                    if(z == p->left)
                    {
                        z = p;
                        rightRotate(p);
                    }
                    p->color = BLACK;
                    g->color = RED;
                    leftRotate(g);
                }
            }
        }
        root->color = BLACK;
    }

    void insert(int key)
    {
        rbNode* z = new rbNode(key);
        rbNode* x = rbRoot;
        rbNode* y = nullptr;
        if(!rbRoot) 
        {
            z->color = BLACK;
            rbRoot = z;
            return;
        }
        while(x)
        {
            y = x;
            if(z->key < x->key) { x = x->left;}
            else { x = x->right;}
        }
        z->parent = y;
        if(z->key < y->key) { y->left = z;}
        else {y->right = z;}
        fixViolation(rbRoot, z);
    }

    void transplant(rbNode* a, rbNode* b)
    {
        if(!a->parent) { rbRoot = b;}
        else if(a == a->parent->left) { a->parent->left = b;}
        else { a->parent->right = b;}
        if(b) { b->parent = a->parent;}
    }

    rbNode* find(int key) const
    {
        rbNode* x = rbRoot;
        if(key == x->key) { return x;}
        while(x && x->key != key)
        {
            if(key < x->key) { x = x->left;}
            else { x = x->right;}
        }
        return x;
    }

    rbNode* min_node(rbNode* node)
    {
        while(node && node->left) { node = node->left;}
        return node;
    }
    void delete_node(int key)
    {
        rbNode* z = find(key);
        if(!z) return;

        rbNode* y = z;
        Color yColor = y->color;
        rbNode* x = nullptr;
        
        if(z->left == nullptr)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if(z->right == nullptr)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = min_node(z->right);
            yColor = y->color;
            x = y->right;
            
            if(y->parent == z)
            {
                if(x) { x->parent = y;}
            }
            else
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y; 
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            yColor = z->color;
        }
        delete z;
        if(yColor == BLACK)
        {
            fixDelete(x);
        }
    }

    void fixDelete(rbNode* x)
{
    while (x != rbRoot && (!x || x->color == BLACK))
    {
        rbNode* p = x ? x->parent : nullptr;
        if (!p) break;

        bool xIsLeft = (p->left == x);
        rbNode* w = xIsLeft ? p->right : p->left; 
        if (w && w->color == RED)
        {
            w->color = BLACK;
            p->color = RED;
            if (xIsLeft) leftRotate(p);
            else         rightRotate(p);
            w = xIsLeft ? p->right : p->left;
        }
        rbNode* wLeft  = w ? w->left  : nullptr;
        rbNode* wRight = w ? w->right : nullptr;

        bool leftBlack  = (!wLeft  || wLeft->color  == BLACK);
        bool rightBlack = (!wRight || wRight->color == BLACK);

        if (leftBlack && rightBlack)
        {
            if (w) w->color = RED;
            x = p; 
        }

        else if (xIsLeft && rightBlack)
        {
            if (wLeft) wLeft->color = BLACK;
            if (w) w->color = RED;
            rightRotate(w);
            w = p->right;
        }
        else if (!xIsLeft && leftBlack)
        {
            if (wRight) wRight->color = BLACK;
            if (w) w->color = RED;
            leftRotate(w);
            w = p->left;
        }
        if (w)
        {
            w->color = p->color;
            p->color = BLACK;
            if (xIsLeft)
            {
                if (w->right) w->right->color = BLACK;
                leftRotate(p);
            }
            else
            {
                if (w->left) w->left->color = BLACK;
                rightRotate(p);
            }
        }

        x = rbRoot;
    }

    if (x) x->color = BLACK;
}
};
