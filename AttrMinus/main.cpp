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
��Ŀ����� ���ҵ���

��ȡ���ݼ���
*/
void getDateaSet(vector<vector<string>> *ds, int *rows, int cols)
{
	cout << "==========================================================" << endl;
	cout << "=======================���ݶ�ȡ===========================" << endl;
	cout << "==========================================================" << endl;
	int count = 0;
	ifstream input("data.txt");
	string str;
	if (input.is_open() == false)
	{
		cout << "file open fail !" << endl;
	}
	while( ! input.eof())
	//for (int row = 0; row < rows; row++)
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
		count++;
	}
	*rows = count;
}


void getAttrVal(vector<vector<string>> *ds, vector<set<string>>* av,int rows= ROWS,int cols= COLS)
{
	cout << "==========================================================" << endl;
	cout << "=======================��ӡ����ֵ=========================" << endl;
	cout << "==========================================================" << endl;
	av->resize(4);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols - 1; j++)
		{
			set<string> *sets = &(*av)[j];
			string str = (*ds)[i][j];
			sets->insert(str);
		}
	}
	for (int j = 0; j < cols - 1; j++)
	{
		set<string>::iterator it;
		set<string> sets = (*av)[j];
		cout << "���� " << j + 1 << " : ";
		for (it = sets.begin(); it != sets.end(); it++)
		{
			cout << *it << '\t';
		}
		cout << endl;
	}
}
/*
	����������Եȼ���
*/
void calIND_D(vector<vector<string>> *ds,vector<vector<int>>* D, int rows = ROWS, int cols = COLS)
{
	int flag;
	vector<int> A,B;
	int t;
	D->resize(2);
	for (int row = 0; row < rows; row++)
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



//�ж�vector��ĳһԪ���Ƿ����
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

//���� Y �Ƿ� ���� X ��Y��X�����Ӽ���
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

void calPOS(vector<vector<int>>* D, vector<vector<int>> *ind_p_ai, vector<int> *POS)
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
			// Y ���ڵ��� Xʱ��
			bool is_belongTo = is_vector_in_vector(X, Y);
			if (is_belongTo == true)
			{
				for(it = Y->begin(); it != Y->end(); it++)
					POS->push_back(*it);
			}
		}
	}
	std::sort(POS->begin(), POS->end());
	cout << "POS p-..."  << "(D) : { ";

	for (it = POS->begin(); it != POS->end(); it++)
		cout << *it << ",";
	cout <<" }"<< endl;
}

