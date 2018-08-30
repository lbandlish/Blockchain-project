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


// string sha256(const string str)
// {
//     unsigned char hash[SHA256_DIGEST_LENGTH];
//     SHA256_CTX sha256;
//     SHA256_Init(&sha256);
//     SHA256_Update(&sha256, str.c_str(), str.size());
//     SHA256_Final(hash, &sha256);
//     stringstream ss;
//     for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
//     {
//         ss << hex << setw(2) << setfill('0') << (int)hash[i];
//     }
//     return ss.str();
// }       

// int main() {

//     // string s1 = sha256("123");
//     cout << s1 << endl; 

//     cout << sha256("1234567890_1") << endl;
//     cout << sha256("1234567890_2") << endl;
//     cout << sha256("1234567890_3") << endl;
//     cout << sha256("1234567890_4") << endl;
//     return 0;
// }

void sha256(char *str, char* outputBuffer);
void add_block( block* B, char* last_hash, unordered_map<char*, int> order, int i, char* keyin);

int main()
{
    unordered_map<char*, int> order;

    block* B = (block*)calloc(NUM_BLOCKS,sizeof(block));

    // input mechanism for new transactions. (what is a transaction?)
    // bitcoin's block no. 0 has prev. hash field 0000000..000

    char* last_hash = (char*)malloc(HASH_LEN*sizeof(char));

    for (int i = 0; i < HASH_LEN-1; i++)
        last_hash[i] = '0';    //  hash value for B[0].back 

    last_hash[HASH_LEN-1] = 0;

    int num_tran;
    cin >> num_tran;

    int size_tran;
    char* tran;

    for (int i = 0; i < num_tran; i++)
    {
        cin >> size_tran;
        tran = (char*)malloc((size_tran+1)*sizeof(char));
        cin >> tran;

        add_block(B, last_hash, order, i, tran); // i is the loop variable (represents order[new_hash] = i)

        puts(B[i].back);
        puts(last_hash);

    }

    // for (int i = 0; i < )

    return 0;
}

void add_block( block* B, char* last_hash, unordered_map<char*, int> order, int i, char* keyin)
{
    int s = strlen(keyin);
    B[i].key = (char*) malloc((s+1)*sizeof(char));  // s+1 to include null termination of string.

    strcpy(B[i].back, last_hash);
    strcpy(B[i].key, keyin);

    char* strForHash = (char*) malloc((s+HASH_LEN)*sizeof(char));

    strcpy(strForHash, B[i].back);  //
    strcat(strForHash, B[i].key);   //  Generate unique string for hash computation.

    sha256(strForHash,last_hash);   // stores latest hash-value in last_hash field.

    // add last_hash and i to order.
    order[last_hash] = i;
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

// int main()
// {
//     char* buffer = (char*)malloc(HASH_LEN*sizeof(char));

//     sha256("string1", buffer);
//     cout << buffer << endl;
//     sha256("string2", buffer);
//     cout << buffer << endl;

//     cout << strlen(buffer)<< endl;
    
// }