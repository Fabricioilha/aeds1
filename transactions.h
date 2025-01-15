#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

typedef struct Transaction {
    char sender[50];
    char receiver[50];
    int amount;
} Transaction;


#endif
