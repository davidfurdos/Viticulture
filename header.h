#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>

#ifndef HEADER_H
#define HEADER_H

#define maxworkers 68

char *weekdaynames[7] = {"hétfő", "kedd", "szerda", "csütörtök", "péntek", "szombat", "vasárnap"};
int capacities[7] = {10, 13, 4, 12, 10, 10, 10};

char *readmenu;
char *name;
size_t len;

struct Worker
{
    int id;
    char *name;
};

struct message
{
    long mid;
    int passengernum;
};

int **matrix;
struct Worker *workers;

// 1. beadandó

void initmatrix();

void initworkers();

void freematrix();

void freeworkers();

void readfromfile();

void writetofile();

int getnum(int numlength, int min, int max);

int checkday(char *input);

int countworkers();

void applytowork();

void *setshift(int id);

int getworker(int id);

void changename(int id);

void deleteworker(int id);

void printdailyworkers(int index);

void printallworker();

// 2. beadandó

void handler(int signumber);

struct Worker *initworkersonbus(int passengers);

struct Worker *getdailyworkers(int index);

struct Worker *checkdailyworkers(int index, struct Worker *localworkers);

int getdailynumofworkers(int index, struct Worker *localworkers);

struct Worker *formatdailyworkers(int length, struct Worker *localworkers);

int sendmsg(int messagequeue, long id, int numofpassengers);

int recivemsg(int messagequeue, long id);

void transportworkers(int dayindex, char *argv);

#endif