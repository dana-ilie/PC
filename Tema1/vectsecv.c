#include <stdio.h>

int descrescator(int N);
int crescator(int N);

int main(void)
{
	int N, se_repeta = 0, v[100] = {[0 ... 99] = -1}, dif;
	int pozitie_gasit, n = 1;
	int gasit_in_vector;

	scanf("%d", &N);
	while (se_repeta == 0) {
		dif = descrescator(N) - crescator(N);
		gasit_in_vector = 0;
		for (int i = 0; i < n; i++) {
			if (v[i] == dif) {
				gasit_in_vector = 1;
				pozitie_gasit = i;
			}
		}
		if (gasit_in_vector == 0) {
			v[n - 1] = dif;
			n++;
			N = dif;
		} else {
			se_repeta = 1;
		}
	}
	printf("%d\n", pozitie_gasit);
	for (int i = pozitie_gasit; i < n - 1; i++)
		printf("%d ", v[i]);
	printf("\n");

	return 0;
}

int descrescator(int N)
{
	int v[100], i = 0, aux, n = 0, ordonat;
	while (N != 0) {
		v[i] = N % 10;
		i++;
		n++;
		N = N / 10;
	}
	do {
		ordonat = 1;
		for (i = 0; i < n - 1; i++)
			if (v[i] < v[i + 1]) {
				aux = v[i]; v[i] = v[i + 1]; v[i + 1] = aux;
				ordonat = 0;
			}

	} while (ordonat == 0);
	for (i = 0; i < n; i++)
		N = N * 10 + v[i];
	return N;
}

int crescator(int N)
{
	int v[100], i = 0, aux, n = 0, ordonat;
	while (N != 0) {
		v[i] = N % 10;
		i++;
		n++;
		N = N / 10;
	}
	do {
		ordonat = 1;
		for (i = 0; i < n - 1; i++)
			if (v[i] > v[i + 1]) {
				aux = v[i]; v[i] = v[i + 1]; v[i + 1] = aux;
				ordonat = 0;
			}

	} while (ordonat == 0);
	for (i = 0; i < n; i++)
		N = N * 10 + v[i];
	return N;
}
