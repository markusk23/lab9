#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

// HashType - hash table structure
struct HashType
{
    struct RecordType record;
    struct HashType* next;
};

// Compute the hash function
int hash(int x)
{
    return x % 23;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    struct HashType* temp;
    for (int i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].next != NULL) {
            printf("index %d -> ", i);
            temp = pHashArray[i].next;
            while (temp != NULL)
            {
                printf("%d, %c, %d", temp->record.id, temp->record.name, temp->record.order);
                if (temp->next != NULL)
                    printf(" -> ");
                temp = temp->next;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    int hashSz = 2 * recordSz;

    struct HashType* pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz);

    for (int i = 0; i < hashSz; ++i)
    {
        pHashArray[i].next = NULL;
    }

    for (int i = 0; i < recordSz; ++i)
    {
        int index = hash(pRecords[i].id);

        struct HashType* newNode = (struct HashType*)malloc(sizeof(struct HashType));
        newNode->record = pRecords[i];
        newNode->next = NULL;
        if (pHashArray[index].next == NULL) {
            pHashArray[index].next = newNode;
        } else {
            newNode->next = pHashArray[index].next;
            pHashArray[index].next = newNode;
        }
    }

    displayRecordsInHash(pHashArray, hashSz);
    free(pRecords);
    for (int i = 0; i < hashSz; ++i)
    {
        struct HashType* temp = pHashArray[i].next;
        while (temp != NULL)
        {
            struct HashType* next = temp->next;
            free(temp);
            temp = next;
        }
    }
    free(pHashArray);

    return 0;
}