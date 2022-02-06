#include<stdio.h>
#include<string.h>
#include<stdlib.h>

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
       //printf("%s %s\n",opcode,optab[i].mnemonic);
       if(strcmp(opcode,optab[i].mnemonic)==0){
           return optab[i].value;
       }
   }
   return -1;
}

struct symtab{
    char symbol[10];
    int address;
}symtab[100];

int symtab_len;

int search_symtab(char label[10]){
   for(int i=0;i<symtab_len;i++){
       if(strcmp(label,symtab[i].symbol)==0){
           return symtab[i].address;
       }
   }
   return -1;
}

void load_symtab()
{
    FILE *symtab_file=fopen("symtab.txt", "r");
    int i=0;
    while (!(feof(symtab_file)))
    {
        fscanf(symtab_file,"%s %X",symtab[i].symbol,&symtab[i].address);
        i++;
    }
    
    symtab_len=i;
    fclose(symtab_file);
}

struct inttab
{
    char addr[10];
    char symbol[10];
    char inst[10];
    char value[10];

} inttab[100], int_inst;

int inttab_len;

void load_inttab(FILE *inter_file)
{
    int i=0,l=0;
    char str[40];
    char s[40];
    char *tokens[4];
    char *str1;

    while (!(feof(inter_file)))
    {
        fgets(str,40,inter_file);
        strtok(str,"\n");
        strcpy(s,str);
        str1=strtok(str," ");
        while(str1!=NULL){
            tokens[l++]=str1;
            str1=strtok(NULL," ");
        }
        //printf("%s",tokens[1]);
        if((l==3 && strcmp(tokens[1],"START")==0) || (l==3 && strcmp(tokens[1],"START")!=0 && strcmp(tokens[2],"START")!=0)){
            strcpy(inttab[i].addr,tokens[0]);
            strcpy(inttab[i].value,tokens[2]);
            strcpy(inttab[i].inst,tokens[1]); 
            strcpy(inttab[i].symbol,"");    
        }
        else if(l==3 && strcmp(tokens[2],"START")==0){
            strcpy(inttab[i].addr,tokens[0]);
            strcpy(inttab[i].inst,tokens[2]);
            strcpy(inttab[i].symbol,tokens[1]); //printf("%s233",inttab[i].symbol);
            strcpy(inttab[i].value,"");            
        }
        else if(l==2){
            strcpy(inttab[i].addr,tokens[0]);
            strcpy(inttab[i].inst,tokens[1]);
            strcpy(inttab[i].symbol,""); 
            strcpy(inttab[i].value,""); 
        }
        else if(l==4){
            strcpy(inttab[i].addr,tokens[0]);
            strcpy(inttab[i].symbol,tokens[1]);
            strcpy(inttab[i].inst,tokens[2]);
            strcpy(inttab[i].value,tokens[3]);
        }
        //printf("%s ",inttab[i].addr);
        i++;
        l=0;
    }
    
    inttab_len=i-1;
    fclose(inter_file);
}

void main(){
    FILE *inter_file=fopen("inter_file.txt","r");
    FILE *output=fopen("output.txt","a");
    FILE *list_file=fopen("list.txt","a");
   
    load_optab();
    load_inttab(inter_file);
    load_symtab();
    
    int f=1,len=0,i=1,start_add,k;
    int address,s;
    if(strcmp(inttab[1].inst,"START")==0){
        start_add=(int)strtol(inttab[1].value, NULL, 16);
    }
    else{
        start_add=(int)strtol(inttab[1].addr, NULL, 16);
    }
    
    if (strcmp(inttab[1].inst, "START")==0 || f==1){
        fprintf(output, "H^%6s^%06X^%06X", inttab[1].symbol, start_add,strtol(inttab[inttab_len].addr,NULL,16)-start_add);
        if(strcmp(inttab[1].inst, "START")==0){
            i++;
        }
    }
    
    while(1){
        if(strcmp(inttab[i].inst,"END")==0){
            fprintf(output,"\nE^%06X",start_add);
            break;
        }
        //printf("%s",inttab[i].inst);
        int size=atoi(inttab[i+1].addr)-atoi(inttab[i].addr);
        
        if((len+size)>30 || f==1){
            fprintf(output,"\nT^%06s^00",inttab[i].addr);
            f=0;
            len=0;
        }
        else{
            len+=size;
            i++;
        }
        //printf("%d",i);
        int op_value=search_optab(inttab[i].inst);
        //printf("%02X %s %s\n",op_value,inttab[i].inst,inttab[i].value);
        if(search_optab(inttab[i].inst)!=-1){
            
            if(strcmp(inttab[i].value,"")!=0 && search_symtab(inttab[i].value)!=-1){
                address=search_symtab(inttab[i].value);
                fprintf(output,"^%02X%04X",op_value,address);
            }
            /*else if(strcmp(inttab[i].value,"")!=0 && search_symtab(inttab[i].value)!=-1 && inttab[i].value[strlen(inttab[i].value)-1]=='X'){
                address=search_symtab(inttab[i].value);
                //printf("%X",address);
                printf("%s ",inttab[i].value);
                if(inttab[i].value[strlen(inttab[i].value)-1]=='X'){
                    s=address/0x1000;
                    s+=8;printf("%s",inttab[i].value[strlen(inttab[i].value)-1]);
                    //printf("%X ",s);
                    fprintf(output,"^%02X%X%04X",op_value,s,address/0x1000);
                }
            }*/
            else if(strcmp(inttab[i].value,"")!=0 && search_symtab(inttab[i].value)==-1){
                fprintf(output,"^%02X0000",op_value);
                fprintf(list_file,"Undefined symbol\n");
            }
            else{
                fprintf(output,"^%02X0000",op_value);
            }
        }
        else if(strcmp(inttab[i].inst,"BYTE")==0){
            if(inttab[i].value[0]=='C'){
                fprintf(output,"^");
                //printf("%s",inttab[i].value);
                for(int j=2;j<strlen(inttab[i].value)-1;j++){
                    fprintf(output,"%02X",inttab[i].value[j]);//printf("%s",inttab[i].value);
                }
            }
            else if(inttab[i].value[0]=='X'){
                fprintf(output,"^%X",atoi(inttab[i].value));
            }
        }
        else if(strcmp(inttab[i].inst,"WORD")==0){
            fprintf(output,"^%06x", atoi(inttab[i].value));
        }
        else if(strcmp(inttab[i].inst,"RESW")==0){
            k=i;
            while(strcmp(inttab[++k].inst,"RESW")!=0 || strcmp(inttab[k].inst,"RESB")!=0){
                if(strcmp(inttab[k].inst,"END")==0){
                    break;
                }
                fprintf(output,"\nT^%06s^00",inttab[k].addr);
                break;
            }
        }
        else if(strcmp(inttab[i].inst,"RESB")==0){
            k=i;
            while(strcmp(inttab[++k].inst,"RESB")!=0 || strcmp(inttab[k].inst,"RESW")!=0){
                if(strcmp(inttab[k].inst,"END")==0){
                    break;
                }
                fprintf(output,"\nT^%06s^00",inttab[k].addr);
                break;
            }
        }
       // write_list_file(list_file,s);
    }
    fclose(output);
}
