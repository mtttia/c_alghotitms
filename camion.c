#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>

#define SOGLIA 25

typedef struct pacco Pacco;
typedef struct camion Camion;
typedef struct pacco_in_dogana PaccoInDogana;

struct pacco{
    int id_pacco;
    int peso_kg;
    int valore;
    Pacco* before;
};

struct camion{
    int id_camion;
    Pacco *pacco;
    Camion *next;
    Camion *last;
};

struct pacco_in_dogana{
    int id_camion;
    int id_pacco;
    int peso_kg;
    PaccoInDogana *next;
};

Camion *camion;

PaccoInDogana* pacchi_leggeri_in_dogana;
PaccoInDogana* pacchi_leggeri_in_dogana_last;
PaccoInDogana* pacchi_pesanti_in_dogana;
PaccoInDogana* pacchi_pesanti_in_dogana_last;

Pacco* get_pacco(Camion *c)
{
    Pacco *ret = c->pacco;
    if(c->pacco != NULL)
    {
        c->pacco = c->pacco->before;
    }
    return ret;
}

void push_pacco_pesante(struct pacco_in_dogana* pacco)
{
    pacco->next = NULL;
    if(pacchi_pesanti_in_dogana == NULL)
    {
        pacchi_pesanti_in_dogana = pacco;
        pacchi_pesanti_in_dogana_last = pacco;
    }
    else
    {
        pacchi_pesanti_in_dogana_last->next = pacco;
        pacchi_pesanti_in_dogana_last = pacco;
    }
}

void push_pacco_leggero(struct pacco_in_dogana* pacco)
{
    pacco->next = NULL;
    if(pacchi_leggeri_in_dogana == NULL)
    {
        pacchi_leggeri_in_dogana = pacco;
        pacchi_leggeri_in_dogana_last = pacco;
    }
    else
    {
        pacchi_leggeri_in_dogana_last->next = pacco;
        pacchi_leggeri_in_dogana_last = pacco;
    }
}

int calc_peso(Pacco *pacchi)
{
    if(pacchi == NULL)
    {
        return 0;
    }
    else
    {
        return pacchi->peso_kg + calc_peso(pacchi->before);
    }
}

int calc_valore(Pacco *pacchi)
{
    if(pacchi == NULL)
    {
        return 0;
    }
    else
    {
        return pacchi->valore + calc_peso(pacchi->before);
    }
}

void arrivo_in_dogana(Camion *c)
{
    printf("%d: peso...%d, valore...%d\n", c->id_camion, calc_peso(c->pacco), calc_valore(c->pacco));
    while(c->pacco == NULL)
    {
        Pacco* pacco = get_pacco(c);
        PaccoInDogana *p = malloc(sizeof(PaccoInDogana));
        p->peso_kg =pacco->peso_kg;
        p->id_pacco = pacco->id_pacco;
        p->id_camion = c->id_camion;

        if(pacco->peso_kg > SOGLIA)
        {
            push_pacco_pesante(p);
        }
        else
        {
            push_pacco_leggero(p);
        }
    }
}

typedef struct camion_stat CamionStat;

struct camion_stat{
    int id_camion;
    int num;
    CamionStat *next;
};

void statistica_pacchi_pesanti()
{
    PaccoInDogana *curr = pacchi_pesanti_in_dogana;
    CamionStat* stat = NULL;
    while(curr != NULL)
    {
        //cerco id camion
        if(stat == NULL)
        {
            //aggiungo
            stat = malloc(sizeof(CamionStat));
            stat->id_camion = curr->id_camion;
            stat->num = 1;
            stat->next = NULL;
        }
        else
        {
            CamionStat * sc = stat;
            CamionStat * sprev = sc;
            while(sc != NULL)
            {
                if(sc->id_camion == curr->id_camion)
                {
                    sc->num++;
                    return;
                }

                sprev = sc;
                sc = sc->next;
            }
            if(sc == NULL)
            {
                sprev->next = malloc(sizeof(CamionStat));
                sprev->next->id_camion = curr->id_camion;
                sprev->next->num = 1;
                sprev->next = NULL;
            }
        }


        curr = curr->next;
    }

    //ho la lista
    int id_camion, num = -1;
    CamionStat *cs = stat;
    while(cs != NULL)
    {
        if(cs->num > num)
        {
            id_camion = cs->id_camion;
            num = cs->num;
        }
    }

    printf("id: %d, num: %d\n", id_camion, num);
}

PaccoInDogana* pacchi_in_dogana;

void unisci_pacchi(PaccoInDogana* pacchi1, PaccoInDogana* pacchi2)
{
    if(pacchi1 == NULL && pacchi2 == NULL)
    {
        return;
    }
    else
    {
        PaccoInDogana *pacchi = pacchi1;
        if(pacchi1 == NULL)
        {
            pacchi = pacchi2;
            pacchi2 = NULL;
        }
        pacchi1 = NULL;

        if(pacchi_in_dogana == NULL)
        {
            pacchi_in_dogana = pacchi;
        }
        else
        {
            pacchi_in_dogana->next = pacchi;
        }



        unisci_pacchi(pacchi1, pacchi2)
    }
}

int main() {
    camion = NULL;
    pacchi_in_dogana = NULL;

    return 0;
}
