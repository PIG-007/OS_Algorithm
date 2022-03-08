#include<stdio.h>
#include<stdlib.h>
#include<iostream>

using namespace std;

struct PCB {
    int PID;
    char state;
    int priority;
    int runTime;
    int workTime;
    struct PCB* next;
};

PCB* head;
int num;

void showInfo(PCB* process) {
    cout << "PID\tState\tPriority\tRunTime\tLeaveTime" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << process->PID << "\t";
    cout << process->state << "\t";
    cout << process->priority << "\t\t";
    cout << process->runTime << "\t";
    cout << process->runTime - process->workTime << "\t" << endl;
    cout << "---------------------------------------------------" << endl;
}




void sortPCB()
{
    PCB* hn = NULL, * p = NULL, * q = NULL, * pmin = NULL, * qmin = NULL;
    while (head)
    {
        for (p = head, pmin = head; p; q = p, p = p->next)
        {
            if (p->priority < pmin->priority)
            {
                pmin = p;
                qmin = q;
            }
        }
        if (pmin == head)
        {
            head = head->next;
        }
        else
        {
            qmin->next = pmin->next;
        }

        pmin->next = hn;
        hn = pmin;
    }
    head = hn;
}


void priorityRun()
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
            (p->priority)--;
            if (p->priority < 0)
            {
                p->priority = 0;
            }
            p->state = 'W';
            p->next = head;
            head = p;
            sortPCB();
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
    head = NULL;
    PCB* p;
    cout << "Please input the information of PCB!" << endl;
    cout << "Split the information by space.Such as \"1 2 3\"!" << endl;
    for (int i = 0; i < num; i++) {
        p = (PCB*)malloc(sizeof(PCB));
        p->PID = i;
        cout << "The " << i << " process(Priority Runtime):";
        scanf("%d %d",  &p->priority, &p->runTime);
        p->workTime = 0;
        p->state = 'W';
        p->next = NULL;
        if (head != NULL)
        {
            p->next = head;
            head = p;
        }
        else
        {
            head = p;
        }

    }
}

int main()
{
    cout << "-----------------Priority Algorithm----------------" << endl;
	cout << "-------------------Author:PIG-007------------------" << endl;
    cout << "------------------------------------Time:2021.05.28" << endl;
    cout << "---------------------------------------------------" << endl;
    cout << "Please input the number of processes:";
    cin >> num;
    initPCB();
    sortPCB();
    priorityRun();
    return 0;
}