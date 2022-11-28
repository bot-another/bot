#ifndef ACTUATOR_H
#define ACTUATOR_H
#include <vector>
#include "Compiler.h"
class Actuator{
    private:
        std::vector<instruction> code;
        std::vector<int> stack;
    public:
        Actuator(std::vector<instruction>& code){
            this->code = code;
            stack = std::vector<int>(10000, 0);
        }
        Actuator(GrammarAnalysis& A){
            this->code = A.code;
            stack = std::vector<int>(10000, 0);
        }
        int base(int currentLevel, int levelDiff){
            int b = currentLevel;
            while (levelDiff--)
                b = stack[b];
            return b;
        } // base   
        void interpret(){
            std::cout<<"Start running."<<std::endl;
            int pc = 0;
            int top = -1;
            int b = 0;
            int ret_val = 0;
            instruction i;
            do{
                i = code[pc++];
               	switch (i.f)
                {
                case POP://test
                    if(i.l == 1){
                        ret_val = stack[top--];
                        top-=i.a;
                        stack[++top] = ret_val;
                    }
                    else{
                        top-=i.a;
                    }
                    break;
                case LIT:
                    stack[++top] = i.a;
                    break;
                case LEA:
                    stack[++top] = base(b, i.l) + i.a;
                    break;
                case LODA:
                    stack[top] = stack[stack[top] + i.a + base(b, i.l)];
                    break;
                case STOA:
                    stack[stack[top - 1] + i.a + base(b, i.l)] = stack[top];
                    stack[top - 1] = stack[top];
                    top -= 1;
                    break;
                case OPR:
                    switch (i.a) // operator
                    {
                    case OPR_RET:
                        ret_val = stack[top];
                        top = b - 1;
                        pc = stack[top + 3];
                        b = stack[top + 2];
                        stack[++top] = ret_val;
                        break;
                    case OPR_NEG:
                        stack[top] = -stack[top];
                        break;
                    case OPR_ADD:
                        top--;
                        stack[top] += stack[top + 1];
                        break;
                    case OPR_MIN:
                        top--;
                        stack[top] -= stack[top + 1];
                        break;
                    case OPR_MUL:
                        top--;
                        stack[top] *= stack[top + 1];
                        break;
                    case OPR_DIV:
                        top--;
                        if (stack[top + 1] == 0)
                        {
                            printf("Runtime Error: Divided by zero.\n");
                            printf("Program terminated.\n");
                            exit(0);
                        }
                        stack[top] /= stack[top + 1];
                        break;
                    case OPR_ODD:
                        stack[top] %= 2;
                        break;
                    case OPR_EQU:
                        top--;
                        stack[top] = stack[top] == stack[top + 1];
                        break;
                    case OPR_NEQ:
                        top--;
                        stack[top] = stack[top] != stack[top + 1];
                        break;
                    case OPR_LES:
                        top--;
                        stack[top] = stack[top] < stack[top + 1];
                        break;
                    case OPR_GEQ:
                        top--;
                        stack[top] = stack[top] >= stack[top + 1];
                        break;
                    case OPR_GTR:
                        top--;
                        stack[top] = stack[top] > stack[top + 1];
                        break;
                    case OPR_LEQ:
                        top--;
                        stack[top] = stack[top] <= stack[top + 1];
                        break;
                    } // switch
                    break;
                case LOD:
                    stack[++top] = stack[base(b, i.l) + i.a];
                    break;
                case STO:
                    stack[base(b, i.l) + i.a] = stack[top];
                    // printf("%d\n", stack[top]);
                    // top--;
                    break;
                case CAL:
                    if(i.l == -1){
                        if(i.a == 0){
                            while(stack[top] != (int)'\n'){
                                putchar((char)stack[top--]);
                            }
                            top--;
                            putchar('\n');
                        }
                        else if(i.a == 1){
                            int len = stack[top];
                            int first = top - len;
                            top--;
                            while(len > 0){
                                printf("%d ", stack[first++]);
                                top--;
                                len--;
                            }
                            putchar('\n');
                        }
                        else if(i.a == 2){
                            int len = stack[top];
                            int first = top - len;
                            top--;
                            while(len > 0){
                                scanf("%d", &stack[stack[first++]]);
                                len--;
                                top--;
                            }
                        }
                    }
                    else if(i.l == -2){
                        if(i.a == 0){
                            int addr = stack[top];
                            top--;
                            stack[addr] = pc - 2;
                            stack[addr + 1] = b;
                            stack[addr + 2] = top;
                        }
                        else if(i.a == 1){
                            int addr = stack[top - 1];
                            stack[addr + 3] = stack[top];
                            pc = stack[addr];
                            b = stack[addr + 1];
                            top = stack[addr + 2];
                        }
                    }
                    else{
                        stack[top + 1] = base(b, i.l);
                        // generate new block mark
                        stack[top + 2] = b;
                        stack[top + 3] = pc;
                        b = top + 1;
                        pc = i.a;
                    }
                    break;
                case INT:
                    top += i.a;
                    break;
                case JMP:
                    pc = i.a;
                    break;
                case JPC:
                    if (stack[top] == i.l)
                        pc = i.a;
                    top--;
                    break;
                } // switch
            // printf("top:%d\n", top);
            }while(pc);
        }
};
#endif
