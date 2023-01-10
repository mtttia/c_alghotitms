#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <malloc.h>

typedef struct Agente Agente;
typedef struct Appuntamenti Appuntamenti;

struct Agente{
    char nome_cognome_agente[71];
    char id_agente[11];
    int n_clienti_agente;
    Appuntamenti* appuntamenti;
    Agente* next;
};

Agente* agenti;

struct Appuntamenti{
    char id_cliente [11];
    char id_agente [11];
    char codice_richiesta_prestazione [16];
    Appuntamenti* next;
};

struct agente_nomi{
    char nome_cognome_agente[71];
    char id_agente[11];
};

struct agente_nomi id_nomi_agenti[100];

char filename [20]= "appuntamenti.txt";

char* get_nome_agente(char id_agente[11])
{
    for (int i = 0; i < 100; ++i) {
        if(strcmp(id_nomi_agenti[i].id_agente, id_agente))
        {
            return id_nomi_agenti[i].nome_cognome_agente;
        }
    }

    return "";
}

Agente* get_agente(Agente* lista, char id_agente[11])
{
    Agente* curr = lista;
    while(curr != NULL)
    {
        if(strcmp(curr->id_agente, id_agente) == 0)
        {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

bool agente_exists(Agente* lista, char id_agente[11])
{
    Agente* curr = lista;
    while(curr != NULL)
    {
        if(strcmp(curr->id_agente, id_agente) == 0)
        {
            return true;
        }
        curr = curr->next;

    }

    return false;
}

Agente* push_agente(Agente* lista, Agente* toAdd)
{
    toAdd->next = NULL;

    Agente *prec, *curr;
    curr = lista;

    while((curr != NULL) && (strcmp(curr->id_agente, toAdd->id_agente) < 0))
    {
        prec = curr;
        curr = curr->next;
    }

    if(prec != NULL)
    {
        prec->next = toAdd;
    }
    else
    {
        lista = toAdd;
    }

    toAdd->next = curr;

    return lista;
}

void push_appointment_to_agente(Agente* agente, Appuntamenti* appuntamento)
{
    appuntamento->next = NULL;
    if(agente->appuntamenti == NULL)
    {
        agente->appuntamenti = appuntamento;
    }
    else
    {
        //guardo se posso aggiungere l'appuntamento in mezzo ordinato
        Appuntamenti* curr = agente->appuntamenti;
        bool added = false;
        while(curr->next != NULL)
        {
            if(strcmp(curr->id_cliente, appuntamento->id_cliente) == 0)
            {
                //sono uguali
                Appuntamenti *temp = curr->next;
                curr->next = appuntamento;
                appuntamento->next = temp;
                added = true;
                break;
            }

            curr = curr->next;
        }
        if(!added)
        {
            curr->next = appuntamento;
        }


    }
}

Agente* push_appointment(Agente* lista, Appuntamenti* appuntamento)
{
    if(!agente_exists(lista, appuntamento->id_agente))
    {
        Agente* agente = (Agente*) malloc(sizeof(Agente));

        strcpy(agente->nome_cognome_agente, get_nome_agente(appuntamento->id_agente));
        strcpy(agente->id_agente, appuntamento->id_agente);
        agente->appuntamenti = NULL;
        agente->n_clienti_agente = 0;

        lista = push_agente(lista, agente);
    }

    Agente* curr = get_agente(lista, appuntamento->id_agente);
    push_appointment_to_agente(curr, appuntamento);

    return lista;
}

Agente* leggi_appuntamenti(char* filename)
{
    FILE* file = fopen(filename, "r");
    
    if(file == NULL)
    {
        exit(1);
    }

    char id_cliente[11];
    char id_agente[11];
    char codice_richiesta_prestazione[16];

    while(fscanf(file, "%10s %10s %15s", id_cliente, id_agente, codice_richiesta_prestazione) == 3)
    {
        Appuntamenti* curr = (Appuntamenti*) malloc(sizeof(Appuntamenti));
        strcpy(curr->id_cliente, id_cliente);
        strcpy(curr->id_agente, id_agente);
        strcpy(curr->codice_richiesta_prestazione, codice_richiesta_prestazione);

        agenti = push_appointment(agenti, curr);
    }

    fclose(file);
    return agenti;
}

void stampa_agentir(Agente* lista)
{
    if(lista != NULL)
    {
        printf("%s\n", lista->id_agente);
        stampa_agentir(lista->next);
    }
}

int get_numero_appuntamenti(Appuntamenti* appuntamenti)
{
    if(appuntamenti == NULL)
        return 0;
    else if(appuntamenti->next == NULL)
        return 1;
    else return 1 + get_numero_appuntamenti(appuntamenti->next);
}

Agente* migliori_tre_agenti(Agente* lista)
{
    int app_uno = 0, app_due = 0, app_tre = 0;
    Agente* uno = NULL;
    Agente* due = NULL;
    Agente* tre = NULL;

    Agente *curr = lista;
    while(curr != NULL)
    {
        int temp_num = get_numero_appuntamenti(curr->appuntamenti);
        if(temp_num >= app_tre)
        {
            app_tre = temp_num;
            tre = curr;
            if(temp_num >= app_due)
            {
                app_tre = app_due;
                tre = due;
                app_due = temp_num;
                due = curr;
                if(temp_num >= app_uno)
                {
                    app_due = app_uno;
                    due = uno;
                    app_tre = app_due;
                    tre = due;
                    app_uno = temp_num;
                    uno = curr;
                }
            }
        }

        curr = curr->next;
    }

    if(due != NULL)
        uno->next = due;
    if(tre != NULL)
    {
        due->next = tre;
        tre->next = NULL;
    }
    return uno;
}

void premio_agente_anno(Agente* lista)
{

    Agente *migliori_tre = migliori_tre_agenti(lista);
    printf("MIGLIORI_TRE\n");
    stampa_agentir(migliori_tre);
}



int main() {

    agenti = NULL;
    agenti = leggi_appuntamenti("appuntamenti.txt");
    premio_agente_anno(agenti);

    return 0;
}
