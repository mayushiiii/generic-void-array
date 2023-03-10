# Void array generic

Un vector de tip void care respecta o anumita forma. Elementul cheie al programului este folosirea de memmove si de memcpy, datorata faptului ca vectorul este de tip void.
Functionalitatile sunt explicate mai jos.

## Main
In main se intampla multe. as prefera sa nu intru in tot legat de ce s-a 
initializat, insa important de alocat ar fi header-ul, datele retinute, array-ul
(array ul se aloca in functie) si structura. In rest sunt multe chestii 
ajutatoare statice.
Buffer-ul citeste o linie, si se opreste cand se citeste secventa 'exit'.
Cu strstr se verifica comenzile: inserturile le fac in aceeasi secventa, avand in vedere ca add_at e doar un caz particular de add_last.

Ce e cel mai important in main e ce se intampla in insert; aici se afla informatia de introdus in structura de date:
- verific la inceput daca insertul este simplu sau insert_at si aflu indexul
din timp
- verific tipul, deci felul in care adaug elementul
- verific daca ceea ce citesc este un cuvant sau numar si il adaug diferit in 
functie de asta
- la fiecare adaugare se realizeaza o realocare (pregatire pt adaugare) si
apoi un memmove
- in toiul acestei parcurgeri, am un 'cursor' i care la final va avea 
lungimea totala a datelor, deci va fi atribuit lui data->header->len
- in functie de tipul de insert realizez inserarea
- eliberez data pt a adauga date noi dupa
in rest cum am zis verific restul functiilor

## Insert

- se verifica daca vectorul e gol, se aloca daca da, se parcurge vectorul pentru 
a afla lungimea si se realoca cu lungimea aflata + lungimea din data;
- stiindu-se lungimea vectorului pre-adaugare se muta cu memmove imediat dupa
lungimea vectorului in octeti intai header-ul intreg, apoi datele. stim
size-urile la amandoua deci stim cat de adauga si cum.
- daca nu s a adaugat se returneaza o eroare
- altfel creste len-ul aka nr de elemente

Aceeasi logica se aplica si la add_at:
- aflam 'marginile' noastre, cea dinainte de pct de adaugare si finalul vectorului
- pt a pregati vectorul, se aloca lungimea finala + data->header->len si se muta 
tot ce e dupa pre_add_len cu data->header->len spatii mai in fata
- introduci elementele ca mai sus

## Delete

- aflu 'marginile' si aici, adica de unde incep stergerea, cat am de sters, si
lungimea efectiva. stiind astea 3, am produs matematica.
- suprascriu cu memmove, oarecum ai putea zice ca e opusul lui add_at; se muta
de la punctul unde incepe stergerea tot ce e dupa pct din care incepe+lungimea
elem de sters, un nr de total - elem_sters+elem_dinainte_de_sters elemente.
e ciudat sa explic asta pentru ca am luat atatea variabile, dar pe scurt mut
totul la stanga cu lungime_elem_de_sters pozitii
- realoc ca sa scap de resturi
- verific erori(cumva?)
- scad lenul

## Print

Am un 'cursor' care se tot muta, o data isi afla tipul si apoi introduc
functia de printare a unui sg element. o sa detaliez despre ea aici. o apelez
de len ori aici.

- se verifica tipul(a trebuit sa il iau ca variabila separata, altfel nu merge)
- am aflat ce voiam , mut cursorul cu sizeof(header) adica 8 (1 + 3 padding + 4)
- in functie de tip afisez banutii diferit , in rest se fac scheme cu stringuri 
si cu acel cursor. aici se afla top 10 linii lungi, refuz sa explic mai mult.
- am scris cu sizeof ca sa para mai riguros macar ceva din haosul ala
- fun fact: pot printa cu %s multumita terminatoarelor de sir. multumesc pt asta
- retin cursorul in functie asa ca pot continua printarea lejer.

## Find

- se parcurge pana la index
- se afiseaza o data de la arr + lungimea parcursa pana la index

_______________________________________________________________________________

sidenote pt ca simt ca e important de mentionat: pe parcurs iau diverse 
variabile din vector sau le bag. cam orice legat de vectorul asta se poate face 
doar cu memcpy sau cu memmove, pentru ca nu iti poti accesa informatia asa usor.
