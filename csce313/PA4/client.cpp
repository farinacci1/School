#include "common.h"
#include "BoundedBuffer.h"
#include "Histogram.h"
#include "common.h"
#include "HistogramCollection.h"
#include "FIFOreqchannel.h"
#include <thread>
#include <signal.h>
using namespace std;

FIFORequestChannel* create_new_channel(FIFORequestChannel* main)
{
    char name[1024];
    MESSAGE_TYPE m = NEWCHANNEL_MSG;
    main->cwrite(&m,sizeof(m));
    main->cread(name,1024);
    FIFORequestChannel* chan = new FIFORequestChannel(name, FIFORequestChannel::CLIENT_SIDE);
    return chan;
}
void patient_thread_function(int n,int pno,BoundedBuffer* rb){
    datamsg d(pno,0.0,1);
    double resp = 0;
    for(int i=0;i<n;i++)
    {
        rb->push((char*)&d,sizeof(datamsg));
        d.seconds += 0.004;
       
    }
}

void worker_thread_function(FIFORequestChannel* chan,BoundedBuffer* rb,HistogramCollection* hc,int mb){
   char buf[1024];
   char recvbuf[mb];
   double resp = 0;
   while(true)
   {
       rb->pop(buf,1024);
       MESSAGE_TYPE* m = (MESSAGE_TYPE*)buf;
       
       if(*m == DATA_MSG)
       {
            chan->cwrite(buf,sizeof(datamsg));
            chan->cread(&resp,sizeof(double));
            hc->update(((datamsg*)buf)->person,resp);
       }
       else if(*m == FILE_MSG)
       {
            filemsg *fm = (filemsg*) buf;
            string fname = (char*)(fm + 1);
            int sz = sizeof(filemsg) + fname.size() + 1;
            chan->cwrite(buf,sz);
            chan->cread(recvbuf,mb);

            string recvfname = "recv/"+fname;
            FILE* fp = fopen(recvfname.c_str(),"r+");
            fseek(fp,fm->offset,SEEK_SET);
            fwrite(recvbuf,1,fm->length,fp);
            fclose(fp);

       }
       else if(*m == QUIT_MSG)
       {
            chan->cwrite(m,sizeof(MESSAGE_TYPE));
            delete chan;
            break;
       }
   }
}
void file_thread_function(string fname,BoundedBuffer* rb,FIFORequestChannel* chan,int mb)
{
    //create file
    string recvfname = "recv/"+fname;
    char buf[1024];
    filemsg f(0,0);
    memcpy(buf,&f,sizeof(f));
    strcpy(buf+sizeof(f),fname.c_str());
    chan->cwrite(buf,sizeof(f)+fname.size()+1);

    __int64_t filelength;
    chan->cread(&filelength,sizeof(filelength));
    FILE* fp = fopen(recvfname.c_str(),"w");
    fseek(fp,filelength,SEEK_SET);
    fclose(fp);
        //generate all file msgs
    filemsg* fm = (filemsg*) buf;
    __int64_t remlen = filelength;
    while(remlen > 0)
    {
        fm->length = min(remlen,(__int64_t)mb);
        rb->push(buf,sizeof(filemsg)+fname.size() + 1);
        fm->offset += fm->length;
        remlen -= fm->length; 
    }
}


int main(int argc, char *argv[])
{
    int n = 1000;    //default number of requests per "patient"
    int p = 1;     // number of patients [1,15]
    int w = 100;    //default number of worker threads
    int b = 20; 	// default capacity of the request buffer, you should change this default
	int m = MAX_MESSAGE; 	// default capacity of the message buffer
    char* msgSize = NULL;
    bool newMaxMessage = false;
    string fname = "";
    srand(time_t(NULL));
    int opt = -1;
    signal(SIGALRM, handle_alarm);
    while((opt = getopt(argc,argv,"m:n:b:w:p:f:")) != -1)
    {
        switch(opt){
            case 'm':
                msgSize = optarg;
                m = atoi(optarg);
                newMaxMessage = true;
                break;
            case 'n':
                n = atoi(optarg);
                break;
            case 'p':
                p = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 'w':
                w = atoi(optarg);
                break;
            case 'f':
            const char* str= optarg;
            fname = string(str);
        }
    }
    
    int pid = fork();
    if (pid == 0){
        if(newMaxMessage){
            char* args[] = {"./server", "-m", msgSize,NULL};
            execvp(args[0], &args[0]);
        }
        else{
            char* args[] = {"./server",NULL};
            execvp(args[0], &args[0]);
        }

       
    }
    
	FIFORequestChannel* chan = new FIFORequestChannel("control", FIFORequestChannel::CLIENT_SIDE);
    BoundedBuffer request_buffer(b);
	HistogramCollection hc;
    //make historgrams and add to hc
	for(int i=0; i < p; i++)
    {
        Histogram * h = new Histogram(10,-2.0,2.0);
        hc.add(h);
    }
    
	FIFORequestChannel* wChans[w];
    for(int i=0; i < w; i++)
    {
        wChans[i] = create_new_channel(chan);
    }
	
    struct timeval start, end;
    gettimeofday (&start, 0);

    /* Start all threads here */
    if(fname.size() == 0)
    {
        thread patient[p];
        for(int i=0; i <p;i++)
        { 
            patient[i] = thread(patient_thread_function,n,i+1,&request_buffer);
        }
        thread workers[w];
        for(int i=0; i <w;i++)
        { 
            workers[i] = thread(worker_thread_function,wChans[i],&request_buffer,&hc,m);
        }
        for(int i=0; i <p;i++)
        {
            patient[i].join();
        }
          cout << "patient threads finished"<<endl;
        for(int i=0;i<w;i++)
        {
            MESSAGE_TYPE q = QUIT_MSG;
            request_buffer.push((char*)&q,sizeof(q));
        }
        for(int i=0; i <w;i++)
        {
            workers[i].join();
        }          
    }
    if(fname.size() > 0)
    {
        thread filethread (file_thread_function,fname,&request_buffer,chan,m);
        thread workers[w];
        for(int i=0; i <w;i++)
        { 
            workers[i] = thread(worker_thread_function,wChans[i],&request_buffer,&hc,m);
            
        }
        filethread.join();
        cout << "file thread finished"<<endl;
        for(int i=0;i<w;i++)
        {
            MESSAGE_TYPE q = QUIT_MSG;
            request_buffer.push((char*)&q,sizeof(q));
        }
        for(int i=0; i <w;i++)
        {
            workers[i].join();
        }        
    }



    gettimeofday (&end, 0);
    // print the results
	hc.print ();

    int secs = (end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)/(int) 1e6;
    int usecs = (int)(end.tv_sec * 1e6 + end.tv_usec - start.tv_sec * 1e6 - start.tv_usec)%((int) 1e6);
    cout << "Took " << secs << " seconds and " << usecs << " micro seconds" << endl;

    MESSAGE_TYPE q = QUIT_MSG;
    chan->cwrite ((char *) &q, sizeof (MESSAGE_TYPE));
    cout << "All Done!!!" << endl;
    delete chan;
    
}
