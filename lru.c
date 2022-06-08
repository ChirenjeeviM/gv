#include<stdio.h>

int checkHit(int pageRef, int queue[], int occupied)
{
    for(int i = 0; i < occupied; i++){
        if(pageRef == queue[i])
            return 1;
    }
    return 0;
}

void printOuterStructure(int frames)
{   
    printf("Page ");

    for(int i = 0; i < frames; i++)
        printf("\tFrame%d \t", i+1);

    printf("\n");
}

void printFrame(int queue[], int occupied)
{
    for(int i = 0; i < occupied; i++)
        printf("%d \t\t",queue[i]);
}

int main()
{
    int n;
    printf("Enter the number of page references: ");
    scanf("%d", &n);

    int pageRef[n];
    printf("Enter the page references in sequence: ");
    for(int i = 0; i < n; i++)
        scanf("%d", &pageRef[i]);

    int frames;
    printf("Enter the number of frames: ");
    scanf("%d", &frames);

    int queue[frames];
    int distance[n];
    int occupied = 0;
    int pagefault = 0;

    printOuterStructure(frames);

    for(int i = 0;i < n; i++)
    {
        printf("%d:   \t",pageRef[i]);
        if(checkHit(pageRef[i], queue, occupied))
        {
            printFrame(queue, occupied);
        }
        else if(occupied < frames)
        {
            queue[occupied] = pageRef[i];
            pagefault++;
            occupied++;

            printFrame(queue, occupied);
        }
        else
        {
            int max = -1;
            int index;

            for (int j = 0; j < frames; j++)
            {
                distance[j] = 0;
                for(int k = i - 1; k >= 0; k--)
                {
                    ++distance[j];

                    if(queue[j] == pageRef[k])
                        break;
                }
                if(distance[j] > max)
                {
                    max = distance[j];
                    index = j;
                }
            }
            queue[index] = pageRef[i];
            printFrame(queue, occupied);
            pagefault++;
        }
        printf("\n");
    }

    printf("Page Fault: %d\n",pagefault);
    return 0;
}