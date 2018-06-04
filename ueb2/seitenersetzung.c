/* Seitenersetzungsstrategien
 *
 * Dieses Programm dient als Rahmen zum Experimentieren mit
 * Seitenersetzungsstrategien. 
 *
 * Bitte ergaenzen Sie die Funktion "seitenzugriff()" um die
 * noetige Funktionalitaet zur Zuweisung eines Seitenrahmens
 * an die Seite, auf die zugegriffen werden soll.
 * 
 * Der Übersicht halber werden die Seiten mit "Namen"
 * aus einem Zeichen (char) bezeichnet, nicht mit numerischen Adressen.
 *
 * Falls Sie die "Aging"-Strategie realisieren moechten, steht
 * Ihnen die Funktion "zaehler_aktualisieren()" als Basis zur
 * Verfuegung, in der Sie die Aktualisierung der Zaehlerstaende
 * aus den uebergebenen "Referenziert"-Bits ref_bits[] ergaenzen
 * koennen.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define PAGEFRAMES 3

typedef struct pf {
    char ref_bit;  /* "Referenced"-Flag */
    char seite;    /* Name ("Adresse") der in diesem Rahmen gelagerten Seite (oder Null) */
    unsigned int zaehler;   /* Zaehlervariable fuer Aging-Verfahren */
} PageFrame;

/** Hilfsfunktion: Bitweise Ausgabe von 'c' **/
void printbits(unsigned int c) {
  int i;
  for (i=7; i>=0; i--) {
    printf("%d", (c & 1<<i) != 0);
  }
}

/** Hilfsfunktion: Seitenrahmen und Zaehlerstaende ausgeben **/
void dumpframes(PageFrame rahmen[]) {
  int j;
  printf("*** Seitenrahmen- und Zaehlerstaende ***\n");
  for (j=0; j < PAGEFRAMES; j++) {
    printf("* Rahmen %d: Inhalt='%c', ref=%d, Zaehler=", j,
	    rahmen[j].seite, rahmen[j].ref_bit);
    printbits(rahmen[j].zaehler);
    printf("\n");
  }
}

/****************************************************************************/

void zaehler_aktualisieren(PageFrame rahmen[]) {

   /* * * B I T T E   H I E R   E R G A E N Z E N * * *
    *
    * Aktualisierung der Zaehlerstaende fuer alle Seitenrahmen
    * mit den Referenziert-Bits der dort gespeicherten Seiten (ref_bits),
    * danach ref_bits loeschen.
    *
    */

}


void seitenzugriff(char seite, PageFrame rahmen[]) {
	int i = 0;
	int index = 0;
	int counter_size = 0;
	printf("\nZugriff auf Seite %c\n",seite);

   /* * * B I T T E   H I E R   E R G A E N Z E N * * *
    *
    * Gibt es schon Rahmen mit Seite 'seite'?
    *   ja? gut
    *   nein?
    *     Ist noch ein Rahmen frei?
    *       ja? Seite in freien Rahmen einstellen
    *       nein? Einen Rahmen gemaess 'Aging-Verfahren' frei machen,
    *             Seite dort einstellen
    *
    * Durch Bildschirmausgabe die Schritte bitte dokumentieren
    */
    
    for(i=0; i<PAGEFRAMES; i++){
		if(rahmen[i].seite == seite){
			rahmen[i].ref_bit = 1;
			rahmen[i].zaehler = 1;
			printf("%s %c\n", "Zugriff auf Seite", seite);
			printf("%s %c %s %d\n", "Seite", seite, "ist bereits in Rahmen", i);
			return;
		}
	}
	printf("%s %c\n", "SEITENFEHLER fuer Seite", seite);
	for(i=0; i<PAGEFRAMES; i++){
		if(rahmen[i].seite == 0){
			printf("%s %d %s %c %s\n", "Rahmen", i, "ist noch frei, wird mit", rahmen[i].seite, "belegt" );
			rahmen[i].seite = seite;
			rahmen[i].ref_bit = 1;
			rahmen[i].zaehler = 1;
			return;
		}
	}
	
	printf("%s %c\n", "Zugriff auf Seite", seite);
	printf("%s %c %s %d\n", "Seite", seite, "ist bereits in Rahmen", i);
	
	counter_size = rahmen[i-1].zaehler;
	for(i=1; i<PAGEFRAMES; i++){
		if(counter_size > rahmen[i].zaehler){
			counter_size = rahmen[i].zaehler;
			index = i;
		}
	}
	printf("%s %d %s %c" ,"Belege Rahmen", index, "mit Seite", rahmen[index].seite);
	rahmen[index].zaehler = 0;
	rahmen[index].zaehler = 1;
	rahmen[index].ref_bit = 1;
}

/*
 * H a u p t f u n k t i o n
 */
int main(int argc, char **argv) {
  int i, n;

  PageFrame rahmen[PAGEFRAMES] = {{0}};
  char seite;

  /* Genau einen Parameter entgegennehmen, sonst Abbruch */
  assert(argc == 2);

  n = strlen(argv[1]);
  for (i=0; i<n; i++) {
    /* Kommandozeilenparameter zeichenweise ('seite') abarbeiten */
    seite = argv[1][i];
    assert((seite >= 'A') && (seite<= 'Z'));

    /* Seitenzugriff fuer dieses Seite simulieren */
    seitenzugriff(seite, rahmen);

    /* Aging-Verfahren: Zaehler aktualisieren */
    zaehler_aktualisieren(rahmen);

    dumpframes(rahmen);

  }
  printf("\n*** ENDE ***\n");
  return 0;
}

