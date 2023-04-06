#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define DELIM "\n "

typedef struct pgm {
	int width, height, maxval;
	unsigned char **imag;
	int type;
	long info_start;
	int is_loaded;
} pgm;

typedef struct pixel {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} pixel;

typedef struct ppm {
	int width, height, maxval;
	int type;
	int is_loaded;
	pixel **imag;
	long info_start;
} ppm;

int is_number(char string[10])
{
	int n = strlen(string);
	for (int i = 0; i < n; i++)
		if (string[i] >= 'a' && string[i] <= 'z')
			return 0;

	return 1;
}

void free_matrix(pgm *pgm_img, ppm *ppm_img)
{
	if (pgm_img->is_loaded) {
		for (int i = 0; i < pgm_img->height; i++)
			free(pgm_img->imag[i]);
		free(pgm_img->imag);
	}
	if (ppm_img->is_loaded) {
		for (int i = 0; i < ppm_img->height; i++)
			free(ppm_img->imag[i]);
		free(ppm_img->imag);
	}
}

int min(int a, int b)
{
	if (a <= b)
		return a;
	else
		return b;
}

void init(pgm *pgm_img, ppm *ppm_img)
{
	pgm_img->width = 0;
	pgm_img->height = 0;
	pgm_img->type = 0;
	pgm_img->is_loaded = 0;
	ppm_img->width = 0;
	ppm_img->height = 0;
	ppm_img->type = 0;
	ppm_img->is_loaded = 0;
}

void read_pgm(pgm *pgm_img, char *filename)
{
	FILE *in_file;
	if (pgm_img->type == 2)
		in_file = fopen(filename, "rt");
	else
		in_file = fopen(filename, "rb");

	pgm_img->imag = malloc(pgm_img->height * sizeof(unsigned char *));
	for (int i = 0; i < pgm_img->height; i++)
		pgm_img->imag[i] = malloc(pgm_img->width * sizeof(unsigned char));
	fseek(in_file, pgm_img->info_start, SEEK_SET);

	for (int i = 0; i < pgm_img->height; i++)
		for (int j = 0; j < pgm_img->width; j++) {
			if (pgm_img->type == 2)
				fscanf(in_file, "%hhd", &pgm_img->imag[i][j]);
			else
				fread(&pgm_img->imag[i][j], sizeof(unsigned char), 1, in_file);
		}
	fclose(in_file);
	pgm_img->is_loaded = 1;
}

void read_ppm(ppm *ppm_img, char *filename)
{
	FILE *in_file;
	if (ppm_img->type == 3)
		in_file = fopen(filename, "rt");
	else
		in_file = fopen(filename, "rb");
	ppm_img->imag = malloc(ppm_img->height * sizeof(pixel *));
	for (int i = 0; i < ppm_img->height; i++)
		ppm_img->imag[i] = malloc(ppm_img->width * sizeof(pixel));
	fseek(in_file, ppm_img->info_start, SEEK_SET);

	for (int i = 0; i < ppm_img->height; i++)
		for (int j = 0; j < ppm_img->width; j++) {
			if (ppm_img->type == 3) {
				fscanf(in_file, "%hhd", &ppm_img->imag[i][j].R);
				fscanf(in_file, "%hhd", &ppm_img->imag[i][j].G);
				fscanf(in_file, "%hhd", &ppm_img->imag[i][j].B);
			} else {
				fread(&ppm_img->imag[i][j], sizeof(pixel), 1, in_file);
			}
		}
	fclose(in_file);
	ppm_img->is_loaded = 1;
}

