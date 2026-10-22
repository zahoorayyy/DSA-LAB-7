#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
using namespace std;

struct Student {
    int studentID;
    string name;
    float GPA;
    Student* left;
    Student* right;

    Student(int id, const string& n, float g)
        : studentID(id), name(n), GPA(g), left(nullptr), right(nullptr) {}
};

//  Logic layer 
class StudentBST {
private:
    Student* root;

    Student* insertNode(Student* node, Student* s, bool& added) {
        if (!node) { added = true; return s; }
        if (s->studentID < node->studentID) node->left = insertNode(node->left, s, added);
        else if (s->studentID > node->studentID) node->right = insertNode(node->right, s, added);
        return node;
    }

    Student* searchNode(Student* node, int id) const {
        if (!node || node->studentID == id) return node;
        return id < node->studentID ? searchNode(node->left, id) : searchNode(node->right, id);
    }

    Student* minNode(Student* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    Student* deleteNode(Student* node, int id, bool& removed) {
        if (!node) return nullptr;
        if (id < node->studentID) node->left = deleteNode(node->left, id, removed);
        else if (id > node->studentID) node->right = deleteNode(node->right, id, removed);
        else {
            removed = true;
            if (!node->left || !node->right) {
                Student* child = node->left ? node->left : node->right;
                delete node;
                return child;
            }
            Student* succ = minNode(node->right);
            node->studentID = succ->studentID;
            node->name = succ->name;
            node->GPA = succ->GPA;
            bool dummy = false;
            node->right = deleteNode(node->right, succ->studentID, dummy);
        }
        return node;
    }

    void inorder(Student* node, vector<const Student*>& out) const {
        if (!node) return;
        inorder(node->left, out);
        out.push_back(node);
        inorder(node->right, out);
    }

    int countNodes(Student* node) const {
        return node ? 1 + countNodes(node->left) + countNodes(node->right) : 0;
    }

    void destroy(Student* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    StudentBST() : root(nullptr) {}
    ~StudentBST() { destroy(root); }
    StudentBST(const StudentBST&) = delete;
    StudentBST& operator=(const StudentBST&) = delete;

    bool Insert(int id, const string& name, float gpa) {
        bool added = false;
        Student* s = new Student(id, name, gpa);
        root = insertNode(root, s, added);
        if (!added) delete s;
        return added;
    }

    const Student* Search(int id) const { return searchNode(root, id); }

    bool Delete(int id) {
        bool removed = false;
        root = deleteNode(root, id, removed);
        return removed;
    }

    vector<const Student*> Inorder() const {
        vector<const Student*> out;
        inorder(root, out);
        return out;
    }

    int Count() const { return countNodes(root); }

    vector<const Student*> HighestGPA() const { return extremeGPA(true); }
    vector<const Student*> LowestGPA() const { return extremeGPA(false); }

    vector<const Student*> AboveThreshold(float threshold) const {
        vector<const Student*> result;
        for (const Student* s : Inorder())
            if (s->GPA > threshold) result.push_back(s);
        return result;
    }

private:
    vector<const Student*> extremeGPA(bool highest) const {
        vector<const Student*> all = Inorder(), result;
        if (all.empty()) return result;
        float best = all[0]->GPA;
        for (const Student* s : all)
            if (highest ? s->GPA > best : s->GPA < best) best = s->GPA;
        for (const Student* s : all)
            if (s->GPA == best) result.push_back(s);
        return result;
    }
};

// ---------------- Display layer ----------------
void printHeader() {
    cout << left << setw(10) << "ID" << setw(25) << "Name" << "GPA\n"
         << string(40, '-') << "\n";
}

void printStudent(const Student* s) {
    cout << left << setw(10) << s->studentID << setw(25) << s->name
         << fixed << setprecision(2) << s->GPA << "\n";
}

void printList(const vector<const Student*>& list, const string& emptyMsg) {
    if (list.empty()) { cout << emptyMsg << "\n"; return; }
    printHeader();
    for (const Student* s : list) printStudent(s);
}

void printMenu() {
    cout << "\n===== Student Record Management System =====\n"
         << "1. Insert student\n2. Search student by ID\n3. Delete student by ID\n"
         << "4. Display all students (sorted by ID)\n5. Total number of students\n"
         << "6. Highest and lowest GPA\n7. Students above GPA threshold\n0. Exit\n";
}

// Input helpers 
int readInt(const string& prompt) {
    int v;
    cout << prompt;
    while (!(cin >> v)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. " << prompt;
    }
    return v;
}

float readGPA(const string& prompt) {
    float v;
    cout << prompt;
    while (!(cin >> v) || v < 0.0f || v > 4.0f) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "GPA must be between 0.0 and 4.0. " << prompt;
    }
    return v;
}

void loadSampleData(StudentBST& bst) {
    bst.Insert(105, "Ayesha Khan", 3.60f);
    bst.Insert(102, "Bilal Ahmed", 2.80f);
    bst.Insert(110, "Hira Malik", 3.90f);
    bst.Insert(101, "Usman Tariq", 2.40f);
    bst.Insert(104, "Sara Noor", 3.20f);
    bst.Insert(108, "Ali Raza", 3.00f);
    bst.Insert(112, "Zainab Fatima", 3.75f);
}

int main() {
    StudentBST bst;
    loadSampleData(bst);
    int choice;

    do {
        printMenu();
        choice = readInt("Enter choice: ");
        switch (choice) {
        case 1: {
            int id = readInt("Student ID: ");
            cout << "Name: ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            string name;
            getline(cin, name);
            float gpa = readGPA("GPA (0.0 - 4.0): ");
            cout << (bst.Insert(id, name, gpa) ? "Student added.\n"
                                               : "Error: ID already exists.\n");
            break;
        }
        case 2: {
            const Student* s = bst.Search(readInt("Student ID to search: "));
            if (s) { printHeader(); printStudent(s); }
            else cout << "Student not found.\n";
            break;
        }
        case 3:
            cout << (bst.Delete(readInt("Student ID to delete: ")) ? "Student deleted.\n"
                                                                  : "Student not found.\n");
            break;
        case 4:
            printList(bst.Inorder(), "No records available.");
            break;
        case 5:
            cout << "Total students: " << bst.Count() << "\n";
            break;
        case 6:
            cout << "Highest GPA:\n";
            printList(bst.HighestGPA(), "No records available.");
            cout << "\nLowest GPA:\n";
            printList(bst.LowestGPA(), "No records available.");
            break;
        case 7: {
            float t = readGPA("Enter GPA threshold: ");
            printList(bst.AboveThreshold(t), "No students above this GPA.");
            break;
        }
        case 0:
            cout << "Goodbye.\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    return 0;
}