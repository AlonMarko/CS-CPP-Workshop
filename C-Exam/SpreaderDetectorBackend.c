//
// Created by alonn on 8/2/2020.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "SpreaderDetectorParams.h"

#define MAXNAMELENGTH 1015
#define LINELNEGTH 1025
#define ARGS 3
#define USAGE "Usage: ./SpreaderDetectorBackend <Path to People.in> <Path to Meetings.in>\n"
#define INPUTFERROR "Error in input files.\n"
#define OUTERROR "Error in output file.\n"
#define LOWER -1
#define EQUAL 0
#define HIGHER 1
#define NAME 0
#define ID 1
#define IDS 0
#define TIME 3
#define DISTANCE 2
#define FIRSTLINE 1
#define MEETINGSF 1
#define MEETINGSE 2
#define PRINTNORMAL 1
#define PRINTEMPTY 2

/**
 * @brief color for RBtree
 */
typedef enum Color
{
    RED,
    BLACK
} Color;


/**
 * @brief person struct for the array we sort contains name id and chance to get infected. age is
 * not neccesary here so it is not included
 */
typedef struct Person
{
    int id;
    char name[MAXNAMELENGTH];
    float chance;
} Person;

/**
 * @brief the node of our RBTree data structure
 */
typedef struct Node
{
    struct Node *parent, *left, *right;
    Color color;
    int id;
    char name[MAXNAMELENGTH];
    float age, chance;
} Node;

/**
 * @brief RBtree struct - root node and size of the tree.
 */
typedef struct RBTree
{
    Node *root;
    long unsigned size;
} RBTree;

/**
 * @brief prints the error to stderr and exits the program with failure exit code, memory is freed
 * before this function is called.
 * @param errorCode
 */
void errorHandling(char *errorCode)
{
    if (strcmp(errorCode, STANDARD_LIB_ERR_MSG) == 0)
    {
        fprintf(stderr, STANDARD_LIB_ERR_MSG);
    }
    if (strcmp(errorCode, INPUTFERROR) == 0)
    {
        fprintf(stderr, INPUTFERROR);
    }
    if (strcmp(errorCode, OUTERROR) == 0)
    {
        fprintf(stderr, OUTERROR);
    }
    if (strcmp(errorCode, USAGE) == 0)
    {
        fprintf(stderr, USAGE);
    }
    exit(EXIT_FAILURE);
}


/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree *newRBTree()
{
    RBTree *redBlackTree;
    redBlackTree = (RBTree *) malloc(sizeof(RBTree));
    if (redBlackTree == NULL)
    {
        errorHandling(STANDARD_LIB_ERR_MSG);
    }
    redBlackTree->root = NULL; // will not be null since we exit the program
    redBlackTree->size = 0; // same
    return redBlackTree;
}


/**
 * @brief gets the parent
 * @param node node
 * @return NULL or the parent node
 */
Node *getParent(Node *node)
{
    return node == NULL ? NULL : node->parent;
}


/**
 * @brief gets the brother of the cur node
 * @param node the cur node
 * @return the brother node or NULL
 */
Node *getBrother(Node *node)
{
    Node *temp = getParent(node);
    if (temp == NULL)
    {
        return NULL;
    }
    if (node == temp->left)
    {
        return temp->right;
    }
    return temp->left;
}

/**
 * @brief gets the uncle NODE
 * @param node the node to get its uncle
 * @return the uncle node or NULL
 */
Node *getUncle(Node *node)
{
    Node *temp = getParent(node);
    return getBrother(temp);
}

/**
 * @brief right rotate function - performs a rotation around teh given node, cannot rotate right
 * if left child is RBNULL
 * @param tree tree object
 * @param node node to rotate
 */
