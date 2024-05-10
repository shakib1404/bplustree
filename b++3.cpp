#include<bits/stdc++.h>
using namespace std;
  // Number of children
int numofchild,mx;
int n=0;


struct node
{
    int totalnode;
    node* parent;
    node* next;
    string* english;
    string* meaning;
    bool lf;
    node** child;
};

node* root;



node* createNode()
{
    node* node1 = new node();

    node1->child = new node * [numofchild + 1];
    node1->english = new string[numofchild];
    node1->meaning = new string[numofchild];

    node1->totalnode= 0;
    node1->parent = nullptr;
    node1->next = nullptr;
    node1->lf = true;
    return node1;
}


void printPathToLeaf(node* leaf) {
    vector<string> path;
    while (leaf) {
        for (int i = 0; i < leaf->totalnode; ++i) {
            path.push_back(leaf->english[i]);
        }
       // path.push_back(leaf->english[i]);
        leaf = leaf->parent;
    }

    for (int i = path.size() - 1; i >= 0; --i) {
        cout << path[i];
        if (i > 0) {
            cout << " -> ";
        }
    }
    cout << endl;
}

node* searchleaf(node* temp, string english)
{

    while (temp->lf == false)
    {
        int i;

        for (i = 0; i < temp->totalnode; i++)
            if (english < temp->english[i]) break;
        temp = temp->child[i];

    }

    return temp;
}
node* searchleaffinal(node* temp, string english, int depth)
{
    while (temp->lf == false)
    {
        int i;

        for (i = 0; i < temp->totalnode; i++)
            if (english < temp->english[i]) break;
        temp = temp->child[i];

        depth++;
    }
    cout << "Depth: " << depth ;
    return temp;
}


void insertValueAndchild(node* parent, string value, node* right)
{
    int i = parent->totalnode - 1;
    for (; i >= 0; i--)
    {
        if (parent->english[i] <= value) break;
        else
        {
            parent->english[i + 1] = parent->english[i];
            parent->child[i + 2] = parent->child[i + 1];
        }
    }
    parent->english[i + 1] = value;
    parent->child[i + 2] = right;
    parent->totalnode++;
}


void insertintoMiddle(node* parent, string value, node* left, node* right)
{
    if (parent == nullptr)
    {
        parent = createNode();
        parent->english[0] = value;
        parent->child[0] = left;
        parent->child[1] = right;
        parent->totalnode++;
        parent->lf = false;
        root = parent;
        left->parent = parent;
        right->parent = parent;
        return;
    }
    right->parent = parent;
    insertValueAndchild(parent, value, right);
    if (parent->totalnode == mx)
    {
        node* splitNode = createNode();
        splitNode->lf = false;
        int j = 0;
        int i = parent->totalnode - (numofchild - 1) / 2;
        while ( i < mx )
        {
            splitNode->english[j] = parent->english[i];
            if (j == 0)
            {
                splitNode->child[0] = parent->child[i];
                splitNode->child[0]->parent = splitNode;
            }
            splitNode->child[j + 1] = parent->child[i + 1];
            splitNode->child[j + 1]->parent = splitNode;
            i++;
            j++;
        }
        parent->totalnode -= (numofchild- 1) / 2 + 1;
        splitNode->totalnode = (numofchild - 1) / 2;
        insertintoMiddle(parent->parent, parent->english[parent->totalnode], parent, splitNode);
    }
}


void insertLeaf(string english, string meaning)
{
    node* leaf = searchleaf(root, english);
    int i = leaf->totalnode - 1;
    if (i > -1)
    {
        while ( i >= 0)
        {
            if (english < leaf->english[i])
            {
                leaf->english[i + 1] = leaf->english[i];
                leaf->meaning[i + 1] = leaf->meaning[i];
            }
            else break;

            i--;
        }
    }


    leaf->english[i + 1] = english;
    leaf->meaning[i + 1] = meaning;
    leaf->totalnode++;

    if (leaf->totalnode== mx)
    {
        node* splitNode = createNode();
        int j = 0;
        int i = leaf->totalnode - numofchild / 2;
        do
        {
            splitNode->english[j] = leaf->english[i];
            splitNode->meaning[j] = leaf->meaning[i];
            i++;
            j++;
        }
        while(i < mx);


        leaf->totalnode -= numofchild / 2;
        splitNode->totalnode = numofchild / 2;
        splitNode->next = leaf->next;
        leaf->next = splitNode;
        insertintoMiddle(leaf->parent, splitNode->english[0], leaf, splitNode);
    }
}

int main()
{

    string key, value, findword;
    cout<<"number of child :";
    cin>>numofchild;
    mx=numofchild;

    root = createNode();
    node* lastleaf;
    ifstream ifile;
    ifile.open("word.txt");
    if (!ifile) cout << "file nai khoj";
    else
    {
        while (ifile >> key)
       {
            getline(ifile, value);
            insertLeaf(key, value);
        }
        cout << "Enter word for finding (end with x): ";
        while (cin >> findword)
        {
            if (findword == "x" || findword== "X")
            {
                break;
            }


            lastleaf = searchleaffinal(root, findword,0);
            int k=0;

            while( k < lastleaf->totalnode)
            {
                k++;
                if (findword == lastleaf->english[k])
                    break;
            }



            if (k == lastleaf->totalnode) cout << "No word found\n";

            else cout << findword << ": " << lastleaf->meaning[k] << endl;
            printPathToLeaf(lastleaf);



            cout << "Enter text to search:(x to terminate)";
        }
    }
    return 0;
}