void load_image(pgm *pgm_img, ppm *ppm_img)
{
	char *filename = strtok(NULL, DELIM);
	FILE *in_file = fopen(filename, "r");
	if (!in_file) {
		printf("Failed to load %s\n", filename);
		return;
	}
	free_matrix(pgm_img, ppm_img);
	init(pgm_img, ppm_img);
	char line[20];
	fgets(line, 20, in_file);
	while (line[0] == '#')
		fgets(line, 20, in_file);
	if (line[1] == '2') {
		pgm_img->type = 2;
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		char *aux = strtok(line, DELIM);
		pgm_img->width = atoi(aux);
		aux = strtok(NULL, DELIM);
		pgm_img->height = atoi(aux);
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		aux = strtok(line, DELIM);
		pgm_img->maxval = atoi(aux);
		pgm_img->info_start = ftell(in_file);
		read_pgm(pgm_img, filename);
	} else if (line[1] == '3') {
		ppm_img->type = 3;
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		char *aux = strtok(line, DELIM);
		ppm_img->width = atoi(aux);
		aux = strtok(NULL, DELIM);
		ppm_img->height = atoi(aux);
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		aux = strtok(line, DELIM);
		ppm_img->maxval = atoi(aux);
		ppm_img->info_start = ftell(in_file);
		read_ppm(ppm_img, filename);
	} else if (line[1] == '5') {
		pgm_img->type = 5;
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		char *aux = strtok(line, DELIM);
		pgm_img->width = atoi(aux);
		aux = strtok(NULL, DELIM);
		pgm_img->height = atoi(aux);
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		aux = strtok(line, DELIM);
		pgm_img->maxval = atoi(aux);
		pgm_img->info_start = ftell(in_file);
		read_pgm(pgm_img, filename);
	} else if (line[1] == '6') {
		ppm_img->type = 6;
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		char *aux = strtok(line, DELIM);
		ppm_img->width = atoi(aux);
		aux = strtok(NULL, DELIM);
		ppm_img->height = atoi(aux);
		fgets(line, 20, in_file);
		while (line[0] == '#')
			fgets(line, 20, in_file);
		aux = strtok(line, DELIM);
		ppm_img->maxval = atoi(aux);
		ppm_img->info_start = ftell(in_file);
		read_ppm(ppm_img, filename);
	}
	printf("Loaded %s\n", filename);
	fclose(in_file);
}

