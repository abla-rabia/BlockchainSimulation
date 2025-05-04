#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp.h"
#include <omp.h>

Transaction* create_transaction(int id, float amount, String sender, String receiver) {
    Transaction* transaction = (Transaction*)malloc(sizeof(Transaction));
    if (transaction == NULL) {
        return NULL;
    }
    
    transaction->id = id;
    transaction->amount = amount;
    transaction->sender = strdup(sender);
    transaction->receiver = strdup(receiver);
    transaction->hash = id + (int)amount + (int)sender[0] + (int)receiver[0]; // Simple hash function
    transaction->next = NULL;
    
    return transaction;
}

void add_transaction(Block* block, Transaction* transaction) {
    if (!can_add_transaction(block)) {
        return;
    }
    
    if (block->transactions == NULL) {
        block->transactions = transaction;
    } else {
        Transaction* current = block->transactions;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = transaction;
    }
}

bool can_add_transaction(Block* block) {
    return block->can_insert;
}

Block* create_block(int index, Transaction* transactions, int previous_hash) {
    Block* block = (Block*)malloc(sizeof(Block));
    if (block == NULL) {
        return NULL;
    }
    
    block->index = index;
    block->transactions = transactions;
    block->previous_hash = previous_hash;
    block->timestamp = time(NULL);
    block->can_insert = true;
    
    return block;
}

void add_block(Blockchain* blockchain, Block* block, Node* node,int hash_previous) {
    if (node->typeNode == "miner") {
        Block* last_block = blockchain->last_block;
        int merkle_root = calculate_merkle_root(last_block->transactions);
        if (merkle_root == hash_previous) {
            blockchain->last_block=block;
            blockchain->size++;
            block->next=NULL;
            last_block->next=block;
        }
        else {
            printf("Hash is not correct");
        }
    }
    else {
        printf("Node is not a miner");
    }
}

Blockchain* create_blockchain() {
    Blockchain* blockchain = (Blockchain*)malloc(sizeof(Blockchain));
    if (blockchain == NULL) {
        return NULL;
    }
    
    blockchain->blocks = NULL;
    blockchain->size = 0;
    
    return blockchain;
}





int calculate_merkle_root(Block* block){
    //a table to store the hash of the transactions
    int hash_table[100];
    //calculate the merkle root of the block
    Transaction* current = block->transactions;
    int i = 0;
    while (current  != NULL) {
        int hash = calculate_hash(current);
        current = current->next;
        hash_table[i] = hash;
        i++;
    }
    //next ...
}
int calculate_hash(Transaction* transaction){
    //hash the transaction by adding the sender and receiver
    int hash = 0;
    for (int i = 0; i < strlen(transaction->sender); i++) {
        hash += transaction->sender[i];
    }
    for (int i = 0; i < strlen(transaction->receiver); i++) {
        hash += transaction->receiver[i];
    }
    return hash;
}



//simulation de la réplication de la blockchain dans tous les noeuds
void replicate_blockchain(Blockchain* blockchain, Node* node){
    //on crée une nouvelle blockchain
    Blockchain* new_blockchain = create_blockchain();
    //on ajoute les blocks de la blockchain originale à la nouvelle blockchain
    Block* current = blockchain->blocks;
    while (current != NULL) {   
        add_block(new_blockchain, current, node, current->previous_hash);
        current = current->next;
    }
    //on ajoute la nouvelle blockchain à la liste des blockchains du noeud
    node->blockchains = new_blockchain;
    //on affiche la blockchain du noeud
    printf("Blockchain du noeud %s:\n", node->typeNode);
    current = node->blockchains->blocks;
    while (current != NULL) {
        printf("Block %d: %d\n", current->index, current->hash);
        current = current->next;
    }
}

void main(){
    Node nodes[3];
    //on crée une blockchain de départ comune
    Blockchain* blockchain = create_blockchain();
    //on remplie la blockchain avec des blocks de départ
    for (int i = 0; i < 10; i++) {
        add_block(blockchain, create_block(i, NULL, 0), "miner", 0);
    }
    //on crée 3 noeuds comme des thread
    #pragma omp parallel for
    for (int i = 0; i < 3; i++) {
        Node* node = create_node();
        if (i == 0) {
            node->typeNode = "miner";
        }
        else {
            node->typeNode = "user";
        }
        nodes[i]=node;
        replicate_blockchain(blockchain, node);
    }
    
}