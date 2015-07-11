#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "enums.h"

Node0 * createNode0(NodeAction action) {
    Node0 * node = malloc(sizeof(Node0));
    node->type = NODE_0;
    node->action = action;
    return node;
}

Node1 * createNode1(NodeAction action, NodeSyntax * node1) {
    Node1 * node = malloc(sizeof(Node1));
    node->type = NODE_1;
    node->action = action;
    node->node = node1;
    return node;
}

Node2 * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2) {
    Node2 * node = malloc(sizeof(Node2));
    node->type = NODE_2;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    return node;
}

Node3 * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3) {
    Node3 * node = malloc(sizeof(Node3));
    node->type = NODE_3;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    node->node3 = node3;
    return node;
}

Node4 * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4) {
    Node4 * node = malloc(sizeof(Node4));
    node->type = NODE_4;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    node->node3 = node3;
    node->node4 = node4;
    return node;
}

NodeN * createNodeN(NodeAction action) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->nodes = 0;
    node->length = 0;
    return node;
}

NodeN * appendNodeN(NodeN * nodeBase, NodeSyntax * nodeNew) {
    if (!nodeNew) return nodeBase;
    NodeN * node = nodeBase;
    assert(node->type == NODE_N);
    node->length++;
    node->nodes = realloc(node->nodes, sizeof(NodeSyntax *) * node->length);
    node->nodes[node->length-1] = nodeNew;
    return node;
}

NodeN * mergeNodeN(NodeN * nodeBase, NodeN * nodeNew) {
    if (!nodeNew) return nodeBase;
    assert(nodeBase->type == NODE_N);
    assert(nodeNew->type == NODE_N);
    int newLength = nodeBase->length + nodeNew->length;
    nodeBase->nodes = realloc(nodeBase->nodes, sizeof(NodeSyntax *) * newLength);
    for (int i = 0; i < nodeNew->length; i++)
        nodeBase->nodes[i + nodeBase->length] = nodeNew->nodes[i];
    nodeBase->length = newLength;
    return nodeBase;
}

NodeSym * createNodeSym(char * name) {
    NodeSym * node = malloc(sizeof(NodeSym));
    node->type = NODE_SYM;
    node->name = name;
    return node;
}

NodeVal * createNodeValChar(char * value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_CHAR;
    node->value.i = value[1];
// TODO (Δενθρ#1#): Escape check
    free(value);
    return node;
}

NodeVal * createNodeValInt(int value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_INT;
    node->value.i = value;
    return node;
}

NodeVal * createNodeValDouble(double value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = CONSTANT_DOUBLE;
    node->value.d = value;
    return node;
}

NodeVal * createNodeValString(char * value) {
    NodeVal * node = malloc(sizeof(NodeVal));
    node->type = NODE_VAL;
    node->action = STRING_LITERAL;
    node->value.s = value;
// TODO (Δενθρ#1#): Escape check
    return node;
}

extern void printfl(int level);

void printNode0(Node0 * node, int level) {
    printfl(level);
    printf("%s, Node0", enum_tostring(node->action));
}

void printNode1(Node1 * node, int level) {
    printfl(level);
    printf("%s, Node1", enum_tostring(node->action));
    printNodeSyntax(node->node, level + 1);
}

void printNode2(Node2 * node, int level) {
    printfl(level);
    printf("%s, Node2", enum_tostring(node->action));
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
}

void printNode3(Node3 * node, int level) {
    printfl(level);
    printf("%s, Node3", enum_tostring(node->action));
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
    printNodeSyntax(node->node3, level + 1);
}

void printNode4(Node4 * node, int level) {
    printfl(level);
    printf("%s, Node4", enum_tostring(node->action));
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
    printNodeSyntax(node->node3, level + 1);
    printNodeSyntax(node->node4, level + 1);
}

void printNodeN(NodeN * node, int level) {
    printfl(level);
    printf("%s, length: %d", enum_tostring(node->action), node->length);
    for (int i = 0; i < node->length; i++)
        printNodeSyntax(node->nodes[i], level + 1);
}

void printNodeSym(NodeSym * node, int level) {
    printfl(level);
    printf("%s (symbol)", node->name);
}

void printNodeVal(NodeVal * node, int level) {
    printfl(level);
    //printf("%s", enum_tostring(node->action));
    switch (node->action) {
case CONSTANT_CHAR: printf("%c (char)", node->value.i); break;
case CONSTANT_INT: printf("%d (int)", node->value.i); break;
case CONSTANT_DOUBLE: printf("%f (double)", node->value.d); break;
case STRING_LITERAL: printf("%s", node->value.s); break;
default: assert(0);
    }
}

void printNodeSyntax(NodeSyntax * node, int level) {
    if (!node) {
        printfl(level);
        printf("NULL NODE");
        return;
    }
    if (node < 2000) {
        printfl(level);
        printf("%s", enum_tostring(node));
        return;
    }
    switch (node->type) {
        case NODE_0: printNode0((Node0 *)node, level); break;
        case NODE_1: printNode1((Node1 *)node, level); break;
        case NODE_2: printNode2((Node2 *)node, level); break;
        case NODE_3: printNode3((Node3 *)node, level); break;
        case NODE_4: printNode4((Node4 *)node, level); break;
        case NODE_N: printNodeN((NodeN *)node, level); break;
        case NODE_SYM: printNodeSym((NodeSym *)node, level); break;
        case NODE_VAL: printNodeVal((NodeVal *)node, level); break;
        default: assert(0);
    }
}
