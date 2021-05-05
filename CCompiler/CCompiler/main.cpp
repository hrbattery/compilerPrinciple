//
//  main.cpp
//  CCompiler
//
//  Created by HRBattery on 2021/4/28.
//

#include <iostream>
#include <unordered_map>
#include "LALR.cpp"
#include "token.h"
using std::string;

unordered_map<string, int> symbolTable;
unordered_map<string, int> tempSymbolTable;
int varCount = 32;
int tempVarCount = 0;
int operandUsed = 0;
int tv = 0;

int ifLabel = 0;
int whileLabel = 0;
bool hasPrint = false;
bool hasScan = false;
bool hasElse = false;
bool hasEnd = false;
string expCode = "";

string addHead() {
    string main_header = "    .text\n"
    "    .globl main\n"
    "    .ent main\n"
    "main:\n"
    // "    # prologue area\n"
    "    .set noreorder\n"
    "    .frame $s8, 1024, $ra\n"
    "    .cpload $t9\n"
    "    .set reorder \n"
    "    addi $sp, $sp, -1024\n"
    "    .cprestore 24\n"
    "    sw $ra, 20($sp)\n"
    "    sw $s8, 28($sp)\n"
    "    move $s8, $sp\n"
    "main_$$branch_0:\n";
    return main_header;
}

string addScan() {
    string read_fix = ".extern scanf\n"
"    .rdata\n"
"data_section_$$0: \n"
"    .asciiz \"%d\"\n"
// "# gcc headers for __c_read\n"
"    .text\n"
"    .globl __c_read\n"
"    .ent __c_read\n"
"__c_read:\n"
// "    # prologue area\n"
"    .set noreorder\n"
"    .frame $s8, 48, $ra\n"
"    .cpload $t9\n"
"    .set reorder \n"
"    addi $sp, $sp, -48\n"
"    .cprestore 32\n"
"    sw $ra, 28($sp)\n"
"    sw $s8, 36($sp)\n"
"    move $s8, $sp\n"
"__c_read_$$branch_0:\n"
"    li $t0, 40\n"
"    add $t1, $t0, $s8\n"
"    la $t0, data_section_$$0\n"
"\n"
// "    # start calling scanf\n"
"    sw $t0, 0($s8)\n"
"    sw $t1, 4($s8)\n"
"    lw $a0, 0($s8)\n"
"    lw $a1, 4($s8)\n"
"    jal scanf\n"
"     \n"
// "    # end calling scanf\n"
"    lw $t1, 40($s8)\n"
"    move $v0, $t1\n"
"__c_read_$$epilogue:\n"
// "    # epilogue area\n"
"    move $sp, $s8\n"
"    lw $s8, 36($sp)\n"
"    lw $ra, 28($sp)\n"
"    addi $sp, $sp, 48\n"
"    jr $ra\n"
"    .end __c_read\n";
    return read_fix;
};

string addPrint() {
    string write_fix = ".extern printf\n"
    "    .rdata\n"
"data_section_$$1: \n"
"    .asciiz \"%d\\n\"\n"
"# gcc headers for __c_write\n"
"    .text\n"
"    .globl __c_write\n"
"    .ent __c_write\n"
"__c_write:\n"
// "    # prologue area\n"
"    .set noreorder\n"
"    .frame $s8, 40, $ra\n"
"    .cpload $t9\n"
"    .set reorder \n"
"    addi $sp, $sp, -40\n"
"    .cprestore 32\n"
"    sw $ra, 28($sp)\n"
"    sw $s8, 36($sp)\n"
"    move $s8, $sp\n"
"__c_write_$$branch_0:\n"
"    la $t0, data_section_$$1\n"
// "    # start calling printf\n"
"    sw $t0, 0($s8)\n"
"    sw $a0, 4($s8)\n"
"    lw $a0, 0($s8)\n"
"    lw $a1, 4($s8)\n"
"    jal printf\n"
// "    # end calling printf\n"
"__c_write_$$epilogue:\n"
// "    # epilogue area\n"
"    move $sp, $s8\n"
"    lw $s8, 36($sp)\n"
"    lw $ra, 28($sp)\n"
"    addi $sp, $sp, 40\n"
"    jr $ra\n"
"    .end __c_write\n";
    return write_fix;
};
string addEnd() {
    string main_end = "main_$$epilogue:\n"
// "    # epilogue area\n"
"    move $sp, $s8\n"
"    lw $s8, 28($sp)\n"
"    lw $ra, 20($sp)\n"
"    addi $sp, $sp, 1024\n"
"    jr $ra\n"
"    .end main\n";
    return main_end;
};

