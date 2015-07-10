#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include "enums.h"

typedef enum NodeType {
    NODE_0,
    NODE_1,
    NODE_2,
    NODE_3,
    NODE_4,
    NODE_N,
    NODE_SYM,
    NODE_VAR
} NodeType;

typedef int NodeAction;

typedef struct NodeSyntax {
    NodeType type;
} NodeSyntax;

typedef struct Node0 {
    NodeType type;
    NodeAction action;
} Node0;

typedef struct Node1 {
    NodeType type;
    NodeAction action;
    NodeSyntax * node;
} Node1;

typedef struct Node2 {
    NodeType type;
    NodeAction action;
    NodeSyntax * node1, * node2;
} Node2;

typedef struct Node3 {
    NodeType type;
    NodeAction action;
    NodeSyntax * node1, * node2, * node3;
} Node3;

typedef struct Node4 {
    NodeType type;
    NodeAction action;
    NodeSyntax * node1, * node2, * node3, * node4;
} Node4;

typedef struct NodeSym {
    NodeType type;
    char * name;
} NodeSym;

typedef struct NodeVar {
    NodeType type;
    NodeAction action;
    YYSTYPE value;
} NodeVar;

typedef struct NodeN {
    NodeType type;
    NodeAction action;
    int length;
    NodeSyntax ** nodes;
} NodeN;

Node0 * createNode0(NodeAction action);
Node1 * createNode1(NodeAction action, NodeSyntax * node);
Node2 * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2);
Node3 * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3);
Node4 * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4);
NodeN * createNodeN(NodeAction action);
NodeN * appendNodeN(NodeN * NodeN, NodeSyntax * node);
NodeSym * createNodeSym(char * name);
NodeVar * createNodeVar(NodeAction action, YYSTYPE value);

void printNode0(Node0 * node, int level);
void printNode1(Node1 * node, int level);
void printNode2(Node2 * node, int level);
void printNode3(Node3 * node, int level);
void printNode4(Node4 * node, int level);
void printNodeN(NodeN * node, int level);
void printNodeSym(NodeSym * node, int level);
void printNodeSyntax(NodeSyntax * node, int level);

#endif // AST_H_INCLUDED
