#include<stdio.h>
#include<stdlib.h>
# define name 0
# define burst_time 1
# define remaining_time 2
# define arrival_time 3 
# define priority 4

typedef struct Process{
  int* details;
  struct Process* next;
}Process;

Process* createProcess(int p_name, int burst, int at, int pri){
  Process* process = (Process*)malloc(sizeof(Process*));
  process->details = (int*)malloc(5 * sizeof(int));
  process->details[name] = p_name;
  process->details[burst_time] = burst;
  process->details[remaining_time] = burst;
  process->details[arrival_time] = at;
  process->details[priority] = pri;
  process->next = NULL;
  return process;
}

typedef struct Queue{
  int size;
  Process* front, *rear;
}Queue;

int isEmpty(Queue* queue){
  return queue->front == NULL;
}

Queue* createQueue(int size){
  Queue* queue = (Queue*)malloc(sizeof(Queue*));
  queue->size = size;
  queue->front = queue->rear = NULL;
  return queue;
}

void enqueue(Queue* queue, Process* process){
  if (isEmpty(queue)){
    queue->front = queue->rear = process;
  }
  else{
    queue->rear->next = process;
    queue->rear = process;
  }
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
      while(temp->next != process)
        temp = temp->next;
      temp->next = process->next;
      if (queue->rear == process)
        queue->rear = temp;
    }
  }
}

Process* findHighPriority(Queue* queue){
  Process* nextProcess = queue->front;
  Process* temp = queue->front->next;
  while (temp != NULL){
    if (temp->details[priority] < nextProcess->details[priority])
      nextProcess = temp;
    temp = temp->next;
  }
  return nextProcess;
}

int main()
{
  int size;
  printf("Enter number of processes: ");
  scanf("%d", &size);

  Queue* jobQueue = createQueue(size);
  int p_name, burst, at, pri;
  printf("Enter name, burst time, arrival time and priority of each process\n");
  for (int i=0; i<size; i++){
    scanf("%d %d %d %d", &p_name, &burst, &at, &pri);
    Process* process = createProcess(p_name, burst, at, pri);
    enqueue(jobQueue, process);
  }

  int table[size+1][3];
  int timer = 0;
  Process* curr = NULL;
  Queue* readyQueue = createQueue(size);

  while (!isEmpty(jobQueue) || !isEmpty(readyQueue)){
    while (!isEmpty(jobQueue) && jobQueue->front->details[arrival_time] <= timer){
      Process* temp = jobQueue->front;
      dequeue(jobQueue, jobQueue->front);
      temp->next = NULL;
      enqueue(readyQueue, temp);
    }

    if (isEmpty(readyQueue)){
      timer++;
      continue;
    }

    curr = findHighPriority(readyQueue);
    if (curr->details[remaining_time] == curr->details[burst_time])
      table[curr->details[name]][2] = timer - curr->details[arrival_time];
    
    curr->details[remaining_time]--;
    timer++;

    if (curr->details[remaining_time] == 0){
      table[curr->details[name]][0] = timer - curr->details[arrival_time];
      table[curr->details[name]][1] = table[curr->details[name]][0] - curr->details[burst_time];
      dequeue(readyQueue, curr);
    }
  }

  int tta = 0, twait = 0, tresponse = 0;
  printf("Process\tTA\tWait\tResponse\n");
  for (int i=1; i<size+1; i++){
    printf("P%d\t%d\t%d\t%d\n", i, table[i][0], table[i][1], table[i][2]);
    tta = tta + table[i][0];
    twait = twait + table[i][1];
    tresponse = tresponse + table[i][2];
  }

  float ta = (float)tta / size;
  float wait = (float)twait / size;
  float response = (float)tresponse / size;
  printf("Average turnaround time = %.2f\n", ta);
  printf("Average waiting time = %.2f\n", wait);
  printf("Average response time = %.2f", response);
  return 0;
}