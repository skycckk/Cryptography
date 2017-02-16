#include <stdlib.h>
#include <stdio.h>

const int KEY_LENGTH = 7;
unsigned char key[KEY_LENGTH] = {0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F, 0x77};
void RC4Initialization(unsigned char sbox[16][16]);
void RC4KeystreamGeneration(unsigned char sbox[16][16], unsigned char *key_steram, const int count);
void Swap(unsigned char *a, unsigned char *b);
void PrintSbox(unsigned char sbox[16][16]);

int main (int argc, const char *argv[])
{
	if (argc <= 1)
		return 0;

	int key_stream_len = atoi(argv[1]);
	unsigned char sbox[16][16] = {0};
	int s_size = 256;

	printf("----after initialization----\n");
	RC4Initialization(sbox);
	PrintSbox(sbox);
	
	printf("----after %d bytes generation----\n", key_stream_len);
	unsigned char *key_steram = (unsigned char*) malloc(key_stream_len);
	RC4KeystreamGeneration(sbox, key_steram, key_stream_len);
	PrintSbox(sbox);

	free(key_steram);

	return 0;
}

void PrintSbox(unsigned char sbox[16][16])
{
	printf("   ");
	for (int i = 0; i < 16; i++)
		printf("  %02d ", i);

	printf("\n");

	for (int i = 0; i < 16; i++)
	{
		printf("%02d ", i);
		for (int j = 0; j < 16; j++)
			printf("0x%02x ", sbox[i][j]);
		printf("\n");
	}
}

void RC4Initialization(unsigned char sbox[16][16])
{
	for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
			sbox[i][j] = i * 16 + j;
	}

	int j = 0;
	for (int i = 0; i < 256; i++)
	{
		int si = (int)(i / 16);
		int sj = (int)(i % 16);
		j = (j + sbox[si][sj] + key[i % KEY_LENGTH]) % 256;

		int si2 = (int)(j / 16);
		int sj2 = (int)(j % 16);
		Swap(&sbox[si][sj], &sbox[si2][sj2]);
	}

	return;
}

void RC4KeystreamGeneration(unsigned char sbox[16][16], unsigned char *key_steram, int count)
{
	int i = 0;
	int j = 0;

	while (count > 0)
	{
		i = (i + 1) % 256;
		int si = (int)(i / 16);
		int sj = (int)(i % 16);

		j = (j + sbox[si][sj]) % 256;
		int si2 = (int)(j / 16);
		int sj2 = (int)(j % 16);

		Swap(&sbox[si][sj], &sbox[si2][sj2]);

		int k = (sbox[si][sj] + sbox[si2][sj2]) % 256;
		si = (int)(k / 16);
		sj = (int)(k % 16);

		key_steram[count] = sbox[si][sj];
		count--;
	}
}

void Swap(unsigned char *a, unsigned char *b)
{
	unsigned char tmp = *a;
	*a = *b;
	*b = tmp;
}