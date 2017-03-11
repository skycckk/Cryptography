#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define DUMP_DEBUG

// from high-freq to low-freq
char monogram[26] = {'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 'l', 'u', 'c', 'm', 'w',
					 'f', 'g', 'y', 'p', 'b', 'v', 'k', 'j', 'x', 'q', 'z'}; 
int char_freq[26] = {0};
double char_bigram[26][26] = {{0}};

void CollectAlphaFreq(const char *str);
void SimpleSub(char *plaintext, const char *ciphertext, const int shift);
void SimpleSubWithMap(char *plaintext, const char *ciphertext, const char *map);
void InitializeBigram(const char *file_path);
char* ReadCiphertext(const char *file_path);
void InitKeyWithFreq(char *key, const char *ciphertext);
double ComputeFitnessUsingBigram(char *plaintext);
void ModifyKeyRandom(char *key);
void ModifyKeyPermutation(char *key, int start, int interval);

int main (int argc, const char *argv[])
{
	char *ciphertext = NULL;
	if (argc < 4)
	{
		printf("[Usage]: bigram.txt ciphertext.txt timeout\n");
		return 0;
	}
	// First arg: bigarm.txt
	const char *bigram_file_name = argv[1];
	InitializeBigram(bigram_file_name);

	// Second arg: ciphertext
	const char *ciphertext_file_name = argv[2];
	ciphertext = ReadCiphertext(ciphertext_file_name);

	// Third arg: timeout round
	const int TIME_OUT = atoi(argv[3]);
	
    char *plaintext = (char *)malloc(strlen(ciphertext) * sizeof(char));
    if (!plaintext) return 0;

    // step 1: initialize key
	char key[26] = "abcdefghijklmnopqrstuvwxyz";
	InitKeyWithFreq(key, ciphertext);
	char init_key[26];
	memcpy(init_key, key, 26 * sizeof(char));

	SimpleSubWithMap(plaintext, ciphertext, key);
	double present_score = ComputeFitnessUsingBigram(plaintext);
	char new_key[26];
	int time_out = 0;
	
	while (time_out < TIME_OUT)
	{
		if (time_out % 1000 == 0) printf("processing...%d/%d\n", time_out, TIME_OUT);
		int modified_index = 0;
		int modified_interval = 1;
		while (modified_interval < 26)
		{
		    // step 2: modify key
		    memcpy(new_key, key, 26 * sizeof(char));
		   	ModifyKeyPermutation(new_key, modified_index, modified_interval);
		   	modified_index++;
		   	if ((modified_index + modified_interval) >= 26)
		   	{
		   		modified_index = 0;
		   		modified_interval++;
		   	}

		    // step 3: compute fitness(score)
		    SimpleSubWithMap(plaintext, ciphertext, new_key);
		    double score = ComputeFitnessUsingBigram(plaintext);
		    if (score > present_score)
		    {
		    	memcpy(key, new_key, 26 * sizeof(char));
		    	present_score = score;
		    }
		}
		time_out++;
	}
	SimpleSubWithMap(plaintext, ciphertext, key);
	
	printf("------------------\n");
	printf("Init key: %s\n", init_key);
	printf("Finl Key: %s\n", key);
	printf("------------------\n");
	printf("max score: %f\n", present_score);

#ifdef DUMP_DEBUG
	for (int i = 0; i < 26; i++) printf("%*c ", 2,  i + 65);
	printf("\n");
	for (int i = 0; i < 26; i++) printf("%*d ", 2, char_freq[i]);
	printf("\n");
#endif

	printf("------------------\n");
	printf("%s\n", plaintext);
	printf("------------------\n");

	if (plaintext)
		free(plaintext);

	if (ciphertext)
		free(ciphertext);

	return 0;
}

void CollectAlphaFreq(const char *str)
{
	while (strcmp(str, "") != 0)
	{
		char token = toupper((int)*str);
		char_freq[(int)token - 'A']++;
		str++;
	}

	return;
}

void SimpleSub(char *plaintext, const char *ciphertext, const int shift)
{
	const int str_len = (unsigned)strlen(ciphertext);
	for (int i = 0; i < str_len; i++)
	{
		int new_ascii = (int)ciphertext[i] - shift;
		if (new_ascii < 65) new_ascii += 26;

		plaintext[i] = (char)tolower(new_ascii);
	}
}

void SimpleSubWithMap(char *plaintext, const char *ciphertext, const char *key)
{
	const int str_len = (unsigned)strlen(ciphertext);
	for (int i = 0; i < str_len; i++)
		plaintext[i] = key[(int)(ciphertext[i] - 'A')];
}

void InitKeyWithFreq(char *key, const char *ciphertext)
{
	CollectAlphaFreq(ciphertext);

	int char_freq_tmp[26];
	memcpy(char_freq_tmp, char_freq, 26 * sizeof(int));
	for (int i = 0; i < 26; i++)
	{
		int max_freq = -1;
		int max_j = 0;
		for (int j = 0; j < 26; j++)
		{
			if (char_freq_tmp[j] > max_freq)
			{
				max_freq = char_freq_tmp[j];
				max_j = j;
			}
		}
		char_freq_tmp[max_j] = -1;
		key[max_j] = monogram[i];
	}
}

void InitializeBigram(const char *file_path)
{
	FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(file_path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    long long count_sum = 0;
    while ((read = getline(&line, &len, fp)) != -1)
    {
        char *pch = strtok(line, " ");
        char *bi = pch;
        
        int bi_index0 = toupper(bi[0]) - 'A';
        int bi_index1 = toupper(bi[1]) - 'A';

        pch = strtok(NULL, " ");
        int count = atoi(pch);

		char_bigram[bi_index0][bi_index1] = count;
		count_sum += count;    
    }

    // compute log probability
    for (int i = 0; i < 26; i++)
    {
    	for (int j = 0; j < 26; j++)
    	{
    		char_bigram[i][j] = log(char_bigram[i][j] / count_sum);
    	}
    }

    fclose(fp);
    if (line)
        free(line);
}

char* ReadCiphertext(const char *file_path)
{
    FILE *fp = fopen(file_path, "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    int c;
    int len = 0;
    while ((c = fgetc(fp)) != EOF)
    {
        char new_c = (char)c;
        if ((new_c >= 'a' && new_c <= 'z') || (new_c >= 'A' && new_c <= 'Z'))
        	len++;
    }

    char *ctext = (char *)malloc(len * sizeof(char));
    rewind(fp);
    len = 0;
    while ((c = fgetc(fp)) != EOF)
    {
    	int new_c = (char)c;
    	if ((new_c >= 'a' && new_c <= 'z') || (new_c >= 'A' && new_c <= 'Z'))
    	{
    		ctext[len] = new_c;
    		len++;
    	}
    }

    fclose(fp);

    return ctext;
}

double ComputeFitnessUsingBigram(char *plaintext)
{
	double score = 0.0;
	while (strcmp(plaintext + 1, "") != 0)
	{
		char letter0 = toupper((int)*plaintext);
		char letter1 = toupper((int)*(plaintext + 1));

		int index0 = letter0 - 'A';
		int index1 = letter1 - 'A';

		score += char_bigram[index0][index1];
		plaintext++;
	}

	return score;
}

void ModifyKeyRandom(char *key)
{
	int a = rand() % 26;	
	int b = rand() % 26;

	char tmp = key[a];
	key[a] = key[b];
	key[b] = tmp;
}

void ModifyKeyPermutation(char *key, int start, int interval)
{
	char tmp = key[start];
	key[start] = key[start + interval];
	key[start + interval] = tmp;
}
