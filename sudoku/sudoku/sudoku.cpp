// sudoku.cpp: 定义控制台应用程序的入口点。
//

//#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<string>
#include<stdlib.h>
#include<time.h>

using namespace std;


class sudoku {

	int array[9][9];

public:
	sudoku() {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				array[i][j] = 0;
			}
		}
		array[0][0] = 7;//设置第一个恒为7
	}
	sudoku(int array[9][9]) {
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				this->array[i][j] = array[i][j];
			}
		}
	}

	bool isinraw(int num, int raw_num);//数字是否在一行中已存在
	bool isincolumn(int num, int c_num);//数字是否在一列中已存在
	bool isinblock(int num, int blocknum);//数字是否在3*3的小块中已存在
	int insert(int num, int blocknum, int marked[]);
	int insert(int num, int x, int y, int marked[]);
	void printsudoku(char *s, int &count_s);
	bool del(int judge, int blocknum, int pos);
};

void getsudoku(char * string, char *str, int &count_s);
void solve(sudoku sudo, int x[], int y[],int total, int & count, char *str, int &count_s);
bool isAllNum(char * string);
void produce(int total, int nums[], int block_num, int & count_total, int count_nums, sudoku s, char *str, int &count_s);
void outputfile(char* s, int &count_s);

int main(int argc, char* argv[])
{
	int total = 0;
	char * s1, *s2;
	s1 = "-c";//生成数独终局
	s2 = "-s";//解数独

	char * str = new char [100000];//用来存储输出
	int count_s = 0;

	srand((unsigned)(time(NULL)));//随机数种子

	int nums1[9] = { 7,1,2,3,4,5,6,8,9 };
	int nums[9] = {7, 0 };
	int mark[8] = { 0 };
	int count_total = 0;
	sudoku s;
	//随机初始化
	for (int i = 1; i < 9; i++) {
		int count = 0;
		int move = (rand() % (8-i+1) + 1);

		for (int j = 1; j < 9; j++) {
			if (mark[j-1]==0) count++;
			if (count == move) {
				nums[i] = nums1[j];
				mark[j-1] = 1;
				break;
			}
		}
	//cout << nums[i] << " ";
	}
	//cout << endl;

	if (argc < 3) {
		cout << "the number of arguments is at least two" << endl;
		return 0;
	}
	if (argc > 3) cout << "too many arguments " << endl;
	if (strcmp(argv[1] , s1)==0) {//生成数独终局
		if (!isAllNum(argv[2])) {
			cout << "error: argument is not a positive integer" << endl;
			return 0;
		}
		total = atoi(argv[2]);
		if (total <= 0) {
			cout << "error: argument is not a positive integer" << endl;
			return 0;
		}
		produce(total, nums, 1, count_total, 0, s, str, count_s);
		
	}
	else if (strcmp(argv[1] , s2)==0) {//解数独

		getsudoku(argv[2],str, count_s);

	}
	else {
		cout << "no such instruction" << endl;
		return 0;
	}
//	getsudoku("a.txt", str, count_s);
//	produce(10000, nums, 1, count_total, 0, s, str, count_s);
	outputfile(str, count_s);
//	delete[] str;
    return 0;
}

void getsudoku(char * string, char *str, int &count_s) {
	int sudo[9][9];
	int x[81];
	int y[81];
	int count = 0, total = 0;
	fstream f(string);
	if (!f) {
		cout << string << " can't open" << endl;
		return;
	}
	while (!f.eof()) {
		
		count = 0;
		total = 0;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 9; j++) {
				f >> sudo[i][j];
				if (f.eof()) return;
				if (sudo[i][j] == 0) {
					x[total] = i;
					y[total] = j;
					total++;
				}
			}
		}
		sudoku s(sudo);
		solve(sudo, x, y,total, count, str, count_s);
	}

}

void solve(sudoku sudo, int x[], int y[],int total, int & count, char *str, int &count_s) {
	int marked[9] = { 0 };
	int new_count = count;
	while (true) {

		int now = sudo.insert(1, x[new_count], y[new_count], marked);

		if (now < 0) return;
		else marked[now-1] = 1;

		if (new_count == total - 1) {
			//cout << count<<" "<<new_count << endl;
			sudo.printsudoku(str, count_s);
			return;
		}
		count = new_count + 1;
		solve(sudo, x, y, total, count, str, count_s);
		if (count == total - 1) return;
		sudo.del(1, x[new_count], y[new_count]);
	}
	
}