void swap(int *a, int *b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

int valid(int x1, int y1, int x2, int y2, pgm *pgm_img, ppm *ppm_img)
{
	int width, height;
	if (pgm_img->is_loaded) {
		width = pgm_img->width;
		height = pgm_img->height;
	} else {
		width = ppm_img->width;
		height = ppm_img->height;
	}
	if ((y1 == y2 + 1) || y1 < 0 || (y2 > height - 1))
		return 0;
	if ((x1 == x2 + 1) || x1 < 0 || (x2 > width - 1))
		return 0;
	return 1;
}

void save_txt(pgm *pgm_img, ppm *ppm_img, char *filename)
{
	FILE *out_file = fopen(filename, "wt");
	if (pgm_img->is_loaded) {
		if (pgm_img->type == 2 || pgm_img->type == 5)
			fprintf(out_file, "P2\n");
		fprintf(out_file, "%d %d\n", pgm_img->width, pgm_img->height);
		fprintf(out_file, "%d\n", pgm_img->maxval);
		for (int i = 0; i < pgm_img->height; i++) {
			for (int j = 0; j < pgm_img->width; j++)
				fprintf(out_file, "%d ", pgm_img->imag[i][j]);
			fprintf(out_file, "\n");
		}
	} else if (ppm_img->is_loaded) {
		if (ppm_img->type == 3 || ppm_img->type == 6)
			fprintf(out_file, "P3\n");
		fprintf(out_file, "%d %d\n", ppm_img->width, ppm_img->height);
		fprintf(out_file, "%d\n", ppm_img->maxval);
		for (int i = 0; i < ppm_img->height; i++) {
			for (int j = 0; j < ppm_img->width; j++) {
				fprintf(out_file, "%d ", ppm_img->imag[i][j].R);
				fprintf(out_file, "%d ", ppm_img->imag[i][j].G);
				fprintf(out_file, "%d ", ppm_img->imag[i][j].B);
			}
			fprintf(out_file, "\n");
		}
	} else {
		printf("No image loaded\n");
	}
	fclose(out_file);
}

void save_binary(pgm *pgm_img, ppm *ppm_img, char *filename)
{
	FILE *out = fopen(filename, "wb");
	if (pgm_img->is_loaded) {
		if (pgm_img->type == 2 || pgm_img->type == 5)
			fprintf(out, "P5\n");
		fprintf(out, "%d %d\n", pgm_img->width, pgm_img->height);
		fprintf(out, "%d\n", pgm_img->maxval);
		for (int i = 0; i < pgm_img->height; i++) {
			for (int j = 0; j < pgm_img->width; j++)
				fwrite(&pgm_img->imag[i][j], sizeof(unsigned char), 1, out);
		}
	} else if (ppm_img->is_loaded) {
		if (ppm_img->type == 3 || ppm_img->type == 6)
			fprintf(out, "P6\n");
		fprintf(out, "%d %d\n", ppm_img->width, ppm_img->height);
		fprintf(out, "%d\n", ppm_img->maxval);
		for (int i = 0; i < ppm_img->height; i++) {
			for (int j = 0; j < ppm_img->width; j++)
				fwrite(&ppm_img->imag[i][j], sizeof(pixel), 1, out);
		}
	} else {
		printf("No image loaded\n");
	}
	fclose(out);
}

void save(pgm *pgm_img, ppm *ppm_img)
{
	if (pgm_img->is_loaded == 0 && ppm_img->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	char *filename = strtok(NULL, DELIM);
	char *arg;
	arg = strtok(NULL, DELIM);
	if (!arg) {
		save_binary(pgm_img, ppm_img, filename);
		printf("Saved %s\n", filename);
	} else if (strcmp(arg, "ascii") == 0) {
		save_txt(pgm_img, ppm_img, filename);
		printf("Saved %s\n", filename);
	}
}

void select_all(pgm *pgm_img, ppm *ppm_img, int *select_type,
				int *x1, int *y1, int *x2, int *y2)
{
	if (pgm_img->is_loaded) {
		*x1 = 0; *y1 = 0;
		*x2 = pgm_img->width - 1;
		*y2 = pgm_img->height - 1;
		*select_type = 1;
	} else if (ppm_img->is_loaded) {
		*x1 = 0; *y1 = 0;
		*x2 = ppm_img->width - 1;
		*y2 = ppm_img->height - 1;
		*select_type = 1;
	}
	printf("Selected ALL\n");
}

void read_coords(int *x1_cpy, int *y1_cpy, int *x2_cpy, int *y2_cpy,
				 int *is_nr, char *arg)
{
	if (!arg)
		*is_nr = 0;
	else
		*is_nr = is_number(arg);
	if (*is_nr == 1 && arg)
		*x1_cpy = atoi(arg);
	else
		*is_nr = 0;
	if (is_nr) {
		arg = strtok(NULL, DELIM);
		if (!arg)
			*is_nr = 0;
		else
			*is_nr = is_number(arg);
		if (*is_nr == 1 && arg)
			*y1_cpy = atoi(arg);
		else
			*is_nr = 0;
	}
	if (*is_nr) {
		arg = strtok(NULL, DELIM);
		if (!arg)
			*is_nr = 0;
		else
			*is_nr = is_number(arg);
		if (*is_nr == 1 && arg)
			*x2_cpy = atoi(arg);
		else
			*is_nr = 0;
	}
	if (*is_nr) {
		arg = strtok(NULL, DELIM);
		if (!arg)
			*is_nr = 0;
		else
			*is_nr = is_number(arg);
		if (*is_nr == 1 && arg)
			*y2_cpy = atoi(arg);
		else
			*is_nr = 0;
	}
}

void selectt(pgm *pgm_img, ppm *ppm_img, int *select_type,
			 int *x1, int *y1, int *x2, int *y2)
{
	char *arg;
	if (pgm_img->is_loaded || ppm_img->is_loaded) {
		arg = strtok(NULL, DELIM);
		if (strcmp(arg, "ALL") == 0) {
			select_all(pgm_img, ppm_img, select_type, x1, y1, x2, y2);
		} else {
			int x1_cpy, x2_cpy, y1_cpy, y2_cpy, v, is_nr;
			read_coords(&x1_cpy, &y1_cpy, &x2_cpy, &y2_cpy, &is_nr, arg);
			if (is_nr == 1) {
				if (x1_cpy == x2_cpy || y1_cpy == y2_cpy) {
					v = 0;
				} else {
					if (x1_cpy > x2_cpy)
						swap(&x1_cpy, &x2_cpy);
					if (y1_cpy > y2_cpy)
						swap(&y1_cpy, &y2_cpy);
					x2_cpy = x2_cpy - 1;
					y2_cpy = y2_cpy - 1;
					v = valid(x1_cpy, y1_cpy, x2_cpy, y2_cpy, pgm_img, ppm_img);
				}
				if (v == 1) {
					*x1 = x1_cpy;
					*y1 = y1_cpy;
					*x2 = x2_cpy;
					*y2 = y2_cpy;
					printf("Selected %d %d ", *x1, *y1);
					printf("%d %d\n", *x2 + 1, *y2 + 1);
					*select_type = 0;
				} else {
					printf("Invalid set of coordinates\n");
				}
			} else {
				printf("Invalid command\n");
			}
		}
	} else {
		printf("No image loaded\n");
	}
}

void rotate(int x1, int y1, int x2, int y2, pgm *pgm_img, ppm *ppm_img)
{
	int n = x2 - x1 + 1;
	int m = y2 - y1 + 1;
	if (n == m) {
		if (pgm_img->is_loaded) {
			for (int i = 0; i < n / 2; i++) {
				for (int j = i; j < n - i - 1; j++) {
					int aux;
					int temp = pgm_img->imag[y1 + i][x1 + j];
					aux = pgm_img->imag[y1 + j][x1 + n - 1 - i];
					pgm_img->imag[y1 + i][x1 + j] = aux;
					aux = pgm_img->imag[y1 + n - 1 - i][x1 + n - 1 - j];
					pgm_img->imag[y1 + j][x1 + n - 1 - i] = aux;
					aux = pgm_img->imag[y1 + n - 1 - j][x1 + i];
					pgm_img->imag[y1 + n - 1 - i][x1 + n - 1 - j] = aux;
					pgm_img->imag[y1 + n - 1 - j][x1 + i] = temp;
				}
			}
		}
		if (ppm_img->is_loaded) {
			for (int i = 0; i < n / 2; i++) {
				for (int j = i; j < n - i - 1; j++) {
					pixel aux;
					pixel temp = ppm_img->imag[y1 + i][x1 + j];
					aux = ppm_img->imag[y1 + j][x1 + n - 1 - i];
					ppm_img->imag[y1 + i][x1 + j] = aux;
					aux = ppm_img->imag[y1 + n - 1 - i][x1 + n - 1 - j];
					ppm_img->imag[y1 + j][x1 + n - 1 - i] = aux;
					aux = ppm_img->imag[y1 + n - 1 - j][x1 + i];
					ppm_img->imag[y1 + n - 1 - i][x1 + n - 1 - j] = aux;
					ppm_img->imag[y1 + n - 1 - j][x1 + i] = temp;
				}
			}
		}
	}
}

void rotate_all(pgm *pgm_img, ppm *ppm_img)
{
	if (pgm_img->is_loaded) {
		//rotate pgm
		unsigned char **mat;
		mat = malloc(pgm_img->width * sizeof(unsigned char *));
		for (int i = 0; i < pgm_img->width; i++)
			mat[i] = malloc(pgm_img->height * sizeof(unsigned char));
		for (int i = 0; i < pgm_img->height; i++) {
			for (int j = 0; j < pgm_img->width; j++)
				mat[pgm_img->width - 1 - j][i] = pgm_img->imag[i][j];
		}
		free_matrix(pgm_img, ppm_img);
		pgm_img->imag = mat;

	} else if (ppm_img->is_loaded) {
		//rotate ppm
		pixel **mat2;
		mat2 = malloc(ppm_img->width * sizeof(pixel *));
		for (int i = 0; i < ppm_img->width; i++)
			mat2[i] = malloc(ppm_img->height * sizeof(pixel));
		for (int i = 0; i < ppm_img->height; i++) {
			for (int j = 0; j < ppm_img->width; j++)
				mat2[ppm_img->width - 1 - j][i] = ppm_img->imag[i][j];
		}
		free_matrix(pgm_img, ppm_img);
		ppm_img->imag = mat2;
	}
}

void rotate_manage(pgm *pgm_img, ppm *ppm_img, int *select_type,
				   int *x1, int *y1, int *x2, int *y2)
{	int angle;
	char *arg;
	if (pgm_img->is_loaded || ppm_img->is_loaded) {
		arg = strtok(NULL, DELIM);
		angle = atoi(arg);
		int contor;
		int supported_angle = 1;
		if (angle == -90 || angle == 270)
			contor = 1;
		else if (angle == -180 || angle == 180)
			contor = 2;
		else if (angle == -270 || angle == 90)
			contor = 3;
		else if (angle == -360 || angle == 360 || angle == 0)
			contor = 0;
		else
			supported_angle = 0;
		if (pgm_img->is_loaded)
			if (*x1 * *y1 == 0 && *x2 == pgm_img->width - 1)
				if (*y2 == pgm_img->height - 1)
					*select_type = 1;
		if (ppm_img->is_loaded)
			if (*x1 * *y1 == 0 && *x2 == ppm_img->width - 1)
				if (*y2 == ppm_img->height - 1)
					*select_type = 1;
		if (supported_angle == 1) {
			if (*select_type == 1) {
				for (int i = 0; i < contor; i++) {
					rotate_all(pgm_img, ppm_img);
					if (pgm_img->is_loaded) {
						int aux;
						aux = pgm_img->height;
						pgm_img->height = pgm_img->width;
						pgm_img->width = aux;
					}
					if (ppm_img->is_loaded) {
						int aux;
						aux = ppm_img->height;
						ppm_img->height = ppm_img->width;
						ppm_img->width = aux;
					}
					*x1 = 0; *y1 = 0;
					if (pgm_img->is_loaded) {
						*x2 = pgm_img->width - 1;
						*y2 = pgm_img->height - 1;
					}
					if (ppm_img->is_loaded) {
						*x2 = ppm_img->width - 1;
						*y2 = ppm_img->height - 1;
					}
				}
				printf("Rotated %d\n", angle);
			}
			if (*select_type == 0) {
				if (*x2 - *x1 + 1 == *y2 - *y1 + 1) {
					for (int i = 0; i < contor; i++)
						rotate(*x1, *y1, *x2, *y2, pgm_img, ppm_img);
					printf("Rotated %d\n", angle);
				} else {
					printf("The selection must be square\n");
				}
			}
		} else {
			printf("Unsupported rotation angle\n");
		}
	} else {
		printf("No image loaded\n");
	}
}

void grayscale(pgm *pgm_img, ppm *ppm_img, int x1, int y1, int x2, int y2)
{
	if (pgm_img->is_loaded == 0 && ppm_img->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	if (ppm_img->is_loaded == 0) {
		printf("Grayscale filter not available\n");
		return;
	}

	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++) {
			double aux = ppm_img->imag[i][j].G + ppm_img->imag[i][j].B;
			double sum = (ppm_img->imag[i][j].R + aux) / 3;

			ppm_img->imag[i][j].R = min(round(sum), ppm_img->maxval);
			ppm_img->imag[i][j].G = min(round(sum), ppm_img->maxval);
			ppm_img->imag[i][j].B = min(round(sum), ppm_img->maxval);
		}
	printf("Grayscale filter applied\n");
}

void sepia(pgm *pgm_img, ppm *ppm_img, int x1, int y1, int x2, int y2)
{
	if (pgm_img->is_loaded == 0 && ppm_img->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}

	if (ppm_img->is_loaded == 0) {
		printf("Sepia filter not available\n");
		return;
	}

	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++) {
			double red, green, blue, aux;
			aux = 0.769 * ppm_img->imag[i][j].G + 0.189 * ppm_img->imag[i][j].B;
			red = 0.393 * ppm_img->imag[i][j].R + aux;
			aux = 0.686 * ppm_img->imag[i][j].G + 0.168 * ppm_img->imag[i][j].B;
			green = 0.349 * ppm_img->imag[i][j].R + aux;
			aux = 0.534 * ppm_img->imag[i][j].G + 0.131 * ppm_img->imag[i][j].B;
			blue = 0.272 * ppm_img->imag[i][j].R + aux;

			ppm_img->imag[i][j].R = min(round(red), ppm_img->maxval);
			ppm_img->imag[i][j].G = min(round(green), ppm_img->maxval);
			ppm_img->imag[i][j].B = min(round(blue), ppm_img->maxval);
		}
	printf("Sepia filter applied\n");
}

void crop(pgm *pgm_img, ppm *ppm_img, int x1, int y1, int x2, int y2)
{
	if (pgm_img->is_loaded == 0 && ppm_img->is_loaded == 0) {
		printf("No image loaded\n");
		return;
	}
	if (pgm_img->is_loaded) {
		unsigned char **mat;
		int height, width;
		height = y2 - y1 + 1;
		width = x2 - x1 + 1;

		mat = malloc(height * sizeof(unsigned char *));
		for (int i = 0; i < height; i++)
			mat[i] = malloc(width * sizeof(unsigned char));

		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				mat[i][j] = pgm_img->imag[y1 + i][x1 + j];
		free_matrix(pgm_img, ppm_img);
		pgm_img->imag = mat;
		pgm_img->height = height;
		pgm_img->width = width;
	} else if (ppm_img->is_loaded) {
		pixel **mat2;
		int height, width;
		height = y2 - y1 + 1;
		width = x2 - x1 + 1;

		mat2 = malloc(height * sizeof(pixel *));
		for (int i = 0; i < height; i++)
			mat2[i] = malloc(width * sizeof(pixel));
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
				mat2[i][j] = ppm_img->imag[y1 + i][x1 + j];
		free_matrix(pgm_img, ppm_img);
		ppm_img->imag = mat2;
		ppm_img->height = height;
		ppm_img->width = width;
	}
	printf("Image cropped\n");
}

int main(void)
{
	pgm pgm_img;
	ppm ppm_img;
	init(&pgm_img, &ppm_img);
	int x1, x2, y1, y2;
	int select_type = -1; //1 = pt select all, 0 = pt select dimensiuni
	while (1) {
		char *arg;
		char command[100];
		fgets(command, 100, stdin);
		arg = strtok(command, DELIM);
		if (strcmp(arg, "LOAD") == 0) {
			pgm_img.is_loaded = 0;
			ppm_img.is_loaded = 0;
			load_image(&pgm_img, &ppm_img);
			if (pgm_img.is_loaded) {
				x1 = 0; y1 = 0;
				x2 = pgm_img.width - 1;
				y2 = pgm_img.height - 1;
				select_type = 1;
			} else if (ppm_img.is_loaded) {
				x1 = 0; y1 = 0;
				x2 = ppm_img.width - 1;
				y2 = ppm_img.height - 1;
				select_type = 1;
			}
		} else if (strcmp(arg, "SAVE") == 0) {
			save(&pgm_img, &ppm_img);
		} else if (strcmp(arg, "SELECT") == 0) {
			selectt(&pgm_img, &ppm_img, &select_type, &x1, &y1, &x2, &y2);
		} else if (strcmp(arg, "CROP") == 0) {
			crop(&pgm_img, &ppm_img, x1, y1, x2, y2);
			if (pgm_img.is_loaded) {
				x1 = 0; y1 = 0;
				x2 = pgm_img.width - 1;
				y2 = pgm_img.height - 1;
				select_type = 1;
			} else if (ppm_img.is_loaded) {
				x1 = 0; y1 = 0;
				x2 = ppm_img.width - 1;
				y2 = ppm_img.height - 1;
				select_type = 1;
			}
		} else if (strcmp(arg, "SEPIA") == 0) {
			sepia(&pgm_img, &ppm_img, y1, x1, y2, x2);
		} else if (strcmp(arg, "GRAYSCALE") == 0) {
			grayscale(&pgm_img, &ppm_img, y1, x1, y2, x2);
		} else if (strcmp(arg, "ROTATE") == 0) {
			rotate_manage(&pgm_img, &ppm_img, &select_type, &x1, &y1, &x2, &y2);
		} else if (strcmp(arg, "EXIT") == 0) {
			if (pgm_img.is_loaded || ppm_img.is_loaded)
				free_matrix(&pgm_img, &ppm_img);
			if (pgm_img.is_loaded == 0 && ppm_img.is_loaded == 0)
				printf("No image loaded\n");
			exit(0);
		} else {
			printf("Invalid command\n");
		}
	}

	return 0;
}
