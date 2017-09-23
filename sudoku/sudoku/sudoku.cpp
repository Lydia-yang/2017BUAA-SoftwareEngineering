// sudoku.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
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
	void printsudoku();
	bool del(int judge, int blocknum, int pos);
};

void getsudoku(char * string);
void solve(sudoku sudo, int x[], int y[],int total, int & count);
bool isAllNum(char * string);
void produce(int total, int nums[], int block_num, int & count_total, int count_nums, sudoku s);


int main(int argc, char* argv[])
{
	int total = 0;
	char * s1, *s2;
	s1 = "-c";//生成数独终局
	s2 = "-s";//解数独

	int nums[9] = { 7,1,2,3,4,5,6,8,9 };
	int count_total = 0;
	sudoku s;
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
		produce(total, nums, 1, count_total, 0, s);

	}
	else if (strcmp(argv[1] , s2)==0) {//解数独
		getsudoku(argv[2]);
	}
	else {
		cout << "no such instruction" << endl;
		return 0;
	}
	
	
	cout << "end" << endl;
    return 0;
}

void getsudoku(char * string) {
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
		solve(sudo, x, y,total, count);
	}
	

}

void solve(sudoku sudo, int x[], int y[],int total, int & count) {
	int marked[9] = { 0 };
	int new_count = count;
	while (true) {

		int now = sudo.insert(1, x[new_count], y[new_count], marked);

		if (now < 0) return;
		else marked[now-1] = 1;

		if (new_count == total - 1) {
			//cout << count<<" "<<new_count << endl;
			sudo.printsudoku();
			return;
		}
		count = new_count + 1;
		solve(sudo, x, y, total, count);
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
void produce(int total, int nums[], int block_num, int & count_total, int count_nums, sudoku s) {
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
				s.printsudoku();//打印数独
				s.del(0, new_block_num, now);
				return;
			}
		}
		produce(total, nums, new_block_num, count_total, new_count_nums, s);
		if (count_total == total) return;
		s.del(0, new_block_num, now);
	}
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
	//随机生成一个0-8序列????????????????????????????????
	int pos1[9] = {0,1,2,3,4,5,6,7,8};
	int mark[9] = { 0 };
	int pos[9];
	
	srand((unsigned)(time(NULL) + num + blocknum));
	for (int i = 0; i < 9; i++) {
		int count = 0;
		int move = (rand() % (9-i) + 1);
	
		for (int j = 0; j < 9; j++) {
			if (mark[j]==0) count++;
			if (count == move) {
				pos[i] = pos1[j];
				mark[j] = 1;
				break;
			}
		}
		
	}


	int x;
	int y;
	for (int i = 0; i < 9; i++) {
		x = (pos[i] / 3) + ((blocknum-1)/3)*3;
		y = (pos[i] % 3 )+ (((blocknum-1)%3)*3);
		if (this->isinraw(num, x) || this->isincolumn(num, y) || this->array[x][y]!=0 || marked[pos[i]]==1) continue;
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
		if (this->isinblock(num[i], blocknum) || this->isincolumn(num[i], y) || this->isinraw(num[i], x) || marked[num[i]-1]==1) continue;
		else {
			this->array[x][y] = num[i];
			return num[i];
		}
	}
	return -1;
}

void sudoku::printsudoku() {
	ofstream f;
	f.open ("sudoku.txt", ios::app);
	if (!f) {
		cout << "sudoku.txt can't open" << endl;
		return;
	}
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			if (j == 8) f << this->array[i][j];
			else f << this->array[i][j] << " ";
			//cout << this->array[i][j] << " ";
		}
		f << endl;
	}
	f << endl;
	f.close();


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

