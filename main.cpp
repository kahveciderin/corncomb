#include <string.h>
#include <stdlib.h>
using namespace std;
#include <stdio.h>
#include <iostream>
#include <fstream>
int main(){

    ifstream file("compiled.cmb");

    //FILE* file = fopen("compiled.cmb", "r"); 
    char c;
    uint32_t valcnt;
    file.seekg()
    for (; file.get(c); ){
        if(isspace(c)){
            valcnt = valcnt + 1;
        }
    }
    valcnt++;
    //printf("%d\n", valcnt);
    string line;

    //////////////////////////////////////////////////////////////
   ////                                                      ////
  ////                   REGISTERS                          ////
 ////                                                      ////
//////////////////////////////////////////////////////////////


    uint64_t *r, v, pc, *ptrram;

    r = new uint64_t[8];
    pc = 0;



    file.clear();
    file.seekg(0);


    uint32_t *virtmem = new uint32_t[valcnt];

    uint32_t i = 0;
    while( getline( file, line ) )
    {
        char *linec = (char*)line.c_str();
        char *p = strtok(linec, " ");
        
        while (p != NULL){
            virtmem[i] = (uint32_t)strtol(p, NULL, 16);
            
            i++;
            p = strtok (NULL, " ");
            
        }

    }
    
    bool cont = 1;
    while(cont){
        uint32_t opcode = virtmem[pc];
        
        uint64_t *vals = new uint64_t[8];
        uint32_t reguse = virtmem[pc + 1];

        bool virt = 0;
        
        
        for(uint32_t i = 0; i < 8; i++){
            int regsel = (reguse & (0xF << (i*4))) >> (i*4);
            //printf("%x\n", regsel);
            switch(regsel){

                case 1:
                vals[i] = virtmem[pc + 2 + i];
                break;

                case 2:
                vals[i] = v;
                //printf("%d, %d\n", opcode, v);
                break;

                case 3:
                vals[i] = r[virtmem[pc + 2 + i]];
                break;

                case 4:
                vals[i] = v;
                virt = 1;
                break;
            }
        }

        printf("DEBUG: opcode: %d reguse: %x pc: %d val0: %d val1: %d v: %d\n", opcode, reguse, pc, vals[0], vals[1], v);
        switch(opcode){

            case 0: //dealloc
                free((void*)vals[0]);
                pc+=3;
            break;

            case 1: //malloc
                v = (uint64_t)malloc(vals[0]);
                pc+=3;
            break;

            case 2: //ptrram
                //printf("%d\n", vals[0]);
                //ptrram = new uint64_t[vals[0]];
                pc+=3;
                printf("haha no\n");
            break;

            case 3: //setptr
                //printf("%d\n", vals[0]);
                //ptrram[vals[1]] = vals[0];
                pc+=4;
                printf("haha no\n");
            break;

            case 4: //getptr
                //v = ptrram[vals[0]];
                //printf("%d\n", v);
                pc+=3;
                printf("haha no\n");
            break;

            case 5: //set
                printf("%d %d\n", vals[0], vals[1]);
                *(uint64_t*)vals[1] = vals[0];
                pc+=4;
            break;

            case 6: //get
                v = *(uint64_t*)vals[0];
                pc+=3;
            break;

            case 7: //print
                printf("%c", vals[0]);
                pc+=3;
            break;

            case 8: //scan
                scanf("%c", &v);
                pc+=3;
            break;

            case 9: //add
                v = vals[0] + vals[1];
                pc+=4;
            break;

            case 10: //sub
                v = vals[0] - vals[1];
                pc+=4;
            break;

            case 11: //mul
                v = vals[0] * vals[1];
                pc+=4;
            break;

            case 12: //div
                v = vals[0] / vals[1];
                pc+=4;
            break;

            case 13: //setreg
                r[vals[0]] = vals[1];
                pc+=4;
            break;

            case 14: //getreg
                v = r[vals[0]];
                pc+=3;
            break;

            case 15: //joz
                pc = vals[0] == 0 ? vals[1] : pc + 1;
                pc+=4;
            break;

            case 16: //call - yet to be implemented
                pc+=3;
            break;

            case 17: //ret - yet to be implemented
                pc+=3;
            break;

            case 18: //halt
                cont = 0;
                pc+=3;
            break;
        }
        //printf("%d\n", opcode);
    }

    return 0;
}