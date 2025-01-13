#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#define MAX_DATA_SIZE 256

typedef struct Transaction {
    char data[MAX_DATA_SIZE];
} Transaction;

#endif
