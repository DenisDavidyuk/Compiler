#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"

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
    NodeN * node = nodeBase;
    assert(node->type == NODE_N);
    node->length++;
    node->nodes = realloc(node->nodes, sizeof(NodeSyntax *) * node->length);
    node->nodes[node->length-1] = nodeNew;
    return node;
}

NodeSym * createNodeSym(char * name) {
    NodeSym * node = malloc(sizeof(NodeSym));
    node->type = NODE_SYM;
    node->name = name;
    return node;
}

NodeVar * createNodeVar(NodeAction action, YYSTYPE value) {
    NodeVar * node = malloc(sizeof(NodeVar));
    node->type = NODE_VAR;
    node->action = action;
    node->value = value;
    return node;
}

extern void printfl(int level);

void printNode0(Node0 * node, int level) {
    printfl(level);
    printf("%s, length: 0", enum_tostring(node->action));
}

void printNode1(Node1 * node, int level) {
    printfl(level);
    printf("%s, length: 1", enum_tostring(node->action));
    printNodeSyntax(node->node, level + 1);
}

void printNode2(Node2 * node, int level) {
    printfl(level);
    printf("%s, length: 2", enum_tostring(node->action));
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
}

void printNode3(Node3 * node, int level) {
    printfl(level);
    printf("%s, length: 3", enum_tostring(node->action));
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
    printNodeSyntax(node->node3, level + 1);
}

void printNode4(Node4 * node, int level) {
    printfl(level);
    printf("%s, length: 4", enum_tostring(node->action));
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
    printf("symbol: %s", node->name);
}

void printNodeVar(NodeVar * node, int level) {
    printfl(level);
    printf("%s", enum_tostring(node->action));
    printfl(level + 1);
    switch (node->action) {
/*case CONSTANT_CHAR: printf("%c", node->value.c); break;
case CONSTANT_INT: printf("%d", node->value.i); break;
case CONSTANT_DOUBLE: printf("%lf", node->value.d); break;
case STRING_LITERAL: printf("%s", node->value.s); break;*/
default: assert(0);
    }
}

void printNodeSyntax(NodeSyntax * node, int level) {
    if (!node) {
        printfl(level);
        printf("NULL NODE");
        return;
    }
    switch (node->type) {
        case NODE_1: printNode1((Node1 *)node, level); break;
        case NODE_2: printNode2((Node2 *)node, level); break;
        case NODE_3: printNode3((Node3 *)node, level); break;
        case NODE_4: printNode4((Node4 *)node, level); break;
        case NODE_N: printNodeN((NodeN *)node, level); break;
        case NODE_SYM: printNodeSym((NodeSym *)node, level); break;
        case NODE_VAR: printNodeVar((NodeVar *)node, level); break;
        default: assert(0);
    }
}
