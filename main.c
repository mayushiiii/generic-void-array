#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include <inttypes.h>
#include <ctype.h>

int add_last(void** arr, int* len, data_structure* data)
{
	// deci ipotetic. sa zicem ca len este lungimea pre-adaugare. atunci ce am face
	// pe langa ca se va schimba aici, vom adauga Dupa Ea
	int total_len = 0;
	if ((*len) == 0)
		*arr = calloc(1, data->header->len + sizeof(data->header));
	else
	{
		int i, curr_len;
		for (i = 0; i < *len; i++)
		{
			total_len += 4;
			memmove(&curr_len, *arr + total_len, sizeof(int));
			total_len += curr_len + 4;
		}
		*arr = realloc(*arr, total_len + sizeof(data->header) + data->header->len);
	}
	memmove(*arr + total_len, data->header, sizeof(head));
	memmove(*arr + total_len + sizeof(data->header), data->data, data->header->len);
	if (*arr + total_len == NULL)
		return -1;

	(*len)++;
	return 0;
}

int add_at(void** arr, int* len, data_structure* data, int index)
{
	// toate elementele trebuie translatate cu len mai in fata, 
	// pt a face loc pt ce urmeaza. aici cred ca e motivul pt care memmove 
	// e mai bun? el muta, nu copiaza deci merge ok si la translatari de genul
	// aici e ca la delete dar pe dos.
	// daca acolo retin totalul, de unde incep sa sterg si cat sterg, aici aflu
	// totalul, de unde adaug si cat.
	int i, curr_len, total_len = 0, add_len, pre_add_len = 0;
	if (index < 0)
		return -1;
	if ((*len) == 0)
		*arr = calloc(1, data->header->len + sizeof(data->header));
	else // parcurgere
		for (i = 0; i < *len; i++)
		{
			total_len += 4;
			memmove(&curr_len, *arr + total_len, sizeof(int));
			total_len += curr_len + 4;
			if (i < index)
				pre_add_len = total_len;
		}
	// se realoca pt cat era inainte + cat vr sa adaug.
	*arr = realloc(*arr, total_len + sizeof(data->header) + data->header->len);
	add_len = sizeof(data->header) + data->header->len;
	memmove(*arr + add_len, *arr + pre_add_len, total_len + add_len - pre_add_len);
	memmove(*arr + pre_add_len, data->header, sizeof(head));
	memmove(*arr + pre_add_len + sizeof(data->header), data->data, data->header->len);
	if (*arr + pre_add_len == NULL)
		return -1;

	(*len)++;
	return 0;
}

// afisare pt un sg element ca sa nu copiez blocul de text si la find
void print_element(void* arr, int* position_counter)
{
	char type = *(char*)(arr + *position_counter);
	printf("Tipul %c\n", type);
	(*position_counter) += 8;
	if (type == '1')
	{
		// position_counter este ca un i insa foarte specific si il mut
		// in functie de nevoile mele.
		// este important sa stiu toate size urile.
		// aici e stringul de acolo + terminatorul primului sir +
		// sizeof (cele 2 tipuri) mai incolo, in fct de tip: 3, 7 , 9
		printf("%s pentru %s\n", (char*)(arr + *position_counter), (char*)(arr + *position_counter + strlen(arr + *position_counter) + 3));
		(*position_counter) += strlen(arr + *position_counter) + 1;
		int8_t bill_one, bill_two;
		memmove(&bill_one, arr + *position_counter, sizeof(int8_t));
		memmove(&bill_two, arr + *position_counter + sizeof(int8_t), sizeof(int8_t));
		printf("%" PRId8 "\n%" PRId8 "\n", bill_one, bill_two);
		*position_counter += sizeof(int8_t) + sizeof(int8_t);
		// +1 pt celalalt terminator
		*position_counter += strlen(arr + *position_counter) + 1;
	}
	if (type == '2')
	{
		printf("%s pentru %s\n", (char*)(arr + *position_counter), (char*)(arr + *position_counter + strlen(arr + *position_counter) + 7));
		*position_counter += strlen(arr + *position_counter) + 1;
		int16_t bill_one;
		int32_t bill_two;
		memmove(&bill_one, arr + *position_counter, sizeof(int16_t));
		memmove(&bill_two, arr + *position_counter + sizeof(int16_t), sizeof(int32_t));
		printf("%" PRId16 "\n%" PRId32 "\n", bill_one, bill_two);
		*position_counter += sizeof(int16_t) + sizeof(int32_t);
		*position_counter += strlen(arr + *position_counter) + 1;
	}
	if (type == '3')
	{
		printf("%s pentru %s\n", (char*)(arr + *position_counter), (char*)(arr + *position_counter + strlen(arr + *position_counter) + 9));
		*position_counter += strlen(arr + *position_counter) + 1;
		int32_t bill_one;
		int32_t bill_two;
		memmove(&bill_one, arr + *position_counter, sizeof(int32_t));
		memmove(&bill_two, arr + *position_counter + sizeof(int32_t), sizeof(int32_t));
		printf("%" PRId32 "\n%" PRId32 "\n", bill_one, bill_two);
		*position_counter += sizeof(int32_t) + sizeof(int32_t);
		*position_counter += strlen(arr + *position_counter) + 1;
	}
	printf("\n");
}

