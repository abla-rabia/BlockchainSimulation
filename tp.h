#include <time.h>
#include <string.h>
#include <stdbool.h>

typedef struct Transaction {
    int id;
    float amount;
    String sender;
    String receiver;
    int hash;
    struct Transaction* next;
} Transaction;


typedef struct Block {
    int index;
    Transaction* transactions; 
    int previous_hash;
    time_t timestamp; 
    bool can_insert;
    Block* next;

} Block;

typedef struct Blockchain {
    Block* blocks;
    Block* last_block;
    int size;
} Blockchain;

typedef struct Node {
    String typeNode;
    Blockchain* blockchains;
}

//functions to manage transactions and blocks
Transaction* create_transaction(int id, float amount, String sender, String receiver);
void add_transaction(Block* block, Transaction* transaction);
bool can_add_transaction(Block* block);
Block* create_block(int index, Transaction* transactions, int previous_hash);
void add_block(Blockchain* blockchain, Block* block, Node* node,int hash_previous);
void stop_adding_transactions(Block* block);
//functions to manage the blockchain
Blockchain* create_blockchain();
int calculate_merkle_root(Block* block);
int calculate_hash(Transaction* transaction);




