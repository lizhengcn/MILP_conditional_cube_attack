/*
	Teston6rKetjeMinor_rightkey.cpp;
	Test whether the cube sum with the right key guess is zero or not. 
	The program is run in Visual Studio 2010 with x64 platform Release.  
	One experiment averagely takes 76 minutes with this procedure.  
*/


#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

typedef unsigned char UINT8;
typedef unsigned long int UINT32;
typedef signed long long int INT64;
typedef unsigned long long int UINT64;

#define random(x) (rand())%x;
#define nrRounds 6
UINT32 KeccakRoundConstants[nrRounds];//these are constant,
#define nrLanes 25
unsigned int KeccakRhoOffsets[nrLanes];//these are constant,
#define nrMessage 0x8000

void KeccakPermutationOnWords(UINT32 *state);
void theta(UINT32 *A);
void rho(UINT32 *A);
void pi(UINT32 *A);
void chi(UINT32 *A);
void iota(UINT32 *A, unsigned int indexRound);



void KeccakPermutationOnWords(UINT32 *state)
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


// used in theta
#define index(x, y) (((x)%5)+5*((y)%5))
//used in theta
#define ROL32(a, offset) ((offset != 0) ? ((((UINT32)a) << offset) ^ (((UINT32)a) >> (32-offset))) : a)


void theta(UINT32 *A)
{
    unsigned int x, y;
    UINT32 C[5], D[5];//C are the Xors of the five bits in every column. D are the Xors of the ten bits in right-behind column and right column

    for(x=0; x<5; x++) {
        C[x] = 0;
        for(y=0; y<5; y++)
            C[x] ^= A[index(x, y)];
    }
    for(x=0; x<5; x++)
        D[x] = ROL32(C[(x+1)%5], 1) ^ C[(x+4)%5];
    for(x=0; x<5; x++)
        for(y=0; y<5; y++)
            A[index(x, y)] ^= D[x];
}

void rho(UINT32 *A)
{
    unsigned int x, y;

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(x, y)] = ROL32(A[index(x, y)], KeccakRhoOffsets[index(x, y)]);
}

void pi(UINT32 *A)
{
    unsigned int x, y;
    UINT32 tempA[25];

    for(x=0; x<5; x++) for(y=0; y<5; y++)
        tempA[index(x, y)] = A[index(x, y)];
    for(x=0; x<5; x++) for(y=0; y<5; y++)
        A[index(0*x+1*y, 2*x+3*y)] = tempA[index(x, y)];//learn from this!
}

void chi(UINT32 *A)
{
    unsigned int x, y;
    UINT32 C[5];

    for(y=0; y<5; y++) {
        for(x=0; x<5; x++)
            C[x] = A[index(x, y)] ^ ((~A[index(x+1, y)]) & A[index(x+2, y)]);
        for(x=0; x<5; x++)
            A[index(x, y)] = C[x];
    }
}

void iota(UINT32 *A, unsigned int indexRound)
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
                KeccakRoundConstants[i] ^= (UINT32)1<<bitPosition;
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
        KeccakRhoOffsets[index(x, y)] = ((t+1)*(t+2)/2) % 32;
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



