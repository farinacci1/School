/*Assembler for Hack Assembally language */
#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <algorithm>

std::map<std::string,int> SYMBOLS_TABLE = {
   {"R0", 0},
   {"R1", 1},
   {"R2", 2},
   {"R3", 3},
   {"R4", 4},
   {"R5", 5},
   {"R6", 6},
   {"R7", 7},
   {"R8", 8},
   {"R9", 9},
   {"R10", 10},
   {"R11", 11},
   {"R12", 12},
   {"R13", 13},
   {"R14", 14},
   {"R15", 15},
   {"SP", 0X0000},
   {"LCL", 0X0001},
   {"ARG", 0X0002},
   {"THIS", 0X0003},
   {"THAT", 0X0004},
   {"SCREEN", 0X4000},
   {"KBD", 0X6000 }

};

const std::map<std::string,std::string> COMP_TABLE = 
{
    {"0" , "0101010"},
    {"1" , "0111111"},
    {"-1" , "0111010"},
    {"D" , "0001100"},
    {"A" , "0110000"},
    {"M" , "1110000"},
    {"!D" , "0001101"},
    {"!A" , "0110001"},
    {"!M" , "1110001"},
    {"-D" , "0001111"},
    {"-A" , "0110011"},
    {"-M" , "1110011"},
    {"D+1" , "0011111"},
    {"A+1" , "0110111"},
    {"M+1" , "1110111"},
    {"D-1" , "0001110"},
    {"A-1" , "0110010"},
    {"M-1" , "1110010"},
    {"D+A" , "0000010"},
    {"D+M" , "1000010"},
    {"D-A" , "0010011"},
    {"D-M" , "1010011"},
    {"A-D" , "0000111"},
    {"M-D" , "1000111"},
    {"D&A" , "0000000"},
    {"D&M" , "1000000"},
    {"D|A" , "0010101"},
    {"D|M" , "1010101"}

};
const std::map<std::string,std::string> DEST_TABLE = 
{
    {"","000"},
    {"M", "001"},
    {"D", "010"},
    {"MD", "011"},
    {"A", "100"},
    {"AM", "101"},
    {"AD", "110"},
    {"AMD", "111"}
};
const std::map<std::string,std::string> JMP_TABLE = 
{
    {"","000"},
    {"JGT", "001"},
    {"JEQ", "010"},
    {"JGE", "011"},
    {"JLT", "100"},
    {"JNE", "101"},
    {"JLE", "110"},
    {"JMP", "111"}
};
const std::map<char,std::string> CMD_TABLE =
{
    {'@',"A_COMMAND"},
    {'A',"C_COMMAND"},
    {'D',"C_COMMAND"},
    {'M',"C_COMMAND"},
    {'0',"C_COMMAND"},
    {'1',"C_COMMAND"},
    {'-',"C_COMMAND"},
    {'!',"C_COMMAND"},
    {'(',"L_COMMAND"}
};
class Parser
{
    
