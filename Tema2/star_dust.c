#include <stdio.h>
#include <stdlib.h>

double medie(int **mat, int *nci, int nl)
{
	double medie_aritmetica;
	double suma = 0, cat = 0;
	char byte0, byte1, byte2, byte3;
	//bytes-ii de pe linia 0
	for (int j = 0; j < *(nci); j++) {
		byte0 = (int)(mat[0][j] & 0xFF);
		byte1 = (int)((mat[0][j] >> 8) & 0xFF);
		byte2 = (int)((mat[0][j] >> 16) & 0xFF);
		byte3 = (int)((mat[0][j] >> 24) & 0xFF);
		suma = suma + byte0 + byte1 + byte2 + byte3;
		cat = cat + 4;
	}
	//bytes-ii de pe linia nl(ultima linie)
	for (int j = 0; j < *(nci + nl - 1); j++) {
		byte0 = (int)(mat[nl - 1][j] & 0xFF);
		byte1 = (int)((mat[nl - 1][j] >> 8) & 0xFF);
		byte2 = (int)((mat[nl - 1][j] >> 16) & 0xFF);
		byte3 = (int)((mat[nl - 1][j] >> 24) & 0xFF);
		suma = suma + byte0 + byte1 + byte2 + byte3;
		cat = cat + 4;
	}
	//bytes-ii de pe coloana 0 mai putin primul si ultimul
	for (int i = 1; i < nl - 1; i++) {
		//iau primul byte de la fiecare nr din matrice
		byte0 = (int)(mat[i][0] & 0xFF);
		suma = suma + byte0;
		cat = cat + 1;
	}
	//bytes-ii de pe ultima coloana mai putin primul si ultimul
	for (int i = 1; i < nl - 1; i++) {
		byte3 = (int)((mat[i][*(nci + i) - 1] >> 24) & 0xFF);
		suma = suma + byte3;
		cat = cat + 1;
	}
	medie_aritmetica = (double)suma / cat;
	return medie_aritmetica;
}

void modify(int **mat, int *nci)
{
	int linie, index, valoare;
	char dim;
	unsigned char byte[4];
	scanf(" %c", &dim);
	scanf("%d%d%X", &linie, &index, &valoare);
	if (dim == 'C') {
		if (index > (int)((*(nci + linie)) * sizeof(int))) {
			int *aux;
			aux = realloc(mat[linie], ((index - 1) / 4 + 1) * sizeof(int));
			mat[linie] = aux;
			for (int i = *(nci + linie); i < ((index - 1) / 4 + 1); i++)
				mat[linie][i] = 0;//0 in zona suplimentara
			*(nci + linie) = (index - 1) / 4 + 1;
			byte[0] = valoare & 0xFF;
			byte[1] = (valoare >> 8) & 0xFF;
			byte[2] = (valoare >> 16) & 0xFF;
			byte[3] = (valoare >> 24) & 0xFF;
			mat[linie][*(nci + linie) - 1] = *(int *)byte;
		} else {
			byte[0] = (mat[linie][(index - 1) / 4]) & 0xFF;
			byte[1] = ((mat[linie][(index - 1) / 4]) >> 8) & 0xFF;
			byte[2] = ((mat[linie][(index - 1) / 4]) >> 16) & 0xFF;
			byte[3] = ((mat[linie][(index - 1) / 4]) >> 24) & 0xFF;
			byte[(index - 1) % 4] = valoare & 0xFF;
			mat[linie][(index - 1) / 4] = *(int *)byte;
		}
	}
	if (dim == 'S') {
		if (index * 2 > (int)((*(nci + linie)) * sizeof(int))) {
			int *aux;
			aux = realloc(mat[linie], ((index - 1) / 2 + 1) * sizeof(int));
			mat[linie] = aux;
			for (int i = *(nci + linie); i < ((index - 1) / 2 + 1); i++)
				mat[linie][i] = 0;//0 in zona suplimentara
			*(nci + linie) = (index - 1) / 2 + 1;
			byte[0] = valoare & 0xFF;
			byte[1] = (valoare >> 8) & 0xFF;
			byte[2] = (valoare >> 16) & 0xFF;
			byte[3] = (valoare >> 24) & 0xFF;
			mat[linie][*(nci + linie) - 1] = *(int *)byte;
		} else {
			//memorez toti bytes-ii si inlocuies 2
			byte[0] = (mat[linie][(index - 1) / 2]) & 0xFF;
			byte[1] = ((mat[linie][(index - 1) / 2]) >> 8) & 0xFF;
			byte[2] = ((mat[linie][(index - 1) / 2]) >> 16) & 0xFF;
			byte[3] = ((mat[linie][(index - 1) / 2]) >> 24) & 0xFF;
			if ((index - 1) % 2 != 0) {
				byte[2] = valoare & 0xFF; //byte[2]=byte0 al valorii citite
				byte[3] = (valoare >> 8) & 0xFF; //byte[3]=byte1 al valorii
			}
			if ((index - 1) % 2 == 0) {
				byte[0] = valoare & 0xFF;
				byte[1] = (valoare >> 8) & 0xFF;
			}
			mat[linie][(index - 1) / 2] = *(int *)byte;
		}
	}
	if (dim == 'I') {
		if (index * 4 > (int)((*(nci + linie)) * sizeof(int))) {
			mat[linie] = realloc(mat[linie], index * sizeof(int));
			for (int i = *(nci + linie); i < index; i++)
				mat[linie][i] = 0;//0 in zona suplimentara
			*(nci + linie) = index;
			byte[0] = valoare & 0xFF;
			byte[1] = (valoare >> 8) & 0xFF;
			byte[2] = (valoare >> 16) & 0xFF;
			byte[3] = (valoare >> 24) & 0xFF;
			mat[linie][*(nci + linie) - 1] = *(int *)byte;
		} else {
			byte[0] = valoare & 0xFF;
			byte[1] = (valoare >> 8) & 0xFF;
			byte[2] = (valoare >> 16) & 0xFF;
			byte[3] = (valoare >> 24) & 0xFF;
			mat[linie][index - 1] = *(int *)byte;
		}
	}
}

