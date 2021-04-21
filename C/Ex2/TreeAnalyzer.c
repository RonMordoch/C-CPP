//
// Created by Ron on 07-Aug-19.
//

// -------------------------------------- INCLUDES --------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
// ---------------------------------- CONST DEFINITIONS ---------------------------------

#define MAX_NUM_OF_ARGS 4
#define INVALID_ARGS_NUM_MSG "Usage: TreeAnalyzer <Graph File Path> <First Vertex> <Second Vertex>\n"
#define INVALID_INPUT_MSG "Invalid input\n"
#define NOT_TREE "The given graph is not a tree\n"
#define INPUT_DELIMS " \t\r\n\0"
#define LINE_MAX_SIZE 1024
#define ERROR -1

// -------------------------------------- STRUCTS ---------------------------------------

typedef struct Node Node;

/**
 * A struct representing a node in a graph.
 */
struct Node
{
    int key;
    int numOfChildren;
    int numOfParents;
    int visited;
    int prev; // prev node in path during BFS run
    int distance;
    Node **parents; // pointer to parent nodes
    Node **children; // pointer to children nodes
};

typedef struct Graph Graph;

/**
 * A struct representing a graph.
 */
struct Graph
{
    Node **nodes; // Pointer to nodes
    int numOfNodes;
    int numOfEdges;
};

// -------------------------------------- FUNCTIONS -------------------------------------

void freeGraph(Graph *g, int n)
{
    for (int i = 0; i < n; ++i)
    {
        free(g->nodes[i]->children);
        g->nodes[i]->children = NULL;
        free(g->nodes[i]->parents);
        g->nodes[i]->parents = NULL;
        free(g->nodes[i]);
        g->nodes[i] = NULL;
    }
    free(g->nodes);
    g->nodes = NULL;
    free(g);
}

/**
 * Initialize graph with values
 * @param g graph to initialize
 * @param n number of nodes read from first line
 * @return 0 is graph was successfully initialized, -1 otherwise
 */
int initGraph(Graph *g, int n)
{
    g->numOfNodes = 0;
    g->numOfEdges = 0;
    g->nodes = (Node **) malloc(n * sizeof(Node *));
    if (g->nodes == NULL)
    {
        return ERROR;
    }
    for (int i = 0; i < n; ++i)
    {
        g->nodes[i] = (Node *) malloc(sizeof(Node));
        if (g->nodes[i] == NULL)
        {
            return ERROR;
        }
        g->nodes[i]->key = -1; // default init
        g->nodes[i]->children = (Node **) malloc(n * sizeof(Node *));
        if (g->nodes[i]->children == NULL)
        {
            return ERROR;
        }
        g->nodes[i]->parents = (Node **) malloc(n * sizeof(Node *));
        if (g->nodes[i]->parents == NULL)
        {
            return ERROR;
        }
    }
    return 0;
}


/**
 * Runs breadth-first search on the given graph from the given start node.
 * @param g graph to query
 * @param start node to calculate distances from
 * @return the value of the last node queried
 */
unsigned int BFS(Graph *g, Node *start)
{
    // init distance values
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        g->nodes[i]->distance = -1;
    }
    start->distance = 0;
    start->prev = -1;
    Queue *q = allocQueue();
    enqueue(q, start->key);
    unsigned int lastNode = peekQueue(q);
    while (queueIsEmpty(q) != 1)
    {
        lastNode = peekQueue(q);
        unsigned int u = dequeue(q);
        for (int i = 0; i < g->nodes[u]->numOfChildren; ++i)
        {
            if (g->nodes[u]->children[i]->distance == -1)
            {
                enqueue(q, g->nodes[u]->children[i]->key);
                g->nodes[u]->children[i]->prev = g->nodes[u]->key;
                g->nodes[u]->children[i]->distance = g->nodes[u]->distance + 1;
            }
        }
        for (int i = 0; i < g->nodes[u]->numOfParents; ++i)
        {
            if (g->nodes[u]->parents[i]->distance == -1)
            {
                enqueue(q, g->nodes[u]->parents[i]->key);
                g->nodes[u]->parents[i]->prev = g->nodes[u]->key;
                g->nodes[u]->parents
                [i]->distance = g->nodes[u]->distance + 1;
            }
        }
    }
    freeQueue(&q);
    return lastNode;
}

