#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct 
{
    uint8_t r1;         // Register 1
    uint8_t r2;         // Register 2
    uint8_t pc;         // Program Counter
    uint8_t ram[256];   // 256 Byte RAM
    int halted;
    int zero_flag;      // set if r1 == 0
} CPU;

typedef enum : uint8_t
{
    NOP = 0x00,
    LDA_Val = 0x01,
    ADD = 0x02,
    SUB = 0x03,
    JMP = 0x04,
    JZ = 0x05,
    STA = 0x06,
    LDA_Ram = 0x07,
    HLT = 0x08
}OpCode;

void run(CPU* cpu)
{
    while(!cpu->halted)
    {
        uint8_t operation = cpu->ram[cpu->pc++];
        uint8_t arg = cpu->ram[cpu->pc++];

        switch(operation)
        {
            case NOP: //nop -> Does Nothing
            {
                break;
            }
            case LDA_Val: //LDA-> Load Argument into register
            {
                cpu->r1 = arg;
                break;
            }
            case ADD:// ADD-> Register + Argument
            {
                cpu->r1 += arg;
                cpu->zero_flag = (cpu->r1 == 0);
                break;
            }
            case SUB: // Sub-> Register - Argument
            {
                cpu->r1 -= arg;
                cpu->zero_flag = (cpu->r1 == 0);
                break;
            }
            case JMP:// jmp->jump to adress
            {
                cpu->pc = arg;
                break;
            }
            case JZ: //JZ-> jump to adress if zero_flag is set
            {
                if(cpu->zero_flag == 1) cpu->pc = arg;
                break;
            }
            case STA://STA-> store r1 value in the ram
            {
                cpu->ram[arg] = cpu->r1;
                break;
            }
            case LDA_Ram: //LDA-> Load value from ram into r1
            {
                cpu->r1 = cpu->ram[arg];
                break;
            }
            case HLT: //HLT-> stop process
            {
                cpu->halted = 1;
                return;
            }
        }

        // Comment this out to get a specific Log for every instruction
        // printf("PC: %d | OP: 0x%02X | A: %d | RAM[100]: %d\n", 
        // cpu->pc, operation, cpu->r1, cpu->ram[100]);
    }
}

int main()
{
    CPU cpu = {0};

    uint8_t programm[]=
    {
        LDA_Val,4,
        STA, 100,
        LDA_Val, 0,
        STA,101,
        LDA_Ram, 101,
        ADD,3,
        STA, 101,
        LDA_Ram,100,
        SUB,1,
        STA,100,
        JZ, 24,
        JMP, 8,
        HLT,0
    };
    memcpy(cpu.ram, programm, sizeof(programm));
    run(&cpu);
    printf("%d", cpu.ram[101]);
}

/** EXAMPLE: Multiply 3 by 4
    uint8_t programm[]=
    {
        0x01,4,
        0x06, 100,
        0x01, 0,
        0x06,101,
        0x07, 101,
        0x02,3,
        0x06, 101,
        0x07,100,
        0x03,1,
        0x06,100,
        0x05, 24,
        0x04, 8,
        0x08,0
    };
*/