string lookup(string var, int offset) {
    string c = var;
    if (symbolTable.count(c) == 0) {
        if (tempSymbolTable.count(c) == 0) {
            printf("Undefined symbol: %s", var.c_str());
            exit(1);
        } else {
            // printf("find var \n");
            return std::to_string(tempSymbolTable[c]+offset*4)+"($s8)";
        }
    } else {
        // printf("find var \n");
        return std::to_string(symbolTable[c]+offset*4)+"($s8)";
    }
};

int lookup(string var) {
    string c = var;
    if (symbolTable.count(c) == 0) {
        if (tempSymbolTable.count(c) == 0) {
            printf("Undefined symbol: %s", var.c_str());
            exit(1);
        } else {
            // printf("find var \n");
            return tempSymbolTable[c];
        }
    } else {
        // printf("find var \n");
        return symbolTable[c];
    }
};

int saveVar(string var, int len) {
    if (symbolTable.count(var) == 0) {
        // printf("count %lu. coundn't find id %s. add id\n", map.count(c), c_str(c));
        symbolTable[var] = varCount;
        // map.insert(pair<string, int>(c, varCount));
        varCount += 4 * len;
        return varCount - 4 * len;
    } else {
        return symbolTable[var];
    }
}

int saveTempVar(string var, int len) {
    if (tempSymbolTable.count(var) == 0) {
        // printf("count %lu. coundn't find id %s. add id\n", map.count(c), c_str(c));
        tempSymbolTable[var] = tempVarCount + varCount;
        // map.insert(pair<string, int>(c, varCount));
        tempVarCount += 4 * len;
        return tempVarCount + varCount - 4 * len;
    } else {
        return tempSymbolTable[var];
    }
}

string registTempVar() {
    string tempVarName = "&tv" +std::to_string(tv);
    tv++;
    saveTempVar(tempVarName, 1);
    string swPos = lookup(tempVarName, 0);
    return swPos;
}

