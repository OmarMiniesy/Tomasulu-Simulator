#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
using namespace std;

struct Load_FU
{
    int Vj;
    // int Vk;
    string Qj;
    // int Qk;
    bool busy;
    int offset;
    int address;
    int cycles;
    string operation;
    string name;
};

struct Store_FU
{
    int Vj;
    // int Vk;
    string Qj;
    // int Qk;
    bool busy;
    int offset;
    int address;
    int cycles;
    string operation;
    string name;
};

struct BNE_FU
{
    int Vj;
    int Vk;
    string Qj;
    string Qk;
    bool busy;
    int offset;
    int cycles;
    string operation;
    string name;
};

struct JAL_FU
{
    int Vj;
    // int Vk;
    string Qj;
    // int Qk;
    bool busy;
    int offset;
    int cycles;
    string operation;
    string name;
};

struct Add_FU
{
    int Vj;
    int Vk;
    string Qj;
    string Qk;
    bool busy;
    int cycles;
    string operation;
    string name;
};

struct Neg_FU
{
    int Vj;
    // int Vk;
    string Qj;
    // int Qk;
    bool busy;
    int cycles;
    string operation;
    string name;
};

struct Nand_FU
{
    int Vj;
    int Vk;
    string Qj;
    string Qk;
    bool busy;
    int cycles;
    string operation;
    string name;
};

struct Sll_FU
{
    int Vj;
    int Vk;
    string Qj;
    string Qk;
    bool busy;
    int cycles;
    string operation;
    string name;
};

void initialize_reservation_stations(int num_add, int num_load, int num_store,
                                     int num_neg, int num_nand, int num_jal, int num_bne, int num_sll,
                                     vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
                                     vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
                                     vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs, int add_cycles, int load_cycles,
                                     int store_cycles, int neg_cycles, int nand_cycles, int jal_cycles, int bne_cycles,
                                     int sll_cycles)
{
    for (int i = 0; i < num_add; i++)
    {
        Add_FU temp;
        temp.busy = false;
        temp.name = "ADD" + to_string(i);
        temp.Vj = 0;
        temp.Vk = 0;
        temp.Qj = "";
        temp.Qk = "";
        temp.operation = "";
        temp.cycles = add_cycles;
        Add_FUs.push_back(temp);
    }
    for (int i = 0; i < num_load; i++)
    {
        Load_FU temp;
        temp.busy = false;
        temp.name = "LOAD" + to_string(i);
        temp.offset = 0;
        temp.address = 0;
        temp.Qj = "";
        temp.Vj = 0;
        temp.operation = "";
        temp.cycles = load_cycles;
        Load_FUs.push_back(temp);
    }
    for (int i = 0; i < num_store; i++)
    {
        Store_FU temp;
        temp.busy = false;
        temp.name = "STORE" + to_string(i);
        temp.offset = 0;
        temp.address = 0;
        temp.Qj = "";
        temp.Vj = 0;
        temp.operation = "";
        temp.cycles = store_cycles;
        Store_FUs.push_back(temp);
    }
    for (int i = 0; i < num_neg; i++)
    {
        Neg_FU temp;
        temp.busy = false;
        temp.name = "NEG" + to_string(i);
        temp.Vj = 0;
        temp.Qj = "";
        temp.operation = "";
        temp.cycles = neg_cycles;
        Neg_FUs.push_back(temp);
    }
    for (int i = 0; i < num_nand; i++)
    {
        Nand_FU temp;
        temp.busy = false;
        temp.name = "NAND" + to_string(i);
        temp.Vj = 0;
        temp.Vk = 0;
        temp.Qj = "";
        temp.Qk = "";
        temp.operation = "";
        temp.cycles = nand_cycles;
        Nand_FUs.push_back(temp);
    }
    for (int i = 0; i < num_jal; i++)
    {
        JAL_FU temp;
        temp.busy = false;
        temp.name = "JAL" + to_string(i);
        temp.Vj = 0;
        temp.Qj = "";
        temp.operation = "";
        temp.offset = 0;
        temp.cycles = jal_cycles;
        JAL_FUs.push_back(temp);
    }
    for (int i = 0; i < num_bne; i++)
    {
        BNE_FU temp;
        temp.busy = false;
        temp.name = "BNE" + to_string(i);
        temp.Vj = 0;
        temp.Vk = 0;
        temp.Qj = "";
        temp.Qk = "";
        temp.operation = "";
        temp.offset = 0;
        temp.cycles = bne_cycles;
        BNE_FUs.push_back(temp);
    }
    for (int i = 0; i < num_sll; i++)
    {
        Sll_FU temp;
        temp.busy = false;
        temp.name = "SLL" + to_string(i);
        temp.Vj = 0;
        temp.Vk = 0;
        temp.Qj = "";
        temp.Qk = "";
        temp.operation = "";
        temp.cycles = sll_cycles;
        Sll_FUs.push_back(temp);
    }
}

