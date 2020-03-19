#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;
struct Point
{
	int x; //массив
	int local_degree = 0; //значение  локальной степени
	int part;//на какие части делются
	int ribs = 0;// для коэффициент разбиения
	int degree_z = 0;//локальные степени вощедшие в часть
	double k = 0;//коэффициент разбиения
};
//объекты  структуры
Point* value;
Point ** a; //двухмерный массив

class Task
{
public:
	int v, r;//кол-во вершин и ребер
	int n;//сколько частей разрезать графS

public:
	void Read_from_file() //читаем из файла
	{
		ifstream fraph("Input3.txt"); // открыли файл для чтения

		if (!fraph.is_open()) // если файл не открыт
		{
			cout << "Файл не может быть открыт!\n"; // сообщить об этом
			system("pause");
		}
		else
		{
			fraph >> v >> r;
			cout << " количество вершин главного графа: " << v << endl;
			cout << " количество ребер главного графа: " << r << endl;
		}

		value = new Point[v];//объект структуры

		a = new Point *[v]; //хранит строчки
		for (int i = 0; i < v; i++)
			a[i] = new Point[v]; //cтроки//каждой строке присваиваем новый дим массив

		int *m = new int[r * 2]; //временный массив
		for (int i = 0; i < r * 2; i++)
			fraph >> m[i];

		fraph.close(); // закрываем файл

		cout << "\nМатрица смежности неориентированного графа:" << endl;
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
		delete m;//освобождение памяти
		for (int i = 0; i < v; i++)
		{
			for (int j = 0; j < v; j++)
			{
				value[i].local_degree = value[i].local_degree + a[i][j].x; //локальные степени всех вершин
				cout << "  " << a[i][j].x;
			}
			cout << endl;
		}
		cout << "------------------------" << endl;
		for (int i = 0; i < v; i++)
			cout << "  " << value[i].local_degree;
		cout << "     - локальные степени" << endl;
		cout << "\n Введите количество горизонтальных уровней? n=";
		cin >> n;
		cout << endl;

		if (n<2 || n>v)
		{
			cout << "Ошибка!" << endl;
			system("pause");
		}
		int lo;
		cout << "\n Введите количество вертикальных уровней? n=";
		cin >> lo;
		for (int i = 0; i < n; i++)
		{
		 value[i].part = lo;
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	void Answer(vector<int>& mass_i)
	{
		ofstream Result("Result.txt"); //открытие файла для записи
									   //запись ответ в файл
		int l = 0, q = 0;
		Result << endl << l + 1 << " уровень = ";
		for (int j = 0; j < mass_i.size(); j++)
		{
			if (value[l].part == q)
			{
				l++;
				Result << endl << l + 1 << " уровень = ";
				q = 0;
			}
			Result << "  " << mass_i[j];
			q++;
		}
		Result << endl;
		//Result << "коэффициент разбиения = " << (double)value->k << endl;
		Result << "runtime=" << clock() / 1000.0 << endl;
		cout << "runtime=" << clock() / 1000.0 << endl;
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Algorithm
{
	Task task; //объект класса Task
public:
	void Realization()        //функция реализации алгоритма
	{
		task.Read_from_file(); //обращаемся к классу Task для предоставления исходных данных

		vector<int> adjacent_degree; //смежные cтепень
		vector<int> index;// временный массив
		vector<int>raw_vertices;//необработанные вершины
		vector<int>mass_i;//нужное расположение массивов
		vector<int>ribs_old; //обработанные вершины для коэффициента  разбиения
		vector<int>first_index;//начальные локальные степени всех вершин

		int counter_n = 0;//счетчик основного разделения
		int coun = 0;// счетчик для коэффициента  разбиения
		int check = 0; //проверка

		for (int i = 0; i < task.v; i++)
		{
			raw_vertices.push_back(i); //0 1 2 3 4 5 6 7 ... v
			first_index.push_back(value[i].local_degree); //заполняем
		}
		//основной код
		while (counter_n <  task.n)//пока не разделили на n частей
		{
			check = check + value[counter_n].part;//сумма частей для проверки
												  //если последний шаг, то добавляем все вершины в ответ, считаем количество ребер   и выходим из цикла while
			if ((counter_n == (task.n - 1)))
			{
				for (int i = 0; i < raw_vertices.size(); i++)
					mass_i.push_back(raw_vertices[i]);

				Build_Array(a, value, mass_i, ribs_old, task.v, coun);
				//суммв количества ребер с каждой частью 
				value->k = value->k + value->ribs;
				//cout << endl << counter_n + 1 << " часть = ";
				for (int j = 0; j < mass_i.size(); j++)
					cout << " " << mass_i[j];
				cout << endl;
				break;
			}

			//поиск максимальной начальной вершины по локальной степени
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
			mass_i.push_back(raw_vertices[m_i]); //найденную макс вершину записываем в ответ

												 //ищем смежные с этой max вершины и фиксируем их локальную степень
			for (int j = 0; j < task.v; j++)
			{
				if (!Contains(mass_i, j))//отсекаем которые обработали
					if (a[raw_vertices[m_i]][j].x != 0)// номер строки из главной матрицы и ищем смежные вершины
					{
						for (int i = 0; i < raw_vertices.size(); i++)
							if (j == raw_vertices[i])
								adjacent_degree.push_back(value[i].local_degree);//записываем степени смежных вершин
						index.push_back(j);////записываем индексы смежные вершины
					}
			}

			raw_vertices.erase(raw_vertices.begin() + m_i);//удаление обработанную max вершину. так как ее мы обработали

														   //ищем нужное количество смежных вершин от главной(max) вершины
			for (int j = 0; j < (value[counter_n].part - 1) && adjacent_degree.size() != 0; j++)
				Search_Max(value, adjacent_degree, index, raw_vertices, mass_i);

			for (int i = 0; i < task.v; i++)
				value[i].local_degree = 0;	//освобождаем степени

											//строим оставшийся матрицу смежности и находим степени вершин
			Build_Array(a, value, mass_i, ribs_old, task.v, coun);
			cout << "------------------------" << endl;
			for (int i = 0; i < task.v - mass_i.size(); i++)
				cout << "  " << value[i].local_degree;//выводим их локальные степени

													  //если часть не полная, до добавляем вершины
			if (mass_i.size() != check)
			{
				Adding_Vertices(value, raw_vertices, mass_i, first_index, task.v, counter_n, check); //добавление вершин с помощью определения число связности b(xj)
																									 //обнуляем степени вершин
				for (int i = 0; i < task.v; i++)
				{
					value[i].local_degree = 0;
					value[i].degree_z = 0;
				}
				value->ribs = 0;//обнуляем 

								//кол-во вершин в части полное и находим их степени теперь 
				Build_Array(a, value, mass_i, ribs_old, task.v, coun);
				cout << "------------------------" << endl;
				for (int i = 0; i < task.v - mass_i.size(); i++)
					cout << "  " << value[i].local_degree;//выводим их локальные степени
			}
			//отмечаем вершины как обработанные для коэффициента  разбиения
			for (int i = 0; i < mass_i.size(); i++)
				ribs_old.push_back(mass_i[i]);
			cout << endl;

			cout << endl << /*counter_n + 1 <<*/ "  = ";
			for (int j = 0; j < mass_i.size(); j++)
				cout << " " << mass_i[j];
			cout << endl;
			//суммв количества ребер с каждой частью 
			value->k = value->k + value->ribs;
			//освобождаем
			adjacent_degree.clear();// смежные степени
			index.clear();//номера вершин
			value->ribs = 0;//обнуляем 
			counter_n++;//отмечаем что обработана часть
		}

		value->k = value->k / 2;
		value->k = (double)value->k / (task.r - value->k);
		//cout << "коэффициент размещения = " << (double)value->k << endl;
		//записываем в файл ответ
		task.Answer(mass_i);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void Adding_Vertices(Point* value, vector<int>& raw_vertices, vector<int>& mass_i, vector<int>& first_index, int v, int &counter_n, int check)
	{
		vector<int>m;
		vector<int>raw_m(raw_vertices);//создаем вектор и копируем в него значения raw_vertices
		int j = 0;
		for (int i = 0; i < v; i++)
			if (Contains(raw_vertices, i))//фильтр не обработанных вершин
			{
				if (value[j].local_degree == first_index[i])//вычищаем вершины которые не связны(не смежны) с областью ответа
					raw_m.erase(raw_m.begin() + j); //удаляем
				else
					m.push_back(first_index[i] - 2 * value[j].local_degree);//число связности b(xj), для смежных вершин с областью
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
			raw_vertices.erase(raw_vertices.begin() + mi); //отмечаем как обработанной
			raw_m.erase(raw_m.begin() + mi);
			m.erase(m.begin() + mi);
		}
		cout << endl;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	void Build_Array(Point**a, Point*value, vector<int>& mass_i, vector<int>& ribs_old, int v, int &coun)
	{
		int** newmatrix = new int *[v - mass_i.size()]; //хранит строчки
		for (int i = 0; i <(v - mass_i.size()); i++)
			newmatrix[i] = new int[v - mass_i.size()]; //cтроки//каждой строке присваиваем новый дим массив
		cout << endl;

		int k = 0, z = 0;//строка нового массива l-столбец нового массива
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
		//выводим оставшийся массив
		for (int i = 0; i < v - mass_i.size(); i++) {
			for (int j = 0; j < v - mass_i.size(); j++)
				cout << "  " << newmatrix[i][j];
			cout << endl;
		}
		//удаление матрицы
		for (int i = 0; i < v - mass_i.size(); i++)
			delete[] newmatrix[i];
		delete[] newmatrix;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//получаем массив и число// ищем это число в массиве, если есть то true, иначе false
	bool Contains(vector<int>&mass_i, int x)
	{
		for (int i = 0; i < mass_i.size(); i++)
			if (mass_i[i] == x) return true;
		// Если не нашли 
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//добавление вершин в ответ
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
		mass_i.push_back(max_i);//записываем в ответ

		if (adjacent_degree.size() != 0)
		{
			adjacent_degree.erase(adjacent_degree.begin() + t);//удаляем эту вершину из списка смежных    
			index.erase(index.begin() + t);//удаляем инднкс этой вершины из списка смежных
		}
		for (int i = 0; i < raw_vertices.size(); i++)
			if (max_i == raw_vertices[i])
				raw_vertices.erase(raw_vertices.begin() + i); //отмечаем как обработанной
	}
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
	setlocale(LC_ALL, "rus");
	srand(time(0));
	Algorithm a;        //создаем объект класса алгоритм
	a.Realization();   // обращаемся к функции реализации алгоритма класса алгоритм
	system("pause");
	cin.get();
	return 0;
}
