#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct mem{
    int add;
    char obj[10];
}mem[100];

void main(){
    FILE *f=fopen("output.txt","r");
    FILE *f1=fopen("memory.txt","w");
    char str[1000],pgm_name[10],strt_add[10],len[10],lk[10],half[4],ad[10];
    int i,j,k,l,len1,l1,c=0,length;
    
    while(1){
        fgets(str,1000,f);
        if(str[0]=='H'){
            for(i=2,j=9,k=16,l=0;i<8,j<15,k<20,l<6;i++,j++,k++,l++){
                pgm_name[l]=str[i];
                strt_add[l]=str[j];
                len[l]=str[k];
            }
            pgm_name[l]='\0';
            strt_add[l]='\0';
            len[l]='\0';
            printf("Program name is %s\nLength is %s\nStarting address is %s\n",pgm_name,strt_add,len);
            len1=strtol(strt_add,NULL,16);//break;
            i=0;
        }
        else if(str[0]=='T'){
            for(l=0,j=2;l<6,j<8;l++,j++){
                lk[l]=str[j];
            }
            lk[l]='\0';
            l1=strtol(lk,NULL,16);
            mem[i].add=l1;

            while(l1!=len1){
                mem[i].add=len1;
                mem[i].obj[0]='x';
                mem[i].obj[1]='x';
                mem[i].obj[2]='x';
                mem[i].obj[3]='x';
                mem[i].obj[4]='x';
                mem[i].obj[5]='x';
                //printf("%s-%X ",mem[i].obj,mem[i].add);
                len1+=3;
                i++;
            }

            for(k=0;str[12+k]!='\n';){
                for(l=0,c=0;l<8 && str[12+k]!='\n';l++,k++){
                    c++;
                    if(str[12+k]=='^'){
                        if(c!=1){
                            k++;
                            break;
                        }
                       k++;
                    }
                    mem[i].obj[l]=str[12+k];
                    
                }
                mem[i].obj[l]='\0';
                mem[i].add=len1;
                //printf("%s-%X-%d ",mem[i].obj,mem[i].add,i);
                len1=len1+(c/2);
                i++;
            }
            length=i;
        }
        else if(str[0]=='M'){
            char s[10];
            for(l=0,j=2;l<6,j<8;l++,j++){
                lk[l]=str[j];
            }
            lk[l]='\0';
            int m_len=strtol(lk,NULL,16);
            for(l=0,j=9;l<2,j<11;l++,j++){
                half[l]=str[j];
            }
            if(strtol(half,NULL,10)==5){
                m_len-=1;
                //printf("%X\n",m_len);
            }
            for(i=0;i<100;i++){
                if(mem[i].add==m_len){
                    for(l=0,j=12;l<6,j<18;l++,j++){
                        ad[l]=str[j];
                    }
                    int d=strtol(mem[i].obj,NULL,16)+strtol(ad,NULL,16);
                    itoa(d,mem[i].obj,16);
                }
            }
        }
        else if(str[0]=='E'){
            //printf("%d\n",length);
            for(j=0;j<length;j++){
                fprintf(f1,"%X: %s\n",mem[j].add,mem[j].obj,j);
            }
            break;
        }
    }
}
