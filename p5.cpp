#include <iostream>
#include <fstream>

#include <vector>

using namespace std;

// class definitions

class Node {
public:
    int data;

    Node* left;
    Node* right;

    Node();
    Node(int data);
};

class HuffmanTree {
public:
    Node* root;
    vector<int> inOrder;
    vector<int> levelOrder;

    void populateArrays(ifstream& inOrderFile, ifstream& levelOrderFile);
    Node* buildTree(vector<int> const &inOrder, vector<int> const &levelOrder, int parent, string side);
    const void printArrays();
};


// class declarations

Node::Node() {
    this->data = -1;
}

Node::Node(int data) {
    this->data = data;
}

void HuffmanTree::populateArrays(ifstream& inOrderFile, ifstream& levelOrderFile) {
    int in;
    int level;
    inOrderFile >> in;
    levelOrderFile >> level;
    
    while (!inOrderFile.eof()) {
        this->inOrder.push_back(in);
        this->levelOrder.push_back(level);
        inOrderFile >> in;
        levelOrderFile >> level;  
    }
}

const void HuffmanTree::printArrays() {
    int size = this->inOrder.size();

    cout << "In order:    [";
    for (int i = 0; i < size; i ++) {
        cout << this->inOrder.at(i);
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl;
    cout << "Level order: [";
    for (int i = 0; i < size; i ++) {
        cout << this->levelOrder.at(i);
        if (i < size - 1) {
            cout << ", ";
        }
    }
    cout << "]" << endl << endl;
}

Node* HuffmanTree::buildTree(vector<int> const &inOrder, vector<int> const &levelOrder, int parent, string side) {
    int size = levelOrder.size();

    // test
    // cout << "Parent node: " << parent<< ", side: " << side << endl;
    // cout << "In order: ";
    // for (int i = 0; i < size; i ++) {
    //     cout << inOrder.at(i) << ", ";
    // }
    // cout << endl;

    // cout << "Level order: ";
    // for (int i = 0; i < size; i ++) {
    //     cout << levelOrder.at(i) << ", ";
    // }
    // cout << endl << endl;

    // if inputting an empty array
    if (size == 0) {
        return nullptr;
    }
    
    int target = levelOrder.at(0);
    bool left = true;

    vector<int> leftInOrder;
    vector<int> leftLevelOrder;
    vector<int> rightInOrder;
    vector<int> rightLevelOrder;

    
    for (int i = 0; i < size; i ++) {
        if (inOrder.at(i) == target) {
            left = false;
            continue;
        }
        if (left) {
            leftInOrder.push_back(inOrder.at(i));
        } else {
            rightInOrder.push_back(inOrder.at(i));
        }
    }

    int leftSize = leftInOrder.size();
    int rightSize = rightInOrder.size();
    
    // for each element in the level order, check if its in the in order of each side, and if it is, add it to that sides level order
    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < leftSize; j ++) {
            if (levelOrder.at(i) == leftInOrder.at(j)) {
                leftLevelOrder.push_back(levelOrder.at(i));
                break;
            }
        }
    }

    for (int i = 0; i < size; i ++) {
        for (int j = 0; j < rightSize; j ++) {
            if (levelOrder.at(i) == rightInOrder.at(j)) {
                rightLevelOrder.push_back(levelOrder.at(i));
                break;
            }
        }
    }
            
    // cout << "Left In-Order: ";
    // for (int i = 0; i < leftSize; i ++) {
    //     cout << leftInOrder.at(i) << ", ";
    // }
    // cout << endl;

    // cout << "Left Level-Order: ";
    // for (int i = 0; i < leftSize; i ++) {
    //     cout << leftLevelOrder.at(i) << ", ";
    // }
    // cout << endl;

    // cout << "Right In-Order: ";
    // for (int i = 0; i < rightSize; i ++) {
    //     cout << rightInOrder.at(i) << ", ";
    // }
    // cout << endl;

    // cout << "Right Level-Order: ";
    // for (int i = 0; i < rightSize; i ++) {
    //     cout << rightLevelOrder.at(i) << ", ";
    // }
    // cout << endl;
    
    Node* newNode = new Node(levelOrder.at(0));

    cout << "Created node " << levelOrder.at(0) << " as the " << side << " child to " << parent << endl;
    

    if (leftInOrder.size() > 0) {
        newNode->left = this->buildTree(leftInOrder, leftLevelOrder, levelOrder.at(0), "left");
    } 

    if (rightInOrder.size() > 0) {
        newNode->right = this->buildTree(rightInOrder, rightLevelOrder, levelOrder.at(0), "right");
    }

    return newNode;
}

const void printInOrder(Node* root) {
    cout<< "lets print some shit" << endl;
    if (root == NULL)
        return;
    printInOrder(root->left);
    if(root && root->data){
       cout << root->data << " " << endl; 
    }
    else{
        cout << "bro u wrong" << endl;
        return;
    }
    
    printInOrder(root->right);
}

int main (int argc, char* argv[]) {
    // open input files

    if (argc != 4) {
        cout << "Error: incorrect number of command line arguments." << endl;
        return 1;
    }

    ifstream inOrderFile;
    ifstream levelOrderFile;
    ifstream encodedFile;

    inOrderFile.open(argv[1]);
    levelOrderFile.open(argv[2]);
    encodedFile.open(argv[3]);

    if (!inOrderFile.is_open() || !levelOrderFile.is_open() || !encodedFile.is_open()) {
        cout << "Error: one or more of the files did not open properly." << endl;
        return 1;
    }


    // create class and populate vectors

    HuffmanTree* tree = new HuffmanTree;

    tree->populateArrays(inOrderFile, levelOrderFile);
    tree->printArrays();
    tree->root = tree->buildTree(tree->inOrder, tree->levelOrder, 0, "origin");
    //printInOrder(tree->root);

    //get encoding
    string encodingLine;
    string encoding;
    encodedFile >> encodingLine;
    
    while (!inOrderFile.eof()) {
        encoding.append(encodingLine);
        encodedFile >> encodingLine; 
    }

    int encodingLen = encoding.length();
    Node* currNode = tree->root;

    cout << " here" << endl;
    for (int i = 0; i < encodingLen; i ++) {
        cout << "here" << endl;
        if (currNode->data < 128) {
            cout << char(currNode->data);
            currNode = tree->root;
        } else {
            if (encoding.at(i)) {
                currNode = currNode->right;
            } else {
                currNode = currNode->left;
            }
        }
    }
    cout << endl;


    return 0;
}