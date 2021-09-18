#include <stdio.h>
#include <stdlib.h>
typedef unsigned char BYTE; 

// Projet "Implémentation d'un DES et cryptanalyse différentielle"
// dans le cadre du cours "Programmation pour la cryptographie" de S. Mesnager
// réalisé par Aldjia Moumou, Aurore Reboux et Michel Nguyen.
// Mai 2021.


// ***************************************** PROJET ************************************************************** //
// ******************** Implémentation d'un DES et cryptanalyse différentielle ************************************************************** //


// Déclaration de prototypes pour la suite
void pack8(BYTE *unpacked, BYTE *binary);
void unpack8(BYTE *packed, BYTE *binary);



// Implémentation de l'algorithme setkey (génération de clés)
// L'algorithme prend en entrée la clé principale ainsi qu'un tableau qui contiendra les 16 sous-clés générées.
void setkey(BYTE *pkey, BYTE  KS[16][48])
{
    // déclaration des variables à utiliser
    int i, j, k, t1, t2;

    // déclaration de PC1
    static BYTE PC1[64] = {57, 49, 41, 33, 25, 17, 9,
                     1, 58, 50, 42, 34, 26, 18,
                     10, 2, 59, 51, 43, 35, 27,
                     19, 11, 3, 60, 52, 44, 36,
                     63, 55, 47, 39, 31, 23, 15,
                     7, 62, 54, 46, 38, 30, 22,
                     14, 6, 61, 53, 45, 37, 29,
                     21, 13, 5, 28, 20, 12, 4};

    // déclaration de PC2
    static BYTE PC2[56] = {14, 17, 11, 24, 1, 5,
                         3, 28, 15, 6, 21, 10,
                         23, 19, 12, 4, 26, 8,
                         16, 7, 27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55,
                         30, 40, 51, 45, 33, 48,
                         44, 49, 39, 56, 34, 53,
                         46, 42, 50, 36, 29, 32};

    // déclaration de shifts
    unsigned short shifts[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

    // transformation du tableau pkey de 8 octets en tableau key de 64 bits
    static BYTE key[64];
    unpack8(pkey,key);

    // permutation par PC1 de key
    static BYTE CD[56];
    for (i=0; i<56; i++) CD[i] = key[PC1[i]-1];

    // considérer une partie gauche et droite de 32 bits chacun
    // appliquer un ou deux décalages à gauche selon le nombre de tour
    for (i=0; i<16; i++) {
        for (j=0; j<shifts[i]; j++) {
            t1 = CD[0];
            t2 = CD[28];
        for (k=0; k<27; k++) {
            CD[k] = CD[k+1];
            CD[k+28] = CD[k+29];
        }
        CD[27] = t1;
        CD[55] = t2;}

    // permutation par PC2 de CD puis remplissage du tableau des sous-clés
        for (k=0; k<48; k++) KS[i][k] = CD[PC2[k]-1];
    }
}



// Implémentation d'un algorithme pour visualiser setkey
void visualisationSetKey(BYTE *key){

    BYTE KS[16][48];
    BYTE A[48];
    BYTE B[6];

    setkey(key,KS);
    int i,j,w;

    printf("----------------VISUALISATION DE SETKEY-----------------\n\n");
    printf("La cle principale est :\n");
    for(i=0;i<8;i++)
        printf("%x ",key[i]);

    printf("\n");
    printf("\n");
    printf("Les sous cles sont : \n");

    for(i=0;i<16;i++){
        printf("tour %d : ",i+1);
        for(j=0;j<48;j++)
            A[j]=KS[i][j];
        pack8(A,B);

    for(w=0;w<6;w++)
        printf("%x ",B[w]);
     printf("\n");
    }
    printf("\n");
}





// ***************************************** EXERCICE 1 ************************************************************** //
// ***************************************** question 1 ************************************************************** //

// Implémentation d'un algorithme pour la génération de clés dans le sens inverse
// L'algorithme prend en entrée le nombre de tours, la clé principale et le tableau qui contiendra les sous-clés
void reverse_key_schedule(BYTE r, BYTE *pkey, BYTE reverseKS[16][48]){

    // Utilisation de setkey
    BYTE KS[16][48];
    setkey(pkey,KS);

    // déclaration des variables
    int i,j,w;

    // attribution des sous-clés en inversant l'ordre
    for(i=0;i<r;i++){
        for(j=0;j<48;j++)
            reverseKS[i][j]=KS[r-i-1][j];
    }

    // affichage inspiré de l'algorithme visualisationSetKey
    BYTE A[48];
    BYTE B[6];

    printf("----------------VISUALISATION DE REVERSE_KEY_SCHEDULE-----------------\n\n");
    printf("La cle principale est :\n");
    for(i=0;i<8;i++)
        printf("%x ",pkey[i]);

    printf("\n");
    printf("\n");
    printf("Les sous cles sont : \n");

    for(i=0;i<r;i++){
        printf("tour %d : ",r-i);
        for(j=0;j<48;j++)
            A[j]=reverseKS[i][j];
        pack8(A,B);

    for(w=0;w<6;w++)
        printf("%x ",B[w]);
     printf("\n");
    }
    printf("\n");
}






// ***************************************** question 2 ************************************************************** //

// Implémentation de l'algorithme indice
// prend en entrée le nombre de tours et le tableau indice de 48 cases
void indice(BYTE rounds, BYTE *ind){

    // si le nombre de tours n'est pas compris entre 1 et 16 inclus alors sortie du programme
    if((rounds<=0) || (16<rounds)){
        printf("Mettez un nombre de tours compris entre 1 et 16.\nFIN DU PROGRAMME.");
        exit(EXIT_FAILURE);
    }

    // déclaration de PC1
    static BYTE PC1[64] = {57, 49, 41, 33, 25, 17, 9,
                     1, 58, 50, 42, 34, 26, 18,
                     10, 2, 59, 51, 43, 35, 27,
                     19, 11, 3, 60, 52, 44, 36,
                     63, 55, 47, 39, 31, 23, 15,
                     7, 62, 54, 46, 38, 30, 22,
                     14, 6, 61, 53, 45, 37, 29,
                     21, 13, 5, 28, 20, 12, 4};

    // déclaration de PC2
    static BYTE PC2[56] = {14, 17, 11, 24, 1, 5,
                         3, 28, 15, 6, 21, 10,
                         23, 19, 12, 4, 26, 8,
                         16, 7, 27, 20, 13, 2,
                         41, 52, 31, 37, 47, 55,
                         30, 40, 51, 45, 33, 48,
                         44, 49, 39, 56, 34, 53,
                         46, 42, 50, 36, 29, 32};

    // déclaration de shifts
    unsigned short shifts[] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};

    // déclaration des variables
    BYTE tabDeRef[64];
    BYTE CD[56];
    int i,j,k,c,t1,t2;

    // nous utilisons un tableau de référence où la valeur d'une case représente son indice+1
    // l'idée est la suivante : nous allons suivre le cheminement de setkey en utilisant comme clé principale tabDeRef
    // à la fin de la manipulation, nous observons les valeurs que contiendra le tableau ind
    // à l'aide d'un algorithme de tri pour plus de visibilité, nous pourrons voir ainsi les indices des bits utilisés à tel tour

    // attribution des valeurs de tabDeRef
    for(i=0;i<64;i++)
        tabDeRef[i]=i+1;

    // application de PC1 à tabDeRef
    for(i=0;i<56;i++)
        CD[i] = tabDeRef[PC1[i]];

    // boucle de "rounds" tours
    // même procédure que dans setkey
    for (i=0; i<rounds; i++) {
        for (j=0; j<shifts[i]; j++) {
            t1 = CD[0];
            t2 = CD[28];
        for (k=0; k<27; k++) {
            CD[k] = CD[k+1];
            CD[k+28] = CD[k+29];
        }
        CD[27] = t1;
        CD[55] = t2;}
    }

    // application de PC2
    // seuls les valeurs entre 1 et 64 compris nous intéresse
    for(i=0;i<48;i++){
            if(( 1<= CD[PC2[i]]) && (CD[PC2[i]] <= 64))
                    ind[i] = CD[PC2[i]];
    }

    // algorithme de tri par sélection
    for(i=0;i<48-1;i++)
        for(j=i+1;j<48;j++)
            if ( ind[i] > ind[j] ) {
                c = ind[i];
                ind[i] = ind[j];
                ind[j] = c;
        }


    // affichage
    printf("----------------VISUALISATION DES INDICES-----------------\n");
    printf("------------------------AU TOUR %d-----------------\n\n", rounds);
    printf("Les bits utilises sont :\n");
    for(i=0;i<48;i++){
        if ((1<=ind[i]) && (ind[i]<=64))    // seuls les valeurs entre 1 et 64 compris nous intéresse
            printf("%d ",ind[i]);
    }
    printf("\n\n");
}






