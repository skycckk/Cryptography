#include <stdlib.h>
#include <stdio.h>

const int BLOCK_SIZE = 8; // bytes
const int SKIP_BYTE = 10; // bytes
typedef unsigned char BYTE;

void Encrypt(uint32_t *v, uint32_t *u);
void Decrypt(uint32_t *v, uint32_t *k);

int main (int argc, const char *argv[])
{
	if (argc < 2)
	{
		printf("[Use]: 'input filename' 'output filename'\n");
		return 0;
	}

	FILE *fpi = fopen(argv[1], "rb");
	if (!fpi) return 0;

	FILE *fpo = fopen(argv[2], "wb");
	if (!fpo) return 0;

	fseek(fpi, 0, SEEK_END);
	int file_size = ftell(fpi);

	rewind(fpi);
	if (file_size < SKIP_BYTE) return 0;

	BYTE *orig_bmp = (BYTE *)malloc(file_size * sizeof(BYTE));
	int file_read = fread(orig_bmp, 1, file_size * sizeof(BYTE), fpi);
	if (file_read != file_size) return 0;
	
	// leave first 10 blocks unencrypted
	BYTE *bmp = orig_bmp + (SKIP_BYTE * BLOCK_SIZE);

	// use ECB block cipher mode
	int num_of_8_blocks = (file_size - SKIP_BYTE) / BLOCK_SIZE;
	uint32_t key[4] = {0x12341234, 0xab1287ce, 0x98778f71, 0x8fdcf18}; // random generated
	for (int i = 0; i < num_of_8_blocks; i++)
	{
		Encrypt((uint32_t *)(bmp + i * BLOCK_SIZE), (uint32_t *)key);
		// Decrypt((uint32_t *)(bmp + i * BLOCK_SIZE), (uint32_t *)key);
	}
	fwrite(orig_bmp, 1, file_size * sizeof(BYTE), fpo);

	free(orig_bmp);
	fclose(fpi);
	fclose(fpo);

	return 0;
}

void Encrypt(uint32_t *v, uint32_t *k)
{
    uint32_t L = v[0], R = v[1], sum = 0;
    uint32_t delta = 0x9e3779b9;
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (int i = 0; i < 32; i++) 
    {
        sum += delta;
        L += (((R << 4) + k0) ^ (R + sum) ^ ((R >> 5) + k1));
        R += (((L << 4) + k2) ^ (L + sum) ^ ((L >> 5) + k3));
    }
    v[0] = L; v[1] = R;
}

void Decrypt(uint32_t *v, uint32_t *k)
{
    uint32_t L = v[0], R = v[1];
    uint32_t delta = 0x9e3779b9;
    uint32_t sum = delta << 5;
    uint32_t k0 = k[0], k1 = k[1], k2 = k[2], k3 = k[3];
    for (int i = 0; i < 32; i++)
    {
        R -= (((L << 4) + k2) ^ (L + sum) ^ ((L >> 5) + k3));
        L -= (((R << 4) + k0) ^ (R + sum) ^ ((R >> 5) + k1));
        sum -= delta;
    }
    v[0] = L; v[1] = R;
}

