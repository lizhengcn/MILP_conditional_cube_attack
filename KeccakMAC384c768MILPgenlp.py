'''
KeccakMAC384c768MILPgenlp.py
Using SageMath, run this file and obtain file "lpKeccakMAC384c768.lp" as a model in Gurobi.
'''
from brial import *
import copy
import pdb
import xdrlib ,sys
import random
import time

Keccak=declare_ring([Block('k',128),Block('v',704),Block('c',1)],globals())

def theta(state):
	tempstate=[[] for i in range(25)]
	for i in range(25):
		for j in range(64):
			tempstate[i].append(state[i][j])
			for k in range(5):
				tempstate[i][j]+=state[(i%5+5-1)%5+5*k][j]+state[(i%5+1+5)%5+5*k][(j-1+64)%64]

	for i in range(25):
		for j in range(64):
			state[i][j]=tempstate[i][j]



def rio(state):
	rot=[0,1,62,28,27,36,44,6,55,20,3,10,43,25,39,41,45,15,21,8,18,2,61,56,14]
	tempstate=[[] for i in range(25)]
	for i in range(25):
		for j in range(64):
			tempstate[i].append(state[i][(j-rot[i]+64)%64])

	for i in range(25):
		for j in range(64):
			state[i][j]=tempstate[i][j]

def pi(state):
	tempstate=[[] for i in range(25)]
	for i in range(25):
		y=floor(i/5)
		x=i%5
		x1=y
		y1=(2*x+3*y)%5
		temp=5*y1+x1
		for j in range(64):
			tempstate[temp].append(state[i][j])
	for i in range(25):
		for j in range(64):
			state[i][j]=tempstate[i][j]

def chi(state):
	tempstate=[[] for i in range(25)]
	for i in range(5):
		for j in range(5):
			for k in range(64):
				tempstate[5*i+j].append(state[5*i+j][k]+(state[5*i+(j+1)%5][k]+1)*state[5*i+(j+2)%5][k])

	for i in range(25):
		for j in range(64):
			state[i][j]=tempstate[i][j]



state=[[] for i in range(25)]
state1=[[] for i in range(25)]
for i in range(25):
	for j in range(64):
		state1[i].append(0)


for i in range(2):
	for j in range(64):
		state[i].append(k(64*i+j))

for i in range(11):
	for j in range(64):
		state[i+2].append(v(64*i+j))

#print state[2][0],state[7][0]

for i in xrange(12):
	for j in xrange(64):
		state[i+13].append(0)


state[2][0]=c(0)
state[7][0]=c(0)

f=open('lpKeccakMAC384c768.lp','w')


f.write('Maximize\n')
for z in range(63):
	f.write('X0Y1Z'+str(z)+' + ')
	f.write('X0Y2Z'+str(z)+' + ')
	f.write('X1Y1Z'+str(z)+' + ')
	f.write('X1Y2Z'+str(z)+' + ')
	f.write('X2Y0Z'+str(z)+' + ')
	f.write('X2Y1Z'+str(z)+' + ')
	f.write('X2Y2Z'+str(z)+' + ')
	f.write('X3Y0Z'+str(z)+' + ')
	f.write('X3Y1Z'+str(z)+' + ')
	f.write('X4Y0Z'+str(z)+' + ')
	f.write('X4Y1Z'+str(z)+' + ')
f.write('X0Y1Z63 + X0Y2Z63 + X1Y1Z63 + X1Y2Z63 + X2Y0Z63 + X2Y1Z63 + X2Y2Z63 + X3Y0Z63 + X3Y1Z63 + X4Y0Z63 + X4Y1Z63')		


for x in range(5):
	for z in range(64):
		f.write(' - X'+str(x)+'Z'+str(z))
f.write('\n')



f.write('Subject to\n')