void rotateRight(RBTree *tree, Node *node)
{
    Node *temp = node->left;
    node->left = temp->right;
    if (temp->right != NULL)
    {
        temp->right->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else if (node == node->parent->right)
    {
        node->parent->right = temp;
    }
    else
    {
        node->parent->left = temp;
    }
    temp->right = node;
    node->parent = temp;
}

/**
 * @brief rotates left around the given node - cannot rotate left if the right child is null
 * @param tree tree struct
 * @param node node to rotate around
 */
void rotateLeft(RBTree *tree, Node *node)
{
    Node *temp = node->right;
    node->right = temp->left;
    if (temp->left != NULL)
    {
        temp->left->parent = node;
    }
    temp->parent = node->parent;
    if (node->parent == NULL)
    {
        tree->root = temp;
    }
    else
    {
        if (node == node->parent->left)
        {
            node->parent->left = temp;
        }
        else
        {
            node->parent->right = temp;
        }
    }
    temp->left = node;
    node->parent = temp;
}

/**
 * @brief function that returns the minimum node in the tree
 * @param root the root of the tree - can also be a node in teh tree so we will get back the
 * minimum in the current sub-tree.
 * @return the minimum node
 */
Node *findMin(Node *root)
{
    Node *current = root;
    while (current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

/**
 * @brief function to get successor node in the tree.
 * @param node the node we want to get successor of.
 * @return the successor node
 */
Node *successor(Node *node)
{
    if (node->right != NULL)
    {
        return findMin(node->right);
    }
    Node *current = node->parent;
    while (current != NULL && node == current->right)
    {
        node = current;
        current = current->parent;
    }
    return current;
}

/**
 * @brief finds the requested ID and returns the node containing the person
 * @param tree
 * @param id
 * @return the node that matches
 */
Node *find(RBTree *tree, int id)
{
    Node *temp = tree->root;
    while (temp != NULL)
    {
        if (id == temp->id)
        {
            return temp;
        }
        if (id < temp->id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    return NULL;
} // cannot reach end without finding nothing since we know the value is in the tree but we still
//  return NULL just for coding style.

/**
 * @brief function that gets a node that needs to be fixed and fixed the whole tree accordingly.
 * if the node is the root it colors it black.
 * @param tree tree struct
 * @param node node that was added and has to be fixed.
 */
static void insertFix(RBTree *tree, Node *node)
{
    if (node->parent == NULL)// case 1
    {
        node->color = BLACK;
        return;
    }
    if (node->parent->color == BLACK)// case 2
    {
        return;
    }
    Node *uncleNode = getUncle(node);
    Color uncle = uncleNode ? uncleNode->color : BLACK;
    if (node->parent->color == RED && uncle == RED) // case 3
    {
        node->parent->color = BLACK;
        uncleNode->color = BLACK; // cannot be NULL since we enter if red
        node->parent->parent->color = RED;
        insertFix(tree, node->parent->parent);
    }
    else if (node->parent->color == RED && uncle == BLACK) // case 4
    {
        if (node == node->parent->right && node->parent == node->parent->parent->left)
        {
            rotateLeft(tree, node->parent);
            rotateRight(tree, node->parent);
            node->color = BLACK;
            node->right->color = RED;
            return;
        }
        else if (node == node->parent->left && node->parent == node->parent->parent->right)
        {
            rotateRight(tree, node->parent);
            rotateLeft(tree, node->parent);
            node->color = BLACK;
            node->left->color = RED;
            return;
        }
        if (node == node->parent->left && node->parent == node->parent->parent->left)
        {
            rotateRight(tree, node->parent->parent);
            node->parent->color = BLACK;
            node->parent->right->color = RED;
        }
        else
        {
            rotateLeft(tree, node->parent->parent);
            node->parent->color = BLACK;
            node->parent->left->color = RED;
        }
    }
}


/**
 * @brief function that creates a new tree node to be added and allocates its memory.
 * @param data to be added
 * @return NULL if failed or pointer to the node else.
 */
Node *createNode(int id, char *name, float age)
{
    Node *node = NULL;
    node = (Node *) malloc(sizeof(Node));
    if (node == NULL)
    {
        return NULL;
    }
    node->id = id;
    strcpy(node->name, name);
    node->chance = 0;
    node->age = age;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->color = RED;
    return node;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, int id, char *name, float age)
{
    Node *toAdd = NULL;
    toAdd = createNode(id, name, age);
    if (toAdd == NULL)
    {
        return 0;
    }
    Node *temp = tree->root;
    Node *temp2 = NULL;
    while (temp != NULL)
    {
        temp2 = temp;
        if (id < temp->id)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }
    toAdd->parent = temp2;
    if (temp2 == NULL)
    {
        tree->root = toAdd;
    }
    else
    {
        if (toAdd->id < temp2->id)
        {
            temp2->left = toAdd;
        }
        else
        {
            temp2->right = toAdd;
        }
    }
    tree->size++;
    insertFix(tree, toAdd);
    return 1;
}

/**
* @brief free recursive helper function to free the nodes and data
* @param tree object
* @param root current node to recurse upon
*/
void freeNodes(RBTree **tree, Node **root)
{
    if (*root == NULL)
    {
        return;
    }
    freeNodes(&(*tree), &(*root)->right);
    freeNodes(&(*tree), &(*root)->left);
    free(*root);
    *root = NULL;
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    if (*tree == NULL)
    {
        return;
    }
    freeNodes(tree, &(*tree)->root);
    free(*tree);
}

/**
 * @brief frees the memory from allocated objects.
 * @param tree pointer to rbtree
 * @param array pointer to person array
 */
void emergancyFree(RBTree **tree, Person *array, char *errorCode)
{
    freeRBTree(tree);
    free(array);
    if (errorCode != NULL)
    {
        errorHandling(errorCode);
    }
}

/**
 * @brief checks if a given pointer points to an empty file
 * @param fileP file pointer
 * @return NULL if empty or the the first line of the file, if the file is empty it is also closed?
 */
char *checkEmpty(FILE *fileP)
{
    char *line = malloc(LINELNEGTH);
    if (line == NULL)
    {
        emergancyFree(NULL, NULL, STANDARD_LIB_ERR_MSG);
    }
    if (fgets(line, LINELNEGTH, fileP) == NULL)
    {
        free(line);
        return NULL;
    }
    return line;
}

/**
 * @brief checks if given filename/path exists and not empty
 * @param fileName - name or path to the file
 * @return true if it exists or false otherwise!
 */
bool checkExists(char *fileName)
{
    FILE *fileP;
    fileP = fopen(fileName, "r");
    if (fileP == NULL)
    {
        return false;
    }
    return true;
}

/**
 * @brief the crazy complicated function that calcuklates the chance of someone to be infected by
 * staying next to a possibly infected person.
 * @param time
 * @param distance
 * @return the crna value
 */
float crna(float time, float distance)
{
    return ((time * MIN_DISTANCE) / (distance * MAX_TIME));
}

/**
 * @brief converts the id from string to int
 * @param ptr
 * @return converted id
 */
int idConvert(char *ptr)
{
    int id;
    sscanf(ptr, "%d", &id);
    return id;
}

/**
 * @brief converts age from string to float
 * @param ptr
 * @return the converted float
 */
float ageConvert(char *ptr)
{
    float age = strtof(ptr, NULL);
    return age;
}

/**
 * @brief reads from the people input file and adds them to the RBTree
 * @param fileP pointer to the open people file
 * @param tree pointer to tree
 * @param fLine pointer to the first line of input
 * @param fileM pointer to the meetings file for emergancy exit and close
 */
void readPeople(FILE *fileP, RBTree *tree, char *fLine, FILE *fileM)
{
    char line[LINELNEGTH];
    int id = 0;
    float age = 0;
    char name[MAXNAMELENGTH];
    if (fLine != NULL)
    {
        strcpy(line, fLine);
        free(fLine);
    }
    do
    {
        char *newLineR = strchr(line, '\n');
        if (newLineR)
        {
            *newLineR = '\0';
        }
        int counter = 0;
        char *ptr = strtok(line, " ");
        while (ptr != NULL)
        {
            if (counter == NAME)
            {
                sscanf(ptr, "%s", name);
            }
            if (counter == ID)
            {
                id = idConvert(ptr);
            }
            else
            {
                age = ageConvert(ptr);
            }
            ptr = strtok(NULL, " ");
            counter++;
        }
        if (insertToRBTree(tree, id, name, age) == 0)
        {
            if (fileM != NULL)
            {
                fclose(fileM);
            }
            fclose(fileP);
            emergancyFree(&tree, NULL, STANDARD_LIB_ERR_MSG);
        }
    } while (fgets(line, sizeof(line), fileP) != NULL);
}


/**
 * @brief reads the meeting file and calculates the crna value of each person according to the
 * proper meeting
 * @param fileP pointer to the file
 * @param tree RBtree pointer
 * @param fLine first line of input from the file
 */
void readMeetings(FILE *fileP, RBTree *tree, char *fLine)
{
    char line[LINELNEGTH];
    int lineCounter = 0, spreader = 0, currentSpreader = 0, possibleInfected = 0;
    float distance = 0, time = 0, spreaderChance;
    Node *nPtr;
    if (fLine != NULL)
    {
        strcpy(line, fLine);
        free(fLine);
    }
    do
    {
        char *newLineR = strchr(line, '\n');
        if (newLineR)
        {
            *newLineR = '\0';
        }
        lineCounter++;
        if (lineCounter == FIRSTLINE)
        {
            sscanf(line, "%d", &spreader);
            currentSpreader = spreader;
            nPtr = find(tree, spreader);
            nPtr->chance = 1;
            continue;
        }
        char *ptr = strtok(line, " ");
        int counter = 0;
        while (ptr != NULL)
        {
            if (counter == IDS)
            {
                sscanf(ptr, "%d", &spreader);
            }
            if (counter == ID)
            {
                sscanf(ptr, "%d", &possibleInfected);
            }
            else if (counter == DISTANCE)
            {
                distance = ageConvert(ptr);
            }
            else if (counter == TIME)
            {
                time = ageConvert(ptr);
            }
            ptr = strtok(NULL, " ");
            counter++;
        }
        spreaderChance = find(tree, spreader)->chance;
        if (spreader == currentSpreader)
        {
            nPtr = find(tree, possibleInfected);
            nPtr->chance = spreaderChance * crna(time, distance);
        }
        else
        {
            currentSpreader = spreader;
            spreaderChance = find(tree, currentSpreader)->chance;
            nPtr = find(tree, possibleInfected);
            nPtr->chance = spreaderChance * crna(time, distance);
        }
    } while (fgets(line, sizeof(line), fileP) != NULL);
}

/**
 * @brief according to if the meeting file is empty of not it reads the proper files and updates the
 * data structure and calculates the crna values if neccasary
 * @param filePP people file pointer
 * @param filePM meeting file pointer
 * @param tree RBtree pointer
 * @param properUsage indicator to wheter the meeting file is empty or not
 * @param pLine first people file line
 * @param mLine  first meeting file line
 */
void readFiles(FILE *filePP, FILE *filePM, RBTree *tree, int properUsage, char *pLine, char *mLine)
{
    switch (properUsage)
    {
        case MEETINGSF:
        {
            readPeople(filePP, tree, pLine, filePM);
            readMeetings(filePM, tree, mLine);
            break;
        }
        case MEETINGSE:
        {
            readPeople(filePP, tree, pLine, NULL);
            break;
        }
        default:
            break;
    }
}

/**
 * @brief creates an array of Persons from the RBtree!
 * @param tree the red black tree that holds all the persons
 * @return pointer to the array
 */
Person *createArrayFromRBT(RBTree *tree)
{
    unsigned int arraySize = tree->size;
    int index = 0;
    Person *array;
    array = malloc(arraySize * sizeof(Person));
    if (array == NULL)
    {
        emergancyFree(&tree, NULL, STANDARD_LIB_ERR_MSG);
    }
    Node *current = findMin(tree->root);
    Person p;
    p.chance = current->chance;
    strcpy(p.name, current->name);
    p.id = current->id;
    array[index] = p; //cannot be NULL if it is we already exit in the NULL if above.
    while (successor(current) != NULL)
    {
        index++;
        current = successor(current);
        p.chance = current->chance;
        strcpy(p.name, current->name);
        p.id = current->id;
        array[index] = p;
    }
    return array;
}

/**
 * @brief compare function for the chance of infection between 2 persons.
 * @param p1 person 1
 * @param p2 person 2
 * @return Positive if p1 chance is lower the p2 chance zero if equal or negative if p1 has higher
 * chance
 */
int infectionCompare(const void *p1, const void *p2)
{
    float a = ((Person *) p1)->chance;
    float b = ((Person *) p2)->chance;
    if (a < b)
    {
        return HIGHER;
    }
    else if (a == b)
    {
        return EQUAL;
    }
    return LOWER;
}

/**
 * @brief this function prints to the output file considering different situations.
 * @param arr
 * @param tree
 */
void filePrint(Person *arr, RBTree *tree, int printSituation)
{
    FILE *fptr;
    fptr = fopen(OUTPUT_FILE, "w");
    if (fptr == NULL)
    {
        emergancyFree(&tree, arr, OUTERROR);
    }
    switch (printSituation)
    {
        case PRINTNORMAL:
        {
            for (unsigned int i = 0; i < tree->size; i++)
            {
                if (arr[i].chance >= MEDICAL_SUPERVISION_THRESHOLD)
                {
                    fprintf(fptr, MEDICAL_SUPERVISION_THRESHOLD_MSG, arr[i].name, (unsigned long)
                            arr[i].id);
                    continue;
                }
                if (arr[i].chance >= REGULAR_QUARANTINE_THRESHOLD)
                {
                    fprintf(fptr, REGULAR_QUARANTINE_MSG, arr[i].name, (unsigned long) arr[i].id);
                    continue;
                }
                fprintf(fptr, CLEAN_MSG, arr[i].name, (unsigned long) arr[i].id);
            }
        }
        case PRINTEMPTY:
            break;
    }
    fclose(fptr);
}


/**
 * @brief the main driver fucntion which runs all the proccess from input read up to output print
 * or error exit
 * @return 0 upon successful run or prints to stderr and exits with the error code 1.
 */
int main(int argc, char *argv[])
{
    if (argc != ARGS)
    {
        errorHandling(USAGE);
    }
    if (checkExists(argv[1]) && checkExists(argv[2]))
    {
        FILE *filePP = fopen(argv[1], "r");
        FILE *filePM = fopen(argv[2], "r");
        char *firstLineP, *firstLineM;
        firstLineP = checkEmpty(filePP);
        firstLineM = checkEmpty(filePM);
        if (firstLineP != NULL)
        {
            RBTree *tree = newRBTree();
            if (firstLineM != NULL)
            {
                readFiles(filePP, filePM, tree, MEETINGSF, firstLineP, firstLineM);
            }
            else
            {
                readFiles(filePP, NULL, tree, MEETINGSE, firstLineP, NULL);
            }
            Person *arr = createArrayFromRBT(tree);
            qsort(arr, tree->size, sizeof(Person), (__compar_fn_t) infectionCompare);
            filePrint(arr, tree, PRINTNORMAL);
            emergancyFree(&tree, arr, NULL);
            fclose(filePP);
            fclose(filePM);
        }
        else if (firstLineM != NULL)
        {
            fclose(filePP);
            fclose(filePM);
            errorHandling(INPUTFERROR);
        }
        else
        {
            fclose(filePP);
            fclose(filePM);
            filePrint(NULL, NULL, PRINTEMPTY);
        }
    }
    else
    {
        errorHandling(INPUTFERROR);
    }
    return EXIT_SUCCESS;
}