// ***************************************** question 3 ************************************************************** //

// Implémentation de l'algorithme bits de parité
// prend en entrée le tableau des indices et le tableau des bits de parité
void bitdeparite(BYTE *ind){

    // déclaration des variables
    int i;
    int t=1;
    int booleen=0;

    printf("----------------VISUALISATION DES BITS DE PARITE-----------------\n\n");

    // boucles de la valeur 1 jusqu'à 64
    // si l'entier t n'est pas compris dans le tableau indice alors on l'affiche
    while(t<=64){
        for(i=0;i<64;i++){
            if(ind[i]==t)
                booleen=1;
        }
        if(booleen!=1)
            printf("%d ",t);
        booleen=0;
        t++;
    }
    printf("\n\n");
}





// ***************************************** EXERCICE 2 ************************************************************** //
// La réponse est dans le document PDF à trouver dans le dossier





// ***************************************** EXERCICE 3 ************************************************************** //
// ***************************************** question 1 ************************************************************** //
// Implémentation de pack8 qui prend en entrée un tableau de 64 bits et sort un tableau de 8 octets
// unpacked est le tableau de 64 bits et bytes est le tableau de 8 octets
void pack8(BYTE *unpacked, BYTE *bytes){

    // déclaration des variables
    int i,somme=0;
    int j=0;
    int cpt=0;

    // boucle sur les 64 bits de unpacked
    for(i=0;i<64;i++)
    {
        somme = (somme << 1) + (unpacked[i]); // faire un décalage à gauche puis additioner
        cpt++; // le compteur sert à identifier le 8eme bit, on additione à chaque boucle
        if(cpt==8) //lorsque le compteur a identifié le 8eme bit
        {
            bytes[j]=somme; //la j-ième case de bytes prend la valeur de somme
            j++; // j représente l'indice du tableau bytes, on additione à chaque if
            somme=0; // reinitialiser somme
            cpt=0; //reinitialiser cpt
        }
    }
}



