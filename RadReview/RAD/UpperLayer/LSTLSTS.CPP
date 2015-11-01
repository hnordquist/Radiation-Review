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


