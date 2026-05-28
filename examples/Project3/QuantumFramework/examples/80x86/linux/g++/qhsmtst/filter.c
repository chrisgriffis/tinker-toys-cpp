#include <stdio.h>

char buffer[128];

int main()
{
	int i;
	for (i = 0; ; ++i)
	{
		char input;
		int j;
		
		if (fgets(buffer, sizeof buffer, stdin) == NULL)
		{
			break;
		}
		buffer[strlen(buffer)-1] = 0;
		input = buffer[0];
		printf("\tconst char * const results%02d[] =\n", i);
		printf("\t{\n");
		printf("\t\"");
		for (j = 0; j < strlen(buffer); ++j)
		{
			printf("%c", buffer[j]);
			if (buffer[j] == ':' || buffer[j] == ';')
			{
				printf("\"");
				if (j != strlen(buffer) - 1)
				{
					printf(", \"");
				}
			}
		}
		printf("\n\t};\n");
		printf("\tdispatch(%c_SIG);\n", input);
		printf("\tCHECK_RESULTS(results%02d)\n", i);
		printf("\n");
	}
	return 0;
}
