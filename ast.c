#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"

NodeSyntax * createNode1(NodeAction action, NodeSyntax * node1) {
    Node1 * node = malloc(sizeof(Node1));
    node->type = NODE_1;
    node->action = action;
    node->node = node1;
    return node;
}

NodeSyntax * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2) {
    Node2 * node = malloc(sizeof(Node2));
    node->type = NODE_2;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    return node;
}

NodeSyntax * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3) {
    Node3 * node = malloc(sizeof(Node3));
    node->type = NODE_3;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    node->node3 = node3;
    return node;
}

NodeSyntax * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4) {
    Node4 * node = malloc(sizeof(Node4));
    node->type = NODE_4;
    node->action = action;
    node->node1 = node1;
    node->node2 = node2;
    node->node3 = node3;
    node->node4 = node4;
    return node;
}

NodeSyntax * createNodeN(NodeAction action) {
    NodeN * node = malloc(sizeof(NodeN));
    node->type = NODE_N;
    node->action = action;
    node->nodes = 0;
    node->length = 0;
    return node;
}

NodeSyntax * appendNodeN(NodeSyntax * nodeBase, NodeSyntax * nodeNew) {
    NodeN * node = nodeBase;
    assert(node->type == NODE_N);
    node->length++;
    node->nodes = realloc(node->nodes, sizeof(NodeSyntax *) * node->length);
    node->nodes[node->length-1] = nodeNew;
    return node;
}

NodeSyntax * createNodeSym(NodeAction action) {
    NodeSym * node = malloc(sizeof(NodeSym));
    node->type = NODE_SYM;
    node->action = action;
    return node;
}

static void printfl(int level) {
    printf("\n");
    while(level--)
        printf("  ");
}

void printNode1(Node1 * node, int level) {
    printfl(level);
    printf("%d, length: 1", node->action);
    printNodeSyntax(node->node, level + 1);
}

void printNode2(Node2 * node, int level) {
    printfl(level);
    printf("%d, length: 2", node->action);
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
}

void printNode3(Node3 * node, int level) {
    printfl(level);
    printf("%d, length: 3", node->action);
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
    printNodeSyntax(node->node3, level + 1);
}

void printNode4(Node4 * node, int level) {
    printfl(level);
    printf("%d", node->action);
    printNodeSyntax(node->node1, level + 1);
    printNodeSyntax(node->node2, level + 1);
    printNodeSyntax(node->node3, level + 1);
    printNodeSyntax(node->node4, level + 1);
}

void printNodeN(NodeN * node, int level) {
    printfl(level);
    printf("%d, length: %d", node->action, node->length);
    for (int i = 0; i < node->length; i++)
        printNodeSyntax(node->nodes[i], level + 1);
}

void printNodeSym(NodeSym * node, int level) {
    printfl(level);
    printf("%d", node->action);
}

void printNodeSyntax(NodeSyntax * node, int level) {
    if (!node) {
        printfl(level);
        printf("NULL");
        return;
    }
    switch (node->type) {
    case NODE_1: printNode1(node, level);
    case NODE_2: printNode2(node, level);
    case NODE_3: printNode3(node, level);
    case NODE_4: printNode4(node, level);
    case NODE_N: printNodeN(node, level);
    case NODE_SYM: printNodeSym(node, level);
    default: assert(0);
    }
}