void find(void* data_block, int len, int index)
{
	// presupunand ca imi stiu nr de elemente deoarece il returnez
	// ar trebui sa fac parcurgerile in functie de tipuri
	// si cand am ajuns la a index-a parcurgere, sa afisez
	int total_len = 0, curr_len, i;
	for (i = 0; i < index; i++)
	{
		total_len += 4;
		memmove(&curr_len, data_block + total_len, sizeof(int));
		total_len += curr_len + 4;
	}
	print_element(data_block, &total_len);
}

int delete_at(void** arr, int* len, int index)
{
	// curr_len e ca sa tin cont de ce lungime are elementul nr i si o adaug la
	// total.
	// deoarece stergerea implica o translatare a array-ului
	// cu (lungimea elementului de sters) pozitii mai la stanga
	// consider ca e necesar sa stiu capatul (deoarece el se muta), lungimea
	// elementelor inainte de cel care va fi sters (deoarece acolo se muta)
	// si, evident, cat am de mutat.
	int i, curr_len, total_len = 0, remove_len = 0, pre_remove_len = 0;
	// parcurgere sa aflu toate lungimile cu care lucrez
	for (i = 0; i < *len; i++)
	{
		total_len += 4;
		if (i == index)
		{
			// 8 = sizeof(header); il adaug manual pt ca nu e bagat in functie
			memmove(&remove_len, (*arr + total_len), sizeof(int));
			remove_len += 8;
		}
		memmove(&curr_len, *arr + total_len, sizeof(int));
		total_len += curr_len + 4;
		if (i < index)
			pre_remove_len = total_len;
	}
	// suprascrierea/ translatarea
	memmove(*arr + pre_remove_len, *arr + (pre_remove_len + remove_len), total_len - pre_remove_len - remove_len);
	// si evident va trb sa realoc pt ca acum e ca s
	total_len = total_len - remove_len;
	*arr = realloc(*arr, total_len);
	(*len)--;
	return 0;
}

void print(void* arr, int len)
{
	int i = 0, position_counter = 0;
	for (i = 0; i < len; i++)
	{
		// faptul ca apelez cu & imi permite sa continui de unde am ramas.
		print_element(arr, &position_counter);
	}
}

