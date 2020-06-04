#include "iostream"
#include <time.h>
#include <math.h>
#include <algorithm>
#include <fstream>
#include <vector>

using namespace std;

string sortFileName(char* filename) {
	/*
		Функция преобразует переданное имя файла по типу:
		filename.txt -> filename.sort.txt
	*/
	string result(filename); //новое имя файла

	for (int i = int(result.size() - 1); i >= 0; --i) {
		/*идем с конца строки пока не встретим точку, перед которой вставим строку .sort*/
		if (result[i] == '.') {
			result.insert(i, ".sort");
			break;
		}
	}
	return result;
}

int compare(string num1, string num2) {
	/*функция сравнивает два числа
		Возвращает:
		1 если первое больше второго
		0 если они равны
	   -1 если первое меньше
	*/
	for (int i = 0; i < int(num1.size()); ++i) {
		/*идем слева направо сравниваем посимвольно*/
		if (num1[i] < num2[i]) {
			return -1;
		}
		if (num1[i] > num2[i]) {
			return 1;
		}
	}
	return 0;
}

// Процедура для преобразования в двоичную кучу поддерева
int heapify(string arr[], int n, int i, int result)
{
	int largest = i;
	// Инициализируем наибольший элемент как корень
	int l = 2 * i + 1; // левый = 2*i + 1
	int r = 2 * i + 2; // правый = 2*i + 2
	// Если левый дочерний элемент больше корня
	if (l < n && arr[l] > arr[largest])
		largest = l;
	// Если правый дочерний элемент больше, чем самый большой элемент на данный момент
	if (r < n && arr[r] > arr[largest])
		largest = r;
	// Если самый большой элемент не корень
	if (largest != i)
	{
		swap(arr[i], arr[largest]);
		result+=3;

		// Рекурсивно преобразуем в двоичную кучу затронутое поддерево
		heapify(arr, n, largest, result);
	}
	return result;
}
// Основная функция, выполняющая пирамидальную сортировку
int heapSort(string arr[], int n)
{
	int result = 0;
	// Построение кучи (перегруппируем массив)
	for (int i = n / 2 - 1; i >= 0; i--)
		result = heapify(arr, n, i, result);

	// Один за другим извлекаем элементы из кучи
	for (int i = n - 1; i >= 0; i--)
	{
		// Перемещаем текущий корень в конец
		swap(arr[0], arr[i]);
		result++;
		// вызываем процедуру heapify на уменьшенной куче
		result = heapify(arr, i, 0, result);
	}
	return result;
}

int SortDataset(char* filename) {
	int result = 0, len = 0;
	//объявляем файлы для считывания и массив
	ifstream fin(filename);
	ofstream fout(sortFileName(filename));
	vector <string> numbers = vector<string>();
	
	while (!fin.eof()) {
		//пока не конец файла считываем из файла в массив числа
		string str;
		fin >> str;
		numbers.push_back(str);
		++len;
	}
	string* arr = &numbers[0]; //делаем масив из вектора
	result = heapSort(arr, len);

	/*------------------------------------------------------------------
	int d;//шаг сортировки Шелла 
	for (d = 1; d * 2 < len; d *= 2); //подбираем такой шаг что d = 2^i < N

	//сортировка Шелла
	for (; d > 0; d /= 2) {
		for (int i = d; i < len; ++i) {
			for (int j = i - d; j >= 0 && compare(numbers[j], numbers[j + d]) == 1; j -= d) {
				swap(numbers[j], numbers[j + d]);
				++result;
			}
		}
	}
	*/
	//вывод чисел в файл
	for (int i = 0; i < len - 1; ++i) {
		fout << numbers[i] << '\n';
	}
	fout << numbers[len - 1];

	//возвращаем кол-во сравнений
	return result;
}

char numberToChar(int x) {
	/*Функция переводит число в символ
	0->'0'...10->'A'....
	*/
	if (x < 10) {
		return char('0' + x);
	}
	else {
		return char('A' + x - 10);
	}
}

string randNumber(int NS, int len) {
	/*
	Функция генерирует число в системе счисления NS с len разрядами
	и возвращает его в формате строки
	*/
	string result = "";

	for (int i = 0; i < len; ++i) {
		/*берем случайное число и записываем его в разряд*/
		result += numberToChar(rand() % NS);
	}

	/*Так как последний разряд не может быть нулем
	меняем его пока он не будет отличным от нуля*/
	while (result[0] == '0') {
		result[0] = numberToChar(rand() % NS);
	}
	return result;
}

void GenerateDataset(char* filename, int num) {
	/*функция из задания*/

	/*объявляем файл для считывания*/
	ofstream fout(filename);

	for (int i = 0; i < num - 1; ++i) {
		/*выводим туда случайное число и перевод строки*/
		fout << randNumber(16, 3) << '\n';
	}
	/*Выводим последнее число без перевода строки*/
	fout << randNumber(16, 3);
	fout.close();
}

int main() {
	srand(time(NULL));/*для случайной генерации*/
	setlocale(LC_ALL, "Russian"); /*для русского консольного вывода*/

	/*шапка таблицы*/
	string strin, strout, head = "num	ТЭ	T1	T2	ТЭ/T1	ТЭ/T2\n";

	double T, T1, T2;

	cout << "Введите имя входного файла: "; cin >> strin;
	cout << "Введите имя выходного файла для записи таблицы: "; cin >> strout;

	/*переводим строку в массив указателей чтобы передать в функцию*/
	char* fileName = new char[strin.size() + 1];
	copy(strin.begin(), strin.end(), fileName);
	fileName[strin.size()] = '\0';

	/*файл для вывода конечной таблицы*/
	ofstream fout(strout);
	fout << head;

	/*цикл для таблицы
	num: 8..4096
	*/
	for (int num = 8; num < 5000; num *= 2) {

		GenerateDataset(fileName, num);
		T = SortDataset(fileName);
		T1 = pow(num, 2);
		T2 = num * log2(num);

		/*вывод экспериментальных данных в формате таблицы*/
		fout << num << "	" << T << "	"
			<< T1 << "	" << T2 << "	"
			<< T / T1 << "	" << T / T2 << '\n';
	}

	fout.close();
}

