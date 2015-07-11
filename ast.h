#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include "enums.h"

typedef enum NodeType {
    NODE_N,
    NODE_SYM,
    NODE_VAL
} NodeType;

typedef int NodeAction;

typedef struct NodeSyntax {
    NodeType type;
} NodeSyntax;

typedef struct NodeN {
    NodeType type;
    NodeAction action;
    int length;
    NodeSyntax ** nodes;
} NodeN;

typedef struct NodeSym {
    NodeType type;
    char * name;
} NodeSym;

typedef struct NodeVal {
    NodeType type;
    NodeAction action;
    union {
        int i;
        double d;
        char * s;
    } value;
} NodeVal;

NodeN * createNode0(NodeAction action);
NodeN * createNode1(NodeAction action, NodeSyntax * node);
NodeN * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2);
NodeN * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3);
NodeN * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4);

NodeN * createNodeN(NodeAction action);
NodeN * appendNodeN(NodeN * NodeN, NodeSyntax * node);
NodeN * mergeNodeN(NodeN * nodeBase, NodeN * nodeNew);
NodeSyntax * popFirstNodeN(NodeN * node);

NodeVal * createNodeValChar(char * value);
NodeVal * createNodeValInt(int value);
NodeVal * createNodeValDouble(double value);
NodeVal * createNodeValString(char * value);
NodeSym * createNodeSym(char * name);

void printNodeN(NodeN * node, int level);
void printNodeSym(NodeSym * node, int level);
void printNodeVal(NodeVal * node, int level);
void printNodeSyntax(NodeSyntax * node, int level);

#endif // AST_H_INCLUDED
