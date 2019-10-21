//g++ Mixin.cpp -I../ -fpermissive -Wno-pointer-arith -Wno-narrowing -std=c++11 -fPIC -o out

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h> 
#include <thread>
#include <vector>
#include <map>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>
#include <fstream>
#include <streambuf>

#include <elfio/elfio.hpp>
#include "mixinHandeler.hpp"







using namespace ELFIO;
void* findFunctionAddress(std::string functionName)
{
	elfio reader;
    
    // Load ELF data
    if ( !reader.load("/proc/self/exe") ) {
        printf("Can't find or process ELF file\n");
    }
	// Print ELF file segments info
	Elf_Half sec_num = reader.sections.size();
    for ( int i = 0; i < sec_num; ++i ) {
        section* psec = reader.sections[i];
        // Check section type
        if ( psec->get_type() == SHT_SYMTAB ) {
            const symbol_section_accessor symbols( reader, psec );
            for ( unsigned int j = 0; j < symbols.get_symbols_num(); ++j ) {
                std::string   name;
                Elf64_Addr    value;
                Elf_Xword     size;
                unsigned char bind;
                unsigned char type;
                Elf_Half      section_index;
                unsigned char other;
                
                // Read symbol properties
                symbols.get_symbol( j, name, value, size, bind,
                                       type, section_index, other );
				if(name==functionName)
					return (void*)value;
            }
        }
    }
	return 0;
}





















/*
void testInject(int a)
{
	printf("ello_ %i\n",a);
}*/


void testMixin(void* lua_State )
{
	printf("lua_State at %p\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n",lua_State);
}



static void init() __attribute__((constructor));
void init()
{
	printf("Installing Mixins\n");
	void* target=findFunctionAddress("_Z15luaopen_serpentP9lua_State");
	installPrefunctionMixin(target,(void*)&testMixin);
		
}


















/*
//inject Code
{

push eax
push edx


mov eax 0xMIXIN ADDRESS









pop eax
pop edx
	
	
	
	
}

//Make redirect function and inject at header of wanted function
//when function is called and redirected, overwrite the jump with the original code
//push self onto the stack so when it returns it returns to the injected bit
//then put the jump code back


//EACH MIXIN WILL 95%CHANCE NEED ITS OWN JUMP handeling function cause theres no way to see where the jump came from

//ANOTHER possibility (maybe) is that there are 2 handelers a jump in, which runs the event sets the original data back then another func which resets the event (much harder to do)


using namespace ELFIO;
void mapFunctionNamesToAddress()
{
	
}
push rax
push rcx
push rdx
push rbx
push rsi
push rdi
push rsp
push rbp
push r8
push r9
push r10
push r11
push r12
push r13
push r14
push r15















pop rax
pop rcx
pop rdx
pop rbx
pop rsi
pop rdi
pop rsp
pop rbp
pop r8
pop r9
pop r10
pop r11
pop r12
pop r13
pop r14
pop r15





















#THis system wont work if the mixin target either needed stuff from the stack or has more than 6 arguments

#here push all registers call mixin function pop all registers

push r15


#set bytes back to original data/state
mov r15, 0xDEEDBEEF # pointer to mixin target (32 bit address)
mov BYTE PTR [r15], 0x1 # set the jump data back to original
mov BYTE PTR [r15+0x1], 0x2#might be able to just use a DWORD (32 bits) and set like that, might be faster and more space efficent
mov BYTE PTR [r15+0x2], 0x3
mov BYTE PTR [r15+0x3], 0x4
mov BYTE PTR [r15+0x4], 0x5
mov BYTE PTR [r15+0x5], 0x2
mov BYTE PTR [r15+0x6], 0x3
mov BYTE PTR [r15+0x7], 0x4
mov BYTE PTR [r15+0x8], 0x5



call r15




#put back the mixin target jmp injection

mov BYTE PTR [r15], 0x1 # set the jump data back to original
mov BYTE PTR [r15+0x1], 0x2#might be able to just use a DWORD (32 bits) and set like that, might be faster
mov BYTE PTR [r15+0x2], 0x3
mov BYTE PTR [r15+0x3], 0x4
mov BYTE PTR [r15+0x4], 0x5
mov BYTE PTR [r15+0x5], 0x2
mov BYTE PTR [r15+0x6], 0x3
mov BYTE PTR [r15+0x7], 0x4
mov BYTE PTR [r15+0x8], 0x5


pop r15

ret #ret to before the mixin target
*/