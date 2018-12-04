#include <windows.h>
#include <deque>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <conio.h>
#include <time.h>
#include <iostream>




int numThreads;
std::string filePath;
std::string filePath_to_save;
static std::vector<std::string> **sorted_string_arrs;
//typedef std::function<void (std::vector<std::string> vector, int id)> Func;
static int Synch = 0;
static int countEl;
static int isAdditing;
std::vector<std::string> *vect;
std::vector<std::string> Arr;
/*
//void sort(std::vector<std::string> vector, int id)
//void WINAPI sort(std::vector<std::string> vector)
*/







class Arr_id
{
public:
	std::vector<std::string> vector;
	//Arr_id(int id, std::vector<std::string> vector, HANDLE hMutex)
	Arr_id(int id, std::vector<std::string> vector)
	{
		this->id = id;
		this->vector = vector;
		//this->hMutex = hMutex;
	}

	int getId()
	{
		return this->id;
	}

	std::vector<std::string>* getVector()
	{
		return &(this->vector);
	}

	//HANDLE getHMutes()
	//{
	//	return this->hMutex;
	//}

private:
	int id;
	
	//HANDLE hMutex;
};


Arr_id* arr_id;

CRITICAL_SECTION cs1;
CRITICAL_SECTION cs2;
HANDLE hMutex;



DWORD WINAPI sort_func(LPVOID lParam)
{
	
	//vector_temp = ((std::vector<std::string>*) lParam);
	//std::vector<std::string> vector_temp = ((Element*) lParam)->getstringArr();
	//int b = *((int*)lParam);

	//std::vector<std::string> vector_temp = (*(std::vector<std::string>*) lParam);

	Arr_id* arr_id = ((Arr_id*)lParam);


	
	int id = arr_id->getId();
	//std::vector<std::string> vector;
	
	//for (int i = 0; i < (arr_id->getVector()).size(); i++)
	//{
	//	vector.push_back((arr_id->getVector())[i]);
	//}

	//std::vector<std::string> vector = arr_id->getVector();
	//int c = 0;
	//printf("Hello WOrld");

	std::vector<std::string>* vector = arr_id->getVector();
	std::sort((*vector).begin(), (*vector).end());





	//hMutex = arr_id->getHMutes();
	//EnterCriticalSection(&cs2);
	//WaitForSingleObject(hMutex,  INFINITE);
	sorted_string_arrs[id] = vector;
	//LeaveCriticalSection(&cs2);
	//ReleaseMutex(hMutex);
	//for (int i  = 0; i < (vector).size(); i++)
	//{
	//	printf("%s", (vector)[i]);
	//}
	//printf("\n");
	//vect = &vector;
	return 0;
}




class Element
{
public:

	Element(std::vector<std::string> str_Arr)
		//void (*func) (std::vector<std::string>, int))
	//	DWORD(*func) (LPVOID lParam))

	{
		this->str_Arr = str_Arr;
		//this->func = func;
	}

	std::vector<std::string>* getstringArr()
	{
		return &this->str_Arr;
	}

	//LPVOID getFunc()
	//{
	//	return &(this->func);
	//}

private:
	std::vector<std::string> str_Arr;
	//Func func;
	//void (*func) (std::vector<std::string>, int);
	//DWORD (*func) (LPVOID lParam);
};

class Queue
{
public:

	Queue()
	{
		//this->isAdditing = 0;
		//this->countEl = 0;
		isAdditing = 0;
		this->countEl = 0;
	}

	int addToQueue(Element element)
	{
		WaitForSingleObject(hMutex, INFINITE);
		while (isAdditing)
			Sleep(100);
		isAdditing = 1;
		Arr_elements.push_back(element);
		this->countEl += 1;
		isAdditing = 0;
		ReleaseMutex(hMutex);
		return this->countEl;
	}

	int getCountEl()
	{
		return this->countEl;
	}

	Element getElement()
	{
		while (isAdditing)
			Sleep(10);
		isAdditing = 1;
		//if (!this->countEl <= 0)
		//	NULL;
		

		
		Element Temp = Arr_elements.back();
		Arr_elements.pop_back();
		this->countEl--;
		isAdditing = 0;
		return Temp;
		
	}

	
private:
	std::vector<Element> Arr_elements;
	int countEl;
};


class ThreadsApi
{
public:
	HANDLE * Threads;

	ThreadsApi(int numThreads)
	{
		this->Threads = new HANDLE[numThreads];
		this->numThreads = numThreads;
	}

