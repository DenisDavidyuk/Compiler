#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

typedef enum NodeType {
    NODE_1,
    NODE_2,
    NODE_3,
    NODE_4,
    NODE_N,
    NODE_SYM
} NodeType;

typedef enum NodeAction {
    /* 1..255 - chars*/
    /* 258..315 - enum yytokentype */
    INC_PREF  = 1000,
    INC_SUFF  = 1001,
    DEC_PREF  = 1002,
    DEC_SUFF  = 1003,
    ARR_OP    = 1004,
    CALL_OP   = 1005,
    FIELD_OP  = 1006,
    CAST_OP   = 1007,
    TERNAL_OP = 1008,
    CONST_CK  = 1009,
    DECL_OP   = 1010,
    WHILE_PRE,
    WHILE_POST,
    COMPAUND,
    NONE
} NodeAction;

typedef struct NodeSyntax {
    NodeType type;
} NodeSyntax;

typedef struct Node1 {
    NodeType type;
    NodeAction action;
    struct NodeBase * node;
} Node1;

typedef struct Node2 {
    NodeType type;
    NodeAction action;
    struct NodeBase * node1, * node2;
} Node2;

typedef struct Node3 {
    NodeType type;
    NodeAction action;
    struct NodeBase * node1, * node2, * node3;
} Node3;

typedef struct Node4 {
    NodeType type;
    NodeAction action;
    struct NodeBase * node1, * node2, * node3, * node4;
} Node4;

typedef struct NodeSym {
    NodeType type;
    NodeAction action;
} NodeSym;

typedef struct NodeN {
    NodeType type;
    NodeAction action;
    int length;
    struct NodeBase ** nodes;
} NodeN;

NodeSyntax * createNode1(NodeAction action, NodeSyntax * node);
NodeSyntax * createNode2(NodeAction action, NodeSyntax * node1, NodeSyntax * node2);
NodeSyntax * createNode3(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3);
NodeSyntax * createNode4(NodeAction action, NodeSyntax * node1, NodeSyntax * node2, NodeSyntax * node3, NodeSyntax * node4);
NodeSyntax * createNodeN(NodeAction action);
NodeSyntax * appendNodeN(NodeSyntax * NodeN, NodeSyntax * node);
NodeSyntax * createNodeSym(NodeAction action);

void printNode1(Node1 * node, int level);
void printNode2(Node2 * node, int level);
void printNode3(Node3 * node, int level);
void printNode4(Node4 * node, int level);
void printNodeN(NodeN * node, int level);
void printNodeSym(NodeSym * node, int level);
void printNodeSyntax(NodeSyntax * node, int level);

#endif // AST_H_INCLUDED
