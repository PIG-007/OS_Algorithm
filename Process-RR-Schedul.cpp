#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

struct PCB {
	int PID;
	char state;
	int runTime;
	int workTime;
	struct PCB* next;
};

PCB* head;
int num;

void showInfo(PCB* process) {
	cout << "PID\tState\tRunTime\tLeaveTime" << endl;
	cout << "---------------------------------------" << endl;
	cout << process->PID << "\t";
	cout << process->state << "\t";
	cout << process->runTime << "\t";
	cout << process->runTime - process->workTime << "\t" << endl;
}


void tailInsert(PCB* p)
{
	PCB* temp = NULL;
	if (head != NULL)
	{
		temp = head;
		while (temp != NULL)
		{
			if (temp->next == NULL)
			{
				temp->next = p;
				break;
			}
			else {
				temp = temp->next;
			}
		}
	}
	else
	{
		head = p;
	}
}



void roundRobinRun()
{
	PCB* p;
	int runCount = 0;
	while (head != NULL)
	{
		getchar();
		cout << "***************************************************" << endl;
		cout << "Run Times :" << runCount << endl;
		p = head;
		head = p->next;
		p->next = NULL;
		p->state = 'R';

		//show information
		PCB* pr;
		showInfo(p);
		pr = head;
		while (pr != NULL) {
			showInfo(pr);
			pr = pr->next;
		}

		(p->workTime)++;
		if (p->workTime == p->runTime) {
			cout << "Process " << p->PID << " has been done!" << endl;
			free(p);
		}
		else {
			p->state = 'W';
			tailInsert(p);
		}
		runCount++;
		cout << "Press anything to continue!" << endl;
		cout << "***************************************************" << endl;
	}
	cout << "All Process have been done!" << endl;
	getchar();
}



void initPCB()
{
	PCB* temp = NULL;
	PCB* p;
	cout << "Please input the information of PCB!" << endl;
	cout << "Split the information by space.Such as \"1 2\"!" << endl;
	for (int i = 0; i < num; i++) {
		p = (PCB*)malloc(sizeof(PCB));
		p->PID = i;
		cout << "The " << i << " process(Runtime):";
		scanf_s("%d",&p->runTime);
		p->workTime = 0;
		p->state = 'W';
		p->next = NULL;
		tailInsert(p);
	}
	p = NULL;
	temp = NULL;
}


int main()
{
	cout << "--------------------RR Algorithm-------------------" << endl;
	cout << "-------------------Author:PIG-007------------------" << endl;
	cout << "------------------------------------Time:2021.05.27" << endl;
	cout << "---------------------------------------------------" << endl;
	cout << "Please input the number of processes:";
	cin >> num;
	initPCB();
	roundRobinRun();
	return 0;
}