	int getFree()
	{
		bool isFree = false;
		int i;
		while (isFree)
		{
			this->numActiv = 0;
			for (i = 0; i < this->numThreads; i++)
			{
				if  ((this->Threads[i] != NULL) || (WaitForSingleObject(this->Threads[i], 5)))
					isFree = true;
				else
					this->numActiv++;
			}
			if (isFree)
				return i;
		}
	}

	int getNumActiv()
	{
		return this->numActiv;
	}

private:

	int numActiv = 0;
	int numThreads;
};


class QueueWorker
{
public:

	QueueWorker()
	{
		
	}

	QueueWorker(int numThreads, Queue queue)
	{
		this->queue = queue;
		this->numThreads = numThreads;	
		this->Threads = new ThreadsApi(numThreads);
		this->countEl = queue.getCountEl();
	}



	void startQueue()
	{
		int index = 0;
		int thread_state;
		


		while (this->countEl-- > 0)
		{
			//index = this->Threads->getFree();
			Element element = this->queue.getElement();
			//std::vector<std::string> str_arr = element.getstringArr();
			

			// 1) lpThreadAttributes - данный аргумент определяет, 
			// может ли создаваемый поток быть унаследован дочерним 
			// процессом. Мы не будем создавать дочерние процессы, 
			// поэтому ставим NULL.
			// 2) dwStackSize - размер стека в байтах. Если передать 0, 
			// то будет использоваться значение по-умолчанию (1 мегабайт). 
			// 3) lpStartAddress - адрес функции, которая будет выполняться потоком
			// 4) lpParameter - указатель на переменную,
			// 5) dwCreationFlags - флаги создания. Здесь можно отложить запуск 
			//выполнения потока. Мы будем запускать поток сразу же, 
			//передаём 0.
			// 6)  lpThreadId - указатель на переменную, куда будет сохранён 
			// идентификатор потока. Нам идентификатор не нужен, передаём NULL

			arr_id = new Arr_id(index, *element.getstringArr());


			this->Threads->Threads[index++] = CreateThread(NULL,
				0,
				//(LPTHREAD_START_ROUTINE) element.getFunc(),
				(LPTHREAD_START_ROUTINE) sort_func,
			    //(LPVOID) &Arr,
				arr_id,
				0, 
				NULL);
			//this->Threads->Threads[index++].

		}
		int mes = waitForAll();
	}

	int waitForAll()
	{
		return WaitForMultipleObjects(numThreads, (const HANDLE*)this->Threads->Threads,
			TRUE, INFINITE);
			//while (this->queue.getCountEl() > 0)
		//	Sleep(50);

		//while ()
		//{
		//}
	}

private:
	ThreadsApi* Threads;
	Queue queue;
	int numThreads;
	int countEl;
};


class Sort
{

public:
	Sort()
	{

	}

	Sort(int numThreads, std::string filePath, std::string filePath_to_save)
	{
		this->filePath = filePath;
		this->readFile(filePath);
		this->filePath_to_save = filePath_to_save;
		sorted_string_arrs = new std::vector<std::string>*[numThreads];
		this->numThreads = numThreads;
		this->makeQueue();
	}



	void startSorting()
	{
		this->queueWorker = new QueueWorker(this->numThreads, this->queue);
		this->queueWorker->startQueue();
		this->unionSortesStrings();
		this->sorted_string_vector = this->merge(sorted_string_arrs, numThreads);
		this->saveToFile(this->filePath_to_save);
		return;
	}



	


private:



	std::string filePath_to_save;
	std::string filePath;
	QueueWorker* queueWorker;
	std::vector<std::string> string_vector;
	std::vector<std::string> sorted_string_vector;
	int numThreads;
	Queue queue;


	std::vector<std::string> merge(std::vector<std::string>** Arr, int num)
	{
		int arr_len = 0; int key; int Num_Min = 0;
		for (int i = 0; i < num; i++)
		{
			arr_len += (*(Arr[i])).size();
		}

		//std::string* Arr_merged = new std::string[arr_len];
		std::vector<std::string> Arr_merged(arr_len);
		int* indexes = new int[num];
		for (int i = 0; i < num; i++)
			indexes[i] = 0;
		std::string Min;
		//ZeroMemory(indexes, num);
		for (int i = 0; i < arr_len; i++)
		{
			for (Num_Min = 0; Num_Min < num; Num_Min++)
			{
				if (indexes[Num_Min] < (*Arr[Num_Min]).size())
				{
					Min = (*Arr[Num_Min])[indexes[Num_Min]];
					break;
				}
			}
			for (key = 0; key < num; key++)
			{
				if ((indexes[key] < (*Arr[key]).size()) && (Min > (*Arr[key])[indexes[key]]))
				{
					Num_Min = key;
				}
			}
			Arr_merged[i] = (*Arr[Num_Min])[indexes[Num_Min]];
			indexes[Num_Min]++;
		}
		return Arr_merged;
	}




