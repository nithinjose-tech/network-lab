#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void main()
{
    int bucket_size, wait_time, packet_size;
    printf("Enter the bucket size\n");
    scanf("%d", &bucket_size);
    printf("Enter time to wait\n");
    scanf("%d", &wait_time);
    // int flag = 0;
    int temp = bucket_size;
    do
    {
        printf("Enter packet size(-1 to quit)\n");
        scanf("%d", &packet_size);
        if (packet_size > bucket_size)
        {
            printf("Cannot send\n");
        }
        else if (packet_size > temp)
        {
            printf("Waiting\n");
            sleep(wait_time);
            temp = bucket_size;
        }
        else
        {
            temp -= packet_size;
            printf("Added packet of size %d to queue.\nRemaining size in bucket %d\n", packet_size, temp);
        }
    } while (packet_size != -1);
}