/**
 * Recursive function to explore start node and all of the nodes linked to it through edges.
 * @param g graph to explore
 * @param start start node
 */
void explore(Graph *g, Node *start)
{
    start->visited = 1;
    for (int i = 0; i < start->numOfChildren; ++i)
    {
        if (start->children[i]->visited == -1)
        {
            explore(g, start->children[i]);
        }
    }
    for (int j = 0; j < start->numOfParents; ++j)
    {
        if (start->parents[j]->visited == -1)
        {
            explore(g, start->parents[j]);
        }
    }
}

/**
 * Runs depth-first search on a given graph.
 * @param g graph to query
 * @return number of connected components in the graph (while treating graph as undirected)
 */
int DFS(Graph *g)
{
    int connectivityComponents = 0;
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        g->nodes[i]->visited = -1;
    }

    for (int j = 0; j < g->numOfNodes; ++j)
    {
        if (g->nodes[j]->visited == -1)
        {
            ++connectivityComponents;
            explore(g, g->nodes[j]);
        }
    }

    return connectivityComponents;
}

/**
 * Checks if the graph is a tree, i.e. if it fulfills Euler's formula, has only one connected
 * component, only one node with no parents (root) and the rest of the nodes have exactly one
 * parent node.
 * @param g graph
 * @return 0 if graph is a tree, -1 otherwise
 */
int checkTree(Graph *g)
{
    if (g->numOfEdges != g->numOfNodes - 1)
    {
        return ERROR;
    }
    // else, check for connectivity and only one node without parent;
    int connectivityComponents = 0;
    connectivityComponents = DFS(g);
    int invalidParentCounter = 0;
    int parentlessCounter = 0;
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        if (g->nodes[i]->numOfParents > 1)
        {
            ++invalidParentCounter;
        }
        if (g->nodes[i]->numOfParents == 0)
        {
            ++parentlessCounter;
        }
    }
    if (connectivityComponents == 1 && invalidParentCounter == 0 && parentlessCounter == 1)
    {
        return 0;
    }
    // else, not a tree
    return ERROR;
}

/**
 * Returns the root node of a given graph (after checking its a tree).
 * @param g graph of a tree
 * @return the root of the graph
 */
int getRoot(Graph *g)
{
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        if (g->nodes[i]->numOfParents == 0) // we already validated this is a tree
        {
            return (g->nodes[i]->key);
        }
    }
    return 0;
}

/**
 * Returns the diameter of the graph.
 * @param g graph representing a tree
 * @return diameter of the graph
 */
int getDiameter(Graph *g)
{
    unsigned int node1 = BFS(g, g->nodes[getRoot(g)]);
    unsigned int node2 = BFS(g, g->nodes[node1]);
    return g->nodes[node2]->distance;
}

/**
 * Returns the shortest path between 2 nodes in a graph.
 * @param g graph containing nodes
 * @param node1 start node
 * @param node2 end node
 * @return 0 and prints path if successful; else -1;
 */
int getShortestPath(Graph *g, int node1, int node2, int *path)
{
    BFS(g, g->nodes[node1]);

    int i = 0; // index
    int currNode = node2;
    while (currNode != node1)
    {
        currNode = g->nodes[currNode]->prev;
        path[i] = currNode;
        ++i;
    }
    for (int j = i - 1; j >= 0; --j)
    {
        printf("%d ", path[j]);
    }
    printf("%d", node2);
    free(path);
    path = NULL;
    return 0;
}

/**
 * Returns the max length of a branch in graph.
 * @param g graph
 * @param root root node of graph
 * @return max length of branch in graph
 */
unsigned int getMaxLength(Graph *g, int root)
{
    int maxLength = 0;
    BFS(g, g->nodes[root]);
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        if (maxLength < g->nodes[i]->distance)
        {
            maxLength = g->nodes[i]->distance;
        }
    }
    return maxLength;
}

/**
 * Returns the min length of a branch in graph.
 * @param g graph
 * @param root root node of graph
 * @return min length of branch in graph
 */
unsigned int getMinLength(Graph *g, int root)
{
    if (g->numOfNodes == 1)
    {
        return 0;
    }
    int minLength = g->numOfNodes; // maximum length possible
    BFS(g, g->nodes[root]);
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        if (g->nodes[i]->key != root &&
            g->nodes[i]->numOfChildren == 0 && g->nodes[i]->distance < minLength)
        {
            minLength = g->nodes[i]->distance;
        }
    }
    return minLength;
}