void delete(int **mat)
{
	char dim;
	int linie, index;
	scanf(" %c", &dim);
	scanf("%d", &linie);
	scanf("%d", &index);
	if (dim == 'C') {
		unsigned char byte[4];
			//memorez toti bytes-ii si inlocuies doar un byte
			byte[0] = (mat[linie][(index - 1) / 4]) & 0xFF;
			byte[1] = ((mat[linie][(index - 1) / 4]) >> 8) & 0xFF;
			byte[2] = ((mat[linie][(index - 1) / 4]) >> 16) & 0xFF;
			byte[3] = ((mat[linie][(index - 1) / 4]) >> 24) & 0xFF;
			byte[(index - 1) % 4] = 000000000 & 0xFF;
			//in matrice pun nr format dupa modificarea byte-ului respectiv
			mat[linie][(index - 1) / 4] = *(int *)byte;
	}
	if (dim == 'S') {
		unsigned char byte[4];
			//memorez toti bytes-ii si inlocuies 2
			byte[0] = (mat[linie][(index - 1) / 2]) & 0xFF;
			byte[1] = ((mat[linie][(index - 1) / 2]) >> 8) & 0xFF;
			byte[2] = ((mat[linie][(index - 1) / 2]) >> 16) & 0xFF;
			byte[3] = ((mat[linie][(index - 1) / 2]) >> 24) & 0xFF;
			if ((index - 1) % 2 != 0) {
				byte[2] = 00000000 & 0xFF;
				byte[3] = (00000000 >> 8) & 0xFF;
			}
			if ((index - 1) % 2 == 0) {
				byte[0] = 00000000 & 0xFF;
				byte[1] = 00000000 & 0xFF;
			}
			mat[linie][(index - 1) / 2] = *(int *)byte;
	}
	if (dim == 'I') {
		unsigned char byte[4];
			byte[0] = 00000000 & 0xFF;
			byte[1] = (00000000 >> 8) & 0xFF;
			byte[2] = (00000000 >> 16) & 0xFF;
			byte[3] = (00000000 >> 24) & 0xFF;
			mat[linie][index - 1] = *(int *)byte;
	}
}

void swap(int **mat)
{
	char dim;
	int linie, index;
	scanf(" %c", &dim);
	scanf("%d", &linie);
	scanf("%d", &index);
	if (dim == 'C')
		mat[linie][(index - 1) / 4] = mat[linie][(index - 1) / 4];
	if (dim == 'S') {
		unsigned char byte[4];
		if (index % 2 == 0) {
			byte[1] = (mat[linie][index / 2]) & 0xFF;
			byte[0] = ((mat[linie][index / 2]) >> 8) & 0xFF;
			byte[2] = ((mat[linie][index / 2]) >> 16) & 0xFF;
			byte[3] = ((mat[linie][index / 2]) >> 24) & 0xFF;
		}
		if (index % 2 != 0) {
			byte[0] = (mat[linie][index / 2]) & 0xFF;
			byte[1] = ((mat[linie][index / 2]) >> 8) & 0xFF;
			byte[3] = ((mat[linie][index / 2]) >> 16) & 0xFF;
			byte[2] = ((mat[linie][index / 2]) >> 24) & 0xFF;
		}
			mat[linie][index / 2] = *(int *)byte;
	}
	if (dim == 'I') {
		unsigned char byte[4];
			byte[3] = mat[linie][index] & 0xFF;
			byte[2] = (mat[linie][index] >> 8) & 0xFF;
			byte[1] = (mat[linie][index] >> 16) & 0xFF;
			byte[0] = (mat[linie][index] >> 24) & 0xFF;
			mat[linie][index] = *(int *)byte;
	}
}

