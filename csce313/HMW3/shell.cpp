/*
* Assignment 3 
* written by- Jorge Farinacci
* Spring 2020 - Dr.Tanzir Ahmed
*/
#include <iostream>
#include <cstdlib>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <sys/stat.h>
#include <fcntl.h>

uint length (char * str) {
    char * t;    
    int sz = 0;
   
    for (t = str; *t != '\0'; t++) {
        sz++;
    }
    return sz;
}
uint size (char** tokens)
{
    char** t = tokens;
    int sz = 0;
    for (sz = 0; tokens[sz] != NULL; sz++);
    return sz;
}
bool isEqual(char ch1, char ch2)
{
    if(ch1 == ch2) 
        return true;
    return false;
}
char* substring(char* str,int start,int offset)
{
    if(offset == -1) offset = length (str); 
    int sz = offset;
    //char *sub = (char*)(malloc (sizeof (char) * sz));
    char* sub = new char[sz + 1];
    int pos = start;
    for(pos; pos < offset; pos++)
    {
        sub[pos-start] = str[pos];
    }
    if(isEqual(sub[sz-1], '\0') == 0)
    {
        sub[sz] = '\0';
    }
    return sub;
}


std::vector<int> inidcesOf(char * str,char delim = ' ')
{
    std::vector<int> indices;
    //exclude instances of our delimiter within quotes
    char sQuote = '\'';
    char dQuote = '"';
    char ignoreInstance = '\0';
    bool inIgnore = false;
    int strIter = 0;
    uint sz = length(str);
    

    if(sz == 0) return indices;
    for(strIter; strIter < sz; strIter++)
    {

        if( isEqual(str[strIter], sQuote) || isEqual(str[strIter], dQuote) )
        { 
            if(!inIgnore) 
            {
                inIgnore = true;
                if(isEqual(str[strIter], sQuote))
                {
                    ignoreInstance = sQuote;
                }
                else
                {
                    ignoreInstance = dQuote;
                }
            }
            else
            {
                if( isEqual(str[strIter], ignoreInstance) )
                {
                    inIgnore = false;
                    ignoreInstance = '\0';
                }
            }    
        }  
        if(!inIgnore && isEqual(str[strIter], delim))
        {
            indices.push_back(strIter);
        }
        
    }
    return indices;
}
bool contains(char* str, char* seq)
{
    uint str_len = length(str);
    uint seq_len = length(seq);
    if(seq_len > str_len)
    {
        return false;
    }
    else
    {
        uint str_it = 0;
        uint seq_it = 0;        
        for(str_it; str_it < str_len; str_it++)
        {
            if( isEqual(str[str_it], seq[seq_it]) )
            {
                seq_it++;
                if(seq_it == seq_len)
                {
                    return true;
                }
            }
            else
            {
                seq_it = 0;
            }
        }
    }
}
char** splitOn(char * str,char delim = ' ')
{
    
    std::vector<int> indices = inidcesOf(str,delim);
    uint numTokens = indices.size();
    uint sz = length (str);
    uint i = 0,j = 0; 
    if(numTokens == 0) 
    {
        char** tokens = new char*[2];
        uint strlen = length (str);
        tokens[0] = new char[strlen];
        tokens[1] = NULL;
        strcpy(tokens[0], str);
        return tokens;
    }
    char ** tokens =  new char*[numTokens+2];

    tokens[0] =  substring(str,0,indices[0]);
    for(i; i < numTokens -1; i++)
    {
        tokens[i + 1] = substring(str,indices[i] + 1,indices[i+1]);
    }
    tokens[numTokens] = substring(str,indices[numTokens - 1] + 1,-1);
    tokens[numTokens + 1] = NULL;
    return tokens; 

}

