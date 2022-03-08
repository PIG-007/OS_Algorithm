#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include <time.h>

using namespace std;

string table[6] = { "Process","MAX(Amount)","Allocation(Amount)","Need(Amount)","LatsApply(Serial Number)","State" };
struct PCB {
	int  PID;
	int  maxRequestResource;
	int  lastApplyResourceNumber;
	int  runApplyResourceNumber;
	char state;
	vector<int> allocatedResource;
};
struct RCB {
	int  RID;
	int  rState;
};


vector<vector<int>> processApplySequence;
PCB** processes;
RCB** resources;
int resourceCount = 10;
int processCount = 3;
char alogrithmKind;
char dispalyKind;
int* maxResourceArray;



//create and init func
void createProcess()
{
	processes = new PCB * [processCount];
	for (int i = 0; i < processCount; i++) {
		processes[i] = new PCB;
	}
}
void createResource()
{
	resources = new RCB * [resourceCount];
	for (int i = 0; i < resourceCount; i++) {
		resources[i] = new RCB;
	}
}
void initProcesses() {
	for (int i = 0; i < processCount; i++)
	{

		processes[i]->maxRequestResource = maxResourceArray[i];
		processes[i]->runApplyResourceNumber = 0;
		processes[i]->lastApplyResourceNumber = 0;
		processes[i]->state = 'R';
		processes[i]->PID = i;
	}
}
void initResources() {
	for (int i = 0; i < resourceCount; i++)
	{
		resources[i]->RID = i + 1;
		resources[i]->rState = 0;
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
void PrintProcess(int i)
{
	cout << "|";
	cout << "P" << i;
	PrintStringSpace(0);

	cout << "|";
	cout << processes[i]->maxRequestResource << " ";
	PrintStringSpace(1);

	cout << "|";
	cout << processes[i]->allocatedResource.size() << " ";
	PrintStringSpace(2);

	cout << "|";
	cout << processes[i]->maxRequestResource - processes[i]->allocatedResource.size() << " ";
	PrintStringSpace(3);

	cout << "|";
	cout << processes[i]->lastApplyResourceNumber << " ";
	PrintStringSpace(4);

	cout << "|";
	cout << processes[i]->state << " ";
	PrintStringSpace(5);

}
void PrintAssignableResources()
{
	cout << "The serial number of assignable resource is : ";
	for (int i = 0; i < resourceCount; i++)
	{
		if (resources[i]->rState == 0)
		{
			cout << resources[i]->RID << " ";
		}
	}
	cout << endl;
}
void PrintTable()
{
	cout << "-----------------------------------------------Table--------------------------------------" << endl;
	for (auto& i : table)
	{
		cout << "|" << i << "  ";

	}
	cout << endl;

	cout << "------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < processCount; i++)
	{
		PrintProcess(i);
		cout << endl;
	}

	cout << "------------------------------------------------------------------------------------------" << endl;
	PrintAssignableResources();
	cout << "------------------------------------------------------------------------------------------" << endl;
}

//functional func
void distribute(int PID)
{
	processes[PID]->lastApplyResourceNumber = processes[PID]->runApplyResourceNumber;
	processes[PID]->allocatedResource.push_back(processes[PID]->runApplyResourceNumber);
	resources[processes[PID]->runApplyResourceNumber - 1]->rState = 1;
	processes[PID]->state = 'R';
	if (processes[PID]->maxRequestResource == processes[PID]->allocatedResource.size())
	{
		processes[PID]->state = 'F';
		vector<int> temp = processes[PID]->allocatedResource;
		processApplySequence.push_back(temp);
		for (auto i : processes[PID]->allocatedResource)
		{
			resources[i - 1]->rState = 0;
		}
		processes[PID]->allocatedResource.clear();
		processes[PID]->maxRequestResource = 0;
		processes[PID]->runApplyResourceNumber = 0;
		processes[PID]->lastApplyResourceNumber = 0;
	}
}
int safeCheck(int PID)
{
	//last apply == 0;The leave resource can satisfy the process;
	if (processes[PID]->lastApplyResourceNumber == 0 &&
		(resourceCount - processes[PID]->runApplyResourceNumber + 1) >=
		(processes[PID]->maxRequestResource - processes[PID]->allocatedResource.size()))
	{
		distribute(PID);
		return 1;
	}
	else {
		if (processes[PID]->runApplyResourceNumber <= processes[PID]->lastApplyResourceNumber)
		{
			cout << "Sorry.You can't apply the resource!It maybe cause the loop waiting !Again!" << endl;
			return 0;
		}
		else
		{
			if ((resourceCount - processes[PID]->runApplyResourceNumber + 1) <
				(processes[PID]->maxRequestResource - processes[PID]->allocatedResource.size()))
			{
				cout << "Sorry.You can't apply the resource!" << endl;
				cout << "The leave resource can't satisfy the process' requirement!Again!" << endl;
				cout << "The process can only apply the number of resource is " << processes[PID]->lastApplyResourceNumber + 1
					<< " to " << resourceCount - (processes[PID]->maxRequestResource - processes[PID]->allocatedResource.size()) + 1
					<< "!" << endl;
				return 0;
			}
		}
		distribute(PID);
		return 1;
	}
}
int finishCheck()
{
	int flag = 0;
	for (int i = 0; i < processCount; i++)
	{
		if (processes[i]->state == 'F')
			flag += 1;
	}
	if (flag == processCount)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
int menu()
{
	char choice;
	int PID;
	if (finishCheck())
	{
		cout << "All process have been done!The apply sequence is :" << endl;
		for (int i = 0; i < processCount; i++)
		{
			cout << "P" << i << ":";
			for (int j = 0; j < processApplySequence.at(i).size(); j++)
			{
				cout << processApplySequence.at(i).at(j) << " ";
			}
			cout << endl;
		}
		return 0;
	}

	cout << "Apply some resource for one process?(Y/N):";
	cin >> choice;
	if (choice == 'Y')
	{
		while (1)
		{
			cout << "Please choose one process(Index:0 or 1 or ...):";
			cin >> PID;
			//PID in range
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
			if (processes[PID]->state == 'W')
			{
				//The waiting resource has been released
				if (resources[processes[PID]->runApplyResourceNumber - 1]->rState == 0)
				{
					distribute(PID);
					return 1;
				}
				else
				{
					cout << "The process is waiting for resource " << processes[PID]->runApplyResourceNumber
						<< " which have been assigned to another Process!" << endl
						<< "Choose another Process!" << endl;
					return 1;
				}
			}
			cout << "What kind of resource do you want to apply for the process(such 1 or 2 or ...):";
			cin >> processes[PID]->runApplyResourceNumber;
			//RID in range
			if (processes[PID]->runApplyResourceNumber <= 0 || processes[PID]->runApplyResourceNumber > 10)
			{
				cout << "Out of the amoun of resources!Again!" << endl;
				processes[PID]->runApplyResourceNumber = 0;
				return 1;
			}
			//Determine whether the resource is used
			if (resources[processes[PID]->runApplyResourceNumber - 1]->rState == 1)
			{
				if (processes[PID]->lastApplyResourceNumber == processes[PID]->runApplyResourceNumber)
				{
					cout << "Sorry.You have applied for the resource!Again!" << endl;
				}
				else
				{
					cout << "Sorry.You can't apply the resource!It has been allocated!Please wait!" << endl;
					processes[PID]->state = 'W';
				}
			}
			else
			{
				if (alogrithmKind == 'S')
				{
					if (safeCheck(PID))
					{
						break;
					}
					else
					{
						continue;
					}
				}
				else if (alogrithmKind == 'R')
				{
					distribute(PID);
					break;
				}
			}
		}
		return 1;
	}
	else
	{
		return 1;
	}

}

//init func
int initCustomMaxResourceArray()
{
	maxResourceArray = new int[processCount];
	for (int i = 0; i < processCount; i++)
	{
		cout << "Please input the maxRequestResource of process " << i << " :";
		cin >> maxResourceArray[i];
		if (maxResourceArray[i] > resourceCount || maxResourceArray[i] < 0)
		{
			cout << "The maxRequestResource of process " << i << " has exceeded the system resource or inValid!Agadin!" << endl;
			maxResourceArray[i] = 0;
			return 0;
		}
		else
		{
			continue;
		}
	}
	return 1;

}
void initCustom()
{
	cout << "Please input the amount of resource:";
	cin >> resourceCount;
	cout << "Please input the amount of process:";
	cin >> processCount;

	//init Custom Max Resource Array
	while (1)
	{
		if (initCustomMaxResourceArray())
		{
			break;
		}
		else {
			continue;
		}
	}
}
void initDemo()
{
	maxResourceArray = new int[processCount];
	srand((unsigned)time(NULL));
	for (int i = 0; i < processCount; i++)
	{
		//maxResourceArray[i] = 6;  //test
		maxResourceArray[i] = (rand() % resourceCount) + 1;
	}

}

//choose func
void alogrithmChoose()
{
	while (1)
	{
		cout << "Sequential Alogrithm or Random Alogrithm?(S or R):";
		cin >> alogrithmKind;
		if (alogrithmKind == 'S' || alogrithmKind == 'R')
		{
			break;
		}
		else
		{
			continue;
		}
	}
}
void demoOrCustom()
{
	while (1)
	{
		cout << "Custom or Demo.Please input C or D:";
		cin >> dispalyKind;
		if (dispalyKind == 'D' || dispalyKind == 'C')
		{
			break;
		}
		else
		{
			continue;
		}
	}


}



int main()
{
	cout << "----------------Sequential Algorithm---------------" << endl;
	cout << "-------------------Author:PIG-007------------------" << endl;
	cout << "------------------------------------Time:2021.05.28" << endl;
	cout << "---------------------------------------------------" << endl;

	demoOrCustom();
	//dispaly
	if (dispalyKind == 'D')
	{
		initDemo();
	}
	else if (dispalyKind == 'C')
	{
		initCustom();
	}
	createProcess();
	createResource();
	initProcesses();
	initResources();
	PrintTable();

	while (1)
	{
		//choose alogrithm
		alogrithmChoose();
		if (menu())
		{
			PrintTable();
		}
		else {
			break;
		}
	}

	return 0;
}



