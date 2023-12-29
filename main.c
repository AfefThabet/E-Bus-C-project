#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#define MAXnbvoyage 30
#define NBrecherche 3
int choix;
typedef struct date{
    int jour;
    int mois;
    int annee;
}date;

typedef struct voyageur {
    int cin;
    char nom[50], prenom[50],email[50];
    date datenais;
}voyageur;

typedef struct bus {
    char idbus[50],depart[50],destination[50];
    int nbplace;
    bool clima;
}bus;

typedef struct voybus {
    int ref;
    char idbus[50],depart[50],destination[50];
    char heure_depart[6],heure_arrivee[6];
    float prix;
}voybus;

typedef struct reservation{
    int cinvoy,refvoy,code_reservation;
    date datevoy;
}reservation;

typedef struct recherche{
    date datevoy;
    char depart[50],destination[50];
}recherche;


typedef struct Noeudvoy Noeudvoy;
struct Noeudvoy {
    struct voyageur v;
    struct Noeudvoy *suivant;
};
typedef Noeudvoy *Listevoy;
Listevoy Lvoy;

typedef struct Noeudres Noeudres;
struct Noeudres {
    struct reservation r;
    struct Noeudres *suivant;
};
typedef Noeudres *Listeres;

typedef voybus T_voyage[7][MAXnbvoyage];
T_voyage t;
int nbvoy[7];

typedef struct Noeudrech
{ recherche rech; 
  struct Noeudrech * suiv; 
} Noeudrech;

typedef struct Filerech
{ Noeudrech * T; 
  Noeudrech * Q;
  int taille;
}Filerech;
Filerech F;
void initfile (Filerech* F){
    F->T = NULL;
    F->Q = NULL;
    F->taille=0;
}

void enfiler (Filerech* F,recherche r)
{   Noeudrech * nouveau;
    nouveau = (Noeudrech*) malloc (sizeof(Noeudrech));
    (*nouveau).rech = r;
    if ((F->T == NULL) && (F->Q == NULL)){ 
      F->T = nouveau;
      F->Q = nouveau;
    }
    else { 
        F->Q->suiv = nouveau; // Enfiler le nouveau nœud
        F->Q = nouveau; // Mettre à jour la queue
    }
    (F->taille)++;
}

int taillefile(Filerech* F){
    return F->taille;
}

recherche defiler (Filerech* F)
{   Noeudrech * temp; // Pointeur temporaire utilisé pour la suppression du nœud en tête
    recherche r;
    if ((F->T == NULL) && (F->Q == NULL)){ 
        printf ("Pas de recherche recente!\n"); return r; 
    }
    temp = F->T;
    r = (*temp).rech;
    if (F->T == F->Q) // Si la file contient un seul élément
    { F->T = F->Q = NULL; }
    else // Si la file contient plusieurs éléments
    { F->T = F->T->suiv; } // Mettre à jour la tête de la file
    free (temp); // Défiler le premier élément de la file
    (F->taille)--;
    return r; // Renvoyer la donnée en tête
}

