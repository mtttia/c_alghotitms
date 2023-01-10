#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define MAX_ORDINI 15

/*
 * 9 giugno 2021
 */

typedef struct prodotto Prodotto;
typedef struct ordine Ordine;
typedef struct queue_ordine QueueOrdine;
typedef struct ordine_cliente OrdineCliente;

struct prodotto{
    int id_prodotto;
    char descrizione[101];
    int disponibilita_residua;
    int codice_errore;
    float prezzo;
};

struct ordine{
    int id_prodotto;
    int quantita;
    Ordine* next;
};

struct queue_ordine{
    OrdineCliente * first;
    OrdineCliente * last;
};

struct ordine_cliente{
    char nome_e_cognome[71];
    char indirizzo[71];
    Ordine* ordini;
    int num_ordini;
    OrdineCliente *next;
};

void add_ordine_cliente(QueueOrdine *ordini, OrdineCliente * ordine)
{
    ordine->next = NULL;
    if(ordini->first == NULL)
    {
        ordini->first = ordine;
        ordini->last = ordine;
    }
    else
    {
        ordini->last->next = ordine;
        ordini->last = ordine;
    }
}

int sottrai_dip_residua(Prodotto* prod, int num, int id_prodotto, int qt)
{
    for (int i = 0; i < num; ++i) {
        if(prod[i].id_prodotto == id_prodotto)
        {
            if(prod[i].disponibilita_residua > qt)
            {
                prod[i].disponibilita_residua -= qt;
                return 1;
            }
            else
            {
                prod[i].codice_errore = 1;
                return -1;
            }
        }
    }

    return -1;
}

void add_prodotto_to_ordini(OrdineCliente *ordine, Ordine* to_add, Prodotto* prodotti, int num)
{
    for (int i = 0; i < ordine->num_ordini; ++i) {
        if(ordine->ordini[i].id_prodotto == to_add->id_prodotto)
        {
            if(sottrai_dip_residua(prodotti, num, to_add->id_prodotto, to_add->quantita) > 0)
            {
                ordine->ordini[i].quantita += to_add->quantita;
                return;
            }
        }
    }

    if(ordine->num_ordini >= MAX_ORDINI)
    {
        return;
    }

    if(sottrai_dip_residua(prodotti, num, to_add->id_prodotto, to_add->quantita) > 0)
    {
        ordine->ordini[ordine->num_ordini] = *to_add;
        ordine->num_ordini++;
    }
}

void leggi_ordini(char *filename, Prodotto* prodotti, int num, QueueOrdine *ordini)
{
    FILE* file = fopen(filename, "r");

    char nome_e_cognome[71];
    char indirizzo[71];
    while(fscanf(file, "%s %s", nome_e_cognome, indirizzo) == 2)
    {
        //creo un ordine
        OrdineCliente *oc = malloc(sizeof(OrdineCliente));
        strcpy(oc->nome_e_cognome, nome_e_cognome);
        strcpy(oc->indirizzo, indirizzo);
        oc->next = NULL;
        oc->ordini = malloc(sizeof(Ordine)*MAX_ORDINI);
        oc->num_ordini = 0;

        int id_prodotto, quantita;
        while(fgetc(file) != '\n' && fscanf(file, "%d %d", &id_prodotto, &quantita) == 2)
        {
            Ordine* ord = malloc(sizeof(Ordine));
            ord->id_prodotto = id_prodotto;
            ord->quantita = quantita;
            ord->next = NULL;
            add_prodotto_to_ordini(oc, ord, prodotti, num);
        }
        add_ordine_cliente(ordini, oc);
    }
}

void stampa_lista(OrdineCliente *ordini)
{
    if(ordini == NULL)
    {
        return;
    }
    else
    {
        printf("ORDINE: %s, %s\n", ordini->nome_e_cognome,ordini->indirizzo);
        stampa_lista(ordini->next);
    }
}

float get_prezzo(int id_prodotto, Prodotto* prodotti, int num)
{
    for (int i = 0; i < num; ++i) {
        if(prodotti[i].id_prodotto == id_prodotto)
        {
            return prodotti[i].prezzo;
        }
    }

    return 0;
}

float calcola_scontrino_di(Ordine* ordini, int num, Prodotto* prodotti, int num_prodotti)
{
    float count = 0;
    for (int i = 0; i < num; ++i) {
        count += get_prezzo(ordini[i].id_prodotto, prodotti, num_prodotti) * ordini[i].quantita;
    }
    return count;
}

void calcola_scontrino_clienti(OrdineCliente *ordini, Prodotto* prodotti, int num_prodotti)
{
    if(ordini == NULL) return;
    else
    {
        //calcola scontrino
        printf("%s, SPESA: %.2f\n", ordini->nome_e_cognome, calcola_scontrino_di(ordini->ordini, ordini->num_ordini, prodotti, num_prodotti));
        calcola_scontrino_clienti(ordini->next, prodotti, num_prodotti);
    }
}

void initProdotti(Prodotto* prodotti)
{
    prodotti[0].id_prodotto = 23;
    prodotti[0].codice_errore = 0;
    prodotti[0].prezzo = 10;
    prodotti[0].disponibilita_residua = 100;
    prodotti[1].id_prodotto = 11;
    prodotti[1].codice_errore = 0;
    prodotti[1].prezzo = 20;
    prodotti[1].disponibilita_residua = 1;
}

int main() {
    Prodotto prodotti[150];
    initProdotti(prodotti);
    int num_prodotti = 2;
    QueueOrdine *ordini = malloc(sizeof(QueueOrdine));
    ordini->first = NULL;
    ordini->last = NULL;


    leggi_ordini("ordini.txt", prodotti, num_prodotti, ordini);
    stampa_lista(ordini->first);
    printf("\n\nSCONTRINI\n");
    calcola_scontrino_clienti(ordini->first, prodotti, num_prodotti);

    return 0;
}
