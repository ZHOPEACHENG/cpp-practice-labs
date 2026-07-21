#ifndef BOY_H
#define BOY_H

struct Boy {
	int code;
	Boy* pNext;
	Boy(int num) :code(num), pNext(nullptr) {};
};


int josephus_cll(int n, int m);
int josephus_arr(int n, int m);

#endif