unordered_map<string, string> Register_Status; // key: name of register, value: Qi

void initialize_register_status()
{
    for (int i = 0; i < 8; i++)
    {
        string name = "R" + to_string(i);
        Register_Status[name] = "";
    }
}

unordered_map<string, int> Register_File; // key: name of register, value: value

void initialize_registers_map()
{
    for (int i = 0; i < 8; i++)
    {
        // convert i to string Ri
        string Ri = "R" + to_string(i);
        Register_File[Ri] = 0;
    }
}

void initialize_registers_file(string filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        // Register Ri and value separated by space
        string Ri = line.substr(0, line.find(" "));
        int value = stoi(line.substr(line.find(" ") + 1));
        Register_File[Ri] = value;
    }
    file.close();
}

unordered_map<int, int> memory;

void initialize_memory_map(int starting_address)
{
    int size = 128 / 4 * 1024;
    for (int i = starting_address; i < size + starting_address; i++)
    {
        memory[i] = 0;
    }
}

void initialize_memory_file(string filename)
{
    ifstream file(filename);
    string line;
    while (getline(file, line))
    {
        // address and value separated by space
        int address = stoi(line.substr(0, line.find(" ")));
        int value = stoi(line.substr(line.find(" ") + 1));
        memory[address] = value;
    }
    file.close();
}

struct Instruction
{
    int rs1;
    int rs2;
    int rd;
    int imm;
    bool issued;
    int issue_cycle;
    bool executed;
    int execute_cycle; // cycles remaining for execution
    int execute_time;  // cycle when execution finished
    int execeute_begin_time;
    bool readyToExecute;
    bool written;
    int write_cycle;
    bool branched;
    string operation;
    string unit;
    string rawInstruction;
    bool waiting;
};
vector<string> RawInstructions;
vector<string> Instruction_Types;
vector<Instruction> Instructions;
vector<Instruction> Issued_Instructions;
vector<Instruction> executedInstructions;

vector<Instruction> ISSUE_TABLE;
vector<Instruction> EXECUTE_BEGIN_TABLE;
vector<Instruction> EXECUTE_END_TABLE;
vector<Instruction> WRITE_TABLE;

// read instructions from file and store in vector
void read_instructions(string file_name)
{
    ifstream file(file_name);
    string line;
    while (getline(file, line))
    {
        RawInstructions.push_back(line);
    }
    file.close();
}

void read_instruction_types(string file_name)
{
    ifstream file(file_name);
    string line;
    while (getline(file, line))
    {
        string type = "";
        for (int i = 0; i < line.size(); i++)
        {
            if (line[i] == ' ')
            {
                break;
            }
            type += line[i];
        }
        Instruction_Types.push_back(type);
    }
    file.close();
}

