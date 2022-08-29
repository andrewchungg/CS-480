#include <iostream>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <pthread.h>
#include <fstream>

using namespace std;

#ifndef POPULATETREE_H
#define POPULATETREE_H

struct dictNode*initNode();
void add(struct dictNode *root, string s);
void *populateTree(void *arg);

#endif