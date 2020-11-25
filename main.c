#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define DEBUG
typedef struct linked_list {
    uint32_t value;
    struct linked_list *next;
} q_t;
typedef struct dual_linked_list {
    uint32_t value;
    struct dual_linked_list *next;
    struct dual_linked_list *prev;
} dq_t;
int main(int argc, char *kwargs[]) {
    FILE *f = fopen("compiled.cmb", "r");
    
    #ifdef DEBUG
    char *hahano_dbg[] = {"dealloc", "malloc", "ptrram", "setptr", "getptr", "set", "get", "print", "scan", "add", "sub", "mul", "div", "setreg", "getreg", "joz", "call", "ret", "halt", "push", "pop", "mod"};
    #endif



    q_t *head = malloc(sizeof(q_t));
    q_t *tail = head;
    int i;
    for (i=0;fscanf(f, "%x", &tail->value) > 0; i++) {
        tail->next = malloc(sizeof(q_t));
        tail = tail->next;
    };
    fclose(f);
    uint32_t *virtualmem = malloc(sizeof(uint32_t) * i);
    for (int j=0;j<i;j++) {
        virtualmem[j] = head->value;
        q_t *n = head->next;
        free(head);
        head = n;
    }
    free(head);
    int virtmemsize = i;
    uint64_t registers[8];
    uint64_t v_register=0;
    uint64_t pc=0;
    uint64_t args[8];
    int argcounts[] = {1,1,1,2,1,2,1,1,1,2,2,2,2,2,1,2,1,1,1,2};
    bool cont = 1;
    dq_t *stack = malloc(sizeof(dq_t));
    dq_t *tmp;
    while (cont) {
        uint32_t opcode = virtualmem[pc];
        uint64_t oldpc = pc;

        pc += argcounts[opcode];
        pc += 2;

        
        for (int i=0;i<8;i++) {
            int regsel = (virtualmem[oldpc+1] >> (i*4)) & 0xf;
            switch(regsel){

                case 1:
                args[i] = virtualmem[oldpc + 2 + i];
                break;

                case 2:
                args[i] = v_register;
                //printf("%d, %d\n", opcode, v_register);
                break;

                case 3:
                args[i] = registers[virtualmem[oldpc + 2 + i]];
                break;

                case 4:
                args[i] = virtualmem[oldpc + 2 + i];
                //virtual stuff idk
                break;
            }
        }
        #ifdef DEBUG
        printf("DEBUG: opcode: %s %d %d PC: %d\n", hahano_dbg[opcode], args[0], args[1], oldpc);

        
        #endif
        switch(opcode){

            case 0: //dealloc
                free((void*)args[0]);
            break;

            case 1: //malloc
                v_register = (uint64_t)malloc(args[0]);
            break;

            case 2: //ptrram
                //printf("%d\n", args[0]);
                //ptrram = new uint64_t[args[0]];
                printf("haha no\n");
            break;

            case 3: //setptr
                //printf("%d\n", args[0]);
                //ptrram[args[1]] = args[0];
                printf("haha no\n");
            break;

            case 4: //getptr
                //v_register = ptrram[args[0]];
                //printf("%d\n", v_register);
                printf("haha no\n");
            break;

            case 5: //set
                printf("%d %d\n", args[0], args[1]);
                *(uint64_t*)args[1] = args[0];
            break;

            case 6: //get
                v_register = *(uint64_t*)args[0];
            break;

            case 7: //print
                printf("%c", args[0]);
            break;

            case 8: //scan
                scanf("%c", &v_register);
            break;

            case 9: //add
                v_register = args[0] + args[1];
            break;

            case 10: //sub
                v_register = args[0] - args[1];
            break;

            case 11: //mul
                v_register = args[0] * args[1];
            break;

            case 12: //div
                v_register = args[0] / args[1];
            break;

            case 13: //setreg
                registers[args[0]] = args[1];
            break;

            case 14: //getreg
                v_register = registers[args[0]];
            break;

            case 15: //joz
                pc = args[0] == 0 ? args[1] : pc;
            break;

            case 16: //call - yet to be implemented
                stack->next = (dq_t*) malloc(sizeof(dq_t));
                stack->next->prev = stack;
                stack = stack->next;
                stack->value = pc;
                pc = args[0];
            break;

            case 17: //ret - yet to be implemented
                pc = stack->value;
                tmp = stack->prev;
                free(stack);
                stack = tmp;
                tmp->next = NULL;
            break;

            case 18: //halt
                cont = 0;
            break;

            case 19: //push
                stack->next = malloc(sizeof(dq_t));
                tmp = stack;
                stack = stack->next;
                stack->prev = tmp;
                stack->value = args[0];
            break;
            case 20: //pop
                v_register = stack->value;
                tmp = stack->prev;
                free(stack);
                stack = tmp;
                tmp->next = NULL;
            break;
            case 21: //mod
                v_register = args[0] % args[1];
            break;
        }
        #ifdef DEBUG
        char a; scanf("%c", &a);

        if(a == 's'){
            printf("Stack Trace:\n");
            for(tmp = stack; tmp->prev; tmp = tmp->prev){
            
            printf("- %d\n", tmp->value);
            }
            printf("\n\n");
        }
        #endif
    }
    

    return 0;
}