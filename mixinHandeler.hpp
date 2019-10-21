#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/mman.h>


int change_page_permissions_of_address(void* addr) {
    int page_size = getpagesize();
    addr -= (unsigned long)addr % page_size;

    if(mprotect(addr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC) == -1) {
        return -1;
    }

    return 0;
}
void set_perms(void* addr,size_t size)
{
	for(void* mem=addr;mem<(size+addr);mem++)
		change_page_permissions_of_address(mem);
}
void* emalloc(size_t size)
{
	void* addr=malloc(size);
	set_perms(addr,size);
	return addr;
}













//helper function
#define S(var,amount) (((uint64_t)var)>>(8*amount))&(0xff)

void installPrefunctionMixin(void* mixinTarget,void* mixinCallback)
{
	char* original_data=(char*)mixinTarget;
	
	void* self_address=emalloc(127);
	
	
	
	unsigned long long calculatedJump = (unsigned long long)(self_address)-(unsigned long long)(mixinTarget)-5;
	
	char mixinhandel[]={
		0x50, 0x51, 0x52, 0x53, 0x56, 0x57, 0x54, 0x55, 0x41, 0x50, 0x41, 0x51, 0x41, 0x52, 0x41, 0x53, 0x41, 0x54, 0x41, 0x55, 0x41, 0x56, 0x41, 0x57, 
		
		0x49, 0xBF,
		S(mixinCallback,0),
		S(mixinCallback,1), 
		S(mixinCallback,2), 
		S(mixinCallback,3), 
		S(mixinCallback,4), 
		S(mixinCallback,5), 
		S(mixinCallback,6), 
		S(mixinCallback,7), 
		
		0x41, 0xFF, 0xD7, 
		0x41, 0x5F, 0x41, 0x5E, 0x41, 0x5D, 0x41, 0x5C, 0x41, 0x5B, 0x41, 0x5A, 0x41, 0x59, 0x41, 0x58, 0x5D, 0x5C, 0x5F, 0x5E, 0x5B, 0x5A, 0x59, 0x58,
		
		//66 bytes bellow
		0x41, 0x57, // push r15
		0x49,0xBF,// Set r15 to mixin target
		S(mixinTarget,0), 
		S(mixinTarget,1), 
		S(mixinTarget,2), 
		S(mixinTarget,3), 
		
		0x00, 0x00, 0x00, 0x00, //I have no idea
		
		0x41, 0xC6, 0x07, //set first byte at mixin target to original
		original_data[0],
		0x41, 0xC6, 0x47, 0x01, //set second byte at mixin target to original... etc 
		original_data[1], 
		0x41, 0xC6, 0x47, 0x02, 
		original_data[2], 
		0x41, 0xC6, 0x47, 0x03, 
		original_data[3], 
		0x41, 0xC6, 0x47, 0x04, 
		original_data[4],
		
		0x41, 0xFF, 0xD7, //Call the mixin target
		
		0x41, 0xC6, 0x07, //setback the data to the injector jmp function
		0xE9,//E9 is jump 
		0x41, 0xC6, 0x47, 0x01, 
		S(calculatedJump,0), 
		0x41, 0xC6, 0x47, 0x02, 
		S(calculatedJump,1), 
		0x41, 0xC6, 0x47, 0x03, 
		S(calculatedJump,2), 
		0x41, 0xC6, 0x47, 0x04, 
		S(calculatedJump,3),
		
		0x41, 0x5F,//pop r15

		0xC3 //ret
	};
	
	memcpy(self_address,(void*)mixinhandel,sizeof(mixinhandel));
	
	
	set_perms(mixinTarget,5);
	
	
	//Set/install mixin
	original_data[0]=0xe9;
	original_data[1]=S(calculatedJump,0);
	original_data[2]=S(calculatedJump,1);
	original_data[3]=S(calculatedJump,2);
	original_data[4]=S(calculatedJump,3);
}