// Implémentation de unpack8 qui prend en entrée un tableau de 8 octets et sort un tableau de 64 bits
// packed est le tableau de 8 octets et binary est le tableau de 64 bits
void unpack8(BYTE *packed, BYTE *binary){

    // déclaration des variables
    int i,j,k=0;

    // boucle qui fixe une case de packed
    for(i=0;i<8;i++)
    {
        // boucle pour les 8 bits d'une case de packed
        for(j=7;j>=0;j--)
        {
            binary[k] = ((packed[i]>>j)&1); //attribuer les valeurs de binary par méthode de décalage
            k++; // additioner k à chaque boucle
        }
    }
}




// ***************************************** question 2 ************************************************************** //
//vérification de pack8 et unpack8 à l'aide des données étalons
void verifpack(){

    // vérification sur trois données étalons
    BYTE plain[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    BYTE pkey[8]={0x13,0x34,0x57,0x79,0x9b,0xbc,0xdf,0xf1};
    BYTE out[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};

    // déclaration des variables pour la suite
    BYTE bits1[64];
    BYTE bytes1[8];
    BYTE bits2[64];
    BYTE bytes2[8];
    BYTE bits3[64];
    BYTE bytes3[8];
    int i;

    printf("Donnee originale :\n");
    for(i=0;i<8;i++)
        printf("%x ",plain[i]);
    printf("\n\n");
    unpack8(plain,bits1);
    printf("Affichage de plain en bits :\n");
    for(i=0;i<64;i++)
        printf("%d ",bits1[i]);
     printf("\n\n");
     pack8(bits1,bytes1);
     printf("Affichage de plain en octets :\n");
     for(i=0;i<8;i++)
        printf("%x ",bytes1[i]);
     printf("\n\n");
     for (i=0;i<8;i++) {
        if (plain[i] != bytes1[i])
            printf("La verification a echoue.\n"); // si aucune affichage de cette phrase alors la vérification a réussi
     }
     printf("La verification a reussi.\n\n\n\n");


    printf("Donnee originale :\n");
    for(i=0;i<8;i++)
        printf("%x ",pkey[i]);
    printf("\n\n");
    unpack8(pkey,bits2);
    printf("Affichage de plain en bits :\n");
    for(i=0;i<64;i++)
        printf("%d ",bits2[i]);
     printf("\n\n");
     pack8(bits2,bytes2);
     printf("Affichage de plain en octets :\n");
     for(i=0;i<8;i++)
        printf("%x ",bytes2[i]);
     printf("\n\n");
     for (i=0;i<8;i++) {
        if (pkey[i] != bytes2[i])
            printf("La verification a echoue.\n"); // si aucune affichage de cette phrase alors la vérification a réussi
     }
     printf("La verification a reussi.\n\n\n\n");


    printf("Donnee originale :\n");
    for(i=0;i<8;i++)
        printf("%x ",out[i]);
    printf("\n\n");
    unpack8(out,bits3);
    printf("Affichage de plain en bits :\n");
    for(i=0;i<64;i++)
        printf("%d ",bits3[i]);
     printf("\n\n");
     pack8(bits3,bytes3);
     printf("Affichage de plain en octets :\n");
     for(i=0;i<8;i++)
        printf("%x ",bytes3[i]);
     printf("\n\n");
     for (i=0;i<8;i++) {
        if (out[i] != bytes3[i])
            printf("La verification a echoue.\n"); // si aucune affichage de cette phrase alors la vérification a réussi
     }
     printf("La verification a reussi.\n");
}





// ***************************************** question 3 ************************************************************** //
// Implémentation de DES à 16 tours
// prend en entrée in (clair), out (chiffré) et pkey (clé principale)

// Nous devons verifier qu'avec le clair 01 23 45 67 89 AB CD EF et la clé 13 34 57 79 9B BC DF F1,
// nous trouvons le chiffré 85 E8 13 54 0F 0A B4 05.
// Malheuresement, nous ne trouvons pas le bon résultat car nous obtenons out = E0 06 37 5F 28 9F 81 FB
// mais voici nos algorithmes :
// des est l'algorithme de chiffrement
// desDech est l'algorithme de déchiffrement
void des(BYTE *in, BYTE *out, BYTE *pkey){

    // déclaration de la permutation IP
    BYTE IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7};

    // déclaration de l'inverse de IP
    BYTE RFP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25};

    // déclaration de la fonction d'expansion
    BYTE E[48] = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1};

    // déclaration de la permutation finale
    BYTE P[32] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25};

    // déclaration des 8 S-Box
    BYTE S[8][48]={
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},

        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
        3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
        0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},

        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
        1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},

        {7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
        3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},

        {2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
        4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},

        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
        9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
        4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},

        {4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
        1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
        6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},

        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
        1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
        7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
        2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

    // déclaration des variables
    int i,j;
    int t,k,u;

    // transformation du clair en un tableau de 64 bits
    static BYTE block[64];
    unpack8(in,block);

    // génération des 16-sous clés à partir de pkey
    BYTE KS[16][48];
    setkey(pkey,KS);

    // application de la permutation IP qu'on attribue à LR
    static BYTE LR[64];
    for (j=0; j<64; j++) LR[j] = block[IP[j]-1];

    // déclaration des tableaux
    BYTE preS[48];
    BYTE f[48];

