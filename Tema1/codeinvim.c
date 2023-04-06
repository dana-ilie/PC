#include <stdio.h>

int main(void)
{
	int n, x[100], c[100], p[100], p_min, p_total = 0;
	int m = -1;
	int p_max, poz_max;

	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf("%d", &x[i]);
	for (int i = 0; i < n; i++) {
		scanf("%d", &c[i]);
		p[i] = c[i] * (10 - x[i]);
		p_total = p_total + x[i] * c[i];
	}
	scanf("%d", &p_min);
	if (p_total < p_min) {
		m = 0;
		while (p_total < p_min && p_max != 0) {
			p_max = 0;
			for (int i = 0; i < n; i++) {
				if (p[i] >= p_max) {
					p_max = p[i];
					poz_max = i;
				}
			}
			p_total = p_total + p_max;
			p[poz_max] = 0;
			m++;
		}
	}
	if (p_max == 0)
		m = -1;
	printf("%d\n", m);

	return 0;
}
