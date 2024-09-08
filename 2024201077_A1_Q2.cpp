#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <cstdlib>
using namespace std;

void printForUser(struct stat st,string filetype){
    int read=st.st_mode & S_IRUSR;
    int write=st.st_mode & S_IWUSR;
    int execute=st.st_mode & S_IXUSR;
    cout<<"User has read permissions on "<<filetype<<" :";
    if(read!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"User has write permissions on "<<filetype<<" :";
    if(write!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"User has execute permissions on "<<filetype<<" :";
    if(execute!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
}

void printForGroup(struct stat st,string filetype){
    int read=st.st_mode & S_IRGRP;
    int write=st.st_mode & S_IWGRP;
    int execute=st.st_mode & S_IXGRP;
    cout<<"Group has read permissions on "<<filetype<<" :";
    if(read!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"Group has write permissions on "<<filetype<<" :";
    if(write!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"Group has execute permissions on "<<filetype<<" :";
    if(execute!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}



}

void printForOther(struct stat st,string filetype){

    int read=st.st_mode & S_IROTH;
    int write=st.st_mode & S_IWOTH;
    int execute=st.st_mode & S_IXOTH;
    cout<<"Other has read permissions on "<<filetype<<" :";
    if(read!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"Other has write permissions on "<<filetype<<" :";
    if(write!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}
    cout<<"Other has execute permissions on "<<filetype<<" :";
    if(execute!=0){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}

}
void displayPermission(string filetype,const char *filepath){
    struct stat st;
    if(stat(filepath,&st)<0){
        perror("Don't get in file attribute");
    }
    printForUser(st,filetype);
    printForGroup(st,filetype);
    printForOther(st,filetype);
    
}


bool isReverse(int fd_input,int fd_output,off_t inputfilesize,off_t outputfilesize){
    if(inputfilesize!=outputfilesize){return false;}

     char buf1[1], buf2[1];
     
    for (off_t i = 0; i < inputfilesize; i++) {
        //Read from file1 forward
        lseek(fd_input,i,SEEK_SET);
        if (read(fd_input, &buf1, 1) != 1) {
            perror("Error reading file1");
            return false;
        }

        //Read from file2 backward
        lseek(fd_output,inputfilesize-i-1,SEEK_SET);
        if (read(fd_output, &buf2, 1) != 1) {
            perror("Error reading file2");
            return false;
        }

        //Compare characters
        if (buf1[0] != buf2[0]) {
            return false;
        }
    }
        return true;
}


int main(int argc,char *argv[]){

    //check is directory is created 
    struct stat st;
    string path;
    string append=".";     //append dot for going to present directory
    path=append+argv[3];
    const char *c=path.c_str();
    if(stat(c,&st)<0){
    perror("Didn't get Directory data");
    }

    int isDirectoryExist=S_ISDIR(st.st_mode);
    cout<<"Directory is created:";
    if(isDirectoryExist>0){cout<<"Yes"<<endl;} 
    else{cout<<"No"<<endl; return EXIT_FAILURE;}

    //open both file
    path=append+argv[2];
    c=path.c_str();
    int fd_input,fd_output; 
    fd_input=open(c,O_RDWR);
    if(fd_input<0){
        perror("opening input file");
        return EXIT_FAILURE;
    }

    struct stat st1;
    stat(c,&st1);
    off_t inputfilesize=st1.st_size;
    
    path=append+argv[1];
    c = path.c_str();
    fd_output=open(c,O_RDWR);
    if(fd_output<0){
        perror("opening output file");
        return EXIT_FAILURE;
    }
    struct stat st2;
    stat(c,&st2);
    off_t outputfilesize=st2.st_size;
    
 
  

    //check new file content is reversed of old file
    cout<<"Whether file contents are reversed in newfile:";
    bool ans=isReverse(fd_input,fd_output,inputfilesize,outputfilesize);
    if(ans==1){cout<<"Yes"<<endl;}
    else{cout<<"No"<<endl;}

    cout<<"Both Files Sizes are Same :";
    if(inputfilesize==outputfilesize)
    {cout<<"Yes"<<endl;}
    else
    {cout<<"No"<<endl;};

    //print permission
    string filetype[]={"newfile","oldfile","directory"};
    string tempfilepath[]={argv[1],argv[2],argv[3]};
    for(int i=0;i<3;i++){          //change file type
    string temp='.'+tempfilepath[i];
        const char *filepath=temp.c_str(); 
        displayPermission(filetype[i],filepath);
    }

}