#pragma once
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

using namespace ELFIO;


//Pass in a list of function names, and it will try find the address of those functions
std::vector<void*> findAddressFromSymTable(std::vector<std::string> functions)
{
	std::vector<void*> addresses(functions.size());
	
	
	
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
				
				
				
				for(uint32_t i=0;i<functions.size();i++)
					if(functions[i]==name)
						addresses[i]=(void*)value;
            }
        }
    }
	return addresses;
}




