void cree_compte(){
    voyageur v1;
    FILE *file;
    printf("cin : ");scanf("%d", &v1.cin);
    printf("Nom : ");scanf("%s", v1.nom);
    printf("Prenom : ");scanf("%s", v1.prenom);
    printf("Email : ");scanf("%s", v1.email);
    printf("Date de naissance(jj/mm/aaaa) : ");scanf("%d/%d/%d",&v1.datenais.jour,&v1.datenais.mois,&v1.datenais.annee);
    while(v1.datenais.jour < 1 || v1.datenais.jour > 31 || v1.datenais.mois < 1 || v1.datenais.mois > 12 || v1.datenais.annee < 1900 || v1.datenais.annee > 2024){
        printf("Saisie incorrecte. Reessayez (au format jj/mm/aaaa) : ");
        scanf("%d/%d/%d",&v1.datenais.jour,&v1.datenais.mois,&v1.datenais.annee);
    }
    file = fopen("./voyageurs.txt", "a+");
    fwrite(&v1, sizeof(voyageur), 1, file);
    if(fwrite != 0)
        printf("voyageur ajoute avec succes\n");
    else
        printf("erreur\n");
    Listevoy p = malloc(sizeof(Noeudvoy));
    p->v=v1;
    p->suivant=Lvoy;
    Lvoy=p;
    fclose(file);
}
int afficher_bus(){
    bus b;
    FILE *file;
    if((file = fopen("./bus.txt","r"))==NULL) {
            printf("aucun bus existe\n");
            return (-1);
    }
    printf("|  id bus  | ville de depart | ville de destination | nombre de places | climatise |\n");
    printf("|:--------:|:---------------:|:--------------------:|:----------------:|:---------:|\n");
    char cn[4]="non";
    char co[4]="oui";
    while((fscanf(file, "%s %s %s %d %d", b.idbus,b.depart,b.destination,&b.nbplace,&b.clima))!= EOF){
        if(b.clima)
            printf("| %-9.9s| %-16.16s| %-21.21s| %-17.d| %-10.10s|\n", b.idbus,b.depart,b.destination,b.nbplace,co);
        else
            printf("| %-9.9s| %-16.16s| %-21.21s| %-17.d| %-10.10s|\n", b.idbus,b.depart,b.destination,b.nbplace,cn);
    }  
    fclose(file);
}
int joursemaine(date d){
    struct tm date = {0}; 
    date.tm_year = d.annee - 1900; 
    date.tm_mon = d.mois - 1; 
    date.tm_mday = d.jour; 
    time_t timestamp = mktime(&date);
    struct tm *local_time = localtime(&timestamp);
    int day_of_week = local_time->tm_wday;
    return day_of_week;
}
void strcapitalize( char * s ) {
    if(s!="")
        s[0]=toupper(s[0]);
    for (int i = 1; i < strlen(s); i++) {
        s[i]=tolower(s[i]);
    }
}
void chercher_bus(){
    recherche rch;
    voybus vb;
    bool found=false;
    printf("date de depart(jj/mm/aaaa) : ");scanf("%d/%d/%d",&rch.datevoy.jour,&rch.datevoy.mois,&rch.datevoy.annee);
    while(rch.datevoy.jour < 1 || rch.datevoy.jour > 31 || rch.datevoy.mois < 1 || rch.datevoy.mois > 12 || rch.datevoy.annee < 2023 || rch.datevoy.annee > 2025){
        printf("Saisie incorrecte. Reessayez (au format jj/mm/aaaa) : ");
        scanf("%d/%d/%d",&rch.datevoy.jour,&rch.datevoy.mois,&rch.datevoy.annee);
    }
    printf("ville de depart : ");scanf("%s", &rch.depart);
    printf("ville de destination : ");scanf("%s", &rch.destination);
    strcapitalize(rch.depart);strcapitalize(rch.destination);
    int j=joursemaine(rch.datevoy);
    printf("| reference |  id bus  | ville de depart | ville de destination | heure de depart | heure d'arrivee |  prix  |\n");
    printf("|:---------:|:--------:|:---------------:|:--------------------:|:---------------:|:---------------:|:------:|\n");
    for(int i=0;i<nbvoy[j];i++){
        vb=t[j][i];
        if((strcmp(vb.depart,rch.depart)==0) && (strcmp(vb.destination,rch.destination)==0)){
            printf("| %-10.d| %-9.9s| %-16.16s| %-21.21s| %-16.16s| %-16.16s| %-7.3f|\n",vb.ref,vb.idbus,vb.depart,vb.destination,vb.heure_depart,vb.heure_arrivee,vb.prix);
            found=true;
        }
    }
    if(!found){
        printf(" Aucun voyage disponible\n");
    }
    else if(taillefile(&F)<NBrecherche){
        enfiler(&F,rch);
    }
    else{
        defiler(&F);
        enfiler(&F,rch);
    }
}

