#include <stdio.h>
#include <stdlib.h>

typedef struct Process
{
	int name, burst, rt, at;
	struct Process* next;
}Process;

Process* createProcess(int name, int burst, int at)
{
	Process* process = (Process*)malloc(sizeof(Process*));
	process->name = name;
	process->burst = burst;
	process->rt = burst;
    process->at = at;
    process->next = NULL;
	return process;
}

typedef struct Queue
{
	int maxSize;
	Process* front, *rear;    
}Queue;

int isEmpty(Queue* queue){
	return queue->front == NULL;
}

Queue* createQueue(int size){
	Queue* queue = (Queue*)malloc(sizeof(Queue*));
	queue->maxSize = size;
	queue->front = queue->rear = NULL;
	return queue;
}

void enqueue(Queue* queue, Process* process){
    if (queue->front == NULL)
        queue->front = queue->rear = process;
    else{
        queue->rear->next = process;
	    queue->rear = process;
    }
}

// returns the process with shortest remaining time but not dequeues
Process* findShortestRemaining(Queue* queue)
{
	Process* nextProcess = queue->front;
	Process* temp = queue->front;
	while (temp != NULL){
    	if (temp->rt < nextProcess->rt){
        	nextProcess = temp;
        }
    	temp = temp->next;
	}
	return nextProcess;
}

void dequeue(Queue* queue, Process* process){
	if (!isEmpty(queue)){
    	if (queue->front == process){
        	queue->front = queue->front->next;
            if (queue->rear == process)
                queue->rear = NULL;
        }
    	else{
        	Process* temp = queue->front;
        	while (temp->next != process)
            	temp = temp->next;
        	temp->next = process->next;
    	    if (process == queue->rear)
                queue->rear = temp;
        }
	}
}

int main()
{
	int size;
    printf("Enter the number of processes ");
    scanf("%d", &size);

    printf("Enter process name, burst time and arrival time\n");
    Queue* jobQueue = createQueue(size); // stores all the processes
    int name, burst, arrival;
    for (int i=0; i<size; i++){
        scanf("%d %d %d", &name, &burst, &arrival);
        enqueue(jobQueue, createProcess(name, burst, arrival));
    }
    
    Queue* readyQueue = createQueue(size); // contains only processes which are considered for comparison
    int timer = 0; // increments at every iteration of the loop
    int table[size+1][3]; // stores turn around time, waiting time, response time
        
    Process* curr = NULL;
    while (!isEmpty(jobQueue) || !isEmpty(readyQueue)){
        // checks any process should be added to the ready queue
        while (!isEmpty(jobQueue) && jobQueue->front->at == timer){
            Process* t = jobQueue->front;
            enqueue(readyQueue, createProcess(t->name, t->burst, t->at));
            dequeue(jobQueue, t);
            free(t);
        }

        if (isEmpty(readyQueue)){
            timer++;
            continue;
        }
            
        curr = findShortestRemaining(readyQueue);
        
        if (curr->rt == curr->burst)
            table[curr->name][2] = timer - curr->at; //response time
        curr->rt--;
        timer++;
        if (curr->rt == 0){
            dequeue(readyQueue, curr);
            table[curr->name][0] = timer - curr->at; //Turn around time
            table[curr->name][1] = table[curr->name][0] - curr->burst; //Waiting Time
            curr = NULL;
        }
    }

    float ta, wait, response;
    int tta=0, twait=0, tresponse=0;
    printf("TA\tWait\tResponse\n");
    for (int i=1; i<size+1; i++){
        printf("%d\t%d\t%d\n", table[i][0], table[i][1], table[i][2]);
        tta = tta + table[i][0];
        twait = twait + table[i][1];
        tresponse = tresponse + table[i][2];
    }
    ta = (float) tta / size;
    wait = (float) twait / size;
    response = (float) tresponse / size;
    printf("Average Turnaround time = %.2f\n", ta);
    printf("Average waiting time = %.2f\n", wait);
    printf("Average response time = %.2f", response);
    return 0;
}