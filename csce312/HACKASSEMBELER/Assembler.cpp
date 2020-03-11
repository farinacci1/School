/*Assembler for Hack Assembally language */
#include <iostream>
#include "Assembler.H"
#include <string.h>
#include <fstream>
#include <bitset>
int main(int argc,char *argv[])
{
    if (argc != 2)
    {
        std::cerr <<"ERROR::INVALID_ARGUMENT_COUNT::PROGRAM_REQUIRES_EXACTLY_2_ARGS" <<std::endl;
         _Exit(1);
    }

    //std::cout <<"Argument count is " << argc << " with arguments provided being "<< argv[0]<< " " << argv[1] << std::endl;
    std::string filePath = std::string(argv[1]);
    std::string fileExt = filePath.substr(filePath.find_last_of(".") + 1);
    std::string fileName = filePath.substr(0,filePath.find_last_of("."));
    std::cout << "File type: " << fileExt << std::endl;
    std::cout << "File name: " << fileName << std::endl;
 
    if (fileExt != "asm")
    {
        std::cerr << "ERROR::INCORRECT_FILE_FORMAT_DETECTED::FILE_TYPE_REQUESTED_IS_ASM"<<std::endl;
        _Exit(1);
    }
    //Firstpass stores program labels with ROM addresses
    //Rom address starts at 0 and increments whenevr A_COMMAND or C_COMMAND encountered
    Parser FirstPass(filePath);
    int ROM_ADDRESS = 0;
    while(true)
    {
        FirstPass.advance();
        if(!FirstPass.hasMoreCommands()) break;
        if(FirstPass.commandType() == "A_COMMAND" || FirstPass.commandType() == "C_COMMAND") ROM_ADDRESS++;
        if(FirstPass.commandType() == "L_COMMAND" && !contains(FirstPass.symbol())) addEntry(FirstPass.symbol(),ROM_ADDRESS);
    }
    //Secondpass parses file replacing symoblic instructions with value in symbol table and complete commands translation
    //allocated RAM addresses are consevtive numbers starting at 16
    std::ofstream outFile;
    std::string outputfile = std::string(fileName) +".hack";
    ROM_ADDRESS = 16;
    outFile.open(outputfile);
    Parser SecondPass(argv[1]);
    while(true)
    {
        SecondPass.advance();
        if(!SecondPass.hasMoreCommands()) break;
        if(SecondPass.commandType() == "A_COMMAND")
        {
            outFile << "0";
            //check if symbol is numeric
            if(SecondPass.symbol().find_first_not_of("0123456789") == std::string::npos)
            {
                outFile <<  std::bitset<15>(std::stoull(SecondPass.symbol(), nullptr)).to_string();
            }
            else
            {
               // Check if the symbol is a variable.
                if (!contains(SecondPass.symbol())) {
                    addEntry(SecondPass.symbol(), ROM_ADDRESS++);
                }
                // Retrieve the address, and convert the decimal number to a binary number.
                outFile << std::bitset<15>(static_cast<unsigned long long>(getAddress(SecondPass.symbol()))).to_string();
            }
            outFile << std::endl;
        }
        else if(SecondPass.commandType() == "C_COMMAND")
        {
            outFile << "111";
            outFile <<comp(SecondPass.comp());
            outFile << dest(SecondPass.dest());
            outFile << jump(SecondPass.jump());
            outFile << std::endl;
        }
    }
    outFile.close();
    return 0;
}