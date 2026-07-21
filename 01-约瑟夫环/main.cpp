#include <iostream>
#include "boy.h"
using namespace std;



int main() {
	int n, m;
	cout << "total：";
	cin >> n;
	cout << "step：";
	cin >> m;

	//分别调用函数输出获胜者
	int winner = josephus_arr(n, m);
	cout << "(arr)Winner is :" << winner << endl;
	cout << endl;
	winner = josephus_cll(n, m);
	cout << "(cll)Winner is :" << winner << endl;

	system("pause");
	return 0;
}
