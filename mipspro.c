//Alec Romine
//CDA 3103C

#include "spimcore.h"

/* ALU */ /* 10 Points */ 
void ALU(unsigned A,unsigned B,char ALUControl,unsigned *ALUresult,char *Zero) 
{
    //determines ALU control
    switch ((int) ALUControl)
    {
    case 000:
    	*ALUresult = A + B;
	break;
    case 001:
	*ALUresult = A - B;
	break;
    case 010:
	if ((signed)A < (signed)B)
	{
		*ALUresult = 1;
	}
	else
	{
		*ALUresult = 0;
	}
	break; 
    case 011:	
	if (A < B)
	{
		*ALUresult = 1;
	}
	else
	{
		*ALUresult = 0;
	}
	break;
    case 100:        
	*ALUresult = A & B;
	break;
    case 101:
	*ALUresult = A | B;
	break;
    case 110:
	B << 16;
	break; 
    case 111:
	*ALUresult = ~A;
	break;
	}
    
	
	if(*ALUresult == 0)
        {
		*Zero = 1;
	}	
	else
        {
		*Zero = 0;
	}
}

/* instruction fetch */ /* 10 Points */ 
int instruction_fetch(unsigned PC,unsigned *Mem,unsigned *instruction) 
{
    //correct memory index
    unsigned MemoryIndex = PC >> 2;
    
    //check if instruction is word-aligned 
    if (PC % 4 == 0) 
    {            
        *instruction = Mem[MemoryIndex];
        return 0;
    }
    else
    {   //halts if not word-aligned
	return 1; 
    }
}

/* instruction partition */ /* 10 Points */ 
void instruction_partition(unsigned instruction, unsigned *op, unsigned *r1,unsigned *r2, unsigned *r3, unsigned *funct, unsigned *offset, unsigned *jsec) 
{
    //creating partitions    
    unsigned funPart	= 0x0000003f;
    unsigned rPart	= 0x1f;	    	
    unsigned offsetPart	= 0x0000ffff;	
    unsigned jsecPart	= 0x03ffffff;	

    //adding partitions 
    *op		= (instruction >> 26) & funPart; // [31-26]
    *r1		= (instruction >> 21) & rPart; // [25-21]
    *r2		= (instruction >> 16) & rPart; // [20-16]
    *r3		= (instruction >> 11) & rPart; // [15-11]
    *funct	= instruction & funPart; // [5-0]
    *offset	= instruction & offsetPart; // [15-0]
    *jsec	= instruction & jsecPart; // [25-0]
}

/* instruction decode */ /* 15 Points */ 
int instruction_decode(unsigned op,struct_controls *controls) 
{
    //decodes op code for controls struct  
    switch(op)
    {
        case 0:   //R type
            controls->RegDst = 1; 
            controls->Jump = 0; 
            controls->Branch = 0; 
            controls->MemRead = 0; 
            controls->MemtoReg = 0;
            controls->ALUOp = 7;
            controls->MemWrite = 0; 
            controls->ALUSrc = 0; 
            controls->RegWrite = 1;
            break; 
        case 2:  //jump
   	    controls->RegDst = 0; 
            controls->Jump = 1; 
	    controls->Branch = 0; 
            controls->MemRead = 0; 
	    controls->MemtoReg = 0;
	    controls->ALUOp = 0;
	    controls->MemWrite = 0; 
	    controls->ALUSrc = 0; 
	    controls->RegWrite = 0;
            break;
        case 4:  //branch equal
   	    controls->RegDst = 2; 
            controls->Jump = 0; 
	    controls->Branch = 1; 
	    controls->MemRead = 0; 
	    controls->MemtoReg = 2;
	    controls->ALUOp = 1;
	    controls->MemWrite = 0; 
	    controls->ALUSrc = 0; 
	    controls->RegWrite = 0;
            break;
        case 8:   //addi
            controls->RegDst = 0; 
            controls->Jump = 0; 
            controls->Branch = 0; 
            controls->MemRead = 0; 
            controls->MemtoReg = 0;
            controls->ALUOp = 0;
            controls->MemWrite = 0; 
            controls->ALUSrc = 1; 
            controls->RegWrite = 1;
            break;                                   
        case 10:  //slti
            controls->RegDst = 0; 
            controls->Jump = 0; 
            controls->Branch = 0; 
            controls->MemRead = 0; 
            controls->MemtoReg = 0;
            controls->ALUOp = 2;
            controls->MemWrite = 0; 
            controls->ALUSrc = 1; 
            controls->RegWrite = 1;
            break;          
        case 11:  //sltiu
   	    controls->RegDst = 0; 
            controls->Jump = 0; 
	    controls->Branch = 0; 
	    controls->MemRead = 0; 
	    controls->MemtoReg = 0;
	    controls->ALUOp = 3;
	    controls->MemWrite = 0; 
	    controls->ALUSrc = 1; 
	    controls->RegWrite = 1;
            break;
        case 15:  //load unsigned immediate
   	    controls->RegDst = 0; 
            controls->Jump = 0; 
	    controls->Branch = 0; 
	    controls->MemRead = 0; 
	    controls->MemtoReg = 0;
	    controls->ALUOp = 6;
	    controls->MemWrite = 0; 
	    controls->ALUSrc = 1; 
	    controls->RegWrite = 1;
            break;                 
        case 35:  //load word
   	    controls->RegDst = 0; 
            controls->Jump = 0; 
	    controls->Branch = 0; 
	    controls->MemRead = 1; 
	    controls->MemtoReg = 1;
	    controls->ALUOp = 0;
	    controls->MemWrite = 0; 
	    controls->ALUSrc = 1; 
	    controls->RegWrite = 1;
            break;                          
        case 43:  //store word
   	    controls->RegDst = 2; 
            controls->Jump = 0; 
	    controls->Branch = 0; 
	    controls->MemRead = 0; 
	    controls->MemtoReg = 2;
	    controls->ALUOp = 0;
	    controls->MemWrite = 1; 
	    controls->ALUSrc = 1; 
	    controls->RegWrite = 0;
            break;
        default:    //halt
	    return 1;
	
     } 
       return 0;
}

