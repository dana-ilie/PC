#include <stdio.h>

int verif_puzzle(void)
{
	int N, M;
	int restrict_lin[100][100], restrict_col[100][100], nonogram[100][100];
	int K;
	int S1[100] = {0}, S2[100] = {0}, s1[100] = {0}, s2[100] = {0};
	scanf("%d", &N);
		scanf("%d", &M);
		//citim matricea restrictiilor liniilor
		for (int i = 0; i < N; i++) {
			scanf("%d", &K);
			for (int j = 0; j < K; j++) {
				scanf("%d", &restrict_lin[i][j]);
				s1[i] = s1[i] + restrict_lin[i][j];
			}
		}
		//citim matricea restrictiilor coloanelor
		for (int i = 0; i < M; i++) {
			scanf("%d", &K);
			for (int j = 0; j < K; j++) {
				scanf("%d", &restrict_col[i][j]);
				s2[i] = s2[i] + restrict_col[i][j];
			}
		}
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				scanf("%d", &nonogram[i][j]);

		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
				S1[i] = S1[i] + nonogram[i][j];
		for (int j = 0; j < M; j++)
			for (int i = 0; i < N; i++)
				S2[j] = S2[j] + nonogram[i][j];
		for (int i = 0; i < N; i++)
			if (S1[i] != s1[i])
				return 0;
		for (int i = 0; i < M; i++)
			if (S2[i] != s2[i])
				return 0;
		return 1;
}

int main(void)
{
	int T;
	scanf("%d", &T);
	for (int i = 0; i < T; i++) {
		if (verif_puzzle())
			printf("Corect\n");
		else
			printf("Eroare\n");
	}

	return 0;
}
