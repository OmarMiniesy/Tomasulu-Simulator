#include "initialize.h"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <unordered_map>
using namespace std;

int main()
{
    // iniiatilize the reservation stations
    int num_add = 3;
    int add_cycles = 2;
    int num_load = 3;
    int load_cycles = 2;
    int num_store = 3;
    int store_cycles = 2;
    int num_neg = 1;
    int neg_cycles = 2;
    int num_nand = 1;
    int nand_cycles = 1;
    int num_jal = 1;
    int jal_cycles = 1;
    int num_bne = 1;
    int bne_cycles = 1;
    int num_sll = 1;
    int sll_cycles = 8;
    cout << "Enter number of ADD reservation stations: ";
    cin >> num_add;
    cout << "Enter number of ADD cycles: ";
    cin >> add_cycles;
    cout << "Enter number of LOAD reservation stations: ";
    cin >> num_load;
    cout << "Enter number of LOAD cycles: ";
    cin >> load_cycles;
    cout << "Enter number of STORE reservation stations: ";
    cin >> num_store;
    cout << "Enter number of STORE cycles: ";
    cin >> store_cycles;
    cout << "Enter number of NEG reservation stations: ";
    cin >> num_neg;
    cout << "Enter number of NEG cycles: ";
    cin >> neg_cycles;
    cout << "Enter number of NAND reservation stations: ";
    cin >> num_nand;
    cout << "Enter number of NAND cycles: ";
    cin >> nand_cycles;
    cout << "Enter number of JAL reservation stations: ";
    cin >> num_jal;
    cout << "Enter number of JAL cycles: ";
    cin >> jal_cycles;
    cout << "Enter number of BNE reservation stations: ";
    cin >> num_bne;
    cout << "Enter number of BNE cycles: ";
    cin >> bne_cycles;
    cout << "Enter number of SLL reservation stations: ";
    cin >> num_sll;
    cout << "Enter number of SLL cycles: ";
    cin >> sll_cycles;
    vector<Add_FU> Add_FU;
    vector<Load_FU> Load_FU;
    vector<Store_FU> Store_FU;
    vector<Neg_FU> Neg_FU;
    vector<Nand_FU> Nand_FU;
    vector<JAL_FU> JAL_FU;
    vector<BNE_FU> BNE_FU;
    vector<Sll_FU> Sll_FU;
    initialize_reservation_stations(num_add, num_load, num_store, num_neg,
                                    num_nand, num_jal, num_bne, num_sll, Add_FU, Load_FU, Store_FU, Neg_FU,
                                    Nand_FU, JAL_FU, BNE_FU, Sll_FU, add_cycles, load_cycles, store_cycles, neg_cycles,
                                    nand_cycles, jal_cycles, bne_cycles, sll_cycles);
    // initialize register status
    initialize_register_status();

    // initialize registers.
    initialize_registers_map();
    // read from file if any values to be set in registers
    initialize_registers_file("registers.txt");

    // initialize memory
    int starting_address = 0;
    initialize_memory_map(starting_address);
    // read from file if any values to be set in memory
    initialize_memory_file("memory.txt");

    // initialize instructions
    read_instructions("instructions.txt");
    read_instruction_types("instructions.txt");
    parse_instructions(starting_address);

    int cycle = 1;
    int issue_index = 0;
    int execute_count = 0;
    int write_back_count = 0;
    bool setBool = false;
    bool setBranch = false;
    while (true)
    {
        // issue one instruction
        if (issue_index != Instructions.size())
        {
            if (issue(Instruction_Types[issue_index], Instructions[issue_index], Add_FU, Load_FU,
                      Store_FU, Neg_FU, Nand_FU, JAL_FU, BNE_FU, Sll_FU, cycle, issue_index, setBool, setBranch))
            {
                if (Instructions[issue_index].issued)
                {
                    ISSUE_TABLE.push_back(Instructions[issue_index]);
                }
                issue_index++;
            }
        }

        // execute all that can be executed
        for (int i = 0; i < Issued_Instructions.size(); i++)
        {
            if (Issued_Instructions[i].issue_cycle != cycle)
            {

                execute(Issued_Instructions[i], Add_FU, Load_FU,
                        Store_FU, Neg_FU, Nand_FU, JAL_FU, BNE_FU, Sll_FU, cycle);

                if (Issued_Instructions[i].executed)
                {
                    EXECUTE_END_TABLE.push_back(Issued_Instructions[i]);
                    executedInstructions.push_back(Issued_Instructions[i]);
                    Issued_Instructions.erase(Issued_Instructions.begin() + i);
                    i--;
                    execute_count++;
                }
            }
        }

        // sort by issue time
        if (executedInstructions.size() > 0)
        {
            sort(executedInstructions.begin(), executedInstructions.end(), [](Instruction a, Instruction b)
                 { return a.issue_cycle < b.issue_cycle; });
            if (executedInstructions[0].execute_time != cycle)
            {
                writeBack(executedInstructions[0], Add_FU, Load_FU,
                          Store_FU, Neg_FU, Nand_FU, JAL_FU, BNE_FU, Sll_FU, cycle, setBool, issue_index, setBranch);
                WRITE_TABLE.push_back(executedInstructions[0]);
                executedInstructions.erase(executedInstructions.begin());
                write_back_count++;
            }
        }

        cycle++;
        if (cycle > 1)
        {
            if (terminate(Add_FU, Load_FU,
                          Store_FU, Neg_FU, Nand_FU, JAL_FU, BNE_FU, Sll_FU))
                break;
        }
    }
    cout << endl;
    cout << endl;
    cout << "ISSUE TABLE" << endl;
    cout << "INSTRUCTION  ::: CYCLE" << endl;
    for (int i = 0; i < ISSUE_TABLE.size(); i++)
    {
        cout << ISSUE_TABLE[i].rawInstruction << " ::: " << ISSUE_TABLE[i].issue_cycle << endl;
    }
    cout << endl;
    cout << "EXECUTE BEGIN TABLE" << endl;
    cout << "INSTRUCTION  ::: CYCLE" << endl;
    for (int i = 0; i < EXECUTE_BEGIN_TABLE.size(); i++)
    {
        cout << EXECUTE_BEGIN_TABLE[i].rawInstruction << " ::: " << EXECUTE_BEGIN_TABLE[i].execeute_begin_time << endl;
    }
    cout << endl;
    cout << "EXECUTE END TABLE" << endl;
    cout << "INSTRUCTION  ::: CYCLE" << endl;
    for (int i = 0; i < EXECUTE_END_TABLE.size(); i++)
    {
        cout << EXECUTE_END_TABLE[i].rawInstruction << " ::: " << EXECUTE_END_TABLE[i].execute_time << endl;
    }
    cout << endl;
    cout << "WRITE TABLE" << endl;
    cout << "INSTRUCTION  ::: CYCLE" << endl;
    for (int i = 0; i < WRITE_TABLE.size(); i++)
    {
        cout << WRITE_TABLE[i].rawInstruction << " ::: " << WRITE_TABLE[i].write_cycle << endl;
    }
    cout << endl;
    cout << endl;
    cout << "Total number of cycles: " << cycle - 1 << endl;
    cout << "IPC: " << (float)Instructions.size() / ((float)cycle - 1) << endl;
    if (num_branches == 0)
    {
        cout << "BRANCH MISPREDICTION: 0" << endl;
    }
    else
    {
        float branch_misprediction = num_misses / (float)num_branches;
        cout << "BRANCH MISPREDICTION: " << branch_misprediction << endl;
    }
    cout << "REGS" << endl;
    for (auto it = Register_File.begin(); it != Register_File.end(); it++)
    {
        cout << it->first << " " << it->second << endl;
    }
    cout << "MEM" << endl;
    for (auto it = memory.begin(); it != memory.end(); it++)
    {
        if (it->second != 0)
            cout << it->first << " " << it->second << endl;
    }
}