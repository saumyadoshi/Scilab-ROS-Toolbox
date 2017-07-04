//@author : Saumya Doshi, DA-IICT.

//This code receives a command as an input from Scilab (GUI/CLI), runs it at the terminal,
//and the output from the terminal is stored in a string, which in turn, is transferred
//to Scilab as a string and stored there.



#include "api_scilab.h"
#include "BOOL.h"
#include <localization.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/* ==================================================================== */
int ros_toolbox(char *fname, unsigned long fname_len)
{


/*
 */
	//=========================INPUT==================================//
	SciErr sciErr;
	int i,j;
	int iLen		= 0;
	//variable info
	int iRows		= 0;
	int iCols		= 0;
	int* piAddr		= NULL;
	int* piLen		= NULL;
	int iRowsOut	= 1;
	int iColsOut	= 1;
	char* pstOut	= NULL;
	char** pstData	= NULL;
	//check input and output arguments

    	CheckInputArgument(pvApiCtx, 1, 1);
    	CheckOutputArgument(pvApiCtx, 1, 1);

	//get variable address
	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//first call to retrieve dimensions
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, NULL, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	piLen = (int*)malloc(sizeof(int) * iRows * iCols);

	//second call to retrieve length of each string
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, NULL);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
	}

	//third call to retrieve data
	sciErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, piLen, pstData);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

        //====================================================================//
	char *command = pstData[0];
	//=======================TERMINAL OPERATION============================//

	char a[100][10000];
	int size = 0;
 	FILE *fpipe = (FILE*)popen(command,"r");
 	char line[10000];
 	while(fgets(line, sizeof(line), fpipe)){
		strcpy(a[size],line);
		size++;
		if(strncmp(line,"---",3)==0){
			break;
		}
		//sciprint("%s", line);

 	}

 	pclose(fpipe);



	//========================================================================//
	//================OUTPUT STRING WRITING=====================================
	int iRows2		= size;
	int iCols2		= 1;
	char** pstData2	= NULL;
	pstData2			= (char**)malloc(sizeof(char*) * iRows2 * iCols2);

	//printf("%d", size);
	int k;
	for(k=0;k<size;k++){
		pstData2[k] = a[k];
	}

	sciErr = createMatrixOfString(pvApiCtx, nbInputArgument(pvApiCtx) + 1, iRows2, iCols2, pstData2);
	if(sciErr.iErr)
	{
		printError(&sciErr, 0);
		return 0;
	}

	//free container
	free(pstData2);
	free(pstData);
	//assign allocated variables to Lhs position
   	AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

   	return 0;
}
/* ==================================================================== */