/**
 * Checks if a string is a valid positive integer
 * @param string string to validate
 * @return integer representation of string if valid; -1 otherwise;
 */
int checkNumber(char const *string, size_t length)
{
    int num = 0;
    for (size_t i = 0; i < length; ++i)
    {
        if (string[i] > '9' || string[i] < '0')
        {
            return ERROR;
        }
        num *= 10;
        int c = string[i] - '0';
        num += c;
    }
    // number is definitely only digits and positive, return the integer type
    return num;
}

/**
 * Returns the file size of given file
 * @param src pointer to file
 * @return size of file
 */
long getFileSize(FILE *src)
{
    fseek(src, 0, SEEK_END); // point to end of file
    long size = ftell(src); // get size
    fseek(src, 0, 0); // reset pointer to beginning of file
    return size;
}

/**
 * Connect to nodes as parent and child
 * @param g graph containing nodes
 * @param parent parent node
 * @param child child node
 */
void connectNodes(Graph *g, int parent, int child)
{
    // Link parent to child
    g->nodes[parent]->children[g->nodes[parent]->numOfChildren] = g->nodes[child];
    ++g->nodes[parent]->numOfChildren;
    // Link child to parent
    g->nodes[child]->parents[g->nodes[child]->numOfParents] = g->nodes[parent];
    ++g->nodes[child]->numOfParents;
}

/**
 * Initiliaze a node with given values
 * @param node node
 * @param key key of node
 * @param numOfChildren number of children nodes
 * @param numOfParents number of porent nodes
 */
void initNode(Node *node, int key, int numOfChildren, int numOfParents)
{
    node->key = key;
    node->numOfChildren = numOfChildren;
    node->numOfParents = numOfParents;
}

/**
 * Check whether 2 nodes are in graph
 * @param g graph
 * @param node1 first node to search for
 * @param node2 second node to search for
 * @return 0 if both nodes are found, -1 otherwise
 */
int findNodes(Graph *g, int node1, int node2)
{
    int firstNodeFound = -1;
    int secondNodeFound = -1;
    for (int i = 0; i < g->numOfNodes; ++i)
    {
        if (g->nodes[i]->key == node1)
        {
            firstNodeFound = 1;
        }
        if (g->nodes[i]->key == node2)
        {
            secondNodeFound = 1;
        }
    }
    if (firstNodeFound == 1 && secondNodeFound == 1)
    {
        return 0; // found both
    }
    else
    {
        return -1;
    } // not found
}

/**
 * Create children for a parent node from line of values
 * @param g graph containing nodes
 * @param parent parent node to create children nodes for
 * @param line line containing the children nodes data
 * @param maxNum maximum value for node
 * @return 0 if children nodes are valid and were successfully added , -1 otherwise
 */
int createChildren(Graph *g, int parent, char *line, int maxNum)
{
    char *value = NULL;
    // grab the first value
    value = strtok(line, INPUT_DELIMS);
    if (value == NULL)
    {
        return ERROR; // empty line
    }
    // Check if node is a leaf
    if (strcmp(value, "-") == 0)
    {
        return 0; // node is a leaf, finish checking line
    }
    else
    {
        // else, node has children
        while (value != NULL)
        {
            int key = checkNumber(value, strlen(value));
            if (key == ERROR)
            {
                return ERROR;
            }
            if (key > maxNum)
            {
                return ERROR;
            }
            // check if child node was already initialized
            if (g->nodes[key]->key == -1)
            {
                initNode(g->nodes[key], key, 0, 0);
                connectNodes(g, parent, key);
                ++g->numOfNodes;
            }
            else
            {
                connectNodes(g, parent, key);
            }
            ++g->numOfEdges;
            // grab the next value
            value = strtok(NULL, INPUT_DELIMS);
        }
        return 0;
    }
}

/**
 * Checks if node represented by line was already created, if not
 * create note, and creates children for node.
 * @param g graph
 * @param line line containing data about children nodes
 * @param lineIndex key of parent node
 * @param maxNum maximum number of node to create / maximum key for nodes
 * @return 0 if successfully created children and node, -1 otherwise
 */