// boucles de 16 tours
// à chaque boucle : application de la fonction d'expansion, puis addition de la i-ème sous-clé, puis application des S-box
// et enfin, une permutation finale
    for (i=0; i<16; i++) {
        for (j=0; j<48; j++) preS[j] = LR[E[j]+31] ^ KS[i][j];
        for (j=0; j<8; j++) {
            k = 6*j;
            t = preS[k];
            t = (t<<1) | preS[k+5];
            t = (t<<1) | preS[k+1];
            t = (t<<1) | preS[k+2];
            t = (t<<1) | preS[k+3];
            t = (t<<1) | preS[k+4];
            t = S[j][t];
            k = 4*j;
            f[k] = (t>>3) & 1;
            f[k+1] = (t>>2) & 1;
            f[k+2] = (t>>1) & 1;
            f[k+3] = t & 1;
        }
        for (j=0; j<32; j++) {
            t = LR[j+32];
            LR[j+32] = LR[j] ^ f[P[j]-1];
            LR[j] = t;
        }

// affichage du message à chaque tour
/*        pack8(LR,out);
        printf("%d tour : ",i+1);
        for(u=0;u<8;u++)
            printf("%x ",out[u]);
        printf("\n");
*/
    }

    // application de l'inverse de IP
    for (j=0; j<64; j++) block[j] = LR[RFP[j]-1];
            pack8(block,out); // 8 BITS --> BYTE
}




