/*
    Tanzir Ahmed
    Department of Computer Science & Engineering
    Texas A&M University
    Date  : 2/8/20
 */
#include <iostream>
#include <stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <string> 

#include "common.h"
#include "FIFOreqchannel.h"

#define INT_MIN -999999
#define DOUB_MIN -999999.999999
using namespace std;



int main(int argc, char *argv[]){
    int n = 100;    // default number of requests per "patient"
	int p = 15;		// number of patients

    int selectPatient = INT_MIN;
    int selectEcgNo = INT_MIN;
    int bufferSize = MAX_MESSAGE;
    double selectTime = DOUB_MIN;
    char* filename = NULL;
    char* msgSize = NULL;
    bool patientSpecified = false;
    bool fileSpecified = false;
    bool specificPoint = false;
    bool newMaxMessage = false;
    bool newChannel = false;

    srand(time_t(NULL));
    int opt;
    while ((opt = getopt(argc, argv, "p:t:e:m:f:c")) != -1) 
    {
        switch(opt)
        {
            case 'p':
                selectPatient = atoi(optarg);
                patientSpecified = true;
                break;
            case 't':
                selectTime = atof(optarg);
                break;
            case 'e':
                selectEcgNo  = atoi(optarg);
                break;
            case 'm':
                msgSize = optarg;
                bufferSize = atoi(optarg);
                newMaxMessage = true;
                break;
            case 'f':
                filename = optarg;
                fileSpecified = true;
                break;
            case 'c':
                newChannel = true;
                break;
            case ':':
                cout << "argument not provided"<< endl;
                return EXIT_FAILURE;
                break;
            case '?':
                cout << "An unkown argument has been recieved and will be ignored" << endl;
                cout << "possible argument type(s) are p,t,e,m,f,c" << endl;
                break;
        }
    }
    /* 
    A selected patient must be a valid patient
    */
    if(patientSpecified)
    {
        if(selectPatient <= 0 || selectPatient > p)
        {
            cout << "selected Patient is " << selectPatient<< endl;
            cout << "selected Patient must be in range 1 and " << p << endl;
            return EXIT_FAILURE;
        }    
    }

    /* 
    if either selectTime or selectEcgNo is initialized then
    selectPatient,selectTime and selectEcgNo must be initialized
    */
    if(selectTime != DOUB_MIN || selectEcgNo != INT_MIN)
    {
        if(!patientSpecified)
        {
            cout << "A patient has not been selected"<< endl;
            return EXIT_FAILURE;
        }            
        if(selectTime < 0.00 || selectTime > 59.996)
        {
            cout << "selected time is " << selectTime << endl;
            cout << "selected time must be in range  0.00 and 59.996" << endl;
            return EXIT_FAILURE;           
        }
        if (selectEcgNo < 1 || selectEcgNo > 2)
        {
            cout << "selected ecg is " << selectEcgNo << endl;
            cout << "selected ecg must be either 1 or 2" << endl;
            return EXIT_FAILURE;               
        }
        specificPoint = true;
    }
    /*
     if a file is provided ensure that the file is valid
    */
/*     if(fileSpecified)
    {
        uint length = (unsigned)strlen(filename);
        uint index = 0;
        uint backPos = 0;

        bool periodFound = false;
        for(index = length  ; index > 0; index--)
        {
            if(filename[index] == '.')
            {
                periodFound = true;
                break;
            }
        }
        char* file_root;
        char* file_ext;

        if (periodFound) backPos = length - index;
        char* filename_root = new char[length - backPos + 1];
        char* ext = new char[backPos+1];
        strncpy(filename_root, filename, length - backPos);
        //strncpy(ext, filename, length - diff);
        copy(filename + index, filename + length, ext);
        filename_root[length - backPos + 1] = '\0';
        ext[length+1] = '\0';
        int pid = atoi (filename_root);

        if(pid <= 0 || pid > p)
        {
            cout << "An inavlid filename has been provided:: File name given is '" << filename  << "'" << endl;
            cout << "Filename must identify a valid patient by being in range of 1  and " << p  << endl;
            return false;           
        }
    } */
    //start server
    if (!fork())
    {//if server is not running create a new process and run the server
        
        
        if(newMaxMessage){
            char* args[] = {"./server", "-m", msgSize,NULL};
            execvp(args[0], &args[0]);
        }
        else{
            char* args[] = {"./server",NULL};
            execvp(args[0], &args[0]);
        }
        
    }
    else
    {
        //initialize communication with server
        FIFORequestChannel chan ("control", FIFORequestChannel::CLIENT_SIDE);
        struct timeval start, end;
        double time_elapsed;

        
        if(newChannel)
        {// request a new channel
            MESSAGE_TYPE m_chan = NEWCHANNEL_MSG;
            chan.cwrite(&m_chan, sizeof(MESSAGE_TYPE));
            char chanBuf [bufferSize];
            chan.cread(chanBuf,bufferSize);
            string name = string(chanBuf);

            FIFORequestChannel new_chan (name, FIFORequestChannel::CLIENT_SIDE);            
            
            //test that new channel is operational with an input
            cout << "Testing new channel with input parameters Patient = " 
            << 3 << " Time = " << 59.00 << " Ecg = " << 2 << endl;       
            datamsg d_msg(3, 59.00, 2);
            new_chan.cwrite(&d_msg, sizeof(d_msg));  
            char buf [bufferSize];
            new_chan.cread (buf, bufferSize);
            double val = *(double*)buf;
            cout << "server returned value: " << val << endl;
            
         
        }
        if(specificPoint)
        {//Request single data point

            cout << "Request made for value for input parameters Patient = " 
            << selectPatient << " Time = " << selectTime << " Ecg = "
            << selectEcgNo << endl;    
            gettimeofday(&start, NULL);
            datamsg d_msg(selectPatient, selectTime, selectEcgNo);
            chan.cwrite(&d_msg, sizeof(d_msg));  
            char buf [bufferSize];
            chan.cread (buf, bufferSize);
            double val = *(double*)buf;
            gettimeofday(&end, NULL);
            cout << "server returned value: " << val << endl;
            time_elapsed = (end.tv_sec - start.tv_sec) * 1e6;
            time_elapsed = (time_elapsed + (end.tv_sec - start.tv_sec)) * 1e-6;            
            cout <<"time to retrieve single value:" <<time_elapsed << endl;
        }
        else if (patientSpecified)
        {// Request data points for specific patient
            datamsg d_msg(selectPatient, 0.0, 1);
            string out = "received/x" + to_string(selectPatient) + ".csv"; 
            ofstream f(out, ios::out | ios::binary | ios::trunc);
            if (f.is_open())
            {
                gettimeofday(&start, NULL);
                cout << "Requesting data points for " << selectPatient << endl;
                double t = 0;
                while(t < 59.996)
                {
                    datamsg d1(selectPatient , t , 1);
                    chan.cwrite(&d1, sizeof(d1));  
                    char buf1 [bufferSize];
                    chan.cread (buf1, bufferSize);
                    double ecg1 = *(double*)buf1;     

                    datamsg d2(selectPatient , t , 2);
                    chan.cwrite(&d2, sizeof(d2));  
                    char buf2 [bufferSize];
                    chan.cread (buf2, bufferSize);
                    double ecg2 = *(double*)buf2;

                    f << t << "," << ecg1 << "," << ecg2 << "\n";
                    t += .004;
                }

                f.close();
                gettimeofday(&end, NULL);
                time_elapsed = (end.tv_sec - start.tv_sec) * 1e6;//seconds elapsed to ms
                time_elapsed += (end.tv_usec - start.tv_usec) / 1000.0;//us elapsed to ms                     
                cout << "Finished requesting data points for " << selectPatient << endl;
                cout << "time elapsed is " << time_elapsed << endl;
            }
            else
            {
                cout <<"failed to open file "<< out << endl;
                cout<< "suspending operation" << endl;
            }

        }
        if(fileSpecified)
        {//request data points by file
            filemsg f_msg(0, 0);

            uint size = sizeof(filemsg) + strlen(filename) + 1;
            char buff[size];
            memcpy(buff, &f_msg, sizeof(filemsg));
            memcpy(buff + sizeof(filemsg), filename, strlen(filename) + 1);      
            chan.cwrite(buff,size);
            char buf2 [bufferSize];
            chan.cread (buf2, bufferSize);
            
            __int64_t fileLength = *(__int64_t*)buf2;
            
           

            string out = "received/" + string(filename);
            ofstream f(out, ios::out | ios::binary | ios::trunc);  
            if (f.is_open())
            {
                uint offset = 0;
                
                gettimeofday(&start, NULL);
                
                while(true)
                {
                    
                    if(offset + bufferSize <= fileLength)
                    {
                        
                        char bf[size];
                        filemsg f_msg(offset, bufferSize);
                        memcpy(bf, &f_msg, sizeof(filemsg));
                        memcpy(bf + sizeof(filemsg), filename, strlen(filename) + 1);      
                        chan.cwrite(bf,size);
                        char buf1 [bufferSize];
                        chan.cread (buf1, bufferSize);  
                        f.write(buf1, bufferSize); 
                    }
                    else
                    {
                        char bf[size];
                        filemsg f_msg(offset, fileLength - offset);
                        memcpy(bf, &f_msg, sizeof(filemsg));
                        memcpy(bf + sizeof(filemsg), filename, strlen(filename) + 1);      
                        chan.cwrite(bf,size);
                        char buf1 [fileLength - offset];
                        chan.cread (buf1, fileLength - offset);  
                        f.write(buf1, fileLength - offset);                         
                        break;
                    }

                    offset += bufferSize;
                    
                }
                
                f.close();
                gettimeofday(&end, NULL);
                time_elapsed = (end.tv_sec - start.tv_sec) * 1e6;//seconds elapsed to ms
                time_elapsed += (end.tv_usec - start.tv_usec) / 1000.0;//us elapsed to ms                
                cout << "Finished requesting all data points in file " <<string(filename) << endl;
                cout << "time elapsed is " << time_elapsed << endl;                
            }   
            else
            {
                cout <<"failed to open file "<< out << endl;
                cout<< "suspending operation" << endl;
            }                           
        }



        

        //close  the server
        MESSAGE_TYPE m_quit = QUIT_MSG;
        chan.cwrite(&m_quit, sizeof(MESSAGE_TYPE));
        wait(NULL);
    }
    return EXIT_SUCCESS;

}