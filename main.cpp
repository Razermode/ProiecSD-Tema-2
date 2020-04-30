#include <bits/stdc++.h>
#define RED false
#define BLACK true
using namespace std;

ifstream fin ("abce.in");
ofstream fout ("abce.out");

struct Node
{
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data = INT_MAX, Node *l = NULL, Node *r = NULL, Node *p = NULL, bool col = RED): data(data), color(col){
       left = l;
       right = r;
       parent = p;
    }
};

class RBTree
{
public:

    Node *root;
    void rotateLeft(Node *&root, Node *&pt);
    void rotateRight(Node *&root, Node *&pt);
    void fixViolation(Node *&root, Node *&pt);
    RBTree() { root = NULL; }
    Node *searchVal(const int n);
    void insert(const int n);
    void inorder(Node *root, int MIN_VAL, int MAX_VAL);
    void fixDoubleBlack(Node *x);
    void deleteNode(Node *v);
    int searchSmaller(Node* root, const int X);
    int searchBigger(Node* root, const int X);
};

Node *RBTree::searchVal(const int n)
{
    Node *temp = root;
    while (temp != NULL) {
      if (n < temp->data) {
        if (temp->left == NULL)
          break;
        else
          temp = temp->left;
      } else if (n == temp->data) {
        break;
      } else {
        if (temp->right == NULL)
          break;
        else
          temp = temp->right;
      }
    }

    return temp;
}

void RBTree::inorder(Node *root, int MIN_VAL, int MAX_VAL)
{
    if (root == NULL)
        return;

    inorder(root->left, MIN_VAL, MAX_VAL);
    if (root->data >= MIN_VAL && root->data <= MAX_VAL)
        fout << root->data << " ";
    inorder(root->right, MIN_VAL, MAX_VAL);
}

Node *BSTreplace(Node *x)
{

    if (x->left != NULL and x->right != NULL){
        Node *temp = x->right;
        while (temp->left != NULL)
            temp = temp->left;
        return temp;
    }

    if (x->left == NULL and x->right == NULL)
      return NULL;

    if (x->left != NULL)
      return x->left;
    else
      return x->right;
  }

Node* BSTInsert(Node* root, Node *pt)
{
    if (root == NULL)
        return pt;

    if (pt->data < root->data){
        root->left  = BSTInsert(root->left, pt);
        root->left->parent = root;
    }
    else if (pt->data > root->data){
        root->right = BSTInsert(root->right, pt);
        root->right->parent = root;
    }

    return root;
}


void RBTree::rotateLeft(Node *&root, Node *&pt)
{
    Node *pt_right = pt->right;
    pt->right = pt_right->left;

    if (pt->right != NULL)
        pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_right;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_right;

    else
        pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
}

void RBTree::rotateRight(Node *&root, Node *&pt)
{
    Node *pt_left = pt->left;

    pt->left = pt_left->right;

    if (pt->left != NULL)
        pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == NULL)
        root = pt_left;

    else if (pt == pt->parent->left)
        pt->parent->left = pt_left;

    else
        pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
}