// Comme expliqué dans l'exercice 2, l'algorithme de déchiffrement
// est identique à l'algorithme de chiffrement moyennant l'ordre des sous-clés
void desDech(BYTE *in, BYTE *out, BYTE *pkey){

    // déclaration de la permutation IP
    BYTE IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7};

    // déclaration de l'inverse de IP
    BYTE RFP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25};

    // déclaration de la fonction d'expansion
    BYTE E[48] = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1};

    // déclaration de la permutation finale
    BYTE P[32] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25};

    // déclaration des 8 S-Box
    BYTE S[8][48]={
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},

        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
        3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
        0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},

        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
        1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},

        {7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
        3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},

        {2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
        4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},

        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
        9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
        4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},

        {4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
        1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
        6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},

        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
        1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
        7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
        2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

    // déclaration des variables
    int i,j;
    int t,k,u;

    // transformation du clair en un tableau de 64 bits
    static BYTE block[64];
    unpack8(in,block);

    // génération des 16-sous clés dans le sens inverse à partir de pkey
    BYTE reverseKS[16][48];
    reverse_key_schedule(16,pkey,reverseKS);

    // application de la permutation IP qu'on attribue à LR
    static BYTE LR[64];
    for (j=0; j<64; j++) LR[j] = block[IP[j]-1];

    // déclaration des tableaux
    BYTE preS[48];
    BYTE f[48];

// boucles de 16 tours
// à chaque boucle : application de la fonction d'expansion, puis addition de la i-ème sous-clé, puis application des S-box
// et enfin, une permutation finale
    for (i=0; i<16; i++) {
        for (j=0; j<48; j++) preS[j] = LR[E[j]+31] ^ reverseKS[i][j];
        for (j=0; j<8; j++) {
            k = 6*j;
            t = preS[k];
            t = (t<<1) | preS[k+5];
            t = (t<<1) | preS[k+1];
            t = (t<<1) | preS[k+2];
            t = (t<<1) | preS[k+3];
            t = (t<<1) | preS[k+4];
            t = S[j][t];
            k = 4*j;
            f[k] = (t>>3) & 1;
            f[k+1] = (t>>2) & 1;
            f[k+2] = (t>>1) & 1;
            f[k+3] = t & 1;
        }
        for (j=0; j<32; j++) {
            t = LR[j+32];
            LR[j+32] = LR[j] ^ f[P[j]-1];
            LR[j] = t;
        }
    }

    // application de l'inverse de IP
    for (j=0; j<64; j++) block[j] = LR[RFP[j]-1];
            pack8(block,out); // 8 BITS --> BYTE
}







