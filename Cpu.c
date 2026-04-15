#include <stdint.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    uint8_t r1;        // Register
    uint8_t pc;       // Program Counter
    uint8_t ram[256]; // 256 Byte RAM
    int halted;
    int zero_flag;    // set if r1 == 0
} CPU;

void run(CPU* cpu)
{
    while(!cpu->halted)
    {
        uint8_t operation = cpu->ram[cpu->pc++]; 
        uint8_t arg = cpu->ram[cpu->pc++];

        switch(operation)
        {
            case 0x00: break; //nop -> Does Nothing
            case 0x01: cpu->r1 = arg;break;  //LDA-> Load Argument into register
            case 0x02: cpu->r1 += arg;cpu->zero_flag = (cpu->r1 == 0);break; // ADD-> Register + Argument
            case 0x03: cpu->r1 -= arg;cpu->zero_flag = (cpu->r1 == 0);break; // Sub-> Register - Argument
            case 0x04: cpu->pc = arg;break; // jmp->jump to adress
            case 0x05: if(cpu->zero_flag == 1) cpu->pc = arg;break; //JZ-> jump to adress if zero_flag is set
            case 0x06: cpu->ram[arg] = cpu->r1;break;//STA-> store r1 value in the ram
            case 0x07: cpu->r1 = cpu->ram[arg];break;//LDA-> Load value from ram into r1
            case 0x08: cpu->halted = 1;return;//HLT-> stop process
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
