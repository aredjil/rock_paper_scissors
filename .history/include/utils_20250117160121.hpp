#ifndef UTILS_H
#define UTILS_H
#include<petsc.h>
#include <filesystem>
#include <fstream>
#include <string>
#include<iostream>


/*
    Struct to store the payoffs
*/
typedef struct PAYOFFS
{
    PetscReal a; /*A_00*/
    PetscReal b; /*A_01*/
    PetscReal c; /*A_02*/
    PetscReal d; /*A_10*/
    PetscReal e; /*A_11*/
    PetscReal f; /*A_12*/
    PetscReal g; /*A_20*/
    PetscReal h; /*A_21*/
    PetscReal i; /*A_22*/
} payoffs;
/**
 * Initial configuration
 */
typedef struct INIT_CONFG
{
    PetscReal p1;
    PetscReal p2;
    PetscReal p3;
} init_conf;


#endif // UTILS_H