void parse_instructions(int starting_address)
{
    for (int i = 0; i < Instruction_Types.size(); i++)
    {
        Instruction temp;
        if (Instruction_Types[i] == "ADD")
        {
            temp.operation = "ADD";
            temp.issued = false;
            temp.executed = false;
            temp.written = false;
            temp.readyToExecute = true;
            temp.branched = false;
            temp.issue_cycle = 0;
            temp.execeute_begin_time = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.imm = 0;
            temp.rs1 = RawInstructions[i][8] - '0';
            temp.rs2 = RawInstructions[i][11] - '0';
            temp.rd = RawInstructions[i][5] - '0';
            temp.waiting = false;
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "ADDI")
        {
            temp.operation = "ADDI";
            temp.issued = false;
            temp.executed = false;
            temp.written = false;
            temp.readyToExecute = true;
            temp.execute_time = 0;
            temp.execeute_begin_time = 0;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.write_cycle = 0;
            temp.waiting = false;
            temp.rs1 = RawInstructions[i][9] - '0';
            temp.rd = RawInstructions[i][6] - '0';
            // go to the 11th character and read until the end
            string imm = "";
            for (int j = 11; j < RawInstructions[i].size(); j++)
            {
                imm += RawInstructions[i][j];
            }
            temp.imm = stoi(imm);
            temp.rs2 = 0;
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "LOAD")
        {
            temp.operation = "LOAD";
            temp.issued = false;
            temp.executed = false;
            temp.execeute_begin_time = 0;
            temp.readyToExecute = true;
            temp.branched = false;
            temp.execute_time = 0;
            temp.written = false;
            temp.issue_cycle = 0;
            temp.waiting = false;
            temp.execute_cycle = 0;
            temp.write_cycle = 0;
            // go to the 8th character and read the number until the first (
            string offset = "";
            for (int j = 8; j < RawInstructions[i].size(); j++)
            {
                if (RawInstructions[i][j] == '(')
                {
                    break;
                }
                offset += RawInstructions[i][j];
            }
            temp.imm = stoi(offset) + starting_address;
            temp.rd = RawInstructions[i][6] - '0';
            int size = RawInstructions[i].size();
            temp.rs1 = RawInstructions[i][size - 2] - '0';
            temp.rs2 = 0;
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "STORE")
        {
            temp.operation = "STORE";
            temp.issued = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.execeute_begin_time = 0;
            temp.branched = false;
            temp.execute_time = 0;
            temp.waiting = false;
            temp.written = false;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.write_cycle = 0;
            // go to the 8th character and read the number until the first (
            string offset = "";
            for (int j = 8; j < RawInstructions[i].size(); j++)
            {
                if (RawInstructions[i][j] == '(')
                {
                    break;
                }
                offset += RawInstructions[i][j];
            }
            temp.imm = stoi(offset) + starting_address;
            temp.rs1 = RawInstructions[i][7] - '0';
            int size = RawInstructions[i].size();
            temp.rd = RawInstructions[i][size - 2] - '0';
            temp.rs2 = 0;
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "NEG")
        {
            temp.operation = "NEG";
            temp.issued = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.execeute_begin_time = 0;
            temp.branched = false;
            temp.written = false;
            temp.waiting = false;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.imm = 0;
            temp.rs1 = RawInstructions[i][8] - '0';
            temp.rs2 = 0;
            temp.rd = RawInstructions[i][5] - '0';
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "NAND")
        {
            temp.operation = "NAND";
            temp.issued = false;
            temp.waiting = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.execeute_begin_time = 0;
            temp.branched = false;
            temp.written = false;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.imm = 0;
            temp.rd = RawInstructions[i][6] - '0';
            temp.rs1 = RawInstructions[i][9] - '0';
            temp.rs2 = RawInstructions[i][12] - '0';
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "JAL")
        {
            temp.operation = "JAL";
            temp.issued = false;
            temp.readyToExecute = true;
            temp.branched = true;
            temp.executed = false;
            temp.execeute_begin_time = 0;
            temp.waiting = false;
            temp.written = false;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.rs1 = 0;
            temp.rs2 = 0;
            temp.rd = 0;
            // read from the 4th character until the end
            string offset = "";
            for (int j = 4; j < RawInstructions[i].size(); j++)
            {
                offset += RawInstructions[i][j];
            }
            temp.imm = stoi(offset);
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "BNE")
        {
            temp.operation = "BNE";
            temp.issued = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.branched = true;
            temp.written = false;
            temp.execeute_begin_time = 0;
            temp.waiting = false;
            temp.issue_cycle = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.rs1 = RawInstructions[i][5] - '0';
            temp.rs2 = RawInstructions[i][8] - '0';
            temp.rd = 0;
            // read from the 10th character until the end
            string offset = "";
            for (int j = 10; j < RawInstructions[i].size(); j++)
            {
                offset += RawInstructions[i][j];
            }
            temp.imm = stoi(offset);
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "SLL")
        {
            temp.operation = "SLL";
            temp.issued = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.branched = false;
            temp.written = false;
            temp.execeute_begin_time = 0;
            temp.issue_cycle = 0;
            temp.waiting = false;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.rd = RawInstructions[i][5] - '0';
            temp.rs1 = RawInstructions[i][8] - '0';
            temp.rs2 = RawInstructions[i][11] - '0';
            temp.imm = 0;
            Instructions.push_back(temp);
        }
        else if (Instruction_Types[i] == "RET")
        {
            temp.operation = "RET";
            temp.issued = false;
            temp.executed = false;
            temp.readyToExecute = true;
            temp.branched = true;
            temp.written = false;
            temp.issue_cycle = 0;
            temp.execeute_begin_time = 0;
            temp.execute_cycle = 0;
            temp.execute_time = 0;
            temp.write_cycle = 0;
            temp.rd = 0;
            temp.waiting = false;
            temp.rs1 = 0;
            temp.rs2 = 0;
            temp.imm = 0;
            Instructions.push_back(temp);
        }
    }
}

vector<Instruction> loadStore_queue;
int num_branches = 0;
int num_misses = 0;

