#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>

typedef struct medico Medico;
typedef struct singolo_turno SingoloTurno;
typedef struct esubero_reparto EsuberoReparto;

struct medico{
    char id_medico[13];
    char reparto[6];
    SingoloTurno *turni[7];
};

struct singolo_turno{
    int id_turno;
    int pazienti_per_turno;
    SingoloTurno *next;
};

struct esubero_reparto{
    char reparto[6];
    SingoloTurno *turni[7];
};

Medico *medici;
int num_medici;
EsuberoReparto *esuberi;
int num_esuberi;


char **repartiMedici;

void add_esubero(char reparto[6], int giorno_richiesto, int turno_richiesto)
{
    int indice = -1;
    for (int i = 0; i < num_esuberi; ++i) {
        if(strcmp(esuberi[i].reparto, reparto) == 0)
        {
            indice = i;
            break;
        }
    }

    if(indice == -1 && num_esuberi >= 200) return;

    strcpy(esuberi[num_esuberi].reparto, reparto);
    for (int i = 0; i < 7; ++i) {
        esuberi[num_esuberi].turni[i] = NULL;
    }
    indice = num_esuberi++;

    if(esuberi[num_esuberi].turni[giorno_richiesto] == NULL)
    {
        SingoloTurno *turno = malloc(sizeof(SingoloTurno));
        turno->pazienti_per_turno = 1;
        turno->next = NULL;
        turno->id_turno = turno_richiesto;
        esuberi[num_esuberi].turni[giorno_richiesto] = turno;
    }
    else
    {
        //lo cerco
        SingoloTurno *curr = esuberi[num_esuberi].turni[giorno_richiesto];
        SingoloTurno *prev = curr;
        while (curr != NULL)
        {
            if(curr->id_turno == turno_richiesto)
            {
                curr->pazienti_per_turno++;
                break;
            }
            prev  = curr;
            curr = curr->next;
        }

        if(curr == NULL)
        {
            SingoloTurno *turno = malloc(sizeof(SingoloTurno));
            turno->pazienti_per_turno = 1;
            turno->next = NULL;
            turno->id_turno = turno_richiesto;
            prev->next = turno;
        }
    }
}

void add_prenotazione_to(int i, int giorno_richiesto, int turno_richiesto)
{
    Medico* m = &medici[i];

    if(m->turni[giorno_richiesto] == NULL)
    {
        SingoloTurno *turno = malloc(sizeof(SingoloTurno));
        turno->id_turno = turno_richiesto;
        turno->pazienti_per_turno = 1;
        turno->next = NULL;
        m->turni[giorno_richiesto] = turno;
    }
    else
    {
        //cerco se c'è già
        SingoloTurno *curr = m->turni[giorno_richiesto];
        SingoloTurno *prev = curr;
        while(curr != NULL)
        {
            if(curr->id_turno == turno_richiesto)
            {
                //trovato
                if(curr->pazienti_per_turno <5)
                {
                    //tutto OK
                    curr->pazienti_per_turno++;
                }
                else
                {
                    //lo metto negli esuberi
                    add_esubero(m->reparto, giorno_richiesto, turno_richiesto);
                }
                break;
            }
            prev = curr;
            curr = curr->next;
        }

        if(curr == NULL)
        {
            SingoloTurno *turno = malloc(sizeof(SingoloTurno));
            turno->id_turno = turno_richiesto;
            turno->pazienti_per_turno = 1;
            turno->next = NULL;
            prev->next = turno;
        }
    }
}

int add_medico(char id_medico[13], char reparto[6])
{
    for (int i = 0; i < num_medici; ++i) {
        if(strcmp(medici[i].id_medico, id_medico) == 0)
        {
            return i;
        }
    }

    if(num_medici == 200)
    {
        return -1;
    }

    strcpy(medici[num_medici].id_medico, id_medico);
    strcpy(medici[num_medici].reparto, reparto);
    for (int i = 0; i < 7; ++i) {
        medici[num_medici].turni[i] = NULL;
    }

    return num_medici++;
}

