#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ast.h"
#include "enums.h"

NodeN * createNode0(NodeAction action) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 0;
    node->nodes = 0;
    return node;
}

NodeN * createNode1(NodeAction action, NodeSyntax * node1) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 1;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    return node;
}

NodeN * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 2;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    return node;
}

NodeN * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 3;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    node->nodes[2] = node3;
    return node;
}

NodeN * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->length = 4;
    node->nodes = malloc(node->length * sizeof(NodeSyntax *));
    node->nodes[0] = node1;
    node->nodes[1] = node2;
    node->nodes[2] = node3;
    node->nodes[3] = node4;
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
    assert(nodeBase->type == NODE_N);
    nodeBase->length++;
    nodeBase->nodes = realloc(nodeBase->nodes, sizeof(NodeSyntax *) * nodeBase->length);
    nodeBase->nodes[nodeBase->length-1] = nodeNew;
    return nodeBase;
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

NodeSyntax * popFirstNodeN(NodeN * node) {
    assert(node->type == NODE_N);
    NodeSyntax * res = node->nodes[0];
    node->length--;
    node->nodes = memmove(node->nodes, node->nodes + 1, sizeof(NodeSyntax *) * node->length);
    return res;
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

int compareNodeN(NodeN * node1, NodeN * node2) {
    if (node1->length != node2->length) return 0;
    if (node1->action != node2->action) return 0;
    int t = 1;
    for (int i = 0; i < node1->length; i++)
        t &= node1 == NULL && node2 == NULL ||
            compareNodeSyntax(node1->nodes[i], node2->nodes[i]);
    return t;
}

int compareNodeSym(NodeSym * node1, NodeSym * node2) {
    return strcmp(node1->name, node2->name) == 0;
}

int compareNodeVal(NodeVal * node1, NodeVal * node2) {
    if (node1->action != node2->action) return 0;
    switch (node1->action) {
        case CONSTANT_CHAR:
        case CONSTANT_INT: return node1->value.i == node2->value.i;
        case CONSTANT_DOUBLE: return node1->value.d == node2->value.d;
        case STRING_LITERAL: return strcmp(node1->value.s, node2->value.s) == 0;
        default: assert(0);
    }
}

int compareNodeSyntax(NodeSyntax * node1, NodeSyntax * node2) {
    int t = node1->type == node2->type;
    if (t) switch (node1->type) {
        case NODE_N: return compareNodeN((NodeN *)node1, (NodeN *)node2);
        case NODE_SYM: return compareNodeSym((NodeSym *)node1, (NodeSym *)node2);
        case NODE_VAL: return compareNodeVal((NodeVal *)node1, (NodeVal *)node2);
        default: assert(0);
    }
}

extern void printfl(int level);

void printNodeN(NodeN * node, int level) {
    printfl(level);
    if (node->length == 0)
        printf("%s", enum_tostring(node->action));
    else
        if (node->action == NONE) printf("length: %d", node->length);
        else printf("%s, length: %d", enum_tostring(node->action), node->length);
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
        printf("NULL (no syntax node)");
        return;
    }
    if (node < 2000) {
        printfl(level);
        printf("%s", enum_tostring(node));
        return;
    }
    switch (node->type) {
        case NODE_N: printNodeN((NodeN *)node, level); break;
        case NODE_SYM: printNodeSym((NodeSym *)node, level); break;
        case NODE_VAL: printNodeVal((NodeVal *)node, level); break;
        default: assert(0);
    }
}
