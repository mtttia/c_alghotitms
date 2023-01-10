#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

typedef struct carriera Carriera;
typedef struct esame Esame;
typedef struct piano_di_studi PianoDiStudi;
typedef struct studente Studente;

struct carriera{
    int matricola;
    char nome_e_cognome[101];
    Esame* esami;
    int curr_esame;
};

struct esame{
    char id_esame[16];
    char esito[5];
    Esame *next;
};

struct piano_di_studi{
    int matricola;
    char **esami;
};

struct studente{
    int matricola;
    char *nome;
    char *cognome;
    Esame *esami;
    Studente* next;
};

PianoDiStudi* piano;
Studente* studenti;
int indice_studenti;

void leggi_carriera(char *filename)
{
    FILE* file = fopen(filename, "r");
    int matricola;
    char nome[101];
    char cognome[101];

    while(!feof(file)){
        Studente *s = malloc(sizeof(Studente));
        s->nome = malloc(sizeof(char) * 101);
        s->cognome = malloc(sizeof(char) * 101);
        s->esami = NULL;
        s->next = NULL;

        fscanf(file, "%d %s %s", &s->matricola, s->nome, s->cognome);
        char *nome_temp = malloc(sizeof(char) *101), *voto=malloc(sizeof(char) *101);
        while(fscanf(file, "%s %s", nome_temp, voto) == 2)
        {
            Esame *esame = malloc(sizeof(Esame));
            strcpy(esame->id_esame, nome_temp);
            strcpy(esame->esito, voto);
            esame->next = NULL;

            if(s->esami == NULL)
            {
                s->esami = esame;
            }
            else
            {
                Esame* curr_esame = s->esami;
                while(curr_esame->next != NULL)
                {
                    curr_esame = curr_esame->next;
                }
                curr_esame->next = esame;
            }

            if(fgetc(file) == '\n') break;
        }
        if(studenti == NULL)
        {
            studenti = s;
        }
        else
        {
            Studente* curr = studenti;
            while(curr->next != NULL)
            {
                curr = curr->next;
            }
            curr->next = s;
        }
    }
}

Studente *pulisci_lista(Studente* testa, int matricola)
{
    if(testa == NULL)
        return testa;
    else if(testa->matricola == matricola)
    {
        //sono il primo;
        testa = testa->next;
        return testa;
    }
    else return pulisci_lista(testa->next, matricola);

}

typedef struct stat{
    float perc_sup;
    int v_30l;
    int ritirati;
} Stat;

int tot_esame(Studente* testa, char *id_esame)
{
    if(testa == NULL)
        return 0;

    bool ha_esame = false;
    Esame* te = testa->esami;
    while(te!=NULL)
    {
        if(strcmp(te->id_esame, id_esame) == 0)
        {
            ha_esame = true;
            break;
        }

        te = te->next;
    }

    int sum = 0;

    if(ha_esame) {
        sum = 1;
    }

    return sum + tot_esame(testa->next, id_esame);
}

int sup_esame(Studente* testa, char *id_esame)
{
    if(testa == NULL)
        return 0;

    bool sup = false;
    Esame* te = testa->esami;
    while(te!=NULL)
    {
        if(strcmp(te->id_esame, id_esame) == 0)
        {
            if(strcmp(te->esito, "R") != 0 && strcmp(te->esito, "I") != 0 )
                sup = true;
            break;
        }

        te = te->next;
    }

    int sum = 0;

    if(sup) {
        sum = 1;
    }

    return sum + tot_esame(testa->next, id_esame);
}

int ha_30l(Studente* testa, char *id_esame)
{
    if(testa == NULL)
        return 0;

    bool max = false;
    Esame* te = testa->esami;
    while(te!=NULL)
    {
        if(strcmp(te->id_esame, id_esame) == 0)
        {
            if(strcmp(te->esito, "30L") == 0)
                max = true;
            break;
        }

        te = te->next;
    }

    int sum = 0;

    if(max) {
        sum = 1;
    }

    return sum + tot_esame(testa->next, id_esame);
}

int ritirato(Studente* testa, char *id_esame)
{
    if(testa == NULL)
        return 0;

    bool max = false;
    Esame* te = testa->esami;
    while(te!=NULL)
    {
        if(strcmp(te->id_esame, id_esame) == 0)
        {
            if(strcmp(te->esito, "R") == 0)
                max = true;
            break;
        }

        te = te->next;
    }

    int sum = 0;

    if(max) {
        sum = 1;
    }

    return sum + tot_esame(testa->next, id_esame);
}



Stat* statistiche_esame(Studente* testa, char *id_esame)
{
    Stat* statistica = malloc(sizeof(Stat));

    statistica->perc_sup = (float) sup_esame(testa, id_esame) / tot_esame(testa, id_esame);

    statistica->v_30l = ha_30l(testa, id_esame);
    statistica->ritirati = ritirato(testa, id_esame);

    return statistica;
}

int main() {
    leggi_carriera("carriera.txt");
    Stat* statistica = statistiche_esame(studenti, "INF100");
    studenti = pulisci_lista(studenti, 333145);


    return 0;
}
