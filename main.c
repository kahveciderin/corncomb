#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
//#define DEBUG
typedef struct linked_list {
    uint32_t value;
    struct linked_list *next;
} q_t;
typedef struct dual_linked_list {
    uint32_t value;
    struct dual_linked_list *next;
    struct dual_linked_list *prev;
} dq_t;
void write_to_return(uint64_t value, uint32_t *virtualmem, uint64_t instr_pc, uint64_t **register_banks) {
    uint16_t dest = (uint16_t)(virtualmem[instr_pc + 1] >> 16);
    uint8_t reggroup = dest >> 8;
    uint8_t regindex = dest;
    register_banks[reggroup][regindex] = value;
}

void exit_error(){

}

int main(int argc, char *kwargs[]) {

    if(argc < 2){
        printf("Usage: comb <filename>\n");
        return 0;
    }

    if(access(kwargs[1], F_OK ) == -1){
        printf("File \"%s\" does not exist.\n", kwargs[1]);
        return 0;
    }
    FILE *f = fopen(kwargs[1], "r");
    
    #ifdef DEBUG
    char *hahano_dbg[] = {"dealloc", "malloc", "ptrram", "setptr", "getptr", "set", "get", "print", "scan", "add", "sub", "mul", "div", "setreg", "getreg", "joz", "call", "ret", "halt", "push", "pop", "mod"};
    #endif



    q_t *head = (q_t*)malloc(sizeof(q_t));
    q_t *tail = head;
    int i;
    for (i=0;fscanf(f, "%x", &tail->value) > 0; i++) {
        tail->next = (q_t*)malloc(sizeof(q_t));
        tail = tail->next;
    };
    fclose(f);
    uint32_t *virtualmem = (uint32_t*)malloc(sizeof(uint32_t) * i);
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
    uint64_t *register_banks[] = {&v_register, registers, &pc};
    bool cont = 1;
    dq_t *stack = (dq_t*)malloc(sizeof(dq_t));
    dq_t *tmp;




    int argcounts[] = {1,1,1,2,1,2,1,1,1,2,2,2,2,2,1,2,1,1,1,1,1,2};
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
                write_to_return((uint64_t)malloc(args[0]), virtualmem, oldpc, register_banks);
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
                write_to_return(*(uint64_t*)args[0], virtualmem, oldpc, register_banks);
            break;

            case 7: //print
                printf("%c", args[0]);
            break;

            case 8: //scan
                scanf("%c", &v_register);
            break;

            case 9: //add
                write_to_return(args[0] + args[1], virtualmem, oldpc, register_banks);
            break;

            case 10: //sub
                write_to_return(args[0] - args[1], virtualmem, oldpc, register_banks);
            break;

            case 11: //mul
                write_to_return(args[0] * args[1], virtualmem, oldpc, register_banks);
            break;

            case 12: //div
                write_to_return(args[0] / args[1], virtualmem, oldpc, register_banks);
            break;

            case 13: //setreg
                registers[args[0]] = args[1];
            break;

            case 14: //getreg
                write_to_return(registers[args[0]], virtualmem, oldpc, register_banks);
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

            case 19: //push
                stack->next = (dq_t*)malloc(sizeof(dq_t));
                tmp = stack;
                stack = stack->next;
                stack->prev = tmp;
                stack->value = args[0];
            break;
            case 20: //pop
                write_to_return(stack->value, virtualmem, oldpc, register_banks);
                tmp = stack->prev;
                free(stack);
                stack = tmp;
                tmp->next = NULL;
            break;
            case 21: //mod
                write_to_return(args[0] % args[1], virtualmem, oldpc, register_banks);
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
    
    fflush(stdout);
    return 0;
}