void afficher_recherches(){                
    recherche rch;              
    voybus vb;              
    int taille=taillefile(&F);                             
    if(taille==0){
        printf("pas de recherche recente!\n");
    }
    for(int i=1;i<=taille;i++){
        rch=defiler(&F);
        int j=joursemaine(rch.datevoy);
        printf("Recherche N%d:\n",i);
        printf("date de voyage: %d/%d/%d\n",rch.datevoy.jour,rch.datevoy.mois,rch.datevoy.annee);
        printf("| reference |  id bus  | ville de depart | ville de destination | heure de depart | heure d'arrivee |  prix  |\n");
        printf("|:---------:|:--------:|:---------------:|:--------------------:|:---------------:|:---------------:|:------:|\n");
        for(int i=0;i<nbvoy[j];i++){
            vb=t[j][i];
            if((strcmp(vb.depart,rch.depart)==0) && (strcmp(vb.destination,rch.destination)==0)){
                printf("| %-10.d| %-9.9s| %-16.16s| %-21.21s| %-16.16s| %-16.16s| %-7.3f|\n",vb.ref,vb.idbus,vb.depart,vb.destination,vb.heure_depart,vb.heure_arrivee,vb.prix);
            }
        }
        enfiler(&F,rch);
    }
}

void cree_reservation(){
    int cin;
    printf("cin : ");scanf("%d", &cin);
    bool found=false;
    Listevoy Q=Lvoy;
    while(Q){
        if(Q->v.cin==cin){
            found=true;
            break;
        }
        Q=Q->suivant;
    }

    if(!found){
        printf("compte voyageur introuvable\n");
    }
    else{
        reservation r;
        r.cinvoy=cin;
        printf("bienvenue Mr/Mme %s %s\n",Q->v.nom,Q->v.prenom);
        printf("code reservation(chiffres): ");scanf("%d",&r.code_reservation);
        found=false;
        FILE *file;
        if((file = fopen("./reservations.txt","r"))!=NULL) {
            reservation r1;
            Listeres Lres=NULL;
            Listeres aux;
            while(fread(&r1, sizeof(reservation), 1, file)) {
                Listeres p = malloc(sizeof(Noeudres));
                p->r=r1;
                p->suivant=NULL;
                if(r1.code_reservation==r.code_reservation){
                    found=true;
                    break;
                }      
                if (Lres==NULL){
                    Lres=p;
                    aux=p;
                }
                else{
                    aux->suivant=p;
                    aux=aux->suivant;
                }
            }
            fclose(file);
        }
        if(found){
            printf("code reservation existant\n");
        }
        else{
            printf("reference voyage: ");scanf("%d",&r.refvoy);
            printf("date de voyage(jj/mm/aaaa): ");scanf("%d/%d/%d",&r.datevoy.jour,&r.datevoy.mois,&r.datevoy.annee);
            while(r.datevoy.jour < 1 || r.datevoy.jour > 31 || r.datevoy.mois < 1 || r.datevoy.mois > 12 || r.datevoy.annee < 2023 || r.datevoy.annee > 2025){
                printf("Saisie incorrecte. Reessayez (au format jj/mm/aaaa) : ");
                scanf("%d/%d/%d",&r.datevoy.jour,&r.datevoy.mois,&r.datevoy.annee);
            }
            int j=joursemaine(r.datevoy);
            voybus vb;
            found=false;
            for(int i=0;i<nbvoy[j];i++){
                vb=t[j][i];
                if(r.refvoy==vb.ref){
                    found=true;
                }
            }
            if(!found){
                printf("aucun voyage avec la reference %d n'est programme pour cette date.\n",r.refvoy);
            }
            else{
                FILE *file;
                file = fopen("./reservations.txt", "a");
                fwrite(&r, sizeof(reservation), 1, file);
                if(fwrite != 0)
                    printf("reservation terminee avec succes\n");
                else
                    printf("erreur\n");
                fclose(file);
            }
        } 
    }
}
int afficher_reservation(){
    FILE *file;
    reservation r;
    int code;
    bool found=false;
    printf("code reservation(chiffres): ");scanf("%d",&code);
    if((file = fopen("./reservations.txt","r"))==NULL) {
            printf("aucune reservation existe\n");
            return (-1);
    }
    while(fread(&r, sizeof(reservation), 1, file)){
        if(r.code_reservation==code){
            int j=joursemaine(r.datevoy);
            voyageur v1;
            Listevoy Q=Lvoy;
            while(Q){
                if(Q->v.cin==r.cinvoy){
                    v1=Q->v;
                    break;
                }
                Q=Q->suivant;
            }
            voybus vb;
            for(int i=0;i<nbvoy[j];i++){
                vb=t[j][i];
                if(r.refvoy==vb.ref){
                    break;
                }
            }
            printf("cin voyageur: %-14.d  nom et prenom voyageur: %s %s\n",r.cinvoy,v1.nom,v1.prenom);
            printf("reference voyage: %-10.d  date de voyage: %d/%d/%d\n",r.refvoy,r.datevoy.jour,r.datevoy.mois,r.datevoy.annee);
            printf("ville de depart: %-12.12s ville de destination: %s\n",vb.depart,vb.destination);
            printf("heure de depart: %-12.12s heure d'arrivee: %s\n",vb.heure_depart,vb.heure_arrivee);
            printf("prix: %.3f\n",vb.prix);
            found=true;
            break;
        }
    }
    if(!found){
        printf("aucune reservation avec le code %d existe\n",code);
    }
}
int modifier_reservation(){
    reservation r1;
    int code;
    bool found=false;
    FILE *file;

    printf("Code reservation a modifier : ");
    scanf("%d", &code);
    
    if((file = fopen("./reservations.txt","r"))==NULL) {
        printf("aucune reservation existe\n");
        return(-1);
    }
    Listeres Lres=NULL;
    Listeres aux;
    while(fread(&r1, sizeof(reservation), 1, file)) {
        Listeres p = malloc(sizeof(Noeudres));
        p->r=r1;
        p->suivant=NULL;
        if(r1.code_reservation==code)
            found=true;
        if (Lres==NULL){
            Lres=p;
            aux=p;
        }
        else{
            aux->suivant=p;
            aux=aux->suivant;
        }
    }
    fclose(file);
    if(!found){
        printf("aucune reservation avec le code %d existe\n",code);
    }
    else{
        reservation nvr;
        nvr.code_reservation=code;
        printf("nouvelle reference de voyage : ");
        scanf("%d", &nvr.refvoy);
        printf("nouvelle date de voyage : ");
        scanf("%d/%d/%d", &nvr.datevoy.jour,&nvr.datevoy.mois,&nvr.datevoy.annee);
        found=false;
        int j=joursemaine(nvr.datevoy);
        voybus vb;
        for(int i=0;i<nbvoy[j];i++){
            vb=t[j][i];
            if(nvr.refvoy==vb.ref){
                found=true;
            }
        }
        if(!found){
            printf("aucun voyage avec la reference %d n'est programme pour cette date.\n",nvr.refvoy);
        }
        else{
            FILE *nf;
            nf = fopen("./reservations.txt", "w");
            Listeres Q=Lres;
            while (Q){
                if (Q->r.code_reservation != code)
                    fwrite(&Q->r, sizeof(reservation), 1, nf);
                else{
                    nvr.cinvoy=Q->r.cinvoy;
                    fwrite(&nvr, sizeof(reservation), 1, nf);
                }
                Q=Q->suivant;
            }
            fclose(nf);
            printf("la reservation est modifiee avec succes\n");
        }
    }
}
int annuler_reservation(){
    reservation r1;
    int code;
    bool found=false;
    FILE *file;

    printf("Code reservation a annuler : ");
    scanf("%d", &code);

    if((file = fopen("./reservations.txt","r"))==NULL) {
        printf("aucune reservation existe\n");
        return(-1);
    }

    Listeres Lres=NULL;
    Listeres aux=Lres;
    while(fread(&r1, sizeof(reservation), 1, file)) {
        Listeres p = malloc(sizeof(Noeudres));
        p->r=r1;
        p->suivant=NULL;
        if(r1.code_reservation==code)
            found=true;
        if (Lres==NULL){
            Lres=p;
            aux=p;
        }
        else{
            aux->suivant=p;
            aux=aux->suivant;
        }
    }
    fclose(file);
    remove("./reservations.txt");
    if(!found){
        printf("aucune reservation avec le code %d existe\n",code);
    }
    else{
        FILE *nf;
        nf = fopen("./reservations.txt", "w");
        Listeres Q=Lres;
        while (Q){
            if (Q->r.code_reservation != code)
                fwrite(&Q->r, sizeof(reservation), 1, nf);
            Q=Q->suivant;
        }
        fclose(nf);
        printf("la reservation est annulee avec succes\n");
    }
}
void menu_gestion_reservation(){
    int choixs;
    do{
        printf("-----------GESTION DES RESERVATIONS-----------\n");
        printf("1-creer une nouvelle reservation\n");
        printf("2-afficher les details d'une reservation\n");
        printf("3-modifier une reservation\n");
        printf("4-annuler une reservation\n");
        printf("5-Retour au menu principal\n");
        printf("----------------------------------------------\n");
        printf("choix: ");scanf("%d",&choixs);
        if(choixs==1)
            cree_reservation();
        else if(choixs==2)
            afficher_reservation();
        else if(choixs==3)    
            modifier_reservation();
        else if(choixs==4)
            annuler_reservation();
        else if(choixs!=5)
            printf("mauvais choix\n");
    }while(choixs>5);
}

