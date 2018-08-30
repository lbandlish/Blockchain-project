/*
 *  compile: g++ local-blockchain.cpp -lcrypto
 *
 *  Current issues:
 *      - blocks are in sequential order, can access last block by just using the latest index i
 *          + this can be avoided if blockchain is branced.
 *          + need to input prev block hash in transaction input to make that possible.
 *  
 */

#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <iomanip>

#define NUM_BLOCKS 100
#define HASH_LEN 65

using namespace std;

typedef struct block_structure {
    
    char back[HASH_LEN];
    char* key;

} block;

void sha256(char *str, char* outputBuffer);
void add_block( block* B, char* last_hash, unordered_map<char*, int> &umap, int i, char* keyin);


int main()
{
    unordered_map<char*, int> umap;

    block* B = (block*)calloc(NUM_BLOCKS,sizeof(block));

    char* last_hash = (char*)malloc(HASH_LEN*sizeof(char));

    for (int i = 0; i < HASH_LEN-1; i++)
        last_hash[i] = '0';    //  hash value for B[0].back 

    last_hash[HASH_LEN-1] = 0;

    umap[last_hash] = -1;

    cout << "Usage:" << endl;
    // cout << "./a.out" << endl;
    cout << "<number of transactions>" << endl;
    cout << "number of transactions times: <size of key> key" << endl; 

    int num_tran;
    cin >> num_tran;

    int size_tran;
    char* tran;

    for (int i = 0; i < num_tran; i++)
    {
        cin >> size_tran;
        tran = (char*)malloc((size_tran+1)*sizeof(char));
        cin >> tran;

        add_block(B, last_hash, umap, i, tran); // i is the loop variable (represents umap[new_hash] = i)

        puts(B[i].back);
        puts(last_hash);
        free(tran);
    }

    cout << "backward traversal of blockchain:" << endl;
    cout << endl;

    int ind = umap.find(last_hash)->second;

    cout << "ind initial value" << ind <<  endl;

    while (ind != -1)
    {
        cout << ind << endl;
        ind = umap.find(B[ind].back)->second;
    }

    return 0;
}

void add_block( block* B, char* last_hash, unordered_map<char*, int> &umap, int i, char* keyin)
{
    int s = strlen(keyin);
    B[i].key = (char*) malloc((s+1)*sizeof(char));  // s+1 to include null termination of string.

    strcpy(B[i].back, last_hash);
    strcpy(B[i].key, keyin);

    char* strForHash = (char*) malloc((s+HASH_LEN)*sizeof(char));

    strcpy(strForHash, B[i].back);  //
    strcat(strForHash, B[i].key);   //  Generate unique string for hash computation.

    sha256(strForHash,last_hash);   // stores latest hash-value in last_hash field.

    free(strForHash);
    // add last_hash and i to umap.
    umap[last_hash] = i;
}

void sha256(char *str, char* outputBuffer)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str, strlen(str));
    SHA256_Final(hash, &sha256);
    int i = 0;
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}
