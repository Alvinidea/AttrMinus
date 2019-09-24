#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<set>
#include<algorithm>
using namespace std;
#define ROWS 14
#define COLS 5
/*
单目运算符 从右到左

获取数据集合
*/
void getDateaSet(vector<vector<string>> *ds, int rows, int cols)
{
	
	ifstream input("data.txt");
	string str;
	if (input.is_open() == false)
	{
		cout << "file open fail !" << endl;
	}
	for (int row = 0; row < rows; row++)
	{
		vector<string> row_data;
		
		for (int col = 0; col < cols; col++)
		{
			input >> str;
			cout << str << ",";
			row_data.push_back(str);
		}
		ds->push_back(row_data);
		cout << endl;
	}
}


void getAttrVal(vector<vector<string>> *ds, vector<set<string>>* av)
{
	av->resize(4);
	for (int i = 0; i < ROWS; i++)
	{
		for (int j = 0; j < COLS - 1; j++)
		{
			set<string> *sets = &(*av)[j];
			string str = (*ds)[i][j];
			sets->insert(str);
		}
	}
	for (int j = 0; j < COLS - 1; j++)
	{
		set<string>::iterator it;
		set<string> sets = (*av)[j];
		cout << "属性 " << j + 1 << " : ";
		for (it = sets.begin(); it != sets.end(); it++)
		{
			cout << *it << '\t';
		}
		cout << endl;
	}
}
/*
	计算决策属性等价类
*/
void calIND_D(vector<vector<string>> *ds,vector<vector<int>>* D)
{
	int flag;
	vector<int> A,B;
	int t;
	D->resize(2);
	for (int row = 0; row < 14; row++)
	{
		string str = (*ds)[0][4];
		string str2 = (*ds)[row][4];
		if (flag = str.compare(str2))
		{
			t = row + 1;
			A.push_back(t);
			((*D)[0]).push_back(t);
		}
		else
		{
			t = row + 1;
			B.push_back(t);
			((*D)[1]).push_back(t);
		}
	}
	cout << "IND(D): { {";
	vector<int>::iterator it;
	for (it = A.begin(); it != A.end(); it++)
		cout << *it << ",";
	cout << "},{";
	for (it = B.begin(); it != B.end(); it++)
		cout << *it << ",";
	cout << "} }" << endl;
}

void setDivAttr(int dontConsider, vector<int> *needAttr,int NUM = 4)
{
	
	for (int i = 0; i < NUM; i++)
	{
		if (i != dontConsider)
		{
			needAttr->push_back(i);
		}
	}
}

//判断vector的某一元素是否存在
bool is_element_in_vector(vector<int> *v, int element) {
	vector<int>::iterator it;
	it = find(v->begin(), v->end(), element);
	if (it != v->end()) {
		return true;
	}
	else {
		return false;
	}
}

//集合 Y 是否 属于 X （Y是X的真子集）
bool is_vector_in_vector(vector<int> *X, vector<int> *Y) {
	vector<int>::iterator it;
	for (it = Y->begin(); it != Y->end(); it++)
	{
		int y_elem = *it;
		bool flag = is_element_in_vector( X, y_elem);
		if (flag == false)
		{
			return false;
		}
	}
	return true;
}

void calPOS(vector<vector<int>>* D, vector<vector<int>> *ind_p_ai, vector<int> *POS,int ai)
{
	vector<vector<int>>::iterator it_D;
	vector<vector<int>>::iterator it_P;
	vector<int>::iterator it;
	for (it_D = D->begin(); it_D != D->end(); it_D++)
	{
		vector<int> *X = &(*it_D);
		for (it_P = ind_p_ai->begin(); it_P != ind_p_ai->end(); it_P++)
		{
			vector<int> *Y = &(*it_P);
			// Y 属于等于 X时候
			bool is_belongTo = is_vector_in_vector(X, Y);
			if (is_belongTo == true)
			{
				for(it = Y->begin(); it != Y->end(); it++)
					POS->push_back(*it);
			}
		}
	}
	std::sort(POS->begin(), POS->end());
	if( ai != -1)
		cout << "POS p-a" << ai+1 << "(D) : { ";
	else
		cout << "POS p(D) : { ";
	for (it = POS->begin(); it != POS->end(); it++)
		cout << *it << ",";
	cout <<" }"<< endl;
}

void calINDP_ai(vector<vector<string>> *ds, vector<vector<int>> *ind_p_ai,int dontConsider)
{
	vector<int> *needAttr = new vector<int>();;
	
	vector<vector<int>>::iterator it_u;
	vector<int>::iterator it;
	int C[14] = { 0 };
	setDivAttr(dontConsider, needAttr);

	for (int z = 0; z < 14; z++)
	{
		if (C[z] == 0)
		{
			vector<int> type;
			for (int p = 0; p < 14; p++)
			{
				int a1 = (*needAttr)[0];
				int a2 = (*needAttr)[1];
				int a3 = (*needAttr)[2];
				if (((*ds)[z])[a1] == ((*ds)[p])[a1] &&
					((*ds)[z])[a2] == ((*ds)[p])[a2] &&
					((*ds)[z])[a3] == ((*ds)[p])[a3] )
				{
					C[p] = 1;
					type.push_back(p+1);

				}
			}
			ind_p_ai->push_back(type);
		}
	}
	cout <<"IND(P-a" <<dontConsider+1 <<") : { ";
	for (it_u = ind_p_ai->begin(); it_u != ind_p_ai->end(); it_u++)
	{
		cout << "{";
		vector<int> *type = &(*it_u);
		for (it = type->begin(); it != type->end(); it++)
		{
			cout << *it << ",";
		}
		cout << "}  ";
	}
	cout << " }" << endl;
}