// IND(P-ai)
void calINDP_ai(vector<vector<string>> *ds, vector<vector<int>> *ind_p_ai,int dontConsider, int rows = ROWS, int cols = COLS)
{
	vector<int> *needAttr = new vector<int>();;
	
	vector<vector<int>>::iterator it_u;
	vector<int>::iterator it;
	//������� 
	int *C = new int[rows];
	for (int z = 0; z < rows; z++)
	{
		C[z] = 0;
	}
	setDivAttr(dontConsider, needAttr);

	for (int z = 0; z < rows; z++)
	{
		if (C[z] == 0)
		{
			vector<int> type;
			for (int p = 0; p < rows; p++)
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


void setDivAttr(vector<int> *dontConsider, vector<int> *needAttr, int NUM = 4)
{
	bool flag = true;
	for (int i = 0; i < NUM; i++)
	{
		flag = true;
		for (int _att = 0; _att < dontConsider->size(); _att++)
		{
			if (i == (*dontConsider)[_att])
			{//���ںͲ�����������ͬ��ʱ�� 
				flag = false;
			}
		}
		if(flag == true)
		{
			needAttr->push_back(i);
		}
	}
}

/*
ds �����ݼ���
ind_p_ai�����ؽ��
dontConsider�������ǵ�����
dataNum��������Ŀ

IND(P-{ai,aj,...,an})
*/
void calINDP_matli(vector<vector<string>> *ds, vector<vector<int>> *ind_p_ai, vector<int> *dontConsider,int rows = ROWS)
{
	//��Ҫ���ǵ�����
	vector<int> *needAttr = new vector<int>();;

	vector<vector<int>>::iterator it_u;
	vector<int>::iterator it;
	//������� 
	int *C = new int[rows];
	for (int z = 0; z < rows; z++)
	{
		C[z] = 0;
	}

	setDivAttr(dontConsider, needAttr);

	for (int z = 0; z < rows; z++)
	{
		if (C[z] == 0)
		{//��ֹ����ж� ��������һ���������
			vector<int> type;
			for (int p = 0; p < rows; p++)
			{
				bool flag = true;
				int att;
				//ѭ���ж�Ҫ���ǵ����� ������ʱ��flag = true��
				for (int _attr = 0; _attr < needAttr->size(); _attr++)
				{
					att = (*needAttr)[_attr];
					if (((*ds)[z])[att] != ((*ds)[p])[att])
					{
						flag = false;
					}
				}
				if ( flag == true )
				{
					C[p] = 1;
					type.push_back(p + 1);
				}
			}
			ind_p_ai->push_back(type);
		}
	}
	cout << "\nIND(P-{";
	for (int t = 0; t < dontConsider->size(); t++)
	{
		cout << (*dontConsider)[t] + 1<<',';
	}
	cout << "}) : { ";
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

// IND(P)
void calIND_P(vector<vector<string>> *ds, vector<vector<int>> *ind_p, int rows = ROWS)
{
	vector<int> *needAttr = new vector<int>();;

	vector<vector<int>>::iterator it_u;
	vector<int>::iterator it;
	//������� 
	int *C = new int[rows];
	for (int z = 0; z < rows; z++)
	{
		C[z] = 0;
	}

	for (int z = 0; z < rows; z++)
	{
		if (C[z] == 0)
		{
			vector<int> type;
			for (int p = 0; p < rows; p++)
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
// ��RED
void calRED(vector<vector<int>*>* POSs, vector<int> *pos_p, vector<vector<int>> *red, int rows = ROWS, int cols = COLS)
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
				for (int i = 0; i < cols - 1; i++)
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


//����vector�󽻼�
vector<int> vectors_intersection(vector<int> v1, vector<int> v2) {
	vector<int> v;
	sort(v1.begin(), v1.end());
	sort(v2.begin(), v2.end());
	set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(v));//�󽻼� 
	return v;
}

void calCORE(vector<vector<int>> *red, vector<int>* core, int rows = ROWS, int cols = COLS)
{
	vector<vector<int>>::iterator it_r;
	vector<int> v;
	for (int i = 1; i < rows ; i++)
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
	int rows = ROWS;
	int cols = COLS;
	cout << "������������Ŀ(������������) : " ;
	cin >>  cols;
	// �洢���ݱ�
	vector<vector<string>> *ds = new vector<vector<string>>();
	// ��ӡ������������ֵ
	vector<set<string>> *attrVal = new vector<set<string>>();
	// �洢 IND(D)
	vector<vector<int>>* D = new vector<vector<int>>();
	// �洢���е� IND(P-ai)
	vector<vector<vector<int>>*> *ind_p = new vector<vector<vector<int>>*>();
	// �洢 IND(P)
	vector<vector<int>> *ind_p_ = new vector<vector<int>>();
	// �洢���� POS p-ai(D)
	vector<vector<int>*>* POSs = new vector<vector<int>*>();
	// �洢 RED
	vector<vector<int>> *red = new vector<vector<int>>();
	// �洢 Core
	vector<int>* core = new vector<int>();

	getDateaSet(ds, &rows,cols);

	getAttrVal(ds, attrVal, rows, cols);

	calIND_D(ds, D);

	calIND_P(ds,ind_p_);

	vector<int> *POS_p = new vector<int>();
	calPOS(D, ind_p_, POS_p);

	cout << endl;
	// �ֱ���� IND  ��  POS
	for (int i = 0; i < COLS - 1; i++)
	{
		// ���� IND(P-ai)
		vector<vector<int>> *ind_p_ai = new vector<vector<int>>();
		calINDP_ai(ds, ind_p_ai, i,rows, cols);
		ind_p->push_back(ind_p_ai);
		// ���� POS p-ai(D)
		vector<int> *POS = new vector<int>();
		calPOS(D, ind_p_ai, POS);
		POSs->push_back(POS);
		cout << endl;
	}


	//--------------------------------------------------------------
		// �洢���е� IND(P-ai)
	vector<vector<vector<int>>*> *ind_pp = new vector<vector<vector<int>>*>();
	//--------------------------------------------------------------
	cout << "\n=================== test ==============================" << endl;
	// �ֱ���� IND  ��  POS
	for (int i = 0; i < COLS - 1; i++)
	{
		for (int j = i+1; j < COLS - 1; j++)
		{
			vector<int> *dontConsider = new vector<int>();
			dontConsider->push_back(i);
			dontConsider->push_back(j);
			// ���� IND(P- {...})
			vector<vector<int>> *ind_p_ai_t = new vector<vector<int>>();
			calINDP_matli(ds, ind_p_ai_t, dontConsider, rows);
			ind_pp->push_back(ind_p_ai_t);
			// ���� POS p-ai(D)
			vector<int> *POS = new vector<int>();
			calPOS(D, ind_p_ai_t, POS);
			POSs->push_back(POS);
			cout << endl;
		}
	}

	// ����RED
	calRED(POSs, POS_p, red, rows, cols);
	// ����CORE
	calCORE(red, core, rows, cols);
}
