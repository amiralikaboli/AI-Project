import pandas as pd
import random as rand
import functools
import matplotlib.pyplot as plt

data=pd.read_csv('input.csv')
y=list(map(float,list(data.head())))
numberOfPoint=len(y)
populationSize=50
tornumentSize=2
numberOfGeneration=5000
mutationRate=0.1
sigmaVar=1


def MSE(indi):
    cnt=0
    for i in range(numberOfPoint):
        x=i/10
        cnt+=(y[i]-(indi[3]*x**3+indi[2]*x**2+indi[1]*x+indi[0]))**2
    return cnt/numberOfPoint

def fitness(indi):
    return 1/(1+MSE(indi))

def initialPopulation():
    pop=[None]*populationSize
    for i in range(populationSize):
        indi=(round(rand.uniform(-100,100),2),round(rand.uniform(-100,100),2),round(rand.uniform(-100,100),2),round(rand.uniform(-100,100),2))
        pop[i]=indi
    return pop

def mutation(child):
    indi=child
    for gen in indi:
        prob=rand.uniform(0,1)
        if prob<=mutationRate:
            gen+=rand.gauss(0,sigmaVar)
    return indi

def selectParent(initpop):
    ind1=rand.randint(0,populationSize-1)
    ind2=rand.randint(0,populationSize-1)
    while ind1==ind2:
        ind2=rand.randint(0,populationSize-1)
    par1=initpop[ind1]
    par2=initpop[ind2]
    if fitness(par1)>fitness(par2):
        return par1
    else:
        return par2

def generateNewPopulation(initpop):
    newpop=[None]*populationSize
    for i in range(populationSize):
        par1=selectParent(initpop)
        par2=selectParent(initpop)

        # g1=rand.randint(0,3)
        # g2=rand.randint(0,3)
        # while g1==g2:
        #     g2=rand.randint(0,3)
        # child=list(par1)
        # child[g1]=par2[g1]
        # child[g2]=par2[g2]
        # child=tuple(child)
        child=((par1[0]+par2[0])/2,(par1[1]+par2[1])/2,(par1[2]+par2[2])/2,(par1[3]+par2[3])/2)
        newpop[i]=mutation(child)
    return newpop

def populationCmp(indi1,indi2):
    return fitness(indi2)-fitness(indi1)


bst=[None]*numberOfGeneration
wrs=[None]*numberOfGeneration
avg=[None]*numberOfGeneration

pop=initialPopulation()
for t in range(numberOfGeneration):
    newpop=generateNewPopulation(pop)
    pop=pop+newpop
    pop=sorted(pop,key=functools.cmp_to_key(populationCmp))
    pop=pop[:populationSize]
    sumft=0
    for i in range(populationSize):
        sumft+=fitness(pop[i])
    bst[t]=fitness(pop[0])
    wrs[t]=fitness(pop[-1])
    avg[t]=sumft/populationSize

plt.plot(bst)
plt.plot(avg)
plt.plot(wrs)

# reg=pop[0]
# yr=[None]*numberOfPoint
# for i in range(numberOfPoint):
#     x=i/10
#     yr[i]=reg[3]*x**3+reg[2]*x**2+reg[1]*x+reg[0]
# print(reg)
# plt.plot(y)
# plt.plot(yr)
plt.show()