int main(int argc, char** argv)
{
	// initalizari.
	void* arr = NULL;
	int len = 0, i, index;
	char* buffer;
	char* token = NULL;
	data_structure* dedications;
	dedications = (data_structure*)calloc(1, sizeof(data_structure));
	dedications->header = calloc(2, sizeof(dedications->header));
	dedications->header->type = '\0';
	size_t bufsize = 256;
	buffer = (char*)calloc(bufsize, sizeof(char));
	dedications->data = NULL;
	// chestie interesanta! strtok face singur free deci nu mai trb sa dau free
	// la final lui buffer. sau lui token dealtfel.
	while (strstr(fgets(buffer, 256, stdin), "exit") == 0)
	{
		buffer[strlen(buffer) - 1] = '\0';
		token = strtok(buffer, " ");
		if (strstr(buffer, "insert"))
		{
			// inserturile sunt ambele aici! dar vin cu inputuri diferite
			// deci trb sa ma asigur ca am ce trebuie, in cazul in care
			// trebuie sa inserez la indexul respectiv
			if (strstr(buffer, "insert_at"))
			{
				token = strtok(NULL, " ");
				index = atoi(token);
			}
			token = strtok(NULL, " ");
			dedications->header->len = 0;
			// deoarece type e de acelasi size mereu ajunge memmove sa suprascriu
			// no need to realloc imo, desi ar fi fost mai curat asa
			memmove(&dedications->header->type, token, 1);
			i = 0;
			// obtinerea de date se face aici, de aia fac ambele inserturi in
			// aceeasi zona. am 3 ifuri pt 3 tipuri, il explic doar pe primul
			// +cv mic la al 2lea
			if (dedications->header->type == '1')
			{
				dedications->data = malloc(sizeof(void*));
				token = strtok(NULL, " ");
				while (token != NULL)
				{
					// sutn 2 cazuri evident: nume si bancnote, deci verific
					// in fct de primul caracter
					if ((token[0] >= '0' && token[0] <= '9') || token[0] == '-')
					{
						// e char. trb convertit
						int8_t cool_number = atoi(token);
						// se realoca cum adaug, in fct de size-ul elementelor
						dedications->data = realloc(dedications->data, i + sizeof(int8_t));
						memcpy((char*)(dedications->data) + i, &cool_number, sizeof(int8_t));
						i++;
					}
					else
					{
						// sau de lungimea stringului
						dedications->data = realloc(dedications->data, i + strlen(token) + 1);
						memcpy((char*)(dedications->data) + i, token, strlen(token) + 1);
						i += strlen(token) + 1;
					}
					token = strtok(NULL, " ");
				}
			}
			if (dedications->header->type == '2')
			{
				dedications->data = malloc(sizeof(void*));
				token = strtok(NULL, " ");
				// pt ca s 2 tipuri am cele 2 cazuri si cand am trecut o data
				// prin primul se schimba ok ul
				int ok = 0;
				while (token != NULL)
				{
					if ((token[0] >= '0' && token[0] <= '9') || token[0] == '-')
					{
						if (ok == 1)
						{
							int32_t cool_number = atoi(token);
							dedications->data = realloc(dedications->data, i + sizeof(int32_t));
							memcpy((char*)(dedications->data) + i, &cool_number, sizeof(int32_t));
							i += 4;
						}
						else
						{
							int16_t cool_number = atoi(token);
							dedications->data = realloc(dedications->data, i + sizeof(int16_t));
							memcpy((char*)(dedications->data) + i, &cool_number, sizeof(int16_t));
							i += 2;
							ok = 1;
						}
					}
					else
					{
						dedications->data = realloc(dedications->data, i + strlen(token) + 1);
						memcpy((char*)(dedications->data) + i, token, strlen(token) + 1);
						i += strlen(token) + 1;
					}
					token = strtok(NULL, " ");
				}
			}
			if (dedications->header->type == '3')
			{
				dedications->data = malloc(sizeof(void*));
				token = strtok(NULL, " ");
				while (token != NULL)
				{
					if ((token[0] >= '0' && token[0] <= '9') || token[0] == '-')
					{
						int32_t cool_number = atoi(token);
						dedications->data = realloc(dedications->data, i + sizeof(int32_t));
						memcpy((char*)(dedications->data) + i, &cool_number, sizeof(int32_t));
						i += 4;
					}
					else
					{
						dedications->data = realloc(dedications->data, i + strlen(token) + 1);
						memcpy((char*)(dedications->data) + i, token, strlen(token) + 1);
						i += strlen(token) + 1;
					}
					token = strtok(NULL, " ");
				}
			}
			// len ul se afla abia la final
			dedications->header->len = i;
			// si apoi verific dupa caz cum inserez
			if (strstr(buffer, "_at"))
			{
				if (add_at(&arr, &len, dedications, index) != 0)
				{
					printf("oh nu\n");
					return -1;
				}
			}
			else if (add_last(&arr, &len, dedications) != 0)
			{
				printf("ce fraiera\n");
				return -1;
			}
			free(dedications->data);
		}

		if (strstr(buffer, "delete_at"))
		{
			// e numar. trb sa l stiu
			token = strtok(NULL, " ");
			int index = atoi(token);
			if (delete_at(&arr, &len, index) != 0)
			{
				printf("nici sa stergi nu poti\n");
				return -1;
			}
		}

		if (strstr(buffer, "find"))
		{
			// idem
			token = strtok(NULL, " ");
			int index = atoi(token);
			find(arr, len, index);
		}

		if (strstr(buffer, "print"))
		{
			print(arr, len);
		}
	}

	free(token);
	free(arr);
	free(dedications->header);
	free(dedications);
	return 0;
}