void trim(char* str)
{
    /*
    implmentation to remove leading and trailing whitespaces from char array found at
    https://stackoverflow.com/questions/25345598/c-implementation-to-trim-char-array-of-leading-trailing-white-space-not-workin
    */
    int start = 0; // number of leading spaces
    char* buffer = str;
    while (*str && *str++ == ' ') ++start;
    while (*str++); // move to end of string
    int end = str - buffer - 1; 
    while (end > 0 && buffer[end - 1] == ' ') --end; // backup over trailing spaces
    buffer[end] = 0; // remove trailing spaces
    if (end <= start || start == 0) return; // exit if no leading spaces or string is now empty
    str = buffer + start;
    while ((*buffer++ = *str++));  // remove leading spaces: K&R

}

void processPipe(char** input)
{
    
    int num_pipes = size (input);
    bool first = true;

    for(int curr = 0; curr < num_pipes; curr ++)
    {
        int pipefd[2];
        int status = pipe(pipefd);
        if(status < 0)
            return; //pipe failed to open
        else if(!fork())
        {
            if(curr < num_pipes - 1)
                dup2(pipefd[1],1); // result of command will be piped to next command
            char* cmd = input[curr];
            char**args = splitOn(cmd,' ');
            if (!( execvp(args[0],args) >= 0)) {  
                    printf("*** ERROR: exec failed\n");
                    exit(1);
            }        
        }
        else
        {

            if(curr ==num_pipes)
                wait(0);
            dup2(pipefd[0],0);
            close(pipefd[1]);

        }
    }


}
void processExec(char* input)
{
    int fd0,fd1,i;
    bool hasIn = false,hasOut = false,isFirst = false;
    char *in,*out;
    int splitAt = -1;
    int pid = fork();
    if(pid < 0) return;
    if (pid == 0)
    { 
        for(i=0; input[i] != '\0'; i++)
        {
            if(isEqual(input[i], '<'))
            {        
                if(splitAt == -1 || splitAt >= i)
                    splitAt = i;
                    
                in = substring(input,i+1,-1);
                trim(in);
                hasIn = true;           
            }               

            if(isEqual(input[i], '>'))
            {    
               
                if(splitAt == -1 || splitAt >= i)
                    splitAt = i;
                out = substring(input,i+1,-1);
                trim(out);
                hasOut = true;
            }         
        }
        if(in)
        {   
            int fd0 = open (in, O_WRONLY|O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);     
            dup2(fd0, 0);
            close(fd0); 
        }
        if (out)
        {

            int fd1 = open (out, O_CREAT|O_WRONLY|O_TRUNC,S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); 
            dup2(fd1, 1);
            close(fd1);
        }
        char * first = substring(input,0,splitAt);
        trim(first);        
        char** args = splitOn(first,' ');
        if (!( execvp(args[0],args) >= 0)) {  
                printf("*** ERROR: exec failed\n");
                exit(1);
        }
    }
    else { 
        wait(0);
    }
}
void process_cmd(char* input,bool& isExit)
{


    if (strcmp(input, "exit") == 0)
    {
        isExit = true;
    }
    else
    {
        char** pipes = splitOn(input, '|');
        size_t length = size (pipes);
        std::cout << length << std::endl;
        if( length == 1) // no pipes
        {
            char** args = splitOn(input,' ');
            if (strcmp(args[0], "cd") == 0)
            {
                    chdir(args[1]);//change directory to arg[1]
            }
            else
            {
                processExec(input);
            }
        
        }
        else if(length > 1)
        {
            processPipe(pipes);
        }
        std::cout << getcwd(NULL, 0) << "$ ";
    }
}

int main()
{
   
    bool isExit = false;
    
    int c,i = 0;
    std::cout << getcwd(NULL, 0) << "$ ";
  
    while(!isExit) 
    {
        std::string inputline;
        while(std::getline (std::cin, inputline))
        {
            char* cmd =(char*)inputline.c_str();
            trim(cmd);
            process_cmd(cmd,isExit);
            if(isExit) break;
        }
        
        std::cout <<"Signal to exit shell has been received."<< std::endl;
        std::cout << "Exiting normally!" << std::endl;
        isExit = true; // needed in case ctrl+d (EOT signal) is sent
    }
    return EXIT_SUCCESS;

}