	void readFile(std::string filePath)
	{
		std::ifstream file;
		file.open(filePath);
		if (!file.is_open() || !file.good())
			return;
		std::string line;
		while (std::getline(file, line))
			this->string_vector.push_back(line);
		return;
	}

	

	void makeQueue()
	{
		int len = this->string_vector.size() / this->numThreads;
		//std::vector<std::string>* Arr = new std::vector<std::string>[this->numThreads];
		std::vector<std::vector<std::string>> Arr;
		for (int i = 0; i < this->numThreads - 1; i++)
		{
			Arr.push_back(std::vector<std::string>(this->string_vector.begin() + i * len, 
				this->string_vector.begin() + (i + 1) * len));
		}


		Arr.push_back(std::vector<std::string>(this->string_vector.begin() + (numThreads-1) * len,
						this->string_vector.end()));
		for (int i = 0; i < this->numThreads; i++)
		{
			countEl++;
			//this->queue.addToQueue(Element(Arr[i], sort_func));
			this->queue.addToQueue(Element(Arr[i]));
		}
	}

	void unionSortesStrings()
	{
		int ArrLen = 0;
		for (int i = 0; i < numThreads; i++)
			ArrLen += (*sorted_string_arrs[i]).size();

		this->sorted_string_vector.reserve(ArrLen);
		

		for (int i = 0; i < numThreads; i++)
		{

			this->sorted_string_vector.insert((this->sorted_string_vector).end(),
				(*sorted_string_arrs[i]).begin(),
				(*sorted_string_arrs[i]).end());
		}
	}

	int saveToFile(std::string filePath_to_save)
	{
		std::ofstream file;
		file.open(filePath_to_save);
		if (!file.is_open())
		{
			return -1;
		}
		for (int i = 0; i < this->sorted_string_vector.size(); i++)
		{
			
			file << this->sorted_string_vector[i] << "\n";
		}

		return 0;
	}
};



void currentDateTime() {
	//time_t     now = time(0);
	//struct tm  tstruct;
	//char       buf[80];
	//tstruct = *localtime(&now);

	SYSTEMTIME lpSysT;
	GetSystemTime(&lpSysT);

//	strftime(buf, sizeof(buf), "%Y-%m-%d.%X.S%", &lpSysT);
	printf("%i-%i-%i %i:%i:%i:%i\n", lpSysT.wYear, lpSysT.wMonth,
		lpSysT.wDay, lpSysT.wHour, lpSysT.wMinute, lpSysT.wSecond, lpSysT.wMilliseconds);
}

int main()
{
	//ReleaseMutex(hMutex);
	/*
	printf("num of threads: ");
	scanf("%i", &numThreads);

	printf("file path: ");
	scanf("%s", &filePath);
	
	printf("file path to save: ");
	scanf("%s", &filePath_to_save);
	*/
	hMutex = CreateMutex(NULL, FALSE, NULL);
	numThreads = 1;
	filePath = "D:\\TEST_SORT.txt";
	filePath_to_save = "D:\\1.txt";
	//sorted_string_arrs = new std::vector<std::string>[numThreads];
	Sort Sorter(numThreads, filePath, filePath_to_save);


    currentDateTime();
	Sorter.startSorting();
	
	
	//std::vector<std::string> vec1 = sorted_string_arrs[0];
	//std::vector<std::string> vec2 = sorted_string_arrs[1];
	//std::vector<std::string> vec3 = sorted_string_arrs[2];
	//std::vector<std::string> vec4 = sorted_string_arrs[3];
	//std::vector<std::string> vec5 = sorted_string_arrs[4];
	//vec = sorted_string_arrs[1];
	//vec = sorted_string_arrs[2];
	//int flag = WaitForSingleObject(handle, 900);
	//printf("num of threads: \n");
	//printf("num of threads: \n");
	//Sleep(1000);
	//flag = WaitForSingleObject(handle, 10);
	//scanf("%i", &numThreads);

	currentDateTime();

	_getch();
}

