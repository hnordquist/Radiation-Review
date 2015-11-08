/*
This work was supported by the United States Member State Support Program to IAEA Safeguards;
the U.S. Department of Energy, Office of Nonproliferation and National Security, International
Safeguards Division; and the U.S. Department of Energy, Office of Safeguards and Security.

LA-CC-14-040. This software was exported from the United States in accordance with the Export
Administration Regulations. Diversion contrary to U.S. law prohibited.

Copyright 2015, Los Alamos National Security, LLC. This software application and associated
material ("The Software") was prepared by the Los Alamos National Security, LLC. (LANS), under
Contract DE-AC52-06NA25396 with the U.S. Department of Energy (DOE). All rights in the software
application and associated material are reserved by DOE on behalf of the Government and LANS
pursuant to the contract.

Redistribution and use in source and binary forms, with or without modification, are permitted
provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of
conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or other materials provided
with the distribution.
3. Neither the name of the "Los Alamos National Security, LLC." nor the names of its contributors
may be used to endorse or promote products derived from this software without specific prior
written permission.

THIS SOFTWARE IS PROVIDED BY LOS ALAMOS NATIONAL SECURITY, LLC AND CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LOS ALAMOS
NATIONAL SECURITY, LLC OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRAT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/
// Steven Edward Buck
// Los Alamos National Laboratory
// NIS-5
// 02/23/97

// LstLsts.CPP
// Main source file for the Integrated Review Table

#include "LstLsts.H"   



/////////////////////////////////////////////////////////////////////////////////////
//  CLList_Of_LLists constructor:  Setup all intial parameters and
//									build the LL structure
/////////////////////////////////////////////////////////////////////////////////////

CLList_Of_LLists::CLList_Of_LLists(UINT uiDeminsionX, UINT uiDeminsionY) :
	uiDimX(uiDeminsionX),
	uiDimY(uiDeminsionY),
	bError(FALSE),
	pppLList(NULL)
	{
	
	UINT ctIndexX = 0, ctIndexY = 0, ctItem = 0;
	
	
	bError = ((pppLList = new (RWTPtrSlist<UINT> (**[uiDimX]))) == NULL);
	ctIndexX = 0;
	while ((!bError) && (ctIndexX < uiDimX)) {
		bError = ((pppLList[ctIndexX] = new (RWTPtrSlist<UINT> (*[uiDimY]))) == NULL);
		ctIndexY = 0;
		while ((!bError) && (ctIndexY < uiDimY)) {
			bError = ((pppLList[ctIndexX][ctIndexY] = new RWTPtrSlist<UINT>) == NULL);
			ctIndexY++;
			}
		ctIndexX++;
		}

}


/////////////////////////////////////////////////////////////////////////////////////
//  CLList_Of_LLists destructor:  Destroy all of the dynamic memory and destroy
//									the LL structure      
/////////////////////////////////////////////////////////////////////////////////////

CLList_Of_LLists::~CLList_Of_LLists(void) {

	UINT ctIndexX, ctIndexY, ctItem, uiNum_Items;


	for (ctIndexX = 0; ctIndexX < uiDimX; ctIndexX++) {
		for (ctIndexY = 0; ctIndexY < uiDimY; ctIndexY++) {
			uiNum_Items = pppLList[ctIndexX][ctIndexY]->entries(); // Changes - must be separated
			for (ctItem = 0; ctItem < uiNum_Items; ctItem++) {
				delete pppLList[ctIndexX][ctIndexY]->get();
				}
			if (pppLList[ctIndexX][ctIndexY])
				delete pppLList[ctIndexX][ctIndexY];
			}
		if (pppLList[ctIndexX])
			delete [] pppLList[ctIndexX];
		}
	if (pppLList)
		delete [] pppLList;

}


/////////////////////////////////////////////////////////////////////////////////////
//  CLList_Of_LLists destructor:  Basic operations on the LL structure
/////////////////////////////////////////////////////////////////////////////////////

BOOL CLList_Of_LLists::bValid(void) {

	return (!bError);

}


/////////////////////////////////////////////////////////////////////////////////////
//  CLList_Of_LLists destructor:  Complex operations on the LL structure
/////////////////////////////////////////////////////////////////////////////////////

// No longer needed
//static RWBoolean MyTester(UINT* a, void* b) {
//
//	return (*a == *(UINT *)b);
//
//}


BOOL CLList_Of_LLists::bEmpty(UINT uiTool, UINT uiList) {

	return (pppLList[uiTool][uiList]->isEmpty());

}


BOOL CLList_Of_LLists::bAdd(UINT uiTool, UINT uiList, UINT uiItem) {

// No longer need tester
//	if (!pppLList[uiTool][uiList]->contains(MyTester, &uiItem))
	if (!pppLList[uiTool][uiList]->contains(&uiItem)) {
		pppLList[uiTool][uiList]->append((new UINT(uiItem)));
		return (uiItem == *(pppLList[uiTool][uiList]->last()));
		}
		
	return TRUE;
	
}

			
BOOL CLList_Of_LLists::bRemove(UINT uiTool, UINT uiList, UINT uiItem) {
    
    UINT *puiItem;

    
// No longer need tester
//	if (puiItem = pppLList[uiTool][uiList]->remove(MyTester, &uiItem)) {
	if (puiItem = pppLList[uiTool][uiList]->remove(&uiItem)) {
		if (puiItem != rwnil) {
			delete puiItem;
			return TRUE;
			}
		}
		
	return FALSE;
		
}

			
BOOL CLList_Of_LLists::bContains(UINT uiTool, UINT uiList, UINT uiItem) {

// No longer need tester
//	return (pppLList[uiTool][uiList]->contains(MyTester, &uiItem));
	return (pppLList[uiTool][uiList]->contains(&uiItem));

}


UINT CLList_Of_LLists::uiGet_Least(UINT uiTool, UINT uiList) {

	UINT ctItem, uiItem, uiLeast = UINT_MAX;


	for (ctItem = 0; ctItem < pppLList[uiTool][uiList]->entries(); ctItem++) {
		uiItem = (*pppLList[uiTool][uiList]->at(ctItem));
//		uiItem = pppLList[uiTool][uiList]->at(ctItem);  Book wrong, returns a pointer not a reference
		if (uiItem < uiLeast) uiLeast = uiItem;
		}
		
	return uiLeast;

}


void CLList_Of_LLists::Remove_Range(UINT uiTool, UINT uiList, UINT uiStart_Value, UINT uiEnd_Value) {

	int ctItem;


	if (!CLList_Of_LLists::bEmpty(uiTool, uiList)) {
		if (uiStart_Value <= uiEnd_Value) {
			for (ctItem = uiStart_Value; ctItem <= (int)uiEnd_Value; ctItem++) {
				if (!CLList_Of_LLists::bEmpty(uiTool, uiList))
					CLList_Of_LLists::bRemove(uiTool, uiList, (UINT)ctItem);
				else
				    ctItem = uiEnd_Value + 1;
				}
			}
		else { // Allow to run backwards in order to optimize for speed
			for (ctItem = uiStart_Value; ctItem >= (int)uiEnd_Value; ctItem--) {
				if (!CLList_Of_LLists::bEmpty(uiTool, uiList))
					CLList_Of_LLists::bRemove(uiTool, uiList,(UINT)ctItem);
				else
				    ctItem = (int)uiEnd_Value - 1;
				}
			}
		}

}


void CLList_Of_LLists::Remove_Range_Range(UINT uiTool, UINT uiStart_Row, UINT uiEnd_Row, UINT uiStart_Value, UINT uiEnd_Value) {

	UINT ctList;


	for (ctList = uiStart_Row; ctList <= uiEnd_Row; ctList++) {
		CLList_Of_LLists::Remove_Range(uiTool, ctList, uiStart_Value, uiEnd_Value);
		}

}