string tokenExecutor(token& tkn) {
    tokenType tknType = tkn.tokentype;
    string ret = "";
    if (!hasEnd) {
        switch(tknType) {
            case start: {
                return tokenExecutor(tkn.children[0]);
                break;
            }
            case program: {
                for (auto &t: tkn.children) {
                    ret += tokenExecutor(t);
                }
                return ret;
                break;
            }
            case var_decls: {
                for (auto &t: tkn.children) {
                    ret += tokenExecutor(t);
                }
                return ret;
                break;
            }
            case var_decl: {
                return tokenExecutor(tkn.children[1]);
                break;
            }
            case decl_list: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    return tokenExecutor(tkn.children[0]) + tokenExecutor(tkn.children[2]);
                }
                break;
            }
            case decl: {
                if (tkn.children.size() == 1) { // ID
                    int pos = saveVar(tkn.children[0].value, 1);
                    return "\tsw $zero, "+std::to_string(pos)+"($s8)\n";
                } else if (tkn.children.size() == 3) { // ID ASSIGN INT_NUM
                    int pos = saveVar(tkn.children[0].value, 1);
                    ret += "\taddi $t0, $zero, " +tkn.children[2].value+"\n";
                    ret += "\tsw $t0, "+ std::to_string(pos) + "($s8)\n";
//                    return "sw "+tkn.children[2].value+", " + std::to_string(pos) + "($s8)\n";
                    return ret;
                } else {// ID LSQUARE NUMBER RSQUARE
                    int pos = saveVar(tkn.children[0].value, atoi(tkn.children[2].value.c_str()));
                    return "\tsw $zero, "+std::to_string(pos)+"($s8)\n";
                }
                break;
            }
            case code_block: {
                if (tkn.children.size() == 1) { // stmt
                    return tokenExecutor(tkn.children[0]);
                } else { // { stmts }
                    return tokenExecutor(tkn.children[1]);
                }
                break;
            }
                // 接下来关于不同statement的处理，
            case stmts: {
                for (auto &t: tkn.children) {
                    ret += tokenExecutor(t);
                }
                return ret;
                break;
            }
            case stmt: {
                if (tkn.children[0].tokentype != SEMI) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    return "";
                }
                break;
            }
            case ctrl_stmt: {
                return tokenExecutor(tkn.children[0]);
                break;
            }
            case rw_stmt: {
                return tokenExecutor(tkn.children[0]);
                break;
            }
            case assign_stmt: {// ID [ expr ] = expr 或者 ID = expr
                if (tkn.children.size() == 3) { // ID = expr
                    string idPos = lookup(tkn.children[0].value, 0);
                    string exprPos = tokenExecutor(tkn.children[2]);
                    ret = expCode;
                    ret += "\tlw $t0, "+exprPos+"\n";
                    ret += "\tsw $t0, "+idPos+"\n";
                    // clear tempVars
                    tempSymbolTable.clear();
                    tempVarCount = 0;
                    expCode = "";
                    return ret;
                } else {
                    string idPos = std::to_string(lookup(tkn.children[0].value));
                    string exprPos1 = tokenExecutor(tkn.children[2]); // 该expr的结果存在exprPos1中了
                    ret = expCode;
                    ret += "\tlw $t0, "+exprPos1+"\n"; // 数组偏移量还需要*4
                    ret += "\tsll $t0, $t0, 2\n";
                    ret += "\taddi $t0, $zero, "+idPos+"\n"; // t0现在存的是到id[expr1]的offset
                    // clear tempVars
                    tempSymbolTable.clear();
                    tempVarCount = 0;
                    expCode = "";
                    string exprPos2 = tokenExecutor(tkn.children[5]);
                    ret += "\tlw $t1, "+exprPos2+"\n";;
                    ret += "\tadd $t2, $t0, $s8\n";;
                    ret += "\tsw $t1, 0($t2)\n";
                    // clear tempVars
                    tempSymbolTable.clear();
                    tempVarCount = 0;
                    expCode = "";
                    return ret;
                }
                break;
            }
            case if_statement: {
                if (tkn.children.size() == 1) { // simple if only
                    int cacheLabel = ifLabel;
                    return tokenExecutor(tkn.children[0]) + "Lexit" + std::to_string(cacheLabel) + ":\n";;
                } else { // if_stmt else codeBlock
                    hasElse = true;
                    int cacheLabel = ifLabel;
                    string ifPart = tokenExecutor(tkn.children[0]);
                    ret += ifPart;
                    ret += "Lelse" + std::to_string(cacheLabel) + ":\n";
                    ret += tokenExecutor(tkn.children[2]);
                    ret += "Lexit" + std::to_string(cacheLabel) + ":\n";
                    return ret;
                }
                break;
            }
            case if_stmt: { // IF ( exp ) code_block
                // expr的结果在临时变量里
                string ifExprPos = tokenExecutor(tkn.children[2]);
                ret += expCode;
                ret += "\tlw $t7, "+ifExprPos+"\n";
                ret += "\tbeqz $t7, ";
                if (hasElse) {
                    ret += "Lelse" + std::to_string(ifLabel) + "\n";;
                } else {
                    ret += "Lexit" + std::to_string(ifLabel) + "\n";;
                }
                ret += "\tnop\n";
                hasElse = false;
                
                // clear tempVars
                tempSymbolTable.clear();
                tempVarCount = 0;
                expCode = "";
                ret += tokenExecutor(tkn.children[4]);
                ret += "\tj Lexit" + std::to_string(ifLabel) + "\n";
                ifLabel++;
                return ret;
                break;
            }
            case while_stmt: { // WHILE ( exp ) code_block
                string whileExprPos = tokenExecutor(tkn.children[2]);
                ret += "wTest" + std::to_string(whileLabel) + ":\n";
                ret += expCode;
                ret += "\tlw $t7, "+whileExprPos+"\n";
                ret += "\tbeqz $t7, wExit" + std::to_string(whileLabel) + "\n";
                ret += "\tnop\n";
                // clear tempVars
                tempSymbolTable.clear();
                tempVarCount = 0;
                expCode = "";
                ret += tokenExecutor(tkn.children[4]);
                ret += "\tj wTest" + std::to_string(whileLabel) + "\n";
                ret += "wExit" + std::to_string(whileLabel) + ":\n";
                whileLabel++;
                return ret;
                break;
            }
            case do_while_stmt: { // DO code_block WHILE ( exp )
                ret += "wDo" + std::to_string(whileLabel) + ":\n";
                ret += tokenExecutor(tkn.children[2]);
                string doExprPos = tokenExecutor(tkn.children[4]);
                ret += expCode;
                ret += "\tlw $t7, "+doExprPos+"\n";
                ret += "\tbeqz $t7, wDo" + std::to_string(whileLabel) + "\n";
                ret += "\tnop\n";
                whileLabel++;
                // clear tempVars
                tempSymbolTable.clear();
                tempVarCount = 0;
                expCode = "";
                return ret;
                break;
            }
            case ret_stmt: {
                ret += addEnd();
                hasEnd = true;
                return ret;
                break;
            }
            case r_stmt: {
                hasScan = true;
                string id = tkn.children[2].value;
                ret += "\tjal __c_read\n";
                ret += "\tsw $v0, " + lookup(id, 0) + "\n";
                return ret;
                break;
            }
            case w_stmt: {
                hasPrint = true;
                string wstmtExprPos = tokenExecutor(tkn.children[2]);
                ret += expCode;
                ret += "\tlw $a0, "+wstmtExprPos+"\n";
                ret += "\tjal __c_write\n";
                // clear tempVars
                tempSymbolTable.clear();
                tempVarCount = 0;
                expCode = "";
                return ret;
                break;
            }
            // 接下来会假定，下面的expr的处理都可以保证返回该expr的计算结果最后缓存的临时变量的内存位置，即offset($s8)的形式
            // 而code的部分会由引用该expr的production来处理，expr部分的code会存在expCode中
            case expr: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else { // OR_OP
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    expCode += "\tor $t2, $t0, $t1\n";
                    // 申请新临时变量
                    string swPos = registTempVar();
                    expCode += "\tsw $t2, " + swPos + "\n";
                    return swPos;
                }
                break;
            }
            case expr2: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else { // ANDAND
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    expCode += "\tand $t2, $t0, $t1\n";
                    // 申请新临时变量
                    string swPos = registTempVar();
                    expCode += "\tsw $t2, " + swPos + "\n";
                    return swPos;
                }
                break;
            }
            case expr3: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else { // OR_OP
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    expCode += "\tor $t2, $t0, $t1\n";
                    // 申请新临时变量
                    string swPos = registTempVar();
                    expCode += "\tsw $t2, " + swPos + "\n";
                    return swPos;
                }
                break;
            }
            case expr4: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else { // AND_OP
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    expCode += "\tand $t2, $t0, $t1\n";
                    // 申请新临时变量
                    string swPos = registTempVar();
                    expCode += "\tsw $t2, " + swPos + "\n";
                    return swPos;
                }
                break;
            }
            case expr5: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    if (tkn.children[1].tokentype == EQ) {
                        expCode += "\tseq $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else { //NOTEQ
                        expCode += "\tsne $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    }
                }
                break;
            }
            case expr6: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    if (tkn.children[1].tokentype == LT) {
                        expCode += "\tslt $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else if (tkn.children[1].tokentype == LTEQ) {
                        expCode += "\tsle $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else if (tkn.children[1].tokentype == GT) {
                        expCode += "\tsgt $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else { // GTEQ
                        expCode += "\tsge $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    }
                }
                break;
            }
            case expr7: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    if (tkn.children[1].tokentype == SHL_OP) {
                        expCode += "\tsllv $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else {
                        expCode += "\tsrav $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    }
                }
                break;
            }
            case expr8: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    if (tkn.children[1].tokentype == MUL_OP) {
                        expCode += "\tadd $t2, $t1, $t0\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else {
                        expCode += "\tdiv $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    }
                }
                break;
            }
            case expr9: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expOp1 = tokenExecutor(tkn.children[0]);
                    string expOp2 = tokenExecutor(tkn.children[2]);
                    expCode += "\tlw $t0, "+expOp1+"\n";
                    expCode += "\tlw $t1, "+expOp2+"\n";
                    if (tkn.children[1].tokentype == MUL_OP) {
                        expCode += "\tmul $t2, $t1, $t0\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    } else {
                        expCode += "\tdiv $t2, $t0, $t1\n";
                        // 申请新临时变量
                        string swPos = registTempVar();
                        expCode += "\tsw $t2, " + swPos + "\n";
                        return swPos;
                    }
                }
                break;
            }
            case expr10: {
                if (tkn.children.size() == 1) {
                    return tokenExecutor(tkn.children[0]);
                } else {
                    string expRet = tokenExecutor(tkn.children[0]);
                    expCode += "\tlw $t0, "+expRet+"\n";
                    if (tkn.children[0].tokentype == MINUS) {
                        expCode += "\tneg $t1, $t0\nsw $t1, "+expRet+"\n";
                        return expRet;
                    } else {
                        expCode += "\tseq $t1, $t0, $zero\nsw $t1 "+expRet+"\n"; // need modification
                        return expRet;
                    }
                }
                break;
            }
            case expr11: {
                if (tkn.children[0].tokentype == INT_NUM) {
                    expCode += "\taddi $t0, $zero, "+tkn.children[0].value + "\n";
                    string swPos = registTempVar();
                    expCode += "\tsw $t0, " + swPos + "\n";
                    return swPos; // 在这里再处理一下
                } else if (tkn.children[0].tokentype == ID && tkn.children.size() == 1) {
                    return lookup(tkn.children[0].value,0);
                } else if (tkn.children[0].tokentype == ID) { // ID [ exp ]
                    string expRet = tokenExecutor(tkn.children[2]); // 约定返回的是offset($s8)的形式
                    expRet = "\tlw $t0, "+expRet+"\n"; // 之后补上exp是int_num的情况，这里是移动exp返回的地址内的值到$t0，也就是偏移量
                    int varOff = lookup(tkn.children[0].value); // varOff = id的偏移量
                    expRet += "\taddi $t1, $t0, "+std::to_string(varOff)+"\n"; // $t1 = $t0 + varOff = 总偏移量，用$t1($s8)就可以取到想取的值了
                    expRet += "\tsll $t1, $t1, 2\n";
                    expRet += "\tadd $t2, $s8, $t1\n";
                    expRet += "\tlw $t0, 0($t2)\n";
                    // 申请一个新临时变量地址(swPos)
    //                string tempVarName = "&tv" +std::to_string(tv);
    //                tv++;
    //                saveTempVar(tempVarName, 1);
                    string swPos = registTempVar();
                    // 用sw存进去地址中
                    expRet += "\tsw $t0, " + swPos + "\n";
                    expCode += expRet;
                    return swPos;
                } else {
                    return tokenExecutor(tkn.children[1]);
                }
                break;
            }
            default: {
                return "";
                break;
            }
        }
    }
    return "";
}

int main(int argc, const char* argv[]) {
    FILE* fp;
    if (argc > 1) {
        if ((fp = fopen(argv[1], "r")) == NULL) {
            printf("Unable to open file.");
            exit(0);
        }
    } else {
        fp = fopen("/Users/hrbattery/Documents/iroiroProject/compilerPrinciple/CParser/CParser/CParser/test.txt", "r");
    }
//    printf("%s", "Result: \n");

    LALR lalr;
    token head = lalr.run(fp);
//    head.printString("");
    string result = "";
    string ret = tokenExecutor(head);
    if (hasScan) {
        result += addScan();
    }
    if (hasPrint) {
        result += addPrint();
    }
    result += addHead();
    result += ret;
    cout << result << endl;
    fclose(fp);
    fp = NULL;
    return 0;
}
