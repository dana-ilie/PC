#include <stdio.h>

int main(void)
{
	int N, xmax_impar = 0, xmin_par = 10000000, nr_copaci_speciali = 0;
	int indice_1 = 1;
	int x_copac_0, x_copac_1, x_copac_2;
	double m_a = 0;
	unsigned long s = 0;

	scanf("%d", &N);
	scanf("%d", &x_copac_0);
	scanf("%d", &x_copac_1);
	for (int i = 2; i < N; i++) {
		scanf("%d", &x_copac_2);
		if (x_copac_1 > x_copac_0 && x_copac_1 > x_copac_2) {
			s = s + x_copac_1;
			nr_copaci_speciali++;
			if (indice_1 % 2 == 0 && x_copac_1 < xmin_par)
				xmin_par = x_copac_1;
			if (indice_1 % 2 != 0 && x_copac_1 > xmax_impar)
				xmax_impar = x_copac_1;
		}
		x_copac_0 = x_copac_1;
		x_copac_1 = x_copac_2;
		indice_1++;
	}
	if (nr_copaci_speciali != 0)
		m_a = (double)s / nr_copaci_speciali;
	else
		xmin_par = 0;
	if (xmin_par == 10000000)
		xmin_par = -1;
	if (N < 3) {
		xmax_impar = 0;
		xmin_par = 0;
	}
	printf("%ld\n", s);
	printf("%.7f\n", m_a);
	printf("%d\n", xmax_impar);
	printf("%d\n", xmin_par);

	return 0;
}
