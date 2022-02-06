#include<stdio.h>
#include<stdlib.h>
#include<string.h>


void main(){
    FILE *f=fopen("output.txt","r");
    FILE *f1=fopen("memory.txt","w");
    char str[1000],pgm_name[10],strt_add[10],len[10],lk[10];
    int i,j,k,l,len1,l1;
    
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
        }
        else if(str[0]=='T'){
            char s[10];
            for(i=0,j=2;i<6,j<8;i++,j++){
                lk[i]=str[j];
            }
            lk[i]='\0';
            //printf("%s",lk);
            l1=strtol(lk,NULL,16);
            //printf("%X %X",l1,len1);
            while(l1!=len1){
                fprintf(f1,"%X: xx\n",len1);
                len1++;
            }

            for(i=0;str[12+i]!='\n';){
                fprintf(f1,"%X: %c",len1,str[12+(i++)]);
                fprintf(f1,"%c\n",str[12+(i++)]);
                if(str[12+i]=='^'){
                    i++;
                }
                //printf(" %d ",i);
                len1+=1;
            }
        }
        else if(str[0]=='E'){
            break;
        }
    }
}
