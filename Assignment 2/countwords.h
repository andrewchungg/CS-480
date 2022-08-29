#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <pthread.h>
#include <sched.h>

using namespace std;

#ifndef COUNTWORDS_H
#define COUNTWORDS_H

int search(struct dictNode *root, string s);
void *countWords(void *arg);

#endif