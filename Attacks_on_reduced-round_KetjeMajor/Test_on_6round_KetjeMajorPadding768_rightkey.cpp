#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char UINT8;
typedef unsigned long int UINT32;
typedef unsigned long long int UINT64;
typedef signed long long int INT64;


#define random(x) (rand())%x;
#define nrRounds 6
UINT64 KeccakRoundConstants[nrRounds];//these are constant,
#define nrLanes 25
unsigned int KeccakRhoOffsets[nrLanes];//these are constant,
#define nrMessage 0x8000
#define index(x, y) (((x)%5)+5*((y)%5))
#define ROL64(a, offset) ((offset != 0) ? ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset))) : a)


void KeccakPermutationOnWords(UINT64 *state);
void theta(UINT64 *A);
void rho(UINT64 *A);
void pi(UINT64 *A);
void chi(UINT64 *A);
void iota(UINT64 *A, unsigned int indexRound);


void KeccakPermutationOnWords(UINT64 *state)
{
    unsigned int i;


    for(i=0; i<nrRounds; i++) {
        theta(state);
        rho(state);
        pi(state);
        chi(state);
        iota(state, i);
    }
}


void theta(UINT64 *A)
{
    unsigned int x, y;
    UINT64 C[5], D[5];//C are the Xors of the five bits in every column. D are the Xors of the ten bits in right-behind column and right column

    for(x=0; x<5; x++) {
        C[x] = 0;
        for(y=0; y<5; y++)
            C[x] ^= A[index(x, y)];
    }
    for(x=0; x<5; x++)
        D[x] = ROL64(C[(x+1)%5], 1) ^ C[(x+4)%5];
    for(x=0; x<5; x++)
        for(y=0; y<5; y++)
            A[index(x, y)] ^= D[x];
}

void rho(UINT64 *A)
{
    unsigned int x, y;

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(x, y)] = ROL64(A[index(x, y)], KeccakRhoOffsets[index(x, y)]);
}

void pi(UINT64 *A)
{
    unsigned int x, y;
    UINT64 tempA[25];

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        tempA[index(x, y)] = A[index(x, y)];
    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(0*x+1*y, 2*x+3*y)] = tempA[index(x, y)];//learn from this!
}

void chi(UINT64 *A)
{
    unsigned int x, y;
    UINT64 C[5];

    for(y=0; y<5; y++) {
        for(x=0; x<5; x++)
            C[x] = A[index(x, y)] ^ ((~A[index(x+1, y)]) & A[index(x+2, y)]);
        for(x=0; x<5; x++)
            A[index(x, y)] = C[x];
    }
}

void iota(UINT64 *A, unsigned int indexRound)
{
    A[index(0, 0)] ^= KeccakRoundConstants[indexRound];
}



int LFSR86540(UINT8 *LFSR)
{
    int result = ((*LFSR) & 0x01) != 0;
    if (((*LFSR) & 0x80) != 0)
        // Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1
        (*LFSR) = ((*LFSR) << 1) ^ 0x71;
    else
        (*LFSR) <<= 1;
    return result;
}

void KeccakInitializeRoundConstants()
{
    UINT8 LFSRstate = 0x01;
    unsigned int i, j, bitPosition;

    for(i=0; i<nrRounds; i++) {
        KeccakRoundConstants[i] = 0;
        for(j=0; j<7; j++) {
            bitPosition = (1<<j)-1; //2^j-1
            if (LFSR86540(&LFSRstate))
                KeccakRoundConstants[i] ^= (UINT64)1<<bitPosition;
        }
    }
}

void KeccakInitializeRhoOffsets()
{
    unsigned int x, y, t, newX, newY;

    KeccakRhoOffsets[index(0, 0)] = 0;
    x = 1;
    y = 0;
    for(t=0; t<24; t++) {
        KeccakRhoOffsets[index(x, y)] = ((t+1)*(t+2)/2) % 64;
        newX = (0*x+1*y) % 5;
        newY = (2*x+3*y) % 5;
        x = newX;
        y = newY;
    }
}

