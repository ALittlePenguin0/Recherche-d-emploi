import math
import numpy as np
import random
import matplotlib.pyplot as plt

def calculdistance(alea,matriceDist,dictionnaire,binaire):
    
    #B est une matrice qui contient les (alea-1) distances
    B = [None] * (len(alea)-1)
    lenB=len(B)
    j=0
    
    A=matriceDist
    D=dictionnaire
    
    # Avec B, je souhaite calculer la distance totale à l'aide d'un dictionnaire et de la matrice des distances
    
    # Pour remplir le tableau B, je me sers du dictionnaire et de la matrice des distances
    while(j<lenB):
        B[j]=A[D[alea[j]]][D[alea[j+1]]] 
        if(binaire==1):
            print("De",alea[j],"à",alea[j+1],"il y a",B[j],"km de distance.")
        j+=1
        
    # Somme de tous les éléments de B
    distTotal=0
    for i in B:
        distTotal+=i
        
    #print("La distance totale est",distTotal,".")
    return distTotal


def recuitsimule(nbGenerations,temp,taux):

    # Déclaration des villes, on utilisera villes2 pour la suite
    villes=['Lille','Paris','Rennes','Brest','Rouen','Caen','Lyon','Marseille',\
'Toulouse','Bordeaux','Amiens','Nantes','Lille']

    villes2=['Paris','Rennes','Brest','Rouen','Caen','Lyon','Marseille',\
'Toulouse','Bordeaux','Amiens','Nantes'] #villes sans Lille
    
    # listeAlea est une permutation représentant un ordre commençant par Lille et se termine par Lille
    
    # listeAlea est un tableau de len(villes2) + 2 éléments (pour Lille)
    listeAlea = [None] * (len(villes2)+2) 
    lenght=len(listeAlea)
    i=1

    listeAlea[0]='Lille'
    listeAlea[12]='Lille'

    # Création de listeAlea en choisissant une ville parmi villes2 à chaque boucle, une ville ne doit être utilisée qu'une seule fois
    while (i<lenght-1):
        r=random.choice(villes2)
        listeAlea[i]=r
        villes2.remove(r)
        i+=1

    # matDistances est la matrice des distances et Dict est le dictionnaire
    matDistances = np.array([\
    [0,220,570,760,240,370,690,830,890,800,140,600],\
    [220,0,350,590,120,235,390,660,680,580,150,380],\
    [570,350,0,240,310,180,740,1040,700,470,430,110],\
    [760,590,240,0,500,372,970,1280,880,650,620,300],\
    [240,120,310,500,0,130,600,900,780,650,115,390],\
    [370,235,180,372,130,0,700,1000,840,600,250,290],\
    [690,390,740,970,600,700,0,310,535,550,620,680],\
    [830,660,1040,1280,900,1000,310,0,400,650,930,990],\
    [890,680,700,880,780,840,535,400,0,240,830,590],\
    [800,580,470,650,650,600,550,650,240,0,740,350],\
    [140,150,430,620,115,250,620,930,830,740,0,500],\
    [600,380,110,300,390,290,680,990,590,350,500,0]])
    
    Dict = {'Lille':0, 'Paris':1, 'Rennes':2, 'Brest':3,\
        'Rouen':4,'Caen':5,'Lyon':6,'Marseille':7,\
       'Toulouse':8, 'Bordeaux':9,'Amiens':10,'Nantes':11} #Dictionnaire qui ne change pas 
    
    # DistTotal est la somme des distances d'une permutation
    DistTotal = (calculdistance(listeAlea,matDistances,Dict,0))
    
    # Dans un premier temps, la meilleure distance est la distance totale initiale
    # de même, le meilleur parcours est le parcours initial
    MeilleureDist = DistTotal
    
    MeilleurParcours = [None] * (len(listeAlea)) 
    for x in range(0,len(listeAlea)):
        MeilleurParcours[x]=listeAlea[x]
    
    # TablDistances est un tableau qui contient toutes les distances totales des permutations
    # TablMeilleuresDist est un tableau qui contient les meilleurs distances
    TablDistances = [None] * (nbGenerations)
    TablMeilleuresDist = [None] * (nbGenerations)
    
    # Remplissage du premier élément
    TablDistances[0] = DistTotal
    TablMeilleuresDist[0] = MeilleureDist
    
    # On crée la variable intermédiaire copielisteAlea pour la suite
    copielisteAlea = [None] * (len(listeAlea)) 
    for x in range(0,len(listeAlea)):
        copielisteAlea[x]=listeAlea[x]
    
    # déclaration de listeAlea2 pour comparer avec listeAlea
    listeAlea2 = [None] * (len(listeAlea)) 
    
    # le tableau de listeAlea2 est identique à copielisteAlea dans un premier temps
    for x in range(0,len(listeAlea)):
        listeAlea2[x]=copielisteAlea[x]
    
    # Maintenant, on souhaite générer (nbGenerations-1) permutations pour les comparer
    # Chaque permutation j+1 provient de la permutation j
    
    j=0
    while(j<nbGenerations-1):
    # on génère deux entiers u et v tels que 1<=u<v<=len(listeAlea)-2
        v = random.randint(2,len(listeAlea)-2) 
        u = random.randint(1,v-1)
        w=v-u+1
        
    # à partir des entiers u et v qui représentent des indices, on inverse l'ordre à partir de u jusqu'à v
        for x in range(0,w):
            listeAlea2[u]=copielisteAlea[v]
            u+=1
            v-=1
    # copielisteAlea prend les valeurs de listeAlea2 pour créer une nouvelle permutation à partir de listeAlea2
        for x in range(0,len(listeAlea)):
            copielisteAlea[x]=listeAlea2[x]
        
    # création d'une nouvelle variable qui contient la nouvelle distance totale
        DistTotal2 = (calculdistance(listeAlea2,matDistances,Dict,0))
        
    # Attribuer la nouvelle distance totale au tableau des distances totales
    # Attribuer la meilleure distance au tableau des meilleurs distances
        TablDistances[j+1] = DistTotal2
        TablMeilleuresDist[j+1] = MeilleureDist
        
        if (DistTotal2 < DistTotal): # le cas où la nouvelle distance totale est inférieure à la distance totale initiale
            DistTotal = DistTotal2 # alors remplacer DistTotal avec probablité 1
            if(MeilleureDist > DistTotal): # si on trouve une meilleure distance, alors on la remplace 
                MeilleureDist = DistTotal
                for x in range(0,len(listeAlea)): # si on trouve un meilleur parcours, alors on le remplace 
                    MeilleurParcours[x]=listeAlea2[x]
                  
        if (DistTotal2 > DistTotal): # le cas où la nouvelle distance totale est supérieure 
            pui = (DistTotal-DistTotal2)/(temp) # calcul de la puissance
            if ((random.uniform(0,1)<(math.exp(-pui)))): # générer un aléa entre 0 et 1 et vérification
                DistTotal = DistTotal2 # si condition vérifiée alors distance totale prend la nouvelle valeur
        temp=temp*(1-taux) #changement de température en fonction du taux
        j+=1 # pour la boucle while
    
    
    # ----------------------------------------------------------
    # affichage
    
    print(MeilleurParcours)
    (calculdistance(MeilleurParcours,matDistances,Dict,1))
    
    # TablGenerations est de type [1,2,3,..., nbGenerations]
    TablGenerations = [None] * (nbGenerations)
    for x in range(1,nbGenerations+1):
        TablGenerations[x-1]=x

    plt.title("Evolution de la distance totale en fonction des générations")
    plt.plot(TablGenerations, TablDistances,'ro', label= "Les distances totales sous forme de points")
    plt.plot(TablGenerations, TablMeilleuresDist,'k',label="Les meilleurs distances totales sous forme de courbe")
    plt.axis([0, nbGenerations, 3800, 9000])
    plt.ylabel("Distance",fontsize=13)
    plt.xlabel("Generation",fontsize=13)
    plt.legend(bbox_to_anchor=(1.05, 1), loc='upper left', borderaxespad=0.)
    plt.show()
    print("La meilleure distance est ", MeilleureDist)

recuitsimule(500,1000,0.01)
