//
//  main.c
//  HashWork
//
//  Created by ibrahim dağcı on 19.12.2022.
//

#include <stdio.h>
#include <string.h>
#define MAX_ROW 10

typedef   struct Customer{
    int customerNumber;
    char customerName[20];
    char customerSurname[20];
    
}Customer;

typedef struct Table{
    Customer rows[MAX_ROW];
    int freeRow;
}Table;

int hashTable[MAX_ROW];
Table CustomerTable;
int processCounter = 0;
int maxSteps = 0;

void defaultInit(void){
    int i ;
    for (i=0; i<MAX_ROW; i++) {
        CustomerTable.rows[i].customerNumber = -1;
        hashTable[i]  = -1;
    }
    CustomerTable.freeRow = MAX_ROW;
}

int hash1(int dataCustomerNumber,int i){
    
    return ((dataCustomerNumber * 3 + 2) + i*(dataCustomerNumber * 7 + 2) )% MAX_ROW;
}

void addCustomer(Customer newCustomer){
    if (CustomerTable.freeRow>0){
        int index = hash1(newCustomer.customerNumber, 0);
        if (hashTable[index] == -1){
            CustomerTable.rows[index] = newCustomer;
            hashTable[index] = newCustomer.customerNumber;
            CustomerTable.freeRow--;
            printf("succesfull:)\n");
        }
        else{
            for (int i = 1; ; i++) {
                index = hash1(newCustomer.customerNumber, i);
                if (hashTable[index] == -1){
                    CustomerTable.rows[index] = newCustomer;
                    hashTable[index] = newCustomer.customerNumber;
                    CustomerTable.freeRow--;
                    printf("succesfull:)\n");
                    break;
                }
            }
        }
    }
    else{
        printf("table ower not be added!!\n");
    }
}

int searchCustomer(int customerNum){
    int index = hash1(customerNum, 0);
    if(CustomerTable.rows[index].customerNumber == customerNum){
        processCounter++;
        return index;
    }
    else{
        processCounter++;
        for (int i = 1 ; ; i++) {
            index = hash1(customerNum, i);
            if(CustomerTable.rows[index].customerNumber == customerNum){
                processCounter++;
                return index;
                break;
            }
            processCounter++;
        }
    }
    return  -1;
}

int searchCustomerFind(int customerNum){
    int index = hash1(customerNum, 0);
    if(CustomerTable.rows[index].customerNumber == customerNum){
        processCounter++;
        return index;
    }
    else{
        processCounter++;
        for (int i = 1 ;i<=maxSteps; i++) {
            index = hash1(customerNum, i);
            if(CustomerTable.rows[index].customerNumber == customerNum){
                processCounter++;
                return index;
                break;
            }
            processCounter++;
        }
    }
    return  -1;
}

void listCustomerTable(void){
    for (int i  = 0; i<MAX_ROW; i++) {
        printf("%d-> %d %s %s\n",i,CustomerTable.rows[i].customerNumber,CustomerTable.rows[i].customerName,CustomerTable.rows[i].customerSurname);
    }

}


Customer readText(FILE *dataFile){
    Customer dataCustomer;
    fscanf(dataFile,"%d %s %s" ,&dataCustomer.customerNumber,dataCustomer.customerName,dataCustomer.customerSurname);
    
    return dataCustomer;
}

void tableInit(FILE *customerFile){
    while(!feof(customerFile)){
        if (CustomerTable.freeRow > 0) {
            Customer c1=readText(customerFile);
            int index  = hash1(c1.customerNumber, 0);
            if (hashTable[index] == -1){
                CustomerTable.rows[index] = c1;
                CustomerTable.freeRow--;
                hashTable[index] = c1.customerNumber;
            }
            else{
                for (int i = 1; ; i++) {
                    index = hash1(c1.customerNumber, i);
                    if (hashTable[index] == -1){
                        CustomerTable.rows[index] = c1;
                        hashTable[index] = c1.customerNumber;
                        CustomerTable.freeRow--;
                        break;
                    }
                }
            }
        }
        else{
            printf("table over !!!\n");
        }
        
    }
}

void averageSteps(void){
    int counter=0;
    float temp=0;
    for (int i=0; i<MAX_ROW; i++) {
        if (hashTable[i]!= -1){
            searchCustomer(hashTable[i]);
            if(maxSteps<processCounter){
                maxSteps = processCounter;
            }
            temp += processCounter;
            processCounter = 0;
            counter++;
            
        }
    }
    temp = temp/counter;
    printf("average steps = %f\n",temp);
    
    
}

void menu(void){
    int a = 1;
    Customer newCustomer;
    int searchingCustomerNum;
    int index;
    while (a == 1) {
        printf("\nplease pick your process...(1,2,3..)\n 1-> add customer\n 2-> list table\n 3-> search customer \n 4-> average number of steps \n 5-> exit\n");
        int pickNum;
        scanf("%d",&pickNum);
        switch (pickNum){
            case 1:
                printf("number->");
                scanf("%d",&newCustomer.customerNumber);
                printf("name->");
                scanf("%s",newCustomer.customerName);
                printf("surname->");
                scanf("%s",newCustomer.customerSurname);
                addCustomer(newCustomer);
                averageSteps();
                printf("max steps = %d\n",maxSteps);
                
                break;
                
            case 2:
                listCustomerTable();
                break;
            
            case 3:
                printf("please write customer number -> (xxxx)\n");
                scanf("%d",&searchingCustomerNum);
                index = searchCustomerFind(searchingCustomerNum);
                if(index==-1){
                    printf("not found!!\n");
                    processCounter = 0;
                }
                else{
                    newCustomer = CustomerTable.rows[index];
                    printf("found in %d steps\n",processCounter);
                    processCounter=0;
                    printf("%d %s %s\n",newCustomer.customerNumber,newCustomer.customerName,newCustomer.customerSurname);
                    
                }
                break;
            
            case 4:
                averageSteps();
                break;
                
            default:
                a = 0;
                return;
                break;
        }
    }
    
}







int main(){
    
    FILE *customerFile=fopen("/Users/ibrahimdagci/Desktop/DataStructure/HashWork/veri.txt","r+");
    defaultInit();
    tableInit(customerFile);
    averageSteps();
    printf("max steps = %d",maxSteps);
    menu();
    if (customerFile != NULL){
        fclose(customerFile);
        printf("customerFile closed\n");
    }
    return  0;
}

