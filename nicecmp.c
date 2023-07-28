#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define LINELEN (80)

char *mygets(char *buf, int len);
int mygeti();

int main(int argc, char *argv[])
{
	char *cmpstr[] = {"lexcmp", "lencmp"};
	int veclen = sizeof(cmpstr)/sizeof(char *);
	char str1[LINELEN + 1];
	char str2[LINELEN + 1];
	char fromchild[LINELEN];
	int index;
	
	int pfdToChildLen[2];
	int pfdToChildLex[2];
	int pfdFromChild[2];
	int result;
	int size1,size2;
	
	if(pipe(pfdToChildLen) == -1)
		return -2;
		
	if(pipe(pfdToChildLex) == -1)
		return -2;
		
	if(pipe(pfdFromChild) == -1)
		return -2;
		
	switch(fork())
	{
		case -1:
			return -2;		
		case 0:		
			switch(fork())
			{
				case -1:
					return -2;
				case 0:
					if(close(pfdToChildLex[1]) == -1)
						return -2;
			
					if(pfdToChildLex[0] != STDIN_FILENO)
					{
						if(dup2(pfdToChildLex[0], STDIN_FILENO) == -1)
							return -2;
						if(close(pfdToChildLex[0]) == -1)
							return -2;
					}
			
					if(close(pfdFromChild[0]) == -1)
						return -2;
			
					if(pfdFromChild[1] != STDOUT_FILENO)
					{
						if(dup2(pfdFromChild[1], STDOUT_FILENO) == -1)
							return -2;
						if(close(pfdFromChild[1]) == -1)
							return -2;
					}
					char* args2[3] = {"./loopcmp",cmpstr[0],NULL};
					execvp(args2[0],args2);
					return -2;
				default:
				
					if(close(pfdToChildLen[1]) == -1)
						return -2;
			
					if(pfdToChildLen[0] != STDIN_FILENO)
					{
						if(dup2(pfdToChildLen[0], STDIN_FILENO) == -1)
							return -2;
						if(close(pfdToChildLen[0]) == -1)
							return -2;
					}
			
					if(close(pfdFromChild[0]) == -1)
						return -2;
					
					if(pfdFromChild[1] != STDOUT_FILENO)
					{
						if(dup2(pfdFromChild[1], STDOUT_FILENO) == -1)
							return -2;
						if(close(pfdFromChild[1]) == -1)
							return -2;
					}
					char* args1[3] = {"./loopcmp",cmpstr[1],NULL};
					execvp(args1[0],args1);
					return -2;
			}
		default:
			if(close(pfdToChildLen[0]) == -1)
				return -2;
			if(close(pfdToChildLex[0]) == -1)
				return -2;
			if(close(pfdFromChild[1]) == -1)
				return -2;
				
			while (1)
			{
				printf("Please enter first string:\n");
				if (mygets(str1, LINELEN) == NULL)
					break;
				size1 = strlen(str1)+1;
				strcat(str1,"\n");
				printf("Please enter second string:\n");
				if (mygets(str2, LINELEN) == NULL)
					break;
				size2 = strlen(str2)+1;
				strcat(str2,"\n");
				do {
					printf("Please choose:\n");
					for (int i=0 ; i < veclen ; i++)
						printf("%d - %s\n", i, cmpstr[i]);
					index = mygeti();
				} while ((index < 0) || (index >= veclen));
				if(index == 0)
				{
					write(pfdToChildLex[1],str1,size1);
					write(pfdToChildLex[1],str2,size2);
				}
				else
				{
					write(pfdToChildLen[1],str1,size1);
					write(pfdToChildLen[1],str2,size2);
				}
				read(pfdFromChild[0],fromchild,sizeof(fromchild));
				result = atoi(fromchild);
				printf("result: %d\n", result);
				fflush(stdin);
			}
		}
	if(close(pfdToChildLen[1]) == -1)
		return -2;
	if(close(pfdToChildLex[1]) == -1)
		return -2;
	if(close(pfdFromChild[0]) == -1)
		return -2;
	return 0;
}

char *mygets(char *buf, int len)
{
	char *retval;

	retval = fgets(buf, len, stdin);
	buf[len] = '\0';
	if (buf[strlen(buf) - 1] == 10) /* trim \r */
		buf[strlen(buf) - 1] = '\0';
	else if (retval) 
		while (getchar() != '\n'); /* get to eol */

	return retval;
}

int mygeti()
{
	int ch;
	int retval=0;

	while(isspace(ch=getchar()));
	while(isdigit(ch))
	{
		retval = retval * 10 + ch - '0';
		ch = getchar();
	}
	while (ch != '\n')
		ch = getchar();
	return retval;
}


