// ***************************************** EXERCICE 4 ************************************************************** //
// Implémentation de DES limité à r tours
// prend en entrée in (clair), out (chiffré), pkey (clé principale) et le nombre de tours
void desRounds(BYTE *in, BYTE *out, BYTE *pkey, int rounds){

    // si le nombre de tours n'est pas compris entre 1 et 16 inclus alors sortie du programme
    if((rounds<=0) || (16<rounds)){
        printf("Mettez un nombre de tours compris entre 1 et 16.\nFIN DU PROGRAMME.");
        exit(EXIT_FAILURE);
    }

    // déclaration de la permutation IP
    BYTE IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,
                             60, 52, 44, 36, 28, 20, 12, 4,
                             62, 54, 46, 38, 30, 22, 14, 6,
                             64, 56, 48, 40, 32, 24, 16, 8,
                             57, 49, 41, 33, 25, 17, 9, 1,
                             59, 51, 43, 35, 27, 19, 11, 3,
                             61, 53, 45, 37, 29, 21, 13, 5,
                             63, 55, 47, 39, 31, 23, 15, 7};

    // déclaration de l'inverse de IP
    BYTE RFP[64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41,  9, 49, 17, 57, 25};

    // déclaration de la fonction d'expansion
    BYTE E[48] = {
    32,  1,  2,  3,  4,  5,
    4,  5,  6,  7,  8,  9,
    8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1};

    // déclaration de la permutation finale
    BYTE P[32] = {
    16,  7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2,  8, 24, 14,
    32, 27,  3,  9,
    19, 13, 30,  6,
    22, 11,  4, 25};

    // déclaration des 8 S-Box
    BYTE S[8][48]={
        {14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
        0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
        4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
        15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13},

        {15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
        3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
        0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
        13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9},

        {10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
        13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
        13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
        1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12},

        {7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
        13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
        10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
        3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14},

        {2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
        14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
        4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
        11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3},

        {12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
        10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
        9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
        4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13},

        {4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
        13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
        1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
        6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12},

        {13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
        1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
        7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
        2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11}};

    // déclaration des variables
    int i,j;
    int t,k,u;

    // transformation du clair en un tableau de 64 bits
    static BYTE block[64];
    unpack8(in,block);

    // génération des 16-sous clés à partir de pkey
    BYTE KS[16][48];
    setkey(pkey,KS);

    // application de la permutation IP qu'on attribue à LR
    static BYTE LR[64];
    for (j=0; j<64; j++) LR[j] = block[IP[j]-1];

    // déclaration de tableaux
    BYTE preS[48];
    BYTE f[48];

// boucles de "rounds" tours
// à chaque boucle : application de la fonction d'expansion, puis addition de la i-ème sous-clé, puis application des S-box
// et enfin, une permutation finale
    for (i=0; i<rounds; i++) {
        for (j=0; j<48; j++) preS[j] = LR[E[j]+31] ^ KS[i][j];
        for (j=0; j<8; j++) {
            k = 6*j;
            t = preS[k];
            t = (t<<1) | preS[k+5];
            t = (t<<1) | preS[k+1];
            t = (t<<1) | preS[k+2];
            t = (t<<1) | preS[k+3];
            t = (t<<1) | preS[k+4];
            t = S[j][t];
            k = 4*j;
            f[k] = (t>>3) & 1;
            f[k+1] = (t>>2) & 1;
            f[k+2] = (t>>1) & 1;
            f[k+3] = t & 1;
        }
        for (j=0; j<32; j++) {
            t = LR[j+32];
            LR[j+32] = LR[j] ^ f[P[j]-1];
            LR[j] = t;
        }

// affichage du message à chaque tour
/*        pack8(LR,out);
        printf("%d tour : ",i+1);
        for(u=0;u<8;u++)
            printf("%x ",out[u]);
        printf("\n");
*/
    }
    // application de l'inverse de IP
    for (j=0; j<64; j++) block[j] = LR[RFP[j]-1];
            pack8(block,out); // 8 BITS --> BYTE
}






