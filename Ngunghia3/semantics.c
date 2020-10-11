/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"
#include "debug.h"

extern SymTab *symtab;
extern Token *currentToken;

Object *lookupObject(char *name)
{
    Scope *currentScope = symtab->currentScope;
    Object *object = NULL;
    while (currentScope != NULL)
    {
        object = findObject(currentScope->objList, name);
        if (object != NULL)
            return object;
        currentScope = currentScope->outer;
    }
    return findObject(symtab->globalObjectList, name);
}
// khi doc dinh danh chung Fres
//
void checkFreshIdent(char *name) // check obj trong 1 scope
{

    if (findObject(symtab->currentScope->objList, name))
    {
        error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
    }
}

Object *checkDeclaredIdent(char *name) //kiem tra ton tai
{

    Object *ident = lookupObject(name);
    if (ident == NULL)
    {
        error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
    }

    return ident;
}

Object *checkDeclaredConstant(char *name) // kiem tra da ton tai constant 
{

    //Object *ident = checkDeclaredIdent(name);
    Object *ident = lookupObject(name);
    if (ident != NULL)
    {
        if (ident->kind == OBJ_CONSTANT)
        {
            //error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
            return ident;
        }
    }
    else
    {
        error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
    }
    return NULL;
}

Object *checkDeclaredType(char *name)
{

    Object *ident = checkDeclaredIdent(name);
    if (ident->kind != OBJ_TYPE)
    {
        error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
    }
    return ident;
}

Object *checkDeclaredVariable(char *name)
{

    Object *ident = checkDeclaredIdent(name);
    if (ident->kind != OBJ_VARIABLE)
    {
        error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
    }
    return ident;
}

Object *checkDeclaredFunction(char *name)
{

    Object *function = checkDeclaredIdent(name);
    if (function->kind != OBJ_FUNCTION)
    {
        error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
    }
    return function;
}

Object *checkDeclaredProcedure(char *name)
{
    Object *procedure = checkDeclaredIdent(name);
    //printObject(procedure, 5);
    if (procedure->kind != OBJ_PROCEDURE)
    {
        error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
    }
    return procedure;
}

Object *checkDeclaredLValueIdent(char *name)// vế trái của biểu thức gán đã hợp lệ hay chưa 
{
    Object *object = checkDeclaredIdent(name);

    if (object->kind == OBJ_FUNCTION || object->kind == OBJ_VARIABLE || object->kind == OBJ_PARAMETER)
    {
        if (object->kind == OBJ_FUNCTION)
        {
            
            if (strcmp(object->name, symtab->currentScope->owner->name) == 0)
            {
                return object;
            }
            else
            {
                error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
            }
        }
        else
        {
            if (strcmp(object->name, symtab->currentScope->owner->name) == 0)
            {
                error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
            }
        }
    }
    else
    {
        error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
    }

    return object;
}
