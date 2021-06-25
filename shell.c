#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

int cd(char* path)
{
    return chdir(path);
}

void printShell(char* arg)
{
    char output[500];
    char cwd[500];
    if(getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("Could not find dir");
    }
    strcpy(output, "MERX~");
    strcat(output, cwd);
    strcat(output, "#");
    printf("%s", output);
}

int main(void)
{
    char cmd[1024];
    char* argv[100];
    char* path="/bin/";
    char fullPath[20];
    int argc, firstRun = 0;
    char cdArg[100];
    
    while(1)
    {
        if(!firstRun)
	{
            printShell("");
	}else{
            printShell(cdArg);
	}

        if(!fgets(cmd, 1024, stdin)){
        break;                        
        }
	size_t length = strlen(cmd);
	if(cmd[length-1]=='\n')
	{
	   cmd[length-1] = '\0';
	}

	if(strcmp(cmd, "exit")== 0)
	{
	    return 0;
	}
        
        char* token;
        token = strtok(cmd, " ");
        int i = 0;

        while(token != NULL)
        {
           argv[i] = token;
           token = strtok(NULL, " ");
           i++;
        }
        argv[i] = NULL;
        argc = i;
        
        //for(int i = 0; i < argc; i++)
        //{
        //    printf("\nToken %s", argv[i]);
        //}
	//
        if(strcmp(cmd, "cd")== 0)
	{
	    if(cd(argv[1]) < 0)
	    {
	        perror(argv[1]);
	    }else{
                strcpy(cdArg, argv[1]);
	    }
	}else if(strcmp(cmd, "clear") ==0){
	    system("clear");
	}else{

        strcpy(fullPath, path);
        strcat(fullPath, argv[0]);
        
        //printf("\nFull Path %s", fullPath);
        
        for(i = 0; i < strlen(fullPath); i++)
        {
            if(fullPath[i]=='\n')
            {      
                fullPath[i]='\0';          
            }
                                    
        }
        
        int pid = fork();
        
        if(pid == 0)
        {
            execvp(fullPath, argv);
            fprintf(stderr, "\nERROR: Command not found.\n");
        }else
        {
            wait(NULL);
        }
       }
    }
   
}