void KeccakInitialize()
{
    KeccakInitializeRoundConstants();
    KeccakInitializeRhoOffsets();
}

void displaystate(UINT64 *state)
{
	unsigned int i;
	for(i=0;i<nrLanes;i++)
	{
		printf("%08x ",(unsigned int)(state[i]));
		if((i+1)%5==0) printf("\n");
	}
	printf("\n");
}

int main()
{
	clock_t start,finish;
	start = clock();

	UINT64 InitialState[25]={0};
	UINT64 TempState[25]={0};
	UINT64 FinalState[2]={0};
	UINT64 Key[3]={0};
	int tempkey[3]={0};


	UINT64 i,j,k,temp,temp1,counter,a;
	unsigned int rightkey[3]={0};

	unsigned int index0[2][2];
	index0[0][0]=3;index0[1][0]=0; 
    index0[0][1]=16;index0[1][1]=0;


	unsigned int index[3][100];

	index[0][0]=9;index[1][0]=24;index[2][0]=2;  
	index[0][1]=9;index[1][1]=24;index[2][1]=4;  
	index[0][2]=9;index[1][2]=24;index[2][2]=10;  
	index[0][3]=9;index[1][3]=24;index[2][3]=11; 
	index[0][4]=3;index[1][4]=18;index[2][4]=14; 
	index[0][5]=3;index[1][5]=18;index[2][5]=17; 
	index[0][6]=9;index[1][6]=24;index[2][6]=19; 
	index[0][7]=9;index[1][7]=24;index[2][7]=20;
	index[0][8]=9;index[1][8]=24;index[2][8]=27; 
	index[0][9]=3;index[1][9]=18;index[2][9]=28;  
	index[0][10]=9;index[1][10]=24;index[2][10]=28;
	index[0][11]=3;index[1][11]=18;index[2][11]=33; 
	index[0][12]=3;index[1][12]=18;index[2][12]=36; 
	index[0][13]=3;index[1][13]=18;index[2][13]=37; 
	index[0][14]=9;index[1][14]=24;index[2][14]=38;  
	index[0][15]=3;index[1][15]=18;index[2][15]=45;  
	index[0][16]=9;index[1][16]=24;index[2][16]=59; 
	index[0][17]=9;index[1][17]=24;index[2][17]=60;  
	index[0][18]=12;index[1][18]=22;index[2][18]=18; 
	index[0][19]=12;index[1][19]=22;index[2][19]=19; 
	index[0][20]=12;index[1][20]=22;index[2][20]=51; 
	index[0][21]=12;index[1][21]=22;index[2][21]=27; 
	index[0][22]=12;index[1][22]=22;index[2][22]=28; 
	index[0][23]=12;index[1][23]=22;index[2][23]=52; 
	index[0][24]=12;index[1][24]=22;index[2][24]=53;
	index[0][25]=12;index[1][25]=22;index[2][25]=36;  
	index[0][26]=12;index[1][26]=22;index[2][26]=37;
	index[0][27]=12;index[1][27]=22;index[2][27]=39; 
	index[0][28]=12;index[1][28]=22;index[2][28]=55; 
	index[0][29]=12;index[1][29]=22;index[2][29]=60;
	index[0][30]=12;index[1][30]=22;index[2][30]=62; 

	


	FILE *f;
	f=fopen("cubesum.txt","w+b");
	srand(time(NULL));

	
	for(i=0;i<25;i++){
		InitialState[i]=0;
	}
	for(i=0;i<3;i++){
		Key[i]=0;
	}
	Key[0]^=((UINT64)0x1<<1);
	Key[0]^=((UINT64)0x1<<4);
	for(i=8;i<64;i++){
		temp=random(2);
		if(temp)
		{
			Key[0]^=((UINT64)0x1<<i);
		}
	}
	for(i=0;i<64;i++){
		temp=random(2);
		if(temp)
		{
			Key[1]^=((UINT64)0x1<<i);
		}
	}
	for(i=0;i<8;i++){
		temp=random(2);
		if(temp)
		{
			Key[2]^=((UINT64)0x1<<i);
		}
	}
	Key[2]^=((UINT64)0x1<<8);



	InitialState[0]=Key[0];
	InitialState[6]=Key[1];
	InitialState[12]=Key[2];
 


	 rightkey[0]=((Key[1]>>42)&1);      
	 rightkey[1]=((Key[1]>>31)&1);    
	 rightkey[2]=((Key[0]>>38)&1)^((Key[1]>>38)&1);    
	




	fprintf(f,"right key:");
	printf("right key:");
	for(i=0;i<3;i++)
	{
		if(rightkey[i])
		{
			fprintf(f,"1");
			printf("1");
		}
		else
		{
			fprintf(f,"0");
			printf("0");
		}
	}
	fprintf(f,"\n");
	printf("\n");

	KeccakInitialize();
	counter=0;
	
	for(j=0;j<2;j++){
		FinalState[j]=0;
	}	
		
		
	for(j=0;j<3;j++){
		tempkey[j]=rightkey[j];
	}
	fprintf(f,"temp  key:");
	printf("temp  key:");

	for(j=0;j<3;j++)
	{
	if(tempkey[j])
	{
		fprintf(f,"1");
		printf("1");
	}
	else
	{
		fprintf(f,"0");
		printf("0");
	}
			
	}
	fprintf(f,"\n");
	printf("\n");
	
	//give the value for dynamic value
	
	
	temp=(InitialState[18]>>41)&1;
	temp1=(tempkey[0]&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[18]^=(UINT64)0x1<<41;
	}

	temp=(InitialState[22]>>31)&1;
	temp1=(tempkey[1]&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[22]^=(UINT64)0x1<<31;
	}

	temp=(InitialState[22]>>11)&1;
	temp1=1;
	if(temp!=temp1)
	{
		InitialState[22]^=(UINT64)0x1<<11;
	}

	temp=(InitialState[10]>>38)&1;
	temp1=(tempkey[2]&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[10]^=(UINT64)0x1<<38;
	}


	//displaystate(InitialState);
	for(j=0;j<(UINT64(1)<<32);j++)//initialize tempstate
	{
		for(k=0;k<25;k++)//fresh the tempstate for the key and initial value
		{
			TempState[k]=InitialState[k];
		}
		//give the value for cube variables

		temp1=j&1;
		for(k=0;k<2;k++){
			TempState[index0[0][k]]^=UINT64(temp1)<<index0[1][k];
		}
		for(k=1;k<32;k++)
		{
			temp1=(j>>k)&1;
			if(temp1){
				TempState[index[0][k-1]]|=UINT64(1)<<index[2][k-1];
				TempState[index[1][k-1]]|=UINT64(1)<<index[2][k-1];
			}
			else{
				TempState[index[0][k-1]]&=(~(UINT64(1)<<index[2][k-1]));
				TempState[index[1][k-1]]&=(~(UINT64(1)<<index[2][k-1]));
			}
					
		}
		
			
			
		KeccakPermutationOnWords((UINT64 *)TempState);
			
		for(k=0;k<2;k++){
			FinalState[k]^=TempState[6*k];
		}
	}
	fprintf(f,"\n");
	printf("\n");
	fprintf(f,"cube sum: %llx,\t%llx\n",FinalState[0],FinalState[1]);
	printf("cube sum: %llx,\t%llx\n",FinalState[0],FinalState[1]);
	if(FinalState[0]==0 && FinalState[1]==0)
	{
		fprintf(f,"zero cube sum, temp key is right key");
		printf("zero cube sum, temp key is right key");
	}
	else{
		fprintf(f,"nonzero cube sum, temp key is wrong key");
		printf("nonzero cube sum, temp key is wrong key");
	}

	fclose(f);
	finish = clock();
	printf("time=%f\n",(double)(finish-start)/CLK_TCK);
	printf("Done!\n");
    getch();
    return 0;

}