int checkLine(Graph *g, char *line, int lineIndex, int maxNum)
{
    if (g->nodes[lineIndex]->key == -1) // check if node doesnt exist already
    {
        initNode(g->nodes[lineIndex], lineIndex, 0, 0);
        g->numOfNodes++;

        int i = createChildren(g, lineIndex, line, maxNum);
        if (i == ERROR)
        {
            return ERROR;
        }

    }
    else
    { // node already exists
        int i = createChildren(g, lineIndex, line, maxNum);
        if (i == ERROR)
        {
            return ERROR;
        }
    }
    return 0;
}


/**
 * Parses the source file to create graph
 * @param file file containing nodes and edges data
 * @param g graph to Initialize
 * @param n number of nodes read from first line
 * @return 0 if successfully created, -1 otherwise
 */
int parseFile(FILE *file, Graph *g, int n)
{
    char line[LINE_MAX_SIZE];
    int lineIndex = 0; // actually second line
    while (fgets(line, LINE_MAX_SIZE, file) != NULL)
    {
        if (lineIndex + 1 > n)
        {

            return ERROR;
        }
        int i = checkLine(g, line, lineIndex, n);

        if (i == ERROR)
        {
            return ERROR;
        }


        ++lineIndex;
    }

    if (n != g->numOfNodes)
    {
        return ERROR;
    }
    return 0;

}

/**
 * Prints the results of the entire graph query
 * @param g graph queried
 * @param node1 first node received from input
 * @param node2 second node received from input
 */
void printResults(Graph *g, int node1, int node2, int *path)
{

    printf("Root Vertex: %d\n"
           "Vertices Count: %d\n"
           "Edges Count: %d\n"
           "Length of Minimal Branch: %d\n"
           "Length of Maximal Branch: %d\n"
           "Diameter Length: %d\n"
           "Shortest Path Between %d and %d: ",
           getRoot(g),
           g->numOfNodes,
           g->numOfEdges,
           getMinLength(g, getRoot(g)),
           getMaxLength(g, getRoot(g)),
           getDiameter(g),
           node1, node2
    );
    getShortestPath(g, node1, node2, path);
}


/**
 * Main function , drives the entire program
 * @param argc number of arguments received from input
 * @param argv array of values received
 * @return 0 if program successfully ran, -1 if invalid input/files were given
 */
int main(int argc, char *argv[])
{
    int flag = 0;
    // Check number of arguments
    if (argc != MAX_NUM_OF_ARGS)
    {
        fprintf(stderr, INVALID_ARGS_NUM_MSG);
        return EXIT_FAILURE;
    }

    FILE *file;
    file = fopen(argv[1], "r");
    //Check if file exists
    if (file == NULL)
    {
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    // Check is file is empty
    if (getFileSize(file) == 0)
    {
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    // check 2 nodes input
    int node1 = checkNumber(argv[2], strlen(argv[2]));
    int node2 = checkNumber(argv[3], strlen(argv[3]));
    if (node1 == ERROR || node2 == ERROR)
    {
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    // read and parse
    // Read first line
    char line[LINE_MAX_SIZE];
    fgets(line, LINE_MAX_SIZE, file);
    int n = checkNumber(line, strlen(line) - 1);
    if (n == ERROR)
    {
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    // Create graph and allocate memory for nodes
    Graph *g = (Graph *) malloc(sizeof(Graph));
    if (g == NULL)
    {
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    flag = initGraph(g, n);
    if (flag != 0)
    {
        fclose(file);
        freeGraph(g, n);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    flag = parseFile(file, g, n);
    if (flag != 0)
    {
        freeGraph(g, n);
        fclose(file);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }
    // Finished parsing, close file
    fclose(file);

    int foundNodes = findNodes(g, node1, node2);
    if (foundNodes != 0)
    {
        freeGraph(g, n);
        fprintf(stderr, INVALID_INPUT_MSG);
        return EXIT_FAILURE;
    }

    int isTree = checkTree(g);
    if (isTree != 0)
    {
        freeGraph(g, n);
        fprintf(stderr, NOT_TREE);
        return EXIT_FAILURE;
    }
    int *path = (int *) malloc(g->numOfNodes * sizeof(int));
    if (path == NULL)
    {
        freeGraph(g, n);
        fprintf(stderr, NOT_TREE);
        return EXIT_FAILURE;
    }
    printResults(g, node1, node2, path);
    freeGraph(g , n);

    return 0;
}
