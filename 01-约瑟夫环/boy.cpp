#include "boy.h"
#include<iostream>
#include<iomanip>
using namespace std;

//数组
int josephus_arr(int n, int m) {
	if (n == 0) {
		return 0;
	}
	bool alive[1000] = { false };
	//初始化全体成员存活
	for (int i = 1; i <= n; i++) {
		alive[i] = true;
	}
	int cnt = 0; //计数器
	int remain = n; //存活成员数
	int pos = 0; //当前成员位置
	while (remain > 1) {
		pos++;
		//超出范围则重置，形成循环
		if (pos > n) {
			pos = 1;
		}
		//存活成员才能参与
		if (alive[pos]) {
			cnt++;
			//淘汰对应成员
			if (cnt == m) {
				alive[pos] = false;
				cout << "淘汰：" << pos << endl;
				remain--;
				cnt = 0;
			}
		}
	}
	//遍历找出唯一1存活者
	for (int i = 1; i <= n; i++) {
		if (alive[i]) {
			return i;
		}
	}
}

//链表
int josephus_cll(int n, int m) {
	if (n == 0) {
		return 0;
	}
	int remain = n;
	//创建头节点
	Boy* head = new Boy(1);
	Boy* cur = head;
	//构建链表
	for (int i = 2; i <= n; i++) {
		cur->pNext = new Boy(i);
		cur = cur->pNext;
	}
	//形成循环
	cur->pNext = head;
	cur = head;
	//依次淘汰
	while (cur->pNext != cur) {
		for (int i = 1; i < remain + m - 1; i++) {
			cur = cur->pNext;
		}
		Boy* del = cur->pNext;
		cout << "淘汰：" << del->code << endl;
		cur->pNext = del->pNext; //重新指向下一个
		delete del;
		remain--;
		cur = cur->pNext; //继续计数
	}
	int winner = cur->code;
	delete cur; //释放内存
	return winner;
}