for z in range(64):
	#xz>=xyz
	f.write('X0Z'+str(z)+' - X0Y1Z'+str(z)+' >= 0\n')
	f.write('X0Z'+str(z)+' - X0Y2Z'+str(z)+' >= 0\n')
	f.write('X1Z'+str(z)+' - X1Y1Z'+str(z)+' >= 0\n')
	f.write('X1Z'+str(z)+' - X1Y2Z'+str(z)+' >= 0\n')
	f.write('X2Z'+str(z)+' - X2Y0Z'+str(z)+' >= 0\n')
	f.write('X2Z'+str(z)+' - X2Y1Z'+str(z)+' >= 0\n')
	f.write('X2Z'+str(z)+' - X2Y2Z'+str(z)+' >= 0\n')
	f.write('X3Z'+str(z)+' - X3Y0Z'+str(z)+' >= 0\n')
	f.write('X3Z'+str(z)+' - X3Y1Z'+str(z)+' >= 0\n')
	f.write('X4Z'+str(z)+' - X4Y0Z'+str(z)+' >= 0\n')
	f.write('X4Z'+str(z)+' - X4Y1Z'+str(z)+' >= 0\n')
	#sumxyz>=2xz		
	f.write('X0Y1Z'+str(z)+' + X0Y2Z'+str(z)+' - X0Z'+str(z)+' - X0Z'+str(z)+' >= 0\n')
	f.write('X1Y1Z'+str(z)+' + X1Y2Z'+str(z)+' - X1Z'+str(z)+' - X1Z'+str(z)+' >= 0\n')
	f.write('X2Y0Z'+str(z)+' + X2Y1Z'+str(z)+' + X2Y2Z'+str(z)+' - X2Z'+str(z)+' - X2Z'+str(z)+' >= 0\n')
	f.write('X3Y0Z'+str(z)+' + X3Y1Z'+str(z)+' - X3Z'+str(z)+' - X3Z'+str(z)+' >= 0\n')
	f.write('X4Y0Z'+str(z)+' + X4Y1Z'+str(z)+' - X4Z'+str(z)+' - X4Z'+str(z)+' >= 0\n')


rio(state)
pi(state)
'''
for i0 in range(25):
	for j0 in range(64):
		if((state[i0][j0]/Keccak(c(0)))!=0):
			if(i0%5==0):
				f.write('state['+str(i0+4)+']['+str(j0)+']=Keccak(1)\nstate['+str(i0+1)+']['+str(j0)+']=Keccak(0)\n')
			if(i0%5==4):
				f.write('state['+str(i0-1)+']['+str(j0)+']=Keccak(1)\nstate['+str(i0-4)+']['+str(j0)+']=Keccak(0)\n')
			if(i0%5==1 or i0%5==2 or i0%5==3):
				f.write('state['+str(i0-1)+']['+str(j0)+']=Keccak(1)\nstate['+str(i0+1)+']['+str(j0)+']=Keccak(0)\n')
'''
for i0 in range(25):
	for j0 in range(64):
		state1[i0][j0]=state[i0][j0]

chi(state1)

for i in range(704):
	for j in range(i+1,704,1):		
		for i0 in range(25):
			for j0 in range(64):
				if((state1[i0][j0]/Keccak(v(i)*v(j)))!=0):
					f.write('X'+str((i/64+2)%5)+'Y'+str((i/64+2)/5)+'Z'+str(i%64)+' + '+'X'+str((j/64+2)%5)+'Y'+str((j/64+2)/5)+'Z'+str(j%64)+' <= 1\n')
					break
					break



for i in range(704):		
	for i0 in range(25):
		for j0 in range(64):
			if((state1[i0][j0]/Keccak(c(0)*v(i)))!=0):
				f.write('X'+str((i/64+2)%5)+'Y'+str((i/64+2)/5)+'Z'+str(i%64)+' = 0\n')
				break
				break
state[10][6]=Keccak(1)
state[12][6]=Keccak(0)
state[24][62]=Keccak(1)
state[21][62]=Keccak(0)

chi(state)

theta(state)#
rio(state)
pi(state)
chi(state)	
	
for i in range(704):		
	for i0 in range(25):
		for j0 in range(64):
			if((state[i0][j0]/Keccak(c(0)*v(i)))!=0):
				f.write('X'+str((i/64+2)%5)+'Y'+str((i/64+2)/5)+'Z'+str(i%64)+' = 0\n')
				break
				break
			

f.write('Binary\n')
for z in range(64):
	f.write('X0Y1Z'+str(z)+'\n')
	f.write('X0Y2Z'+str(z)+'\n')
	f.write('X1Y1Z'+str(z)+'\n')
	f.write('X1Y2Z'+str(z)+'\n')
	f.write('X2Y0Z'+str(z)+'\n')
	f.write('X2Y1Z'+str(z)+'\n')
	f.write('X2Y2Z'+str(z)+'\n')
	f.write('X3Y0Z'+str(z)+'\n')
	f.write('X3Y1Z'+str(z)+'\n')
	f.write('X4Y0Z'+str(z)+'\n')
	f.write('X4Y1Z'+str(z)+'\n')


for x in range(5):
	for z in range(64):
		f.write('X'+str(x)+'Z'+str(z)+'\n')



f.close()


