int verif_cate(int nl, unsigned char **mat_char, int *nci, int lin, int col)
{
	int cate = 0;
	if (mat_char[lin][col] == 0) {
		cate++;
		mat_char[lin][col] = 0xFF;
		if ((lin - 1 >= 0) && *(nci + lin - 1) > col)
			cate = cate + verif_cate(nl, mat_char, nci, lin - 1, col);
		if ((lin + 1 < nl) && *(nci + lin + 1) > col)
			cate = cate + verif_cate(nl, mat_char, nci, lin + 1, col);
		if (col - 1 >= 0)
			cate = cate + verif_cate(nl, mat_char, nci, lin, col - 1);
		if (col + 1 < *(nci + lin))
			cate = cate + verif_cate(nl, mat_char, nci, lin, col + 1);
	} else {
		return 0;
	}
	return cate;
}

void free_mat(unsigned char **mat_char, int **mat, int nl)
{
	for (int i = 0; i < nl; i++)
		free(mat[i]);
	free(mat);
	for (int i = 0; i < nl; i++)
		free(mat_char[i]);
	free(mat_char);
}

int main(void)
{
	int nl, nc, k;
	int **mat;
	int *nci; //numarul de elemente de pe fiecare de linie
	scanf("%d", &nl);
	nci = calloc(nl, sizeof(int));
	if (!nci) {
		printf("nci nu s-a alocat\n");
		return 1;
	}
	mat = malloc(nl * sizeof(int *));
	if (!mat) {
		printf("Mat nu s-a alocat\n");
		return 1;
	}
	for (int i = 0; i < nl; i++) {
		scanf("%d", &nc);
		*(nci + i) = nc;
		mat[i] = calloc(nc, sizeof(int));
		for (int j = 0; j < nc; j++)
			scanf("%X", &mat[i][j]);
	}
	//task1---------------------------------------------------------
	printf("task 1\n");
	printf("%.8f\n", medie(mat, nci, nl));
	//task2---------------------------------------------------------
	char op;
	scanf("%d", &k);
	for (int i = 0; i < k; i++) {
		scanf(" %c", &op);
		if (op == 'M')
			modify(mat, nci);
		else
			if (op == 'D')
				delete(mat);
			else
				swap(mat);
	}
	printf("task 2\n");
	for (int i = 0; i < nl; i++) {
		for (int j = 0; j < *(nci + i); j++)
			printf("%08X ", mat[i][j]);
		printf("\n");
	}
	//task3---------------------------------------------------------
	printf("task 3\n");
	unsigned char **mat_char;
	int jj = 0, cate = 0, max = 0, lin = 0, col = 0;
	mat_char = malloc(nl * sizeof(unsigned char *));
	if (!mat_char) {
		printf("Mat_char nu s-a alocat\n");
		return 1;
	}
	for (int i = 0; i < nl; i++) {
		jj = 0;
		mat_char[i] = calloc(*(nci + i) * 4, sizeof(unsigned char *));
		*(nci + i) = *(nci + i) * 4;
		for (int j = 0; j < *(nci + i); j += 4) {
			mat_char[i][j] = mat[i][jj] & 0xFF;
			mat_char[i][j + 1] = (mat[i][jj] >> 8) & 0xFF;
			mat_char[i][j + 2] = (mat[i][jj] >> 16) & 0xFF;
			mat_char[i][j + 3] = (mat[i][jj] >> 24)  & 0xFF;
			jj++;
		}
	}
	for (int i = 0; i < nl; i++) {
		for (int j = 0; j < *(nci + i); j++) {
			cate = verif_cate(nl, mat_char, nci, i, j);
			if (cate > max) {
				max = cate;
				lin = i;
				col = j;
			}
		}
	}
	printf("%d %d %d\n", lin, col, max);
	free_mat(mat_char, mat, nl);
	free(nci);
	return 0;
}
