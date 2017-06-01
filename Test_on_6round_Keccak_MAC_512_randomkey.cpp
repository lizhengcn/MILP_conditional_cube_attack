/*
	Test_on_6round_Keccak_MAC_512_randomkey.cpp
	Test whether the cube sum with a random key guess(wrong key guess in common cases) is zero or not.
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
	UINT64 Key[2]={0};
	int tempkey[24]={0};



	UINT64 i,j,k,temp,temp1,counter,a;
	unsigned int rightkey[24]={0};

	unsigned int index0[2][2];
	index0[0][0]=2;index0[1][0]=0; 
    index0[0][1]=7;index0[1][1]=0;


	unsigned int index[2][100];

	index[0][0]=3;index[1][0]=56;  
	index[0][1]=2;index[1][1]=1;  
	index[0][2]=2;index[1][2]=8;  
	index[0][3]=2;index[1][3]=12; 
	index[0][4]=2;index[1][4]=23; 
	index[0][5]=2;index[1][5]=41; 
	index[0][6]=2;index[1][6]=43; 
	index[0][7]=2;index[1][7]=45; 
	index[0][8]=2;index[1][8]=50; 
	index[0][9]=2;index[1][9]=53;  
	index[0][10]=2;index[1][10]=62;  
	index[0][11]=3;index[1][11]=3; 
	index[0][12]=3;index[1][12]=4; 
	index[0][13]=3;index[1][13]=9; 
	index[0][14]=3;index[1][14]=12;  
	index[0][15]=3;index[1][15]=13;  
	index[0][16]=3;index[1][16]=14;  
	index[0][17]=3;index[1][17]=20;  
	index[0][18]=3;index[1][18]=23; 
	index[0][19]=3;index[1][19]=27; 
	index[0][20]=3;index[1][20]=33; 
	index[0][21]=3;index[1][21]=35; 
	index[0][22]=3;index[1][22]=39; 
	index[0][23]=3;index[1][23]=40; 
	index[0][24]=3;index[1][24]=46;

	index[0][25]=7;index[1][25]=56;  
	index[0][26]=4;index[1][26]=12; 
	index[0][27]=2;index[1][27]=56; 
	index[0][28]=5;index[1][28]=33; 
	index[0][29]=5;index[1][29]=57; 
	index[0][30]=4;index[1][30]=60; 

	


	FILE *f;
	f=fopen("cubesum.txt","w+b");
	srand(time(NULL));

	
	for(i=0;i<25;i++){
		InitialState[i]=0;
	}
	for(i=0;i<2;i++){
		for(j=0;j<64;j++){
			temp=random(2);
			if(temp)
			{
				Key[i]^=((UINT64)0x1<<j);
			}
		}
	}




	for(i=0;i<2;i++){
		InitialState[i]=Key[i];
	}
 


	 rightkey[0]=((Key[0]>>5)&1) ^ ((Key[1]>>5)&1);      
	 rightkey[1]=((Key[0]>>60)&1);    
	 rightkey[2]=((Key[0]>>19)&1);    
	 rightkey[3]=((Key[0]>>29)&1);    
	 rightkey[4]=((Key[0]>>21)&1);     
	 rightkey[5]=((Key[0]>>35)&1);      
	 rightkey[6]=((Key[0]>>34)&1);   
	 rightkey[7]=((Key[0]>>48)&1);    
	 rightkey[8]=((Key[0]>>40)&1);     
	 rightkey[9]=((Key[0]>>62)&1);
	 rightkey[10]=((Key[0]>>52)&1);
	 rightkey[11]=((Key[0]>>59)&1);
	 rightkey[12]=((Key[0]>>13)&1) ^ ((Key[1]>>12)&1)^ ((Key[1]>>34)&1); 
	 rightkey[13]=((Key[1]>>17)&1);
	 rightkey[14]=((Key[1]>>28)&1);
	 rightkey[15]=((Key[1]>>24)&1);
	 rightkey[16]=((Key[0]>>8)&1) ^ ((Key[1]>>7)&1); 
	 rightkey[17]=((Key[0]>>18)&1);
	 rightkey[18]=((Key[1]>>16)&1);
	 rightkey[19]=((Key[0]>>5)&1);
	 rightkey[20]=((Key[1]>>50)&1);
	 rightkey[21]=((Key[0]>>13)&1);
	 rightkey[22]=((Key[0]>>58)&1);
	 rightkey[23]=((Key[0]>>32)&1);




	fprintf(f,"right key:");
	printf("right key:");
	for(i=0;i<24;i++)
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
		
	for(j=0;j<12;j++){
		temp=random(2);
		if(temp)
		{
			tempkey[j]=1;
		}
		else
		{
			tempkey[j]=0;
		}
	}
		

	fprintf(f,"temp  key:");
	printf("temp  key:");

	for(j=0;j<24;j++)
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
	
	temp=(InitialState[4]>>44)&1;
	temp1=0;
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<44;
	}


	temp=(InitialState[2]>>4)&1;
	temp1=(tempkey[0]&1) ^ ((InitialState[5]>>5)&1) ^ ((InitialState[7]>>4)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<4;
	}


	temp=(InitialState[2]>>59)&1;
	temp1=(tempkey[1]&1) ^ ((InitialState[7]>>59)&1) ^ ((InitialState[5]>>60)&1) ^  1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<59;
	}


	temp=(InitialState[4]>>6)&1;
	temp1=((InitialState[2]>>7)&1) ^ ((InitialState[7]>>7)&1) ^ ((InitialState[8]>>7)&1);
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<6;
	}


	temp=(InitialState[2]>>46)&1;
	temp1=((InitialState[4]>>45)&1) ^ ((InitialState[7]>>46)&1);
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<46;
	}


	temp=(InitialState[4]>>3)&1;
	temp1=(tempkey[0]&1) ^ ((InitialState[5]>>5)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<3;
	}


	temp=(InitialState[2]>>31)&1;
	temp1=((InitialState[4]>>30)&1) ^ ((InitialState[7]>>31)&1);
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<31;
	}


	temp=(InitialState[5]>>19)&1;
	temp1=(tempkey[2]&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT64)0x1<<19;
	}


	temp=(InitialState[8]>>30)&1;
	temp1=(tempkey[3]&1) ^ ((InitialState[3]>>30)&1) ^ ((InitialState[5]>>29)&1);
	if(temp!=temp1)
	{
		InitialState[8]^=(UINT64)0x1<<30;
	}


	temp=(InitialState[8]>>22)&1;
	temp1=(tempkey[4]&1) ^ ((InitialState[3]>>22)&1) ^ ((InitialState[5]>>21)&1);
	if(temp!=temp1)
	{
		InitialState[8]^=(UINT64)0x1<<22;
	}


	temp=(InitialState[8]>>36)&1;
	temp1=(tempkey[5]&1) ^ ((InitialState[3]>>36)&1) ^ ((InitialState[5]>>35)&1);
	if(temp!=temp1)
	{
		InitialState[8]^=(UINT64)0x1<<36;
	}


	temp=(InitialState[5]>>34)&1;
	temp1=(tempkey[6]&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT64)0x1<<34;
	}


	temp=(InitialState[8]>>49)&1;
	temp1=(tempkey[7]&1) ^ ((InitialState[3]>>49)&1)^ ((InitialState[5]>>48)&1);
	if(temp!=temp1)
	{
		InitialState[8]^=(UINT64)0x1<<49;
	}


	temp=(InitialState[8]>>41)&1;
	temp1=(tempkey[8]&1) ^ ((InitialState[3]>>41)&1)^ ((InitialState[5]>>40)&1);
	if(temp!=temp1)
	{
		InitialState[8]^=(UINT64)0x1<<41;
	}


	temp=(InitialState[3]>>63)&1;
	temp1=(tempkey[9]&1) ^ ((InitialState[5]>>62)&1) ^ ((InitialState[8]>>63)&1);
	if(temp!=temp1)
	{
		InitialState[3]^=(UINT64)0x1<<63;
	}


	temp=(InitialState[2]>>51)&1;
	temp1=(tempkey[10]&1) ^ ((InitialState[5]>>52)&1) ^ ((InitialState[7]>>51)&1) ^ 1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<51;
	}


	temp=(InitialState[2]>>63)&1;
	temp1=((InitialState[4]>>62)&1) ^ ((InitialState[7]>>63)&1) ^ ((InitialState[8]>>63)&1);
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<63;
	}


	temp=(InitialState[7]>>58)&1;
	temp1=(tempkey[11]&1) ^ ((InitialState[2]>>58)&1) ^ ((InitialState[5]>>59)&1);
	if(temp!=temp1)
	{
		InitialState[7]^=(UINT64)0x1<<58;
	}
	
	temp=(InitialState[4]>>13)&1;
	temp1=(tempkey[12]&1) ^ ((InitialState[6]>>12)&1) ^ ((InitialState[6]>>34)&1)^1;
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<13;
	}
	
	temp=(InitialState[2]>>26)&1;
	temp1=((InitialState[3]>>26)&1) ^ ((InitialState[4]>>25)&1)^((InitialState[7]>>26)&1)^1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<26;
	}
	
	temp=(InitialState[3]>>16)&1;
	temp1=(tempkey[13]&1) ^ ((InitialState[6]>>17)&1) ^ ((InitialState[8]>>16)&1);
	if(temp!=temp1)
	{
		InitialState[3]^=(UINT64)0x1<<16;
	}
	
	temp=(InitialState[6]>>28)&1;
	temp1=(tempkey[14]&1);
	if(temp!=temp1)
	{
		InitialState[6]^=(UINT64)0x1<<28;
	}
		
	temp=(InitialState[5]>>51)&1;
	temp1=0;
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT64)0x1<<51;
	}
	
	temp=(InitialState[6]>>24)&1;
	temp1=(tempkey[15]&1) ^ ((InitialState[4]>>25)&1)^((InitialState[5]>>25)&1)^1;
	if(temp!=temp1)
	{
		InitialState[6]^=(UINT64)0x1<<24;
	}
	
	temp=(InitialState[2]>>18)&1;
	temp1=((InitialState[7]>>18)&1)^1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<18;
	}


	temp=(InitialState[4]>>8)&1;
	temp1=(tempkey[16]&1) ^ ((InitialState[6]>>7)&1);
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<8;
	}


	temp=(InitialState[2]>>42)&1;
	temp1=((InitialState[4]>>41)&1) ^ ((InitialState[7]>>42)&1) ^ ((InitialState[8]>>42)&1)^1;
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<42;
	}


	temp=(InitialState[4]>>40)&1;
	temp1=1;
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<40;
	}


	temp=(InitialState[2]>>17)&1;
	temp1=(tempkey[17]&1) ^ ((InitialState[5]>>18)&1) ^ ((InitialState[7]>>17)&1);
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<17;
	}


	temp=(InitialState[3]>>15)&1;
	temp1=(tempkey[18]&1) ^ ((InitialState[6]>>16)&1) ^ ((InitialState[7]>>16)&1) ^ ((InitialState[8]>>15)&1)^1;
	if(temp!=temp1)
	{
		InitialState[3]^=(UINT64)0x1<<15;
	}




	temp=(InitialState[3]>>6)&1;
	temp1=(tempkey[19]&1) ^ ((InitialState[5]>>5)&1) ^ ((InitialState[8]>>6)&1)^1;
	if(temp!=temp1)
	{
		InitialState[3]^=(UINT64)0x1<<6;
	}


	temp=(InitialState[4]>>51)&1;
	temp1=(tempkey[20]&1) ^ ((InitialState[6]>>50)&1);
	if(temp!=temp1)
	{
		InitialState[4]^=(UINT64)0x1<<51;
	}


	temp=(InitialState[2]>>33)&1;
	temp1= ((InitialState[7]>>33)&1);
	if(temp!=temp1)
	{
		InitialState[2]^=(UINT64)0x1<<33;
	}


	temp=(InitialState[5]>>13)&1;
	temp1=(tempkey[21]&1)^1;
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT64)0x1<<13;
	}




	temp=(InitialState[3]>>59)&1;
	temp1=(tempkey[22]&1) ^ ((InitialState[5]>>58)&1) ^ ((InitialState[8]>>59)&1)^1; 
	if(temp!=temp1)
	{
		InitialState[3]^=(UINT64)0x1<<59;
	}




	temp=(InitialState[5]>>32)&1;
	temp1=(tempkey[23]&1) ^ ((InitialState[4]>>30)&1) ^ ((InitialState[6]>>32)&1);
	if(temp!=temp1)
	{
		InitialState[5]^=(UINT64)0x1<<32;
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
		for(k=1;k<26;k++)
		{
			temp1=(j>>k)&1;
			if(temp1){
				TempState[index[0][k-1]]|=UINT64(1)<<index[1][k-1];
				TempState[index[0][k-1]+5]|=UINT64(1)<<index[1][k-1];
			}
			else{
				TempState[index[0][k-1]]&=(~(UINT64(1)<<index[1][k-1]));
				TempState[index[0][k-1]+5]&=(~(UINT64(1)<<index[1][k-1]));
			}
					
		}
		for(k=26;k<32;k++)
		{
			temp1=(j>>k)&1;
			if(temp1){
				TempState[index[0][k-1]]|=UINT64(1)<<index[1][k-1];
			}
			else{
				TempState[index[0][k-1]]&=(~(UINT64(1)<<index[1][k-1]));
			}	
		}
			
			
		KeccakPermutationOnWords((UINT64 *)TempState);
			
		for(k=0;k<2;k++){
			FinalState[k]^=TempState[k];
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
