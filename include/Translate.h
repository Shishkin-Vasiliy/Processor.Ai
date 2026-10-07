#ifndef TRANSLATE_H
#define TRANSLATE_H

#define POIZON_IP_PTR ((StackElem_t *)222)
#define MAX_CMD_LEN 100

enum TranslateErr_t {
    TRANSLATION_SUCCESS,
    SYNTAX_ERR,
    BAD_CODESEG_PTR,
    BAD_INDEX_PTR
};

enum CmdNum_t {
    HLT_num,
    PUSH_num,
    ADD_num,
    SUB_num,
    DIV_num,
    OUT_num,
    UNKNOWN_num
};

int Translate(const char *ASM, const char *ByteCode, StackElem_t **CodeSegment);
TranslateErr_t TranslateCommand(const char *string, StackElem_t **CodeSegment);
CmdNum_t GetCmdNum(const char *Command);
void TranslateErrMsg(TranslateErr_t err);

#endif