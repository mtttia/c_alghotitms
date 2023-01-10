#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct medici Medici;
typedef struct singolo_turno SingoloTurno;

struct medici{
    char id_medico[13];
    char reparto[31];
    SingoloTurno *turni[7];
    SingoloTurno *esuberi[7];
};

struct singolo_turno{
    int id_turno;
    int pazienti_per_turno;
    SingoloTurno *next;
};

Medici *medici;
int num_medici;

Medici* get_medico(char id_medico[13])
{
    for (int i = 0; i < num_medici; ++i) {
        if(strcmp(medici[i].id_medico, id_medico) == 0)
        {
            return &medici[i];
        }
    }

    //lo creo
    strcpy(medici[num_medici].id_medico, id_medico);
    strcpy(medici[num_medici].reparto, "");
    for (int i = 0; i < 7; ++i) {
        medici[num_medici].turni[i] = NULL;
        medici[num_medici].esuberi[i] = NULL;
    }

    num_medici++;

    return &medici[num_medici-1];
}

SingoloTurno *create_turno(int turno_richiesto)
{
    SingoloTurno* t = malloc(sizeof(SingoloTurno));
    t->id_turno = turno_richiesto;
    t->pazienti_per_turno = 1;
    t->next = NULL;
    return t;
}

void add_esubero(Medici* m, int turno_richiesto, int giorno_richiesto)
{
    if(m->esuberi[giorno_richiesto] == NULL)
    {
        m->esuberi[giorno_richiesto] = create_turno(turno_richiesto);
    }
    else
    {
        SingoloTurno *curr = m->esuberi[giorno_richiesto];
        SingoloTurno *prev = curr;
        while(curr != NULL)
        {
            if(curr->id_turno == turno_richiesto)
            {
                curr->pazienti_per_turno++;
                break;
            }

            prev = curr;
            curr = curr->next;
        }
        if(curr == NULL)
        {
            prev->next = create_turno(turno_richiesto);
        }
    }
}

void add_turno(Medici* m, int turno_richiesto, int giorno_richiesto)
{
    if(m->turni[giorno_richiesto] != NULL)
    {
        SingoloTurno *curr = m->turni[giorno_richiesto];
        SingoloTurno *prev = curr;
        while(curr != NULL)
        {
            if(curr->id_turno == turno_richiesto)
            {
                if(curr->pazienti_per_turno <5)
                {
                    curr->pazienti_per_turno++;
                }
                else
                {
                    add_esubero(m, turno_richiesto, giorno_richiesto);
                }
            }

            prev = curr;
            curr = curr->next;
        }

        if(curr == NULL)
        {
            prev->next = create_turno(turno_richiesto);
        }
    }
    else
    {
        m->turni[giorno_richiesto] = create_turno(turno_richiesto);
    }
}

void leggi_appuntamenti(char *filename)
{
    FILE* file = fopen(filename, "r");

    char id_paziente[13];
    char id_medico[13];
    int turno_richiesto;
    int giorno_richiesto;
    while(fscanf(file, "%s %s %d %d", id_paziente, id_medico, &turno_richiesto, &giorno_richiesto) == 4)
    {
        Medici* medico = get_medico(id_medico);
        giorno_richiesto--;
        add_turno(medico, turno_richiesto, giorno_richiesto);
    }
}

void stampa_esuberi(SingoloTurno *turni)
{
    if(turni == NULL) return;
    else {
        printf("%d -> %d\n", turni->id_turno, turni->pazienti_per_turno);
        stampa_esuberi(turni->next);
    }
}

int get_tot_ore(SingoloTurno *turno)
{
    if(turno == NULL) return 0;
    else return turno->pazienti_per_turno + get_tot_ore(turno->next);
}

void occupazione_medico(Medici *medici, int num_medici)
{
    for (int i = 0; i < num_medici; ++i) {
        Medici *m = &medici[i];
        int tot_ore = 0;
        for (int j = 0; j < 7; ++j) {
            tot_ore += get_tot_ore(m->turni[j]);
        }
        float media_ore = (float) (tot_ore * 100) / (8*7*6);
        printf("MEDICO %s, PERC...%.2f\n", m->id_medico, media_ore);

    }
}

int main() {
    medici = malloc(sizeof(Medici)*200);
    num_medici = 0;

    leggi_appuntamenti("appuntamenti.txt");
    occupazione_medico(medici, num_medici);


    return 0;
}
