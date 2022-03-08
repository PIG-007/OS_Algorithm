#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>

using namespace std;

string Alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string table[5] = { "MAX","Allocation","Need","Avaliable","State" };


struct PCB {
	int PID;
	char state;
	int* runApplyResource;
	int* maxRequestResource;
	int* allocatedResource;
};

PCB** processes;
int resourceCount = 3;
int processCount = 5;
char alogrithmKind;
vector<int> safeSequence;


int demoMaxMatrix[5][3] =
{
		{7,5,3},
		{3,2,2},
		{9,0,2},
		{2,2,2},
		{4,3,3}
};

int demoAllocationMatrix[5][3] =
{
		{0,1,0},
		{2,0,0},
		{3,0,2},
		{2,1,1},
		{0,0,2}
};

int demoAvaliableResource[3] = { 3,3,2 };

int demoNeedMatrix[5][3] =
{
		{7,4,3},
		{1,2,2},
		{6,0,0},
		{0,1,1},
		{4,3,1}
};


int** maxMatrix;
int** allocationMatrix;
int** needMatrix;
int* avaliableResource;
int* workResourceMatrix;

//Functional func
void updateMatrix()
{
	for (int i = 0; i < processCount; i++)
	{
		for (int j = 0; j < resourceCount; j++)
		{
			maxMatrix[i][j] = processes[i]->maxRequestResource[j];
			allocationMatrix[i][j] = processes[i]->allocatedResource[j];
			needMatrix[i][j] = processes[i]->maxRequestResource[j] - processes[i]->allocatedResource[j];
		}
	}
}
void updatePCB(int PID) {

	int flag = 0;
	for (int i = 0; i < resourceCount; i++)
	{
		processes[PID]->allocatedResource[i] += processes[PID]->runApplyResource[i];
		if (processes[PID]->maxRequestResource[i] == processes[PID]->allocatedResource[i]) {
			flag++;
		}
		avaliableResource[i] -= processes[PID]->runApplyResource[i];
	}

	if (flag == resourceCount)
	{
		processes[PID]->state = 'F';
		for (int i = 0; i < resourceCount; i++)
		{
			avaliableResource[i] += processes[PID]->maxRequestResource[i];
			processes[PID]->maxRequestResource[i] = 0;
			processes[PID]->allocatedResource[i] = 0;
		}
	}

}
void tryDistribute(int PID)
{

	for (int i = 0; i < resourceCount; i++)
	{
		workResourceMatrix[i] = avaliableResource[i];
		allocationMatrix[PID][i] += processes[PID]->runApplyResource[i];
		needMatrix[PID][i] = maxMatrix[PID][i] - allocationMatrix[PID][i];
		workResourceMatrix[i] -= processes[PID]->runApplyResource[i];
	}
}
int safeCheck()
{
	safeSequence.clear();
	bool* Finish = new bool[processCount];
	for (int i = 0; i < processCount; i++)
	{
		Finish[i] = false;
	}


	int n = 0;
	while (n <= processCount)
	{
		for (int i = 0; i < processCount; i++)
		{
			int flag = 0;
			for (int j = 0; j < resourceCount; j++)
			{
				if (needMatrix[i][j] <= workResourceMatrix[j])
				{
					flag++;
				}
			}
			if ((Finish[i] == false) && (flag == resourceCount))
			{
				Finish[i] = true;
				safeSequence.push_back(i);
				for (int j = 0; j < resourceCount; j++)
				{
					allocationMatrix[i][j] = allocationMatrix[i][j] + needMatrix[i][j];
					workResourceMatrix[j] = workResourceMatrix[j] - needMatrix[i][j];
					workResourceMatrix[j] += maxMatrix[i][j];
				}
			}
		}
		n++;
	}

	int safeSign = 0;
	for (int i = 0; i < processCount; i++)
	{
		if (Finish[i] == true)
		{
			safeSign++;
		}
	}

	if (safeSign == processCount)
	{
		cout << "The safe sequence is : ";
		for (auto i : safeSequence)
		{
			cout << i << " ";
		}
		cout << endl;
		return 1;
	}
	else {
		return 0;
	}


	return 1;
}
int showFunc()
{
	char choice;
	int PID;
	int flag = 0;
	cout << "Banker Alogrithm or Random Alogrithm?(B or R):";
	cin >> alogrithmKind;
	if ((alogrithmKind != 'B') && (alogrithmKind != 'R'))
	{
		cout << endl;
		return 1;
	}

	cout << "Apply some resource for one process?(Y/N):";
	cin >> choice;
	if (choice == 'Y')
	{
		cout << "Please choose one process(Index:0 or 1 or ...):";
		cin >> PID;
		if (PID < 0 || PID >= processCount)
		{
			cout << "The process's PID is invalid!Again!" << endl;
			return 1;
		}
		if (processes[PID]->state == 'F')
		{
			cout << "The process has been done!Choose another process!" << endl;
			return 1;
		}
		cout << "How much resource do you want to apply for the process:(ResourceA ResourceB .....):";
		for (int i = 0; i < resourceCount; i++)
		{
			cin >> processes[PID]->runApplyResource[i];
		}
		for (int i = 0; i < resourceCount; i++)
		{
			if ((processes[PID]->runApplyResource[i] >= 0) &&
				(processes[PID]->runApplyResource[i] <= avaliableResource[i]) &&
				(processes[PID]->runApplyResource[i] <= needMatrix[PID][i]))
			{
				flag++;
			}
		}
		if (flag == resourceCount)
		{
			processes[PID]->state = 'R';
			if (alogrithmKind == 'B')
			{
				tryDistribute(PID);
				cout << "Check now!" << endl;
				if (safeCheck() == 1)
				{
					cout << "Allocated!" << endl;
					processes[PID]->state = 'W';
					updatePCB(PID);
					updateMatrix();
					return 1;
				}
				else
				{
					cout << "SafeCheck is failed!Maybe trigger deadlock!" << endl;
					cout << "RollBack!" << endl;
					processes[PID]->state = 'W';
					updateMatrix();
					return 1;
				}
			}
			if (alogrithmKind == 'R')
			{
				cout << "Allocated!" << endl;
				processes[PID]->state = 'W';
				updatePCB(PID);
				updateMatrix();
				return 1;
			}
			return 1;
		}
		else
		{
			cout << "Your apply has exceed the need or avaliable resource!Again!" << endl;
			return 1;
		}

	}
	else if(choice == 'Y')
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


//create func
void createMatrix()
{
	avaliableResource = new int[resourceCount];
	maxMatrix = new int* [processCount];
	allocationMatrix = new int* [processCount];
	needMatrix = new int* [processCount];
	workResourceMatrix = new int[processCount];
	for (int i = 0; i < processCount; i++)
	{
		maxMatrix[i] = new int[resourceCount];
		allocationMatrix[i] = new int[resourceCount];
		needMatrix[i] = new int[resourceCount];
		for (int j = 0; j < resourceCount; j++)
		{
			maxMatrix[i][j] = 0;
			allocationMatrix[i][j] = 0;
			needMatrix[i][j] = 0;
			workResourceMatrix[j] = 0;
		}
	}
}
void createProcess()
{
	processes = new PCB * [processCount];
	for (int i = 0; i < processCount; i++) {
		processes[i] = new PCB;
	}
}

//Demo func
void initDemoMatrix()
{
	for (int i = 0; i < processCount; i++)
	{
		for (int j = 0; j < resourceCount; j++) {
			maxMatrix[i][j] = demoMaxMatrix[i][j];
			allocationMatrix[i][j] = demoAllocationMatrix[i][j];
			needMatrix[i][j] = demoNeedMatrix[i][j];
		}
	}
	for (int i = 0; i < resourceCount; ++i) {
		avaliableResource[i] = demoAvaliableResource[i];
	}

}
void initDemoProcesses() {
	for (int i = 0; i < processCount; i++)
	{
		processes[i]->runApplyResource = new int[resourceCount];
		processes[i]->maxRequestResource = new int[resourceCount];
		processes[i]->allocatedResource = new int[resourceCount];
		processes[i]->state = 'W';
		processes[i]->PID = i;
		for (int j = 0; j < resourceCount; j++)
		{
			processes[i]->allocatedResource[j] = allocationMatrix[i][j];
			processes[i]->maxRequestResource[j] = maxMatrix[i][j];
		}

	}
}

//Custom func
PCB* initPCB(int PID)
{
	PCB* p = new PCB;
	p->runApplyResource = new int[resourceCount];
	p->maxRequestResource = new int[resourceCount];
	p->allocatedResource = new int[resourceCount];
	p->state = 'W';
	p->PID = PID;
	cout << "The max resource requirement of process " << PID << " is(ResourceA ResourceB .....):";
	for (int i = 0; i < resourceCount; i++)
	{
		cin >> p->maxRequestResource[i];
	}
	while (1)
	{
		int flag = 0;
		cout << "The allocated resource of process " << PID << " is(ResourceA ResourceB .....):";
		for (int i = 0; i < resourceCount; i++)
		{
			cin >> p->allocatedResource[i];
			if (p->allocatedResource[i] > p->maxRequestResource[i])
			{
				flag += 1;
			}
		}
		if (flag >= 1)
		{
			cout << "The allocated resource can't exceed the max resource requirement!Again!" << endl;
			continue;
		}
		else
		{
			break;
		}

	}



	return p;
}
void initProcesses() {
	processes = new PCB * [processCount];
	cout << "Please input the information of PCB!" << endl;
	cout << "Split the information by space.Such as \"1 2 3\"!" << endl;
	for (int i = 0; i < processCount; i++)
	{
		processes[i] = initPCB(i);
	}
}
void initAvaliableResource()
{

	while (1)
	{
		cout << "Please input System Avaliable Resource!" << endl;
		cout << "Split the information by space.Such as \"1 2 3\":";
		for (int i = 0; i < resourceCount; i++)
		{
			cin >> avaliableResource[i];
			workResourceMatrix[i] = avaliableResource[i];
		}
		updateMatrix();
		if (safeCheck())
		{
			return;
		}
		cout << "The System Avaliable Resource can't satisfy any process!Again!" << endl;
	}
}


//Print func
void PrintStringSpace(int t)
{
	for (int j = 0; j < table[t].length(); j++)
	{
		cout << " ";
	}
}
void PrintABC(int t)
{
	cout << "|";
	for (int i = 0; i < resourceCount; i++)
	{
		cout << Alphabet[i] << " ";
	}
	PrintStringSpace(t);

}
void Printhead()
{
	for (int i = 0; i < resourceCount; i++)
	{
		cout << " " << " ";
	}
}
void PrintProcess(int i)
{
	cout << "P" << i << "\t" << "\t" << "\t" << "|";
	for (int j = 0; j < resourceCount; j++)
	{
		cout << processes[i]->maxRequestResource[j] << " ";
	}
	PrintStringSpace(0);
	cout << "|";

	for (int j = 0; j < resourceCount; j++)
	{
		cout << processes[i]->allocatedResource[j] << " ";
	}
	PrintStringSpace(1);
	cout << "|";

	for (int j = 0; j < resourceCount; j++)
	{
		cout << processes[i]->maxRequestResource[j] - processes[i]->allocatedResource[j] << " ";
	}
	PrintStringSpace(2);
	cout << "|";

	for (int j = 0; j < resourceCount; j++)
	{
		cout << avaliableResource[j] << " ";
	}
	PrintStringSpace(3);
	cout << "|";

	cout << processes[i]->state << endl;
}
void PrintTable()
{
	cout << "--------------------------------------Table----------------------------------------" << endl;
	cout << "Process\\resource" << "\t";
	for (auto& i : table)
	{
		cout << "|" << i;
		Printhead();
	}
	cout << endl;
	cout << "\t" << "\t" << "\t";

	for (int i = 0; i < 4; i++)
	{
		PrintABC(i);
	}
	cout << endl;

	cout << "-----------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < processCount; i++)
	{
		PrintProcess(i);
	}
}



void menu()
{
	char choice;
	int result;
	cout << "Custom or Demo.Please input C or D:";
	cin >> choice;
	if (choice == 'D')
	{
		initDemoMatrix();
		initDemoProcesses();
		PrintTable();
		while (true)
		{
			result = showFunc();
			if (result)
			{
				PrintTable();
				continue;
			}
			else
			{
				break;
			}

		}
	}
	if (choice == 'C')
	{
		cout << "Please input the kinds of resource:";
		cin >> resourceCount;
		cout << "Please input the number of process:";
		cin >> processCount;
		initProcesses();
		initAvaliableResource();
		PrintTable();
		while (true)
		{
			result = showFunc();
			if (result)
			{
				PrintTable();
				continue;
			}
			else
			{
				break;
			}
		}
	}
}


int main()
{
	cout << "------------------Banker Algorithm-----------------" << endl;
	cout << "-------------------Author:PIG-007------------------" << endl;
	cout << "------------------------------------Time:2021.05.27" << endl;
	cout << "---------------------------------------------------" << endl;
	createMatrix();
	createProcess();
	menu();

	return 0;
}