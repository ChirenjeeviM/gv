#include <stdio.h>
#include <stdlib.h>

struct process{
  int pid,bt,at,rt;
  struct process*next;
};

struct process*create_process(int pid,int bt,int at){
  struct process*new_node = malloc(sizeof(struct process*));
  new_node->pid = pid;
  new_node->bt = bt;
  new_node->at = at;
  new_node->rt = bt;
  new_node->next = NULL;
  return new_node;
}

struct Queue{
  int maxsize;
  struct process*front,*rear;
};

struct Queue*create_queue(int size){
  struct Queue* queue = malloc(sizeof(struct Queue*));
  queue->maxsize = size;
  queue->front = NULL;
  queue->rear = NULL;
  return queue;
}

int isEmpty(struct Queue*queue){
  return queue->front == NULL;
}

void enqueue(struct Queue*queue,struct process*node){
  if(isEmpty(queue)){
    queue->front = node;
    queue->rear = node;
  }
  else{
    queue->rear->next = node;
    queue->rear = node;
  }
}

void dequeue(struct Queue*queue,struct process*node){
  if(!isEmpty(queue)){
    if(queue->front == node){
      queue->front = queue->front->next;
      if(queue->rear == node)
        queue->rear = NULL;
    }
    else{
      struct process*temp = queue->front;
      while(temp->next!=node)
        temp=temp->next;
      temp->next = node->next;
      if(node == queue->rear)
        queue->rear = temp;
    }
  }
}

int main(){
  int size;
  printf("Enter the number of processes - ");
  scanf("%d",&size);
  
  int tq;
  printf("Enter the time Quantum - ");
  scanf("%d",&tq);
  
  struct Queue*queue = create_queue(size);
  int i,pid,burst,arrival;
  printf("Enter the process id, burst time, arrival time\n");
  for(i=0;i<size;i++){
    scanf("%d %d %d",&pid,&burst,&arrival);
    enqueue(queue,create_process(pid,burst,arrival));
  }
  
  int timer =0;
  int table[size+1][3];
  struct process*curr = NULL;
  
  while(!isEmpty(queue)){
    if(curr == NULL){
      if(queue->front->at <= timer){
        curr=queue->front;
      }
      else{
        timer++;
        continue;
      }  
    }
    
    if(curr->rt == curr->bt){
      table[curr->pid][2] = timer - curr->at; //response time
    }
    if(curr->rt <= tq){
      timer = timer + curr->rt;
      curr->rt = 0;
      table[curr->pid][0] = timer - curr->at; // turn around time
      table[curr->pid][1] = table[curr->pid][0] - curr->bt; // waiting time
      dequeue(queue,curr);
    }
    else{
      timer = timer + tq;
      curr->rt = curr->rt - tq;
    }
    if(curr->next != NULL &&curr->next->at <= timer){
      curr = curr->next;
    }
    else{
      curr = NULL;
    }
  }
  
  int ttat = 0,twt = 0,trt = 0;
  printf("Tat\tWait\tResponse\n");
  for(i=1;i<size+1;i++){
    printf("%d\t%d\t%d\n",table[i][0],table[i][1],table[i][2]);
    ttat = ttat + table[i][0];
    twt = twt + table[i][1];
    trt = trt + table[i][2];
  }
  printf("Average turn around time = %.2f\n",(float)ttat/size);
  printf("Average waiting time = %.2f\n",(float)twt/size);
  printf("Average response time = %.2f\n",(float)trt/size);
}