    public:
        int lineCount = 0;
        std::ifstream file;
        std::string currentCommand;
        //opens input file stream and gets ready to parse it
        Parser(std::string asm_file)
        {
            file.open(asm_file);
            if(!file.is_open())
            {
                std::cerr << "ERROR::FAILURE_TO_READ_FROM_FILE"<<std::endl;
                _Exit(1);
            }
        }
        //Are there any more commands in the input?
        bool hasMoreCommands(void)
        {
            return !file.eof();

        }
        //reads the next input and makes it the current command. Should only be called if
        //hasMoreCommands() = true
        //initally there is no current command
        void advance(void)
        {
            bool commandFound = false;
            std::string line = "";
            int lineNum;
            while (std::getline (file,line) && !commandFound )
            {
                
                //remove all instances of white space
                line.erase (std::remove_if (line.begin(), line.end(), ::isspace), line.end());
                //remove comments
                std::size_t commentPos = line.find("//");
                if (commentPos != std::string::npos) {
                    line.erase(commentPos, line.length() - commentPos);
                }
                //check if line is empty
                if (!line.empty()) commandFound = true;
            }
            //set line to be current command
            currentCommand = line;   
        }
        //returns the type of the current command
        std::string commandType(void)
        {
            if (CMD_TABLE.find(currentCommand[0]) != CMD_TABLE.end()) {
                return CMD_TABLE.at(currentCommand[0]);
            }
            std::cerr << "ERROR::commandType::INVALID_SYNTAX"<<std::endl;
            _Exit(1);             
        }
        //Returns the symbol or decimal Xxx of the current command @Xxx or (xxx)
        //should be called only when commandType is A_COMMAND or L_COMMAND
        std::string symbol(void)
        {
            // if commandType = A_COMMAND
            if(currentCommand.at(0) == '@') 
                return currentCommand.substr(1,(currentCommand.length()-1));
            //if commandType = L_COMMAND
            std::size_t oBracket=currentCommand.find('(');
            std::size_t cBacket=currentCommand.find(')');            
            if(oBracket != std::string::npos && cBacket != std::string::npos )
            {
                return currentCommand.substr(oBracket + 1, cBacket - oBracket - 1);
            }
            return "";
        }
        //Returns the dest mnemonic in the current C-command (8 possibilites).
        //should be called only when commandType() is C_COMMAND
        std::string dest(void)
        {
            //get destination of assignment
            std::size_t equal=currentCommand.find('=');
            if (equal != std::string::npos) {
                return currentCommand.substr(0, equal);
            }
            return "";             
        }
        //Returns the comp mnemonic in the current C-command(28 possibilites).
        //Should be called only when commandType() is C_COMMAND
        std::string comp(void)
        {
            std::size_t equal=currentCommand.find('=');
            std::size_t sColon = currentCommand.find(';');
            //dest=comp;jump
            if(equal != std::string::npos && sColon != std::string::npos)
                 return currentCommand.substr(equal + 1, sColon - equal - 1);
            //dest=comp
            else if(equal != std::string::npos)
                 return currentCommand.substr(equal + 1, currentCommand.length() - equal - 1);
            //comp;jmp
            else if(sColon != std::string::npos)
                 return currentCommand.substr(0, sColon);
            return "";
        }
        //Returns the jump mnemonix in the current C-command (8 possibilities).
        //Should be called only when commandType() is C_COMMAND
        std::string jump(void)
        {
            std::size_t sColon = currentCommand.find(';');
            if (sColon != std::string::npos) {
                return currentCommand.substr(sColon + 1, currentCommand.length() - sColon - 1);
            }
            return "";           
        }        
};

//returns binary code of the dest mnemonic
//returns 3 bits
inline std::string dest(std::string mnemonic)
{
    if(DEST_TABLE.find(mnemonic) != DEST_TABLE.end())
        return DEST_TABLE.at(mnemonic);
    std::cerr << "ERROR::dest::INVALID_SYNTAX"<<std::endl;
    _Exit(1);    
}
//returns binary code of the comp mnemonic
//returns 7 bits
inline std::string comp(std::string mnemonic)
{
    if(COMP_TABLE.find(mnemonic) != COMP_TABLE.end())
        return COMP_TABLE.at(mnemonic);
    std::cerr << "ERROR::comp::INVALID_SYNTAX"<<std::endl;
    _Exit(1);    

}
//returns binary code of the jump mnemonic
//returns 3 bits
inline std::string jump(std::string mnemonic)
{
    if(JMP_TABLE.find(mnemonic) != JMP_TABLE.end())
        return JMP_TABLE.at(mnemonic);
    std::cerr << "ERROR::jump::INVALID_SYNTAX"<<std::endl;
    _Exit(1);        
          
}        
//Adds the pair (symbol, adress) to the table
inline void addEntry(std::string symbol, int address)
{
    SYMBOLS_TABLE.insert({symbol,address});
}
//Does the symbol table contain the given symbol?
inline bool contains(std::string symbol)
{
    if(SYMBOLS_TABLE.count(symbol) == 1)return true;
    return false;
}
//Returns the address associated with the symbol
inline int getAddress(std::string symbol)
{
    return SYMBOLS_TABLE.at(symbol);
}