bool isAllNum(char * string) {//支持正号
	if (*string == '+') string++;
	while (*string != '\0') {
		if ((*string - '0') > 9 || (*string - '0' ) < 0) return false;
		string++;
	}
	return true;
}

void produce(int total, int nums[], int block_num, int & count_total, int count_nums, sudoku s, char *str, int &count_s) {
	int marked[9] = { 0 };//标记已经试过的位置
	int new_block_num, new_count_nums;
	
	while (true) {
		new_block_num = block_num + 1;
		new_count_nums = count_nums;
	
		int now = s.insert(nums[new_count_nums], new_block_num, marked);
	
		if (now <0) return;
		else marked[now] = 1;

		if (new_block_num == 9) {
			if (new_count_nums < 8) {
				new_count_nums=count_nums+1;
				new_block_num = 0;
			}
			else {//填写至最后一个
				count_total++;
				s.printsudoku(str, count_s);//打印数独
				s.del(0, new_block_num, now);
				return;
			}
		}
		produce(total, nums, new_block_num, count_total, new_count_nums, s, str, count_s);
		if (count_total == total) return;
		s.del(0, new_block_num, now);
	}
}

void outputfile(char *s, int &count_s) {
	if (count_s == 0) return;
	ofstream f;
	f.open("sudoku.txt", ios::app);
	if (!f) {
		cout << "sudoku.txt can't open" << endl;
		return;
		
	}
	s[count_s] = '\0';
	f << s;
	//cout << s;
	f.close();
	count_s = 0;
}

//function in sudoku
bool sudoku::isinraw(int num, int raw_num) {//行
	for (int i = 0; i < 9; i++) {
		if (this->array[raw_num][i] == num) return true;
	}
	return false;
}
bool sudoku::isincolumn(int num, int c_num) {//列
	for (int i = 0; i < 9; i++) {
		if (this->array[i][c_num] == num) return true;
	}
	return false;
}
bool sudoku::isinblock(int num, int blocknum) {//块
	int x, y;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			x = i +((blocknum - 1) / 3) * 3;
			y = j + (((blocknum - 1) % 3) * 3);
			if (this->array[x][y] == num) return true;
		}
	}
	return false;
}
int sudoku::insert(int num, int blocknum, int marked[]) {
	
	int pos[9] = {0,1,2,3,4,5,6,7,8};

	int x;
	int y;
	for (int i = 0; i < 9; i++) {
		x = (pos[i] / 3) + ((blocknum-1)/3)*3;
		y = (pos[i] % 3 )+ (((blocknum-1)%3)*3);
		if (this->array[x][y] != 0 || marked[pos[i]] == 1 || this->isinraw(num, x) || this->isincolumn(num, y)) continue;
		else {
			this->array[x][y] = num;
			return pos[i];
		}
	}
	return -1;
}

int sudoku:: insert(int n, int x, int y, int marked[]) {
	
	int num[9] = { 1,2,3,4,5,6,7,8,9 };
	int blocknum = x/3*3 + y/3 + 1;

	for (int i = 0; i < 9; i++) {
		if ( marked[num[i] - 1] == 1 || this->isinblock(num[i], blocknum) || this->isincolumn(num[i], y) || this->isinraw(num[i], x) ) continue;
		else {
			this->array[x][y] = num[i];
			return num[i];
		}
	}
	return -1;
}

void sudoku::printsudoku(char *s, int &count_s) {
	//cout << "2";
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (count_s == 100000) outputfile(s, count_s);
			s[count_s++] =( this->array[i][j]+'0');

			if (count_s == 100000) outputfile(s, count_s);

			if (j != 8) s[count_s++] = ' ';

			if (count_s == 100000) outputfile(s, count_s);
		}
		//f << endl;
		s[count_s++] = '\n';

		if (count_s == 100000) outputfile(s, count_s);
	}
	//f << endl;
	s[count_s++] = '\n';

	if (count_s == 100000) outputfile(s, count_s);

}
bool sudoku::del(int judge, int blocknum, int pos) {
	if (judge == 1) {
		this->array[blocknum][pos] = 0;
		return true;
	}
	int x;
	int y;
	x = (pos / 3) + ((blocknum - 1)/3)*3;
	y = (pos % 3) + (((blocknum - 1) % 3) * 3);
	this->array[x][y] = 0;
	return true;
}

