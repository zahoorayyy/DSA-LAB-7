#include <iostream>
#include <string>
#include <stack>
#include <vector>
using namespace std;

// Node of the character binary tree
struct Node {
char data;
Node *left, *right;
Node(char c) : data(c), left(NULL), right(NULL) {}
};

// 1. Build the tree from a string such as a(b(d,e),c(,f(g,)))
//    Grammar:  node := letter [ '(' [node] ',' [node] ')' ]

Node *build(const string &s, size_t &i) {
if (i >= s.size() || !isalpha(s[i])) return NULL;
Node *root = new Node(s[i++]);
if (i < s.size() && s[i] == '(') {
i++;                                        // skip '('
if (s[i] != ',') root->left = build(s, i);  // empty left child allowed
i++;                                        // skip ','
if (s[i] != ')') root->right = build(s, i); // empty right child allowed
i++;                                        // skip ')'
}
return root;
}

Node *buildTree(const string &s) {
size_t i = 0;
return build(s, i);
}


// 2. Preorder (recursive): fills a string with the visit order
void preorder(Node *root, string &out) {
if (!root) return;
out += root->data;
out += ' ';
preorder(root->left, out);
preorder(root->right, out);
}

// 3. Inorder (iterative, using a stack)
string inorderIterative(Node *root) {
string out;
stack<Node *> st;
Node *cur = root;
while (cur || !st.empty()) {
while (cur) {               // go as far left as possible
st.push(cur);
cur = cur->left;
}
cur = st.top(); st.pop();
out += cur->data;
out += ' ';
cur = cur->right;           // then visit the right subtree
}
return out;
}


// 4. Postorder (iterative, using a stack and a last-visited pointer)

string postorderIterative(Node *root) {
string out;
stack<Node *> st;
Node *cur = root, *lastVisited = NULL;
while (cur || !st.empty()) {
if (cur) {
st.push(cur);
cur = cur->left;
} else {
Node *top = st.top();
if (top->right && lastVisited != top->right) {
cur = top->right;   // right subtree not done yet
} else {
out += top->data;
out += ' ';
lastVisited = top;
st.pop();
}
}
}
return out;
}


// 5. Validate the string WITHOUT building the tree
bool validNode(const string &s, size_t &i) {
if (i >= s.size() || !isalpha(s[i])) return false;   // node must be a letter
i++;
if (i < s.size() && s[i] == '(') {
i++;
if (i < s.size() && s[i] != ',') {               // optional left child
if (!validNode(s, i)) return false;
}
if (i >= s.size() || s[i] != ',') return false;  // comma is compulsory
i++;
if (i < s.size() && s[i] != ')') {               // optional right child
if (!validNode(s, i)) return false;
}
if (i >= s.size() || s[i] != ')') return false;  // closing bracket
i++;
}
return true;
}

bool isValidTreeString(const string &s) {
int balance = 0;                                     // balanced parentheses check
for (char c : s) {
if (c == '(') balance++;
else if (c == ')' && --balance < 0) return false;
}
if (balance != 0) return false;
size_t i = 0;
return validNode(s, i) && i == s.size();             // child placement check
}


// 6. Height of the tree and depth of every node (recursive)
int height(Node *root) {                                 // height in edges
if (!root) return -1;
int l = height(root->left), r = height(root->right);
return 1 + (l > r ? l : r);
}

void printDepths(Node *root, int depth) {
if (!root) return;
cout << "  Node " << root->data << " -> depth " << depth << endl;
printDepths(root->left, depth + 1);
printDepths(root->right, depth + 1);
}


// 7. Generate a BST from a list of integers

struct BSTNode {
int key;
BSTNode *left, *right;
BSTNode(int k) : key(k), left(NULL), right(NULL) {}
};

BSTNode *insertBST(BSTNode *root, int key) {
if (!root) return new BSTNode(key);
if (key < root->key) root->left = insertBST(root->left, key);
else if (key > root->key) root->right = insertBST(root->right, key);
return root;                                         // duplicates ignored
}

BSTNode *bstFromList(const vector<int> &v) {
BSTNode *root = NULL;
for (int x : v) root = insertBST(root, x);           // first element becomes root
return root;
}

void inorderBST(BSTNode *root) {
if (!root) return;
inorderBST(root->left);
cout << root->key << " ";
inorderBST(root->right);
}


// Memory clean-up
void freeTree(Node *r) {
if (!r) return;
freeTree(r->left); freeTree(r->right); delete r;
}
void freeBST(BSTNode *r) {
if (!r) return;
freeBST(r->left); freeBST(r->right); delete r;
}

int main() {
string s = "a(b(d,e),c(,f(g,)))";
cout << "Tree string : " << s << endl;

// Task 5 first: only build if the string is valid
cout << "Valid tree string? " << (isValidTreeString(s) ? "Yes" : "No") << endl;
cout << "Valid \"a(b,c\"?    " << (isValidTreeString("a(b,c") ? "Yes" : "No") << endl;
cout << "Valid \"a(b c)\"?   " << (isValidTreeString("a(b c)") ? "Yes" : "No") << endl;
if (!isValidTreeString(s)) return 0;

Node *root = buildTree(s);                           // Task 1

string pre;
preorder(root, pre);
cout << "\nPreorder  (recursive) : " << pre << endl;  // Task 2
cout << "Inorder   (stack)     : " << inorderIterative(root) << endl;   // Task 3
cout << "Postorder (iterative) : " << postorderIterative(root) << endl; // Task 4

cout << "\nHeight of the tree (edges): " << height(root) << endl;        // Task 6
cout << "Depth of each node:" << endl;
printDepths(root, 0);

vector<int> nums = {50, 30, 70, 20, 40, 60, 80};     // Task 7
BSTNode *bst = bstFromList(nums);
cout << "\nBST inorder traversal: ";
inorderBST(bst);
cout << endl;

freeTree(root);
freeBST(bst);
return 0;
}