// ***************************************** EXERCICE 5 ************************************************************** //
// Implémentation de bruteForce
// prend en entrée un clair et un chiffré données, le nombre de tours et une sous-clé
void bruteForce(BYTE *in, BYTE *out, int rounds, BYTE *kr){

// ATTENTION : puisque l'algorithme des ne donne pas les bons résultats, cet algorithme
// donnera aussi des mauvais résultats

// toutefois, voici une proposition d'un algorithme par recherche exhaustive
// l'idée est la suivante : nous disposons d'un couple (m/c) réduit à "rounds" tour
// et d'une clé de "rounds"-tour
// nous cherchons la clé principale
// pour cela, nous allons tester toutes les possibilités de clés principales qu'on applique à desRounds
// si pour une clé principale donnée, nous avons le chiffré c' qui est égale à c
// alors il s'agit de la clé principale qu'on cherche

    //déclaration des variables à utiliser
    BYTE out2[8]; // en prévision de desRounds
    BYTE pkey[8]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};     //clé où on testera toutes les possiblités
    int i,j,p,w=1;
    int booleen=1;
    int booleen2=0;
    int cpt=0; // compteur


    // boucle sur les 8 octets de pkey pour la recherche exhaustive
    // d'abord de gauche à droite en terme de remplissage

    for(i=0;i<8;i++){
            // les 256 possibilités d'un nombre en hexadécimal
            while(w<256){
                cpt+=1;
                desRounds(in, out2, pkey,rounds);
            // vérification si notre nouveau chiffré est identique au chiffré donné
                for(j=0;j<6;j++){
                    if(out2[j]!=out[j])
                        booleen=0;
                        break;
                }
            // si booleen vaut 1 alors les deux chiffrés sont identiques
                if(booleen==1){
                    // affichage
                    printf("%d essais ont ete necessaires.\n",cpt);
                    printf("La cle principale retrouvee par recherche exhaustive est : ");
                    booleen2=1;
                    for(p=0;p<8;p++)
                        printf("%x ", pkey[p]);
                    }
                // si on trouve la bonne clé alors on quitte le programme
                if(booleen2==1)
                    return EXIT_SUCCESS;
                // addition de 1 à chaque boucle avec w
                pkey[i]+=w;
                // réinitialisation de la variable booleen
                booleen=1;
        }
    }

    // faire dans le sens inverse maintenant c'est-à-dire de droite à gauche
    // redéfinir pkey à 0
    for(i=0;i<8;i++)
        pkey[i]=0x00;

    // même boucle que la première
    for(i=7;i>=0;i++){
            while(w<256){
                printf("test2\n");
                cpt+=1;
                desRounds(in, out2, pkey,rounds);
                for(j=0;j<6;j++){
                    if(out2[j]!=out[j])
                        booleen=0;
                        break;
                }
                if(booleen==1){
                    printf("%d essais ont ete necessaires.\n",cpt);
                    printf("La cle principale retrouvee par recherche exhaustive est : ");
                    booleen2=1;
                    for(i=0;i<8;i++)
                        printf("%x ", pkey[i]);
                    }
                if(booleen2==1)
                    return EXIT_SUCCESS;
                pkey[i]+=w;
                booleen=1;
        }
    }
}







// ***************************************** EXERCICE 6 ************************************************************** //
// Implémentation de rand_num
int rand_num(n){

    int r;
    // génération d'un entier modulo n
    r = rand() % n;
    return r;
}