/* Read Register */ /* 5 Points */ 
void read_register(unsigned r1,unsigned r2,unsigned *Reg,unsigned *data1,unsigned *data2) 
{    
    *data1 = Reg[r1];
    *data2 = Reg[r2];
}

/* Sign Extend */ /* 10 Points */ 
void sign_extend(unsigned offset,unsigned *extended_value) 
{
    //extend for negative
	unsigned extend = 0xFFFF0000;     
	unsigned negative = offset >> 15;

    if (negative == 1)
    {
         *extended_value = offset | extend; 
    }
    else
    {
         *extended_value = offset & 0x0000ffff;
    }
}

/* ALU operations */ /* 10 Points */ 
int ALU_operations(unsigned data1,unsigned data2,unsigned extended_value,unsigned funct,char ALUOp,char ALUSrc,unsigned *ALUresult,char *Zero) 
{
    //determines the source of data2
    if(ALUSrc == 1)
    {
	data2 = extended_value;
    }

    //true for r type instructions 
    if(ALUOp == 7)
    {    	
	switch(funct) 
        {            
            case 32: //add
                ALUOp = 0;
                break;
            case 34: //sub
                ALUOp = 1;
                break;                
            case 36: //and
		ALUOp = 4;
                break;
            case 37: //or
		ALUOp = 5;
		break;
            case 39: //nor
                ALUOp = 7;
		break;
            case 42: //slts
                ALUOp = 2;
		break;
            case 43: //sltu
		ALUOp = 3;
		break;
	    case 6://shift left extended 
		ALUOp = 6;
		break;           
            default: //halt
                return 1;
	}
        
        //enter data and opcode in ALU
        ALU(data1,data2,ALUOp,ALUresult,Zero);        
    }
    else
    {    
    ALU(data1,data2,ALUOp,ALUresult,Zero);
    }
	
    //Return
    return 0;
}
/* Read / Write Memory */ /* 10 Points */ 
int rw_memory(unsigned ALUresult,unsigned data2,char MemWrite,char MemRead,unsigned *memdata,unsigned *Mem) 
{
    
    //writing to memory
    if (MemWrite == 1) 
    {
        //will halt if not word aligned
    	if((ALUresult % 4) == 0)
        {
            Mem[ALUresult >> 2] = data2;
        }	
	else
        {
            return 1;
	}
    }
    
    //reading from memory
    if (MemRead == 1)
    {   //will also halt if not word aligned 
	if((ALUresult % 4) == 0)
        {
            *memdata = Mem[ALUresult >> 2];    
	}	
	else
        {
            return 1;
	}
    }   
    
    return 0;
}

/* Write Register */ /* 10 Points */ 
void write_register(unsigned r2,unsigned r3,unsigned memdata,unsigned ALUresult,char RegWrite,char RegDst,char MemtoReg,unsigned *Reg) 
{
    //if writing, determines where to and from data should be written 
    if(RegWrite == 1)
    {
        if (MemtoReg == 1 && RegDst == 0) 
        {  
            Reg[r2] = memdata;
	}	
        else if(MemtoReg == 1 && RegDst == 1)
        {   
            Reg[r3] = memdata;
	}     	 
	else if (MemtoReg == 0 && RegDst == 0) 
        {  
            Reg[r2] = ALUresult;
	}
	else if (MemtoReg == 0 && RegDst == 1)
        {   
            Reg[r3] = ALUresult;
	}
    }
}

/* PC update */ /* 10 Points */ 
void PC_update(unsigned jsec,unsigned extended_value,char Branch,char Jump,char Zero,unsigned *PC) 
{
    //increment PC by 4
    *PC += 4;

    // if jump then shift jump Instruction and combine with PC
    if(Jump == 1)
    {
    	*PC = (jsec << 2) | (*PC & 0xf0000000);
    }
    
    //If branching and we got a zero properly add extended value
    if(Branch == 1 && Zero == 1)
    {
    *PC += extended_value << 2;
    }
    
}
