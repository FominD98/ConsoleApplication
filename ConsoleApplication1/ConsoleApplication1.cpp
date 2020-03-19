#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;
struct Point
{
	int x; //������
	int local_degree = 0; //��������  ��������� �������
	int part;//�� ����� ����� �������
	int ribs = 0;// ��� ����������� ���������
	int degree_z = 0;//��������� ������� �������� � �����
	double k = 0;//����������� ���������
};
//�������  ���������
Point* value;
Point ** a; //���������� ������

class Task
{
public:
	int v, r;//���-�� ������ � �����
	int n;//������� ������ ��������� ����S

public:
	void Read_from_file() //������ �� �����
	{
		ifstream fraph("Input3.txt"); // ������� ���� ��� ������

		if (!fraph.is_open()) // ���� ���� �� ������
		{
			cout << "���� �� ����� ���� ������!\n"; // �������� �� ����
			system("pause");
		}
		else
		{
			fraph >> v >> r;
			cout << " ���������� ������ �������� �����: " << v << endl;
			cout << " ���������� ����� �������� �����: " << r << endl;
		}

		value = new Point[v];//������ ���������

		a = new Point *[v]; //������ �������
		for (int i = 0; i < v; i++)
			a[i] = new Point[v]; //c�����//������ ������ ����������� ����� ��� ������

		int *m = new int[r * 2]; //��������� ������
		for (int i = 0; i < r * 2; i++)
			fraph >> m[i];

		fraph.close(); // ��������� ����

		cout << "\n������� ��������� ������������������ �����:" << endl;
		for (int i = 0; i < v; i++)
		{
			cout << "  " << i + 1;
			for (int j = 0; j < v; j++)
				a[i][j].x = 0;
		}
		cout << "\n------------------------" << endl;

		for (int i = 0; i < r * 2; i = i + 2)
		{
			a[m[i]][m[i + 1]].x++;
			a[m[i + 1]][m[i]].x++;
		}
		delete m;//������������ ������
		for (int i = 0; i < v; i++)
		{
			for (int j = 0; j < v; j++)
			{
				value[i].local_degree = value[i].local_degree + a[i][j].x; //��������� ������� ���� ������
				cout << "  " << a[i][j].x;
			}
			cout << endl;
		}
		cout << "------------------------" << endl;
		for (int i = 0; i < v; i++)
			cout << "  " << value[i].local_degree;
		cout << "     - ��������� �������" << endl;
		cout << "\n ������� ���������� �������������� �������? n=";
		cin >> n;
		cout << endl;

		if (n<2 || n>v)
		{
			cout << "������!" << endl;
			system("pause");
		}
		int lo;
		cout << "\n ������� ���������� ������������ �������? n=";
		cin >> lo;
		for (int i = 0; i < n; i++)
		{
		 value[i].part = lo;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Answer(vector<int>& mass_i)
	{
		ofstream Result("Result.txt"); //�������� ����� ��� ������
									   //������ ����� � ����
		int l = 0, q = 0;
		Result << endl << l + 1 << " ������� = ";
		for (int j = 0; j < mass_i.size(); j++)
		{
			if (value[l].part == q)
			{
				l++;
				Result << endl << l + 1 << " ������� = ";
				q = 0;
			}
			Result << "  " << mass_i[j];
			q++;
		}
		Result << endl;
		//Result << "����������� ��������� = " << (double)value->k << endl;
		Result << "runtime=" << clock() / 1000.0 << endl;
		cout << "runtime=" << clock() / 1000.0 << endl;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Algorithm
{
	Task task; //������ ������ Task
public:
	void Realization()        //������� ���������� ���������
	{
		task.Read_from_file(); //���������� � ������ Task ��� �������������� �������� ������

		vector<int> adjacent_degree; //������� c������
		vector<int> index;// ��������� ������
		vector<int>raw_vertices;//�������������� �������
		vector<int>mass_i;//������ ������������ ��������
		vector<int>ribs_old; //������������ ������� ��� ������������  ���������
		vector<int>first_index;//��������� ��������� ������� ���� ������

		int counter_n = 0;//������� ��������� ����������
		int coun = 0;// ������� ��� ������������  ���������
		int check = 0; //��������

		for (int i = 0; i < task.v; i++)
		{
			raw_vertices.push_back(i); //0 1 2 3 4 5 6 7 ... v
			first_index.push_back(value[i].local_degree); //���������
		}
		//�������� ���
		while (counter_n <  task.n)//���� �� ��������� �� n ������
		{
			check = check + value[counter_n].part;//����� ������ ��� ��������
												  //���� ��������� ���, �� ��������� ��� ������� � �����, ������� ���������� �����   � ������� �� ����� while
			if ((counter_n == (task.n - 1)))
			{
				for (int i = 0; i < raw_vertices.size(); i++)
					mass_i.push_back(raw_vertices[i]);

				Build_Array(a, value, mass_i, ribs_old, task.v, coun);
				//����� ���������� ����� � ������ ������ 
				value->k = value->k + value->ribs;
				//cout << endl << counter_n + 1 << " ����� = ";
				for (int j = 0; j < mass_i.size(); j++)
					cout << " " << mass_i[j];
				cout << endl;
				break;
			}

			//����� ������������ ��������� ������� �� ��������� �������
			int m_i = -1;
			int max1 = -1;
			for (int i = 0; i < task.v; i++)
			{
				if (max1 < value[i].local_degree)
				{
					max1 = value[i].local_degree;
					m_i = i;
				}
			}
			mass_i.push_back(raw_vertices[m_i]); //��������� ���� ������� ���������� � �����

												 //���� ������� � ���� max ������� � ��������� �� ��������� �������
			for (int j = 0; j < task.v; j++)
			{
				if (!Contains(mass_i, j))//�������� ������� ����������
					if (a[raw_vertices[m_i]][j].x != 0)// ����� ������ �� ������� ������� � ���� ������� �������
					{
						for (int i = 0; i < raw_vertices.size(); i++)
							if (j == raw_vertices[i])
								adjacent_degree.push_back(value[i].local_degree);//���������� ������� ������� ������
						index.push_back(j);////���������� ������� ������� �������
					}
			}

			raw_vertices.erase(raw_vertices.begin() + m_i);//�������� ������������ max �������. ��� ��� �� �� ����������

														   //���� ������ ���������� ������� ������ �� �������(max) �������
			for (int j = 0; j < (value[counter_n].part - 1) && adjacent_degree.size() != 0; j++)
				Search_Max(value, adjacent_degree, index, raw_vertices, mass_i);

			for (int i = 0; i < task.v; i++)
				value[i].local_degree = 0;	//����������� �������

											//������ ���������� ������� ��������� � ������� ������� ������
			Build_Array(a, value, mass_i, ribs_old, task.v, coun);
			cout << "------------------------" << endl;
			for (int i = 0; i < task.v - mass_i.size(); i++)
				cout << "  " << value[i].local_degree;//������� �� ��������� �������

													  //���� ����� �� ������, �� ��������� �������
			if (mass_i.size() != check)
			{
				Adding_Vertices(value, raw_vertices, mass_i, first_index, task.v, counter_n, check); //���������� ������ � ������� ����������� ����� ��������� b(xj)
																									 //�������� ������� ������
				for (int i = 0; i < task.v; i++)
				{
					value[i].local_degree = 0;
					value[i].degree_z = 0;
				}
				value->ribs = 0;//�������� 

								//���-�� ������ � ����� ������ � ������� �� ������� ������ 
				Build_Array(a, value, mass_i, ribs_old, task.v, coun);
				cout << "------------------------" << endl;
				for (int i = 0; i < task.v - mass_i.size(); i++)
					cout << "  " << value[i].local_degree;//������� �� ��������� �������
			}
			//�������� ������� ��� ������������ ��� ������������  ���������
			for (int i = 0; i < mass_i.size(); i++)
				ribs_old.push_back(mass_i[i]);
			cout << endl;

			cout << endl << /*counter_n + 1 <<*/ "  = ";
			for (int j = 0; j < mass_i.size(); j++)
				cout << " " << mass_i[j];
			cout << endl;
			//����� ���������� ����� � ������ ������ 
			value->k = value->k + value->ribs;
			//�����������
			adjacent_degree.clear();// ������� �������
			index.clear();//������ ������
			value->ribs = 0;//�������� 
			counter_n++;//�������� ��� ���������� �����
		}

		value->k = value->k / 2;
		value->k = (double)value->k / (task.r - value->k);
		//cout << "����������� ���������� = " << (double)value->k << endl;
		//���������� � ���� �����
		task.Answer(mass_i);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Adding_Vertices(Point* value, vector<int>& raw_vertices, vector<int>& mass_i, vector<int>& first_index, int v, int &counter_n, int check)
	{
		vector<int>m;
		vector<int>raw_m(raw_vertices);//������� ������ � �������� � ���� �������� raw_vertices
		int j = 0;
		for (int i = 0; i < v; i++)
			if (Contains(raw_vertices, i))//������ �� ������������ ������
			{
				if (value[j].local_degree == first_index[i])//�������� ������� ������� �� ������(�� ������) � �������� ������
					raw_m.erase(raw_m.begin() + j); //�������
				else
					m.push_back(first_index[i] - 2 * value[j].local_degree);//����� ��������� b(xj), ��� ������� ������ � ��������
				j++;
			}

		for (int i = mass_i.size(); i < check; i++)
		{
			int max = -1111;
			int mi;
			for (int i = 0; i < m.size(); i++)
				if (max < m[i])
				{
					max = m[i];
					mi = i;
				}
			mass_i.push_back(raw_vertices[mi]);
			raw_vertices.erase(raw_vertices.begin() + mi); //�������� ��� ������������
			raw_m.erase(raw_m.begin() + mi);
			m.erase(m.begin() + mi);
		}
		cout << endl;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Build_Array(Point**a, Point*value, vector<int>& mass_i, vector<int>& ribs_old, int v, int &coun)
	{
		int** newmatrix = new int *[v - mass_i.size()]; //������ �������
		for (int i = 0; i <(v - mass_i.size()); i++)
			newmatrix[i] = new int[v - mass_i.size()]; //c�����//������ ������ ����������� ����� ��� ������
		cout << endl;

		int k = 0, z = 0;//������ ������ ������� l-������� ������ �������
		for (int i = 0; i < v; i++)
		{
			for (int j = 0, l = 0; j < v; j++)
			{
				if (!Contains(mass_i, i) && !Contains(mass_i, j))
				{
					newmatrix[k][l++] = a[i][j].x;
					value[k].local_degree = value[k].local_degree + a[i][j].x;
				}
				else if (Contains(mass_i, i) && Contains(mass_i, j))
				{
					if ((!Contains(ribs_old, i) && !Contains(ribs_old, j)) || coun == 0)
					{
						//cout  << a[i][j].x << " |";
						value[z].degree_z = value[z].degree_z + a[i][j].x;
						value->ribs = value->ribs + a[i][j].x;
					}
				}
			}
			if (!Contains(mass_i, i)) k++;
			if (Contains(mass_i, i)) z++;
		}
		coun++;
		//������� ���������� ������
		for (int i = 0; i < v - mass_i.size(); i++) {
			for (int j = 0; j < v - mass_i.size(); j++)
				cout << "  " << newmatrix[i][j];
			cout << endl;
		}
		//�������� �������
		for (int i = 0; i < v - mass_i.size(); i++)
			delete[] newmatrix[i];
		delete[] newmatrix;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//�������� ������ � �����// ���� ��� ����� � �������, ���� ���� �� true, ����� false
	bool Contains(vector<int>&mass_i, int x)
	{
		for (int i = 0; i < mass_i.size(); i++)
			if (mass_i[i] == x) return true;
		// ���� �� ����� 
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//���������� ������ � �����
	void Search_Max(Point*value, vector<int>&adjacent_degree, vector<int>&  index, vector<int>& raw_vertices, vector<int>&mass_i)
	{
		int max = -1;
		int max_i;
		int t;
		for (int i = 0; i < adjacent_degree.size(); i++)
			if (max < adjacent_degree[i])
			{
				max = adjacent_degree[i];
				max_i = index[i];
				t = i;
			}
		mass_i.push_back(max_i);//���������� � �����

		if (adjacent_degree.size() != 0)
		{
			adjacent_degree.erase(adjacent_degree.begin() + t);//������� ��� ������� �� ������ �������    
			index.erase(index.begin() + t);//������� ������ ���� ������� �� ������ �������
		}
		for (int i = 0; i < raw_vertices.size(); i++)
			if (max_i == raw_vertices[i])
				raw_vertices.erase(raw_vertices.begin() + i); //�������� ��� ������������
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(0));
	Algorithm a;        //������� ������ ������ ��������
	a.Realization();   // ���������� � ������� ���������� ��������� ������ ��������
	system("pause");
	cin.get();
	return 0;
}