// ********************************************** MAIN ************************************************************** //
// ************************************************************************************************************** //

int main()
{
//******************************************************************************************************//
//                                             Affichage
//                                  Retirez les commentaires pour afficher
//******************************************************************************************************//



// ***************************************** EXERCICE 1 ************************************************************** //
// clé principale de référence
// BYTE pkey[8]={0xaa,0xbb,0x09,0x18,0x27,0x36,0xcc,0xdd};

// affichage de setkey
/*
    visualisationSetKey(pkey);
*/


// affichage de reverse_key_schedule
/*    BYTE reverseKS[16][48];
    reverse_key_schedule(4,pkey,reverseKS);
*/

// affichage de indice
/*
    BYTE ind[48];
    indice(2,ind);
*/


// affichage des bits de parité
/*
    BYTE ind[48];
    BYTE bitsdeparite[16];
    indice(16,ind);      // indiquer le nombre de tour, par défaut 16
    bitdeparite(ind);
*/





// ***************************************** EXERCICE 3 ************************************************************** //
// affichage de verification de pack8 et unpack8
//verifpack();


// affichage de des
/*
    BYTE pkey2[8]={0x13,0x34,0x57,0x79,0x9b,0xbc,0xdf,0xf1};
    BYTE plain2[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    BYTE cipher2[8];
    des(plain2,cipher2,pkey2);
    printf("----------------VISUALISATION DU CHIFFREMENT-----------------\n\n");
    int i;
    printf("Le clair est : \n");
    for(i=0;i<8;i++)
        printf("%x ",plain2[i]);
    printf("\n");

    printf("La cle est : \n");
    for(i=0;i<8;i++)
        printf("%x ",pkey2[i]);
     printf("\n");

    printf("Le chiffre est : \n");
    for(i=0;i<8;i++)
        printf("%x ",cipher2[i]);
*/

// affichage de desDech
/*
    BYTE plain2[8];
    BYTE cipher2[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};
    BYTE pkey2[8]={0x13,0x34,0x57,0x79,0x9b,0xbc,0xdf,0xf1};
    desDech(cipher2,plain2,pkey2);
    printf("----------------VISUALISATION DU DECHIFFREMENT-----------------\n\n");
    int i;
    printf("Le chiffre est : \n");
    for(i=0;i<8;i++)
        printf("%x ",cipher2[i]);
    printf("\n");

    printf("La cle est : \n");
    for(i=0;i<8;i++)
        printf("%x ",pkey2[i]);
     printf("\n");

    printf("Le chiffre est : \n");
    for(i=0;i<8;i++)
        printf("%x ",plain2[i]);
*/



// ***************************************** EXERCICE 4 ************************************************************** //
// affichage de desRounds
/*
    BYTE pkey2[8]={0x13,0x34,0x57,0x79,0x9b,0xbc,0xdf,0xf1};
    BYTE plain2[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
    BYTE cipher2[8];
    desRounds(plain2,cipher2,pkey2,16);
    int i;
    printf("Le clair est : \n");
    for(i=0;i<8;i++)
        printf("%x ",plain2[i]);
    printf("\n");

    printf("La cle est : \n");
    for(i=0;i<8;i++)
        printf("%x ",pkey2[i]);
     printf("\n");

    printf("Le chiffre est : \n");
    for(i=0;i<8;i++)
        printf("%x ",cipher2[i]);
*/

// ***************************************** EXERCICE 5 ************************************************************** //
/*
// données étalons
BYTE plain2[8]={0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef};
BYTE cipher2[8]={0x85,0xe8,0x13,0x54,0x0f,0x0a,0xb4,0x05};
BYTE kr16[8]={0xcd,0x3d,0x8b,0x0e,0x17,0xf5};

// affichage
bruteForce(plain2,cipher2,16,kr16);
*/




// ***************************************** EXERCICE 6 ************************************************************** //

    return 0;
}
