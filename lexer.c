#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct Variable
{
	char *name;
	int id;
};

struct Variable variables[100];
char allVars[999];
int allVarsIndex = -1;
int varIndex = -1;

int areEqualStrings(char str1[], char str2[])
{
    int ctr=0;
    while(str1[ctr]==str2[ctr])
    {
        if(str1[ctr]=='\0'||str2[ctr]=='\0')
            break;
        ctr++;
    }
    if(str1[ctr]=='\0' && str2[ctr]=='\0')
        return 1;
    return 0;
}

int reservedWord(char token[])
{
	if (areEqualStrings(token, "array"))
		return 1;
	if (areEqualStrings(token, "boolean"))
		return 2;
	if (areEqualStrings(token, "char"))
		return 3;
	if (areEqualStrings(token, "else"))
		return 4;
	if (areEqualStrings(token, "false"))
		return 5;
	if (areEqualStrings(token, "for"))
		return 6;
	if (areEqualStrings(token, "function"))
		return 7;
	if (areEqualStrings(token, "if"))
		return 8;
	if (areEqualStrings(token, "integer"))
		return 9;
	if (areEqualStrings(token, "print"))
		return 10;
	if (areEqualStrings(token, "return"))
		return 11;
	if (areEqualStrings(token, "string"))
		return 12;
	if (areEqualStrings(token, "true"))
		return 13;
	if (areEqualStrings(token, "void"))
		return 14;
	if (areEqualStrings(token, "while"))
		return 15;
	return 0;
}

int getId(char* s, int* savedCounter) {
	for (int i = 0; i <= varIndex; i++)
		if (areEqualStrings(variables[i].name, s))
			return variables[i].id;

	varIndex = varIndex + 1;
	*savedCounter = *savedCounter + 1;
	variables[varIndex].id = *savedCounter;
	variables[varIndex].name = &allVars[allVarsIndex + 1];
	allVarsIndex = allVarsIndex + strlen(s) + 1;
	strcpy(variables[varIndex].name, s);
	return *savedCounter;
}


int main()
{
	int isComment = 0;
	char line[1000];
	FILE *file = fopen("/home/sahar/workspace/compiler/test/test2.bminor", "r");
	while (fgets(line, 1000, file) != NULL)
	{
		char word[128];
		static int savedCounter = 0;
		if (line[0] == '/' && line[1] == '/') // dont read the comments
			continue;
		for (int i = 0; i < strlen(line); i++) // get a new line
		{
			if (isComment)
			{
				while (1)
				{
					if (i < strlen(line) - 1)
					{
						if(line[i] == '*' && line[i+1]=='/')
						{
							i+=2;
							isComment = 0;
							break;
						}
					}
					if (i + 1 >= strlen(line)-1)
					{
						break;
					}
					i++;
				}
			}
			if (isalpha(line[i])) // if it's a word
			{
				// parse the word
				int wordIndex = 0;
				word[wordIndex] = line[i];
				while (isalnum(line[i + 1]))
				{
					wordIndex++;
					i++;
					word[wordIndex] = line[i];
				}
				word[wordIndex + 1] = '\0';
				int id = reservedWord(word); // check if word is reserved

				if (!id) // if word is not reserved
				{
					int savedId = getId(word,&savedCounter);
					printf("ID %d //%s \n", savedId , word);
				}
				else // if word is reserved
					printf("KEYWORD %d\n", id);
			}
			else if (isdigit(line[i])) // it's a num
			{
				int error = 0;
				int numIndex = 0;
				word[numIndex] = line[i];
				while (isdigit(line[i + 1]))
				{
					numIndex++;
					i++;
					word[numIndex] = line[i];
				}
				if (isalpha(line[i+1])){
					error = 1;
					printf("ERROR: Invalid input\n");
					while (isalnum(line[i+1]))
					{
						i++;
					}
				}
				if (!error) {
					word[numIndex + 1] = '\0';
					printf("%s NUM\n", word);
				}
			}
			else if (line[i] == '"') // it's a string
			{
				while (1)
				{
					i++;
					if (line[i] == '"')
						break;
				}
				printf("STRING 100\n");
			}
			else if(isspace(line[i])){} // it's a whitespace
			else if(line[i]=='/' && line[i+1]=='*')
			{
				while (1)
				{
					i++;
					if (i < strlen(line) - 1)
					{
						if(line[i] == '*' && line[i+1]=='/')
						{
							i++;
							break;
						}
						if (i+1 == strlen(line)-1)
						{
							isComment = 1;
							break;
						}
					}
				}
			}
			else //it's nothing
				printf("%c NONE\n", line[i]);
		}
	}
	fclose(file);
}