int menu(){
    do{
        printf("---------------------MENU---------------------\n");
        printf("1-creer un nouveau compte voyageur\n");
        printf("2-afficher les details des bus\n");
        printf("3-chercher un bus\n");
        printf("4-afficher mes dernieres recherches\n");
        printf("5-gerer les reservations\n");
        printf("6-quitter l'application\n");
        printf("----------------------------------------------\n");
        printf("choix: ");scanf("%d",&choix);
        if(choix==1)
            cree_compte();
        else if(choix==2)
            afficher_bus();
        else if(choix==3)    
            chercher_bus();
        else if(choix==4)
            afficher_recherches();
        else if(choix==5)
            menu_gestion_reservation();
        else if(choix!=6)
            printf("mauvais choix\n");
    }while (choix!=6);
    printf("Au revoir !\n");
    return 0;
}

int main() {
    FILE *file;
    if((file = fopen("./voyages.txt","r"))!=NULL) {
        voybus vb;
        int i=0,j,nb=0;
        while((fscanf(file, "%d %d %s %s %s %s %s %f", &j,&vb.ref,vb.idbus,vb.depart,vb.destination,vb.heure_depart,vb.heure_arrivee,&vb.prix))!= EOF){
            if(i==j){
                t[i][nb]=vb;  
            }else{
                nbvoy[i]=nb;
                i=j;
                nb=0;
                t[i][nb]=vb; 
            }
            nb++;
        }
        nbvoy[i]=nb;
        fclose(file);
    }
    if((file = fopen("./voyageurs.txt","r"))!=NULL) {
        voyageur v1;
        while(fread(&v1, sizeof(voyageur), 1, file)){
            Listevoy p = malloc(sizeof(Noeudvoy));
            p->v=v1;
            p->suivant=Lvoy;
            Lvoy=p;
        }
        fclose(file);
    }
    initfile(&F);
    printf("\n");
    printf("**********Bienvenue sur E-BUS-Travel**********\n");
    printf("**********************************************\n");
    menu();
}