void RBTree::fixViolation(Node *&root, Node *&pt)
{
    Node *parent_pt = NULL;
    Node *grand_parent_pt = NULL;

    while ((pt != root) && (pt->color != BLACK) && (pt->parent->color == RED)) {
        parent_pt = pt->parent;
        grand_parent_pt = pt->parent->parent;

        if (parent_pt == grand_parent_pt->left) {
            Node *uncle_pt = grand_parent_pt->right;

            if (uncle_pt != NULL && uncle_pt->color == RED) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }

            else {
                if (pt == parent_pt->right) {
                    rotateLeft(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateRight(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }

        else {
            Node *uncle_pt = grand_parent_pt->left;

            if ((uncle_pt != NULL) && (uncle_pt->color == RED)) {
                grand_parent_pt->color = RED;
                parent_pt->color = BLACK;
                uncle_pt->color = BLACK;
                pt = grand_parent_pt;
            }
            else {
                if (pt == parent_pt->left) {
                    rotateRight(root, parent_pt);
                    pt = parent_pt;
                    parent_pt = pt->parent;
                }
                rotateLeft(root, grand_parent_pt);
                swap(parent_pt->color, grand_parent_pt->color);
                pt = parent_pt;
            }
        }
    }

    root->color = BLACK;
}

void RBTree::insert(const int data)
{
    Node *pt = new Node(data);
    if (!root) {
         root = BSTInsert(root, pt);
         fixViolation(root, pt);
    }
    else if (searchVal(data)->data != data) {
        root = BSTInsert(root, pt);
        fixViolation(root, pt);
    }
}

void RBTree::fixDoubleBlack(Node *x)
{
    if (x == root)
      return;

    Node *parent = x->parent;
    Node *sibling;
    if (x == parent->left)
        sibling = parent->right;
    else
        sibling = parent->left;

    if (sibling == NULL) {
      fixDoubleBlack(parent);
    } else {
      if (sibling->color == RED) {
        parent->color = RED;
        sibling->color = BLACK;
        if (sibling == sibling->parent->left) {
          rotateRight(root, parent);
        } else {
          rotateLeft(root, parent);
        }
        fixDoubleBlack(x);
      } else {
        if ((sibling->left != NULL && sibling->left->color == RED) || (sibling->right != NULL && sibling->right->color == RED)) {
          if (sibling->left != NULL and sibling->left->color == RED) {
            if (sibling == sibling->parent->left) {
              sibling->left->color = sibling->color;
              sibling->color = parent->color;
              rotateRight(root, parent);
            } else {
              sibling->left->color = parent->color;
              rotateRight(root, sibling);
              rotateLeft(root, parent);
            }
          } else {
            if (sibling == sibling->parent->left) {
              sibling->right->color = parent->color;
              rotateLeft(root, sibling);
              rotateRight(root, parent);
            } else {
              sibling->right->color = sibling->color;
              sibling->color = parent->color;
              rotateLeft(root, parent);
            }
          }
          parent->color = BLACK;
        } else {
          sibling->color = RED;
          if (parent->color == BLACK)
            fixDoubleBlack(parent);
          else
            parent->color = BLACK;
        }
      }
    }
  }

void RBTree::deleteNode(Node *v)
{
    Node *u = BSTreplace(v);
    bool uvBlack = ((u == NULL or u->color == BLACK) and (v->color == BLACK));
    Node *parent = v->parent;

    if (u == NULL) {
      if (v == root) {
        root = NULL;
      } else {
            Node *v_sibling;
            if (v == v->parent->left)
                v_sibling = v->parent->right;
            else
                v_sibling = v->parent->left;
        if (uvBlack) {
          fixDoubleBlack(v);
        } else {
          if (v_sibling != NULL)
            v_sibling->color = RED;
        }

        if (v == v->parent->left) {
          parent->left = NULL;
        } else {
          parent->right = NULL;
        }
      }
      delete v;
      return;
    }

    if (v->left == NULL or v->right == NULL) {
      if (v == root) {
        v->data = u->data;
        v->left = v->right = NULL;
        delete u;
      } else {
        if (v == v->parent->left) {
          parent->left = u;
        } else {
          parent->right = u;
        }
        delete v;
        u->parent = parent;
        if (uvBlack) {
          fixDoubleBlack(u);
        } else {
          u->color = BLACK;
        }
      }
      return;
    }
    int var = u->data;
    u->data = v->data;
    v->data = var;
    deleteNode(u);
}

int RBTree::searchSmaller(Node* root, int X)
{
    if (root == NULL)
        return -INT_MAX;

    if (root->data == X)
        return X;

    if (root->data < X) {
        int k = searchSmaller(root->right, X);
        if (k == -INT_MAX)
            return root->data;
        return k;
    }

    return searchSmaller(root->left, X);
}

int RBTree::searchBigger(Node* root, int X)
{
   if (root == NULL)
        return -INT_MAX;

   if (root->data == X)
        return X;

    if(root->data > X) {
        int k = searchBigger(root->left, X);
        if (k == -INT_MAX)
            return root->data;
        return k;
    }

    return searchBigger(root->right, X);
}



int main()
{

    RBTree Solve;
    int Q, X, Y, T;
    fin >> Q;
    for (int i = 0; i < Q; ++i) {
        fin >> T >> X;
        if (T == 1)
            Solve.insert(X);

        if (T == 2) {
            Node* deleted = Solve.searchVal(X);
            if(deleted->data == X)
                Solve.deleteNode(deleted);
        }
        if (T == 3) {
            Node* searching = Solve.searchVal(X);
            if (searching == NULL)
                fout << 0 << '\n';
            else if (Solve.searchVal(X)->data == X)
                    fout << 1 << '\n';
                else
                    fout << 0 << '\n';
        }
        if (T == 4)
            fout << Solve.searchSmaller(Solve.root, X) << '\n';

        if (T == 5)
            fout << Solve.searchBigger(Solve.root, X) << '\n';

        if (T == 6) {
            fin >> Y;
            Solve.inorder(Solve.root, X, Y);
            fout << '\n';
        }
    }

    return 0;
}
