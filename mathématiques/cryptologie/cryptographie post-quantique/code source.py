# PREMIERE PARTIE : LWE

import random
from sage.stats.distributions.discrete_gaussian_integer import DiscreteGaussianDistributionIntegerSampler

def approx(liste,q):
    M=[]
    for x in liste:
        if(0<=x<=int(q/4)):
            M.append(0)
        elif(int(q/4)<x<=int(q/2)):
            M.append(1)
        elif(int(q/2)<x<=int(3*q/4)):
            M.append(1)
        else:
            M.append(0)
    return M

def generationdeclefs(k,q):
    A=random_matrix(GF(q),k,k)
    D=DiscreteGaussianDistributionIntegerSampler(sigma=sigma)
    s=[D() for x in range(k)]
    s=matrix(k,1,s)
    e=[D() for x in range(k)]
    e=matrix(k,1,e)
    
    b=A*s+e
    
    pk=(A,b)
    sk=s
    print("La clé publique est : ",pk)
    print("La clé privée est : ",sk)
    return (pk,sk)

def chiffrement(pk,m,q,k):
    D=DiscreteGaussianDistributionIntegerSampler(sigma=sigma)
    r=[D() for x in range(k)]
    r=matrix(1,k,r)
    e1=[D() for x in range(k)]
    e1=matrix(1,k,e1)
    e2=[D() for x in range(1)]
    e2=matrix(1,1,e2)

    u=r*pk[0]+e1
    v=[(r*pk[1]+e2)+(int(q/2)*i)for i in m]
    return (u,v)

def dechiffrement(sk,c,q):
    uu=c[0]*sk
    m=[i-uu for i in c[1]]
    m=approx(m,q)
    return m
    
#*************************************************
#*                 AFFICHAGE                     *
#*************************************************

# Déclaration de l'écart-type, du modulo et d'une petite dimension 
sigma=3
q=3329
k=3

# Appel des algorithmes
clefs=generationdeclefs(k,q)
pk=clefs[0]
sk=clefs[1]
# le clair est tiré uniformément dans {0,1}^{256}
m=[ZZ.random_element(0,2) for i in range(256)] 
chiffre=chiffrement(pk,m,q,k)
mm=dechiffrement(sk,chiffre,q)

print("Vérification du schéma de chiffrement :",m==mm)

# DEUXIEME PARTIE : RING-LWE

import random
from sage.stats.distributions.discrete_gaussian_integer import DiscreteGaussianDistributionIntegerSampler

def approx(liste,q):
    M=[]
    for x in liste:
        if(0<=x<=int(q/4)):
            M.append(0)
        elif(int(q/4)<x<=int(q/2)):
            M.append(1)
        elif(int(q/2)<x<=int(3*q/4)):
            M.append(1)
        else:
            M.append(0)
    return M

def trpolynome(list_a,n,q):
    Z=ZZ.quotient(q) 
    P=PolynomialRing(Z,"a")
    a=P.gen()
    I=P.ideal([a**n+1])
    S=P.quotient_ring(I,"x")
    x=S.gen()
    tmp=0 
    j=0
    for i in list_a: 
        tmp = tmp + (i * (x**(j))) 
        j += 1 
    return tmp

def generationdeclefs(n,q):
    R=PolynomialRing(GF(q),"a") 
    a=R.gen()
    I=R.ideal([a^n+1])
    S=R.quotient_ring(I,"x")
    x=S.gen()
    M=MatrixSpace(S,1,1)

    A=M.random_element()
    D=DiscreteGaussianDistributionIntegerSampler(sigma=sigma)
    s=[D() for x in range(n)]
    s=matrix(1,1,trpolynome(s,n,q))
    e=[D() for x in range(n)]
    e=matrix(1,1,trpolynome(e,n,q))

    b=A*s+e

    pk=(A,b)
    sk=s
    print("La clé publique est : ",pk)
    print("La clé privée est : ",sk)
    return (pk,sk)

def chiffrement(pk,m,n,q):
    D=DiscreteGaussianDistributionIntegerSampler(sigma=sigma)
    r=[D() for x in range(n)]
    r=matrix(1,1,trpolynome(r,n,q))
    e1=[D() for x in range(n)]
    e1=matrix(1,1,trpolynome(e1,n,q))
    e2=[D() for x in range(n)]
    e2=matrix(1,1,trpolynome(e2,n,q))

    u=pk[0]*r+e1
    X=(pk[1]*r+e2)
    Y=(int(q/2)*m)
    v=X+Y
    return (u,v)


def dechiffrement(sk,n,c,q):
    m=c[1]-(sk*c[0])
    coeff=list(m[0][0])
    m=approx(coeff,q)
    m=trpolynome(m,n,q)
    return m
    
#*************************************************
#*                 AFFICHAGE                     *
#*************************************************

# Déclaration de l'écart-type, de la puissance de 2 et du modulo  
sigma=3
n=16
q=3329

# Appel des algorithmes
clefs=generationdeclefs(n,q)
pk=clefs[0]
sk=clefs[1]

# le clair est tiré uniformément dans l'anneau des polynômes Z_2/(X^16+1)
m=[randint(0, 1) for i in range(n)]
m=trpolynome(m,n,q)

chiffre=chiffrement(pk,m,n,q)
mm=dechiffrement(sk,n,chiffre,q)

print("Vérification du schéma de chiffrement :",m==mm)
