#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
int m_size,g_size,count=0,range,shift=0;
int *data,*gen,*temp,*cpy_gen,*cpy_0,*rem,*quotient,*temp_db;
int intialize()
{
    int i;
    printf("Enter the length of Data Message\n");
    scanf("%d",&m_size);
    data = (int*)malloc(m_size * sizeof(int)); 
    printf("Enter the Data Message\n");
    for(i=0;i<m_size;i++)
        scanf("%d",data+i);
    printf("Enter the length of Generator\n");
    scanf("%d",&g_size);
    gen = (int*)malloc(g_size * sizeof(int));
    printf("Enter the Generator Bit\n");
    for(i=0;i<g_size;i++)
        scanf("%d",gen+i);
    cpy_gen = (int*)malloc(g_size * sizeof(int));
    cpy_0 = (int*)malloc(g_size * sizeof(int));
    quotient = (int*)malloc(m_size * sizeof(int));
    rem  = (int*)malloc(g_size * sizeof(int));
}
int check_valid(int data[])
{
    int i;
    for(i=0;i<m_size;i++)
    {
        if(data[i] > 1 || data[i] < 0)
        {
            printf("Invalid data stream\n");
            exit(0);
        }
    }
    for(i=0;i<g_size;i++)
    {
        if(gen[i] > 1 || gen[i] < 0)
        {
            printf("Invalid Generator\n");
            exit(0);
        }
    }    
}
int copy_databit()
{
    int len=0,i;
    range = m_size+(g_size-1);
    temp = (int*)malloc(range * sizeof(int));
    temp_db = (int*)malloc(range * sizeof(int));
    for(i=0;i<m_size;i++)
    {
        temp[i] = data[i];
        temp_db[i] = data[i];
        len+=1;
    }
    for(i=0;i<g_size-1;i++)
    {
        temp[len] = 0;
        temp_db[len] = 0;
        len++;
    }
    for(i=0;i<g_size;i++)
    {
        cpy_gen[i]=gen[i];
        cpy_0[i]=0;
        rem[i]=1;
    }
}
int shift_rem(int rem[],int data_bit[])
{
    int i;
    for(i=0;i<g_size-1;i++)
        rem[i] = rem[i+1];
    rem[i] = data_bit[g_size + shift];
    shift++;
}
int initial_rem(int bits[],int div[])
{
    int i;
    quotient[0]=1;
    for(i=0;i<g_size;i++)
        rem[i] = bits[i] ^ div[i];
    shift_rem(rem,bits);
}
int reminder(int rem[],int div[])
{
    int i;
    for(i=0;i<g_size;i ++)
        rem[i] = rem[i] ^ div[i];  
}
void display()
{
    int i,len=0;
    for(i=0;i<m_size;i++)
    {
        temp[i] = data[i];
        len++;
    }
    for(i=0;i<g_size-1;i++)
        temp[len++] = rem[i];
    printf("\n\nThe Tansmitted data is: ");
    for(i=0;i<range;i++)
        printf("%d ",temp[i]);
    printf("\n\n");
}
int syndrome(int temp_data[])
{   
    int i;
    initial_rem(temp_data,cpy_gen);
    for(i=1;i<m_size;i++)
    {
        if(rem[0] == 1)
        {
            quotient[i] = 1;
            reminder(rem,cpy_gen);
            shift_rem(rem,temp_data);
        }
        else
        {
            quotient[i] = 0;
            reminder(rem,cpy_0);
            shift_rem(rem,temp_data);
        }
    }
}
int error_check(int temp[])
{   
    int i;
    shift = 0;
    for(i=0;i<g_size;i++)
        rem[i] = 1;
    syndrome(temp);
    for(i=0;i<g_size-1;i++)
    {
        if(rem[i] != 0)
            return 1;
    }
    return 0;
}
void design()
{
    printf("-----------------------------------------------------------------------------------------------------------------------------------------------");
}
int main()
{   
    int j,i,error,choice;
    char choice1;
    system("clear");
    printf("\n\n\n                                                           CYCLIC REDUNDANCY CHECK\n\n\n");
    intialize();
    copy_databit();
    check_valid(data);
    syndrome(temp_db);
    design();
    sleep(2);
    display();
    design();
    printf("\n\nChecking for errors in recieved message\n\n");
    do
    {
        printf("Select an option\n1.Continue with recieved message\n2.Enter a recieved message\n3.Exit\n");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:error = error_check(temp);
                   break;
            case 2:for(i=0;i<range;i++)
                       scanf("%d",temp + i);
                   check_valid(temp);
                   error = error_check(temp);
                   break;
            case 3:exit(0);
        } 
        design();
        sleep(2);  
        if(error)
            printf("\n\nError detected in the recieved message\n\n");
        else
            printf("\n\nNo error detected in the recieved message\n\n");
        design();
        printf("\n\nDo you want to check again(y/Y): ");
        scanf("\n%c",&choice1);
        design();
    }while(choice1 == 'y' || choice1 == 'Y');
}