void resetStation(vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
                  vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
                  vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs, string name, int &rd)
{
    for (int i = 0; i < Add_FUs.size(); i++)
    {
        if (Add_FUs[i].Qj == name)
        {
            Add_FUs[i].Qj = "";
            Add_FUs[i].Vj = rd;
        }
        if (Add_FUs[i].Qk == name)
        {
            Add_FUs[i].Qk = "";
            Add_FUs[i].Vk = rd;
        }
    }
    for (int i = 0; i < Load_FUs.size(); i++)
    {
        if (Load_FUs[i].Qj == name)
        {
            Load_FUs[i].Qj = "";
            Load_FUs[i].Vj = rd;
        }
    }
    for (int i = 0; i < Store_FUs.size(); i++)
    {
        if (Store_FUs[i].Qj == name)
        {
            Store_FUs[i].Qj = "";
            Store_FUs[i].Vj = rd;
        }
    }
    for (int i = 0; i < Neg_FUs.size(); i++)
    {
        if (Neg_FUs[i].Qj == name)
        {
            Neg_FUs[i].Qj = "";
            Neg_FUs[i].Vj = rd;
        }
    }
    for (int i = 0; i < Nand_FUs.size(); i++)
    {
        if (Nand_FUs[i].Qj == name)
        {
            Nand_FUs[i].Qj = "";
            Nand_FUs[i].Vj = rd;
        }
        if (Nand_FUs[i].Qk == name)
        {
            Nand_FUs[i].Qk = "";
            Nand_FUs[i].Vk = rd;
        }
    }
    for (int i = 0; i < Sll_FUs.size(); i++)
    {
        if (Sll_FUs[i].Qj == name)
        {
            Sll_FUs[i].Qj = "";
            Sll_FUs[i].Vj = rd;
        }
        if (Sll_FUs[i].Qk == name)
        {
            Sll_FUs[i].Qk = "";
            Sll_FUs[i].Vk = rd;
        }
    }
    for (int i = 0; i < BNE_FUs.size(); i++)
    {
        if (BNE_FUs[i].Qj == name)
        {
            BNE_FUs[i].Qj = "";
            BNE_FUs[i].Vj = rd;
        }
        if (BNE_FUs[i].Qk == name)
        {
            BNE_FUs[i].Qk = "";
            BNE_FUs[i].Vk = rd;
        }
    }
    for (int i = 0; i < JAL_FUs.size(); i++)
    {
        if (JAL_FUs[i].Qj == name)
        {
            JAL_FUs[i].Qj = "";
            JAL_FUs[i].Vj = rd;
        }
    }
}

bool issue(string InstructionType, Instruction &Instruction, vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
           vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
           vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs, int &Current_Time, int &issue_index, bool &setBool, bool &setBranch)

