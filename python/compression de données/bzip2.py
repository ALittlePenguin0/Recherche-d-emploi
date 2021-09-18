#ATTENTION : ces algorithmes ne fonctionennt qu'avec un alphabet avec un seul caractère pour chaque élément

# fonctions utiles ----------------------------------------
def rotate_str(x,i): #décaler de i rang vers la gauche
    return x[i:] + x[:i] #supprime les éléments de rang inférieur à i et concatène les élements supprimés 

def movefirst(T,i):
    T.insert(0,T[i]) #copie de l'élément i au rang 0
    T.pop(i+1) #enlève l'élément i qui se retrouve au rang i+1
    return T

# fonctions principales ------------------------------------------
def BW(A,x): #fonction Burrows-Wheeler
    Tab = []
    SortedTab = []
    y = ''
    
    for i in range (0,len(x)): #le tableau Tab se complète avec les rotations de la chaîne de caractères 
        Tab.append(rotate_str(x,i))
    SortedTab = sorted(Tab)
    
    for i in range (0,len(x)): #y prend le dernier caractère de chaque élement de SortedTab
        y = y + SortedTab[i][len(x)-1]
        
    for i in range (0,len(x)): #n correspond au rang du mot initial dans SortedTab
        if SortedTab[i] == x:
            n = i
    return (n,y)

    
def MTF(A,x): #fonction move-to-front 
    Tab = list(A)
    y = []
    
    for i in range (0,len(x)): #chaque caractère se transforme en nombre. Ce nombre correspond au rang par rapport à l'alphabet où on applique movefirst à chaque étape  
        for j in range (0,len(Tab)):
            if Tab[j] == x[i]:
                y.append(j)
                Tab = movefirst(Tab,j) 
    return y


#Les fonctions RLE et invRLE n'affichent aucun 1 sauf éventuellement pour le dernier élement pour un gain en taille

def RLE(A,x): #fonction run-length encoding
    y = ''
    cpt = 1
    
    for i in range(0,len(x)): #chaque nombre se transforme en lettre qui correspond à leur rang dans l'alphabet
        x[i] = (A[x[i]]) 
    
    lettre = x[0]  #on prend en mémoire le premier élément pour le comparer
    for i in range (1,len(x)): #on cherche le nombre de lettres répétitifs à la suite en faisant une comparaison de chaque élément
        if lettre == x[i]:
            cpt = cpt + 1
        else:
            if cpt == 1 : #si le compteur est 1 c'est-à-dire qu'il n'y a pas de répétition alors on ajoute à y que la lettre
                y = (y + lettre)
                cpt = 1
                lettre = x[i]
            else : #si le compteur est différent de 1 alors on ajoute à y le nombre de répétition et la lettre associée
                y = (y + str(cpt) + lettre)
                cpt = 1
                lettre = x[i]
    y = (y + str(cpt) + lettre) #représente le dernier élement une fois les boucles faites 
    return y

# fonctions inverses -----------------------------
def invRLE(A,y): #fonction inverse de run-length encoding 
    x = ''
    cpt = ''
    
    for i in range (0,len(y)): #si l'élément est un chiffre alors cpt prend le chiffre en tant que caractère
        if (y[i].isnumeric()) :
            cpt = cpt + str(y[i])
        else :
            if cpt == '' : #si cpt est vide c'est-à-dire que la lettre n'a pas de répétition alors on ajoute la lettre à x
                x = x + y[i]
            else : #si le cpt est non-vide alors on ajoute à x : (la lettre fois cpt)
                x = x + (int(cpt) * y[i])
                cpt = ''
                
    x = list(x)
    for i in range(0,len(x)) : # on cherche à transformer les lettres par leurs rangs dans l'alphabet 
        for j in range(0,len(A)) :
            if x[i] == A[j] :
                x[i] = int(j)
    return x


def invMTF(A,y): #fonction inverse de move-to-front
    Tab = list(A)
    x = ''
    
    for i in range (0,len(y)): #on ajoute à x le rang de chaque nombre par rapport à l'alphabet puis l'alphabet s'applique movefirst
        valeur = y[i]
        x = x + Tab[valeur]
        Tab = movefirst(Tab,valeur)
    return x



def invBW(n,y): #fonction inverse de Burrows-Wheeler, attention, n est le rang du message initial dans l'algorithme BW
    x = ''
    Tab = []
    
    for i in range(0,len(y)): #Tab prend toutes les valeurs de y et on applique sorted 
        Tab.append(y[i])
    Tab = sorted(Tab)

    for i in range(1,len(y)):
        for j in range(0,len(y)):
            Tab[j] = y[j] + Tab[j]
        Tab = sorted(Tab)
    x = Tab[n] #on retrouve x à l'aide de n
    return x


#fonctions compress, uncompress, et comparaison--------------------------------
def compress(A,x): 
    P = (BW(A,x))[0] #P est le rang du message initial qui est nécessaire pour l'inverse de BW
    res = RLE(A,MTF(A,(BW(A,x))[1])) #RLE prend en entrée l'alphabet A et le message x passé d'abord par la fonction BW puis MTF, faire attention à quel élément appeler d'où BW(A,x)[1]
    return (res,P)
         
def uncompress(A,res):
    inv = invBW((compress(A,X))[1],invMTF(A,invRLE(A,(compress(A,X))[0])))
    return inv

def comparaison(initial,compression):
    if len(initial) >= len(compression):
        print ('la taille initiale du message est',len(initial),'caractères;','\nla taille après compression est', len(compression),'caractères;','\nsoit un gain de', len(initial) - len(compression), 'caractère(s).')
    else: 
        print ('la taille initiale du message est',len(initial),'caractères;','\nla taille après compression est', len(compression),'caractères;')
        print ("la compression n'est pas efficace car il y a", len(compression)- len(initial), "caractère(s) qui se sont ajoutés.\n")            

