#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct optab{
    char mnemonic[10];
    int value;
}optab[100];

int optab_len;

void load_optab(){
    FILE *optab_file=fopen("optab.txt","r");
    int i=0;
    while(!(feof(optab_file))){
        fscanf(optab_file,"%s %X",optab[i].mnemonic,&optab[i].value);
        i++;
    }
    optab_len=i;
    fclose(optab_file);
}

int search_optab(char opcode[10]){
   for(int i=0;i<optab_len;i++){
       if(strcmp(opcode,optab[i].mnemonic)==0){
           return 1;
       }
   }
   return 0;
}

struct symtab{
    char symbol[10];
    int address;
}symtab[100];

int symtab_len=0;

int search_symtab(char label[10]){
   for(int i=0;i<symtab_len;i++){
       if(strcmp(label,symtab[i].symbol)==0){
           return 1;
       }
   }
   return 0;
}

int add_symbol(char symbol[10],int address){
    strcpy(symtab[symtab_len].symbol,symbol);
    symtab[symtab_len].address=address;
    symtab_len++;
    return 1;
}

void print_symtab(){
    FILE *symtab_file=fopen("symtab.txt","a");
    int i=0;
    while(i<symtab_len){
        fprintf(symtab_file,"%s %X\n",symtab[i].symbol,symtab[i].address);
        i++;
    }
    fclose(symtab_file);
}

void write_intermediate_file(FILE *fptr,int add,char str[20]){
   fprintf(fptr,"\n%X %s",add,str);
}

void main(){
    load_optab();
    FILE *input_file=fopen("input.txt","r");
    FILE *intermediatry_file=fopen("inter_file.txt","a");
    int lc;
    int l=0,f=1;
    char opcode[10],value[10],strt_add[10],label[10]="";
    char str[40];
    char strcopy[40];
    char *str1;
    char *tokens[4];
    
    //read first line
    fgets(str,40,input_file);
    strtok(str,"\n");
    strcpy(strcopy,str);
    str1=strtok(str," ");
    while(str1!=NULL){
        tokens[l++]=str1;
        str1=strtok(NULL," ");
    }
    
    if(l==3 && strcmp(tokens[1],"START")==0){
        lc=strtol(tokens[2],NULL,16);
        strcpy(strt_add,tokens[2]);
        write_intermediate_file(intermediatry_file,0,strcopy);
    }
    else if(l==2 && strcmp(tokens[1],"START")==0){
        lc=strtol(0,NULL,16);
        strcpy(strt_add,"0000");
        write_intermediate_file(intermediatry_file,0,strcopy);
    }
    else if(l==2 && strcmp(tokens[0],"START")==0){
        lc=strtol(tokens[1],NULL,16);
        strcpy(strt_add,tokens[1]);
        write_intermediate_file(intermediatry_file,0,strcopy);
    }
    else if(l==1 && strcmp(tokens[0],"START")==0)
    {
        lc=strtol(0,NULL,16);
        strcpy(strt_add,"0000");
        write_intermediate_file(intermediatry_file,0,strcopy);
    }
    else{
        lc=strtol(0,NULL,16);
        strcpy(strt_add,"0000");
        f=0;

        if(l==2){
            strcpy(opcode,tokens[0]);
            strcpy(value,tokens[1]);
        }
        else if(l==3){
            strcpy(opcode,tokens[1]);
            strcpy(value,tokens[2]);
            strcpy(label,tokens[0]);
        }
        else{
            strcpy(opcode,tokens[0]);
        }
    }
    l=0;

    //read next line
    if(f==1){
        fgets(str,40,input_file);
        strtok(str,"\n");
        strcpy(strcopy,str);
        str1=strtok(str," ");
        while(str1!=NULL){
            tokens[l++]=str1;
            str1=strtok(NULL," ");
        }
        if(l==2){
            strcpy(opcode,tokens[0]);
            strcpy(value,tokens[1]);
        }
        else if(l==3){
            strcpy(opcode,tokens[1]);
            strcpy(value,tokens[2]);
            strcpy(label,tokens[0]);
        }
        else{
            strcpy(opcode,tokens[0]);
        }
        l=0;
    }

    while(1){
        int temp=lc;
        if(strcmp(opcode,"END")==0){
            printf("Program size: %d\n",lc-strtol(strt_add,NULL,16));
            write_intermediate_file(intermediatry_file,lc,strcopy);
            //fprintf(intermediatry_file," %06s",strt_add);
            fclose(input_file);
            fclose(intermediatry_file);
            break;
        }
        if(strcmp(label,"")!=0 && search_symtab(label)==0){
            add_symbol(label,lc);
        }
        else if(strcmp(label,"")!=0 && search_symtab(label)==1){
            fprintf(intermediatry_file,"Duplicate Symbol\n");
            break;
        }
        if(search_optab(opcode)==1){
            lc+=3;
        }
        else if(strcmp(opcode,"WORD")==0){
            lc+=3;
        }
        else if(strcmp(opcode,"BYTE")==0){
            if (value[0]=='C'){
               lc+=(strlen(value)-3);
            }
            else if (value[0]=='X'){
               lc+=((strlen(value)-3)/2);
            }
        }
        else if(strcmp(opcode,"RESW")==0){
            lc+=(3*atoi(value));
        }
        else if(strcmp(opcode,"RESB")==0){
            lc+=atoi(value);
        }
        else{
            fprintf(intermediatry_file,"Invalid Opcode\n");
            break;
        }
        write_intermediate_file(intermediatry_file,temp,strcopy);
        if(!feof(input_file)){
            fgets(str,40,input_file);
            strtok(str,"\n");
            strcpy(strcopy,str);
            str1=strtok(str," ");
            while(str1!=NULL){
                tokens[l++]=str1;
                str1=strtok(NULL," ");
            }
            if(l==2){
                strcpy(opcode,tokens[0]);
                strcpy(value,tokens[1]);
            }
            else if(l==3){
                strcpy(opcode,tokens[1]);
                strcpy(value,tokens[2]);
                strcpy(label,tokens[0]);
            }
            else{
                strcpy(opcode,tokens[0]);
            }
            l=0;
        }
        else{
            break;
        }
    }

    print_symtab();
}