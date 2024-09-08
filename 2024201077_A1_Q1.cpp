#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <cstdlib>
using namespace std;


int createAssignmentFolder(){
    //if Assignment1 folder already exist then give error
    string directoryPath="./Assignment1";
    const char*c=directoryPath.c_str();
    if(mkdir(c,S_IRUSR | S_IWUSR | S_IXUSR)){
        perror("Folder already exist Please remove folder first");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int openInputFile(int &fd_input,string filename){
    //open input file 
    string path="./"+filename;
    const char *c=path.c_str();
    fd_input=open(c,O_RDONLY);
    if(fd_input<0){
        perror("Not able to open source file");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void printPercentage(off_t pt){   //print percentage at same position
    string str="\r Percentage Written:";
    str+=to_string(pt)+"%";
    const char *c=str.c_str();
    write(1, c, str.size());
}

void calculatePercentage(off_t inputFileSize, off_t outputFileSize) {  //calculate percentage

    off_t pt=(outputFileSize*100)/inputFileSize;
    printPercentage(pt);
 
}


//reverse the file using buffer
int reverse(int fd_input,int fd_output,int start,int end,off_t filesize){   
    int bufSize=end-start;
    char buf[end-start];
    if(read(fd_input,&buf,bufSize)<0){
        perror("read from file to buffer for copy");
        return EXIT_FAILURE;
    }

    for(int i=0;i<bufSize/2;i++){
        char temp=buf[i];
        buf[i]=buf[bufSize-i-1];
        buf[bufSize-i-1]=temp;
    }

    if(write(fd_output,&buf,bufSize)<0){
        perror("write from buffer to file for copy");
        return EXIT_FAILURE;
    }

    off_t outputfilesize=lseek(fd_output,0,SEEK_END);   //calculate output file size
    calculatePercentage(filesize,outputfilesize);
    return EXIT_SUCCESS;
}

//copy file using buffer
void copyFile(int fd_input,int fd_output,int start,int end,off_t filesize){
    int bufSize=end-start+1;
    char buf[end-start+1];
    if(read(fd_input,&buf,bufSize)<0){
        perror("read from file to buffer for copy");
    }

     if(write(fd_output,&buf,bufSize)<0){
        perror("write from buffer to file for copy");
    }
    off_t outputfilesize=lseek(fd_output,0,SEEK_END);
    calculatePercentage(filesize,outputfilesize);
    return;
}

int main(int argc, char *argv[]){
    
    //create folder
    if(createAssignmentFolder()==EXIT_FAILURE){
        return EXIT_FAILURE;
    }

    //open input file
    int fd_input;
    if(openInputFile(fd_input,argv[1])==EXIT_FAILURE){
        return EXIT_FAILURE;
    }

    //throw error if wrong flag is detected
    if((atoi(argv[2])!=0) && (atoi(argv[2])!=1)){  
        cout<<"Please Enter flag 0 or 1";
        return EXIT_FAILURE;
    }
    
    //create output file
    int fd_output;
    string path="./Assignment1/";
    if(atoi(argv[2])==0){path+="0_";path+=argv[1];}
    if(atoi(argv[2])==1){path+="1_";path+=argv[1];}
    const char *c= path.c_str();
    fd_output=open(c, O_CREAT | O_RDWR , S_IRUSR | S_IWUSR );
    if(fd_output<0){
        perror("Not able to create output file");
        return EXIT_FAILURE;
    }
      
    //calculate input file size
    off_t filesize=lseek(fd_input,0,SEEK_END);   
    int buffersize=1048576;               //1 MB buffer size
    int temp=filesize;                    //temp us used while file size is greater then one buffer

    

    if(atoi(argv[2])==0){
        if(argc>3){
            cout<<"For flag 0 pleas provide 3 arguments";
            return EXIT_FAILURE;
        }
        if(filesize==0){    //if file size is 0 then create only output file
            cout<<"Percentage Written:100%";
            return EXIT_FAILURE;
        }


        if(temp-buffersize>0){      //reverse all buffer before last buffer
            lseek(fd_input,temp-buffersize,SEEK_SET);
    
    
        while(temp-buffersize>0){  

            reverse(fd_input,fd_output,temp-buffersize,temp,filesize);
            temp-=buffersize;
            if(temp-buffersize>0){
            lseek(fd_input,temp-buffersize,SEEK_SET);
            }
    
        }
    
        }
        lseek(fd_input,0,SEEK_SET);  //reverse last buffer
        reverse(fd_input,fd_output,0,temp,filesize);
    }


    if(atoi(argv[2])==1){
        //if user dont provide starting and ending index then print error
        if(argc !=5){
            cout<<"Please enter flag, starting and ending both index";
            return EXIT_FAILURE;
        }
        int startIndex=atoi(argv[3]);       
        int endIndex=atoi(argv[4]);
         
         //print error for 1 because there is starting and ending index, if file size=0
          if(filesize==0){
            cout<<"File size must not be 0, it should contain starting and ending index";
            return EXIT_FAILURE;
          }

          //index are less then 0
         if(startIndex<0 || endIndex<0){
            cout<<"Please enter positive index";
            return EXIT_FAILURE;
         }
         //print error if starting index is less then ending index
         if(endIndex<startIndex){
            cout<<"The ending index must be greater than or equal to the starting index";
            return EXIT_FAILURE;
         }

         //print error if starting index is greater then file size
         if(startIndex>=filesize){
            cout<<"Starting index must be less then filesize";
            return EXIT_FAILURE;
         }

         //print error if ending index is greater then file size
         if(endIndex>=filesize){
            cout<<"Ending index is must less then filesize";
            return EXIT_FAILURE;
         }


        int temp1=startIndex;
        int temp2=endIndex;
        //1-- sort (starting - 1) to 0 index in reverse
         if(temp1-buffersize>0){
            lseek(fd_input,temp1-buffersize,SEEK_SET);
         }

         while(temp1-buffersize>0){

            reverse(fd_input,fd_output,temp1-buffersize,temp1,filesize);
            temp1-=buffersize;
            if(temp1-buffersize>0){
            lseek(fd_input,temp1-buffersize,SEEK_SET);
            }
    
         }

        lseek(fd_input,0,SEEK_SET);
        reverse(fd_input,fd_output,0,temp1,filesize);


        //2-- print file as it is from (startIndex to endIndex)
        if(startIndex+buffersize<endIndex){
            lseek(fd_input,startIndex,SEEK_SET);
        }
        temp1=startIndex;
        while(temp1+buffersize<endIndex){
            copyFile(fd_input,fd_output,temp1,temp1+buffersize,filesize);
            temp1+=buffersize;
            if(temp1+buffersize<endIndex){
            lseek(fd_input,temp1,SEEK_SET);
            }
        }

        lseek(fd_input,temp1,SEEK_SET);
        copyFile(fd_input,fd_output,temp1,endIndex,filesize);


        //3-- print from (endIndex to last)
        
        temp=filesize;
        if(temp-buffersize>endIndex+1){
           lseek(fd_input,temp-buffersize,SEEK_SET);
         }

        while(temp-buffersize>endIndex+1){

            reverse(fd_input,fd_output,temp-buffersize,temp,filesize);
            temp-=buffersize;
            if(temp1-buffersize>endIndex+1){
            lseek(fd_input,temp-buffersize,SEEK_SET);
            }
    
        }

        lseek(fd_input,endIndex+1,SEEK_SET);
        reverse(fd_input,fd_output,endIndex+1,temp,filesize);
    }
}