void leggi_appuntamenti(char *filename)
{
    FILE *file = fopen(filename, "r");
    char id_paziente[13];
    char id_medico[13];
    int id_reparto;
    char codice_richiesta_prestazione[26];
    int turno_richiesto;
    int giorno_richiesto;

    while(fscanf(file, "%s %s-%d %s %d %d", id_paziente, id_medico, id_reparto, codice_richiesta_prestazione, &turno_richiesto, &giorno_richiesto) == 6)
    {
        //fuck off
        char* reparto = malloc(sizeof(char)*6);
        strcpy(reparto, repartiMedici[3]);
        sprintf(id_medico, "%s-%d",  id_medico, id_reparto);
        giorno_richiesto--;
        int i_m = add_medico(id_medico, reparto);
        add_prenotazione_to(i_m, giorno_richiesto, turno_richiesto);
    }

}

int calc_num_esuberi(SingoloTurno * turno)
{
    if(turno== NULL) return 0;
    else return turno->pazienti_per_turno + calc_num_esuberi(turno->next);
}

void stampa_esuberi(EsuberoReparto* lista, int num)
{
    if(lista == NULL)
    {
        return;
    }
    else
    {
        int g1 = 0;
        int g2 = 0;
        int g3 = 0;
        int g4 = 0;
        int g5 = 0;
        int g6 = 0;
        int g7 = 0;

        for (int i = 0; i < num; ++i) {
            g1 += calc_num_esuberi(lista[i].turni[0]);
            g2 += calc_num_esuberi(lista[i].turni[1]);
            g3 += calc_num_esuberi(lista[i].turni[2]);
            g4 += calc_num_esuberi(lista[i].turni[3]);
            g5 += calc_num_esuberi(lista[i].turni[4]);
            g6 += calc_num_esuberi(lista[i].turni[5]);
            g7 += calc_num_esuberi(lista[i].turni[6]);
        }

        printf("%d %d %d %d %d %d %d", g1, g2,g3,g4,g5,g6,g7);

    }
}

typedef struct RepartoStat RepartoStat;

struct RepartoStat{
    char reparto[6];
    int ore;
    int ore_tot;
    RepartoStat *next;
};

int calc_ore(Medico m)
{
    int count = 0;
    for (int i = 0; i < 7; ++i) {
        count += calc_num_esuberi(m.turni[i]);
    }
    return count;
}

void occupazione_reparti(Medico* lista, int num)
{
    RepartoStat* stat = NULL;

    for (int i = 0; i < num; ++i) {
        RepartoStat *rep = stat;
        if(stat == NULL)
        {
            stat = malloc(sizeof(RepartoStat));
            stat->ore = calc_ore(lista[i]);
            stat->ore_tot = 5*7*8;
            strcpy(stat->reparto, lista[i].reparto);
            stat->next = NULL;
        }
        else
        {
            RepartoStat *curr = stat;
            RepartoStat *prev = curr;
            while(curr != NULL)
            {
                if(strcmp(curr->reparto, lista[i].reparto) == 0)
                {
                    curr->ore += calc_ore(lista[i]);
                    curr->ore_tot += 5*7*8;
                    break;
                }

                prev = curr;
                curr = curr->next;
            }
            if(curr == NULL)
            {
                prev->next = malloc(sizeof(RepartoStat));
                prev->next->next =NULL;
                prev->next->ore = calc_ore(lista[i]);
                prev->next->ore_tot = 5*7*8;
                strcpy(prev->next->reparto, lista[i].reparto);
            }
        }
    }

    RepartoStat *curr = stat;
    while (curr != NULL)
    {
        printf("%s: %.2f\n", curr->reparto, (float) curr->ore / curr->ore_tot * 100);

        curr = curr->next;
    }
}

int main() {
    medici = malloc(sizeof(Medico) * 200);
    esuberi = malloc(sizeof(EsuberoReparto) * 200);
    repartiMedici = malloc(sizeof(char) * 6 * 200);
    num_medici = 0;
    num_esuberi = 0;
    repartiMedici[0] = "zero";
    repartiMedici[1] = "uno";
    repartiMedici[2] = "due";
    repartiMedici[3] = "tre";



    leggi_appuntamenti("appuntamenti.txt");

    stampa_esuberi(esuberi, num_esuberi);
    occupazione_reparti(medici, num_medici);

    return 0;
}
