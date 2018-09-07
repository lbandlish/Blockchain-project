/*
 *  compile: g++ local-blockchain.cpp -lcrypto
 */     

#include <bits/stdc++.h>
#include <openssl/sha.h>
#include <iomanip>

#define NUM_BLOCKS 100
#define HASH_LEN 65
#define TRAN_PER_BLOCK 3

using namespace std;

typedef unordered_map<string, int> u_map;

typedef struct transaction_structure {

    int add_remove ; // 0 for adding and 1 for removing
    string key;
    string value;

} transaction;

typedef struct block_structure {
    
    // char back[HASH_LEN];
    string back;
    vector<transaction> tr_seq;
    // char myhash[HASH_LEN];
    string myhash;

} block;

void sha256(string str, string &outputBuffer);
void add_block( block* B, string &last_hash, u_map &umap, int i, vector<transaction> tran);

void printumap(u_map &umap)
{
    cout << "printumap" << endl;

    for (auto i = umap.begin(); i != umap.end(); i++)
    {
        cout << i->second << " ";
    }

    cout << endl;
}

int main()
{
    u_map umap;

    block* B = (block*)calloc(NUM_BLOCKS,sizeof(block));

    // char* last_hash = (char*)malloc(HASH_LEN*sizeof(char));
    string last_hash;

    for (int i = 0; i < HASH_LEN-1; i++)
        last_hash[i] = '0';    //  hash value for B[0].back 

    last_hash[HASH_LEN-1] = 0;

    umap[last_hash] = -1;

    cout << "Usage:" << endl;
    cout << "<number of transactions>" << endl;
    cout << "number of transactions times: <size of key> key" << endl; 


/***************** input from file ******************************/

    FILE* fp = fopen("transactions.txt", "r");
    if (fp==NULL) {fputs ("File error",stderr); exit (1);}

    fseek (fp , 0 , SEEK_END);
    int lSize = ftell (fp);
    rewind (fp);

    char* buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    int result = fread (buffer,1,lSize,fp);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    stringstream ss(buffer);

    cout << "read file" << endl;

/***************** input over ***********************************/

    transaction input_tran;
    vector <transaction> tran;

    int num_trans;
    ss >> num_trans;

    cout << num_trans << "is the number of transzctions" << endl;

    int num_blocks = (num_trans + TRAN_PER_BLOCK - 1) / TRAN_PER_BLOCK;
    // this is just num_trans/TRANS_PER_BLOCK ceiling formula.
    cout << num_blocks << "is the number of blocks" << endl;


    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = 0; j < TRAN_PER_BLOCK; j++)
        {
            cout << "trans[" << j << "] read" << endl;
            ss >> input_tran.add_remove;
            ss >> input_tran.key;
            ss >> input_tran.value;
            cout << "trans[" << j << "] almost read";

            tran.push_back(input_tran);
            cout << "trans[" << j << "] added";

            cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;

        }

        for (int j = 0; j < 3; j++)
        {
            cout << "intermediate: " << tran[j].value << endl;

        }

        cout << "block " << i << " read" << endl;
        cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;
        
        add_block(B, last_hash, umap, i, tran); // i is the loop variable (represents umap[new_hash] = i)
        cout << "add_complete" << endl;

        cout << "block " << i << " added" << endl;
        cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;

        // puts(B[i].back);
        // puts(last_hash);
        // cout << endl;
        // free(tran);
    }



    cout << endl;
    cout << "backward traversal of blockchain:" << endl;
    cout << endl;

    int ind = umap.find(last_hash)->second;

    while (ind != -1)
    {
        cout << ind << endl;
        ind = (umap.find(B[ind].back))->second;
    }

    return 0;
}

void add_block( block* B, string &last_hash, u_map &umap, int i, vector<transaction> tran)
{
    // int s = strlen(keyin);
    // B[i].key = (char*) malloc((s+1)*sizeof(char));  // s+1 to include null termination of string.

    // strcpy(B[i].back, last_hash);
    // strcpy(B[i].key, keyin);
    cout << "before b[" << i << "].back" << endl;
    
    string xyz = "asdfs";
    B[i].back = xyz;

    // B[i].back = last_hash;
    cout << "after b[" << i << "].back" << endl;
    B[i].tr_seq = tran;
    cout << "tran" << endl;
    // cout << 
    // char* strForHash = (char*) malloc((s+HASH_LEN)*sizeof(char));

    string strForHash = B[i].back;     //
    cout << "tran2" << endl;

    strForHash += B[i].tr_seq[0].value;   //  Generate unique string for hash computation.
    cout << "tran3" << endl;
    
    sha256(strForHash,last_hash);   // stores latest hash-value in last_hash field.
    // free(strForHash);
    cout << "tran4" << endl;

    // B[i].myhash = last_hash;
    B[i].myhash = "asdfs";

    umap[last_hash] = i;
    cout << "tran5" << endl;


}

void sha256(string str, string &outputBuffer)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, (char*)&str, strlen((char*)&str));
    SHA256_Final(hash, &sha256);

    int i = 0;
    
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        // sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
        sprintf((char*)&outputBuffer[i*2], "%02x", hash[i]);
    }

    outputBuffer[64] = 0;
}
