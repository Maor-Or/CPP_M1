#include "AdptArray.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct AdptArray_
{
    int ArrSize;
    PElement* pElementArr;
    COPY_FUNC cf;
    DEL_FUNC df;
    PRINT_FUNC pf;

}AdptArray,*PAdptArray;

PAdptArray CreateAdptArray(COPY_FUNC cf, DEL_FUNC df,PRINT_FUNC pf)
{
    //allocating memory for the struct
    PAdptArray Arr = (PAdptArray)calloc(1, sizeof(AdptArray));
    
    //if arr is null then we return null cause there was an error
    if (!Arr)
        return NULL;
    
    //initilizing the attributes
    Arr->ArrSize = 0;
    Arr->cf = cf;
    Arr->df = df;
    Arr->pf = pf;
    Arr->pElementArr = NULL; //no elements yet
    
    return Arr;
}

void DeleteAdptArray(PAdptArray Arr)
{
    //if the struct pointer is already null then we are done
    if (!Arr)
        return;
    
    //if the struct's elem arr isn't empty we first delete it
    if ((Arr->pElementArr)!=NULL)
    {
        DEL_FUNC currDf = Arr->df;
        for (int i = 0; i < (Arr->ArrSize); i++)
        {
            if((Arr->pElementArr)[i])       //if the elem pointer isn't null
                currDf((Arr->pElementArr)[i]); //going through all the pointers in the elem arr and deleting all elems
        }
        free((Arr->pElementArr));
    }
    free(Arr);
}

Result SetAdptArrayAt(PAdptArray Arr, int index, PElement newPelem)
{
    //if the struct is null, or the index is invalid, then we can't do it
    if (!Arr || index < 0)
        return FAIL;
    
    //if we already have an array large enough, we will just put the pelem in
    if (index<(Arr->ArrSize))
    {
        //if there already is an element at the place, then we need to release it 
        if (Arr->pElementArr[index]) // != NULL
        {
            DEL_FUNC currDf = Arr->df;                  //setting up the del func   
            currDf(Arr->pElementArr[index]);            //delete the old elem
        }
        COPY_FUNC currCf = Arr->cf;                     //setting up the copy func       
        (Arr->pElementArr)[index] = currCf(newPelem);   //copying the new elem into the right spot
        return SUCCESS;
    }

    else // index >= (Arr->ArrSize) 
    {
        //create new space:
        PElement* pDelOldElemArr = Arr->pElementArr;//pointer to old array
        
        Arr->pElementArr = (PElement *)calloc((index+1),sizeof(PElement));//new larger array of elemPointers
        if (!(Arr->pElementArr)) //error allocating memory
        {
            Arr->pElementArr = pDelOldElemArr; //we get the old elem arr back
            return FAIL;
        }
        
        //if the Arr isn't null, meaning arrsize is not 0, we will copy it to the new place
        if(Arr->ArrSize)
            memcpy((Arr->pElementArr),(pDelOldElemArr),(Arr->ArrSize)*sizeof(PElement));
        
        //inserting the new pelem
        COPY_FUNC currCf = Arr->cf;
        (Arr->pElementArr)[index] = currCf(newPelem);
        
        if(pDelOldElemArr) //freeing the old pointers array incase it wasn't null
            free(pDelOldElemArr); 
        
        Arr->ArrSize = index+1; //updating the current arrsize
        return SUCCESS;
    }
}

PElement GetAdptArrayAt(PAdptArray Arr, int index)
{
    //if Arr is NULL or the index is out of bounds then error, return null
    if (!Arr || index >= Arr->ArrSize || index <0){
        return NULL;
    }

    //else - case index < (Arr->ArrSize)
    
    //if the element is NULL then we can't copy it
    if (!((Arr->pElementArr)[index]))
        return NULL;

    COPY_FUNC currCf = Arr->cf; //setting up the copy func
    PElement retElem = currCf((Arr->pElementArr)[index]); //copy the element to return
    return retElem;
}

int GetAdptArraySize(PAdptArray Arr)
{
    //if the struct's pointer is NULL or the elem arr is NULL then we return -1
    if (!Arr || !(Arr->pElementArr))
        return -1;

    return Arr->ArrSize; //return the size of the elem arr
}

void PrintDB(PAdptArray Arr)
{
    if (!Arr || !(Arr->pElementArr)) //if Arr or the elem arr is null then return nothing
        return;
    
    PRINT_FUNC currPf = Arr->pf;
    for (int i = 0; i < Arr->ArrSize; i++)
    {
        if((Arr->pElementArr)[i])
            currPf((Arr->pElementArr)[i]);
    }
}
