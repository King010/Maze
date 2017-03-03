#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;


#define MAX_ROW 10
#define MAX_COL 10
#include<fstream>
#include<string>
#include<stack>
struct Seat
{
	int _x;
	int _y;
	Seat(int x , int y )
		: _x(x)
		, _y(y)
	{}
};
//1、地图数据保存文件中
//2、地图空间动态创建出来
class Maze
{
public:

	Maze(const string& fileName)
	{
		string strBuff;
		string strData;
		fstream fin(fileName);
		getline(fin, strBuff);
		strData = strBuff.substr(0, strBuff.find_first_of(','));
		 ROW = atoi(strData.c_str());//按照const char*返回出来

		strData = strBuff.substr(strBuff.find_first_of(',') + 1);
		 COL = atoi(strData.c_str());

		_map = new int*[ROW];
		for (int idx = 0; idx < ROW; idx++)
		{
			_map[idx] = new int[COL];
		}

		int rowIdx = 0, colIdx = 0;
		while (!fin.eof())
		{
			getline(fin, strData);
			char* start = (char*)strData.c_str();
			while (*start)
			{
				if (*start >= '0'&&*start <= '9')
				{
					_map[rowIdx][colIdx++] = *start - '0';
				}
				start++;
			}
			rowIdx += 1;
			colIdx = 0;
		}
		fin.close();
	}
	Maze(int map[][MAX_COL], int row)
	{
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < MAX_COL; ++j)
			{
				_map[i][j] = map[i][j];
			}
		}
	}

	bool IsPass(const Seat& s)
	{
		if (s._x < 0 || s._y < 0 || s._y >= MAX_COL)
			return true;

		if (1 == _map[s._x][s._y])
			return true;

		return false;
	}

	bool PassMaze(const Seat& Entry)//递归
	{
		if (Entry._x < 0 || Entry._y < 0 || Entry._y >= MAX_COL)
			return true;

		if (IsPass(Entry))
		{
			_map[Entry._x][Entry._y] = 2;
			// 超前
			Seat front(Entry._x - 1, Entry._y);
			if (PassMaze(front))
			{
				return true;
			}
			
			// 超左
			Seat left(Entry._x, Entry._y - 1);
			if (PassMaze(left))
			{
				return true;
			}
			// 超右边
			Seat right(Entry._x, Entry._y + 1);
			if (PassMaze(right))
			{
				return true;
			}
			_map[Entry._x][Entry._y] = 3;
		}

		return false;
	}



	bool PassMaze(std::stack<Seat>&s, const Seat& Entry)//循环
	{
		if (!IsPass(Entry))
			return false;
		
			s.push(Entry);
			while (!s.empty())
			{
				Seat Cur = s.top();
				if (Cur._x < 0 || Cur._y < 0 || Cur._y >= MAX_COL)
				{
					s.pop();
					return true;
				}
					_map[Cur._x][Cur._y] = 2;
				//前

				Seat Front(Cur._x - 1, Cur._y);
				
				if (IsPass(Front))
				{
					s.push(Front);
					continue;
				}
				//左
				Seat left (Cur._x , Cur._y-1);
				if (IsPass(left))
				{
					s.push(left);
					continue;
				}
				//右
				Seat right(Cur._x, Cur._y + 1);
				if (IsPass(right))
				{
					s.push(right);
					continue;
				}
				_map[Cur._x][Cur._y] = 3;
				s.pop();
			}
		
		return false;
	}


	void PrintMap()
	{
		for (int i = 0; i < MAX_ROW; ++i)
		{
			for (int j = 0; j < MAX_COL; ++j)
			{
				cout << _map[i][j] << " ";
			}
			cout << endl;
		}
	}
	~Maze()
	{
		for (int i = 0; i < ROW; ++i)
		{
			delete[] _map[i];
		}
		delete[] _map;
	}
private:
	int **_map;
	int ROW, COL;
};

void FunTest1()//递归调用
{
	int mapArr[MAX_ROW][MAX_COL] = {
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 } };
	Maze maze("map.txt");
	//Maze maze(mapArr, MAX_ROW);
	maze.PrintMap();
	cout << endl;
	stack<int>s;
	Seat s1(9, 6);
	maze.PassMaze(s1);
	cout << endl;
	maze.PrintMap();
}

void FunTest2()//循环
{
	int mapArr[MAX_ROW][MAX_COL] = {
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 1, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 1, 1, 1, 1, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0 } };
	Maze maze("map.txt");
	//Maze maze(mapArr, MAX_ROW);
	maze.PrintMap();
	cout << endl;
	stack<Seat>s;
	Seat s1(9, 6);
	maze.PassMaze(s, s1);
	cout << endl;
	maze.PrintMap();
}

int main()
{
	FunTest1();
	system("pause");
	return 0;
}