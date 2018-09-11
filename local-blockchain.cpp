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
    
    string back;
    vector<transaction> tr_seq;
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

    string last_hash;
    last_hash.resize(HASH_LEN - 1);

    for (int i = 0; i < HASH_LEN-1; i++)
    {
        last_hash[i] = '0';    //  hash value for B[0].back 
    }

    umap[last_hash] = -1;

/***************** input from file ******************************/

    FILE* fp = fopen("transactions.txt", "rb");
    if (fp==NULL) {fputs ("File error",stderr); exit (1);}

    fseek (fp , 0 , SEEK_END);
    int lSize = ftell (fp);
    rewind (fp);

    cout << "lsize: " << lSize << endl;

    char* buffer = (char*) malloc (sizeof(char)*(lSize+1));
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    int result = fread (buffer,1,lSize,fp);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    // cout << buffer << endl;

    // char buffer[] = "3\n\n0\nkey1\nvalue1\n\n1\nkey2\nvalue2\n\n0\nkey3\nvalue3";

    stringstream ss(buffer);

    cout << "file read over" << endl;

/***************** input over ***********************************/

    transaction input_tran;
    vector <transaction> tran;

    int num_trans;
    ss >> num_trans;

    cout << num_trans << "is the number of transactions" << endl;

    int num_blocks = (num_trans + TRAN_PER_BLOCK - 1) / TRAN_PER_BLOCK;
    // this is just num_trans/TRANS_PER_BLOCK ceiling formula.
    cout << num_blocks << "is the number of blocks" << endl;

    for (int i = 0; i < num_blocks; i++)
    {
        for (int j = 0; (j < TRAN_PER_BLOCK) && (i*num_blocks + j < num_trans); j++)
        {
            // cout << "trans[" << j << "] before adding to input_tran" << endl;
            ss >> input_tran.add_remove;
            ss >> input_tran.key;
            ss >> input_tran.value;
            // cout << "trans[" << j << "] after adding to input_tran" << endl;

            tran.push_back(input_tran);
            // cout << "trans[" << j << "] pushed to tran vector." << endl;

            cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;
        }

        for (int j = 0; (j < TRAN_PER_BLOCK) && (i*num_blocks + j < num_trans) ; j++)
        {
            cout << "intermediate: " << tran[j].value << endl;
        }

        cout << "block " << i << " transactions read into tran vector" << endl;
        cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;
        // cout << "last hash incoming : " << endl;
        // cout << endl;
        // cout << last_hash << endl;
        add_block(B, last_hash, umap, i, tran); // i is the loop variable (represents umap[new_hash] = i)
        cout << "add_complete" << endl;

        cout << "block " << i << " added" << endl;
        cout << "input_tran values: " << input_tran.add_remove << " " << input_tran.key << " " << input_tran.value << endl;

        for (int j = 0; (j < TRAN_PER_BLOCK) && (i*num_blocks + j < num_trans) ; j++)
        {
            tran.pop_back();
        }
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
    // cout << "before b[" << i << "].back" << endl;
    // cout << "last hash incoming : " << endl;
    // cout << last_hash << endl;
    // string xyz = "sdfa";
    // // strcpy(xyz, last_hash);
    // xyz = last_hash;
    B[i].back = last_hash;

    // B[i].back = last_hash;
    // cout << "after b[" << i << "].back" << endl;
    B[i].tr_seq = tran;

    // cout << endl;


    // cout << "tran" << endl;
    // cout << 
    // char* strForHash = (char*) malloc((s+HASH_LEN)*sizeof(char));

        string strForHash = B[i].back;     //
    // cout << "tran2" << endl;

        strForHash += B[i].tr_seq[0].value;   //  Generate unique string for hash computation.
    // cout << "tran3" << endl;
    
    // merkel_hash(B.tr_seq, last_hash);

    sha256(strForHash,last_hash);   // stores latest hash-value in last_hash field.
    // free(strForHash);
    // cout << "tran4" << endl;

    B[i].myhash = last_hash;
    // B[i].myhash = "asdfs";

    umap[last_hash] = i;
    // cout << last_hash;


    // cout << "tran5" << endl;
    
    cout << "{ " << B[i].tr_seq[0].value << endl;
    cout << B[i].tr_seq[1].value << endl;
    cout << B[i].tr_seq[2].value << " }" << endl;

    cout << "{ " << B[i].tr_seq[0].key << endl;
    cout << B[i].tr_seq[1].key << endl;
    cout << B[i].tr_seq[2].key << " }" << endl;

    cout << B[i].back << endl;
    cout << B[i].myhash << endl;

}

// void merkel_hash (vector< transaction > tran, string &last_hash)
// {
//     for (auto i = tran.begin(); i != tran.end(); i++)
//     {
//         *i = "0" + *i;
//         sha256(*i, *i);
//     }

//     find_merkel_root (last_hash, tran, tran.size(), 1);
// }

// void find_merkel_root (string &last_hash, vector < transaction > &tran, int size, int level)
// {
//     if (size == 2)
//     {
//         tran[0] = to_string(level) + tran[0];
//         tran[1] = to_string(level) + tran[1];
//         tran[0] = tran[0] + tran[1];

//         sha256(tran[0], last_hash);
//         return;
//     }

//     if (size % 2 != 0)
//     {
//         tran.push_back(*tran.top());
//         size++;
//     }

//     for (int i = 0; i < size; i++)
//     {
//         tran[i] = to_string(level) + tran[i];
//     }

//     for (int i = 0; i < size/2; i++)
//     {
//         tran[i] = tran[2i] + tran[2i+1];

//         sha256(tran[i], tran[i]);
//     }

//     find_merkel_root(last_hash, tran, size/2, level+1);
// }

void sha256(string str, string &outputBuffer)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    // SHA256_Update(&sha256, (char*)&str, str.length());
        SHA256_Update(&sha256, str.c_str(), str.length());
    SHA256_Final(hash, &sha256);

    char copywali[64];

    int i = 0;
    
    for(i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        // sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
        // sprintf((char*)&outputBuffer[i*2], "%02x", hash[i]);
        sprintf(copywali + (i*2), "%02x", hash[i]);
    }
    copywali[64] = 0;

    cout << copywali << endl;
    outputBuffer = copywali;
    cout << outputBuffer << endl;
    // outputBuffer[64] = 0;
}