int main()
{
	clock_t start,finish;
	start = clock();
	
    UINT32 InitialState[25]={0};
	UINT32 TempState[25]={0};
	UINT32 FinalState[4]={0};
	UINT32 Key[5]={0};
	int tempkey[12]={0};
	int guessedkey[40][12]={0};
	int GIndex[40];


	UINT64 i,j,k,temp,temp1;

	unsigned int rightkey[12]={0};

	unsigned int index0[2][6];
	index0[0][0]=5;index0[1][0]=19; 
    index0[0][1]=15;index0[1][1]=19;
    index0[0][2]=11;index0[1][2]=15;
    index0[0][3]=16;index0[1][3]=15;
    index0[0][4]=8;index0[1][4]=0;  
    index0[0][5]=13;index0[1][5]=0; 

	unsigned int index[2][31];
	index[0][0]=2;index[1][0]=2;
	index[0][1]=2;index[1][1]=4;
	index[0][2]=2;index[1][2]=7;
	index[0][3]=2;index[1][3]=11;
	index[0][4]=2;index[1][4]=12;
	index[0][5]=2;index[1][5]=20;
	index[0][6]=2;index[1][6]=23;
	index[0][7]=2;index[1][7]=29;
	index[0][8]=2;index[1][8]=30;
	index[0][9]=3;index[1][9]=3;
	index[0][10]=3;index[1][10]=6;
	index[0][11]=3;index[1][11]=12;
	index[0][12]=3;index[1][12]=13;
	index[0][13]=3;index[1][13]=17;
	index[0][14]=3;index[1][14]=21;
	index[0][15]=3;index[1][15]=22;
	index[0][16]=3;index[1][16]=26;
	index[0][17]=3;index[1][17]=31;
	index[0][18]=4;index[1][18]=0;
	index[0][19]=4;index[1][19]=5;
	index[0][20]=4;index[1][20]=8;
	index[0][21]=4;index[1][21]=15;
	index[0][22]=4;index[1][22]=18;
	index[0][23]=4;index[1][23]=22;
	index[0][24]=4;index[1][24]=24;
	index[0][25]=10;index[1][25]=1;
	index[0][26]=10;index[1][26]=5;
	index[0][27]=10;index[1][27]=10;
	index[0][28]=10;index[1][28]=15;
	index[0][29]=10;index[1][29]=31;
	index[0][30]=11;index[1][30]=4;	


	FILE *f;
	f=fopen("result.txt","w+b");
	srand(time(NULL));

	
	for(i=8;i<32;i++){
		temp=random(2);
		if(temp){
			Key[0]^=((UINT32)0x1<<i);
			fprintf(f,"1");
		}
		else{
			fprintf(f,"0");
		}
	}
	Key[0]^=((UINT32)0x1<<1);
	Key[0]^=((UINT32)0x1<<4);
	for(i=1;i<4;i++){
		for(j=0;j<32;j++){
			temp=random(2);
			if(temp)
			{
				Key[i]^=((UINT32)0x1<<j);
				fprintf(f,"1");
			}
			else{
				fprintf(f,"0");
			}
		}
	}
	for(i=0;i<8;i++){
		temp=random(2);
		if(temp){
			Key[4]^=((UINT32)0x1<<i);
			fprintf(f,"1");
		}
		else{
			fprintf(f,"0");
		}
	}
	fprintf(f,"\n");

	Key[4]^=((UINT32)0x1<<8);


	for(i=0;i<40;i++)
	{
		GIndex[i]=-1;
	}

	for(i=0;i<25;i++){
		InitialState[i]=0;
	}

	for(i=0;i<5;i++){
		InitialState[6*i]=Key[i];
	}

	/*for(i=0;i<25;i++)
	{
		if(i%6!=0){
			for(j=0;j<64;j++)
			{
				temp=random(2);
				if(temp)
				{
					InitialState[i]^=((UINT32)0x1<<j);
				}
			}
		}
	}*/
	InitialState[21]|=((UINT32)0x1<<30);
	InitialState[21]|=((UINT32)0x1<<31);

	

	rightkey[0]=((Key[1]>>24)&1);                       
	rightkey[1]=((Key[1]>>31)&1) ^ ((Key[3]>>30)&1);    
	rightkey[2]=((Key[1]>>19)&1) ^ ((Key[3]>>18)&1);    
	rightkey[3]=((Key[0]>>(8+8))&1) ^ ((Key[3]>>17)&1);     
	rightkey[4]=((Key[0]>>(5+8))&1) ^ ((Key[2]>>12)&1);     
	rightkey[5]=((Key[1]>>20)&1);                       
	rightkey[6]=((Key[1]>>10)&1) ^ ((Key[3]>>9)&1);     
	rightkey[7]=((Key[0]>>(19+8))&1) ^ ((Key[3]>>28)&1);    
	rightkey[8]=((Key[0]>>(7+8))&1) ^ ((Key[3]>>16)&1);     
	rightkey[9]=((Key[0]>>(12+8))&1) ^ ((Key[3]>>21)&1);    
	rightkey[10]=((Key[0]>>(18+8))&1) ^ ((Key[2]>>25)&1);   
	rightkey[11]=((Key[1]>>25)&1) ^ ((Key[3]>>24)&1);   


	fprintf(f,"right key:");
	printf("right key:");

	for(i=0;i<12;i++)
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




	for(j=0;j<4;j++){
		FinalState[j]=0;
	}	
	for(j=0;j<12;j++){
		tempkey[j]=rightkey[j];
	}
		
	fprintf(f,"temp  key:");
	printf("temp  key:");
	for(j=0;j<12;j++)
	{
		if(tempkey[j])
		{
			fprintf(f,"1");
			printf("1");
		}
		else
		{
			fprintf(f,"1");
			printf("0");
		}
	}
	fprintf(f,"\n");
	printf("\n");
	//give the value for dynamic value
	temp=(InitialState[1]>>24)&1;
	temp1=(tempkey[0]&1) ^ ((InitialState[4]>>25)&1) ^ ((InitialState[9]>>25)&1) ^ ((InitialState[10]>>25)&1) ^ ((InitialState[11]>>24)&1) ^ ((InitialState[14]>>25)&1) ^ ((InitialState[16]>>24)&1) ^ ((InitialState[19]>>25)&1) ^ ((InitialState[21]>>24)&1) ^ ((InitialState[24]>>25)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<24;
	}

	temp=(InitialState[1]>>31)&1;
	temp1=(tempkey[1]&1) ^ ((InitialState[3]>>30)&1) ^ ((InitialState[8]>>30)&1) ^ ((InitialState[11]>>31)&1) ^ ((InitialState[13]>>30)&1) ^ ((InitialState[16]>>31)&1) ^ ((InitialState[22]>>31)&1) ^ ((InitialState[23]>>30)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<31;
	}

	temp=(InitialState[1]>>19)&1;
	temp1=(tempkey[2]&1) ^ ((InitialState[3]>>18)&1) ^ ((InitialState[7]>>19)&1) ^ ((InitialState[8]>>18)&1) ^ ((InitialState[11]>>19)&1) ^ ((InitialState[13]>>18)&1) ^ ((InitialState[16]>>19)&1) ^ ((InitialState[21]>>19)&1) ^ ((InitialState[23]>>18)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<19;
	}

	temp=(InitialState[5]>>16)&1;
	temp1=(tempkey[3]&1) ^ ((InitialState[10]>>16)&1) ^ ((InitialState[13]>>17)&1) ^ ((InitialState[15]>>16)&1) ^ ((InitialState[19]>>17)&1) ^ ((InitialState[20]>>16)&1) ^ ((InitialState[23]>>17)&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<16;
	}

	temp=(InitialState[5]>>13)&1;
	temp1=(tempkey[4]&1) ^ ((InitialState[10]>>13)&1) ^ ((InitialState[11]>>13)&1) ^ ((InitialState[15]>>13)&1) ^ ((InitialState[17]>>12)&1) ^ ((InitialState[20]>>13)&1) ^ ((InitialState[22]>>12)&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<13;
	}

	temp=(InitialState[1]>>20)&1;
	temp1=(tempkey[5]&1) ^ ((InitialState[4]>>21)&1) ^ ((InitialState[5]>>21)&1) ^ ((InitialState[9]>>21)&1) ^ ((InitialState[11]>>20)&1) ^ ((InitialState[14]>>21)&1) ^ ((InitialState[16]>>20)&1) ^ ((InitialState[19]>>21)&1) ^ ((InitialState[21]>>20)&1) ^ ((InitialState[24]>>21)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<20;
	}

	temp=(InitialState[1]>>10)&1;
	temp1=(tempkey[6]&1) ^ ((InitialState[3]>>9)&1) ^ ((InitialState[8]>>9)&1) ^ ((InitialState[11]>>10)&1) ^ ((InitialState[13]>>9)&1) ^ ((InitialState[16]>>10)&1) ^ ((InitialState[17]>>10)&1) ^ ((InitialState[21]>>10)&1) ^ ((InitialState[23]>>9)&1);
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<10;
	}

	temp=(InitialState[5]>>27)&1;
	temp1=(tempkey[7]&1) ^ ((InitialState[3]>>28)&1) ^ ((InitialState[4]>>28)&1) ^ ((InitialState[8]>>28)&1) ^ ((InitialState[10]>>27)&1) ^ ((InitialState[13]>>28)&1) ^ ((InitialState[15]>>27)&1) ^ ((InitialState[20]>>27)&1) ^ ((InitialState[23]>>28)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<27;
	}

	temp=(InitialState[5]>>15)&1;
	temp1=(tempkey[8]&1) ^ ((InitialState[3]>>16)&1) ^ ((InitialState[8]>>16)&1) ^ ((InitialState[13]>>16)&1) ^ ((InitialState[14]>>16)&1) ^ ((InitialState[20]>>15)&1) ^ ((InitialState[23]>>16)&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<15;
	}

	temp=(InitialState[5]>>20)&1;
	temp1=(tempkey[9]&1) ^ ((InitialState[10]>>20)&1) ^ ((InitialState[13]>>21)&1) ^ ((InitialState[14]>>21)&1) ^ ((InitialState[15]>>20)&1) ^ ((InitialState[20]>>20)&1) ^ ((InitialState[23]>>21)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<20;
	}

	temp=(InitialState[5]>>26)&1;
	temp1=(tempkey[10]&1) ^ ((InitialState[2]>>25)&1) ^ ((InitialState[7]>>25)&1) ^ ((InitialState[10]>>26)&1) ^ ((InitialState[15]>>26)&1) ^ ((InitialState[17]>>25)&1) ^ ((InitialState[20]>>26)&1) ^ ((InitialState[21]>>26)&1) ^ ((InitialState[22]>>25)&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT32)0x1<<26;
	}

	temp=(InitialState[1]>>25)&1;
	temp1=(tempkey[11]&1) ^ ((InitialState[2]>>25)&1) ^ ((InitialState[3]>>24)&1) ^ ((InitialState[8]>>24)&1) ^ ((InitialState[11]>>25)&1) ^ ((InitialState[13]>>24)&1) ^ ((InitialState[16]>>25)&1) ^ ((InitialState[21]>>25)&1) ^ ((InitialState[23]>>24)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[1]^=(UINT32)0x1<<25;
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
		for(k=0;k<6;k++){
			TempState[index0[0][k]]^=UINT32(temp1)<<index0[1][k];
		}
		for(k=1;k<32;k++)
		{
			temp1=(j>>k)&1;
			if(temp1){
				TempState[index[0][k-1]]|=UINT32(1)<<index[1][k-1];
				TempState[index[0][k-1]+5]|=UINT32(1)<<index[1][k-1];
			}
			else{
				TempState[index[0][k-1]]&=(~(UINT32(1)<<index[1][k-1]));
				TempState[index[0][k-1]+5]&=(~(UINT32(1)<<index[1][k-1]));
			}
					
		}
			
		KeccakPermutationOnWords((UINT32 *)TempState);
			
		for(k=0;k<4;k++){
			FinalState[k]^=TempState[6*k];
		}
	}
	fprintf(f,"\n");
	printf("\n");
	fprintf(f,"cube sum: %llx,\t%llx\n",FinalState[0],FinalState[1],FinalState[2],FinalState[3]);
	printf("cube sum: %llx,\t%llx\n",FinalState[0],FinalState[1],FinalState[2],FinalState[3]);
	if(FinalState[0]==0 && FinalState[1]==0 && FinalState[2]==0 && FinalState[3]==0)
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
