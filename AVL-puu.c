

/*
  Ohjelma on rakennettu luennoilta/harjoituksista saadun materiaalin pohjalta.
  Puun tulostuksessa käytetty apuna osoitteesta http://www.daniweb.com/software-development/c/threads/146112 löydettyä koodia.*/



#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>




typedef struct puusolmu_t {
  int luku;
  short tila; /* tasapainoilmaisin */
  struct puusolmu_t *vasen, *oikea;
} puusolmu, *puuosoitin;

void lisaa_solmu(puuosoitin *, int, int *);
void oikea_kierto(puuosoitin *, int *);
void tulosta_puu(puuosoitin,int);/*käytetty apuna osoitteesta http://www.daniweb.com/software-development/c/threads/146112 löydettyä koodia*/
void vasen_kierto(puuosoitin *, int *);
void haeavain(puuosoitin *,int,int *);
void padding(char,int);
int main()
{
  int etp = 0,testi,haku,valinta;
  puuosoitin puu = NULL;
while(1){
  printf("1)Anna luku.\n2)Hae luku\n");
  printf("Valintasi (muu valinta kuin 1 tai 2 lopettaa ohjelman):");
  scanf("%d",&valinta);
  if(valinta==1){
  while(1)/*luvun lisäys*/
  {
    printf("\nAnna luku(negatiivinen luku lopettaa lukujen antamisen):");
    scanf("%d",&testi);
    if(testi<0)break;
    lisaa_solmu(&puu, testi, &etp);
    printf("\n");
    tulosta_puu(puu,0);

  }}
  else if(valinta==2){
  while(1){/*luvun haku*/
    printf("Minkä avaimen haluat hakea (negatiivinen luku lopettaa haun):");
    scanf("%d",&haku);
    if(haku<0)break;
    haeavain(&puu, haku, &etp );
    printf("\n");
    }}
  else{
    printf("Ohjelma lopetetaan.\n");
    break;}    
}
  return 0;
}

void lisaa_solmu(puuosoitin *emo, int luku, int *etp)
{
  if(!(*emo))
  {
    *etp = 1;
    if(!(*emo = (puuosoitin)malloc(sizeof(puusolmu))))/*muistin varaus*/
    {
      perror("malloc");
      exit(1);
    }
    (*emo)->vasen = (*emo)->oikea = NULL;
    (*emo)->tila = 0;
    (*emo)->luku = luku;
    printf("Sijoitetaan %d \n",luku);
  } else if(luku < (*emo)->luku)/*syötetty luku pienempi kuin juuri*/
  {
    lisaa_solmu(&(*emo)->vasen, luku, etp);/*rekursio*/
    printf("Sijoitetaan %d vasemmalle avaimesta %d \n",luku,(*emo)->luku);
    if(*etp)
    {
      switch((*emo)->tila)
      {
        case -1:
          (*emo)->tila = 0;
          *etp = 0;
          break;
        case 0:
          (*emo)->tila = 1;
          break;
        case 1:
          vasen_kierto(emo, etp);/*suoritetaan jos puussa epätasapaino*/

      }
    }
  } else if(luku > (*emo)->luku)/*syötetty luku suurempi kuin juuri*/
  {
    lisaa_solmu(&(*emo)->oikea, luku, etp);/*rekursio*/
    printf("Sijoitetaan %d oikealle avaimesta %d\n",luku,(*emo)->luku);

    if(*etp)
    {
      switch((*emo)->tila)
      {
        case 1:
          (*emo)->tila = 0;
          *etp = 0;
          break;
        case 0:
          (*emo)->tila = -1;
           break;
        case -1:
          oikea_kierto(emo, etp);/*suoritetaan jos puussa epätasapaino*/

      }
    }
  } else
  {
    *etp = 0;
    printf("Luku %d on jo puussa\n", luku);

  }
}

void padding ( char ch, int n )
{
int i;

for ( i = 0; i < n; i++ )
putchar ( ch );
}

void tulosta_puu(puuosoitin solmu,int taso)/*puun tulostus rekursion avulla*/
{

    if(!solmu) return;

    tulosta_puu(solmu->vasen,taso+1);
    padding ( '\t', taso );
    printf("%d\n",solmu->luku);
    tulosta_puu(solmu->oikea,taso+1);
}


void vasen_kierto(puuosoitin *emo, int *etp)
{
  puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->vasen;
  if(lapsi->tila == 1) /* LL-kierto */
  {
    (*emo)->vasen = lapsi->oikea;
    lapsi->oikea = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
    printf("LL-kierto \n");
  } else /* LR-kierto */
  {
    lapsenlapsi = lapsi->oikea;
    lapsi->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = lapsi;
    (*emo)->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = *emo;
    printf("LR-kierto \n");
    switch(lapsenlapsi->tila)
    {
      case 1:
        (*emo)->tila = -1;
        lapsi->tila = 0;
        break;
      case 0:
        (*emo)->tila = lapsi->tila = 0;
        break;
      case -1:
        (*emo)->tila = 0;
        lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}


void oikea_kierto(puuosoitin *emo, int *etp) {
puuosoitin lapsi, lapsenlapsi;

  lapsi = (*emo)->oikea;
  if(lapsi->tila == -1) /* RR-kierto */
  {
    (*emo)->oikea = lapsi->vasen;
    lapsi->vasen = *emo;
    (*emo)->tila = 0;
    (*emo) = lapsi;
    printf("RR-kierto \n");
  } else /* RL-kierto */
  {
    lapsenlapsi = lapsi->vasen;
    lapsi->vasen = lapsenlapsi->oikea;
    lapsenlapsi->oikea = lapsi;
    (*emo)->oikea = lapsenlapsi->vasen;
    lapsenlapsi->vasen = *emo;
    printf("RL-kierto \n");
    switch(lapsenlapsi->tila)
    {
      case 1:
        (*emo)->tila = -1;
        lapsi->tila = 0;
        break;
      case 0:
        (*emo)->tila = lapsi->tila = 0;
        break;
      case -1:
        (*emo)->tila = 0;
        lapsi->tila = 1;
    }
    *emo = lapsenlapsi;
  }
  (*emo)->tila = 0;
  *etp = 0;
}

void haeavain(puuosoitin *emo, int luku, int *etp){
    if(luku < (*emo)->luku)
  {
        if(&(*emo)->vasen->luku==NULL){/*lehden vasen solmu on null*/
            printf("%d ei ole puussa\n",luku);
            return ;}
        haeavain(&(*emo)->vasen, luku, etp);}
    else if(luku > (*emo)->luku)
  {     if(&(*emo)->oikea->luku==NULL){/*lehden oikea solmu on null*/
            printf("%d ei ole puussa\n",luku);
            return;}
        haeavain(&(*emo)->oikea, luku, etp);

  }
    else
  {

        *etp = 0;
        printf("Luku %d on puussa\n", luku);

  }
}