void calIND_P(vector<vector<string>> *ds, vector<vector<int>> *ind_p)
{
	vector<int> *needAttr = new vector<int>();;

	vector<vector<int>>::iterator it_u;
	vector<int>::iterator it;
	int C[14] = { 0 };

	for (int z = 0; z < 14; z++)
	{
		if (C[z] == 0)
		{
			vector<int> type;
			for (int p = 0; p < 14; p++)
			{
				if (((*ds)[z])[0] == ((*ds)[p])[0] &&
					((*ds)[z])[1] == ((*ds)[p])[1] &&
					((*ds)[z])[2] == ((*ds)[p])[2] &&
					((*ds)[z])[3] == ((*ds)[p])[3])
				{
					C[p] = 1;
					type.push_back(p + 1);

				}
			}
			ind_p->push_back(type);
		}
	}
	cout << "IND(P) : { ";
	for (it_u = ind_p->begin(); it_u != ind_p->end(); it_u++)
	{
		cout << "{";
		vector<int> *type = &(*it_u);
		for (it = type->begin(); it != type->end(); it++)
		{
			cout << *it << ",";
		}
		cout << "}  ";
	}
	cout << " }" << endl;
}

void calRED(vector<vector<int>*>* POSs, vector<int> *pos_p, vector<vector<int>> *red )
{
	vector<vector<int>*>::iterator it_pos;
	bool flag;
	int ai = 0;
	for (it_pos = POSs->begin(); it_pos != POSs->end(); it_pos++)
	{
		flag = true;
		vector<int>* pos = *it_pos;
		if (pos->size() == pos_p->size())
		{
			std::sort(pos->begin(), pos->end());
			std::sort(pos_p->begin(), pos_p->end());
			for (int i = 0; i < pos->size(); i++)
			{
				if ((*pos)[i] != (*pos_p)[i])
				{
					flag == false;
				}
			}
			if (flag == true)
			{ 
				vector<int> re;
				for (int i = 0; i < COLS - 1; i++)
				{
					if (i != ai)
						re.push_back(i+1);
				}
				red->push_back(re);
			}
		}
		ai++;
	}
	vector<vector<int>>::iterator it_r;
	vector<int>::iterator it_r_r;
	cout << "RED :{ ";
	for (it_r = red->begin(); it_r != red->end(); it_r++)
	{
		cout << "{ ";
		for (it_r_r = (*it_r).begin(); it_r_r != (*it_r).end(); it_r_r++)
		{
			cout << *it_r_r <<",";
		}
		cout << " },";
	}
	cout << " }"<<endl;
}


//两个vector求交集
vector<int> vectors_intersection(vector<int> v1, vector<int> v2) {
	vector<int> v;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));//求交集 
	return v;
}

void calCORE(vector<vector<int>> *red, vector<int>* core)
{
	vector<vector<int>>::iterator it_r;
	vector<int> v;
	for (int i = 1; i < COLS ; i++)
	{
		v.push_back(i);
	}
	for (it_r = red->begin(); it_r != red->end(); it_r++)
	{
		v = vectors_intersection(*it_r,v);
	}
	core = &v;
	vector<int>::iterator it;
	cout << "CORE : { ";
	for (it = core->begin(); it != core->end(); it++)
	{
		cout << *it<<",";
	}
	cout << " }";
}

int main()
{
	// 存储数据表
	vector<vector<string>> *ds = new vector<vector<string>>();
	// 打印各个属性属性值
	vector<set<string>> *attrVal = new vector<set<string>>();
	// 存储 IND(D)
	vector<vector<int>>* D = new vector<vector<int>>();
	// 存储所有的 IND(P-ai)
	vector<vector<vector<int>>*> *ind_p = new vector<vector<vector<int>>*>();
	// 存储 IND(P)
	vector<vector<int>> *ind_p_ = new vector<vector<int>>();
	// 存储所有 POS p-ai(D)
	vector<vector<int>*>* POSs = new vector<vector<int>*>();
	// 存储 RED
	vector<vector<int>> *red = new vector<vector<int>>();
	// 存储 Core
	vector<int>* core = new vector<int>();

	getDateaSet(ds, 14,5);

	getAttrVal(ds, attrVal);

	calIND_D(ds, D);

	calIND_P(ds,ind_p_);

	vector<int> *POS_p = new vector<int>();
	calPOS(D, ind_p_, POS_p, -1);

	cout << endl;
	// 分别计算 IND  和  POS
	for (int i = 0; i < COLS - 1; i++)
	{
		// 计算 IND(P-ai)
		vector<vector<int>> *ind_p_ai = new vector<vector<int>>();
		calINDP_ai(ds, ind_p_ai, i);
		ind_p->push_back(ind_p_ai);
		// 计算 POS p-ai(D)
		vector<int> *POS = new vector<int>();
		calPOS(D, ind_p_ai, POS, i);
		POSs->push_back(POS);
		cout << endl;
	}
	// 计算RED
	calRED(POSs, POS_p, red);
	// 计算CORE
	calCORE(red,core);
}