{
    if (InstructionType == "ADD")
    {
        for (int i = 0; i < Add_FUs.size(); i++)
        {
            if (Add_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.issue_cycle = Current_Time;
                Add_FUs[i].busy = true;
                Add_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                string reg3 = "R" + to_string(Instruction.rs2);
                if (Register_Status[reg2] == "")
                {
                    Add_FUs[i].Vj = Instruction.rs1;
                    Add_FUs[i].Qj = "";
                }
                else
                {
                    Add_FUs[i].Qj = Register_Status[reg2];
                    Add_FUs[i].Vj = 0;
                }

                if (Register_Status[reg3] == "")
                {
                    Add_FUs[i].Vk = Instruction.rs2;
                    Add_FUs[i].Qk = "";
                }
                else
                {
                    Add_FUs[i].Qk = Register_Status[reg3];
                    Add_FUs[i].Vk = 0;
                }

                Instruction.unit = "ADD" + to_string(i);
                Register_Status[reg1] = "ADD" + to_string(i);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "ADDI")
    {
        for (int i = 0; i < Add_FUs.size(); i++)
        {
            if (Add_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "ADD" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                Add_FUs[i].busy = true;
                Add_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                if (Register_Status[reg2] == "")
                {
                    Add_FUs[i].Vj = Instruction.rs1;
                    Add_FUs[i].Qj = "";
                }
                else
                {
                    Add_FUs[i].Qj = Register_Status[reg2];
                    Add_FUs[i].Vj = 0;
                }
                Add_FUs[i].Vk = Instruction.imm;
                Add_FUs[i].Qk = "";
                Register_Status[reg1] = "ADD" + to_string(i);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "LOAD")
    {
        for (int i = 0; i < Load_FUs.size(); i++)
        {
            if (Load_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "LOAD" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                Load_FUs[i].busy = true;
                Load_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                if (Register_Status[reg2] == "")
                {
                    Load_FUs[i].Vj = Instruction.rs1;
                    Load_FUs[i].Qj = "";
                }
                else
                {
                    Load_FUs[i].Qj = Register_Status[reg2];
                    Load_FUs[i].Vj = 0;
                }
                Load_FUs[i].offset = Instruction.imm;
                Register_Status[reg1] = "LOAD" + to_string(i);
                loadStore_queue.push_back(Instruction);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }

    if (InstructionType == "STORE")
    {
        for (int i = 0; i < Store_FUs.size(); i++)
        {
            if (Store_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "STORE" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                Store_FUs[i].busy = true;
                Store_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                if (Register_Status[reg2] == "")
                {
                    Store_FUs[i].Vj = Instruction.rs1;
                    Store_FUs[i].Qj = "";
                }
                else
                {
                    Store_FUs[i].Qj = Register_Status[reg2];
                    Store_FUs[i].Vj = 0;
                }
                Store_FUs[i].offset = Instruction.imm;
                Register_Status[reg1] = "STORE" + to_string(i);
                loadStore_queue.push_back(Instruction);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "NEG")
    {
        for (int i = 0; i < Neg_FUs.size(); i++)
        {
            if (Neg_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.unit = "NEG" + to_string(i);
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.issue_cycle = Current_Time;
                Neg_FUs[i].busy = true;
                Neg_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                if (Register_Status[reg2] == "")
                {
                    Neg_FUs[i].Vj = Instruction.rs1;
                    Neg_FUs[i].Qj = "";
                }
                else
                {
                    Neg_FUs[i].Qj = Register_Status[reg2];
                    Neg_FUs[i].Vj = 0;
                }
                Register_Status[reg1] = "NEG" + to_string(i);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "NAND")
    {
        for (int i = 0; i < Nand_FUs.size(); i++)
        {
            if (Nand_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "NAND" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                Nand_FUs[i].busy = true;
                Nand_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                string reg3 = "R" + to_string(Instruction.rs2);
                if (Register_Status[reg2] == "")
                {
                    Nand_FUs[i].Vj = Instruction.rs1;
                    Nand_FUs[i].Qj = "";
                }
                else
                {
                    Nand_FUs[i].Qj = Register_Status[reg2];
                    Nand_FUs[i].Vj = 0;
                }
                if (Register_Status[reg3] == "")
                {
                    Nand_FUs[i].Vk = Instruction.rs2;
                    Nand_FUs[i].Qk = "";
                }
                else
                {
                    Nand_FUs[i].Qk = Register_Status[reg3];
                    Nand_FUs[i].Vk = 0;
                }
                Register_Status[reg1] = "NAND" + to_string(i);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "JAL")
    {
        for (int i = 0; i < JAL_FUs.size(); i++)
        {
            if (JAL_FUs[i].busy == false)
            {
                Instruction.issued = true;
                setBool = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "JAL" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                JAL_FUs[i].busy = true;
                JAL_FUs[i].operation = Instruction.operation;
                if (Register_Status["R1"] == "")
                {
                    JAL_FUs[i].Vj = Instruction.imm;
                    JAL_FUs[i].Qj = "";
                }
                else
                {
                    JAL_FUs[i].Qj = Register_Status["R1"];
                    JAL_FUs[i].Vj = 0;
                }
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "RET")
    {
        for (int i = 0; i < JAL_FUs.size(); i++)
        {
            if (JAL_FUs[i].busy == false)
            {
                Instruction.issued = true;
                setBool = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "JAL" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                JAL_FUs[i].busy = true;
                JAL_FUs[i].operation = Instruction.operation;
                // string reg1 = "R" + to_string(Instruction.rd);
                if (Register_Status["R1"] == "")
                {
                    JAL_FUs[i].Vj = Register_File["R1"];
                    JAL_FUs[i].Qj = "";
                }
                else
                {
                    JAL_FUs[i].Qj = Register_Status["R1"];
                    JAL_FUs[i].Vj = 0;
                }
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "BNE")
    {
        for (int i = 0; i < BNE_FUs.size(); i++)
        {
            if (BNE_FUs[i].busy == false)
            {
                Instruction.issued = true;
                num_branches++;
                setBranch = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "BNE" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                BNE_FUs[i].busy = true;
                BNE_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rs1);
                string reg2 = "R" + to_string(Instruction.rs2);
                if (Register_Status[reg1] == "")
                {
                    BNE_FUs[i].Vj = Instruction.rs1;
                    BNE_FUs[i].Qj = "";
                }
                else
                {
                    BNE_FUs[i].Qj = Register_Status[reg1];
                    BNE_FUs[i].Vj = 0;
                }
                if (Register_Status[reg2] == "")
                {
                    BNE_FUs[i].Vk = Instruction.rs2;
                    BNE_FUs[i].Qk = "";
                }
                else
                {
                    BNE_FUs[i].Qk = Register_Status[reg2];
                    BNE_FUs[i].Vk = 0;
                }
                BNE_FUs[i].offset = Instruction.imm;
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    if (InstructionType == "SLL")
    {
        for (int i = 0; i < Sll_FUs.size(); i++)
        {
            if (Sll_FUs[i].busy == false)
            {
                if (setBool)
                {
                    break;
                }
                if (setBranch)
                {
                    Instruction.waiting = true;
                }
                Instruction.issued = true;
                Instruction.rawInstruction = RawInstructions[issue_index];
                Instruction.unit = "SLL" + to_string(i);
                Instruction.issue_cycle = Current_Time;
                Sll_FUs[i].busy = true;
                Sll_FUs[i].operation = Instruction.operation;
                string reg1 = "R" + to_string(Instruction.rd);
                string reg2 = "R" + to_string(Instruction.rs1);
                string reg3 = "R" + to_string(Instruction.rs2);
                if (Register_Status[reg2] == "")
                {
                    Sll_FUs[i].Vj = Instruction.rs1;
                    Sll_FUs[i].Qj = "";
                }
                else
                {
                    Sll_FUs[i].Qj = Register_Status[reg2];
                    Sll_FUs[i].Vj = 0;
                }
                if (Register_Status[reg3] == "")
                {
                    Sll_FUs[i].Vk = Instruction.rs2;
                    Sll_FUs[i].Qk = "";
                }
                else
                {
                    Sll_FUs[i].Qk = Register_Status[reg3];
                    Sll_FUs[i].Vk = 0;
                }
                Register_Status[reg1] = "SLL" + to_string(i);
                Issued_Instructions.push_back(Instruction);
                return true;
            }
        }
    }
    return false;
}

void execute(Instruction &Instruction, vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
             vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
             vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs, int Current_Time)
{
    if (Instruction.operation == "ADD")
    {
        for (int j = 0; j < Add_FUs.size(); j++)
        {
            if (Add_FUs[j].busy == true && Instruction.unit == Add_FUs[j].name && Add_FUs[j].Qj == "" && Add_FUs[j].Qk == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = Add_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "ADDI")
    {
        for (int j = 0; j < Add_FUs.size(); j++)
        {
            if (Add_FUs[j].busy == true && Instruction.unit == Add_FUs[j].name && Add_FUs[j].Qj == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = Add_FUs[j].cycles;
                    Instruction.execeute_begin_time = Current_Time;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "NEG")
    {
        for (int j = 0; j < Neg_FUs.size(); j++)
        {
            if (Neg_FUs[j].busy == true && Instruction.unit == Neg_FUs[j].name && Neg_FUs[j].Qj == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.readyToExecute = false;
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.execute_cycle = Neg_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "NAND")
    {
        for (int j = 0; j < Nand_FUs.size(); j++)
        {
            if (Nand_FUs[j].busy == true && Instruction.unit == Nand_FUs[j].name && Nand_FUs[j].Qj == "" && Nand_FUs[j].Qk == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.readyToExecute = false;
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.execute_cycle = Nand_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "SLL")
    {
        for (int j = 0; j < Sll_FUs.size(); j++)
        {
            if (Sll_FUs[j].busy == true && Instruction.unit == Sll_FUs[j].name && Sll_FUs[j].Qj == "" && Sll_FUs[j].Qk == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = Sll_FUs[j].cycles;
                    Instruction.execeute_begin_time = Current_Time;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "LOAD" && Instruction.issue_cycle == loadStore_queue[0].issue_cycle)
    {
        for (int j = 0; j < Load_FUs.size(); j++)
        {
            if (Load_FUs[j].busy == true && Instruction.unit == Load_FUs[j].name && Load_FUs[j].Qj == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = Load_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "STORE" && Instruction.issue_cycle == loadStore_queue[0].issue_cycle)
    {
        for (int j = 0; j < Store_FUs.size(); j++)
        {
            if (Store_FUs[j].busy == true && Instruction.unit == Store_FUs[j].name && Store_FUs[j].Qj == "" && !Instruction.executed && Instruction.waiting == false)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = Store_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "RET")
    {
        for (int j = 0; j < JAL_FUs.size(); j++)
        {
            if (JAL_FUs[j].busy == true && Instruction.unit == JAL_FUs[j].name && JAL_FUs[j].Qj == "" && !Instruction.executed)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.readyToExecute = false;
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.execute_cycle = JAL_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "JAL")
    {
        for (int j = 0; j < JAL_FUs.size(); j++)
        {
            if (JAL_FUs[j].busy == true && Instruction.unit == JAL_FUs[j].name && JAL_FUs[j].Qj == "" && !Instruction.executed)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = JAL_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
    if (Instruction.operation == "BNE")
    {
        for (int j = 0; j < BNE_FUs.size(); j++)
        {
            if (BNE_FUs[j].busy == true && Instruction.unit == BNE_FUs[j].name && BNE_FUs[j].Qj == "" && !Instruction.executed)
            {
                if (Instruction.readyToExecute) // first time to execute
                {
                    Instruction.execeute_begin_time = Current_Time;
                    Instruction.readyToExecute = false;
                    Instruction.execute_cycle = BNE_FUs[j].cycles;
                    EXECUTE_BEGIN_TABLE.push_back(Instruction);
                }
                else
                { // remaining cycles
                    if (Instruction.execute_cycle == 1)
                    {
                        Instruction.execute_time = Current_Time - 1;
                        Instruction.executed = true;
                    }
                    else
                    {
                        Instruction.execute_cycle--;
                    }
                }
            }
        }
    }
}

void writeBack(Instruction &Instruction, vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
               vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
               vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs, int Current_Time, bool &setBool, int &issue_index, bool &setBranch)
{
    if (Instruction.operation == "ADD")
    {
        for (int j = 0; j < Add_FUs.size(); j++)
        {
            if (Add_FUs[j].busy == true && Instruction.unit == Add_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                int rs2 = Register_File["R" + to_string(Instruction.rs2)];
                Register_File["R" + to_string(Instruction.rd)] = rs1 + rs2;
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Add_FUs[j].busy = false;
                Add_FUs[j].Vj = 0;
                Add_FUs[j].Vk = 0;
                Add_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
            }
        }
    }
    if (Instruction.operation == "ADDI")
    {
        for (int j = 0; j < Add_FUs.size(); j++)
        {
            if (Add_FUs[j].busy == true && Instruction.unit == Add_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                Register_File["R" + to_string(Instruction.rd)] = rs1 + Instruction.imm;
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Add_FUs[j].busy = false;
                Add_FUs[j].Vj = 0;
                Add_FUs[j].Vk = 0;
                Add_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
            }
        }
    }
    if (Instruction.operation == "NEG")
    {
        for (int j = 0; j < Neg_FUs.size(); j++)
        {
            if (Neg_FUs[j].busy == true && Instruction.unit == Neg_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                Register_File["R" + to_string(Instruction.rd)] = -rs1;
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Neg_FUs[j].busy = false;
                Neg_FUs[j].Vj = 0;
                Neg_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
            }
        }
    }
    if (Instruction.operation == "NAND")
    {
        for (int j = 0; j < Nand_FUs.size(); j++)
        {
            if (Nand_FUs[j].busy == true && Instruction.unit == Nand_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                int rs2 = Register_File["R" + to_string(Instruction.rs2)];
                Register_File["R" + to_string(Instruction.rd)] = ~(rs1 & rs2);
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Nand_FUs[j].busy = false;
                Nand_FUs[j].Vj = 0;
                Nand_FUs[j].Vk = 0;
                Nand_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
            }
        }
    }
    if (Instruction.operation == "SLL")
    {
        for (int j = 0; j < Sll_FUs.size(); j++)
        {
            if (Sll_FUs[j].busy == true && Instruction.unit == Sll_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                int rs2 = Register_File["R" + to_string(Instruction.rs2)];
                Register_File["R" + to_string(Instruction.rd)] = rs1 << rs2;
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Sll_FUs[j].busy = false;
                Sll_FUs[j].Vj = 0;
                Sll_FUs[j].Vk = 0;
                Sll_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
            }
        }
    }

    if (Instruction.operation == "LOAD")
    {
        for (int j = 0; j < Load_FUs.size(); j++)
        {
            if (Load_FUs[j].busy == true && Instruction.unit == Load_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                Register_File["R" + to_string(Instruction.rd)] = memory[rs1 + Instruction.imm];
                int rd = Register_File["R" + to_string(Instruction.rd)];
                // set FU to not busy
                Load_FUs[j].busy = false;
                Load_FUs[j].Vj = 0;
                Load_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
                loadStore_queue.erase(loadStore_queue.begin());
            }
        }
    }

    if (Instruction.operation == "STORE")
    {
        for (int j = 0; j < Store_FUs.size(); j++)
        {
            if (Store_FUs[j].busy == true && Instruction.unit == Store_FUs[j].name && Instruction.executed && !Instruction.written && Instruction.waiting == false)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to memory
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                int rd = Register_File["R" + to_string(Instruction.rd)];
                memory[rd + Instruction.imm] = rs1;
                // set FU to not busy
                Store_FUs[j].busy = false;
                Store_FUs[j].Vj = 0;
                Store_FUs[j].operation = "";
                Register_Status["R" + to_string(Instruction.rd)] = "";
                resetStation(Add_FUs, Load_FUs, Store_FUs, Neg_FUs, Nand_FUs, JAL_FUs, BNE_FUs, Sll_FUs, Instruction.unit, rd);
                loadStore_queue.erase(loadStore_queue.begin());
            }
        }
    }
    if (Instruction.operation == "RET")
    {
        for (int j = 0; j < JAL_FUs.size(); j++)
        {
            if (JAL_FUs[j].busy == true && Instruction.unit == JAL_FUs[j].name && Instruction.executed && !Instruction.written)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                issue_index = JAL_FUs[j].Vj - 1;
                // set FU to not busy
                JAL_FUs[j].busy = false;
                JAL_FUs[j].Vj = 0;
                JAL_FUs[j].operation = "";
                setBool = false;
            }
        }
    }
    if (Instruction.operation == "JAL")
    {
        for (int j = 0; j < JAL_FUs.size(); j++)
        {
            if (JAL_FUs[j].busy == true && Instruction.unit == JAL_FUs[j].name && Instruction.executed && !Instruction.written)
            {
                Instruction.written = true;
                setBool = false;
                Instruction.write_cycle = Current_Time;
                // write to register file
                Register_File["R1"] = issue_index + 1;
                issue_index = JAL_FUs[j].Vj + Instruction.issue_cycle;
                // set FU to not busy
                JAL_FUs[j].busy = false;
                JAL_FUs[j].Vj = 0;
                JAL_FUs[j].operation = "";
            }
        }
    }
    if (Instruction.operation == "BNE")
    {
        for (int j = 0; j < BNE_FUs.size(); j++)
        {
            if (BNE_FUs[j].busy == true && Instruction.unit == BNE_FUs[j].name && Instruction.executed && !Instruction.written)
            {
                Instruction.written = true;
                Instruction.write_cycle = Current_Time;
                // write to register file
                int rs1 = Register_File["R" + to_string(Instruction.rs1)];
                int rs2 = Register_File["R" + to_string(Instruction.rs2)];
                if (rs1 != rs2)
                {
                    num_misses++;
                    issue_index = BNE_FUs[j].offset + Instruction.issue_cycle;
                    for (int i = 0; i < Issued_Instructions.size(); i++)
                    {
                        if (Issued_Instructions[i].waiting && Issued_Instructions[i].issue_cycle > issue_index)
                        {
                            Issued_Instructions[i].waiting = false;
                        }
                        if (Issued_Instructions[i].waiting && Issued_Instructions[i].issue_cycle <= issue_index)
                        {
                            Issued_Instructions.erase(Issued_Instructions.begin() + i);
                            i--;
                        }
                    }
                    for (int i = 0; i < loadStore_queue.size(); i++)
                    {
                        if (loadStore_queue[i].waiting)
                        {
                            loadStore_queue.erase(loadStore_queue.begin() + i);
                            i--;
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < Issued_Instructions.size(); i++)
                    {
                        if (Issued_Instructions[i].waiting)
                        {
                            Issued_Instructions[i].waiting = false;
                        }
                    }
                    for (int i = 0; i < loadStore_queue.size(); i++)
                    {
                        if (loadStore_queue[i].waiting)
                        {
                            Issued_Instructions[i].waiting = false;
                        }
                    }
                }
                // set FU to not busy
                BNE_FUs[j].busy = false;
                BNE_FUs[j].Vj = 0;
                BNE_FUs[j].Vk = 0;
                BNE_FUs[j].operation = "";
            }
        }
    }
}

bool terminate(vector<Add_FU> &Add_FUs, vector<Load_FU> &Load_FUs, vector<Store_FU> &Store_FUs,
               vector<Neg_FU> &Neg_FUs, vector<Nand_FU> &Nand_FUs, vector<JAL_FU> &JAL_FUs,
               vector<BNE_FU> &BNE_FUs, vector<Sll_FU> &Sll_FUs)
{
    for (int i = 0; i < Add_FUs.size(); i++)
    {
        if (Add_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < Load_FUs.size(); i++)
    {
        if (Load_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < Store_FUs.size(); i++)
    {
        if (Store_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < Neg_FUs.size(); i++)
    {
        if (Neg_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < Nand_FUs.size(); i++)
    {
        if (Nand_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < JAL_FUs.size(); i++)
    {
        if (JAL_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < BNE_FUs.size(); i++)
    {
        if (BNE_FUs[i].busy)
        {
            return false;
        }
    }
    for (int i = 0; i < Sll_FUs.size(); i++)
    {
        if (Sll_FUs[i].busy)
        {
            return false;
        }
    }
    return true;
}