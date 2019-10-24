//g++ Mixin.cpp -I../ -fpermissive -Wno-pointer-arith -Wno-narrowing -std=c++11 -fPIC -o out


//g++ Mixin.cpp -I../ -fpermissive -Wno-pointer-arith -Wno-narrowing -std=c++11 -fPIC -o out -shared && LD_PRELOAD=./out ../dev/factorio/bin/x64/factorio --start-server-load-latest

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
#include "mixinUtils.hpp"
#include "static_block.hpp"
































std::vector<std::string> MixinTargets;
std::vector<void*> MixinCallbacks;
int registerFunctionMixin(std::string target,void* callback)
{
	printf("Registered mixin %s\n",target.c_str());
	MixinTargets.push_back(target);
	MixinCallbacks.push_back(callback);
}

void installFunctionMixins(std::vector<std::string> mixinTargets,std::vector<void*> mixinCallBacks)
{
	//assert(mixinTargets.size()==mixinCallBacks.size());
	std::vector<void*> targetAddresses=findAddressFromSymTable(mixinTargets);
	for(int i=0;i<mixinTargets.size();i++)
	{
		if(targetAddresses[i]==nullptr)
		{
			printf("Unable to get address of %s, not mixing\n",mixinTargets[i].c_str());
			continue;
		}
		installPrefunctionMixin(targetAddresses[i],mixinCallBacks[i]);
	}
	
	
	
}


#define registerFnMixin(target,function) \
static int mixin_register_id_for_function______ __attribute((unused)) = registerFunctionMixin(target,(void*)&function);

void*(*Achievement__getPrototype)(void*)=(void*)findAddressFromSymTable({"_ZNK22BuildEntityAchievement12getPrototypeEv"})[0];
std::string(*LocalisedString__str_raw)(void*)=(void*)findAddressFromSymTable({"_ZNK15LocalisedString7str_rawEv"})[0];
void*(*AchievementPrototype__getLocalisedName)(void*)=(void*)findAddressFromSymTable({"_ZNK9PrototypeI2IDI20AchievementPrototypetEE16getLocalisedNameEv"})[0];
char*(*AchievementPrototype__getType)(void*)=(void*)findAddressFromSymTable({"_ZNK35DeliverByRobotsAchievementPrototype7getTypeEv"})[0];//MIGHT NOT BE CORRECT

void AchivementLoad(void* self,void* tree)
{
	printf("Achivement prototype Loaded, class located at %p\n\n\n\n",self);
	//void* prototype=Achievement__getPrototype(self);
	//printf("prototype located at %p\n\n\n\n",prototype);
	void* LocalisedString=AchievementPrototype__getLocalisedName(self);
	printf("Localised string at: %p\n\n\n\n",LocalisedString);
	printf("Localised name: %s\n\n\n\n",LocalisedString__str_raw(LocalisedString).c_str());
}
registerFnMixin("_ZN35DontBuildEntityAchievementPrototype4loadERK12PropertyTree",AchivementLoad)//"_ZN20AchievementPrototype4loadERK12PropertyTree"



//THIS SYSTEM WONT WORK WITH arguments that have more than 6 arguments (this includes the class pointer)
static void init() __attribute__((constructor));
void init()
{
	printf("Init function called\n");
	
	
	//void* target=addresses[0];
	//installPrefunctionMixin(target,(void*)&testMixin);
		
}
 
//Its called after init() for some reason
static_block
{
	printf("Installing Mixins\n");
	installFunctionMixins(MixinTargets,MixinCallbacks);
	printf("Continuing execution\n");
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