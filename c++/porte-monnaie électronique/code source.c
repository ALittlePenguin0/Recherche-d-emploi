void intro_data()
{
    int i;
    	
    // vérification de la taille
    if (p3>MAXI)   // MAXI est la taille maximale
	{
	    sw1=0x6c;  // code erreur correspondant à l'erreur de taille
        sw2=MAXI;	// sw2 contient l'information de la taille correcte
		return;
    }
    	
	sendbytet0(ins);	// acquitement

	uint16_t data[p3];  // déclaration du tableau data à p3 cases
	
	for(i=0;i<p3;i++)
	{
           data[i]=recbytet0();     // le tableau data reçoit les octets
           eeprom_write_block(data,&ee_solde,p3); // écriture des octets dans le solde via la fonction eeprom_write_block
	}
	
	taille=p3;  // la variable globale taille est de type uint16_t
	sw1=0x90;   // code qui valide une bonne exécution de la fonction
}

void sortir_data()
{
	uint16_t data[p3];  // déclaration du tableau data à p3 cases
    eeprom_read_block(data,&ee_solde,p3);   // data reçoit les valeurs de ee_solde

    int i;
    
	# vérification de la taille
	if (p3!=taille)
    	{
        	sw1=0x6c;	// code erreur indiquant une taille incorrecte
        	sw2=taille;	// sw2 prend la taille attendue
        	return;
    	}

	sendbytet0(ins);	// acquittement

	// émission des données
	for(i=0;i<p3;i++)
        	sendbytet0(data[i]);
    sw1=0x90; // code qui valide une bonne exécution de la fonction
}

void crediter(){

    // vérification de la taille
    if(p3!=taille)
    {
      sw1=0x6c;
      sw2=taille;
      return;
    }
    
    // récupération des deux octets
    uint8_t x1=recbytet0(); 
    uint8_t x2=recbytet0();
    
    // déclaration du solde 
    uint16_t solde;
    
    // solde reçoit les valeurs de ee_solde
    eeprom_read_block(&solde,&ee_solde,2); 
    
    // addition des deux octets par méthode de décalage
    uint16_t somme=(x1<<8) + x2; 

   if(solde+somme>=solde)   // test de vérification, plus de détails dans la partie 3.4
   {
        solde=solde+somme;  // additioner solde et somme
        eeprom_write_block(&solde,&ee_solde,2); # mettre à jour le solde
        sw1=0x90;   // code qui valide une bonne exécution de la fonction
    }
    else
      sw1=0x6c; // code erreur
}

void debiter(){
    
    // premières instructions identiques à crediter()
    
    if(p3!=taille)
    {
      sw1=0x6c;
      sw2=taille;
      return;
    }
    
    uint8_t x1=recbytet0();  
    uint8_t x2=recbytet0();
    
    uint16_t solde;

    eeprom_read_block(&solde,&ee_solde,2); 
    
    uint16_t somme=(x1<<8) + x2;

    // faire la soustraction
    if(solde-somme<=solde)  # test de vérification
    {
    solde = solde-somme; //soustraire somme à solde
    eeprom_write_block(&solde,&ee_solde,2); # mettre à jour le solde
    sw1=0x90;
    }
   else
    sw1=0x6c;
}
