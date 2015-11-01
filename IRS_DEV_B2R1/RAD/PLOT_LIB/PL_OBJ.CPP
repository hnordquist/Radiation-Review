/* ======================================================================= */
/* ============================== pl_obj.cpp ============================= */
/* ======================================================================= */

/*---------------------------------------------------------------------------
*                         SPECIAL NOTE TO DEVELOPERS
*
*		Information from this file is extracted to create portions of
*		PL_UM.DOC.  Because of this, the appearance of that document
*		is highly dependent on the formatting of information in this file.
*		Thus, it is recommended to remain consistent with existing format
*		conventions in this file.  Please observe the following:
*		o	keep the use of tabs and spaces consistent with existing usage
*			in this file
*		o	set tab width to 4
*		o	use "preserve tabs"
*		o	keep extracted lines shorter than 80 characters
*
*--------------------------------------------------------------------------*/

// TO DO:
// o	implement 'auto position' for label text
// o	various label options not implemented; deal with opaque vs transparent;
//		fill rectangle??; editing (mouse and keyboard) of label;
// o	do better documentation.  For each object type:
//		-	...Mode() should document the mode flags
//		-	...Props() should document the properties
// o	explore whether a filled box should show as filled when it is
//		selected
// o	document that overlapping filled boxes of different colors look
//		weird when in edit mode
// o	update the Structured Hierarchy Chart
// o	examine issues relating to deleting plot area rows and columns,
//		and thus making some plot objects invalid

/*---------------------------------------------------------------------------
*  void PlObjBoxCoordCalcPx (<>pPlot, bShowFullHt, pcoLB, pcoRT)
*   int PlObjBoxCreateAtPlotCursor (<>pPlot, bNoCallback, >ppBox,
*                                bSelect, bHide, bPreferClip,
*                                dValL, dValB, dValR, dValT, rgbBox)
*   int PlObjBoxCreateGeneral (<>pPlot, bNoCallback, >ppBox,
*                                bSelect, bHide,
*                                dValL, iColL, dValB, iRowB,
*                                dValR, iColR, dValT, iRowT, rgbBox)
*   int PlObjBoxCreateSimple (<>pPlot, bNoCallback, >ppBox,
*                                bSelect, bHide,
*                                dValL, dValB, dValR, dValT, rgbBox)
*  void PlObjBoxCvtBoxToCoord (<>pPlot, pBox, bSelected, >pcoLB, >pcoRT)
*  void PlObjBoxCvtCoordToBox (<>pPlot, pcoLB, pcoRT, >pBox)
*   int PlObjBoxHitTest     (<>pPlot, <>pObjCtx, pBox, pcoMouse)
*  void PlObjBoxInit        (<>pPlot, <>pBox, bHide)
*   int PlObjBoxKeyboardMove (<>pPlot, <>pBox, bDeltaPx,
*                                dDeltaX, dDeltaY, iDeltaRow, iDeltaCol)
*   int PlObjBoxPaint       (pPlot, pBox, bSelected)
*  void PlObjBoxPaint_edit  (pPlot)
*   int PlObjBoxProps       (<>pPlot, <>pBox, bNoPaint, bSet, <>pBoxProps)
*   int PlObjBoxPropsDflt   (<>pPlot, bSet, <>pBoxProps)
*   int PlObjBoxZoom        (<>pPlot, pBox, bZoom,
*                                >fXZoom, >fYZoom, >fXScroll, >fYScroll)
*  void PlObjCoordAddDeltaX (<>pPlot, <>pco, bTrackMouse, bDeltaPx, dDeltaX)
*  void PlObjCoordAddDeltaY (<>pPlot, <>pco, bTrackMouse, bDeltaPx, dDeltaY)
*   int PlObjCopyAll        (<>pPlot, pPlotSrc)
*  void PlObjCvtXPxYPxToCoord (<>pPlot, iXPx, iYPx, >pco)
*   int PlObjDelete         (<>pPlot, bNoCallback, pObj)
*   int PlObjDeleteAll      (<>pPlot, bNoCallback)
*  WORD PlObjEditDialog     (<>pPlot, bDblClick, wParam)
*  void PlObjEditDone       (<>pPlot)
*   int PlObjEditDoneIsOK   (<>pPlot)
*  void PlObjEditMove       (<>pPlot)
*  void PlObjEditStart      (<>pPlot)
*   int PlObjEditStartIsOK  (<>pPlot)
*   HDC PlObjHDCHandler     (<>pPlot, pBox, bReleaseDC)
*   int PlObjHitTest        (<>pPlot, <>pObjCtx, pObj, pcoMouse)
*  void PlObjInit           (<>pPlot)
*   int PlObjKeyboardMove   (<>pPlot, <>pObj, bDeltaPx,
*                                dDeltaX, dDeltaY, iDeltaRow, iDeltaCol)
*   int PlObjKeyDown        (<>pPlot, wParam)
*   int PlObjLabelCreate    (<>pPlot, bNoCallback, >ppLabel,
*                                bSelect, dXVal, dYVal,
*                                fFarX_pts, fFarY_pts, fMeet_frac,
*                                szText, rgbLabel)
*   int PlObjLabelHitTest   (<>pPlot, <>pObjCtx, pLabel, pcoMouse)
*  void PlObjLabelInit      (<>pPlot, <>pLabel)
*   int PlObjLabelPaint     (pPlot, pLabel, bSelected)
*  void PlObjLabelPaint_edit (pPlot)
*   int PlObjLabelProps     (<>pPlot, <>pLabel, bNoPaint, bSet, <>pLabelProps)
*   int PlObjLabelPropsDflt (<>pPlot, bSet, <>pLabelProps)
*   int PlObjLegendCreate   (<>pPlot, bNoCallback, >ppLegend,
*                                bHide, eRefPt, bPlotWin, fXFrac, fYFrac,
*                                bBox, rgbBox, szTitle, rgbTitle)
*   int PlObjLegendCreateEntry (<>pPlot, <>pLegend,
*                                eDrawType, fDrawPts,
*                                eMarkType, fMarkPts, rgbData,
*                                szText, rgbText)
*   int PlObjLegendHitTest  (<>pPlot, <>pObjCtx, pLegend, pcoMouse)
*  void PlObjLegendInit     (<>pPlot, <>pLegend)
*   int PlObjLegendPaint    (pPlot, pLegend, bSelected)
*  void PlObjLegendPaint_edit (pPlot)
*   int PlObjLegendProps    (<>pPlot, <>pLegend, bNoPaint, bSet, <>pLegendProps)
*   int PlObjLegendPropsDflt (<>pPlot, bSet, <>pLegendProps)
*   int PlObjMode           (<>pPlot, bNoCallback, bSet, <>pObjMode)
*   int PlObjPaint          (pPlot, pObj, bSelected)
*  void PlObjPaint_edit     (pPlot)
*   int PlObjPaintAll       (pPlot)
*   int PlObjSelect         (<>pPlot, bNoCallback, pObj, bSelect)
*   int PlObjValIsInRange   (pAxis, dVal)
*  void PlObjWrapup         (<>pPlot)
*  void PlWinMouseInit      (<>pPlot)
*  void PlWinMousePreProcess (<>pPlot, hWnd, iMsg, wParam, lParam)
*  void PlWinMouseSelectPointer (<>pPlot, hWnd, iMsg, wParam, lParam)
*  void PlWinMouseWrapup    (<>pPlot)
*   int PlWinRegObjectCallback (<>pPlot, pObjFn, pObjArg)
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
*                                   TEST PLAN
*
*	Some places in this code are somewhat fragile, sometimes because of the
*	complexities and subtleties of the problem--and sometimes because of less-
*	than-optimal coding.  I've taken the code as far as my abilities and the
*	time at hand will allow.
*
*	Regardless of the source of the fragility, it is crucial that any
*	modifications to this module be thoroughly tested.  This test plan uses
*	TST_OBJ.EXE.  If you're not familiar with this module, I strongly urge
*	you to run through the test BEFORE starting to mess with the code, just
*	so you can see what the intended behavior is.
*
*		INTRODUCTION
*		This section introduces some terms.
*
*		'data area'  the part of a plot area where plotted data actually
*				appears.  This is the interior of the plot 'grid'.
*		checkbox  numerous references are made to settings for mode,
*				enables, and properties.  When these refer to checkbox
*				items, values of 0 and 1 are used to mean unchecked
*				and checked, respectively.
*		'pointer'  is the appearance of the mouse pointer.  The usual
*				Windows terminology is 'cursor', which I haven't used
*				because I wanted to avoid confusion with 'plot cursor'.
*				There are several pointers that will be seen during this
*				test:
*				'arrow'  the standard Windows pointer.
*				'NOT!'  the international 'prohibited' circle and backslash.
*						This appears when an operation can't complete with
*						the pointer in its current position.
*				'plus'  the upright cross.  This indicates either that
*						a plot cursor operation is possible or that a plot object
*						operation is is possible.  Generally, the distinction
*						isn't important.
*				'plus and box'  a 'plus' with a 'box' superimposed.  This
*						indicates either: a new box CAN be created or a
*						new box IS being created.
*				'select'  a 'plus and box' with little solid squares at
*						the corners of the box.  This indicates that the
*						pointer is over the perimeter of a box that can
*						be selected for editing.
*				'resize'  any of the horizontal or diagonal double-headed
*						arrows.
*				'move'  the four-headed resize arrow.
*		'Pointer check:'  in the test procedure is a request for you to
*				move the mouse pointer around and verify the items that
*				are indicated.
*
* 1.	BASIC SANITY CHECKS
* 1.1	Mouse Pointer and Readback Checks, Button Up, 'Edit' and 'New' Disabled
*		These steps are done with the mouse buttons UP.
*		o	Select 'reset' from the 'Test:' dropdown list.
*		o	Move the mouse pointer around inside the plot window.  It
*			should be an arrow except in the data areas, where it should
*			be a 'plus'.
*		o	If the pointer is in the plot window, the L and B mouse
*			readbacks should change.  If it's outside, they shouldn't.
*		o	When the mouse pointer is inside the plot window, the 'Readbacks:
*			L' textbox should show:
*			-	0::xxx when the pointer is between the left and right edges
*				of the left data areas;
*			-	1::xxx when the pointer is between the left and right edges
*				of the right data areas; and
*			-	xxx otherwise.
*			While the pointer is inside a data area, the 'xxx' correspond
*			to X data values.  For several pointer positions inside a data
*			area, verify that the readback is consistent with the estimated
*			pointer position.  (When the pointer is outside the X range of
*			a data area, the 'xxx' corresponds to pixels; you have no easy
*			way to validate this readback.)
*		o	When the mouse pointer is inside the plot window, the 'Readbacks:
*			B' textbox should show:
*			-	0::xxx when the pointer is between the top and bottom edges
*				of the top data areas;
*			-	1::xxx when the pointer is between the top and bottom edges
*				of the bottom data areas; and
*			-	xxx otherwise.
*			While the pointer is inside a data area, the 'xxx' correspond
*			to Y data values.  For several pointer positions inside a data
*			area, verify that the readback is consistent with the estimated
*			pointer position.  (When the pointer is outside the X range of
*			a data area, the 'xxx' corresponds to pixels; you have no easy
*			way to validate this readback.)
* 1.2	Mouse Pointer Checks, Button Down, 'Edit' and 'New' Disabled
*		These steps are done with 'clicks' or 'drags' of the left mouse
*		button.  'Press' means: press the mouse button down.  'Release'
*		means: let the mouse button up.  'Click' means: press and release
*		the mouse button, without moving the mouse pointer.  'Drag' means:
*		press the mouse button, and move the mouse pointer while the mouse
*		button is pressed.
*		o	Select 'reset' from the 'Test:' dropdown list.
*		o	Click the left button outside the plot window.  Notice the
*			color of the border around the plot window.  This color means
*			that the plot window does not have the input focus.
*		o	Press the left button inside the plot window but outside any
*			data area.  You should see the 'NOT!' pointer.  Release the
*			left button.  Notice the color of the border around the plot
*			window.  This color means that the plot window has the input
*			focus--i.e., if you type on the keyboard the keystrokes will
*			be processed by the plotting software.
*		o	Click the left button inside a data area.  You should see the
*			normal plot cursor behavior.  (Click back and forth between
*			areas, successive clicks within a single area, etc.)
*		o	Drag the left button inside the plot window but outside a data
*			area.  You should see the 'NOT!' pointer.
*		o	Drag the left button inside a data area.  You should see the
*			'NOT!' pointer.  Drag it across data area boundaries; you should
*			see the 'NOT!' pointer.
*		o	For any of your 'drag' operations, the plot cursor shouldn't
*			change position.
* 1.3	Mouse Pointer Checks, Button Down, 'New' Disabled
*		o	Select 'reset' from the 'Test:' dropdown list.
*		o	Click the 'Edit mode' checkbox to select it.
*		o	Press the left button in a plot area.  You should see the 'plus'
*			pointer.  Drag the left button.  When you move the pointer, it
*			should change to the 'NOT!' pointer.  Release the left button.
*			The pointer should change to 'plus'.  The plot cursor shouldn't
*			change position.
*		o	Repeat the previous step in the other plot areas.  You should
*			observe similar behavior.
* 1.4	Check Mouse Pointer and Readbacks During Create
*		o	Select 'reset' from the 'Test:' dropdown list.
*		o	Click the 'Edit mode' checkbox to select it.
*		o	Click the 'Global enables: New' checkbox to select it.
*		o	With the left mouse button up, verify:
*			-	when the pointer is inside data areas it is 'plus and box'
*			-	when the pointer is outside data areas it is 'arrow'
*		o	Press the left button near the center of a data area.  The
*			pointer should be a 'plus and box'.
*		o	Drag the left left button, still inside the data area.  You
*			should see a 'rubber band' box that tracks the pointer motion.
*			Go above and below and right and left of your starting point.
*		o	While dragging, examine the 'Readbacks:' text boxes.  Compare
*			the value in the 'L' box with the estimated position of the
*			left side of the 'rubber band' box.  Make similar comparisons
*			for the other sides of the 'rubber band' box.
*		o	Release the left button when it is below and to the right of
*			your starting position.  The pointer should be a double-headed
*			resize arrow pointing upper-left to lower-right.  The box should
*			have a dotted border, alternating cyan and red.
*		o	Continue to the next test
* 1.5	Check Functionality and Mouse Pointer During Move/Resize
*		o	With the left button up, position the mouse pointer and
*			check its appearance:
*			-	on box top and bottom sides:  'resize' arrow, vertical
*			-	on box right and left sides:  'resize' arrow, horizontal
*			-	on box corners: 'resize' arrow, diagonal
*			-	inside box: 'move' arrow
*			-	outside box, inside data area: 'plus'
*			-	outside box, outside data area: 'arrow'
*		o	Starting with pointer on box perimeter, press the left mouse
*			button and drag a corner or side of the box to resize it. While
*			you're resizing, you should see both the box and the 'rubber band'
*			box.  When you release the left mouse button, the 'rubber band'
*			and the original box should disappear and the box should reappear
*			where the 'rubber band' box was.  Play with resizing the box,
*			keeping the pointer within the data area.  Try resizing so that
*			what started out as the right side becomes the left.  Do similar
*			resizes for top and bottom, and for the corners.  There shouldn't
*			be any surprises. 
*		o	Starting with pointer in box interior, press the left mouse
*			button and drag the box around.  While you're moving the box,
*			and when you release the left mouse button, the behavior should
*			be similar to when you resized the box.  Move the box around,
*			always keeping it box within the data area.  There shouldn't be
*			any surprises.
*		o	Play with resizing the box, moving the pointer outside the
*			data area but with the final pointer position inside the data
*			area.  While it is outside the data area, the pointer should
*			be 'NOT!'.  When you release the left button, the box should
*			reappear with its new size.
*		o	Play with resizing the box, but with the pointer outside the
*			data area when you release the left button.  When the pointer
*			is outside the data area that contains the box, it should be
*			'NOT!'.  When you release the left button, the box should be
*			unchanged.
*		o	Play with moving the box, moving the pointer so that either
*			the pointer itself or one or more edges of the box are outside
*			a data area.  When any edge of the box is outside a data area,
*			the pointer should be 'NOT!'.  If you release the left button
*			while the pointer is 'NOT!', the box should be unchanged.  If
*			the pointer isn't 'NOT!', the box should appear in its new
*			position.  Move it to the other data area.  There shouldn't
*			be any surprises.
*		o	Continue to the next test
* 1.6	Check Functionality and Mouse Pointer During Deselect/Select
*		o	Click the left button outside the perimeter of the box (but
*			inside the plot window).  The box should become a solid cyan
*			box.  The plot cursor shouldn't change position.
*		o	With the left button up, position the mouse pointer and
*			check its appearance:
*			-	on box sides and corners: 'select'
*			-	inside box: 'plus'
*			-	outside box, inside data area: 'plus'
*			-	outside box, outside data area: 'arrow'
*		o	Click the left button on the perimeter of the box.  The box
*			should become the dotted red/cyan box, and the pointer should
*			become 'resize'.
*		o	Continue to the next test
* 1.7	Check Functionality During Repaint
*		o	Cover the TST_OBJ window with another and then expose the
*			TST_OBJ window.  You should see the dotted red/cyan box.
*		o	Click the left button outside the perimeter of the box.  The
*			box should become a solid cyan box.
*		o	Cover the TST_OBJ window with another and then expose the
*			TST_OBJ window.  You should see the solid cyan box.
*		o	Continue to the next test
* 1.8	Check Functionality During Delete and Create, 'New' Disabled
*		o	Click the left button on the perimeter of the box.  Press the
*			DEL or BACKSPACE key.  The box should disappear.  The pointer
*			should become 'plus and box'.
*		o	Create a new box.  There should be no surprises.
*		o	Attempt to create another new box.  The pointer should be
*			'NOT!' and there shouldn't be a new box.
* 1.9	Check Functionality When Creating Multiple Boxes, 'Many' Enabled
*		o	Select 'reset' from the 'Test:' dropdown list.
*		o	Click the 'Edit mode' checkbox to select it.
*		o	Click the 'Global enables: New' checkbox to select it.
*		o	Click the 'Global enables: Many' checkbox to select it.
*		o	You should be able to create as many boxes as you like.  Play
*			with creating, resizing, moving, and deleting boxes.  There
*			shouldn't be any surprises.
*		o	When you are done with this step, leave at least two boxes to
*			use in the next step.
*		o	With the left button up, position the mouse pointer and
*			check its appearance:
*			-	on perimeter of selected box:  'resize'
*			-	interior of selected box: 'move'
*			-	on perimeter of non-selected box:  'select'
*			-	interior of non-selected box: 'plus and box'
*			-	outside box, in data area: 'plus and box'
*			-	outside box, outside data area: 'arrow'
*		o	Continue to the next test
* 1.10	Check Functionality With Multiple Boxes, 'Many' Disabled
*		o	Click the 'Global enables: Many' checkbox to de-select it.
*		o	Attempt to create another new box.  The pointer should be
*			'NOT!' and there shouldn't be a new box.
*		o	Play with editing existing boxes.  You should be able to do
*			anything you've done in prior tests.  There shouldn't be any
*			surprises.
*		o	One at a time, delete existing boxes and attempt to create
*			a new one.  You shouldn't be able to create a new box until
*			all the existing boxes have been deleted.
* 1.11	Check Functionality For Boxes That Span Plot Areas
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	Start creating a box that spans data areas.  While the pointer
*			is between plot areas, it should be 'NOT!'.  Otherwise, it
*			should be 'plus and box'.  Compare the values in the
*			'Readbacks:' text boxes with the sides of the 'rubber band'
*			box to make sure that the readbacks agree with the estimated
*			positions of the sides of the box.  Release the left mouse
*			button to complete the create.
*		o	Play with creating boxes that span data areas.  Releasing the
*			left button while the pointer is 'NOT!' shouldn't produce a
*			new box.
*		o	Play with moving and resizing boxes so that they span data
*			areas.  When one or more edges of a box are outside a data
*			area, the pointer should be 'NOT!'.  Releasing the left
*			button while the pointer is 'NOT!' shouldn't change the box.
*			Otherwise, the box should take its new size/position where
*			the 'rubber band' was.
*		o	For a box that spans data areas, with the pointer starting
*			BETWEEN data areas, verify that you can select the box,
*			resize it, and move it without getting the 'NOT!' pointer.
*			(Unless, of course, you pass through a state where an edge
*			of the box lies totally outside a data area.)
* 1.12	'Edit mode' Repainting
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	Create a box.
*		o	Switch back and forth between 'Edit mode' 1 and 'Edit mode' 0.
*			Verify that the plot window repaints each time.  Verify that
*			you can't select, edit, create, etc. while 'Edit mode' is 0.
*			If a box is selected when you switch to 'Edit mode' 0, it
*			should repaint as non-selected.  When 'Edit mode' is 0, you
*			should never get 'select', 'resize', or 'move' pointers.
*
* 2		GLOBAL ENABLE FLAGS AND PL_BOX PROPERTIES
* 2.1	Test Functionality Of Box Properties and Global Property Enables
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	In the 'Box properties' section, uncheck the 'Size', 'Move',
*			and 'Del' checkboxes.  Notice that in the 'Global enables'
*			section the 'Size', 'Move', and 'Del' enables are not checked.
*		o	Create a box and select it.    Verify that the box can't be
*			resized, moved, or deleted.  No matter where you put the
*			mouse pointer--on the perimeter or interior of the box--the
*			pointer should be 'NOT!'.
*		o	Unselect the box by clicking elsewhere in the plot window.
*			When you move the mouse pointer onto the box's perimeter,
*			the pointer should be 'select'.
*		o	Select the box.
*		o	Check the 'Size' property and verify that you can resize the
*			box.  Uncheck the 'Size' property and check the 'Size' enable.
*			Verify that you can resize the box.  With both 'Size' enable
*			and 'Size' property checked, verify that you can resize the
*			box.
*		o	Check the 'Move' property and verify that you can move the
*			box.  Uncheck the 'Move' property and check the 'Move' enable.
*			Verify that you can move the box.  With both 'Move' enable
*			and 'Move' property checked, verify that you can move the
*			box.
*		o	Check the 'Del' property and verify that you can delete the
*			box.  Create a new box and leave it selected.  Uncheck the
*			'Del' property and check the 'Del' enable.  Verify that you
*			can delete the box.  Create a new box and leave it selected.
*			With both 'Del' enable and 'Del' property checked, verify
*			that you can delete the box.
* 2.2	Check Other Box Properties
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	In the 'Box properties' section listboxes, select thick line,
*			solid pattern, and RED color.
*		o	Create a new box and deselect it.  Verify that it has the
*			specified properties.
*		o	Select the box and check the 'RRect' checkbox.  Verify that the
*			box becomes a rounded rectangle even though it is still selected.
*		o	Check the 'Fill' checkbox and deselect the box.  Verify that
*			the box is filled.  Select the box and verify that the fill
*			disappears.
*		o	Create a new box and deselect it.  Verify that it has the same
*			properties as the previous one.
*		o	Select the new box and change its properties.  Deselect it to
*			verify that the new properties have been applied.
*		o	With each of the thicknesses create a box, deselect it, and
*			verify that it has the specified thickness.
*		o	With each of the patterns create a box, deselect it, and verify
*			that it has the specified pattern.
*		o	With each of the colors create a box, deselect it, and verify
*			that it has the specified color.
*		o	In succession select boxes that have differing properties and
*			verify for each that the checkboxes and listboxes in the 'Box
*			properties' section match the box's properties.
*		o	Select a box.  Choose a thick line and a non-solid line pattern
*			as properties for the box.  Deselect the box.  Verify that the
*			box has a thin, patterned border.  (Windows 3.1 doesn't support
*			thick patterned lines and PL.LIB overrides thickness with pattern.)
*
* 3		ADVANCED FEATURES
* 3.1	Constrained Move
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	Create a box.
*		o	Hold SHIFT key as you move the box.  Try to move it various
*			places around the data area.  Verify that it snaps back and
*			forth between strictly horizontal and strictly vertical from
*			its starting position.
*		o	Start moving a box without the SHIFT key depressed, but play
*			with pressing and releasing the SHIFT key several times during
*			the move.  Verify that while the SHIFT key is pressed the box
*			snaps to its vertical or horizontal alignment and while the
*			SHIFT key is up the box follows the mouse pointer.
* 3.2	Copy a Box
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	Create a box.
*		o	Position the mouse pointer over the box, so that the 'move'
*			pointer shows.
*		o	Press the CTRL key, then the left button and drag.  The box
*			that you started with should be left behind, in a deselected
*			state.  A 'rubber band' box should follow the mouse pointer.
*			When you release the left mouse button you should have a copy
*			of the original box.  Verify that it has the same size and
*			properties.
* 3.3	Auto Zoom and Restore
*		o	Select 'autoZoom' from the 'Test:' dropdown list.
*		o	Select the upper-left green box and click the 'ZoomSel' button.
*			Verify that the box now occupies the full upper left data area.
*			Moving the mouse pointer over the edges of the box should result
*			in the appropriate 'resize' pointer.  With the pointer in the
*			interior of the box, you should see the 'move' pointer.  Verify
*			that the 'thumb' for the vertical scroll bar is close to the
*			top and that the 'thumb' for the horizontal scroll bar is close
*			to the left.  Click the 'Restore' button and verify that the
*			plot window has been restored to its original un-zoomed state.
*		o	Select the upper-right green box and perform the same test.
*			The 'thumbs' should be at the top and right for the vertical
*			and horizontal scroll bars, respectively.
*		o	Select the lower-right green box and perform the same test.
*			The 'thumbs' should be at the bottom and right for the vertical
*			and horizontal scroll bars, respectively.
*		o	Select the lower-left green box and perform the same test.
*			The 'thumbs' should be at the bottom and left for the vertical
*			and horizontal scroll bars, respectively.
*		o	Select the left red box and perform the same test.  Only the
*			horizontal scroll bar should appear; its 'thumb' should be
*			about one-sixth of the way from the left.
*		o	Select the top red box and perform the same test.  Only the
*			vertical scroll bar should appear; its 'thumb' should be about
*			two-fifths of the way down from the top.
*		o	Select the right red box and perform the same test.  Only the
*			horizontal scroll bar should appear; its 'thumb' should be
*			about one-sixth of the way from the right.
*		o	Select the bottom red box and perform the same test.  Only the
*			vertical scroll bar should appear; its 'thumb' should be about
*			two-fifths of the way up from the bottom.
*		o	Select the magenta box and perform the same test.  The plot
*			window should not be zoomed--neither scroll bar should appear.
* 3.4	Callback Function Messages
*		(Note that the callback log isn't a feature of the plot object facility
*		but is just the way that TST_OBJ uses to display which messages
*		are sent to the object callback function.  For some messages only
*		the message type is displayed; for others, additional information
*		is shown.)
*		o	Select 'autoZoom' from the 'Test:' dropdown list.
*		o	Check the 'Event' and 'Mouse' checkboxes in the 'callback log'
*			panel, to enable TST_OBJ to write the messages to the text
*			box.
*		o	Slowly move the mouse pointer a short distance into the plot
*			window.  Verify that a stream of PL_MOUSE_MOVE messages appears.
*		o	With the mouse pointer in a data area, click the left mouse
*			button, without moving it.  Verify that the following appear:
*			-	PL_OBJ_EV_EDIT: create begin
*			-	PL_OBJ_EV_EDIT: create abort
*			-	PL_MOUSE_MOVE: 0 0
*		o	Create a new box.  Verify that you get:
*			-	PL_OBJ_EV_EDIT: create begin
*			-	a stream of PL_MOUSE_MOVE messages interspersed with
*				'PL_OBJ_EV_EDIT: create coord' messages.
*			-	PL_OBJ_EV_EDIT: create done
*		o	Uncheck the 'Mouse' checkbox, to disable logging mouse messages.
*		o	Deselect the box by clicking in the data area.  You should see:
*			-	PL_OBJ_EV_SEL: 0 0
*			-	PL_OBJ_EV_EDIT: create begin
*			-	PL_OBJ_EV_EDIT: create abort
*		o	Select a box and delete it.  You should see:
*			-	PL_OBJ_EV_SEL: 1 0
*			-	PL_OBJ_EV_SEL: 0 0
*			-	PL_OBJ_EV_DEL: 0 0
*		o	Select a box and then select another.  You should see:
*			-	PL_OBJ_EV_SEL: 1 0
*			-	PL_OBJ_EV_SEL: 0 0
*			-	PL_OBJ_EV_SEL: 1 0
*		o	Move a box.  You should see:
*			-	PL_OBJ_EV_EDIT: move begin
*			-	PL_OBJ_EV_EDIT: move coord -- a few of these
*			-	PL_OBJ_EV_EDIT: move done
*		o	Move a box and release the left mouse button while the pointer
*			is 'NOT!'.  You should see the following as the last message:
*			-	PL_OBJ_EV_EDIT: move abort
*		o	Resize a box.  You should see:
*			-	PL_OBJ_EV_EDIT: resize begin
*			-	PL_OBJ_EV_EDIT: resize coord -- a few of these
*			-	PL_OBJ_EV_EDIT: resize done
*		o	Resize a box and release the left mouse button while the pointer
*			is 'NOT!'.  You should see the following as the last message:
*			-	PL_OBJ_EV_EDIT: resize abort
* 3.5	Keyboard/Mouse Move and Resize
*		(Note that in order to use the keyboard with the plot window, the
*		plot window must have the input focus.  This feature of the plot
*		window was covered the first part of these tests.  If you type a
*		key and it has no effect, you may need to click the mouse inside
*		the plot window do that it has the input focus.)
*		o	Select 'autoZoom' from the 'Test:' dropdown list.
*		o	Select a box and position the mouse pointer on the right side
*			of the box, so that the 'resize' pointer is shown.
*		o	Type the right and left arrow keys, both with and without
*			SHIFT.  Verify that the side of the box moves right and left,
*			1 pixel without SHIFT, 10 pixels with SHIFT.  Verify that the
*			mouse pointer pops to the top right corner and moves as the
*			side moves.
*		o	Position the mouse pointer on the top side of the box, so that
*			the 'resize' pointer is shown.
*		o	Type the up and down arrow keys, both with and without SHIFT.
*			Verify that the side of the box moves up and down, 1 pixel
*			without SHIFT, 10 pixels with SHIFT.  Verify that the mouse
*			pointer pops to the top right corner and moves as the side moves.
*		o	Position the mouse pointer on the other sides, the corners,
*			and the center of the box and perform similar tests.
*		o	Verify the following pointer handling features:
*			-	When the pointer starts in either left corner, on the left
*				side, or on the bottom side, it is forced to the bottom
*				left corner.
*			-	When the pointer starts in either right corner, on the right
*				side, or on the top side, it is forced to the top right
*				corner.
*		o	Uncheck the 'Delta is pixel' box and change the 'delta' values
*			used by up and down arrow: 'Y:F' = 10, 'C' = 100.
*		o	Perform tests similar to the ones above.  Notice what is
*			displayed in the readbacks.  Verify that X coordinates change
*			by 1 or 10 channels and Y coordinates change by 10 or 100 counts.
* 3.6	Keyboard-only Move and Resize
*		In this test you're asked to type several keys.  In the text, the
*		key (e.g. N) is shown upper-case, simply so it stands out.  The
*		software, however, ignores case.  You must be aware, however, that
*		the "input focus" must be in the plot area in order for the
*		keystrokes to have an effect.
*		o	Select 'many,span' from the 'Test:' dropdown list.
*		o	Create several boxes, keeping track of the order of their
*			creation.  Deselect the last one.
*		o	Type the N key.  Verify that the first box you created is
*			selected, with a 'move' pointer in its center.  Deselect
*			the box.
*		o	Type the P key.  Verify that the last box you created is
*			selected, with a 'move' pointer in its center.
*		o	Play with the N and P keys.  Verify that the N key selects
*			the next box in the series you created and that when you
*			reach the last one the first is the next to be selected.
*			Verify that the P key selects the previous box in the series
*			and that when you reach the first one the last is the next
*			to be selected.
*		o	Select a box.  Type the B key and verify that the pointer
*			moves to the lower left corner as a 'resize' pointer.  Type
*			the C key and verify that the pointer moves to the center
*			of the box as a 'move' pointer.  Type the T key and verify
*			that the pointer moves to the upper right corner as a
*			'resize' pointer.
* 3.7	Automatic scrolling
*		o	Select 'autoZoom' from the 'Test:' dropdown list.
*		o	Click 'ZoomH' and 'ZoomV' once each, so that most of the boxes
*			aren't visible.
*		o	Click the left mouse button in the plot window, so it gets
*			the input focus.
*		o	Type the N key.  Verify that the selected box is visible,
*			with the 'move' pointer in its center.  Type the N key to
*			move through the list of boxes, verifying that scroll position
*			automatically changes so that each becomes visible.
*		o	Perform a similar test, with the P key.
*		o	Move one of the boxes using the arrow keys.  Verify that as
*			the mouse pointer passes the edge of the data, the plot is
*			scrolled so that the pointer stays visible.  Verify that the
*			box can't be moved 'out of range'.  E.g., verify that the box
*			can't be moved so that its left edge goes past channel zero.
*		o	Resize the box using the arrow keys (using the B and T keys
*			to select which corner of the box is being resized).  Verify
*			that the scroll position adjusts to keep the pointer visible.
*		o	Select a box that is larger than the visible data area.  Type
*			the B, C, and T in succession and verify that the scroll
*			position automatically adjusts so that the bottom-left, center,
*			and top-right of the box become visible.
* 3.8	Changing Plot Areas With Keyboard Move and Resize
*		o	Uncheck the 'Span' checkbox.
*		o	Create a box in the upper plot area, and select it.  Note
*			the value coordinates of the top and bottom edges.  (An
*			easy way to see the coordinates is to type right_arrow.)
*		o	Type C, then CTRL+down_arrow.  Verify that the box moves
*			into the bottom plot area, with the same value coordinates.
*		o	Type CTRL+up_arrow.  Verify that the box moves back to
*			the upper plot area, with the same value coordinates.
*		o	Try using CTRL+xxx_arrow to move the box to places where
*			it shouldn't go.  Verify that illegal operations aren't
*			performed.
*		o	Check the 'Span' checkbox.
*		o	With the mouse pointer on the various edges of the box,
*			use CTRL+arrow to get the box to span and un-span plot
*			areas.  Verify that up and down arrow keys can change
*			the span.  Verify that the right and left arrow keys
*			have no effect.
*
* 4		NUANCES
* 4.1	Clipping With Zoom and Scroll
*		o	Select 'Y clip' from the 'Test:' dropdown list.
*		o	Click the 'ZoomV' button twice.
*		o	Scroll up and down in increments, making a check when the
*			'thumb' is at the bottom, another check with the 'thumb'
*			at the top, and several checks in between.  Verify that:
*			-	the right and left sides of the green boxes never extend
*				outside the data area
*			-	the right and left sides of the red boxes always appear in
*				the space between data areas
*		o	Select 'X clip' from the 'Test:' dropdown list.
*		o	Click the 'ZoomH' button twice.
*		o	Scroll right and left in increments, making a check when the
*			'thumb' is at the left, another check with the 'thumb'
*			at the right, and several checks in between.  Verify that:
*			-	the top and bottom sides of the green boxes never extend
*				outside the data area
*			-	the top and bottom sides of the red boxes always appear in
*				the space between data areas
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* pl_obj.cpp - plot-object-related plotting functions
*
* Date     Author		Revision
* -------- ------------	--------
* 12-05-96 R. Cole		created
*
*--------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------
* Copyright 1996-97, The Regents Of the University of California.  This
* program was prepared by the Regents of the University of California at
* Los Alamos National Laboratory (the University) under Contract No. W-7405-
* ENG-36 with the U.S. Department of Energy (DOE).  The University has
* certain rights in the program pursuant to the contract and the program
* should not be copied or distributed outside your organization.  All rights
* in the program are reserved by the DOE and the University.  Neither the
* U.S. Government nor the University makes any warranty express or implied,
* or assumes any liability or responsibility for the use of this software.
*
* This software was produced by the Safeguards Science and Technology
* Group (NIS-5).
*--------------------------------------------------------------------------*/

#include <windows.h>
#include <limits.h>		// for INT_MAX
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <listsubr.h>
#include "pl.h"
#include "pl_pvt.h"
#include "wu.h"

#if !defined WIN32
#define FAR16 _far
#define EXPORT16 __export
#define EXPORT32
#else
#define FAR16
#define EXPORT16
#define EXPORT32 __declspec(dllexport)
#endif

/*---------------------------------------------------------------------------
*	local definitions and prototypes
*--------------------------------------------------------------------------*/

#define IDC_PLOBJ_CANCEL 100
#define IDC_PLOBJ_OK 101
#define IDC_PLOBJ_EDIT_BOX 102
#define IDC_PLOBJ_HELP 103

void PASCAL
PlObjBoxInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// IO pointer to PL_BOX
int		bHide);			// I 1 if box is not to be shown, else 0

int PASCAL
PlObjBoxPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_BOX	*pBox,			// I pointer to PL_BOX to draw
int		bSelected);		// I 1 if the PL_BOX is selected

void PASCAL
PlObjCoordAddDeltaX(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pco,			// IO pointer to coordinate to modify
int		bTrackMouse,	// I 1 if want side "stuck to" mouse
int		bDeltaPx,		// I 1 if dDeltaX is pixels
double	dDeltaX);		// I delta X, in pixels

void PASCAL
PlObjCoordAddDeltaY(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pco,			// IO pointer to coordinate to modify
int		bTrackMouse,	// I 1 if want side "stuck to" mouse
int		bDeltaPx,		// I 1 if dDeltaY is pixels
double	dDeltaY);		// I delta Y, in pixels

int PASCAL
PlObjEditDoneIsOK(
PL_CTX	*pPlot);		// IO pointer to plot context structure

int PASCAL
PlObjEditStartIsOK(
PL_CTX	*pPlot);		// I pointer to plot context structure

HDC PASCAL
PlObjHDCHandler(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ	*pObj,			// I the box that's being set up for, or NULL
int		bReleaseDC);	// I 1,0 to release,setup DC

int PASCAL
PlObjHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_OBJ	*pObj,			// I pointer to PL_OBJ to test
PL_COORD *pcoMouse);	// I coordinate to test

int PASCAL
PlObjKeyboardMove(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ	*pObj,			// IO the PL_OBJ to move
int		bDeltaPx,		// I 1 if dDeltaX and dDeltaY are pixels
double	dDeltaX,		// I amount to add to X
double	dDeltaY,		// I amount to add to Y
int		iDeltaRow,		// I amount to add to row number
int		iDeltaCol);		// I amount to add to column number

void PASCAL
PlObjKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_OBJ	*pObj,			// IO the object to move
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac);	// O anchor fraction if scroll change needed

int PASCAL
PlObjLabelHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_LABEL *pLabel,		// I pointer to PL_LABEL to test
PL_COORD *pcoMouse);	// I coordinate to test

void PASCAL
PlObjLabelInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LABEL *pLabel);		// IO pointer to PL_LABEL

void PASCAL
PlObjLabelKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_LABEL *pLabel,		// I the object to place mouse on
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac);	// O anchor fraction if scroll change needed

int PASCAL
PlObjLabelPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_LABEL *pLabel,		// I pointer to PL_LABEL to draw
int		bSelected);		// I 1 if the label is selected

void PASCAL
PlObjLabelPaint_edit(
PL_CTX	*pPlot);		// I pointer to plot context structure

int PASCAL
PlObjLegendHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_LEGEND *pLegend,		// I pointer to PL_LEGEND to test
PL_COORD *pcoMouse);	// I coordinate to test

void PASCAL
PlObjLegendInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LEGEND *pLegend,		// IO pointer to PL_LEGEND
int		bHide);			// I 1 if legend is not to be shown, else 0

void PASCAL
PlObjLegendKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_LEGEND *pLegend,		// I the object to place mouse on
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac);	// O anchor fraction if scroll change needed

int PASCAL
PlObjLegendPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_LEGEND *pLegend,		// I pointer to PL_LEGEND to draw
int		bSelected);		// I 1 if the PL_LEGEND is selected

void PASCAL
PlObjLegendPaint_edit(
PL_CTX	*pPlot);		// I pointer to plot context structure

int PASCAL
PlObjPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_OBJ	*pObj,			// I pointer to object to draw
int		bSelected);		// I 1 if the object is selected

void PASCAL
PlObjPaint_edit(
PL_CTX	*pPlot);		// I pointer to plot context structure

void PASCAL
PlObjTextColor(
PL_CTX	*pPlot,
int		bFill,
COLORREF rgbFill,
COLORREF rgbBkText,
COLORREF *prgbText);

int PASCAL
PlObjValIsInRange(
PL_AXIS	*pAxis,			// I pointer to PL_AXIS structure
double	dVal);			// I data value to check

LRESULT EXPORT16 CALLBACK
PlObjWinProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam);

void PASCAL
PlObjWinPaint(
PL_CTX	*pPlot,			// IO pointer to plot context struct
HWND	hwObjDialog,	// I handle of the window
int		bPaint,			// I 1 to print, 0 to lay out window
RECT	*prWin,			// I pointer to window's rect
HDC		hDC,			// IO handle to DC for edit 'dialog'
int		*piYpx);		// IO pointer to Y position


/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxCoordCalcPx - calculate pixel-related values for PL_BOX
*
* Purpose:
*		Using the dVal-related information in a coordinate pair for a
*		PL_BOX, calculates the corresponding pixel-ralated items.
*
*		This routine does NOT move a coordinate pair from one plot area
*		to another.
*
*		See PlObjBoxCvtBoxToCoord for a fuller description.  This routine
*		is useful when a coordinate pair exists but needs to have the
*		pixel-related items refreshed.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-19-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxCoordCalcPx(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bShowFullHt,	// I 1 if the box is being shown full height
PL_COORD *pcoLB,		// IO the left bottom coordinate
PL_COORD *pcoRT)		// IO the right top coordinate
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		iPlotLeftPx=pPlot->rectPlot.left;
	int		iPlotRightPx=pPlot->rectPlot.right;
	int		iPlotTopPx=pPlot->rectPlot.top;
	int		iPlotBottomPx=pPlot->rectPlot.bottom;
	int		row, col, rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		stat;
	double	dVal, dPx;

	// left side
	// Set values so that this edge CAN'T be drawn.
	pcoLB->x.bValOK = pcoLB->x.bEdgeVis = 0;
	pcoLB->x.iVisPx = pcoLB->x.iDrawPx = pcoLB->x.iClipPx = iPlotRightPx;
	pcoLB->x.dVirtPx = pcoLB->x.iVisPx;
	dVal = pcoLB->x.dVal;
	col = pcoLB->x.iRowCol;
	if (col < 0) goto left_is_done;
	if ((stat = PlAreaArraySelect(pPlot, rowSave, col)) != PL_OK) goto left_is_done;
	if (PlObjValIsInRange(pPlot->pXAxis, dVal))
		pcoLB->x.bValOK = 1;
	dPx = PlWinCvtXValToXPix(pPlot, dVal);
	pcoLB->x.dVirtPx = dPx;
	// move into range of pixels occupied by plot window
	dPx = PL_MAX(dPx, iPlotLeftPx - 10);
	dPx = PL_MIN(dPx, iPlotRightPx + 10);
	if (dPx < pPlot->pXAxis->pxB) {
		// Left side is to left of data area.  Clip box at
		// left side of area.
		pcoLB->x.iClipPx = pPlot->pXAxis->pxB;
		pcoLB->x.iVisPx = pcoLB->x.iClipPx;
		pcoLB->x.iDrawPx = pcoLB->x.iClipPx - 10;
	}
	else if (dPx > pPlot->pXAxis->pxT) {
		// Left side is to right of data area.  Clip box at
		// right side of area.
		pcoLB->x.iClipPx = pPlot->pXAxis->pxT + 1;
		pcoLB->x.iVisPx = pcoLB->x.iClipPx;
		pcoLB->x.iDrawPx = pcoLB->x.iClipPx - 10;
	}
	else {
		pcoLB->x.iClipPx = pPlot->pXAxis->pxB;
		pcoLB->x.iVisPx = pcoLB->x.iDrawPx = (int)dPx;
		pcoLB->x.bEdgeVis = 1;
	}
left_is_done:

	// right side
	// Set values so that this edge CAN'T be drawn.
	pcoRT->x.bValOK = pcoRT->x.bEdgeVis = 0;
	pcoRT->x.iVisPx = pcoRT->x.iDrawPx = pcoRT->x.iClipPx = iPlotLeftPx;
	pcoRT->x.dVirtPx = pcoRT->x.iVisPx;
	dVal = pcoRT->x.dVal;
	col = pcoRT->x.iRowCol;
	if (col < 0) goto right_is_done;
	if ((stat = PlAreaArraySelect(pPlot, rowSave, col)) != PL_OK) goto right_is_done;
	if (PlObjValIsInRange(pPlot->pXAxis, dVal))
		pcoRT->x.bValOK = 1;
	dPx = PlWinCvtXValToXPix(pPlot, dVal);
	pcoRT->x.dVirtPx = dPx;
	// move into range of pixels occupied by plot window
	dPx = PL_MAX(dPx, iPlotLeftPx - 10);
	dPx = PL_MIN(dPx, iPlotRightPx + 10);
	if (dPx > pPlot->pXAxis->pxT) {
		// Right side is to right of data area.  Clip box at
		// right side of area.
		pcoRT->x.iClipPx = pPlot->pXAxis->pxT;
		pcoRT->x.iVisPx = pcoRT->x.iClipPx;
		pcoRT->x.iDrawPx = pcoRT->x.iClipPx + 10;
	}
	else if (dPx < pPlot->pXAxis->pxB) {
		// Right side is to left of data area.  Clip box at
		// left side of area.
		pcoRT->x.iClipPx = pPlot->pXAxis->pxB - 1;
		pcoRT->x.iVisPx = pcoRT->x.iClipPx;
		pcoRT->x.iDrawPx = pcoRT->x.iClipPx + 10;
	}
	else {
		pcoRT->x.iClipPx = pPlot->pXAxis->pxT;
		pcoRT->x.iVisPx = pcoRT->x.iDrawPx = (int)dPx;
		pcoRT->x.bEdgeVis = 1;
	}
right_is_done:

	// bottom side
	// Set values so that this edge CAN'T be drawn.
	pcoLB->y.bValOK = pcoLB->y.bEdgeVis = 0;
	pcoLB->y.iVisPx = pcoLB->y.iDrawPx = pcoLB->y.iClipPx = iPlotTopPx;
	pcoLB->y.dVirtPx = pcoLB->y.iVisPx;
	dVal = pcoLB->y.dVal;
	row = pcoLB->y.iRowCol;
	if (row < 0) goto bottom_is_done;
	if ((stat = PlAreaArraySelect(pPlot, row, colSave)) != PL_OK) goto bottom_is_done;
	if (PlObjValIsInRange(pPlot->pYAxis, dVal))
		pcoLB->y.bValOK = 1;
	dPx = PlWinCvtYValToYPix(pPlot, dVal);
	// For PL_BOXs that are to be shown full-height, expand or contract
	// if they're not in the selected state.
	if (bShowFullHt)
		dPx = pPlot->pYAxis->pxB - 1;
	pcoLB->y.dVirtPx = dPx;
	// move into range of pixels occupied by plot window
	dPx = PL_MAX(dPx, iPlotTopPx - 10);
	dPx = PL_MIN(dPx, iPlotBottomPx + 10);
	if (dPx > pPlot->pYAxis->pxB) {
		// Bottom side is below data area.  Clip box at bottom of area.
		pcoLB->y.iClipPx = pPlot->pYAxis->pxB;
		pcoLB->y.iVisPx = pcoLB->y.iClipPx;
		pcoLB->y.iDrawPx = pcoLB->y.iClipPx + 10;
	}
	else if (dPx < pPlot->pYAxis->pxT) {
		// Bottom side is above data area.  Clip box at top of area.
		pcoLB->y.iClipPx = pPlot->pYAxis->pxT - 1;
		pcoLB->y.iVisPx = pcoLB->y.iClipPx;
		pcoLB->y.iDrawPx = pcoLB->y.iClipPx + 10;
	}
	else {
		pcoLB->y.iClipPx = pPlot->pYAxis->pxB;
		pcoLB->y.iVisPx = pcoLB->y.iDrawPx = (int)dPx;
		pcoLB->y.bEdgeVis = 1;
	}
bottom_is_done:

	// top side
	// Set values so that this edge CAN'T be drawn.
	pcoRT->y.bValOK = pcoRT->y.bEdgeVis = 0;
	pcoRT->y.iVisPx = pcoRT->y.iDrawPx = pcoRT->y.iClipPx = iPlotTopPx;
	pcoRT->y.dVirtPx = pcoRT->y.iVisPx;
	dVal = pcoRT->y.dVal;
	row = pcoRT->y.iRowCol;
	if (row < 0) goto top_is_done;
	if ((stat = PlAreaArraySelect(pPlot, row, colSave)) != PL_OK) goto top_is_done;
	if (PlObjValIsInRange(pPlot->pYAxis, dVal))
		pcoRT->y.bValOK = 1;
	dPx = PlWinCvtYValToYPix(pPlot, dVal);
	// For PL_BOXs that are to be shown full-height, expand or contract
	// if they're not in the selected state.
	if (bShowFullHt)
		dPx = pPlot->pYAxis->pxT + 1;
	pcoRT->y.dVirtPx = dPx;
	// move into range of pixels occupied by plot window
	dPx = PL_MAX(dPx, iPlotTopPx - 10);
	dPx = PL_MIN(dPx, iPlotBottomPx + 10);
	if (dPx < pPlot->pYAxis->pxT) {
		// Top side is above data area.  Clip box at top of area.
		pcoRT->y.iClipPx = pPlot->pYAxis->pxT;
		pcoRT->y.iVisPx = pcoRT->y.iClipPx;
		pcoRT->y.iDrawPx = pcoRT->y.iClipPx - 10;
	}
	else if (dPx > pPlot->pYAxis->pxB) {
		// Top side is below data area.  Clip box at bottom of area.
		pcoRT->y.iClipPx = pPlot->pYAxis->pxB + 1;
		pcoRT->y.iVisPx = pcoRT->y.iClipPx;
		pcoRT->y.iDrawPx = pcoRT->y.iClipPx - 10;
	}
	else {
		pcoRT->y.iClipPx = pPlot->pYAxis->pxT;
		pcoRT->y.iVisPx = pcoRT->y.iDrawPx = (int)dPx;
		pcoRT->y.bEdgeVis = 1;
	}
top_is_done:

	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxCreateAtPlotCursor - create a PL_BOX at the plot cursor
*
* Purpose:
*		Creates a new PL_BOX at the plot cursor.
*
*		dValL specifies, in data units, how far to the left of the plot
*		cursor the left side of the PL_BOX should be; dValR how far to the
*		right the right side should be; dValT how far above the plot cursor
*		the top side of the PL_BOX should be; dValB how far below the bottom
*		side should be.
*
*		If bPreferClip is 1, then this routine tries to keep the PL_BOX
*		positioned as implied by the arguments.  The height and width of the
*		new PL_BOX may be reduced to fit between the edge of the plot area
*		and the plot cursor.  For example, assume that data value X=0, Y=0
*		is at the lower left corner of the plot area and that the plot
*		cursor is at 0,0.  A request to create a PL_BOX with left and right
*		sides -10 and +10 data units left and right of the plot cursor would
*		result in a PL_BOX whose left side is at the cursor and whose right
*		side is at +10.  The top and bottom are treated in a similar way.
*
*		If bPreferClip is 0, then this routine attempts to keep the PL_BOX
*		sized as implied by the arguments.  If the plot cursor is too close
*		to the edge of the plot area to accomodate the specified position
*		and size, then the new PL_BOX is shifted horizontally and/or
*		vertically relative to the plot cursor in order to achieve the
*		implied width and height.  (But if shifting isn't sufficient to
*		accomodate the new box, then its width and/or height will be clipped
*		as needed.)
*
*		If the size and/or position of the box is modified from the values
*		implied by the arguments, this routine returns PL_GEOM_CHANGED as
*		the status.  If the new box is unacceptable, the caller is
*		responsible for deleting it.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_GEOM_CHANGED  if the size or position was modified, or
*		PL_VETOED  if the callback function vetoed the create, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 1 but object edit mode is zero, then bSelect is
*		ignored.
* 2.	If bNoCallback is 0, then the object callback function (if it exists)
*		is called with PL_OBJ_EV_EDIT indicating "create complete".  (The
*		function is not called for "create start".)
* 3.	If bHide is 1, the new PL_BOX won't appear on the screen.  This
*		can be used if the caller wants to set the box's properties to
*		something other than the default box properties prior to having
*		the PL_BOX become visible.
* 4.	This routine can be used regardless of whether bEditMode is 1.
*
* See Also:
*		PlObjBoxCreateGeneral, PlObjBoxCreateSimple
*		PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-22-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxCreateAtPlotCursor(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 if object callback fn is not to be called
PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
int		bSelect,		// I 1 if the box is to be selected
int		bHide,			// I 1 if box is not to be shown, else 0
int		bPreferClip,	// I 1 if position is more important than size
double	dValL,			// I data val., left side, relative to plot cursor
double	dValB,			// I data val., bottom side, relative to plot cursor
double	dValR,			// I data val., right side, relative to plot cursor
double	dValT,			// I data val., top side, relative to plot cursor
COLORREF rgbBox)		// I color to use for drawing PL_BOX
{
	int		retStat=PL_OK, inpErr=0;
	int		stat;
	int		iRowSave, iColSave;
	double	dXValB, dXValT, dYValB, dYValT;
	double	dL, dB, dR, dT, dOffset=0.;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		the plot cursor must exist
* 3		dValL must not be equal to dValR
* 4		dValB must not be equal to dValT
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	iRowSave = pPlot->iAxisRow;
	iColSave = pPlot->iAxisCol;
	PL_INP_CHK(2, !pPlot->bEnbCursor, done)

	PL_INP_CHK(3, dValL == dValR, done)
	PL_INP_CHK(4, dValB == dValT, done)

	retStat = PlAreaArraySelect(pPlot, pPlot->iCursorRow, pPlot->iCursorCol);
	if (retStat != PL_OK) goto done;

	dXValB = pPlot->pXAxis->dDValB;
	dXValT = pPlot->pXAxis->dDValT;
	dYValB = pPlot->pYAxis->dDValB;
	dYValT = pPlot->pYAxis->dDValT;

	dL = pPlot->dCursorX + dValL;
	dR = pPlot->dCursorX + dValR;
	dB = pPlot->dCursorY + dValB;
	dT = pPlot->dCursorY + dValT;
	if (!bPreferClip) {
		if (dXValB < dXValT) {
			if (dL < dXValB) {			dOffset = dXValB - dL;
										dL = dXValB;
										dR += dOffset;			}
			else if (dR > dXValT) {		dOffset = dR - dXValT;
										dR = dXValT;
										dL -= dOffset;			}
		}
		else {
			if (dL > dXValB) {			dOffset = dXValB - dL;
										dL = dXValB;
										dR += dOffset;			}
			else if (dR < dXValT) {	dOffset = dR - dXValT;
										dR = dXValT;
										dL -= dOffset;			}
		}
		if (dYValB < dYValT) {
			if (dB < dYValB) {			dOffset = dYValB - dB;
										dB = dYValB;
										dT += dOffset;			}
			else if (dT > dYValT) {	dOffset = dT - dYValT;
										dT = dYValT;
										dB -= dOffset;			}
		}
		else {
			if (dB > dYValB) {			dOffset = dYValB - dB;
										dB = dYValB;
										dT += dOffset;			}
			else if (dT < dYValT) {	dOffset = dT - dYValT;
										dT = dYValT;
										dB -= dOffset;			}
		}
	}
	// We're here for:
	// o	the "bPreferClip" case; and
	// o	after we've adjusted position as much as possible--and clip
	//		may be needed to finish the job
	if (dXValB < dXValT) {
		dL = PL_MAX(dL, dXValB);
		dR = PL_MIN(dR, dXValT);
	}
	else {
		dL = PL_MIN(dL, dXValB);
		dR = PL_MAX(dR, dXValT);
	}
	if (dYValB < dYValT) {
		dB = PL_MAX(dB, dYValB);
		dT = PL_MIN(dT, dYValT);
	}
	else {
		dB = PL_MIN(dB, dYValB);
		dT = PL_MAX(dT, dYValT);
	}

	retStat = PlObjBoxCreateSimple(pPlot, bNoCallback,
						ppBox, bSelect, bHide, dL, dB, dR, dT, rgbBox);
	if (retStat != PL_OK) goto done;
	if (dOffset != 0.)
		retStat = PL_GEOM_CHANGED;

done:
	PL_IF_INP_ERR("PlObjBoxCreateAtPlotCursor")
	if (pPlot != NULL)
		stat = PlAreaArraySelect(pPlot, iRowSave, iColSave);
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxCreateGeneral - create a PL_BOX
*
* Purpose:
*		Creates a new PL_BOX that may span plot areas.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the create, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 1 but object edit mode is zero, then bSelect is
*		ignored.
* 2.	If bNoCallback is 0, then the object callback function (if it exists)
*		is called with PL_OBJ_EV_EDIT indicating "create complete".  (The
*		function is not called for "create start".)
* 3.	If bHide is 1, the new PL_BOX won't appear on the screen.  This
*		can be used if the caller wants to set the box's properties to
*		something other than the default box properties prior to having
*		the PL_BOX become visible.
* 4.	This routine can be used regardless of whether bEditMode is 1.
*
* See Also:
*		PlObjBoxCreateAtPlotCursor, PlObjBoxCreateSimple
*		PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxCreateGeneral(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 if object callback fn is not to be called
PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
int		bSelect,		// I 1 if the box is to be selected
int		bHide,			// I 1 if box is not to be shown, else 0
double	dValL,			// I data value for left side of PL_BOX
int		iColL,			// I plot area column containing left side
double	dValB,			// I data value for bottom side of PL_BOX
int		iRowB,			// I plot area row containing bottom side
double	dValR,			// I data value for right side of PL_BOX
int		iColR,			// I plot area column containing right side
double	dValT,			// I data value for top side of PL_BOX
int		iRowT,			// I plot area row containing top side
COLORREF rgbBox)		// I color to use for drawing PL_BOX
{
	int		retStat=PL_OK, inpErr=0;
	PL_AXIS	*paXAxis, *paYAxis;
	PL_OBJ_CTX *pObjCtx;
	PL_BOX	*pBox;
	int		iEl0, iEl, col, row, iSide;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*		for each plot area column:
* 2			all plot areas in the column must have the same X range
*		for each plot area row:
* 3			all plot areas in the row must have the same Y range
* 4		iColL must be within the range of columns for the plot
* 5		dValL must be a valid X value for iColL plot areas
* 6		iRowB must be within the range of rows for the plot
* 7		dValB must be a valid Y value for iRowB plot areas
* 8		iColR must be within the range of columns for the plot
* 9		dValR must be a valid X value for iColR plot areas
* 10	iRowT must be within the range of rows for the plot
* 11	dValT must be a valid Y value for iRowT plot areas
* 12	dValL must not be equal to dValR unless they are in different
*				plot area columns
* 13	dValB must not be equal to dValT unless they are in different
*				plot area rows
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	paXAxis = pPlot->paXAxis;
	paYAxis = pPlot->paYAxis;
	pObjCtx = pPlot->pObjCtx;

	// Do the plot area checks.  NOTE WELL!! No test for paXAxis == NULL is
	// needed here since the check starts with row and column 1 and it's
	// guaranteed that paXAxis and paYAxis are both non-NULL if there are
	// more than 1 row and/or column.
	if (pPlot->nAxisRows > 1 || pPlot->nAxisCols > 1) {
		iEl0 = 0 + 0 * pPlot->nAxisRows;
		for (col=1; col<pPlot->nAxisCols; col++) {
			iEl = 0 + col * pPlot->nAxisRows;
			PL_INP_CHK(2, paXAxis[iEl0].dDValB != paXAxis[iEl].dDValB, done)
			PL_INP_CHK(2, paXAxis[iEl0].dDValT != paXAxis[iEl].dDValT, done)
		}
		for (row=1; row<pPlot->nAxisRows; row++) {
			iEl = row + 0 * pPlot->nAxisRows;
			PL_INP_CHK(3, paYAxis[iEl0].dDValB != paYAxis[iEl].dDValB, done)
			PL_INP_CHK(3, paYAxis[iEl0].dDValT != paYAxis[iEl].dDValT, done)
		}
	
		PL_INP_CHK(4, iColL < 0 || iColL >= pPlot->nAxisCols, done)
		if (iColL > 0) {
			iEl = 0 + iColL * pPlot->nAxisRows;
			PL_INP_CHK(5, !PlObjValIsInRange(&pPlot->paXAxis[iEl], dValL), done)
			PL_INP_CHK(6, iRowB < 0 || iRowB >= pPlot->nAxisRows, done)
			iEl = iRowB + 0 * pPlot->nAxisRows;
			PL_INP_CHK(7, !PlObjValIsInRange(&pPlot->paYAxis[iEl], dValB), done)
			PL_INP_CHK(8, iColR < 0 || iColR >= pPlot->nAxisCols, done)
			iEl = 0 + iColR * pPlot->nAxisRows;
			PL_INP_CHK(9, !PlObjValIsInRange(&pPlot->paXAxis[iEl], dValR), done)
			PL_INP_CHK(10, iRowT < 0 || iRowT >= pPlot->nAxisRows, done)
			iEl = iRowT + 0 * pPlot->nAxisRows;
			PL_INP_CHK(11, !PlObjValIsInRange(&pPlot->paYAxis[iEl], dValT), done)
		}
	}
	if (iColL == iColR) {
		PL_INP_CHK(12, dValL == dValR, done)
	}
	if (iRowB == iRowT) {
		PL_INP_CHK(13, dValB == dValT, done)
	}

	if (!pObjCtx->mode.bEditMode)
		bSelect = 0;

	if (bSelect) {
		// Deselect existing selection, if any.
		retStat = PlObjSelect(pPlot, bNoCallback, NULL, 0);
		if (retStat != PL_OK) goto done;
	}
	if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	PlObjBoxInit(pPlot, pBox, bHide);
	pBox->props.rgbBox = rgbBox;

	for (iSide=0; iSide<4; iSide++)
		pBox->aSides[iSide].bValOK = 1;
	pBox->aSides[0].dVal = dValL;
	pBox->aSides[0].iRowCol = iColL;
	pBox->aSides[1].dVal = dValB;
	pBox->aSides[1].iRowCol = iRowB;
	pBox->aSides[2].dVal = dValR;
	pBox->aSides[2].iRowCol = iColR;
	pBox->aSides[3].dVal = dValT;
	pBox->aSides[3].iRowCol = iRowT;

	if (!bNoCallback && pPlot->pObjectFn != NULL) {
		int		stat;
		// Tell callback that create is about to complete.
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, (PL_OBJ *)pBox,
						PL_OBJ_EV_EDIT, PL_OBJ_EV_CREATE, PL_OBJ_EV_DONE);
		if (stat == 0) {
			// Callback want's to veto.
			free(pBox);
			goto done;
		}
	}

	DLIST_APPEND((PL_OBJ *)pBox, pObjCtx->pHead, pObjCtx->pTail);
	pObjCtx->nObjects++;
	pObjCtx->nBoxes++;

	if (bSelect)
		pObjCtx->pSelObj = (PL_OBJ *)pBox;

	retStat = PlObjBoxPaint(pPlot, pBox, bSelect);

	if (ppBox != NULL) *ppBox = pBox;

done:
	PL_IF_INP_ERR("PlObjBoxCreateGeneral")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxCreateSimple - create a PL_BOX in a plot area
*
* Purpose:
*		Creates a new PL_BOX in the currently selected plot area.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the create, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 1 but object edit mode is zero, then bSelect is
*		ignored.
* 2.	If bNoCallback is 0, then the object callback function (if it exists)
*		is called with PL_OBJ_EV_EDIT indicating "create complete".  (The
*		function is not called for "create start".)
* 3.	If bHide is 1, the new PL_BOX won't appear on the screen.  This
*		can be used if the caller wants to set the box's properties to
*		something other than the default box properties prior to having
*		the PL_BOX become visible.
* 4.	This routine can be used regardless of whether bEditMode is 1.
*
* See Also:
*		PlObjBoxCreateAtPlotCursor, PlObjBoxCreateGeneral
*		PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxCreateSimple(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 if object callback fn is not to be called
PL_BOX	**ppBox,		// O place to store pointer to new PL_BOX, or NULL
int		bSelect,		// I 1 if the box is to be selected
int		bHide,			// I 1 if box is not to be shown, else 0
double	dValL,			// I data value for left side of PL_BOX
double	dValB,			// I data value for bottom side of PL_BOX
double	dValR,			// I data value for right side of PL_BOX
double	dValT,			// I data value for top side of PL_BOX
COLORREF rgbBox)		// I color to use for drawing PL_BOX
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_BOX	*pBox;
	int		iSide;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		dValL must be within the range of X values for the plot area
* 3		dValB must be within the range of Y values for the plot area
* 4		dValR must be within the range of X values for the plot area
* 5		dValT must be within the range of Y values for the plot area
* 6		dValL must not be equal to dValR
* 7		dValB must not be equal to dValT
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	PL_INP_CHK(2, !PlObjValIsInRange(pPlot->pXAxis, dValL), done)
	PL_INP_CHK(4, !PlObjValIsInRange(pPlot->pXAxis, dValR), done)
	PL_INP_CHK(6, dValL == dValR, done)
	PL_INP_CHK(3, !PlObjValIsInRange(pPlot->pYAxis, dValB), done)
	PL_INP_CHK(5, !PlObjValIsInRange(pPlot->pYAxis, dValT), done)
	PL_INP_CHK(7, dValB == dValT, done)

	if (!pObjCtx->mode.bEditMode)
		bSelect = 0;

	if (bSelect) {
		// Deselect existing selection, if any.
		retStat = PlObjSelect(pPlot, bNoCallback, NULL, 0);
		if (retStat != PL_OK) goto done;
	}
	if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}

	PlObjBoxInit(pPlot, pBox, bHide);
	pBox->props.rgbBox = rgbBox;

	for (iSide=0; iSide<4; iSide++)
		pBox->aSides[iSide].bValOK = 1;
	pBox->aSides[0].dVal = dValL;
	pBox->aSides[0].iRowCol = pPlot->iAxisCol;
	pBox->aSides[1].dVal = dValB;
	pBox->aSides[1].iRowCol = pPlot->iAxisRow;
	pBox->aSides[2].dVal = dValR;
	pBox->aSides[2].iRowCol = pPlot->iAxisCol;
	pBox->aSides[3].dVal = dValT;
	pBox->aSides[3].iRowCol = pPlot->iAxisRow;

	if (!bNoCallback && pPlot->pObjectFn != NULL) {
		int		stat;
		// Tell callback that create is about to complete.
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, (PL_OBJ *)pBox,
						PL_OBJ_EV_EDIT, PL_OBJ_EV_CREATE, PL_OBJ_EV_DONE);
		if (stat == 0) {
			// Callback want's to veto.
			free(pBox);
			goto done;
		}
	}

	DLIST_APPEND((PL_OBJ *)pBox, pObjCtx->pHead, pObjCtx->pTail);
	pObjCtx->nObjects++;
	pObjCtx->nBoxes++;

	if (bSelect)
		pObjCtx->pSelObj = (PL_OBJ *)pBox;

	retStat = PlObjBoxPaint(pPlot, pBox, bSelect);

	if (ppBox != NULL) *ppBox = pBox;

done:
	PL_IF_INP_ERR("PlObjBoxCreateSimple")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxCvtCoordToBox - convert PL_COORD pair to PL_BOX
*
* Purpose:
*		Converts a pair of PL_COORDs to a PL_BOX.
*
*		This routine:
*		o	stores, without validating, the bValOK, dVal, and iRowCol
*			items
*		o	transposes left/right sides during the store, if necessary
*		o	transposes top/bottom sides during the store, if necessary
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine assumes that the conversion to dVal has already
*		been done in the PL_COORD pairs.  (This may mean that one or both
*		bValOK flags is zero.)
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxCvtCoordToBox(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pcoLB,		// I the left bottom coordinate
PL_COORD *pcoRT,		// I the right top coordinate
PL_BOX	*pBox)			// IO the resulting PL_BOX
{
	PL_COORD *pcoMin, *pcoMax;
	int		bSwapX=0, bSwapY=0;

	if (pcoLB->x.iRowCol >= 0 && pcoRT->x.iRowCol >= 0 &&
								pcoLB->x.iVisPx > pcoRT->x.iVisPx) {
		bSwapX = 1;
	}
	if (pcoLB->y.iRowCol >= 0 && pcoRT->y.iRowCol >= 0 &&
								pcoLB->y.iVisPx < pcoRT->y.iVisPx) {
		bSwapY = 1;
	}

	if (bSwapX)
		pcoMin = pcoRT, pcoMax = pcoLB;
	else
		pcoMin = pcoLB, pcoMax = pcoRT;
	pBox->aSides[0].bValOK = pcoMin->x.bValOK;
	pBox->aSides[0].dVal = pcoMin->x.dVal;
	pBox->aSides[0].iRowCol = pcoMin->x.iRowCol;
	pBox->aSides[2].bValOK = pcoMax->x.bValOK;
	pBox->aSides[2].dVal = pcoMax->x.dVal;
	pBox->aSides[2].iRowCol = pcoMax->x.iRowCol;

	if (bSwapY)
		pcoMin = pcoRT, pcoMax = pcoLB;
	else
		pcoMin = pcoLB, pcoMax = pcoRT;
	pBox->aSides[1].bValOK = pcoMin->y.bValOK;
	pBox->aSides[1].dVal = pcoMin->y.dVal;
	pBox->aSides[1].iRowCol = pcoMin->y.iRowCol;
	pBox->aSides[3].bValOK = pcoMax->y.bValOK;
	pBox->aSides[3].dVal = pcoMax->y.dVal;
	pBox->aSides[3].iRowCol = pcoMax->y.iRowCol;

}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxCvtBoxToCoord - convert PL_BOX to pair of coordinates
*
* Purpose:
*		Converts a PL_BOX to the corresponding pair of PL_COORDs, which
*		contain:
*		o	the 'virtual' pixel coordinates
*		o	the 'visible' pixel coordinates
*		o	the coordinates to use when drawing the box
*		o	the coordinates of a clip region to use when drawing
*		o	flags indicating whether the dVal's are in range
*		o	flags indicating whether the edges are visible
*
*		The 'bSelected' argument is used in conjunction with the .bShowFullHt
*		property of the box, so that this routine can take into account that
*		.bShowFullHt PL_BOXs have a special visual behavior.
*
*		This routine ignores the .bHidden property.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-19-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxCvtBoxToCoord(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// I the PL_BOX to convert
int		bSelected,		// I 1 if the box is selected
PL_COORD *pcoLB,		// O the resulting left bottom coordinate
PL_COORD *pcoRT)		// O the resulting right top coordinate
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		bShowFullHt=0;

	memset(pcoLB, 0, sizeof(PL_COORD));
	memset(pcoRT, 0, sizeof(PL_COORD));

	// left side
	pcoLB->x.dVal = pBox->aSides[0].dVal;
	pcoLB->x.iRowCol = pBox->aSides[0].iRowCol;

	// right side
	pcoRT->x.dVal = pBox->aSides[2].dVal;
	pcoRT->x.iRowCol = pBox->aSides[2].iRowCol;

	// bottom side
	pcoLB->y.dVal = pBox->aSides[1].dVal;
	pcoLB->y.iRowCol = pBox->aSides[1].iRowCol;

	// top side
	pcoRT->y.dVal = pBox->aSides[3].dVal;
	pcoRT->y.iRowCol = pBox->aSides[3].iRowCol;

	if (pBox->props.bShowFullHt && !bSelected)
		bShowFullHt = 1;
	PlObjBoxCoordCalcPx(pPlot, bShowFullHt, pcoLB, pcoRT);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxHitTest - test for hit with a PL_BOX
*
* Purpose:
*		Tests to see if a coordinate is on or within a PL_BOX.  If it is,
*		o	the type of hit is deduced as one of the PL_OBJ_xxx hit types
*		o	a handle to the appropriate edit mouse pointer is stored in
*			pMouse->hPointerHit
*
*		This routine checks only pixel-oriented issues.  It totally ignores
*		issues such as 'edit enable', another object is already selected, etc.
*
* Return Value:
*		PL_OBJ_xxx if there is a hit, or
*		-1 otherwise
*
* Notes:
* 1.	PL_OBJ_C is returned only if the coordinate is inside a selected
*		PL_BOX.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_BOX	*pBox,			// I pointer to PL_BOX to test
PL_COORD *pcoMouse)		// I coordinate to test
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		iHit=-1;
	int		bXL, bXR, bYT, bYB, bXBet, bYBet;
	int		bSelected=(PL_OBJ *)pBox == pObjCtx->pSelObj;
	PL_COORD coLB, coRT;
	int		iMouseX=pcoMouse->x.iVisPx;
	int		iMouseY=pcoMouse->y.iVisPx;

	if (pBox->props.bNoEdit) goto done;

	bXL = bXR = bXBet = bYT = bYB = bYBet = 0;

	// Get the full coordinates for the PL_BOX.
	PlObjBoxCvtBoxToCoord(pPlot, pBox, bSelected, &coLB, &coRT);

	// Check to see if the X mouse coordinate hits the left or right edge
	// of the box or is somewhere between the left and right edges.
	// An 'edge' hit occurs if:
	//	o	the edge is visible; and
	//	o	the X mouse is within PL_OBJ_HIT_PX of the visible edge
	// A 'between' hit occurs if:
	//	o	there is no 'edge' hit; and
	//	o	both edges have OK dVal; and
	//	o	one of the following cases is TRUE
	//		case 1:		both edges are visible; and
	//					the X mouse is between the edges.
	//		case 2:		the left edge is visible; and
	//					the right edge is clipped; and
	//					X mouse is between the left edge and the right clip.
	//		case 3:		the left edge is clipped; and
	//					the right edge is visible; and
	//					X mouse is between the left clip and the right edge.
	//		case 4:		both edges are clipped; and
	//					X mouse is between the left clip and the right clip.
	if (coLB.x.bEdgeVis) {
		if (fabs((double)(iMouseX - coLB.x.iVisPx)) <= PL_OBJ_HIT_PX) bXL = 1;
	}                                                           
	if (coRT.x.bEdgeVis) {
		if (fabs((double)(iMouseX - coRT.x.iVisPx)) <= PL_OBJ_HIT_PX) bXR = 1;
	}
	if (!bXL && !bXR && coLB.x.bValOK && coRT.x.bValOK) {
		if (coLB.x.bEdgeVis && coRT.x.bEdgeVis) {
			if (iMouseX > coLB.x.iVisPx &&
								iMouseX < coRT.x.iVisPx)		bXBet = 1;
		}
		else if (coLB.x.bEdgeVis) {
			if (iMouseX > coLB.x.iVisPx &&
								iMouseX < coRT.x.iClipPx)		bXBet = 1;
		}
		else if (coRT.x.bEdgeVis) {
			if (iMouseX > coLB.x.iClipPx &&
								iMouseX < coRT.x.iVisPx)		bXBet = 1;
		}
		else {
			if (iMouseX > coLB.x.iClipPx &&
									iMouseX < coRT.x.iClipPx)	bXBet = 1;
		}
	}
	// Check to see if the Y mouse coordinate hits the top or bottom edge
	// of the box or is somewhere between the top and bottom edges.  The
	// logic parallels that for the X mouse coordinate.
	if (coLB.y.bEdgeVis) {
		if (fabs((double)(iMouseY - coLB.y.iVisPx)) <= PL_OBJ_HIT_PX) bYB = 1;
	}
	if (coRT.y.bEdgeVis) {
		if (fabs((double)(iMouseY - coRT.y.iVisPx)) <= PL_OBJ_HIT_PX) bYT = 1;
	}
	if (!bYB && !bYT && coLB.y.bValOK && coRT.y.bValOK) {
		if (coLB.y.bEdgeVis && coRT.y.bEdgeVis) {
			if (iMouseY > coRT.y.iVisPx &&
								iMouseY < coLB.y.iVisPx)		bYBet = 1;
		}
		else if (coRT.y.bEdgeVis) {
			if (iMouseY > coRT.y.iVisPx &&
								iMouseY < coLB.y.iClipPx)		bYBet = 1;
		}
		else if (coLB.y.bEdgeVis) {
			if (iMouseY > coRT.y.iClipPx &&
								iMouseY < coLB.y.iVisPx)		bYBet = 1;
		}
		else {
			if (iMouseY > coRT.y.iClipPx &&
								iMouseY < coLB.y.iClipPx)		bYBet = 1;
		}
	}
	
	if (bXBet && bYBet && pObjCtx->pSelObj == (PL_OBJ *)pBox)
								iHit = PL_OBJ_C;
	else if (bXL && bXR) {
		if (bYT)				iHit = PL_OBJ_TR;
		else if (bYBet)			iHit = PL_OBJ_C;
		else if (bYB)			iHit = PL_OBJ_BL;
	}
	else if (bYB && bYT) {
		if (bXR)				iHit = PL_OBJ_TR;
		else if (bXBet)			iHit = PL_OBJ_C;
		else if (bXL)			iHit = PL_OBJ_BL;
	}
	else if (bXL) {
		if (bYT)				iHit = PL_OBJ_TL;
		else if (bYB)			iHit = PL_OBJ_BL;
		else if (bYBet)			iHit = PL_OBJ_L;
	}
	else if (bXR) {
		if (bYT)				iHit = PL_OBJ_TR;
		else if (bYB)			iHit = PL_OBJ_BR;
		else if (bYBet)			iHit = PL_OBJ_R;
	}
	else if (bXBet) {
		if (bYT)				iHit = PL_OBJ_T;
		else if (bYB)			iHit = PL_OBJ_B;
	}
	if (iHit == PL_OBJ_C) {
		if (pBox->props.bNoMove && !pObjCtx->mode.bEnbMove)
			pMouse->bUseNOTPointer = 1;
	}
	else if (iHit >= 0 && pBox->props.bNoResize && !pObjCtx->mode.bEnbResize)
		pMouse->bUseNOTPointer = 1;

#define CHOOSE_HIT_POINTER(idc_cur) pMouse->hPointerHit = LoadCursor(NULL, idc_cur)

	switch (iHit) {
		case PL_OBJ_C:		CHOOSE_HIT_POINTER(IDC_SIZE); break;
		case PL_OBJ_TL:
		case PL_OBJ_BR:		CHOOSE_HIT_POINTER(IDC_SIZENWSE); break;
		case PL_OBJ_TR:
		case PL_OBJ_BL:		CHOOSE_HIT_POINTER(IDC_SIZENESW); break;
		case PL_OBJ_T:
		case PL_OBJ_B:		CHOOSE_HIT_POINTER(IDC_SIZENS); break;
		case PL_OBJ_R:
		case PL_OBJ_L:		CHOOSE_HIT_POINTER(IDC_SIZEWE); break;
	}
#undef CHOOSE_HIT_POINTER

done:
//	if (iHit == PL_OBJ_C) pMouse->hPointerHit = pMouse->hPointerMove;
//	else if (iHit >= 0) pMouse->hPointerHit = pMouse->hPointerSize;
	return iHit;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxInit - initialize a PL_BOX
*
* Purpose:
*		Initializes a PL_BOX:
*		o	sets the properties to default values
*		o	sets the sides to "invalid"
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// IO pointer to PL_BOX
int		bHide)			// I 1 if box is not to be shown, else 0
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	memset(pBox, 0, sizeof(PL_BOX));
	pBox->eObjType = PL_OBJ_TY_BOX;
	pBox->props = pObjCtx->boxDflt;
	pBox->props.bHide = bHide;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxKeyboardMove - move the sides of a PL_BOX
*
* Purpose:
*		Moves the sides of a PL_BOX, and synchronizes the mouse pointer
*		to "keep up" with whatever motion occurred.
*
*		If the move is invalid, a MessageBeep is done and the move is
*		aborted.
*
* Return Value:
*		character indicating where mouse pointer should be positioned
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxKeyboardMove(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// IO the PL_BOX to move
int		bDeltaPx,		// I 1 if dDeltaX and dDeltaY are pixels
double	dDeltaX,		// I amount to add to X
double	dDeltaY,		// I amount to add to Y
int		iDeltaRow,		// I amount to add to row number
int		iDeltaCol)		// I amount to add to column number
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_COORD coLBOld, coRTOld;
	PL_COORD coLB, coRT;
	int		bMoveL=pObjCtx->bMoveLeft, bMoveR=pObjCtx->bMoveRight;
	int		bMoveB=pObjCtx->bMoveBottom, bMoveT=pObjCtx->bMoveTop;
	int		iPointerPos;

/*---------------------------------------------------------------------------
*	Adjust the sides of the PL_BOX as appropriate for the type of edit
*	operation.
*--------------------------------------------------------------------------*/

	// Get the "virtual" pixel values corresponding to the sides.
	PlObjBoxCvtBoxToCoord(pPlot, pBox, 1, &coLBOld, &coRTOld);
	coLB = coLBOld;
	coRT = coRTOld;

	if (bMoveL) {
		if (iDeltaCol != 0)
			coLB.x.iRowCol += iDeltaCol;
		else
			PlObjCoordAddDeltaX(pPlot, &coLB, 0, bDeltaPx, dDeltaX);
	}
	if (bMoveB) {
		if (iDeltaRow != 0)
			coLB.y.iRowCol += iDeltaRow;
		else
			PlObjCoordAddDeltaY(pPlot, &coLB, 0, bDeltaPx, dDeltaY);
	}
	if (bMoveR) {
		if (iDeltaCol != 0)
			coRT.x.iRowCol += iDeltaCol;
		else
			PlObjCoordAddDeltaX(pPlot, &coRT, 0, bDeltaPx, dDeltaX);
	}
	if (bMoveT) {
		if (iDeltaRow != 0)
			coRT.y.iRowCol += iDeltaRow;
		else
			PlObjCoordAddDeltaY(pPlot, &coRT, 0, bDeltaPx, dDeltaY);
	}

	// Calculate new pixel-related values, including the valid and visible
	// flags.
	PlObjBoxCoordCalcPx(pPlot, 0, &coLB, &coRT);

	// Do some error checking on the new coordinate.
	if (!pObjCtx->mode.bEnbBoxSpan) {
		if (coLB.y.iRowCol != coRT.y.iRowCol) goto coord_invalid;
		if (coLB.x.iRowCol != coRT.x.iRowCol) goto coord_invalid;
	}
	if (!coLB.x.bValOK || !coLB.y.bValOK) goto coord_invalid;
	if (!coRT.x.bValOK || !coRT.y.bValOK) goto coord_invalid;

	pObjCtx->bKbEditActive = 1;
	// Erase the PL_BOX.
	PlObjBoxPaint(pPlot, pBox, 1);

	// Set its new coordinates.
	PlObjBoxCvtCoordToBox(pPlot, &coLB, &coRT, pBox);

	// Draw the object.
	PlObjBoxPaint(pPlot, pBox, 1);

	if (bMoveL && bMoveB && bMoveR && bMoveT)
		iPointerPos = 'C';
	else if (iDeltaRow != 0 && coLB.y.iRowCol != coRT.y.iRowCol) {
		if (bMoveB) {
			if (coLB.y.iRowCol < coRT.y.iRowCol)		iPointerPos = 'T';
			else										iPointerPos = 'B';
		}
		else {
			if (coRT.y.iRowCol > coLB.y.iRowCol)		iPointerPos = 'B';
			else										iPointerPos = 'T';
		}
	}
	else if (iDeltaCol != 0 && coLB.x.iRowCol != coRT.x.iRowCol) {
		if (bMoveL) {
			if (coLB.x.iRowCol > coRT.x.iRowCol)		iPointerPos = 'T';
			else										iPointerPos = 'B';
		}
		else {
			if (coRT.x.iRowCol < coLB.x.iRowCol)		iPointerPos = 'B';
			else										iPointerPos = 'T';
		}
	}
	else if (bMoveL && coLB.x.dVirtPx > coRT.x.dVirtPx)
														iPointerPos = 'T';
	else if (bMoveR && coRT.x.dVirtPx < coLB.x.dVirtPx)
														iPointerPos = 'B';
	else if (bMoveB && coLB.y.dVirtPx < coRT.y.dVirtPx)
														iPointerPos = 'T';
	else if (coRT.y.dVirtPx > coLB.y.dVirtPx)
														iPointerPos = 'B';
	else if (bMoveL)									iPointerPos = 'B';
	else if (bMoveR)									iPointerPos = 'T';
	else if (bMoveB)									iPointerPos = 'B';
	else												iPointerPos = 'T';

done:
	return iPointerPos;
coord_invalid:
	MessageBeep(MB_ICONEXCLAMATION);
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxKeyboardPlaceMouse - place the mouse pointer on a PL_BOX
*
* Purpose:
*		Places the mouse pointer on a PL_BOX.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_BOX	*pBox,			// I the object to place mouse on
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac)	// O anchor fraction if scroll change needed
{
	PL_COORD coLB, coRT;

	PlObjBoxCvtBoxToCoord(pPlot, pBox, 1, &coLB, &coRT);
	if (wParam == 'B' || wParam == 'T') {
		if (wParam == 'B') {
			*pcoMouse = coLB;
			*pfAnchFrac = .5F;
		}
		else {
			*pcoMouse = coRT;
			*pfAnchFrac = .5F;
		}
	}
	else if (wParam == 'C') {
		pcoMouse->x.bValOK = coLB.x.bValOK && coRT.x.bValOK;
		pcoMouse->x.iRowCol = (coLB.x.iRowCol + coRT.x.iRowCol) / 2;
		pcoMouse->x.dVirtPx = (coLB.x.dVirtPx + coRT.x.dVirtPx) / 2.;
		pcoMouse->x.dVal = (coLB.x.dVal + coRT.x.dVal) / 2.;
		pcoMouse->y.bValOK = coLB.y.bValOK && coRT.y.bValOK;
		pcoMouse->y.iRowCol = (coLB.y.iRowCol + coRT.y.iRowCol) / 2;
		pcoMouse->y.dVirtPx = (coLB.y.dVirtPx + coRT.y.dVirtPx) / 2.;
		pcoMouse->y.dVal = (coLB.y.dVal + coRT.y.dVal) / 2.;
		*pfAnchFrac = .5F;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxPaint - paint a PL_BOX
*
* Purpose:
*		Paints a PL_BOX.
*
*		If PL_BOX.bHide is set, this routine does nothing.
*
*		It's worth a little explanation of all the fuss made over drawing
*		boxes.  For a box that doesn't span plot areas, there is no
*		complication whatsoever, regardless of the zoom and scroll--just
*		clip at the edges of the data area that contains the box.
*
*		If a box spans plot areas, however, weird stuff can happen with
*		zoom and scroll.  Consider an array of 2 plot areas, side-by-side,
*		with data values that range from 0 to 8.  And suppose that there
*		is a box from 0::5 to 1::1.  L represents the left side of the
*		box and R the right.  When there's no zoom, we have the situation
*		below.  Note that we will _always_ require that the top and bottom
*		sides be a visible between the axes, regardless of zoom and/or scroll.
*
*		                       ---data area 0---     ---data area 1---
*		                       0 1 2 3 4 5 6 7 8     0 1 2 3 4 5 6 7 8
*		                                 L-------------R
*		                                 |             |
*		                                 L-------------R
*
*		Now let's zoom by a factor of 2 and put the 'thumb' toward the
*		left.  Both right and left sides are visible and there's no
*		complication.  (Since this case isn't complicated, I haven't
*		shown the "virtual" axis for either data area.)
*
*		                       ---data area 0---     ---data area 1---
*		                       1   2   3   4   5     1   2   3   4   5
*		                                   L---------R
*		                                   |         |
*		                                   L---------R
*
*		If we move the thumb to the right, suddenly things get weird.
*		Using the value-to-pixel mapping for the plot, the right side
*		of the box suddenly appears in the middle of the left-hand axis
*		in the place where the "virtual" value of 1 is for the right-
*		axis.  In this case, we don't want to see the right-hand side
*		at all (but we still want the top and bottom sides to extend
*		from the left side all the way to the left edge of data area 1).
*
*		                             -----"virtual" data area 1-------
*		                             0   1   2   3   4   5   6   7   8
*		                       ---data area 0---     ---data area 1---
*		                       4   5   6   7   8     4   5   6   7   8
*		                       L---------R-----------
*		                       |         |
*		                       L---------R-----------
*
*		There may be a really elegant solution to this and the other
*		cases, but I haven't found it.  Instead, I've taken a brute
*		force approach which depends on:
*		o	the off-the-wall members in the PL_COORD structure (iVisPx,
*			iDrawPx, iClipPx, dVirtPx);
*		o	the setup done by PlObjBoxCoordCalcPx; and
*		o	the use of clip regions in this routine.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_BOX	*pBox,			// I pointer to PL_BOX to draw
int		bSelected)		// I 1 if the box is selected
{
	int		retStat=PL_OK;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	HDC		hDC;
	HBRUSH	hBrush=0, hBrushOld=0;
	HRGN	rgn=0;
	PL_COORD coLB, coRT;
	int		iXDrawPxL, iXDrawPxR, iYDrawPxT, iYDrawPxB;
	int		stat;

	if (pBox->props.bHide) goto done;

	// Get the "virtual" pixel values corresponding to the sides.
	PlObjBoxCvtBoxToCoord(pPlot, pBox, bSelected, &coLB, &coRT);

	// See if anything is visible.
	if (coLB.x.iClipPx >= coRT.x.iClipPx) goto done;
	if (coRT.y.iClipPx >= coLB.y.iClipPx) goto done;

	hDC = PlObjHDCHandler(pPlot, (PL_OBJ *)pBox, 0);
	if (hDC != 0) {
		rgn = CreateRectRgn(coLB.x.iClipPx, coRT.y.iClipPx,
									coRT.x.iClipPx+1, coLB.y.iClipPx+1);
		if (rgn == 0) goto gdi_error;
		stat = SelectClipRgn(hDC, rgn);
		if (stat == ERROR) goto gdi_error;
		DeleteObject(rgn);

		if (pBox->props.bFillBox && (PL_OBJ *)pBox != pObjCtx->pSelObj) {
			if ((hBrush = CreateSolidBrush(pBox->props.rgbBox)) != 0)
				//hBrushOld = SelectObject(hDC, hBrush);
/*KG*/			hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
		}
		iXDrawPxL = coLB.x.iDrawPx;
		iXDrawPxR = coRT.x.iDrawPx;
		iYDrawPxT = coRT.y.iDrawPx;
		iYDrawPxB = coLB.y.iDrawPx;
		if (iXDrawPxL == iXDrawPxR || iYDrawPxT == iYDrawPxB) {
			MoveToEx(hDC, iXDrawPxL, iYDrawPxT, NULL);
			if (iXDrawPxL == iXDrawPxR)
				LineTo(hDC, iXDrawPxR, iYDrawPxB + 1);
			else
				LineTo(hDC, iXDrawPxR + 1, iYDrawPxB);
		}
		else {
			if (!pBox->props.bRounded) {
				Rectangle(hDC, iXDrawPxL, iYDrawPxT,
								iXDrawPxR + 1, iYDrawPxB + 1);
			}
			else {
				RoundRect(hDC, iXDrawPxL, iYDrawPxT,
								iXDrawPxR + 1, iYDrawPxB + 1, 8, 8);
			}
		}
		if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
		if (hBrush != 0) DeleteObject(hBrush);
	}
	PlObjHDCHandler(pPlot, (PL_OBJ *)pBox, 1);		// release

done:
	return retStat;
gdi_error:
	if (rgn != 0) DeleteObject(rgn);
	return PL_GDI_FAIL;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjBoxPaint_edit - paint the PL_BOX "rubber band"
*
* Purpose:
*		Paints the "rubber band" for the PL_BOX being edited.  Since the
*		drawing uses XOR, alternate calls to this routine with the same
*		edit object alternately draw and erase the edit object.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-08-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjBoxPaint_edit(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_COORD *pcoLB=&pObjCtx->coEditLBCurr;
	PL_COORD *pcoRT=&pObjCtx->coEditRTCurr;
	HDC		hDC;
	HRGN	rgn=0;
	int		stat;
	int		iXDrawPxL, iXDrawPxR, iYDrawPxT, iYDrawPxB;

	if ((hDC = PlObjHDCHandler(pPlot, NULL, 0)) == 0) goto done;
	if (!pObjCtx->bEditVisible) goto done;

	rgn = CreateRectRgn(pcoLB->x.iClipPx, pcoRT->y.iClipPx,
								pcoRT->x.iClipPx+1, pcoLB->y.iClipPx+1);
	if (rgn == 0) goto gdi_error;
	stat = SelectClipRgn(hDC, rgn);
	if (stat == ERROR) goto gdi_error;
	DeleteObject(rgn);
	
	iXDrawPxL = pcoLB->x.iDrawPx;
	iXDrawPxR = pcoRT->x.iDrawPx;
	iYDrawPxT = pcoRT->y.iDrawPx;
	iYDrawPxB = pcoLB->y.iDrawPx;
	if (iXDrawPxL == iXDrawPxR || iYDrawPxT == iYDrawPxB) {
		MoveToEx(hDC, iXDrawPxL, iYDrawPxT, NULL);
		if (iXDrawPxL == iXDrawPxR)
			LineTo(hDC, iXDrawPxR, iYDrawPxB + 1);
		else
			LineTo(hDC, iXDrawPxR + 1, iYDrawPxB);
	}
	else
		Rectangle(hDC, iXDrawPxL, iYDrawPxT, iXDrawPxR + 1, iYDrawPxB + 1);

done:
	PlObjHDCHandler(pPlot, NULL, 1);		// release
	return;
gdi_error:
	goto done;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxProps - set or gets PL_BOX properties
*
* Purpose:
*		Sets the properties for a PL_BOX or retrieves the properties from
*		a PL_BOX.
*
*		When the 'bSet' argument is 0, the current flag settings are
*		stored in the caller's PL_BOX_PROPS structure.
*
*		When the 'bSet' argument is 1,
*		o	the properties from the caller's PL_BOX_PROPS structure are
*			stored in the specified box
*		o	if one or more of the visual properties of the PL_BOX change,
*			then the box will be repainted, unless the 'bNoPaint' argument
*			is 1.
*
*		The PL_OBJ_CTX enable flags provide global control on what the
*		operator is allowed to do using the mouse and keyboard.  The
*		following table illustrates the behavior for deleting an object;
*		the other edit operations operate in a similar way.
*
*		----- PL_OBJ_CTX -----    PL_BOX_PROPS  operator delete is
*		bEditMode   bEnbDelete     bNoDelete
*		    0                                   prohibited
*		    1           1                       allowed
*		    1           0              1        prohibited
*		    1           0              0        allowed
*
* Properties:
*		The following properties affect the appearance and behavior
*		of the PL_BOX object.  Items in the list are members of
*		the PL_BOX_PROPS structure.
*		o	bNoEdit  0 prevents the operator from moving, deleting, and
*					resizing the object and from changing its properties.
*		o	bNoMove  0 prevents the operator from moving the object
*		o	bNoResize  0 prevents the operator from resizing the object
*		o	bNoDelete  0 prevents the operator from deleting the object
*		o	bHide  1 causes the box not to be drawn
*		o	bRounded  1 gives the box rounded corners, 0 for square
*		o	eBorderType  specifies the line style for drawing the
*					box, as one of PL_LINE_xxx
*		o	fBorderPts  drawing thickness for box border, in points
*		o	bFillBox  1 if box (when not selected) is shown filled
*		o	rgbBox  color for drawing (and filling) box
*		o	bShowFullHt  1 to draw box the height of the data area
*		Three members in the PL_BOX_PROPS structure can be used for
*		storing caller-specified information:
*		o	dUserVal  user value
*		o	pUserStruc  user pointer
*		o	szID  user text
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a property is to call this routine
*		to get all properties, set the value of the property of interest,
*		and then call this routine to set all properties.
* 2.	The 'bNoPaint' argument can be used when many PL_BOXs are being
*		changed.  In such cases, the tradeoff may be in favor of changing
*		all the affected PL_BOXs and then calling PlWinRepaint to repaint
*		the entire plot window.
* 3.	Valid line patterns (for pBoxProps->eBorderType) are listed below.
*		(NOTE WELL!! Windows 3.1 has a 'feature' that draws solid lines
*		any time the line thickness is greater than 1 pixel.  Because of
*		this, PlBboxXxx routines draw all patterned lines 1 pixel thick.)
*			PL_LINE				PL_BOX is drawn as solid line
*			PL_LINE_DASH		PL_BOX is drawn as dashed line
*			PL_LINE_DOT			PL_BOX is drawn as dotted line
*			PL_LINE_DASHDOT		PL_BOX is drawn as dotted line
*			PL_LINE_DASHDOTDOT	PL_BOX is drawn as dash-dot-dot line
*
* See Also:
*		PlObjBoxCreateSimple, PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-11-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxProps(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// I pointer to PL_BOX; NULL implies selected box
int		bNoPaint,		// I 1 if the box is not to be repainted
int		bSet,			// I 1 to set PL_BOX properties, 0 to get them
PL_BOX_PROPS *pBoxProps)// IO pointer to caller's PL_BOX_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_BOX_PROPS propsNew;
	int		bRepaintNeeded=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pBox is NULL, there must be a selected PL_BOX
* 3		if pBox is not NULL, it must be PL_BOX
* 4		pBoxProps must not be NULL
*		if bSet is 1 then:
* 5			pBoxProps->eBorderType must be a valid PL_LINE_xxx
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (pBox == NULL) {
		PL_INP_CHK(2, pObjCtx->pSelObj == NULL, done)
		pBox = (PL_BOX *)pObjCtx->pSelObj;
		PL_INP_CHK(2, pBox->eObjType != PL_OBJ_TY_BOX, done)
	}
	else
		PL_INP_CHK(3, pBox->eObjType != PL_OBJ_TY_BOX, done)
	PL_INP_CHK(4, pBoxProps == NULL, done)
	if (bSet) {
		if (pBoxProps->eBorderType == PL_LINE)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASH)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DOT)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASHDOT)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASHDOTDOT)
			;	// no action--just checking
		else
			PL_INP_CHK(5, 1, done)
	}

	if (bSet) {
		propsNew = *pBoxProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)		propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)		propsNew.bNoMove = 1;
		if (propsNew.bNoResize)		propsNew.bNoResize = 1;
		if (propsNew.bNoDelete)		propsNew.bNoDelete = 1;
		if (propsNew.bHide)			propsNew.bHide = 1;
		if (propsNew.bRounded)		propsNew.bRounded = 1;
		if (propsNew.bFillBox)		propsNew.bFillBox = 1;
		if (propsNew.bShowFullHt)	propsNew.bShowFullHt = 1;

		if (propsNew.bHide != pBox->props.bHide)				bRepaintNeeded = 1;
		if (propsNew.bRounded != pBox->props.bRounded)			bRepaintNeeded = 1;
		if (propsNew.fBorderPts != pBox->props.fBorderPts)		bRepaintNeeded = 1;
		if (propsNew.bFillBox != pBox->props.bFillBox)			bRepaintNeeded = 1;
		if (propsNew.rgbBox != pBox->props.rgbBox)				bRepaintNeeded = 1;
		if (propsNew.bShowFullHt != pBox->props.bShowFullHt)	bRepaintNeeded = 1;

		if (bRepaintNeeded && !bNoPaint) {
			int		bSelected=pObjCtx->pSelObj == (PL_OBJ *)pBox;
	
			if (pObjCtx->mode.bEditMode)
				PlObjBoxPaint(pPlot, pBox, bSelected);	// erase existing box
			pBox->props = propsNew;
			PlObjBoxPaint(pPlot, pBox, bSelected);		// paint new box
		}
		else
			pBox->props = propsNew;
	}
	else
		*pBoxProps = pBox->props;

done:
	PL_IF_INP_ERR("PlObjBoxProps")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxPropsDflt - set or gets default PL_BOX properties
*
* Purpose:
*		Sets or retrieves the default PL_BOX properties.
*
*		When the 'bSet' argument is 0, the current default PL_BOX settings
*		are stored in the caller's PL_BOX_PROPS structure.
*
*		When the 'bSet' argument is 1, the properties from the caller's
*		PL_BOX_PROPS structure are stored in the PL_OBJ_CTX structure as
*		defaults to be used when the operator creates a new PL_BOX on
*		the screen.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a default property is to call this
*		routine to get all default properties, set the value of the
*		default property of interest, and then call this routine to set
*		all default properties.
* 2.	See PlObjBoxProps for a list and discussion of line patterns (for
*		pBoxProps->eBorderType).
*
* See Also:
*		PlObjBoxProps, PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-13-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxPropsDflt(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bSet,			// I 1 to set PL_BOX properties, 0 to get them
PL_BOX_PROPS *pBoxProps)// IO pointer to caller's PL_BOX_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_BOX_PROPS propsNew;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pBoxProps must not be NULL
*		if bSet is 1 then:
* 3			pBoxProps->eBorderType must be a valid PL_LINE_xxx
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	PL_INP_CHK(2, pBoxProps == NULL, done)
	if (bSet) {
		if (pBoxProps->eBorderType == PL_LINE)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASH)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DOT)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASHDOT)
			;	// no action--just checking
		else if (pBoxProps->eBorderType == PL_LINE_DASHDOTDOT)
			;	// no action--just checking
		else
			PL_INP_CHK(3, 1, done)
	}

	if (bSet) {
		propsNew = *pBoxProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)		propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)		propsNew.bNoMove = 1;
		if (propsNew.bNoResize)		propsNew.bNoResize = 1;
		if (propsNew.bNoDelete)		propsNew.bNoDelete = 1;
		if (propsNew.bHide)			propsNew.bHide = 1;
		if (propsNew.bRounded)		propsNew.bRounded = 1;
		if (propsNew.bFillBox)		propsNew.bFillBox = 1;
		if (propsNew.bShowFullHt)	propsNew.bShowFullHt = 1;
		pObjCtx->boxDflt = propsNew;
	}
	else
		*pBoxProps = pObjCtx->boxDflt;

done:
	PL_IF_INP_ERR("PlObjBoxPropsDflt")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjBoxZoom - zooms using a PL_BOX
*
* Purpose:
*		Changes the zoom factors and scroll positions:
*		o	to values that move the specified PL_BOX so its edges are at
*			the edges of the plot area(s) that contains it; or
*		o	to their values prior to a previous call to PlObjBoxZoom.
*
*		If the specified PL_BOX spans several rows in a plot area array,
*		then only the horizontal zoom and scroll are altered.  If it spans
*		several columns, then only the vertical zoom and scroll are altered.
*		(If it spans several rows and also several columns, an error status
*		is returned.)
*
*		When bZoom is 1, the current zoom state is saved before the zoom
*		factors and scroll positions are altered.  The saved state can be
*		restored by calling this routine with the bZoom argument set to zero.
*
*		(If there is already a 'saved zoom state', then it is NOT over-
*		written.  The 'saved zoom state' is erased only when a call is
*		made to this routine with bZoom set to 0.)
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	If bZoom is 0 (to do a 'restore'), then pBox is ignored.
* 2.	If pBox is NULL, then the selected PL_BOX is used for zooming.
* 3.	If bZoom is 1 and the new zoom factor(s) are the same as the old,
*		this routine does nothing.  The assumption is that the operator
*		invoked 'auto zoom' twice on the same PL_BOX.
*
* See Also:
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-10-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjBoxZoom(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_BOX	*pBox,			// I pointer to PL_BOX; NULL implies selected box
int		bZoom,			// I 1 zooms using pBox, 0 restores
float	*pfXZoom,		// I place to store horizontal zoom factor
float	*pfYZoom,		// I place to store vertical zoom factor
float	*pfXScroll,		// I place to store horiz. scroll position, or NULL
float	*pfYScroll)		// I place to store vertical scroll position, or NULL
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	int		iSide;
	double	dXLeft, dXRight, dXWidth, dXFullRange;
	double	dYTop, dYBottom, dYHeight, dYFullRange;
	float	fXZoomNew, fYZoomNew;
	double	dXAnchVal, dYAnchVal;
	float	fXAnchFrac, fYAnchFrac;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*		if bZoom is 1 then:
* 2			if pBox is NULL, there must be a selected PL_BOX
* 3			if pBox is not NULL, it must be PL_BOX
* 4			all sides of the specified PL_BOX must have .bValOK == 1
* 5			the specified PL_BOX must not span both rows and columns
*		if bZoom is 0 then:
* 6			a prior PlObjBoxZoom call with bZoom == 1 must have been made
* 7		pfXZoom must not be NULL
* 8		pfYZoom must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (bZoom) {
		if (pBox == NULL) {
			PL_INP_CHK(2, pObjCtx->pSelObj == NULL, done)
			pBox = (PL_BOX *)pObjCtx->pSelObj;
			PL_INP_CHK(2, pBox->eObjType != PL_OBJ_TY_BOX, done)
		}
		else
			PL_INP_CHK(3, pBox->eObjType != PL_OBJ_TY_BOX, done)
		for (iSide=0; iSide<4; iSide++) {
			PL_INP_CHK(4, !pBox->aSides[iSide].bValOK, done)
		}
		if ((pBox->aSides[0].iRowCol != pBox->aSides[2].iRowCol) &&
				(pBox->aSides[1].iRowCol != pBox->aSides[3].iRowCol)) {
			PL_INP_CHK(5, 1, done)
		}
	}
	else {
		PL_INP_CHK(6, !pObjCtx->bRestoreOK, done)
	}
	PL_INP_CHK(7, pfXZoom == NULL, done)
	PL_INP_CHK(8, pfYZoom == NULL, done)

	if (bZoom) {
		if (pBox->aSides[2].iRowCol == pBox->aSides[0].iRowCol) {
			dXLeft = pBox->aSides[0].dVal;
			dXRight = pBox->aSides[2].dVal;
			dXAnchVal = .5 * (dXLeft + dXRight);
			fXAnchFrac = .5F;
			dXWidth = dXRight - dXLeft;
			dXFullRange = (pPlot->pXAxis->dDValT - 1) - (pPlot->pXAxis->dDValB + 1);
			fXZoomNew = (float)(dXFullRange / dXWidth);
		}
		else {
			dXAnchVal = pPlot->pXAxis->dDValAnchor;
			fXAnchFrac = pPlot->pXAxis->fAnchorFrac;
			fXZoomNew = pPlot->pXAxis->fZoom;
		}
		if (pBox->aSides[3].iRowCol == pBox->aSides[1].iRowCol) {
			dYTop = pBox->aSides[3].dVal;
			dYBottom = pBox->aSides[1].dVal;
			dYAnchVal = .5 * (dYTop + dYBottom);
			fYAnchFrac = .5F;
			dYHeight = dYTop - dYBottom;
			dYFullRange = (pPlot->pYAxis->dDValT - 2) - (pPlot->pYAxis->dDValB + 1);
			fYZoomNew = (float)(dYFullRange / dYHeight);
		}
		else {
			dYAnchVal = pPlot->pYAxis->dDValAnchor;
			fYAnchFrac = pPlot->pYAxis->fAnchorFrac;
			fYZoomNew = pPlot->pYAxis->fZoom;
		}

		if (pObjCtx->bRestoreOK &&
							fXZoomNew == pPlot->pXAxis->fZoom &&
							fYZoomNew == pPlot->pYAxis->fZoom) {
			MessageBeep(MB_ICONEXCLAMATION);
		}
		else {
			if (pObjCtx->bRestoreOK == 0) {
				pObjCtx->bRestoreOK = 1;
				pObjCtx->fXZoom = pPlot->pXAxis->fZoom;
				pObjCtx->fXScroll = pPlot->pXAxis->fScroll;
				pObjCtx->fYZoom = pPlot->pYAxis->fZoom;
				pObjCtx->fYScroll = pPlot->pYAxis->fScroll;
			}
			retStat = PlWinZoom(pPlot, fXZoomNew, fYZoomNew);
			retStat = PlWinScroll_pos(pPlot,
						dXAnchVal, dYAnchVal, fXAnchFrac, fYAnchFrac);
		}
	}
	else if (pObjCtx->bRestoreOK) {
		retStat = PlWinZoom(pPlot, pObjCtx->fXZoom, pObjCtx->fYZoom);
		retStat = PlWinScroll(pPlot, pObjCtx->fXScroll, pObjCtx->fYScroll);
		pObjCtx->bRestoreOK = 0;
	}

	*pfXZoom = pPlot->pXAxis->fZoom;
	*pfYZoom = pPlot->pYAxis->fZoom;
	if (pfXScroll != NULL)
		*pfXScroll = pPlot->pXAxis->fScroll;
	if (pfYScroll != NULL)
		*pfYScroll = pPlot->pYAxis->fScroll;

done:
	PL_IF_INP_ERR("PlObjBoxZoom")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjCoordAddDeltaX - add an X offset to a coordinate
*
* Purpose:
*		Adds an X offset to a coordinate.  Limited checks are done on
*		validity and visibility.  The caller is responsible for updating
*		"valid"- "draw"- and "clip"-related items.
*
*		If bTrackMouse is 1, this routine:
*		o	Sets the current pixel value to the mouse's virtual X
*			coordinate.
*		o	If the mouse's plot area column is valid, sets the coordinate's
*			plot area column to the mouse's.
*		o	Calculates the new data value corresponding to the new pixel
*			value (and possibly new column).
*
*		If the offset is pixels, this routine:
*		o	Adds 'dDeltaX' to the current pixel value.
*		o	Checks to see if the coordinate has switched to a different
*			plot area column.
*		o	Calculates the new data value corresponding to the new pixel
*			value (and possibly new column).
*
*		If the offset is a data value, this routine:
*		o	Adds 'dDeltaX' to the current data value.
*		o	Assumes that the coordinate has not switched to a different
*			plot area column.
*		o	Does NOT calculate new pixel-related values
*
* Return Value:
*		void
*
* Notes:
* 1.	bDeltaPx is ignored if bTrackMouse is 1.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-09-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjCoordAddDeltaX(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pco,			// IO pointer to coordinate to modify
int		bTrackMouse,	// I 1 if want side "stuck to" mouse
int		bDeltaPx,		// I 1 if dDeltaX is pixels
double	dDeltaX)		// I delta X
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		stat;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		dataCol, mouseCol=pMouse->coCurr.x.iRowCol;
	double	dZoom, dScroll;
	double	pxLen, vpxLen, vpxB, vpxT;
	double	dAdjust;
	PL_AXIS	*pXAxis;

	if (bTrackMouse) {
		pco->x.dVirtPx = pMouse->coCurr.x.dVirtPx;
		if (mouseCol >= 0)
			pco->x.iRowCol = mouseCol;
		stat = PlAreaArraySelect(pPlot, rowSave, pco->x.iRowCol);
		pco->x.dVal = PlWinCvtXPixToXVal(pPlot, pco->x.dVirtPx);
	}
	else if (bDeltaPx) {
		pco->x.dVirtPx += dDeltaX;
		dataCol = pco->x.iRowCol;
		dAdjust = 0.;
		while (1) {
			stat = PlAreaArraySelect(pPlot, rowSave, dataCol);
			pXAxis = pPlot->pXAxis;
			dZoom = pXAxis->fZoom;
			dScroll = pXAxis->fScroll;
			pxLen = pXAxis->pxT - pXAxis->pxB + 1.;
			vpxLen = pxLen * dZoom;
			vpxB = pXAxis->pxB - (vpxLen - pxLen) * dScroll;
			vpxT = vpxB + vpxLen - 1.;
			if (pco->x.dVirtPx - dAdjust >= vpxB &&
									pco->x.dVirtPx - dAdjust <= vpxT) {
				// adjust pixel coordinates to match this column
				pco->x.dVirtPx -= dAdjust;
				pco->x.iRowCol = dataCol;
				break;
			}
			if (dDeltaX >= 0.) {
				// When an edge leaves the end of one area, we want it to
				// go to the beginning of the next area.  The amount the edge
				// has to jump is from the "virtual" end of its current area
				// to the visible end of its current area.  After that jump,
				// the edge will still have to traverse (in the normal way)
				// the space between its current area and the area it's heading
				// toward.  dAdjust is the accumulation of the jumps that have
				// been made so far.
				if (dataCol >= pPlot->nAxisCols - 1) {
					pco->x.dVirtPx -= dAdjust;
					pco->x.iRowCol = dataCol;
					break;
				}
				dAdjust += vpxT - pXAxis->pxT;
				dataCol++;
			}
			else {
				// The logic is similar to the above, but the jump is from
				// the "virtual" begin of the edge's current area to the visible
				// begin of the area.
				if (dataCol <= 0) {
					pco->x.dVirtPx -= dAdjust;
					pco->x.iRowCol = dataCol;
					break;
				}
				dAdjust += vpxB - pXAxis->pxB;
				dataCol--;
			}
		}
		stat = PlAreaArraySelect(pPlot, rowSave, pco->x.iRowCol);
		pco->x.dVal = PlWinCvtXPixToXVal(pPlot, pco->x.dVirtPx);
	}
	else
		pco->x.dVal += dDeltaX;

	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjCoordAddDeltaY - add a Y offset to a coordinate
*
* Purpose:
*		Adds a Y offset to a coordinate.  Limited checks are done on
*		validity and visibility.  The caller is responsible for updating
*		"valid"- "draw"- and "clip"-related items.
*
*		If bTrackMouse is 1, this routine:
*		o	Sets the current pixel value to the mouse's virtual Y
*			coordinate.
*		o	If the mouse's plot area row is valid, sets the coordinate's
*			plot area row to the mouse's.
*		o	Calculates the new data value corresponding to the new pixel
*			value (and possibly new row).
*
*		If the offset is pixels, this routine:
*		o	Adds 'dDeltaY' to the current pixel value.
*		o	Checks to see if the coordinate has switched to a different
*			plot area row.
*		o	Calculates the new data value corresponding to the new pixel
*			value (and possibly new row).
*
*		If the offset is a data value, this routine:
*		o	Adds 'dDeltaY' to the current data value.
*		o	Assumes that the coordinate has not switched to a different
*			plot area row.
*		o	Does NOT calculate new pixel-related values
*
* Return Value:
*		void
*
* Notes:
* 1.	If bDeltaPx is 0, then dDeltaY is _subtracted_ from the coordinate,
*		since the Y _data_ direction has the opposite sense of the Y
*		_pixel_ direction.
* 2.	bDeltaPx is ignored if bTrackMouse is 1.
*
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-09-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjCoordAddDeltaY(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pco,			// IO pointer to coordinate to modify
int		bTrackMouse,	// I 1 if want side "stuck to" mouse
int		bDeltaPx,		// I 1 if dDeltaY is pixels
double	dDeltaY)		// I delta Y, in pixels
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		stat;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		dataRow, mouseRow=pMouse->coCurr.y.iRowCol;
	double	dZoom, dScroll;
	double	pxLen, vpxLen, vpxB, vpxT;
	double	dAdjust;
	PL_AXIS	*pYAxis;

	if (bTrackMouse) {
		pco->y.dVirtPx = pMouse->coCurr.y.dVirtPx;
		if (mouseRow >= 0)
			pco->y.iRowCol = mouseRow;
		stat = PlAreaArraySelect(pPlot, pco->y.iRowCol, colSave);
		pco->y.dVal = PlWinCvtYPixToYVal(pPlot, pco->y.dVirtPx);
	}
	else if (bDeltaPx) {
		pco->y.dVirtPx += dDeltaY;
		dataRow = pco->y.iRowCol;
		dAdjust = 0.;
		while (1) {
			stat = PlAreaArraySelect(pPlot, dataRow, colSave);
			pYAxis = pPlot->pYAxis;
			dZoom = pYAxis->fZoom;
			dScroll = pYAxis->fScroll;
			pxLen = pYAxis->pxB - pYAxis->pxT + 1.;
			vpxLen = pxLen * dZoom;
			vpxB = pYAxis->pxB + (vpxLen - pxLen) * dScroll;
			vpxT = vpxB - vpxLen + 1.;
			if (pco->y.dVirtPx - dAdjust <= vpxB &&
									pco->y.dVirtPx - dAdjust >= vpxT) {
				// adjust pixel coordinates to match this column
				pco->y.dVirtPx -= dAdjust;
				pco->y.iRowCol = dataRow;
				break;
			}
			if (dDeltaY >= 0.) {
				// When an edge leaves the bottom of one area, we want it to
				// go to the top of the next area.  The amount the edge has
				// to jump is from the "virtual" bottom of its current area
				// to the visible bottom of its current area.  After that jump,
				// the edge will still have to traverse (in the normal way)
				// the space between its current area and the area it's heading
				// toward.  dAdjust is the accumulation of the jumps that have
				// been made so far.
				if (dataRow >= pPlot->nAxisRows - 1) {
					pco->y.dVirtPx -= dAdjust;
					pco->y.iRowCol = dataRow;
					break;
				}
				dAdjust += vpxB - pYAxis->pxB;
				dataRow++;
			}
			else {
				// The logic is similar to the above, but the jump is from
				// the "virtual" top of the edge's current area to the visible
				// top of the area.
				if (dataRow <= 0) {
					pco->y.dVirtPx -= dAdjust;
					pco->y.iRowCol = dataRow;
					break;
				}
				dAdjust += vpxT - pYAxis->pxT;
				dataRow--;
			}
		}
		stat = PlAreaArraySelect(pPlot, pco->y.iRowCol, colSave);
		pco->y.dVal = PlWinCvtYPixToYVal(pPlot, pco->y.dVirtPx);
	}
	else
		pco->y.dVal -= dDeltaY;

	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjCopyAll - copy all objects
*
* Purpose:
*		Copy all objects.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-31-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjCopyAll(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_CTX	*pPlotSrc)		// I pointer to plot context containint the objects
{
	int		retStat=PL_OK;
	PL_OBJ_CTX	*pObjCtx=pPlot->pObjCtx, *pObjCtxSrc=pPlotSrc->pObjCtx;
	PL_OBJ	*pObj, *pObjSrc;

	if (pObjCtxSrc->nObjects <= 0) goto done;

	for (pObjSrc=pObjCtxSrc->pHead; pObjSrc!=NULL; pObjSrc=pObjSrc->pNext) {
		if (pObjSrc->eObjType == PL_OBJ_TY_BOX) {
			PL_BOX	*pBox;
			if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
				retStat = PL_MALLOC_FAIL;
				goto done;
			}
			// because of links, do copy before append
			*pBox = *(PL_BOX *)pObjSrc;
			pObj = (PL_OBJ *)pBox;
			pObjCtx->nBoxes++;
		}
		else if (pObjSrc->eObjType == PL_OBJ_TY_LABEL) {
			PL_LABEL	*pLabel;
			if ((pLabel = (PL_LABEL *)malloc(sizeof(PL_LABEL))) == NULL) {
				retStat = PL_MALLOC_FAIL;
				goto done;
			}
			// because of links, do copy before append
			*pLabel = *(PL_LABEL *)pObjSrc;
			pObj = (PL_OBJ *)pLabel;
		}
		else if (pObjSrc->eObjType == PL_OBJ_TY_LABEL) {
			PL_LABEL	*pLabel;
			if ((pLabel = (PL_LABEL *)malloc(sizeof(PL_LABEL))) == NULL) {
				retStat = PL_MALLOC_FAIL;
				goto done;
			}
			// because of links, do copy before append
			*pLabel = *(PL_LABEL *)pObjSrc;
			pObj = (PL_OBJ *)pLabel;
		}
		else if (pObjSrc->eObjType == PL_OBJ_TY_LEGEND) {
			PL_LEGEND *pLegend, *pLegSrc=(PL_LEGEND *)pObjSrc;
			PL_LEGEND_ENTRY *pEntry=NULL, *pEntSrc;
			if ((pLegend = (PL_LEGEND *)malloc(sizeof(PL_LEGEND))) == NULL) {
				retStat = PL_MALLOC_FAIL;
				goto done;
			}
			*pLegend = *pLegSrc;
			pLegend->pHead = pLegend->pTail = NULL;
			for (pEntSrc=pLegSrc->pHead; pEntSrc!=NULL; pEntSrc=pEntSrc->pNext) {
				pEntry = (PL_LEGEND_ENTRY *)malloc(sizeof(PL_LEGEND_ENTRY));
				if (pEntry == NULL) {
					for (pEntry=pLegend->pHead; pEntry!=NULL; pEntry=pLegend->pHead) {
						DLIST_REMOVE(pEntry, pLegend->pHead, pLegend->pTail);
						free(pEntry);
					}
					free(pLegend);
					retStat = PL_MALLOC_FAIL;
					goto done;
				}
				// because of links, do copy before append
				*pEntry = *pEntSrc;
				DLIST_APPEND(pEntry, pLegend->pHead, pLegend->pTail);
			}
			// because of links, do copy before append
			pObj = (PL_OBJ *)pLegend;
		}
		else {
			MessageBox(NULL, "unexpected object type", "PlObjCopyAll", MB_OK);
			retStat = PL_FAIL;
			goto done;
		}
		// put the copied object on the destination's list
		DLIST_APPEND(pObj, pObjCtx->pHead, pObjCtx->pTail);
		pObjCtx->nObjects++;
		if (pObjSrc == pObjCtxSrc->pSelObj)
			pObjCtx->pSelObj = pObj;
	}

done:
	if (retStat != PL_OK) {
		PlObjDeleteAll(pPlot, 1);
	}
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjCvtXPxYPxToCoord - convert X,Y pixel pair to PL_COORD
*
* Purpose:
*		Converts a non-virtual X,Y pixel pair to a PL_COORD, filling in
*		as much information as possible.
*
* Implicit Outputs:
*		.x.dVirtPx, .x.iVisPx, and .x.iDrawPx are set to iXPx
*		If iXPx is in the X range of a data area:
*			.x.iRowCol is set to the plot area array column
*			.x.dVal is set to the dVal corresponding to iXPx
*			.x.bValOK and . x.bEdgeVis are set to 1
*		.y.dVirtPx, .y.iVisPx, and .y.iDrawPx are set to iYPx
*		If iYPx is in the Y range of a data area:
*			.y.iRowCol is set to the plot area array row
*			.y.dVal is set to the dVal corresponding to iYPx
*			.y.bValOK and . y.bEdgeVis are set to 1
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-18-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjCvtXPxYPxToCoord(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		iXPx,			// I X pixel value, in plot window coordinates
int		iYPx,			// I Y pixel value, in plot window coordinates
PL_COORD *pco)		// O the resulting PL_COORD
{
	int		row, col, rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		stat;

	pco->x.dVirtPx = pco->x.iVisPx = pco->x.iDrawPx = iXPx;
	pco->x.bValOK = pco->x.bEdgeVis = 0;
	col = pco->x.iRowCol = PlWinCvtXPixToAreaCol(pPlot, iXPx);

	pco->y.dVirtPx = pco->y.iVisPx = pco->y.iDrawPx = iYPx;
	pco->y.bValOK = pco->y.bEdgeVis = 0;
	row = pco->y.iRowCol = PlWinCvtYPixToAreaRow(pPlot, iYPx);

	if (col >= 0) {
		stat = PlAreaArraySelect(pPlot, rowSave, col);
		if (stat != PL_OK) goto done;
		pco->x.dVal = PlWinCvtXPixToXVal(pPlot, iXPx);
		pco->x.bValOK = pco->x.bEdgeVis = 1;
	}

	if (row >= 0) {
		stat = PlAreaArraySelect(pPlot, row, colSave);
		if (stat != PL_OK) goto done;
		pco->y.dVal = PlWinCvtYPixToYVal(pPlot, iYPx);
		pco->y.bValOK = pco->y.bEdgeVis = 1;
	}
done:
	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjDelete - delete a plot object
*
* Purpose:
*		Delete the specified plot object.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function the delete, or
*		other codes for other errors
*
* Notes:
* 1.	If pObj is NULL, then the selected PL_OBJ is deleted.
* 2.	If the return value is PL_OK, then pObj is no longer a valid
*		pointer and must not be used by the program.
* 3.	This routine deletes, with no feedback to the caller, a PL_OBJ
*		even if it has the .bNoDelete flag set.  (The .bNoDelete flag
*		is honored only when the operator attempts to delete using the
*		keyboard.)
*
* See Also:
*		PlObjDeleteAll
*		PlObjBoxCreateGeneral, PlObjBoxCreateSimple
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-09-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjDelete(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 to inhibit calling object callback function
PL_OBJ	*pObj)			// I pointer to PL_OBJ; NULL implies selected object
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	int		bUsedSel=0;
	int		stat;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pObj is NULL, there must be a selected PL_OBJ
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	PL_INP_CHK(2, pObj == NULL && pObjCtx->pSelObj == NULL, done)

	if (pObj == NULL) {
		bUsedSel = 1;
		pObj = pObjCtx->pSelObj;
	}

	if (bUsedSel || pObj == pObjCtx->pSelObj) {
		retStat = PlObjSelect(pPlot, 0, NULL, 0);	// deselect
		if (retStat != PL_OK) {
			// Callback want's to veto the deselect.
			goto done;
		}
	}
	if (!bNoCallback && pPlot->pObjectFn != NULL) {
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx,
										pObj, PL_OBJ_EV_DEL, 0., 0.);
		if (stat == 0) {
			// Callback want's to veto.
			retStat = PL_VETOED;
			goto done;
		}
	}

	// erase the "old" version of the object
	PlObjPaint(pPlot, pObj, bUsedSel);

	DLIST_REMOVE(pObj, pObjCtx->pHead, pObjCtx->pTail);
	if (pObj->eObjType == PL_OBJ_TY_BOX)
		pObjCtx->nBoxes--;
	else if (pObj->eObjType == PL_OBJ_TY_LEGEND) {
		PL_LEGEND *pLegend=(PL_LEGEND *)pObj;
		PL_LEGEND_ENTRY *pEntry=NULL;
		for (pEntry=pLegend->pHead; pEntry!=NULL; pEntry=pLegend->pHead) {
			DLIST_REMOVE(pEntry, pLegend->pHead, pLegend->pTail);
			free(pEntry);
		}
	}
	free(pObj);
	pObjCtx->nObjects--;

done:
	PL_IF_INP_ERR("PlObjDelete")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjDeleteAll - delete all objects
*
* Purpose:
*		Delete all objects.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function the delete, or
*		other codes for other errors
*
* Notes:
* 1.	This routine deletes, with no feedback to the caller, an object
*		even if it has the .bNoDelete flag set.  (The .bNoDelete flag
*		is honored only when the operator attempts to delete using the
*		keyboard.)
*
* See Also:
*		PlObjDelete
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-09-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjDeleteAll(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback)	// I 1 to inhibit calling object callback function
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_OBJ	*pObj;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	for (pObj=pObjCtx->pHead; pObj!=NULL; pObj=pObjCtx->pHead) {
		retStat = PlObjDelete(pPlot, bNoCallback, pObj);
		if (retStat != PL_OK) goto done;
	}

done:
	PL_IF_INP_ERR("PlObjDeleteAll")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditDialog - show an edit dialog for an object
*
* Purpose:
*		Shows an edit dialog for an object.
*
* Return Value:
*		a character indicating further desired processing
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-31-96 R. Cole		created
*--------------------------------------------------------------------------*/
WORD PASCAL
PlObjEditDialog(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bDblClick,		// I 1 if called for double click
WPARAM	wParam)			// I virtual key code
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pSelObj=pObjCtx->pSelObj;
	PL_BOX *pBox=NULL;
	PL_LABEL *pLabel=NULL;
	PL_LEGEND *pLegend=NULL;
	HCURSOR	hCurOld;
	PL_LABEL labelDialog;

	if (pSelObj != NULL) {
		if (pSelObj->eObjType == PL_OBJ_TY_BOX) pBox = (PL_BOX *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LABEL) pLabel = (PL_LABEL *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LEGEND) pLegend = (PL_LEGEND *)pSelObj;
	}

	if (pLabel != NULL) {
		labelDialog = *pLabel;
		pPlot->pObjCtx->bObjEditDone = 0;
		hCurOld = SetCursor(LoadCursor(NULL, IDC_ARROW));
		PlObjWinDialog(pPlot, pPlot->hwPlot, 0, (PL_OBJ *)&labelDialog);
		while (!pPlot->pObjCtx->bObjEditDone)
			WuDoEvents();
		if (hCurOld != 0) SetCursor(hCurOld);
		PlObjLabelPaint(pPlot, pLabel, 1);		// erase the label
		*pLabel = labelDialog;
		PlObjLabelPaint(pPlot, pLabel, 1);		// repaint the label
		wParam = 'T';
	}
	else
		wParam = '\0';

	return wParam;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditDone - finish processing altered object
*
* Purpose:
*		Finishes the processing for a newly created object or for a object
*		that has been edited.
*
*		The object will be in the selected state when this routine exits.
*
* Return Value:
*		void
*
* Notes:
* 1.	If memory can't be allocated, the operator is given a message
*		box, the PL_OBJs are deleted, and execution continues.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjEditDone(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pObj;
	int		eEvent;

	if (pObjCtx->bCreateActive) {
		if (pObjCtx->bCreateIsCopy)		eEvent = PL_OBJ_EV_COPY;
		else							eEvent = PL_OBJ_EV_CREATE;
	}
	else if (pObjCtx->bEditIsMove)		eEvent = PL_OBJ_EV_MOVE;
	else								eEvent = PL_OBJ_EV_RESIZE;

	if (pMouse->bLeftClick) goto edit_aborted;

	if (pObjCtx->bEditVisible) {
		// Erase existing edit object.
		PlObjPaint_edit(pPlot);
	}

	pObj = pObjCtx->pEditObj;

	if (!PlObjEditDoneIsOK(pPlot))
		goto edit_aborted;

	if (pPlot->pObjectFn != NULL) {
		int		stat;
		// Tell callback that edit/create is about to complete.
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, pObj,
									PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_DONE);
		if (stat == 0) {
			// Callback want's to veto.
			goto edit_aborted_beep;
		}
	}

	if (pObjCtx->bCreateActive) {
		if (pObj->eObjType == PL_OBJ_TY_BOX)
			pObjCtx->nBoxes++;
		DLIST_APPEND(pObj, pObjCtx->pHead, pObjCtx->pTail);
		pObjCtx->nObjects++;
		pObjCtx->pSelObj = pObj;
		pObjCtx->pEditObj = NULL;
	}
	else {
		// erase the "old" version of the object
		pObj = pObjCtx->pSelObj;
		if (pObj == NULL) goto edit_aborted;
		PlObjPaint(pPlot, pObj, 1);		// 1 ==> selected
		// and copy the edited version back to the "real" version
		if (pObjCtx->pEditObj->eObjType == PL_OBJ_TY_BOX)
			*(PL_BOX *)pObj = *(PL_BOX *)pObjCtx->pEditObj;
		else if (pObjCtx->pEditObj->eObjType == PL_OBJ_TY_LABEL)
			*(PL_LABEL *)pObj = *(PL_LABEL *)pObjCtx->pEditObj;
		else if (pObjCtx->pEditObj->eObjType == PL_OBJ_TY_LEGEND)
			*(PL_LEGEND *)pObj = *(PL_LEGEND *)pObjCtx->pEditObj;
	}

	// paint the new object
	PlObjPaint(pPlot, pObj, 1);	// 1 ==> selected

done:
	if (pObjCtx->pEditObj != NULL)
		free(pObjCtx->pEditObj);
	pObjCtx->pEditObj = NULL;
	pObjCtx->bEditVisible = 0;
	pObjCtx->bEditActive = pObjCtx->bCreateActive = pObjCtx->bCreateIsCopy = 0;
	pMouse->hPointerEdit = 0;
	return;
edit_aborted:
	if (pPlot->pObjectFn != NULL) {
		pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, NULL,
									PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_ABORT);
	}
edit_aborted_beep:
	if (!pMouse->bLeftClick)
		MessageBeep(MB_ICONEXCLAMATION);
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditDoneIsOK - diagnoses the edit-done situation
*
* Purpose:
*		Checks to see if the 

* Return Value:
*		1 if the current state of the edit object is OK, or
*		0 if not
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-22-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjEditDoneIsOK(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pObj;
	int		bEditDoneIsOK=0;

	pObj = pObjCtx->pEditObj;
	if (pObj->eObjType == PL_OBJ_TY_BOX) {
		PL_BOX	*pBox=(PL_BOX *)pObj;
		int		iSide;

		for (iSide=0; iSide<4; iSide++) {
			if (!pBox->aSides[iSide].bValOK)
				goto done;
		}
		if (!pObjCtx->mode.bEnbBoxSpan) {
			if (pBox->aSides[0].iRowCol != pBox->aSides[2].iRowCol)
				goto done;
			if (pBox->aSides[1].iRowCol != pBox->aSides[3].iRowCol)
				goto done;
		}
		bEditDoneIsOK = 1;
	}
	else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
	}

done:
	return bEditDoneIsOK;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditMove - process a mouse move event for an object
*
* Purpose:
*		Processes a mouse move event for an object:
*		o	changes the pixel coordinates of whatever parts are in motion
*		o	updates the corresponding dVal and bValOK
*
*		If the SHIFT key is down during a move, the current position is
*		constrained to be either horizontal or vertical from the  starting
*		position, depending on which is closer to matching the track that
*		the mouse pointer is taking.
*
* Return Value:
*		void
*
* Theory Of Operation:
*		Each time we come to this routine, we start fresh with the "rubber
*		band" box on top of the original box being edited.  Then we
*		calculate where the sides of the "rubber band" box are based on
*		the relation between the current mouse position and the begin
*		mouse position.  I.e., the "deltaX" tells how far to move the
*		right and/or left sides horizontally and the "deltaY" tells how
*		far to move the top and/or bottom sized vertically.  If this is a
*		move, all four sides are moving, so that the size of the "rubber
*		band" box is constant.  If this is a resize, only one or two sides
*		are moving, with the other sides staying on top of the sides of
*		the original box.
*
*		In most cases, the deltaX and deltaY move the affected sides
*		through "virtual" pixel space.  When the zoom is 1, "virtual"
*		pixel space is the same as physical pixel space.  If an axis is
*		zoomed, however, "virtual" pixel space is 'larger' than physical
*		pixel space by the zoom factor, and the data area serves a viewport
*		into "virtual" pixel space.  If there is an array of plot areas,
*		then two (or more) "virtual" pixel locations from two (or more)
*		plot areas will map to the same physical pixel location, with the
*		mapping changing as zoom factor and scroll position changes.  A
*		lot of the complexity associated with moving the "rubber band" box
*		is because of this multiple mapping and also because of the desire
*		to have the behavior of the "rubber band" box feel natural.
*
*		When deltaX and deltaY are moving sides through "virtual" pixel
*		space, the affected sides can be moved outside the visible data
*		area.  Most of the time this looks and feels natural.  This feature
*		allows changing the size or position of a box when only one edge
*		is visible.  The hidden edges can even be moved into different
*		plot areas.
*
*		There are two cases where the natural behavior is to be moving in
*		physical pixel space.  For a resize, it is assumed that a side that
*		is moving is "tied to" the mouse.  Thus when the mouse moves into
*		a new plot area, the side should also move into that plot area,
*		without regard for any "virtual" pixel space considerations.
*
*		For a move that starts out with all four sides of the box being
*		visible, there is a similar expectation that when the mouse moves
*		into a new plot area the entire box will follow. In this
*		implementation, a shortcut is taken that covers the cases that
*		are likely to occur.  If the left and right sides are both in the
*		same plot area column, and if the mouse is in a data area column,
*		then the box's left and right sides are moved to the mouse's
*		column.  This switches the left and right sides from their original
*		"virtual" pixel space to the mouse's current "virtual" pixel space.
*		Afther this switch, the box is then given the normal horizontal
*		treatment using deltaX.  The top and bottom sides are handled in a
*		similar way.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjEditMove(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pObj=pObjCtx->pEditObj;
	PL_COORD *pcoLBCurr=&pObjCtx->coEditLBCurr;
	PL_COORD *pcoRTCurr=&pObjCtx->coEditRTCurr;
	PL_COORD_HALF coordhalfTemp;
	double	dDeltaX, dDeltaY;
	int		bTrackMouse;
	int		bSidesSwitched=0;
	int		eEvent;

	if (pObjCtx->bCreateActive) {
		if (pObjCtx->bCreateIsCopy)		eEvent = PL_OBJ_EV_COPY;
		else							eEvent = PL_OBJ_EV_CREATE;
	}
	else if (pObjCtx->bEditIsMove)		eEvent = PL_OBJ_EV_MOVE;
	else								eEvent = PL_OBJ_EV_RESIZE;

	if (pObjCtx->bEditVisible) {
		// Erase existing edit object.
		PlObjPaint_edit(pPlot);
	}

/*---------------------------------------------------------------------------
*	Calculate the horizontal and vertical distance the mouse has moved
*	since the edit began.
*
*	If this a move and the SHIFT key is currently down, then the move
*	or copy is constrained to be either vertical or horizontal.
*--------------------------------------------------------------------------*/
	dDeltaX = pMouse->coCurr.x.iVisPx - pObjCtx->coMouseBegin.x.iVisPx;
	dDeltaY = pMouse->coCurr.y.iVisPx - pObjCtx->coMouseBegin.y.iVisPx;

	if (pMouse->bShift && (pObjCtx->bEditIsMove || pObjCtx->bCreateIsCopy)) {
		if (fabs(dDeltaX) >= fabs(dDeltaY)) dDeltaY = 0.;
		else dDeltaX = 0.;
	}

/*---------------------------------------------------------------------------
*	Adjust the sides of the "rubber band" box as appropriate for the type
*	of edit operation.
*
*	o	First, make "current" a copy of "begin".  "begin" is the original
*		and "current" is an entirely new "rubber band".
*	o	If this is a move and conditions are right, jump to the mouse's
*		plot area column.
*	o	Add deltaX to the left and/or right side.  (If this is a resize,
*		one side may be tied to the mouse.)
*	o	If this is a move and conditions are right, jump to the mouse's
*		plot area row.
*	o	Add deltaX to the bottom and/or top side.  (If this is a resize,
*		one side may be tied to the mouse.)
*
*	The result of all this is new dVal values and possibly new iRow and/or
*	iCol for the "rubber band" box.  No calculations for clip have been
*	done, and the sides may be "out of proper order".
*--------------------------------------------------------------------------*/
	*pcoLBCurr = pObjCtx->coEditLBBeg;
	*pcoRTCurr = pObjCtx->coEditRTBeg;

	if (pObjCtx->bEditIsMove && dDeltaX != 0. && pPlot->pXAxis->fZoom > 1.F) {
		if (pMouse->coCurr.x.iRowCol < 0) goto move_horiz;
		if (pcoLBCurr->x.iRowCol != pcoRTCurr->x.iRowCol) goto move_horiz;
		pcoLBCurr->x.iRowCol = pMouse->coCurr.x.iRowCol;
		pcoRTCurr->x.iRowCol = pMouse->coCurr.x.iRowCol;
	}
move_horiz:
	if (pObjCtx->bMoveLeft) {
		bTrackMouse = pObjCtx->bMoveRight ? 0 : 1;
		PlObjCoordAddDeltaX(pPlot, pcoLBCurr, bTrackMouse, 1, dDeltaX);
	}
	if (pObjCtx->bMoveRight) {
		bTrackMouse = pObjCtx->bMoveLeft ? 0 : 1;
		PlObjCoordAddDeltaX(pPlot, pcoRTCurr, bTrackMouse, 1, dDeltaX);
	}
	if (pObjCtx->bEditIsMove && dDeltaY != 0. && pPlot->pYAxis->fZoom > 1.F) {
		if (pMouse->coCurr.y.iRowCol < 0) goto move_vert;
		if (pcoLBCurr->y.iRowCol != pcoRTCurr->y.iRowCol) goto move_vert;
		pcoLBCurr->y.iRowCol = pMouse->coCurr.y.iRowCol;
		pcoRTCurr->y.iRowCol = pMouse->coCurr.y.iRowCol;
	}
move_vert:
	if (pObjCtx->bMoveBottom) {
		bTrackMouse = pObjCtx->bMoveTop ? 0 : 1;
		PlObjCoordAddDeltaY(pPlot, pcoLBCurr, bTrackMouse, 1, dDeltaY);
	}
	if (pObjCtx->bMoveTop) {
		bTrackMouse = pObjCtx->bMoveBottom ? 0 : 1;
		PlObjCoordAddDeltaY(pPlot, pcoRTCurr, bTrackMouse, 1, dDeltaY);
	}

/*---------------------------------------------------------------------------
*	Now finish the alterations:
*	o	calculate new pixel-related values--for drawing and for testing
*		if the new box is valid (in "edit done" processing)
*	o	if left and right have switched and/or  top and bottom have switched,
*		re-order the PL_COORD and re-calculate pixel-related values
*	o	calculate new pixel-related values--for drawing and for testing
*		if the new box is valid (in "edit done" processing)
*	o	store the new coordinates as a box (to give the callback function)
*--------------------------------------------------------------------------*/
	PlObjBoxCoordCalcPx(pPlot, 0, pcoLBCurr, pcoRTCurr);
	if (pcoLBCurr->x.iRowCol >= 0 && pcoRTCurr->x.iRowCol >= 0 &&
						pcoLBCurr->x.iVisPx > pcoRTCurr->x.iVisPx) {
		coordhalfTemp = pcoLBCurr->x;
		pcoLBCurr->x = pcoRTCurr->x;
		pcoRTCurr->x = coordhalfTemp;
		bSidesSwitched = 1;
	}
	if (pcoLBCurr->y.iRowCol >= 0 && pcoRTCurr->y.iRowCol >= 0 &&
								pcoLBCurr->y.iVisPx < pcoRTCurr->y.iVisPx) {
		coordhalfTemp = pcoLBCurr->y;
		pcoLBCurr->y = pcoRTCurr->y;
		pcoRTCurr->y = coordhalfTemp;
		bSidesSwitched = 1;
	}
	if (pObj->eObjType == PL_OBJ_TY_BOX) {
		if (bSidesSwitched)
			PlObjBoxCoordCalcPx(pPlot, 0, pcoLBCurr, pcoRTCurr);
		PlObjBoxCvtCoordToBox(pPlot,
					pcoLBCurr, pcoRTCurr, (PL_BOX *)pObj);
	}
	else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
	}

	if (!pMouse->bLeftClick) {
		// Draw new edit object.
		pObjCtx->bEditVisible = 1;
		PlObjPaint_edit(pPlot);
	}

	if (pPlot->pObjectFn != NULL) {
		// Tell callback that size and/or position is about to change.
		pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, pObj,
										PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_COORD);
	}
	if (!PlObjEditDoneIsOK(pPlot))
		pMouse->bUseNOTPointer = 1;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditStart - start the edit/create/copy process for an object
*
* Purpose:
*		Starts the edit/create/copy process for an object:
*		o	if another object is currently selected, it is deselected
*		o	if PlWinMousePreProcess has vetoed the edit/create, a beep is
*			sounded and the edit is not started
*		o	if the object isn't currently selected, it is selected and
*			no edit is started
*		o	if the object is currently selected, the edit begins
*		o	if there is no object at the pointer location, the create
*			begins
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjEditStart(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pObj;
	PL_OBJ	*pSelAtEntry=pObjCtx->pSelObj;
	int		stat;
	int		eEvent;

	pObjCtx->bCreateActive = pObjCtx->bCreateIsCopy =
							pObjCtx->bEditActive = pObjCtx->bKbEditActive = 0;
	pObjCtx->pEditObj = NULL;

	pObjCtx->coMouseBegin = pMouse->coCurr;
	pObjCtx->bShiftBegin = pMouse->bShift;
	pObjCtx->bCtrlBegin = pMouse->bCtrl;

/*---------------------------------------------------------------------------
* DE-SELECT
*
*		Want to deselect the currently selected object if:
*		o	the mouse wasn't in or on it when the "button down" occurred, or
*		o	the mouse was on it, with CTRL, when the "button down" occurred
*			(i.e., the operator is making a copy of it)
*--------------------------------------------------------------------------*/
	if (!pMouse->bHitSelected || pObjCtx->bCtrlBegin) {
		stat = PlObjSelect(pPlot, 0, NULL, 0);	// deselect
		if (stat != PL_OK) {
			// Callback want's to veto the deselect.
			goto done;
		}
	}

	if (!pMouse->bInOtherArea && !pMouse->bInCurrArea &&
								!pMouse->bHitSelected && !pMouse->bHitNonSel) {
		if (!pMouse->bLeftClick)
			pMouse->bUseNOTPointer = 1;
		goto done;
	}

/*---------------------------------------------------------------------------
* SELECT
*
*		Want to do a select if:
*		o	the mouse hit a non-selected object
*--------------------------------------------------------------------------*/
	if (pMouse->bHitNonSel) {
		stat = PlObjSelect(pPlot, 0, pMouse->pHitObj, 1);	// select
		if (stat != PL_OK) {
			// Callback want's to veto the select.
			goto done;
		}
		pMouse->hPointerNew = pMouse->hPointerHit;
		goto done;
	}

/*---------------------------------------------------------------------------
* CREATE/COPY
*
*		The mouse is down.  There are two conditions that indicate a create
*		is desired:
*		o	If the mouse pointer isn't on an object, then a simple create begins.
*		o	If the mouse pointer is on a selected object and the CTRL key is
*			pressed, then a copy begins.
*--------------------------------------------------------------------------*/
	if (pMouse->pHitObj == NULL || pObjCtx->bCtrlBegin) {
		if (!pObjCtx->mode.bEditMode) goto done;
		if (!pObjCtx->mode.bEnbBoxNew) goto done;
		if (pObjCtx->nBoxes >= 1 && !pObjCtx->mode.bEnbBoxMany) goto done;
		if (pObjCtx->bCtrlBegin) {
			// create a copy
			if (pSelAtEntry == NULL) goto done;
			pObjCtx->bCreateIsCopy = 1;
			pObj = pSelAtEntry;
			if (pObj->eObjType == PL_OBJ_TY_BOX) {
				PL_BOX	*pBox;
				if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
					MessageBox(NULL, "Couldn't allocate memory", "PL.LIB", MB_OK);
					goto done;
				}
				*pBox = *(PL_BOX *)pObj;			// make the copy
				PlObjBoxCvtBoxToCoord(pPlot, pBox, 1,
						&pObjCtx->coEditLBBeg, &pObjCtx->coEditRTBeg);
				pObjCtx->coEditLBCurr = pObjCtx->coEditLBBeg;
				pObjCtx->coEditRTCurr = pObjCtx->coEditRTBeg;
				pObjCtx->pEditObj = (PL_OBJ *)pBox;	// and store ptr to copy
			}
			pObj = pObjCtx->pEditObj;
			pObjCtx->bMoveLeft = pObjCtx->bMoveBottom =
									pObjCtx->bMoveTop = pObjCtx->bMoveRight = 1;
			pMouse->hPointerEdit = LoadCursor(NULL, IDC_SIZE);
		}
		else {
			// create a new object
			pObjCtx->coEditLBBeg = pObjCtx->coEditRTBeg = pMouse->coCurr;
			pObjCtx->coEditLBCurr = pObjCtx->coEditRTCurr = pMouse->coCurr;
			if (pObjCtx->mode.eCreateObjType == PL_OBJ_TY_BOX) {
				PL_BOX	*pBox;
				// Create a "default" box.
				if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
					MessageBox(NULL, "Couldn't allocate memory", "PL.LIB", MB_OK);
					goto done;
				}
				PlObjBoxInit(pPlot, pBox, 0);		// not hidden
				PlObjBoxCvtCoordToBox(pPlot, &pObjCtx->coEditLBCurr,
									&pObjCtx->coEditRTCurr, pBox);
				pObjCtx->pEditObj = (PL_OBJ *)pBox;
				pObjCtx->bMoveTop = pObjCtx->bMoveRight = 1;
				pObjCtx->bMoveBottom = pObjCtx->bMoveLeft = 0;
				pMouse->hPointerEdit = pMouse->hPointerCreate;
			}
		}
		if (pPlot->pObjectFn != NULL) {
			// Tell callback that create is about to start.
			if (pObjCtx->bCreateIsCopy)		eEvent = PL_OBJ_EV_COPY;
			else							eEvent = PL_OBJ_EV_CREATE;
			stat = pPlot->pObjectFn(pPlot,
								pPlot->pObjectArg, pObjCtx, pObjCtx->pEditObj,
								PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_BEGIN);
			if (stat == 0) {
				// Callback want's to veto.
				goto done;
			}
		}
		pObjCtx->bCreateActive = 1;
		pMouse->hPointerNew = pMouse->hPointerEdit;
		goto done;
	}

/*---------------------------------------------------------------------------
* EDIT
*
*		The mouse is down.  An edit begins when:
*		o	the mouse pointer is on a selected object; and
*		o	the CTRL key is NOT pressed.
*--------------------------------------------------------------------------*/
	if (PlObjEditStartIsOK(pPlot)) {
		pObjCtx->coMouseBegin = pMouse->coCurr;
		pObjCtx->bShiftBegin = pMouse->bShift;
		pObjCtx->bCtrlBegin = pMouse->bCtrl;
		pObj = pObjCtx->pSelObj;
		if (pObj->eObjType == PL_OBJ_TY_BOX) {
			PL_BOX	*pBox;

			if ((pBox = (PL_BOX *)malloc(sizeof(PL_BOX))) == NULL) {
				MessageBox(NULL, "Couldn't allocate memory", "PL.LIB", MB_OK);
				goto done;
			}
			// copy the box being edited
			*pBox = *(PL_BOX *)pObj;
			PlObjBoxCvtBoxToCoord(pPlot, pBox, 1,
						&pObjCtx->coEditLBBeg, &pObjCtx->coEditRTBeg);
			pObjCtx->coEditLBCurr = pObjCtx->coEditLBBeg;
			pObjCtx->coEditRTCurr = pObjCtx->coEditRTBeg;
			pObjCtx->pEditObj = (PL_OBJ *)pBox;
		}
		if (pPlot->pObjectFn != NULL) {
			// Tell callback that edit is about to start.
			if (pObjCtx->bEditIsMove)				eEvent = PL_OBJ_EV_MOVE;
			else									eEvent = PL_OBJ_EV_RESIZE;
			stat = pPlot->pObjectFn(pPlot,
							pPlot->pObjectArg, pObjCtx, pObjCtx->pEditObj,
							PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_BEGIN);
			if (stat == 0) {
				// Callback want's to veto.
				goto done;
			}
		}
		else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
		}
		pObjCtx->bEditActive = 1;
		pMouse->hPointerEdit = pMouse->hPointerNew = pMouse->hPointerHit;
	}

done:
	if (!pObjCtx->bCreateActive && !pObjCtx->bEditActive) {
		if (pObjCtx->pEditObj != NULL) free(pObjCtx->pEditObj);
		pObjCtx->pEditObj = NULL;
	}
	pObjCtx->bEditVisible = 0;
	if (pSelAtEntry != pObjCtx->pSelObj)
		pMouse->bTossNextClick = 1;
	return;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjEditStartIsOK - diagnoses the edit-start situation
*
* Purpose:
*		o	Figures out, based on pMouse->iHitType, whether move or
*			resize is being requested.
*		o	Checks to see if the requested edit type is allowed (based
*			on the box properties and the global mode flags).
*		o	Figures out which sides are to be moved.
*
*		The edit type (move or resize) and the move flags for the sides
*		are recorded in the PL_OBJ_CTX structure.
*
* Return Value:
*		1 if it is OK to start the edit, or
*		0
*
* Notes:
* 1.	This routine must be called only if pMouse->pHitObj != NULL.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjEditStartIsOK(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pObj;
	PL_BOX_PROPS *pProps;
	int		bEditStartIsOK=1;
	int		bNoMove=0, bNoResize=0;

	pObjCtx->bCreateActive = pObjCtx->bEditActive = 0;
	pObjCtx->bEditIsMove = pObjCtx->bEditIsResize = 0;
	pObjCtx->bMoveLeft = pObjCtx->bMoveBottom =
									pObjCtx->bMoveTop = pObjCtx->bMoveRight = 0;

	pObj = pMouse->pHitObj;
	if (pObj->eObjType == PL_OBJ_TY_BOX) {
		PL_BOX	*pBox=(PL_BOX *)pObj;
		pProps = &pBox->props;

		if (pProps->bNoMove && !pObjCtx->mode.bEnbMove)			bNoMove = 1;
		if (pProps->bNoResize && !pObjCtx->mode.bEnbResize)		bNoResize = 1;
		if (pMouse->iHitType < 0)
			bEditStartIsOK = 0;
		else if (pMouse->iHitType == PL_OBJ_C) {
			if (bNoMove)
				bEditStartIsOK = 0;
			else {
				pObjCtx->bEditIsMove = 1;
				pObjCtx->bMoveLeft = pObjCtx->bMoveBottom =
									pObjCtx->bMoveTop = pObjCtx->bMoveRight = 1;
			}
		}
		else {
			if (bNoResize)
				bEditStartIsOK = 0;
			else {
				pObjCtx->bEditIsResize = 1;
				switch (pMouse->iHitType) {
					case PL_OBJ_TL: pObjCtx->bMoveLeft = pObjCtx->bMoveTop = 1; break;
					case PL_OBJ_BR: pObjCtx->bMoveBottom = pObjCtx->bMoveRight = 1; break;
					case PL_OBJ_TR: pObjCtx->bMoveRight = pObjCtx->bMoveTop = 1; break;
					case PL_OBJ_BL: pObjCtx->bMoveLeft = pObjCtx->bMoveBottom = 1; break;
					case PL_OBJ_T: pObjCtx->bMoveTop = 1; break;
					case PL_OBJ_B: pObjCtx->bMoveBottom = 1; break;
					case PL_OBJ_L: pObjCtx->bMoveLeft = 1; break;
					case PL_OBJ_R: pObjCtx->bMoveRight = 1; break;
				}
			}
		}
	}
	else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
		bEditStartIsOK = 0;
	}
	else
		bEditStartIsOK = 0;

	return bEditStartIsOK;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjFontSetDflt - set the default font for new plot objects
*
* Purpose:
*		Sets the font that will be used when new plot objects are created,
*		whether the create is done by the operator or the program.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-31-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjFontSetDflt(
PL_CTX	*pPlot,			// IO pointer to plot context structure
float	fPts,			// I default text size, in points
const char *szFont)		// I default font name
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		fPts must be >= 1
* 3		szFont must not be NULL
* 4		the length of szFont must be > 0 and less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	PL_INP_CHK(2, fPts < 1.F, done)
	PL_INP_CHK(3, szFont == NULL, done)
	PL_INP_CHK(4, strlen(szFont) == 0 || strlen(szFont) >= PL_MSG_DIM, done)

	pObjCtx->labelDflt.fFontSize_pts = fPts;
	strcpy(pObjCtx->labelDflt.szFont, szFont);
	pObjCtx->legendDflt.fFontSize_pts = fPts;
	strcpy(pObjCtx->legendDflt.szFont, szFont);
done:
	PL_IF_INP_ERR("PlObjFontSetDflt")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjHDCHandler - set up and release a DC for drawing an object
*
* Purpose:
*		Sets up and releases a DC to be used for object drawing operations.
*
*		Call this routine with 'bReleaseDC' == 0 to get a DC.  The setup
*		for the DC depends not only on whether 'bDotted' is 0 or 1, but
*		also on conditions in the object.
*
*		Call this routine with 'bReleaseDC' == 1 to release the DC when
*		the current drawing operation is complete.
*
* Return Value:
*		PL_OK, or
*		other error codes
*
* Notes:
* 1.	When printing, the usual XOR dotted pen is used for the selected
*		box, but a normal pen is used for other boxes--regardless of
*		edit mode.

*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
HDC PASCAL
PlObjHDCHandler(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ	*pObj,			// I the box that's being set up for, or NULL
int		bReleaseDC)		// I 1,0 to release,setup DC
{
	int		stat;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		bEditBox=0, bSelected=0;
	int		eGetRGB, eBorderType;;
	COLORREF rgbEdit, rgbNew;
	float	fPts;

	if (pObj == NULL || pObj == pObjCtx->pEditObj) {
		bEditBox = 1;
		eBorderType = PL_LINE_DOT;
		fPts = .1F;
		rgbEdit = BLACK;
	}
	else if (pObj == pObjCtx->pSelObj) {
		bSelected = 1;
		eBorderType = PL_LINE_DOT;
		fPts = .1F;
		if (pObj->eObjType == PL_OBJ_TY_BOX)
			rgbEdit = ((PL_BOX *)pObj)->props.rgbBox;
		else if (pObj->eObjType == PL_OBJ_TY_LABEL)
			rgbEdit = ((PL_LABEL *)pObj)->props.rgbLabel;
		else if (pObj->eObjType == PL_OBJ_TY_LEGEND)
			rgbEdit = ((PL_LEGEND *)pObj)->props.rgbBox;
	}
	else {
		if (pObj->eObjType == PL_OBJ_TY_BOX) {
			PL_BOX	*pBox=(PL_BOX *)pObj;
			eBorderType = pBox->props.eBorderType;
			if (eBorderType == PL_LINE)
				fPts = pBox->props.fBorderPts;
			else {
				// Win 3.1 won't handle "fat" patterned lines; allow the
				// pattern property to override the "fat line" property
				fPts = .1F;
			}
			rgbEdit = pBox->props.rgbBox;
		}
		else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
			PL_LABEL *pLabel=(PL_LABEL *)pObj;
			eBorderType = PL_LINE;
			fPts = .1F;
			rgbEdit = pLabel->props.rgbLabel;
		}
		else if (pObj->eObjType == PL_OBJ_TY_LEGEND) {
			PL_LEGEND *pLegend=(PL_LEGEND *)pObj;
			eBorderType = PL_LINE;
			fPts = pLegend->props.fBorderPts;
			rgbEdit = pLegend->props.rgbBox;
		}
	}


	if (bReleaseDC) goto release_DC;

/*KG*/	//pObjCtx->hDC = pObjCtx->hPen = pObjCtx->hPenOld = 0;

/*KG*/	pObjCtx->hDC = NULL;
/*KG*/	pObjCtx->hPen = NULL;
/*KG*/	pObjCtx->hPenOld = NULL;

	if (pObjCtx->mode.bEditMode) {
		if (pPlot->bPrinter) eGetRGB = 0;	// normal pen, rgb
		else if (bEditBox)	eGetRGB = 4;	// BLACK/WHITE, with OPAQUE XOR pen
		else if (bSelected)	eGetRGB = 4;	// rgb/^rgb, with OPAQUE XOR pen
		else				eGetRGB = 5;	// reverse color, with OPAQUE XOR pen
	}
	else					eGetRGB = 0;	// use normal pen and specified color

	stat = PlWinHDCSetup(pPlot,
			0, 0, 0,						// clip to area inside plot border
			eBorderType, fPts, eGetRGB, rgbEdit,
			&pObjCtx->hDC, &pObjCtx->hPen, &pObjCtx->hPenOld, &rgbNew);
	if (stat != PL_OK) goto done;
	pObjCtx->rgbBkgOld = GetBkColor(pObjCtx->hDC);
	if (bEditBox || bSelected)
		SetBkColor(pObjCtx->hDC, rgbNew ^ WHITE);
	pObjCtx->rgbTextOld = SetTextColor(pObjCtx->hDC, rgbNew);

done:
	return pObjCtx->hDC;

release_DC:
	if (pObjCtx->hDC != 0) {
		SetBkColor(pObjCtx->hDC, pObjCtx->rgbBkgOld);
		SetROP2(pObjCtx->hDC, R2_COPYPEN);
		SetTextColor(pObjCtx->hDC, pObjCtx->rgbTextOld);
		if (pObjCtx->hPenOld != 0) SelectObject(pObjCtx->hDC, pObjCtx->hPenOld);
		if (pObjCtx->hPen != 0) DeleteObject(pObjCtx->hPen);
		if (pPlot->hDC == 0) ReleaseDC(pPlot->hwPlot, pObjCtx->hDC);
	}
/*KG*/	//pObjCtx->hDC = pObjCtx->hPen = pObjCtx->hPenOld = 0;

/*KG*/	pObjCtx->hDC = NULL;
/*KG*/	pObjCtx->hPen = NULL;
/*KG*/	pObjCtx->hPenOld = NULL;


	return 0;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjHitTest - test for hit with an object
*
* Purpose:
*		Tests to see if a coordinate is on or within an object.  If it is,
*		o	the type of hit is deduced as one of the PL_OBJ_xxx hit types
*		o	a handle to the appropriate edit mouse pointer is stored in
*			pMouse->hPointerHit
*
*		This routine checks only pixel-oriented issues.  It totally ignores
*		issues such as 'edit mode', another object is already selected, etc.
*
* Return Value:
*		PL_OBJ_xxx if there is a hit, or
*		-1 otherwise
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_OBJ	*pObj,			// I pointer to object to test
PL_COORD *pcoMouse)		// I coordinate to test
{
	int		iHit=-1;

	if (pObj->eObjType == PL_OBJ_TY_BOX)
		iHit = PlObjBoxHitTest(pPlot, pObjCtx, (PL_BOX *)pObj, pcoMouse);
	else if (pObj->eObjType == PL_OBJ_TY_LABEL)
		iHit = PlObjLabelHitTest(pPlot, pObjCtx, (PL_LABEL *)pObj, pcoMouse);
	else if (pObj->eObjType == PL_OBJ_TY_LEGEND)
		iHit = PlObjLegendHitTest(pPlot, pObjCtx, (PL_LEGEND *)pObj, pcoMouse);

	return iHit;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjInit - initialize the PL_OBJ_CTX structure
*
* Purpose:
*		Initializes the PL_OBJ_CTX structure, prior to operations.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjInit(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	memset(pObjCtx, 0, sizeof(PL_OBJ_CTX));

	pObjCtx->hDC = 0;
	pObjCtx->pHead = pObjCtx->pTail = NULL;
	pObjCtx->pEditObj = pObjCtx->pSelObj = NULL;

	// The default object properties are set to zero by memset above.  Set the
	// exceptions:
	pObjCtx->boxDflt.eBorderType = PL_LINE;
	pObjCtx->boxDflt.fBorderPts = .1F;
	pObjCtx->boxDflt.rgbBox = GREEN;

	pObjCtx->labelDflt.fLineFarX_pts = 12.F;
	pObjCtx->labelDflt.fLineFarY_pts = 12.F;
	pObjCtx->labelDflt.fLineMeet_frac = .5F;
	pObjCtx->labelDflt.fFontSize_pts = 12.F;
	pObjCtx->labelDflt.rgbLabel = GREEN;
	strcpy(pObjCtx->labelDflt.szFont, "Times New Roman");

	pObjCtx->legendDflt.bHide = 0;
	pObjCtx->legendDflt.bBorderHide = 0;
	pObjCtx->legendDflt.bFill = 0;
	pObjCtx->legendDflt.bTitleTop = 1;
	pObjCtx->legendDflt.bTitleBold = 0;
	pObjCtx->legendDflt.bEntryTextLeft = 0;
	pObjCtx->legendDflt.fBorderPts = 1.F;
	pObjCtx->legendDflt.fFontSize_pts = 12.F;
	pObjCtx->legendDflt.rgbBox = GREEN;
	pObjCtx->legendDflt.rgbTitle = GREEN;
	strcpy(pObjCtx->legendDflt.szTitle, "");
	strcpy(pObjCtx->legendDflt.szFont, "Times New Roman");

	pObjCtx->mode.bEditMode = 0;
	pObjCtx->mode.eCreateObjType = PL_OBJ_TY_BOX;
	pObjCtx->mode.bEnbBoxNew = pObjCtx->mode.bEnbBoxMany = 1;
	pObjCtx->mode.bValuesAsPx = 1;
	pObjCtx->mode.dXFine = pObjCtx->mode.dYFine = 1.;
	pObjCtx->mode.dXCoarse = pObjCtx->mode.dYCoarse = 10.;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjKeyDown - process WM_KEYDOWN events
*
* Purpose:
*		Processes WM_KEYDOWN events.
*
* Return Value:
*		1 if the event was "used up" here, or
*		0 if the caller should process the event
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-09-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjKeyDown(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam)			// I virtual key code
{
	int		retVal=0;		// 0 (or 1) says "I didn't (or did) process it"
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_OBJ	*pSelObj=pObjCtx->pSelObj;
	int		bShift=(GetKeyState(VK_SHIFT) < 0) ? 1 : 0;
	int		bCtrl=(GetKeyState(VK_CONTROL) < 0) ? 1 : 0;
	int		bPlain=0, bCtrlShift=0;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		stat;
	double	dDeltaXPx=0., dDeltaYPx=0.;
	int		iDeltaRow=0, iDeltaCol=0;
	PL_COORD coMouse;
	float	fAnchFrac;
	double	dXVal, dYVal;
	POINT	ptMouse;
	PL_BOX	*pBox=NULL;
	PL_LABEL *pLabel=NULL;
	PL_LEGEND *pLegend=NULL;

	if (bCtrl || bShift) {
		if (bCtrl && bShift) {
			bCtrlShift = 1;
			bCtrl = bShift = 0;
		}
	}
	else
		bPlain = 1;

	if (pSelObj != NULL) {
		if (pSelObj->eObjType == PL_OBJ_TY_BOX) pBox = (PL_BOX *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LABEL) pLabel = (PL_LABEL *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LEGEND) pLegend = (PL_LEGEND *)pSelObj;
	}

/*---------------------------------------------------------------------------
*	DELETE and BACKSPACE keys for deleting the selected object.
*--------------------------------------------------------------------------*/
	if (pSelObj != NULL && (wParam == VK_DELETE || wParam == VK_BACK)) {
		if (!pObjCtx->mode.bEnbDelete && (
					(pBox != NULL && pBox->props.bNoDelete) ||
					(pLabel != NULL && pLabel->props.bNoDelete)			)) {
			// It's an object that says "don't delete me", and PL_OBJ_CTX
			// mode doesn't override.
			MessageBeep(MB_ICONEXCLAMATION);
		}
		else {
			stat = PlObjDelete(pPlot, 0, NULL);	// delete selected
			if (stat != PL_OK) {
				MessageBeep(MB_ICONEXCLAMATION);
				goto done;
			}
		}
		retVal = 1;			// event has been processed
	}
/*---------------------------------------------------------------------------
*	Arrow keys for moving and resizing the selected object, PROVIDED THAT THE
*	MOUSE POINTER HAS HIT THE OBJECT.
*--------------------------------------------------------------------------*/
	if (pMouse->bHitSelected) {
		int		bDoEdit=0;
		if (bShift) {		dDeltaXPx = pObjCtx->mode.dXCoarse;
							dDeltaYPx = pObjCtx->mode.dYCoarse;		}
		else if (bCtrl) {	iDeltaRow = 1;
							iDeltaCol = 1;							}
		else {				dDeltaXPx = pObjCtx->mode.dXFine;
							dDeltaYPx = pObjCtx->mode.dYFine;		}
		if (wParam == VK_LEFT) {
			dDeltaXPx *= -1.; dDeltaYPx = 0.;
			iDeltaCol *= -1; iDeltaRow = 0;
			bDoEdit = 1;
		}
		else if (wParam == VK_UP) {
			dDeltaXPx = 0.; dDeltaYPx *= -1.;
			iDeltaCol = 0; iDeltaRow *= -1;
			bDoEdit = 1;
		}
		else if (wParam == VK_RIGHT) {
			dDeltaYPx = 0.;
			iDeltaRow = 0;
			bDoEdit = 1;
		}
		else if (wParam == VK_DOWN) {
			dDeltaXPx = 0.;
			iDeltaCol = 0;
			bDoEdit = 1;
		}
		if (bDoEdit) {
			if (!PlObjEditStartIsOK(pPlot)) goto done;
			wParam = PlObjKeyboardMove(pPlot, pSelObj, pObjCtx->mode.bValuesAsPx,
						dDeltaXPx, dDeltaYPx, iDeltaRow, iDeltaCol);
			retVal = 1;		// the event is "used up"
			// The 'wParam' returned by the routine determines mouse pointer
			// positioning on the object.  Regardless of the state of the
			// CTRL and SHIFT keys in getting here, we'll claim that neither
			// is down, to satisfy the test down below.
			bPlain = 1;
			bShift = bCtrl = 0;
		}
	}
/*---------------------------------------------------------------------------
*	N and P keys to select the next and previous, respectively, objects in
*	the object list.
*--------------------------------------------------------------------------*/
	if (bPlain && pObjCtx->nObjects > 0 && (wParam == 'N' || wParam == 'P')) {
		if (pSelObj != NULL) {
			stat = PlObjSelect(pPlot, 0, NULL, 0);	// allow callback; deselect
			if (stat != PL_OK) {
				// Callback want's to veto the deselect.
				goto done;
			}
			if (wParam == 'N') {
				pSelObj = pSelObj->pNext;
				if (pSelObj == NULL)
					pSelObj = pObjCtx->pHead;
			}
			else {
				pSelObj = pSelObj->pPrev;
				if (pSelObj == NULL)
					pSelObj = pObjCtx->pTail;
			}
		}
		else if (wParam == 'N')
			pSelObj = pObjCtx->pHead;
		else
			pSelObj = pObjCtx->pTail;
		if (pSelObj == NULL) goto done;
		pBox = NULL;
		pLabel = NULL;
		pLegend = NULL;
		if (pSelObj->eObjType == PL_OBJ_TY_BOX) pBox = (PL_BOX *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LABEL) pLabel = (PL_LABEL *)pSelObj;
		else if (pSelObj->eObjType == PL_OBJ_TY_LEGEND) pLegend = (PL_LEGEND *)pSelObj;
		if (pBox != NULL && pBox->props.bNoEdit) goto done;
		if (pLabel != NULL && pLabel->props.bNoEdit) goto done;
		if (pLegend != NULL && pLegend->props.bNoEdit) goto done;
		stat = PlObjSelect(pPlot, 0, pSelObj, 1);	// allow callback; select
		if (stat != PL_OK) {
			// Callback want's to veto the select.
			goto done;
		}
		retVal = 1;			// event has been processed
		wParam = 'C';		// default pointer position for new select
	}
/*---------------------------------------------------------------------------
*	CTRL+Z and CTRL+R keys to zoom and restore.
*--------------------------------------------------------------------------*/
	if (pObjCtx->nObjects > 0 && bCtrl && (wParam == 'Z' || wParam == 'R')) {
		float	fZoomH, fZoomV;
		if (wParam == 'Z') {
			if (pBox != NULL)
				PlObjBoxZoom(pPlot, NULL, 1, &fZoomH, &fZoomV, NULL, NULL);
		}
		else
			PlObjBoxZoom(pPlot, NULL, 0, &fZoomH, &fZoomV, NULL, NULL);
	}
/*---------------------------------------------------------------------------
*	CTRL+E shows plot object edit 'dialog'.
*--------------------------------------------------------------------------*/
	if (pLabel != NULL && bCtrl && wParam == 'E') {
		wParam = PlObjEditDialog(pPlot, 0, wParam);
		bCtrl = 0;
	}
/*---------------------------------------------------------------------------
*	F1 shows plot object edit 'dialog' as a help window.
*--------------------------------------------------------------------------*/
	if (wParam == VK_F1) {
		HCURSOR	hCurOld;
		pPlot->pObjCtx->bObjEditDone = 0;
		hCurOld = SetCursor(LoadCursor(NULL, IDC_ARROW));
		PlObjWinDialog(pPlot, pPlot->hwPlot, 1, NULL);
		while (!pPlot->pObjCtx->bObjEditDone)
			WuDoEvents();
		if (hCurOld != 0) SetCursor(hCurOld);
	}
/*---------------------------------------------------------------------------
*	CTRL+P prints the plot, provided there is a plot callback function.
*--------------------------------------------------------------------------*/
	if (bCtrl && wParam == 'P') {
		if (pPlot->pPlotFn != NULL)
			PlPrtPrintSimple(pPlot, "plot", 2.F, 1.F,1.F,1.F,1.F);
	}
/*---------------------------------------------------------------------------
*	B, T, and C keys to move the mouse pointer onto the bottom/left or
*	top/right corners or into the center of the selected object.
*--------------------------------------------------------------------------*/
	if (!bCtrl && pSelObj != NULL) {
		if (wParam == 'B' || wParam == 'T' || wParam == 'C') {
			PlObjKeyboardPlaceMouse(pPlot, wParam, pSelObj, &coMouse, &fAnchFrac);
			goto set_mouse_pos;
		}
	}

done:
	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
	if (pObjCtx->bKbEditActive && pPlot->pObjectFn != NULL) {
		// Tell callback that size or position has changed.
		int		eEvent;

		if (pObjCtx->bEditIsMove)		eEvent = PL_OBJ_EV_MOVE;
		else							eEvent = PL_OBJ_EV_RESIZE;
		pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, pSelObj,
									PL_OBJ_EV_EDIT, eEvent, PL_OBJ_EV_COORD);
	}
	pObjCtx->bKbEditActive = 0;
	return retVal;
set_mouse_pos:
	// If the new pointer position isn't visible, scroll the plot so that
	// it is.  If the pointer is between plot area rows, the plot will only
	// be scrolled in the horizontal direction.  And similarly when the
	// pointer is between plot area columns.
	if (pLegend != NULL) {
		ptMouse.x = coMouse.x.iVisPx;
		ptMouse.y = coMouse.y.iVisPx;
		goto set_mouse_px;
	}
	stat = PlAreaArraySelect(pPlot, coMouse.y.iRowCol, coMouse.x.iRowCol);
	if (stat == PL_OK) {
		dXVal = PlWinCvtXPixToXVal(pPlot, coMouse.x.dVirtPx);
		dYVal = PlWinCvtYPixToYVal(pPlot, coMouse.y.dVirtPx);
		if (PlVisibleCheck(pPlot, dXVal, dYVal) == 0) {
			stat = PlWinScroll_pos(pPlot, dXVal, dYVal, fAnchFrac, fAnchFrac);
			if (stat != PL_OK) goto done;
		}
	}
	else if ((stat = PlAreaArraySelect(pPlot, coMouse.y.iRowCol, colSave)) == PL_OK) {
		dYVal = PlWinCvtYPixToYVal(pPlot, coMouse.y.dVirtPx);
		if (PlVisibleCheck(pPlot, dXVal, dYVal) == 0) {
			stat = PlWinScroll_pos(pPlot, pPlot->pXAxis->dDValAnchor, dYVal,
										pPlot->pXAxis->fAnchorFrac, fAnchFrac);
			if (stat != PL_OK) goto done;
		}
	}
	else if ((stat = PlAreaArraySelect(pPlot, rowSave, coMouse.x.iRowCol)) == PL_OK) {
		dXVal = PlWinCvtXPixToXVal(pPlot, coMouse.x.dVirtPx);
		if (PlVisibleCheck(pPlot, dXVal, dYVal) == 0) {
			stat = PlWinScroll_pos(pPlot, dXVal, pPlot->pYAxis->dDValAnchor,
										pPlot->pYAxis->fAnchorFrac, fAnchFrac);
			if (stat != PL_OK) goto done;
		}
	}
	else
		goto done;
	ptMouse.x = (int)PlWinCvtXValToXPix(pPlot, dXVal);
	ptMouse.y = (int)PlWinCvtYValToYPix(pPlot, dYVal);
set_mouse_px:
	ClientToScreen(pPlot->hwPlot, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
	// Let the pointer position change happen while bKbEditActive is 1, so
	// that the mouse move won't be passed on (by PlWinMousePreProcess) to
	// the callback function.
	WuDoEvents();
	retVal = 1;		// the event is "used up"
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjKeyboardMove - move the sides of an object
*
* Purpose:
*		Moves the sides of an object, and synchronizes the mouse pointer
*		to "keep up" with whatever motion occurred.
*
*		If the move is invalid, a MessageBeep is done and the move is
*		aborted.
*
* Return Value:
*		character indicating where mouse pointer should be positioned
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-06-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjKeyboardMove(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ	*pObj,			// IO the object to move
int		bDeltaPx,		// I 1 if dDeltaX and dDeltaY are pixels
double	dDeltaX,		// I amount to add to X
double	dDeltaY,		// I amount to add to Y
int		iDeltaRow,		// I amount to add to row number
int		iDeltaCol)		// I amount to add to column number
{
	int		iPointerPos;

	if (pObj->eObjType == PL_OBJ_TY_BOX) {
		iPointerPos = PlObjBoxKeyboardMove(pPlot, (PL_BOX *)pObj,
							bDeltaPx, dDeltaX, dDeltaY, iDeltaRow, iDeltaCol);
	}
	else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
	}

	return iPointerPos;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjKeyboardPlaceMouse - place the mouse pointer on an object
*
* Purpose:
*		Places the mouse pointer on an object.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_OBJ	*pObj,			// IO the object to move
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac)	// O anchor fraction if scroll change needed
{
	if (pObj->eObjType == PL_OBJ_TY_BOX) {
		PlObjBoxKeyboardPlaceMouse(pPlot,
							wParam, (PL_BOX *)pObj, pcoMouse, pfAnchFrac);
	}
	else if (pObj->eObjType == PL_OBJ_TY_LABEL) {
		PlObjLabelKeyboardPlaceMouse(pPlot,
							wParam, (PL_LABEL *)pObj, pcoMouse, pfAnchFrac);
	}
	else if (pObj->eObjType == PL_OBJ_TY_LEGEND) {
		PlObjLegendKeyboardPlaceMouse(pPlot,
							wParam, (PL_LEGEND *)pObj, pcoMouse, pfAnchFrac);
	}
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLabelCreate - create a PL_LABEL in a plot area
*
* Purpose:
*		Creates a new PL_LABEL in the currently selected plot area.
*		A PL_LABEL object consists of:
*		o	text (which can optionally be modified by the operator)
*		o	an optional box surrounding the text
*		o	an optional line connecting the text/box with a data point
*
*		The new PL_LABEL is created with the default PL_LABEL properties
*		(which can be modified with PlObjLabelPropsDflt).  The label
*		properties can be changed at any time with PlObjLabelProps.
*		In order to allow the operator to change the PL_LABEL text,
*		edit mode must be 1.
*
*		See PlObjLabelProps for descriptions of PL_LABEL properties.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the create, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 1 but object edit mode is zero, then bSelect is
*		ignored.
* 2.	If bNoCallback is 0, then the object callback function (if it exists)
*		is called with PL_OBJ_EV_EDIT indicating "create complete".  (The
*		function is not called for "create start".)
* 4.	This routine can be used regardless of whether bEditMode is 1.
*
* See Also:
*		PlObjLabelCreateAtPlotCursor, PlObjLabelCreateGeneral
*		PlObjLabelPropsDflt, PlObjLabelProps
*		PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLabelCreate(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 if object callback fn is not to be called
PL_LABEL **ppLabel,		// O place to store pointer to new PL_LABEL, or NULL
int		bSelect,		// I 1 if the label is to be selected
double	dXVal,			// I X data value for PL_LABEL
double	dYVal,			// I Y data value for PL_LABEL
float	fLineFarX_pts,	// I datum to far end of line, X distance, pts
float	fLineFarY_pts,	// I datum to far end of line, Y distance, pts
float	fLineMeet_frac,	// I line meets at horiz frac of text length
const char *szText,		// I text for PL_LABEL
COLORREF rgbLabel)		// I color to use for drawing PL_LABEL
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LABEL *pLabel;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		dXVal must be within the range of X values for the plot area
* 3		dYVal must be within the range of Y values for the plot area
* 4		the length of szText must be less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	PL_INP_CHK(2, !PlObjValIsInRange(pPlot->pXAxis, dXVal), done)
	PL_INP_CHK(3, !PlObjValIsInRange(pPlot->pYAxis, dYVal), done)
	if (szText == NULL) szText = "";
	PL_INP_CHK(4, strlen(szText) >= PL_MSG_DIM, done)

	if (!pObjCtx->mode.bEditMode)
		bSelect = 0;

	if (bSelect) {
		// Deselect existing selection, if any.
		retStat = PlObjSelect(pPlot, bNoCallback, NULL, 0);
		if (retStat != PL_OK) goto done;
	}
	if ((pLabel = (PL_LABEL *)malloc(sizeof(PL_LABEL))) == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}

	PlObjLabelInit(pPlot, pLabel);
	pLabel->iRow = pPlot->iAxisRow;
	pLabel->iCol = pPlot->iAxisCol;
	pLabel->dXVal = dXVal;
	pLabel->dYVal = dYVal;
	pLabel->props.fLineFarX_pts = fLineFarX_pts;
	pLabel->props.fLineFarY_pts = fLineFarY_pts;
	pLabel->props.fLineMeet_frac = fLineMeet_frac;
	pLabel->props.rgbLabel = rgbLabel;
	strcpy(pLabel->szText, szText);

	if (!bNoCallback && pPlot->pObjectFn != NULL) {
		int		stat;
		// Tell callback that create is about to complete.
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, (PL_OBJ *)pLabel,
						PL_OBJ_EV_EDIT, PL_OBJ_EV_CREATE, PL_OBJ_EV_DONE);
		if (stat == 0) {
			// Callback want's to veto.
			free(pLabel);
			goto done;
		}
	}

	DLIST_APPEND((PL_OBJ *)pLabel, pObjCtx->pHead, pObjCtx->pTail);
	pObjCtx->nObjects++;

	if (bSelect)
		pObjCtx->pSelObj = (PL_OBJ *)pLabel;

	retStat = PlObjLabelPaint(pPlot, pLabel, bSelect);

	if (ppLabel != NULL) *ppLabel = pLabel;

done:
	PL_IF_INP_ERR("PlObjLabelCreate")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelCvtCoordToLabel - convert PL_COORD set to PL_LABEL
*
* Purpose:
*		Converts a set of PL_COORDs to a PL_LABEL.
*
*		This routine:
*		o	stores, without validating, the bValOK, dVal, and iRowCol
*			items
*		o	transposes left/right sides during the store, if necessary
*		o	transposes top/bottom sides during the store, if necessary
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine assumes that the conversion to dVal has already
*		been done in the PL_COORD sets.  (This may mean that one or both
*		bValOK flags is zero.)
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLabelCvtCoordToLabel(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pcoordLB,		// I the left bottom coordinate
PL_COORD *pcoordRT,		// I the right top coordinate
PL_LABEL *pLabel)		// IO the resulting PL_LABEL
{


}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelCvtLabelToPts - convert PL_LABEL to set of coordinates
*
* Purpose:
*		Converts a PL_LABEL to the corresponding set of PL_COORDs.  There is
*		a pair of PL_COORDs for the line from the data point to the text,
*		and another pair of PL_COORDs for the text/box.  Each pair contains:
*		o	the 'virtual' pixel coordinates
*		o	the 'visible' pixel coordinates
*		o	the coordinates to use when drawing the line/text/box
*		o	the coordinates of a clip region to use when drawing
*		o	flags indicating whether the dVal's are in range
*		o	flags indicating whether the items are visible
*
*		The 'bSelected' argument is used in conjunction with the .bSnapXxx
*		property of the label, so that this routine can take into account that
*		.bSnapXxx PL_LABELs have a special visual behavior.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLabelCvtLabelToPts(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LABEL *pLabel,		// I the PL_LABEL to convert
int		bSelected,		// I 1 if the label is selected
int		*pbLineVis,		// O 1 if the line is visible
int		*pbTextVis,		// O 1 if the text is visible
POINT	*pptNear,		// O the resulting near coordinate for line
POINT	*pptFar,		// O the resulting far coordinate for line
POINT	*pptTextLT,		// O the resulting left bottom coordinate for text
POINT	*pptTextRB)		// O the resulting right top coordinate for text
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		stat;
	float	fPts, fTemp;
	int		bSnapToTop=0, bSnapToBottom=0;
	int		iLineWid_px, iLineHt_px, iTextWid_px, iTextHt_px;
	double	dNearX_px, dNearY_px, dFarX_px, dFarY_px;
	double	dTextL_px, dTextT_px, dTextR_px, dTextB_px;
	int		bLineVis=0, bTextVis=0;
	WU_QUAD	quadText;
	HDC		hDC;
	float	fTextDeg;

	if (pLabel->props.bTextVert)		fTextDeg = 90.F;
	else								fTextDeg = 0.F;

	if (!bSelected) {
		if (pLabel->props.bSnapToTop)			bSnapToTop = 1;
		else if (pLabel->props.bSnapToBottom)	bSnapToBottom = 1;
	}

	stat = PlAreaArraySelect(pPlot, pLabel->iRow, pLabel->iCol);
	if (stat != PL_OK) goto done;

	// Get the preliminary height, width, and coordinates for the line.
	// (The height and width of the line in this context refer to the
	// height and width of the rect with the line as the diagonal.)
	dNearX_px = PlWinCvtXValToXPix(pPlot, pLabel->dXVal);
	fPts = fTemp = pLabel->props.fLineFarX_pts;
	if (fTemp < 0.F) fTemp = -fTemp;
	if (fTemp == 0)
		iLineWid_px = 0;
	else
		iLineWid_px = PlWinCvtPtsToPix(pPlot, 1, 0, fTemp);
	if (fPts < 0.F) iLineWid_px = -iLineWid_px;
	dFarX_px = dNearX_px + iLineWid_px;

	dNearY_px = PlWinCvtYValToYPix(pPlot, pLabel->dYVal);
	fPts = fTemp = pLabel->props.fLineFarY_pts;
	if (fTemp < 0.F) fTemp = -fTemp;
	if (fTemp == 0)
		iLineHt_px = 0;
	else
		iLineHt_px = PlWinCvtPtsToPix(pPlot, 1, 0, fTemp);
	if (fPts < 0.F) iLineHt_px = -iLineHt_px;
	dFarY_px = dNearY_px - iLineHt_px;	// need to subtract, since '+' is up

	// See if the data point is visible.  (This check is elaborated later
	// when both ends of the line have been determined.  The data point
	// visibility is used for some of the intermediate processing.)
	if (dNearX_px >= pPlot->pXAxis->pxB && dNearX_px <= pPlot->pXAxis->pxT &&
		dNearY_px <= pPlot->pYAxis->pxB && dNearY_px >= pPlot->pYAxis->pxT) {
			bLineVis = 1;
	}
	if (!bLineVis)
		bSnapToTop = bSnapToBottom = 0;

	// Get the height and width for the text.
	if ((hDC = PlObjHDCHandler(pPlot, (PL_OBJ *)pLabel, 0)) != 0) {
		const char *szText;
		szText = pLabel->szText;
		if (szText[0] == '\0') szText = "  ";
		stat = WuPrtText_font(hDC, szText,
				pPlot->pXAxis->pxB, pPlot->pYAxis->pxT, PRT_TL,
				pLabel->props.szFont, pLabel->props.fFontSize_pts,
				pLabel->props.bTextBold, fTextDeg, 0, 1, NULL, &quadText);
		PlObjHDCHandler(pPlot, (PL_OBJ *)pLabel, 1);		// release
		if (stat == 1) goto gdi_error;
	}
	if (pLabel->props.bTextVert) {
		iTextWid_px = 3 + (int)(quadText.x[2] - quadText.x[0]);
		iTextHt_px = 3 + (int)(quadText.y[0] - quadText.y[2]);
	}
	else {
		iTextWid_px = 3 + (int)(quadText.x[2] - quadText.x[0]);
		iTextHt_px = 3 + (int)(quadText.y[2] - quadText.y[0]);
	}

	// If the text is being 'snapped' to the top or bottom of the data
	// area, adjust the 'far' Y for the line.  If the data point is already
	// too close to the edge of the data area for the text to appear, skip
	// this step.  ('Too close' is interpreted as: allowing 2 pixels
	// between the text and the data point.)
	if (bSnapToTop && (dNearY_px - iTextHt_px - 2) > pPlot->pYAxis->pxT)
		dFarY_px = pPlot->pYAxis->pxT + iTextHt_px;
	else if (bSnapToBottom && (dNearY_px + iTextHt_px + 2) < pPlot->pYAxis->pxB)
		dFarY_px = pPlot->pYAxis->pxB - iTextHt_px;

	// Set the coordinates for the text.
	dTextL_px = dFarX_px - pLabel->props.fLineMeet_frac * iTextWid_px;
	dTextR_px = dTextL_px + iTextWid_px - 1.;
	if (dFarY_px < dNearY_px) {
		dTextB_px = dFarY_px;
		dTextT_px = dTextB_px - iTextHt_px + 1.;
	}
	else {
		dTextT_px = dFarY_px;
		dTextB_px = dTextT_px + iTextHt_px - 1.;
	}

	// Clip the line to the data area (and determine whether any of it is
	// visible).
	stat = WuClipIt(1, pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
					pPlot->pXAxis->pxT, pPlot->pYAxis->pxB,
					&dNearX_px, &dNearY_px, &dFarX_px, &dFarY_px);
	if (stat != 0)
		bLineVis = 1;
	if (dTextR_px >= pPlot->pXAxis->pxB && dTextL_px <= pPlot->pXAxis->pxT &&
			dTextB_px >= pPlot->pYAxis->pxT && dTextT_px <= pPlot->pYAxis->pxB) {
		bTextVis = 1;
	}

done:
	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
	*pbLineVis = bLineVis;
	*pbTextVis = bTextVis;
	pptNear->x = (int)dNearX_px;
	pptNear->y = (int)dNearY_px;
	pptFar->x = (int)dFarX_px;
	pptFar->y = (int)dFarY_px;
	pptTextLT->x = (int)dTextL_px;
	pptTextLT->y = (int)dTextT_px;
	pptTextRB->x = (int)dTextR_px;
	pptTextRB->y = (int)dTextB_px;
	return;
gdi_error:
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelHitTest - test for hit with a PL_LABEL
*
* Purpose:
*		Tests to see if a coordinate is on or within a PL_LABEL.  If it is,
*		o	the type of hit is deduced as one of the PL_OBJ_xxx hit types
*		o	a handle to the appropriate edit mouse pointer is stored in
*			pMouse->hPointerHit
*
*		This routine checks only pixel-oriented issues.  It totally ignores
*		issues such as 'edit enable', another object is already selected, etc.
*
* Return Value:
*		PL_OBJ_xxx if there is a hit, or
*		-1 otherwise
*
* Notes:
* 1.	PL_OBJ_C is returned only if the coordinate is inside a selected
*		PL_LABEL.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLabelHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_LABEL *pLabel,		// I pointer to PL_LABEL to test
PL_COORD *pcoMouse)		// I coordinate to test
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		iHit=-1, bHitText=0;
	int		bLineVis, bTextVis;
	POINT	ptNear, ptFar, ptTextLT, ptTextRB;
	int		bXL, bXR, bYT, bYB, bXBet, bYBet;
	int		bXHit=0, bYHit=0;
	int		bSelected=(PL_OBJ *)pLabel == pObjCtx->pSelObj;
	int		iMouseX=pcoMouse->x.iVisPx;
	int		iMouseY=pcoMouse->y.iVisPx;

	bXL = bXR = bXBet = bYT = bYB = bYBet = 0;

	if (pcoMouse->x.iRowCol != pLabel->iCol ||
								pcoMouse->y.iRowCol != pLabel->iRow) {
		goto done;
	}

	// Get the pixel values corresponding to the line and text.
	PlObjLabelCvtLabelToPts(pPlot, pLabel, bSelected, &bLineVis, &bTextVis,
								&ptNear, &ptFar, &ptTextLT, &ptTextRB);

	// Check to see if the mouse coordinate hits the line connecting the
	// text with the data point, or if it hits the actual text.  A 'text'
	// hit occurs only if there is no 'line' hit.
                         
	if (bLineVis) {
		int		iMinPx, iMaxPx;
		iMinPx = PL_MIN(ptNear.x, ptFar.x);
		iMaxPx = PL_MAX(ptNear.x, ptFar.x);
		if (fabs((double)(iMouseX - iMinPx)) <= PL_OBJ_HIT_PX) bXHit = bXL = 1;
		if (fabs((double)(iMouseX - iMaxPx)) <= PL_OBJ_HIT_PX) bXHit = bXR = 1;
		if (!bXL && !bXR) {
			if (iMouseX > iMinPx && iMouseX < iMaxPx)		bXHit = bXBet = 1;
		}
		iMinPx = PL_MIN(ptNear.y, ptFar.y);
		iMaxPx = PL_MAX(ptNear.y, ptFar.y);
		if (fabs((double)(iMouseY - iMinPx)) <= PL_OBJ_HIT_PX) bYHit = bYB = 1;
		if (fabs((double)(iMouseY - iMaxPx)) <= PL_OBJ_HIT_PX) bYHit = bYT = 1;
		if (!bYB && !bYT) {
			if (iMouseY > iMinPx && iMouseY < iMaxPx)		bYHit = bYBet = 1;
		}
	}
	if (!(bXHit && bYHit) && bTextVis) {
		if (iMouseX >= ptTextLT.x && iMouseX <= ptTextRB.x &&
				iMouseY >= ptTextLT.y && iMouseY <= ptTextRB.y) {
			bXBet = bYBet = 1;
			bHitText = 1;
		}
	}

	if (bXBet && bYBet)
								iHit = PL_OBJ_C;
	else if (bXL && bXR) {
		if (bYT)				iHit = PL_OBJ_TR;
		else if (bYBet)			iHit = PL_OBJ_C;
		else if (bYB)			iHit = PL_OBJ_BL;
	}
	else if (bYB && bYT) {
		if (bXR)				iHit = PL_OBJ_TR;
		else if (bXBet)			iHit = PL_OBJ_C;
		else if (bXL)			iHit = PL_OBJ_BL;
	}
	else if (bXL) {
		if (bYT)				iHit = PL_OBJ_TL;
		else if (bYB)			iHit = PL_OBJ_BL;
		else if (bYBet)			iHit = PL_OBJ_L;
	}
	else if (bXR) {
		if (bYT)				iHit = PL_OBJ_TR;
		else if (bYB)			iHit = PL_OBJ_BR;
		else if (bYBet)			iHit = PL_OBJ_R;
	}
	else if (bXBet) {
		if (bYT)				iHit = PL_OBJ_T;
		else if (bYB)			iHit = PL_OBJ_B;
	}
	if (iHit == PL_OBJ_C) {
		if (pLabel->props.bNoMove && !pObjCtx->mode.bEnbMove)
			pMouse->bUseNOTPointer = 1;
	}

#define CHOOSE_HIT_POINTER(idc_cur) pMouse->hPointerHit = LoadCursor(NULL, idc_cur)

	if (bHitText)
		CHOOSE_HIT_POINTER(IDC_IBEAM);
	else
		iHit = -1;
	goto done;
	switch (iHit) {
		case PL_OBJ_C:		CHOOSE_HIT_POINTER(IDC_SIZE); break;
		case PL_OBJ_TL:
		case PL_OBJ_BR:		CHOOSE_HIT_POINTER(IDC_SIZENWSE); break;
		case PL_OBJ_TR:
		case PL_OBJ_BL:		CHOOSE_HIT_POINTER(IDC_SIZENESW); break;
		case PL_OBJ_T:
		case PL_OBJ_B:		CHOOSE_HIT_POINTER(IDC_SIZENS); break;
		case PL_OBJ_R:
		case PL_OBJ_L:		CHOOSE_HIT_POINTER(IDC_SIZEWE); break;
	}

//	if (iHit == PL_OBJ_C) pMouse->hPointerHit = pMouse->hPointerMove;
//	else if (iHit >= 0) pMouse->hPointerHit = pMouse->hPointerSize;
done:
	return iHit;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelInit - initialize a PL_LABEL
*
* Purpose:
*		Initializes a PL_LABEL:
*		o	sets the properties to default values
*		o	sets the sides to "invalid"
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLabelInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LABEL *pLabel)		// IO pointer to PL_LABEL
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	memset(pLabel, 0, sizeof(PL_LABEL));
	pLabel->eObjType = PL_OBJ_TY_LABEL;
	pLabel->props = pObjCtx->labelDflt;
	strcpy(pLabel->szText, "label");
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelKeyboardPlaceMouse - place the mouse pointer on a PL_LABEL
*
* Purpose:
*		Places the mouse pointer on a PL_LABEL.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-30-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLabelKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_LABEL *pLabel,		// I the object to place mouse on
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac)	// O anchor fraction if scroll change needed
{
	POINT	ptNear, ptFar, ptTextLT, ptTextRB;
	int		bLineVis, bTextVis;

	// Get the pixel values corresponding to the line and text.
	PlObjLabelCvtLabelToPts(pPlot, pLabel, 1, &bLineVis, &bTextVis,
								&ptNear, &ptFar, &ptTextLT, &ptTextRB);

	pcoMouse->x.bValOK = pcoMouse->y.bValOK = 1;
	pcoMouse->x.iRowCol = pLabel->iCol;
	pcoMouse->y.iRowCol = pLabel->iRow;
	pcoMouse->x.dVal = pLabel->dXVal;
	pcoMouse->y.dVal = pLabel->dYVal;

	if (wParam == 'B' || wParam == 'T') {
		if (wParam == 'B') {
			pcoMouse->x.dVirtPx = ptNear.x;
			pcoMouse->y.dVirtPx = ptNear.y;
			*pfAnchFrac = .5F;
		}
		else {
			pcoMouse->x.dVirtPx = (ptTextLT.x + ptTextRB.x) / 2;
			pcoMouse->y.dVirtPx = (ptTextLT.y + ptTextRB.y) / 2;
			*pfAnchFrac = .5F;
		}
	}
	else if (wParam == 'C') {
		pcoMouse->x.dVirtPx = (ptNear.x + ptFar.x) / 2;
		pcoMouse->y.dVirtPx = (ptNear.y + ptFar.y) / 2;
		*pfAnchFrac = .5F;
	}
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelPaint - paint a PL_LABEL
*
* Purpose:
*		Paints a PL_LABEL.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLabelPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_LABEL *pLabel,		// I pointer to PL_LABEL to draw
int		bSelected)		// I 1 if the label is selected
{
	int		retStat=PL_OK;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	HDC		hDC;
	HRGN	rgn=0;
	HBRUSH	hBrush=0, hBrushOld=0;
	COLORREF rgbFill, rgbBkOld;
	COLORREF rgbBkText, rgbText;
	int		bLineVis, bTextVis;
	POINT	ptNear, ptFar, ptTextLT, ptTextRB;
	int		iXDrawPxL, iXDrawPxR, iYDrawPxT, iYDrawPxB;
	int		stat;
	const char *szText;
	float	fTextDeg;

	if (pLabel->props.bTextVert)		fTextDeg = 90.F;
	else								fTextDeg = 0.F;

	// Get the pixel values corresponding to the line and text.
	PlObjLabelCvtLabelToPts(pPlot, pLabel, bSelected, &bLineVis, &bTextVis,
								&ptNear, &ptFar, &ptTextLT, &ptTextRB);

	retStat = PlAreaArraySelect(pPlot, pLabel->iRow, pLabel->iCol);
	if (retStat != PL_OK) goto done;
	hDC = PlObjHDCHandler(pPlot, (PL_OBJ *)pLabel, 0);
	if (hDC != 0) {
		if (bLineVis && (bSelected || !pLabel->props.bLineHide)) {
			// Draw the line.
			rgn = CreateRectRgn(pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
									pPlot->pXAxis->pxT+1, pPlot->pYAxis->pxB+1);
			if (rgn == 0) goto gdi_error;
			stat = SelectClipRgn(hDC, rgn);
			if (stat == ERROR) goto gdi_error;
			DeleteObject(rgn);
			// Windows QUIRK: drawing "backwards" results in drawing an
			// extra pixel.  Work around the quirk.
			if (ptNear.y > ptFar.y) {
				iXDrawPxL = ptNear.x;
				iYDrawPxB = ptNear.y;
				iXDrawPxR = ptFar.x;
				iYDrawPxT = ptFar.y;
			}
			else {
				iXDrawPxL = ptFar.x;
				iYDrawPxB = ptFar.y;
				iXDrawPxR = ptNear.x;
				iYDrawPxT = ptNear.y;
			}
			MoveToEx(hDC, iXDrawPxL, iYDrawPxB, NULL);
			LineTo(hDC, iXDrawPxR, iYDrawPxT);
		}

		if (bTextVis) {
			// Draw the box and text.
			rgn = CreateRectRgn(pPlot->pXAxis->pxB, pPlot->pYAxis->pxT,
									pPlot->pXAxis->pxT+1, pPlot->pYAxis->pxB+1);
			if (rgn == 0) goto gdi_error;
			stat = SelectClipRgn(hDC, rgn);
			if (stat == ERROR) goto gdi_error;
			DeleteObject(rgn);
			iXDrawPxL = ptTextLT.x;
			iYDrawPxT = ptTextLT.y;
			if (pLabel->props.bTextVert) {
				iXDrawPxR = ptTextRB.x;
				iYDrawPxB = 3 + ptTextRB.y;
			}
			else {
				iXDrawPxR = 3 + ptTextRB.x;
				iYDrawPxB = ptTextRB.y;
			}
			// The 'box' is always drawn filled, using the background color.
			rgbFill = pObjCtx->rgbBkgOld;
			if (pObjCtx->mode.bEditMode)
				rgbFill ^= pObjCtx->rgbBkgOld;
			if ((hBrush = CreateSolidBrush(rgbFill)) != 0)
/*KG*/			//hBrushOld = SelectObject(hDC, hBrush);
/*KG*/			hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
			if (!pLabel->props.bBoxHide)
				Rectangle(hDC, iXDrawPxL-1, iYDrawPxT-1, 1+iXDrawPxR, 1+iYDrawPxB);
			if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
			if (hBrush != 0) DeleteObject(hBrush);
			hBrush = hBrushOld = 0;
			rgbBkText = rgbFill;
			rgbText = pLabel->props.rgbLabel;
			szText = pLabel->szText;
			if (szText[0] == '\0') szText = "  ";
			PlObjTextColor(pPlot, 0, rgbFill, rgbBkText, &rgbText);
			rgbBkOld = SetBkColor(hDC, rgbBkText);
			SetTextColor(hDC, rgbText);
			if (pPlot->bPrinter) {
				stat = WuPrtText_font(hDC, szText,
							iXDrawPxL, iYDrawPxT,
							pLabel->props.bTextVert ? PRT_TR : PRT_TL,
							pLabel->props.szFont, pLabel->props.fFontSize_pts,
							pLabel->props.bTextBold,
							fTextDeg, 0, 0, NULL, NULL);
			}
			else {
				stat = WuPrtText_font_ROP2(hDC, szText, iXDrawPxL, iYDrawPxT,
							pLabel->props.szFont, pLabel->props.fFontSize_pts,
							pLabel->props.bTextBold, pLabel->props.bTextVert);
			}
			if (stat == 1) goto gdi_error;
			SetBkColor(hDC, rgbBkOld);
		}
	}
	PlObjHDCHandler(pPlot, (PL_OBJ *)pLabel, 1);		// release

done:
	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
	return retStat;
gdi_error:
	if (rgn != 0) DeleteObject(rgn);
	return PL_GDI_FAIL;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLabelPaint_edit - paint the PL_LABEL "rubber band"
*
* Purpose:
*		Paints the "rubber band" for the PL_LABEL being edited.  Since the
*		drawing uses XOR, alternate calls to this routine with the same
*		edit object alternately draw and erase the edit object.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLabelPaint_edit(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_COORD *pcoLB=&pObjCtx->coEditLBCurr;
	PL_COORD *pcoRT=&pObjCtx->coEditRTCurr;

	return;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLabelProps - set or gets PL_LABEL properties
*
* Purpose:
*		Sets the properties for a PL_LABEL or retrieves the properties from
*		a PL_LABEL.
*
*		When the 'bSet' argument is 0, the current flag settings are
*		stored in the caller's PL_LABEL_PROPS structure.
*
*		When the 'bSet' argument is 1,
*		o	the properties from the caller's PL_LABEL_PROPS structure are
*			stored in the specified label
*		o	if one or more of the visual properties of the PL_LABEL change,
*			then the label will be repainted, unless the 'bNoPaint' argument
*			is 1.
*
*		The PL_OBJ_CTX enable flags provide global control on what the
*		operator is allowed to do using the mouse and keyboard.  The
*		following table illustrates the behavior for deleting an object;
*		the other edit operations operate in a similar way.
*
*		----- PL_OBJ_CTX -----  PL_LABEL_PROPS  operator delete is
*		bEditMode   bEnbDelete     bNoDelete
*		    0                                   prohibited
*		    1           1                       allowed
*		    1           0              1        prohibited
*		    1           0              0        allowed
*
* Properties:
*		The following properties affect the appearance and behavior
*		of the PL_LABEL object.  Items in the list are members of
*		the PL_LABEL_PROPS structure.
*		o	bNoEdit  0 prevents the operator from moving or deleting
*					the object and from changing its text.
*		o	bNoMove  moving PL_LABELs isn't implemented
*		o	bNoDelete  0 prevents the operator from deleting the object
*		o	bSnapToTop  1 causes the PL_LABEL to snap to the top of the
*					data area.  Thus as vertical zoom factor and scroll
*					position change, the object stays in the same
*					position within the plot area.  If this member is 1,
*					then fLineFarY_pts is ignored.
*		o	bSnapToBottom  is similar to bSnapToTop, except the PL_LABEL
*					is at the bottom of the plot area.
*		o	bLineHide  1 hides the line between datum and text/box
*		o	bBoxHide  1 hides the box around the text
*		o	fLineFarX_pts  the horizontal distance, in printer points,
*					between the data point and the end of the line.
					Positive values are to the right, negative to the left.
*		o	fLineFarY_pts  the vertical distance, in printer points,
*					between the data point and the end of the line.
					Positive values are up, negative are down.
*		o	fLineMeet_frac  specifies where, on the bottom of the text/box
*					the line meets the text/box.  0, .5, and 1 specify
*					the left, center, and right, respectively.  (If
*					bSnapToBottom is 1 or fLineFarY_pts is < 0, then
*					the line joins the top of the box.)
*		o	bTextBold  1 to use bold text
*		o	bTextVert  1 to draw text/box vertical, 0 for horizontal
*		o	szFont  font name for the text
*		o	fFontSize_pts  the font size, printer points
*		o	rgbLabel  the color for the line and text/box
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a property is to call this routine
*		to get all properties, set the value of the property of interest,
*		and then call this routine to set all properties.
* 2.	The 'bNoPaint' argument can be used when many PL_LABELs are being
*		changed.  In such cases, the tradeoff may be in favor of changing
*		all the affected PL_LABELs and then calling PlWinRepaint to repaint
*		the entire plot window.
*
* See Also:
*		PlObjLabelProps, PlObjLabelCreate, PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLabelProps(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LABEL *pLabel,		// I pointer to PL_LABEL; NULL implies selected label
int		bNoPaint,		// I 1 if the label is not to be repainted
int		bSet,			// I 1 to set PL_LABEL properties, 0 to get them
PL_LABEL_PROPS *pLabelProps)// IO pointer to caller's PL_LABEL_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LABEL_PROPS propsNew;
	int		bRepaintNeeded=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pLabel is NULL, there must be a selected PL_LABEL
* 3		if pLabel is not NULL, it must be PL_LABEL
* 4		pLabelProps must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (pLabel == NULL) {
		PL_INP_CHK(2, pObjCtx->pSelObj == NULL, done)
		pLabel = (PL_LABEL *)pObjCtx->pSelObj;
		PL_INP_CHK(2, pLabel->eObjType != PL_OBJ_TY_LABEL, done)
	}
	else
		PL_INP_CHK(3, pLabel->eObjType != PL_OBJ_TY_LABEL, done)
	PL_INP_CHK(4, pLabelProps == NULL, done)

	if (bSet) {
		propsNew = *pLabelProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)		propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)		propsNew.bNoMove = 1;
		if (propsNew.bNoDelete)		propsNew.bNoDelete = 1;
		if (propsNew.bSnapToTop)	propsNew.bSnapToTop = 1;
		if (propsNew.bSnapToBottom)	propsNew.bSnapToBottom = 1;
		if (propsNew.bLineHide)		propsNew.bLineHide = 1;
		if (propsNew.bBoxHide)		propsNew.bBoxHide = 1;
		if (propsNew.bTextBold)		propsNew.bTextBold = 1;
		if (propsNew.bTextVert)		propsNew.bTextVert = 1;

		if (propsNew.bSnapToTop != pLabel->props.bSnapToTop)		bRepaintNeeded = 1;
		if (propsNew.bSnapToBottom != pLabel->props.bSnapToBottom)	bRepaintNeeded = 1;
		if (propsNew.bLineHide != pLabel->props.bLineHide)			bRepaintNeeded = 1;
		if (propsNew.bBoxHide != pLabel->props.bBoxHide)			bRepaintNeeded = 1;
		if (propsNew.bTextBold != pLabel->props.bTextBold)			bRepaintNeeded = 1;
		if (propsNew.bTextVert != pLabel->props.bTextVert)			bRepaintNeeded = 1;

		if (propsNew.fLineFarX_pts != pLabel->props.fLineFarX_pts)	bRepaintNeeded = 1;
		if (propsNew.fLineFarY_pts != pLabel->props.fLineFarY_pts)	bRepaintNeeded = 1;
		if (propsNew.fLineMeet_frac != pLabel->props.fLineMeet_frac) bRepaintNeeded = 1;
		if (propsNew.fFontSize_pts != pLabel->props.fFontSize_pts)	bRepaintNeeded = 1;
		if (propsNew.rgbLabel != pLabel->props.rgbLabel)			bRepaintNeeded = 1;
		if (strcmp(propsNew.szFont, pLabel->props.szFont) != 0)		bRepaintNeeded = 1;

		if (bRepaintNeeded && !bNoPaint) {
			int		bSelected=pObjCtx->pSelObj == (PL_OBJ *)pLabel;
	
			if (pObjCtx->mode.bEditMode)
				PlObjLabelPaint(pPlot, pLabel, bSelected);	// erase existing label
			pLabel->props = propsNew;
			PlObjLabelPaint(pPlot, pLabel, bSelected);		// paint new label
		}
		else
			pLabel->props = propsNew;
	}
	else
		*pLabelProps = pLabel->props;

done:
	PL_IF_INP_ERR("PlObjLabelProps")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLabelPropsDflt - set or gets default PL_LABEL properties
*
* Purpose:
*		Sets or retrieves the default PL_LABEL properties.
*
*		When the 'bSet' argument is 0, the current default PL_LABEL settings
*		are stored in the caller's PL_LABEL_PROPS structure.
*
*		When the 'bSet' argument is 1, the properties from the caller's
*		PL_LABEL_PROPS structure are stored in the PL_OBJ_CTX structure as
*		defaults to be used when the operator creates a new PL_LABEL on
*		the screen.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a default property is to call this
*		routine to get all default properties, set the value of the
*		default property of interest, and then call this routine to set
*		all default properties.
*
* See Also:
*		PlObjLabelProps, PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLabelPropsDflt(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bSet,			// I 1 to set PL_LABEL properties, 0 to get them
PL_LABEL_PROPS *pLabelProps)// IO pointer to caller's PL_LABEL_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LABEL_PROPS propsNew;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pLabelProps must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	PL_INP_CHK(2, pLabelProps == NULL, done)

	if (bSet) {
		propsNew = *pLabelProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)		propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)		propsNew.bNoMove = 1;
		if (propsNew.bNoDelete)		propsNew.bNoDelete = 1;
		if (propsNew.bSnapToTop)	propsNew.bSnapToTop = 1;
		if (propsNew.bSnapToBottom)	propsNew.bSnapToBottom = 1;
		if (propsNew.bLineHide)		propsNew.bLineHide = 1;
		if (propsNew.bBoxHide)		propsNew.bBoxHide = 1;
		if (propsNew.bTextBold)		propsNew.bTextBold = 1;
		if (propsNew.bTextVert)		propsNew.bTextVert = 1;
		pObjCtx->labelDflt = propsNew;
	}
	else
		*pLabelProps = pObjCtx->labelDflt;

done:
	PL_IF_INP_ERR("PlObjLabelPropsDflt")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLegendCreate - create a PL_LEGEND
*
* Purpose:
*		Creates a new PL_LEGEND in the plot window or in the currently
*		selected plot area.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the create, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 1 but object edit mode is zero, then bSelect is
*		ignored.
* 2.	If bNoCallback is 0, then the object callback function (if it exists)
*		is called with PL_OBJ_EV_EDIT indicating "create complete".  (The
*		function is not called for "create start".)
* 3.	If bHide is 1, the new PL_LEGEND won't appear on the screen.  This
*		can be used if the caller wants to add entries to the legend or to
*		change the legend's properties prior to having the PL_LEGEND become
*		visible.
* 4.	This routine can be used regardless of whether bEditMode is 1.
*
* See Also:
*		PlObjLegendCreateEntry
*		PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendCreate(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 if object callback fn is not to be called
PL_LEGEND **ppLegend,	// O place to store ptr to new PL_LEGEND, or NULL
int		bSelect,		// I 1 if the legend is to be selected
int		bHide,			// I 1 if legend is not to be shown, else 0
int		eRefPt,			// I 'reference point' on legend border, PRT_xx
int		bPlotWin,		// I 1 if legend is in plot window, 0 for data area
float	fXFrac,			// I X position, frac. of plot window or data area
float	fYFrac,			// I Y position, frac. of plot window or data area
int		bBox,			// I 1 to show box around legend
COLORREF rgbBox,		// I color for box
const char *szTitle,	// I text for legend title, or "" or NULL
COLORREF rgbTitle)		// I color to use for drawing title
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LEGEND *pLegend;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		eRefPt must be a valid PRT_xx positioning code
* 3		fXFrac must be between 0 and 1
* 4		fYFrac must be between 0 and 1
* 5		the length of szTitle must be less than PL_MSG_DIM
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (szTitle == NULL) szTitle = "";

	PL_INP_CHK(2, eRefPt < PRT_TL || eRefPt > PRT_C, done)
	PL_INP_CHK(3, fXFrac < 0.F || fXFrac > 1.F, done)
	PL_INP_CHK(4, fYFrac < 0.F || fYFrac > 1.F, done)
	PL_INP_CHK(5, strlen(szTitle) >= PL_MSG_DIM, done)

	if (!pObjCtx->mode.bEditMode)
		bSelect = 0;

	if (bSelect) {
		// Deselect existing selection, if any.
		retStat = PlObjSelect(pPlot, bNoCallback, NULL, 0);
		if (retStat != PL_OK) goto done;
	}
	if ((pLegend = (PL_LEGEND *)malloc(sizeof(PL_LEGEND))) == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}

	PlObjLegendInit(pPlot, pLegend, bHide);
	if (bPlotWin)
		pLegend->iRow = pLegend->iCol = -1;
	else {
		pLegend->iRow = pPlot->iAxisRow;
		pLegend->iCol = pPlot->iAxisCol;
	}
	pLegend->eRefPt = eRefPt;
	pLegend->fXFrac = fXFrac;
	pLegend->fYFrac = fYFrac;
	pLegend->props.bBorderHide = bBox ? 0 : 1;
	pLegend->props.rgbBox = rgbBox;
	pLegend->props.rgbTitle = rgbTitle;
	strcpy(pLegend->props.szTitle, szTitle);

	if (!bNoCallback && pPlot->pObjectFn != NULL) {
		int		stat;
		// Tell callback that create is about to complete.
		stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, (PL_OBJ *)pLegend,
						PL_OBJ_EV_EDIT, PL_OBJ_EV_CREATE, PL_OBJ_EV_DONE);
		if (stat == 0) {
			// Callback want's to veto.
			free(pLegend);
			goto done;
		}
	}

	DLIST_APPEND((PL_OBJ *)pLegend, pObjCtx->pHead, pObjCtx->pTail);
	pObjCtx->nObjects++;

	if (bSelect)
		pObjCtx->pSelObj = (PL_OBJ *)pLegend;

	retStat = PlObjLegendPaint(pPlot, pLegend, bSelect);

	if (ppLegend != NULL) *ppLegend = pLegend;

done:
	PL_IF_INP_ERR("PlObjLegendCreate")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLegendCreateEntry - create an entry for a PL_LEGEND
*
* Purpose:
*		Creates a new entry for an existing PL_LEGEND.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*
* See Also:
*		PlObjLegendCreate
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-29-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendCreateEntry(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LEGEND *pLegend,		// IO pointer to PL_LEGEND
int		eDrawType,		// I drawing type--PL_POINT, PL_LINE[_xxx], or 0
float	fDrawPts,		// I drawing thickness, in points, or 0
int		eMarkType,		// I mark type--PL_<mark>, or 0
float	fMarkPts,		// I mark size, in points, or 0
COLORREF rgbData,		// I color for points, lines, and marks, or 0
const char *szText,		// I text
COLORREF rgbText)		// I text color
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LEGEND_ENTRY *pEntry=NULL;
	int		bSelected=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pLegend is NULL, there must be a selected PL_LEGEND
* 3		if pLegend is not NULL, it must point to a PL_LEGEND
* 4		the length of szText must be less than PL_MSG_DIM
*		if eDrawType is not 0:
* 5			it must be PL_POINT or PL_LINE[_xxx]
* 6			fDrawPts must be >= 0
*		if eMarkType is not 0:
* 7			it must be a valid PL_<mark> code
* 8			fDrawPts must be >= 0
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (pLegend == NULL) {
		PL_INP_CHK(2, pObjCtx->pSelObj == NULL, done)
		pLegend = (PL_LEGEND *)pObjCtx->pSelObj;
		PL_INP_CHK(2, pLegend->eObjType != PL_OBJ_TY_LEGEND, done)
	}
	else
		PL_INP_CHK(3, pLegend->eObjType != PL_OBJ_TY_LEGEND, done)
	if (szText == NULL) szText = "";
	PL_INP_CHK(4, strlen(szText) >= PL_MSG_DIM, done)
	if (eDrawType != 0) {
		PL_INP_CHK(5, eDrawType < PL_POINT || eDrawType > PL_LINE_DASHDOTDOT, done)
		PL_INP_CHK(6, fDrawPts < 0.F, done)
	}
	if (eMarkType != 0) {
		PL_INP_CHK(7, eMarkType < PL_SQUARE || eMarkType > PL_CROSS, done)
		PL_INP_CHK(8, fMarkPts < 0.F, done)
	}

	if (pLegend == (PL_LEGEND *)pObjCtx->pSelObj)
		bSelected = 1;

	pEntry = (PL_LEGEND_ENTRY *)malloc(sizeof(PL_LEGEND_ENTRY));
	if (pEntry == NULL) {
		retStat = PL_MALLOC_FAIL;
		goto done;
	}
	pEntry->eDrawType = eDrawType;
	pEntry->eMarkType = eMarkType;
	pEntry->fDrawPts = fDrawPts;
	pEntry->fMarkPts = fMarkPts;
	pEntry->rgbData = rgbData;
	pEntry->rgbText = rgbText;
	strcpy(pEntry->szText, szText);

	if (pObjCtx->mode.bEditMode) {
		// Erase existing version of the legend.
		retStat = PlObjLegendPaint(pPlot, pLegend, bSelected);
		if (retStat != PL_OK) goto done;
	}
	DLIST_APPEND(pEntry, pLegend->pHead, pLegend->pTail);
	pEntry = NULL;
	// Draw new version of the legend.
	retStat = PlObjLegendPaint(pPlot, pLegend, bSelected);
	if (retStat != PL_OK) goto done;

done:
	if (pEntry != NULL) free(pEntry);
	PL_IF_INP_ERR("PlObjLegendCreateEntry")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendCvtCoordToLegend - convert PL_COORD set to PL_LEGEND
*
* Purpose:
*		Converts a set of PL_COORDs to a PL_LEGEND.
*
*		This routine:
*		o	stores, without validating, the bValOK, dVal, and iRowCol
*			items
*		o	transposes left/right sides during the store, if necessary
*		o	transposes top/bottom sides during the store, if necessary
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine assumes that the conversion to dVal has already
*		been done in the PL_COORD sets.  (This may mean that one or both
*		bValOK flags is zero.)
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLegendCvtCoordToLegend(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_COORD *pcoordLB,		// I the left bottom coordinate
PL_COORD *pcoordRT,		// I the right top coordinate
PL_LEGEND *pLegend)		// IO the resulting PL_LEGEND
{


}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendCvtLegendToPx - convert PL_LEGEND to set of pixel positions
*
* Purpose:
*		Converts a PL_LEGEND to the corresponding set of pixel positions.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLegendCvtLegendToPx(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LEGEND *pLegend,		// I the PL_LEGEND to convert
RECT	*prectBox,		// O Windows-compatible RECT for box
int		*pbTitle,		// O 1 if a title is to be drawn
int		*piXTitle_px,	// O X coord for title, pixels
int		*piYTitle_px,	// O Y coord for title, pixels
int		*piYEntry_px,	// O Y coord for first entry, pixels
int		*piXDataL_px,	// O X coord for left end of data drawing
int		*piXDataR_px,	// O X coord for right end of data drawing
int		*piXText_px)	// O X coord for text drawing
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	PL_LEGEND_ENTRY *pEntry;
	int		stat;
	const char *szText;
	int		iPxL, iPxR, iPxT, iPxB;
	WU_QUAD	quadText;
	int		iXLeft_px=0, iXRight_px=0, iYTop_px=0, iYBottom_px=0;
	int		iTextMaxWid_px=0, iTextSumHt_px=0;
	int		iTitleWid_px=0, iTitleHt_px=0;
	int		iTextWid_px, iTextHt_px;
	int		iWidHalfInch_px, iHtFourPts_px;
	int		iDrawMaxWid_px=0, iDrawWid_px;
	int		iXTitle_px, iYTitle_px;
	int		iYEntry_px;
	int		iXDataL_px, iXDataR_px;
	int		iXText_px;
	int		iBoxWid_px=0, iBoxHt_px=0;
	int		iXRef_px, iYRef_px;
	int		bTitle=0;
	HDC		hDC;

	// 1 inch = 72 points, 1 point = 20 twip
	iWidHalfInch_px = (int)(720.F / pPlot->fTwipsPerPixX);
	iHtFourPts_px = (int)(80.F / pPlot->fTwipsPerPixX);

	if (pLegend->iRow >= 0 && pLegend->iCol >= 0) {
		stat = PlAreaArraySelect(pPlot, pLegend->iRow, pLegend->iCol);
		if (stat != PL_OK) goto done;
		iPxL = pPlot->pXAxis->pxB;
		iPxR = pPlot->pXAxis->pxT;
		iPxB = pPlot->pYAxis->pxB;
		iPxT = pPlot->pYAxis->pxT;
	}
	else {
		iPxL = pPlot->rectPlot.left;
		iPxR = pPlot->rectPlot.right - 1;
		iPxB = pPlot->rectPlot.bottom - 1;
		iPxT = pPlot->rectPlot.top;
	}
	iXRef_px = iPxL + (int)(pLegend->fXFrac * (iPxR - iPxL + 1));
	iYRef_px = iPxB - (int)(pLegend->fYFrac * (iPxB - iPxT));

	if ((hDC = PlObjHDCHandler(pPlot, (PL_OBJ *)pLegend, 0)) == 0) goto gdi_error;

	// Get the height and width for the title.
	szText = pLegend->props.szTitle;
	if (szText[0] != '\0') {
		bTitle = 1;
		stat = WuPrtText_font(hDC, szText, 0, 0, PRT_TL,
				pLegend->props.szFont, pLegend->props.fFontSize_pts,
				pLegend->props.bTitleBold, 0.F, 0, 1, NULL, &quadText);
		iTitleWid_px = 1 + (int)(quadText.x[2] - quadText.x[0]);
		iTitleHt_px = 1 + (int)(quadText.y[2] - quadText.y[0]);
	}
	// Go through all the entries, getting the accumulated height and maximum
	// width for the text strings.  Also get the maximum width for drawing the
	// data.
	for (pEntry=pLegend->pHead; pEntry!=NULL; pEntry=pEntry->pNext) {
		// Get the height and width for the text.
		szText = pEntry->szText;
		if (szText[0] == '\0') szText = "  ";
		stat = WuPrtText_font(hDC, szText, 0, 0, PRT_TL,
				pLegend->props.szFont, pLegend->props.fFontSize_pts,
				0, 0.F, 0, 1, NULL, &quadText);  // not bold, 0 degrees
		iTextWid_px = 1 + (int)(quadText.x[2] - quadText.x[0]);
		iTextHt_px = 1 + (int)(quadText.y[2] - quadText.y[0]);
		pEntry->iTextHt_px = iTextHt_px;
		pEntry->iTextWid_px = iTextWid_px;
		iTextMaxWid_px = PL_MAX(iTextMaxWid_px, iTextWid_px);
		iTextSumHt_px += iTextHt_px;
		if (pEntry->eDrawType != 0)		// there is some sort of a line
			iDrawWid_px = iWidHalfInch_px;
		else							// there is only a plot mark
			iDrawWid_px = iWidHalfInch_px  / 4;
		iDrawMaxWid_px = PL_MAX(iDrawMaxWid_px, iDrawWid_px);
	}
	// Calculate box size, allowing an extra 1/8 inch on each side and
	// an extra 4 points above and below.  If there are no entries and
	// no title, the box will be 1/2 inch wide and 8 points high.
	iBoxWid_px = iTextMaxWid_px + iDrawMaxWid_px;
	iBoxWid_px = PL_MAX(iBoxWid_px, iTitleWid_px);
	if (iBoxWid_px == 0)		iBoxWid_px = iWidHalfInch_px;
	else						iBoxWid_px += iWidHalfInch_px / 2;
	iBoxHt_px = iTitleHt_px + iTextSumHt_px;
	if (iBoxHt_px == 0)			iBoxHt_px = 2 * iHtFourPts_px;
	else						iBoxHt_px += 2 * iHtFourPts_px;

	// Position the top left corner of the box in the plot window or data area.
#define REF_IS(name) pLegend->eRefPt == PRT_##name
	if (REF_IS(TL) || REF_IS(BL) || REF_IS(LC))
		iXLeft_px = iXRef_px;
	else if (REF_IS(TC) || REF_IS(BC) || REF_IS(C))
		iXLeft_px = iXRef_px - iBoxWid_px / 2;
	else
		iXLeft_px = iXRef_px - iBoxWid_px;
	if (REF_IS(TL) || REF_IS(TC) || REF_IS(TR))
		iYTop_px = iYRef_px;
	else if (REF_IS(LC) || REF_IS(RC) || REF_IS(C))
		iYTop_px = iYRef_px - iBoxHt_px / 2;
	else
		iYTop_px = iYRef_px - iBoxHt_px;
	// Position the bottom right corner of the box, and the other elements
	// within the box.
	iXRight_px = iXLeft_px + iBoxWid_px - 1;
	iYBottom_px = iYTop_px + iBoxHt_px - 1;
	if (pLegend->props.bEntryTextLeft) {
		// If the text is to be on the left, the caller will have to
		// handle the details of right-justifying the text at iXText_px.
		iXDataR_px = iXRight_px - iWidHalfInch_px / 4;
		iXDataL_px = iXDataR_px - iDrawWid_px;
		iXText_px = iXDataL_px;
		iXDataL_px += iWidHalfInch_px / 8;
	}
	else {
		// The caller can left-justify the text at iXText_px.
		iXDataL_px = iXLeft_px + iWidHalfInch_px / 4;
		iXDataR_px = iXDataL_px + iDrawWid_px;
		iXText_px = iXDataR_px;
		iXDataR_px -= iWidHalfInch_px / 8;
	}

	// The caller can left-justify the title at iXTitle_px.
	iXTitle_px = (iXLeft_px + iXRight_px) / 2 - iTitleWid_px / 2 - 1;
	if (bTitle && pLegend->props.bTitleTop) {
		iYTitle_px = iYTop_px + iHtFourPts_px;
		iYEntry_px = iYTitle_px + iTitleHt_px;
	}
	else {
		iYEntry_px = iYTop_px + iHtFourPts_px;
		iYTitle_px = iYBottom_px - iTitleHt_px - iHtFourPts_px;
	}

done:
	stat = PlAreaArraySelect(pPlot, rowSave, colSave);
	PlObjHDCHandler(pPlot, (PL_OBJ *)pLegend, 1);		// release

	prectBox->left = iXLeft_px;
	prectBox->right = iXRight_px + 1;
	prectBox->top = iYTop_px;
	prectBox->bottom = iYBottom_px + 1;
	*piXDataL_px = iXDataL_px;
	*piXDataR_px = iXDataR_px;
	*piXText_px = iXText_px;
	*piXTitle_px = iXTitle_px;
	*piYTitle_px = iYTitle_px;
	*piYEntry_px = iYEntry_px;
	*pbTitle = bTitle;
	return;
gdi_error:
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendHitTest - test for hit with a PL_LEGEND
*
* Purpose:
*		Tests to see if a coordinate is on or within a PL_LEGEND.  If it is,
*		o	the type of hit is deduced as one of the PL_OBJ_xxx hit types
*		o	a handle to the appropriate edit mouse pointer is stored in
*			pMouse->hPointerHit
*
*		This routine checks only pixel-oriented issues.  It totally ignores
*		issues such as 'edit enable', another object is already selected, etc.
*
* Return Value:
*		PL_OBJ_xxx if there is a hit, or
*		-1 otherwise
*
* Notes:
* 1.	PL_OBJ_C is always returned if the coordinate is on or inside a
*		selected PL_LEGEND.
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendHitTest(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_OBJ_CTX *pObjCtx,	// IO pointer to plot object context structure
PL_LEGEND *pLegend,		// I pointer to PL_LEGEND to test
PL_COORD *pcoMouse)		// I coordinate to test
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	int		iHit=-1;
	int		iMouseX=pcoMouse->x.iVisPx;
	int		iMouseY=pcoMouse->y.iVisPx;
	RECT	rectBox;
	int		bTitle, iXTitle_px, iYTitle_px;
	int		iYEntry_px, iXDataL_px, iXDataR_px, iXText_px;

	// Get the pixel values corresponding to the box and entries.
	PlObjLegendCvtLegendToPx(pPlot, pLegend,
				&rectBox, &bTitle, &iXTitle_px, &iYTitle_px,
				&iYEntry_px, &iXDataL_px, &iXDataR_px, &iXText_px);

	// Check to see if the mouse coordinate is on or inside the border
	// of the legend.
	if (iMouseX >= rectBox.left && iMouseX <= rectBox.right &&
						iMouseY >= rectBox.top && iMouseY <= rectBox.bottom) {
		iHit = PL_OBJ_C;
	}
	if (iHit == PL_OBJ_C) {
		if (pLegend->props.bNoMove && !pObjCtx->mode.bEnbMove)
			pMouse->bUseNOTPointer = 1;
	}

#define CHOOSE_HIT_POINTER(idc_cur) pMouse->hPointerHit = LoadCursor(NULL, idc_cur)

	switch (iHit) {
		case PL_OBJ_C:		CHOOSE_HIT_POINTER(IDC_SIZE); break;
	}

//	if (iHit == PL_OBJ_C) pMouse->hPointerHit = pMouse->hPointerMove;
//	else if (iHit >= 0) pMouse->hPointerHit = pMouse->hPointerSize;

	return iHit;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendInit - initialize a PL_LEGEND
*
* Purpose:
*		Initializes a PL_LEGEND:
*		o	sets the properties to default values
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLegendInit(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LEGEND *pLegend,		// IO pointer to PL_LEGEND
int		bHide)			// I 1 if legend is not to be shown, else 0
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	memset(pLegend, 0, sizeof(PL_LEGEND));
	pLegend->eObjType = PL_OBJ_TY_LEGEND;
	pLegend->props = pObjCtx->legendDflt;
	pLegend->props.bHide = bHide;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendKeyboardPlaceMouse - place the mouse pointer on a PL_LEGEND
*
* Purpose:
*		Places the mouse pointer on a PL_LEGEND.
*
* Return Value:
*		void
*
* Notes:
* 1.	This routine doesn't return a data value coordinate for the mouse.
*		Only .iVisPx and .dVirtPx are returned.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-30-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLegendKeyboardPlaceMouse(
PL_CTX	*pPlot,			// IO pointer to plot context structure
WPARAM	wParam,			// I virtual key code
PL_LEGEND *pLegend,		// I the object to place mouse on
PL_COORD *pcoMouse,		// O the new coordinate for the mouse
float	*pfAnchFrac)	// O anchor fraction if scroll change needed
{
	RECT	rectBox;
	int		bTitle, iXTitle_px, iYTitle_px;
	int		iYEntry_px, iXDataL_px, iXDataR_px, iXText_px;

	// Get the pixel values corresponding to the box and entries.
	PlObjLegendCvtLegendToPx(pPlot, pLegend,
				&rectBox, &bTitle, &iXTitle_px, &iYTitle_px,
				&iYEntry_px, &iXDataL_px, &iXDataR_px, &iXText_px);

	pcoMouse->x.bValOK = pcoMouse->y.bValOK = 1;
	pcoMouse->x.iRowCol = pcoMouse->y.iRowCol = -1;

	pcoMouse->x.dVirtPx = ((double)rectBox.left + (double)rectBox.right) / 2.;
	pcoMouse->y.dVirtPx = ((double)rectBox.top + (double)rectBox.bottom) / 2.;

	pcoMouse->x.iVisPx = (int)pcoMouse->x.dVirtPx;
	pcoMouse->y.iVisPx = (int)pcoMouse->y.dVirtPx;
	*pfAnchFrac = .5F;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendPaint - paint a PL_LEGEND
*
* Purpose:
*		Paints a PL_LEGEND.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_LEGEND *pLegend,		// I pointer to PL_LEGEND to draw
int		bSelected)		// I 1 if the PL_LEGEND is selected
{
	int		retStat=PL_OK;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_LEGEND_ENTRY *pEntry;
	HDC		hDC=0;
	HPEN	hPen=0, hPenOld=0;
	HBRUSH	hBrush=0, hBrushOld=0;
	COLORREF rgbFill, rgbBkOld;
	COLORREF rgbBkText, rgbText;
	int		stat;
	const char *szFont=pLegend->props.szFont;
	float	fFontSize=pLegend->props.fFontSize_pts;
	RECT	rectBox;
	int		bTitle, iXTitle_px, iYTitle_px;
	int		iYEntry_px, iXDataL_px, iXDataR_px, iXText_px;
	int		iXpx, iYpx;

	if (pLegend->props.bHide) goto done;

	// Get the pixel values corresponding to the box and entries.
	PlObjLegendCvtLegendToPx(pPlot, pLegend,
				&rectBox, &bTitle, &iXTitle_px, &iYTitle_px,
				&iYEntry_px, &iXDataL_px, &iXDataR_px, &iXText_px);

	hDC = PlObjHDCHandler(pPlot, (PL_OBJ *)pLegend, 0);
	if (hDC == 0) goto gdi_error;
	// The 'box' is always drawn filled.  If the bFill property is set,
	// the fill is with the rgbBox color; otherwise, it is with the
	// background color.
	if (pPlot->bPrinter && !pPlot->bPrinterColor)
		rgbFill = pObjCtx->rgbBkgOld;
	else if (pLegend->props.bFill)
		rgbFill = pLegend->props.rgbBox;
	else
		rgbFill = pObjCtx->rgbBkgOld;
	if (pObjCtx->mode.bEditMode)
		rgbFill ^= pObjCtx->rgbBkgOld;
	if ((hBrush = CreateSolidBrush(rgbFill)) != 0)
/*KG*/	//hBrushOld = SelectObject(hDC, hBrush);
/*KG*/	hBrushOld = (HBRUSH)SelectObject(hDC, hBrush);
	if (!bSelected && pLegend->props.bBorderHide)
/*KG*/	//hPenOld = SelectObject(hDC, GetStockObject(NULL_PEN));
/*KG*/	hPenOld = (HPEN)SelectObject(hDC, GetStockObject(NULL_PEN));
	Rectangle(hDC, rectBox.left, rectBox.top, rectBox.right, rectBox.bottom);
	if (hBrushOld != 0) SelectObject(hDC, hBrushOld);
	if (hBrush != 0) DeleteObject(hBrush);
	hBrush = hBrushOld = 0;
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	hPenOld = 0;
	rgbBkText = rgbFill;
	if (pLegend->props.bFill && pObjCtx->mode.bEditMode)
		rgbBkText = BLACK;
	if (bTitle) {
		rgbText = pLegend->props.rgbTitle;
		PlObjTextColor(pPlot, pLegend->props.bFill, rgbFill, rgbBkText, &rgbText);
		rgbBkOld = SetBkColor(hDC, rgbBkText);
		SetTextColor(hDC, rgbText);
		if (pPlot->bPrinter) {
			stat = WuPrtText_font(hDC, pLegend->props.szTitle,
						iXTitle_px, iYTitle_px, PRT_TL,
						szFont, fFontSize, pLegend->props.bTitleBold,
						0.F, 0, 0, NULL, NULL);
		}
		else {
			stat = WuPrtText_font_ROP2(hDC, pLegend->props.szTitle,
						iXTitle_px, iYTitle_px,
						szFont, fFontSize, pLegend->props.bTitleBold, 0);
		}
		if (stat == 1) goto gdi_error;
		SetBkColor(hDC, rgbBkOld);
	}

	iYpx = iYEntry_px;
	for (pEntry=pLegend->pHead; pEntry!=NULL; pEntry=pEntry->pNext) {
		int		iYData_px;
		COLORREF rgbData;
		rgbData = pEntry->rgbData;
		if (pPlot->bPrinter && !pPlot->bPrinterColor)
			rgbData = BLACK;
		iYData_px = iYpx + pEntry->iTextHt_px / 2;
		if (pLegend->props.bEntryTextLeft)
			iXpx = iXText_px - pEntry->iTextWid_px;
		else
			iXpx = iXText_px;
		rgbText = pEntry->rgbText;
		PlObjTextColor(pPlot, pLegend->props.bFill, rgbFill, rgbBkText, &rgbText);
		rgbBkOld = SetBkColor(hDC, rgbBkText);
		SetTextColor(hDC, rgbText);
		if (pPlot->bPrinter) {
			stat = WuPrtText_font(hDC, pEntry->szText, iXpx, iYpx, PRT_TL,
						szFont, fFontSize, 0, 0.F, 0, 0, NULL, NULL);
		}
		else {
			stat = WuPrtText_font_ROP2(hDC,
						pEntry->szText, iXpx, iYpx, szFont, fFontSize, 0, 0);
		}

		if (stat == 1) goto gdi_error;
		SetBkColor(hDC, rgbBkOld);
		if (pEntry->eDrawType == 0)
			;	// no action
		else if (pEntry->eDrawType == PL_POINT) {
			int		i, iXTemp_px;
			int		ht=PL_YPTS_TO_YPIX(pEntry->fDrawPts);
			int		wid=PL_XPTS_TO_XPIX(pEntry->fDrawPts);
			for (i=0; i<4; i++) {
				iXTemp_px = iXDataL_px + i * (iXDataR_px - iXDataL_px + 1) / 3;
				retStat = PlDraw_discrete(pPlot, hDC,
						iXTemp_px, iYData_px, wid, ht, PL_POINT,
						0, 0, 0, 0, 0, 0, rgbData);
				if (retStat != PL_OK) goto done;
			}
		}
		else {
			if (pEntry->eDrawType == PL_LINE) {
				int		iThick;
				iThick = PL_PTS_TO_PIX(pEntry->fDrawPts);
				if (iThick < 1) iThick = 1;
				hPen = CreatePen(PS_SOLID, iThick, rgbData);
			}
			else {
				int		pattern;
				switch (pEntry->eDrawType) {
					case PL_LINE_DASH:		pattern = PS_DASH; break;
					case PL_LINE_DOT:		pattern = PS_DOT; break;
					case PL_LINE_DASHDOT:	pattern = PS_DASHDOT; break;
					case PL_LINE_DASHDOTDOT:pattern = PS_DASHDOTDOT; break;
					default:				pattern = PS_SOLID; break;
				}
				hPen = CreatePen(pattern, 1, rgbData);
			}
			if (hPen != 0)
/*KG*/			//hPenOld = SelectObject(hDC, hPen);
/*KG*/			hPenOld = (HPEN)SelectObject(hDC, hPen);
			MoveToEx(hDC, iXDataL_px, iYData_px, NULL);
			LineTo(hDC, iXDataR_px, iYData_px);
			if (hPenOld != 0) SelectObject(hDC, hPenOld);
			if (hPen != 0) DeleteObject(hPen);
			hPen = hPenOld = 0;
		}
		if (pEntry->eMarkType != 0) {
			int		ht=PL_YPTS_TO_YPIX(pEntry->fMarkPts);
			int		wid=PL_XPTS_TO_XPIX(pEntry->fMarkPts);
			hPen = CreatePen(PS_SOLID, 1, pEntry->rgbData);
			if (hPen != 0)
/*KG*/			//hPenOld = SelectObject(hDC, hPen);
/*KG*/			hPenOld = (HPEN)SelectObject(hDC, hPen);
			retStat = PlDraw_discrete(pPlot, hDC,
					iXDataL_px, iYData_px, wid, ht, pEntry->eMarkType,
					0, 0, 0, 0, 0, 0, rgbData);
			if (retStat != PL_OK) goto done;
			if (pEntry->eDrawType != 0) {
				retStat = PlDraw_discrete(pPlot, hDC,
						iXDataR_px, iYData_px, wid, ht, pEntry->eMarkType,
						0, 0, 0, 0, 0, 0, rgbData);
				if (retStat != PL_OK) goto done;
			}
			if (hPenOld != 0) SelectObject(hDC, hPenOld);
			if (hPen != 0) DeleteObject(hPen);
			hPen = hPenOld = 0;
		}
		iYpx += pEntry->iTextHt_px;
	}

done:
	if (hPenOld != 0) SelectObject(hDC, hPenOld);
	if (hPen != 0) DeleteObject(hPen);
	if (hDC != 0)
		PlObjHDCHandler(pPlot, (PL_OBJ *)pLegend, 1);		// release
	return retStat;
gdi_error:
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjLegendPaint_edit - paint the PL_LEGEND "rubber band"
*
* Purpose:
*		Paints the "rubber band" for the PL_LEGEND being edited.  Since the
*		drawing uses XOR, alternate calls to this routine with the same
*		edit object alternately draw and erase the edit object.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjLegendPaint_edit(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_COORD *pcoLB=&pObjCtx->coEditLBCurr;
	PL_COORD *pcoRT=&pObjCtx->coEditRTCurr;

	return;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLegendProps - set or gets PL_LEGEND properties
*
* Purpose:
*		Sets the properties for a PL_LEGEND or retrieves the properties from
*		a PL_LEGEND.
*
*		When the 'bSet' argument is 0, the current flag settings are
*		stored in the caller's PL_LEGEND_PROPS structure.
*
*		When the 'bSet' argument is 1,
*		o	the properties from the caller's PL_LEGEND_PROPS structure are
*			stored in the specified legend
*		o	if one or more of the visual properties of the PL_LEGEND change,
*			then the legend will be repainted, unless the 'bNoPaint' argument
*			is 1.
*
*		The PL_OBJ_CTX enable flags provide global control on what the
*		operator is allowed to do using the mouse and keyboard.  The
*		following table illustrates the behavior for deleting an object;
*		the other edit operations operate in a similar way.
*
*		----- PL_OBJ_CTX ----- PL_LEGEND_PROPS  operator delete is
*		bEditMode   bEnbDelete     bNoDelete
*		    0                                   prohibited
*		    1           1                       allowed
*		    1           0              1        prohibited
*		    1           0              0        allowed
*
*
* Properties:
*		The following properties affect the appearance and behavior
*		of the PL_LEGEND object.  Items in the list are members of
*		the PL_LEGEND_PROPS structure.
*		o	bNoEdit  0 prevents the operator from moving or deleting
*					the object and from changing its properties.
*		o	bNoMove  not implemented
*		o	bNoDelete  0 prevents the operator from deleting the object
*		o	bHide  1 causes the legend object not to be drawn
*		o	bBorderHide  1 causes the legend border not to be drawn
*		o	bFill  1 causes the legend legend box (when not selected)
*					to be shown filled (i.e., solid)
*		o	bTitleTop  1 if title is at top of box
*		o	bTitleBold  1 to use bold text for title
*		o	bEntryTextLeft  1 causes the legend entry to be drawn with
*					the text on the left and the line/mark/etc. that
*					is used for drawing on the right; 0 reverses the order
*		o	fBorderPts  drawing thickness for legend border, in points
*		o	fFontSize_pts  title and entry text font size, printer points
*		o	rgbBox  color for border and fill
*		o	rgbTitle  color for title
*		o	szTitle  title for the legend
*		o	szFont  font name for title and entry text
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a property is to call this routine
*		to get all properties, set the value of the property of interest,
*		and then call this routine to set all properties.
* 2.	The 'bNoPaint' argument can be used when many PL_LEGENDs are being
*		changed.  In such cases, the tradeoff may be in favor of changing
*		all the affected PL_LEGENDs and then calling PlWinRepaint to repaint
*		the entire plot window.
*
* See Also:
*		PlObjLegendCreateSimple, PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendProps(
PL_CTX	*pPlot,			// IO pointer to plot context structure
PL_LEGEND *pLegend,		// I pointer to PL_LEGEND; NULL implies selected legend
int		bNoPaint,		// I 1 if the legend is not to be repainted
int		bSet,			// I 1 to set PL_LEGEND properties, 0 to get them
PL_LEGEND_PROPS *pLegendProps)// IO pointer to caller's PL_LEGEND_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LEGEND_PROPS propsNew;
	int		bRepaintNeeded=0;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		if pLegend is NULL, there must be a selected PL_LEGEND
* 3		if pLegend is not NULL, it must point to a PL_LEGEND
* 4		pLegendProps must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (pLegend == NULL) {
		PL_INP_CHK(2, pObjCtx->pSelObj == NULL, done)
		pLegend = (PL_LEGEND *)pObjCtx->pSelObj;
		PL_INP_CHK(2, pLegend->eObjType != PL_OBJ_TY_LEGEND, done)
	}
	else
		PL_INP_CHK(3, pLegend->eObjType != PL_OBJ_TY_LEGEND, done)
	PL_INP_CHK(4, pLegendProps == NULL, done)

	if (bSet) {
		propsNew = *pLegendProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)			propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)			propsNew.bNoMove = 1;
		if (propsNew.bNoDelete)			propsNew.bNoDelete = 1;
		if (propsNew.bHide)				propsNew.bHide = 1;
		if (propsNew.bBorderHide)		propsNew.bBorderHide = 1;
		if (propsNew.bFill)				propsNew.bFill = 1;
		if (propsNew.bTitleTop)			propsNew.bTitleTop = 1;
		if (propsNew.bTitleBold)		propsNew.bTitleBold = 1;
		if (propsNew.bEntryTextLeft)	propsNew.bEntryTextLeft = 1;

		if (propsNew.bHide != pLegend->props.bHide)					bRepaintNeeded = 1;
		if (propsNew.bBorderHide != pLegend->props.bBorderHide)		bRepaintNeeded = 1;
		if (propsNew.bFill != pLegend->props.bFill)					bRepaintNeeded = 1;
		if (propsNew.bTitleTop != pLegend->props.bTitleTop)			bRepaintNeeded = 1;
		if (propsNew.bTitleBold != pLegend->props.bTitleBold)		bRepaintNeeded = 1;
		if (propsNew.bEntryTextLeft != pLegend->props.bEntryTextLeft) bRepaintNeeded = 1;

		if (propsNew.fBorderPts != pLegend->props.fBorderPts)		bRepaintNeeded = 1;
		if (propsNew.fFontSize_pts != pLegend->props.fFontSize_pts)	bRepaintNeeded = 1;
		if (propsNew.rgbBox != pLegend->props.rgbBox)				bRepaintNeeded = 1;
		if (propsNew.rgbTitle != pLegend->props.rgbTitle)			bRepaintNeeded = 1;
		if (strcmp(propsNew.szTitle, pLegend->props.szTitle) != 0)	bRepaintNeeded = 1;
		if (strcmp(propsNew.szFont, pLegend->props.szFont) != 0)	bRepaintNeeded = 1;

		if (bRepaintNeeded && !bNoPaint) {
			int		bSelected=pObjCtx->pSelObj == (PL_OBJ *)pLegend;
	
			if (pObjCtx->mode.bEditMode)
				PlObjLegendPaint(pPlot, pLegend, bSelected);// erase existing legend
			pLegend->props = propsNew;
			PlObjLegendPaint(pPlot, pLegend, bSelected);	// paint new legend
		}
		else
			pLegend->props = propsNew;
	}
	else
		*pLegendProps = pLegend->props;

done:
	PL_IF_INP_ERR("PlObjLegendProps")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjLegendPropsDflt - set or gets default PL_LEGEND properties
*
* Purpose:
*		Sets or retrieves the default PL_LEGEND properties.
*
*		When the 'bSet' argument is 0, the current default PL_LEGEND settings
*		are stored in the caller's PL_LEGEND_PROPS structure.
*
*		When the 'bSet' argument is 1, the properties from the caller's
*		PL_LEGEND_PROPS structure are stored in the PL_OBJ_CTX structure as
*		defaults to be used when the operator creates a new PL_LEGEND on
*		the screen.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	An effective way to alter a default property is to call this
*		routine to get all default properties, set the value of the
*		default property of interest, and then call this routine to set
*		all default properties.
*
* See Also:
*		PlObjLegendProps, PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-28-97 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjLegendPropsDflt(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bSet,			// I 1 to set PL_LEGEND properties, 0 to get them
PL_LEGEND_PROPS *pLegendProps)// IO pointer to caller's PL_LEGEND_PROPS struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_LEGEND_PROPS propsNew;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pLegendProps must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	PL_INP_CHK(2, pLegendProps == NULL, done)

	if (bSet) {
		propsNew = *pLegendProps;
		// make sure that all booleans are either 0 or 1
		if (propsNew.bNoEdit)			propsNew.bNoEdit = 1;
		if (propsNew.bNoMove)			propsNew.bNoMove = 1;
		if (propsNew.bNoDelete)			propsNew.bNoDelete = 1;
		if (propsNew.bHide)				propsNew.bHide = 1;
		if (propsNew.bBorderHide)		propsNew.bBorderHide = 1;
		if (propsNew.bFill)				propsNew.bFill = 1;
		if (propsNew.bTitleTop)			propsNew.bTitleTop = 1;
		if (propsNew.bTitleBold)		propsNew.bTitleBold = 1;
		if (propsNew.bEntryTextLeft)	propsNew.bEntryTextLeft = 1;
		pObjCtx->legendDflt = propsNew;
	}
	else
		*pLegendProps = pObjCtx->legendDflt;

done:
	PL_IF_INP_ERR("PlObjLegendPropsDflt")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjMode - set/get object edit mode and enable flags
*
* Purpose:
*		Sets or retrieves the object edit mode flag and PL_OBJ_CTX enable
*		flags.
*
*		When the 'bSet' argument is 0, the object edit mode flag and
*		PL_OBJ_CTX enable flags are stored in the caller's PL_OBJ_MODE
*		structure.
*
*		When the 'bSet' argument is 1,
*		o	the flags from the caller's PL_OBJ_MODE structure are
*			stored in the PL_OBJ_CTX structure
*		o	if the new flags cause a change from edit mode to non-edit
*			mode and there is a currently selected object it will be
*			deselected.  In this case, the object callback function is
*			called unless the 'bNoCallback' argument is 1.
*		o	if the new flags cause a change between edit mode and
*			non-edit mode, the entire plot window is repainted.
*
*		The PL_OBJ_CTX mode flags provide global control on what the operator
*		is allowed to do using the mouse and keyboard.  For example,
*		if a PL_BOX has .bNoResize set to 1 the operator can't resize
*		it if the .bEnbResize enable flag is 0.  If the .bEnbResize enable
*		flag is 1, then any PL_BOX can be resized--regardless of the
*		state of its .bNoResize property.  The other pairs of PL_BOX
*		properties with PL_OBJ_CTX enable flags operate in a similar fashion.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the deselect, or
*		other codes for other errors
*
* Notes:
* 1.	If bEditMode is 0, the enable flags have no effect on plot object
*		operations.
* 2.	bEnbBoxSpan and bEnbBoxMany only affect operator actions.  Calls to
*		PlObjBoxCreateXxx aren't affected by these flags.
*
* See Also:
*		PlObjBoxCreateGeneral, PlObjBoxCreateSimple
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-05-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjMode(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 to inhibit calling object callback function
int		bSet,			// I 1 to set global MODE properties, 0 to get them
PL_OBJ_MODE *pMode)		// IO pointer to caller's PL_OBJ_MODE struct
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_OBJ_MODE modeNew;
	int		iEl0, iEl, col, row;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
* 2		pMode must not be NULL
*		if bSet is 1 the following input checks are done:
*			if bEditMode is 1 and bEnbBoxSpan is 1:
*				for each plot area column:
* 3					all plot areas in the column must have the same X range
*				for each plot area row:
* 4					all plot areas in the row must have the same Y range
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	PL_INP_CHK(2, pMode == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	if (bSet) {
		// Do the plot area checks.  NOTE WELL!! No test for paXAxis == NULL is
		// needed here since the check starts with row and column 1 and it's
		// guaranteed that paXAxis and paYAxis are both non-NULL if there are
		// more than 1 row and/or column.
//		if (pPlot->nAxisRows > 1 || pPlot->nAxisCols > 1) {
		if ((pPlot->nAxisRows > 1 || pPlot->nAxisCols > 1) && (pMode->bEnbBoxSpan)) {	// Added bEnbBoxSpan condition. 27-May-98 ToM
			iEl0 = 0 + 0 * pPlot->nAxisRows;
			for (col=1; col<pPlot->nAxisCols; col++) {
				iEl = 0 + col * pPlot->nAxisRows;
				PL_INP_CHK(2, pPlot->paXAxis[iEl0].dDValB != pPlot->paXAxis[iEl].dDValB, done)
				PL_INP_CHK(2, pPlot->paXAxis[iEl0].dDValT != pPlot->paXAxis[iEl].dDValT, done)
			}
			for (row=1; row<pPlot->nAxisRows; row++) {
				iEl = row + 0 * pPlot->nAxisRows;
				PL_INP_CHK(3, pPlot->paYAxis[iEl0].dDValB != pPlot->paYAxis[iEl].dDValB, done)
				PL_INP_CHK(3, pPlot->paYAxis[iEl0].dDValT != pPlot->paYAxis[iEl].dDValT, done)
			}
		}
	}

	if (bSet) {
		modeNew = *pMode;
		// make sure that all booleans are either 0 or 1
		if (pMode->bEditMode) {
			modeNew.bEditMode = 1;
			modeNew.bEnbBoxNew = modeNew.bEnbBoxNew?1:0;
			modeNew.bEnbBoxSpan = modeNew.bEnbBoxSpan?1:0;
			modeNew.bEnbBoxMany = modeNew.bEnbBoxMany?1:0;
			modeNew.bEnbMove = modeNew.bEnbMove?1:0;
			modeNew.bEnbResize = modeNew.bEnbResize?1:0;
			modeNew.bEnbDelete = modeNew.bEnbDelete?1:0;
		}
		else {
			modeNew.bEditMode = 0;
			modeNew.bEnbBoxNew = 0;
			modeNew.bEnbBoxSpan = 0;
			modeNew.bEnbBoxMany = 0;
			modeNew.bEnbMove = 0;
			modeNew.bEnbResize = 0;
			modeNew.bEnbDelete = 0;
			if (pObjCtx->mode.bEditMode && pObjCtx->pSelObj != NULL) {
				retStat = PlObjSelect(pPlot, bNoCallback, NULL, 0);
				if (retStat != PL_OK) goto done;
			}
		}
		if (pObjCtx->mode.bEditMode != modeNew.bEditMode) {
			// Edit mode has changed.  If any boxes exist, and if this isn't
			// the printer, trigger a repaint of the plot so that the objects
			// will be drawn in the appropriate mode.
			if (pObjCtx->nObjects > 0 && !pPlot->bPrinter)
				retStat = PlWinRepaint(pPlot);
		}
		pObjCtx->mode = modeNew;
	}
	else
		*pMode = pObjCtx->mode;

done:
	PL_IF_INP_ERR("PlObjMode")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjPaint - paint an object
*
* Purpose:
*		Paints a object.
*
*		If the .bHide property is set, this routine does nothing.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjPaint(
PL_CTX	*pPlot,			// I pointer to plot context structure
PL_OBJ	*pObj,			// I pointer to object to draw
int		bSelected)		// I 1 if the box is selected
{
	int		retStat=PL_OK;

	if (pObj->eObjType == PL_OBJ_TY_BOX)
		retStat = PlObjBoxPaint(pPlot, (PL_BOX *)pObj, bSelected);
	else if (pObj->eObjType == PL_OBJ_TY_LABEL)
		retStat = PlObjLabelPaint(pPlot, (PL_LABEL *)pObj, bSelected);
	else if (pObj->eObjType == PL_OBJ_TY_LEGEND)
		retStat = PlObjLegendPaint(pPlot, (PL_LEGEND *)pObj, bSelected);

	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjPaint_edit - paint the "rubber band" edit object
*
* Purpose:
*		Paints the "rubber band" edit object.  Since the drawing uses
*		XOR, alternate calls to this routine with the same edit object
*		alternately draw and erase the edit object.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-26-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjPaint_edit(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	if (pObjCtx->pEditObj->eObjType == PL_OBJ_TY_BOX)
		PlObjBoxPaint_edit(pPlot);
	else if (pObjCtx->pEditObj->eObjType == PL_OBJ_TY_LABEL)
		PlObjLabelPaint_edit(pPlot);

}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjPaintAll - paint the objects
*
* Purpose:
*		Paints the objects.
*
*		When a program is using plot objects, it is responsible for
*		painting them.  In the plot callback function, use the following
*		statement (prior to the PlPaintCursor call):
*
*				stat = PlObjPaintAll(pPlot);
*
*		If 'filled' PL_BOXs are being used, then this routine should
*		be called before the plot callback function paints the background
*		or data.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		other codes for other errors
*
* Notes:
* 1.	There several issues that complicate the drawing of PL_BOXs.
*		Because of these issues, it is recommended that user programs
*		call this routine only from their plot callback function.
*
* See Also:
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjPaintAll(
PL_CTX	*pPlot)			// I pointer to plot context structure
{
	int		retStat=PL_OK, inpErr=0;
	PL_OBJ_CTX *pObjCtx;
	PL_OBJ	*pObj;
	int		bSelected;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;

	for (pObj=pObjCtx->pHead; pObj!=NULL; pObj=pObj->pNext) {
		bSelected = pObj == pObjCtx->pSelObj;
		retStat = PlObjPaint(pPlot, pObj, bSelected);
		if (retStat != PL_OK) goto done;
	}

done:
	PL_IF_INP_ERR("PlObjPaintAll")
	return retStat;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjSelect - select/deselect an object
*
* Purpose:
*		Selects or deselects an object.
*
* Return Value:
*		PL_OK, or
*		PL_BAD_INPUT if one of the input checks fails, or
*		PL_VETOED  if the callback function vetoed the select/deselect, or
*		other codes for other errors
*
* Notes:
* 1.	If bSelect is 0 (to deselect) but there is no currently selected
*		box, this routine does nothing.
*
* See Also:
*		PlObjBoxCreateGeneral, PlObjPaintAll, PlObjDelete
*		PlObjMode
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-10-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjSelect(
PL_CTX	*pPlot,			// IO pointer to plot context structure
int		bNoCallback,	// I 1 to inhibit calling object callback function
PL_OBJ	*pObj,			// I pointer to PL_BOX to select if bSelect is 1
int		bSelect)		// I 1 to select, 0 to deselect
{
	int		retStat=PL_OK, inpErr=0;
	int		iSide;
	int		stat;
	PL_OBJ_CTX *pObjCtx;

/*+--------------------------------------------------------------------------
*
* Input Checks:
* 1		pPlot must not be NULL
*		if bSelect is 1, then
* 2			pObj must not be NULL
* 3			pObjCtx->mode.bEditMode must be 1
*			if the object is a PL_BOX
* 4				all four sides of pBox must have bValOK == 1
*--------------------------------------------------------------------------*/
	PL_INP_CHK(1, pPlot == NULL, done)
	pObjCtx = pPlot->pObjCtx;
	if (bSelect) {
		PL_INP_CHK(2, pObj == NULL, done)
		PL_INP_CHK(3, !pObjCtx->mode.bEditMode, done)
		if (pObj->eObjType == PL_OBJ_TY_BOX) {
			for (iSide=0; iSide<4; iSide++) {
				PL_INP_CHK(4, !((PL_BOX *)pObj)->aSides[iSide].bValOK, done)
			}
		}
	}

	if (pObjCtx->pSelObj != NULL) {
		PL_OBJ	*pObjTemp=pObjCtx->pSelObj;
		if (!bNoCallback && pPlot->pObjectFn != NULL) {
			// Tell callback that a deselect is about to happen.
			stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx,
							pObjTemp, PL_OBJ_EV_SEL, 0., 0.);
			if (stat == 0) {
				// Callback want's to veto.
				retStat = PL_VETOED;
				goto done;
			}
		}
		// Deselect other box before starting on this one.
		PlObjPaint(pPlot, pObjTemp, 1);	// erase the selected version
		pObjCtx->pSelObj = NULL;
		PlObjPaint(pPlot, pObjTemp, 0);	// and repaint it as non-selected
	}

	if (bSelect) {
		PlObjPaint(pPlot, pObj, 0);	// erase the non-selected version
		pObjCtx->pSelObj = pObj;
		if (!bNoCallback && pPlot->pObjectFn != NULL) {
			// Tell callback that a select has happened.
			stat = pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx,
							pObj, PL_OBJ_EV_SEL, 1., 0.);
			if (stat == 0) {
				// Callback want's to veto.
				PlObjPaint(pPlot, pObj, 0);	// paint the non-selected version
				pObjCtx->pSelObj = NULL;
				retStat = PL_VETOED;
				goto done;
			}
		}
		PlObjPaint(pPlot, pObj, 1);	// paint as selected
	}

done:
	PL_IF_INP_ERR("PlObjSelect")
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjTextColor - massage text color
*
* Purpose:
*		Examine text color and possibly modify it, depending
*		on the current situation.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 01-31-97 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlObjTextColor(
PL_CTX	*pPlot,
int		bFill,
COLORREF rgbFill,
COLORREF rgbBkText,
COLORREF *prgbText)
{
	COLORREF rgbText=*prgbText;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;

	if (pPlot->bPrinter && !pPlot->bPrinterColor)
		rgbText = BLACK;
	else if (pObjCtx->mode.bEditMode) {
		// If we're using XOR for drawing, then we need to take special
		// care that the text will be visible.
		if (bFill) {
			if (rgbFill != pObjCtx->rgbBkgOld)	rgbText = rgbFill;
			else if (rgbText == BLACK)			rgbText = WHITE;
			else								rgbText ^= rgbBkText;
		}
		else
			rgbText ^= pObjCtx->rgbBkgOld;
	}
	*prgbText = rgbText;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjValIsInRange - check if DVal is in range for axis
*
* Purpose:
*		Checks to see if a data value is in range for an axis.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-18-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjValIsInRange(
PL_AXIS	*pAxis,			// I pointer to PL_AXIS structure
double	dVal)			// I data value to check
{
	if (pAxis->dDValB < pAxis->dDValT) {
		if (dVal >= pAxis->dDValB && dVal <= pAxis->dDValT)
			return 1;
	}
	else {
		if (dVal >= pAxis->dDValT && dVal <= pAxis->dDValB)
			return 1;
	}
	return 0;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjWinCreate - create a plot object edit 'dialog'
*
* Purpose:
*		Creates a plot object edit 'dialog'.
*
* Return Value:
*		HWND, or
*		NULL if error occurs
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-28-96 R. Cole		created, as a clone from pl_prt.c
*--------------------------------------------------------------------------*/
HWND PASCAL
PlObjWinCreate(
HINSTANCE hInst,		// I handle to this instance
HWND	hwParent,		// I parent window handle
int		bHelpWin,		// I 1 to display the 'help' window
PL_CTX	*pPlot)			// IO pointer to plot context struct
{
	HWND	hwObjDialog=0;
	char	*szClass="PLOBJWIN";
	WNDCLASS statusClass;
	ATOM	atom;
	POINT	ptPlot;
	int		stat;
	int		iXPos, iYPos, iWid, iPlotWid, iScreenWid;

	stat = GetClassInfo(hInst, szClass, &statusClass);
	if (stat == 0) {  // the class hasn't been registered yet
		statusClass.style		= CS_OWNDC;
		statusClass.lpfnWndProc	= PlObjWinProc;
		statusClass.cbClsExtra	= 0;
		statusClass.cbWndExtra	= 1 * sizeof(void *);
		statusClass.hInstance	= hInst;
		statusClass.hIcon		= NULL;
		statusClass.hCursor		= NULL;
		statusClass.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
		statusClass.lpszMenuName = NULL;
		statusClass.lpszClassName = szClass;
		atom = RegisterClass(&statusClass);
		if (atom == 0)
			goto done;
	}

	ptPlot.x = pPlot->rectPlot.left;
	ptPlot.y = pPlot->rectPlot.top;
	ClientToScreen(pPlot->hwPlot, &ptPlot);
	iXPos = ptPlot.x;
	iYPos = ptPlot.y + 50;
	if (bHelpWin)
		iWid = 500;
	else
		iWid = 300;
	iPlotWid = pPlot->rectPlot.right - pPlot->rectPlot.left;
	iScreenWid = GetSystemMetrics(SM_CXFULLSCREEN);
	if (iWid < iPlotWid)				iXPos += (iPlotWid - iWid) / 2;
	if (iXPos + iWid > iScreenWid)		iXPos = iScreenWid - iWid;
	if (iXPos < 0)						iXPos = 0;
	hwObjDialog = CreateWindow(szClass, "", WS_POPUPWINDOW | WS_CAPTION,
							iXPos, iYPos, iWid, 100,
							hwParent, 0, hInst, NULL);
	if (hwObjDialog == 0)
		goto done;
	SetWindowLong(hwObjDialog, 0, (DWORD)pPlot);
done:
	return hwObjDialog;
}
/*+/csubr/TOC----------------------------------------------------------------
* PlObjWinDialog - show or dismiss the plot object edit 'dialog'
*
* Purpose:
*		Shows or dismisses the plot object edit 'dialog'.
*
* Return Value:
*		PL_OK, or
*		other codes for other errors
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-28-96 R. Cole		created, as a clone from pl_prt.c
*--------------------------------------------------------------------------*/
int PASCAL
PlObjWinDialog(
PL_CTX	*pPlot,			// IO pointer to plot context struct
HWND	hwParent,		// I handle for parent window
int		bHelpWin,		// I 1 to display the 'help' window
PL_OBJ	*pObject)		// I non-NULL (or NULL) to show (or dismiss) the window
{
	int		retStat=PL_OK;
	HINSTANCE hInst;
	HWND	hwObjDialog, hwCtrl;
	RECT	rectWin;
	HDC		hDC=0;
	int		iYpx=5;

	if (pObject == NULL && !bHelpWin) {
		WuDoEvents();	// allow possible "Cancel" click to take effect
		if (pPlot->pObjCtx->hwObjDialog != 0)
			DestroyWindow(pPlot->pObjCtx->hwObjDialog);
		pPlot->pObjCtx->hwObjDialog = 0;
		pPlot->pObjCtx->bObjEditDone = 1;
	}
	else {
		hInst = WuGetHInstance(hwParent);
		hwObjDialog = PlObjWinCreate(hInst, hwParent, bHelpWin, pPlot);
		if (hwObjDialog == 0) goto gdi_error;
		pPlot->pObjCtx->hwObjDialog = hwObjDialog;
		pPlot->pObjCtx->pDialogObj = pObject;
		hDC = GetDC(hwObjDialog);
		if (hDC == 0) goto gdi_error;
		GetWindowRect(hwObjDialog, &rectWin);
		if (bHelpWin) {
			SetWindowText(hwObjDialog, "plot window special keys");
			hwCtrl = WuEditBig(hwObjDialog, IDC_PLOBJ_HELP,
					5, iYpx, rectWin.right - rectWin.left - 10, 200);
			if (hwCtrl == 0) goto gdi_error;
			iYpx += 200;
			WuEditPrintf(hwCtrl,
				"NOTE: In the discussion below, LEFT, RIGHT, UP, and DOWN\n"
				"refer to the left, right, up, and down arrow keys, respectively.\n"
				" \n"
				"F1...brings up this screen.\n"
				"N...selects next object.\n"
				"P...selects previous object.\n"
				"CTRL+P...prints the plot window.\n"
				" \n");
			WuEditPrintf(hwCtrl,
				"If there is a selected object:\n"
				"	DEL (or BACKSPACE)...deletes the object.\n"
				"	B, C, or T...puts the mouse pointer at various places\n"
				"		on the object so arrow keys can move or resize it.\n"
				"	E...brings up an edit dialog (for label objects).\n"
				"	Z...zooms the plot so the box fills the plot area\n"
				"		(object must be a box).\n"
				"	R...restores the plot to its pre-zoom state.\n"
				"	ARROW...move or resize an object.  The placement of the\n"
				"		mouse pointer on the object determines the operation.\n"
				"		The key moves the pointer in the direction associated\n"
				"		with the key ARROW moves a small amount;\n"
				"		SHIFT+ARROW moves a larger amount;\n"
				"		CTRL+ARROW moves to the adjacent plot area.\n"
				" \n");
			WuEditPrintf(hwCtrl,
				"If there isn't a selected object, and the plot window has a\n"
				"plot cursor, the arrow keys move the plot cursor in the\n"
				"direction associated with the key:\n"
				"	LEFT and RIGHT...ARROW moves the cursor a small amount;\n"
				"		SHIFT+ARROW moves a larger amount; CTRL+ARROW\n"
				"		moves to the adjacent plot area.\n"
				"	UP and DOWN...ARROW moves to the next data trace in\n"
				"		the plot area; CTRL+ARROW moves to the adjacent\n"
				"		plot area.\n"
				" \n");
			WuEditPrintf(hwCtrl,
				"When the plot is zoomed, HOME, END, PAGEUP, and PAGEDOWN\n"
				"scroll the plot.  Without SHIFT, the vertical scroll bar\n"
				"is scrolled in the usual way.  With SHIFT, the horizontal\n"
				"bar is scrolled, HOME and PAGEUP moving to the left and\n"
				"END and PAGEDOWN moving to the right.\n");
			iYpx += 5;
			hwCtrl = WuButton(hwObjDialog, IDC_PLOBJ_OK, "OK", 
					(rectWin.right - rectWin.left) / 2 - 50, iYpx, 100, 22);
			if (hwCtrl == 0) goto gdi_error;
			iYpx += 27;
		}
		else {
			SetWindowText(hwObjDialog, "edit object");
			// Do a preliminary paint of whatever is needed in the plot object
			// edit 'dialog', and use the resulting height and width of the
			// contents to:
			//	o	position the OK and/or Cancel buttons; and
			//	o	set the final size of the plot object edit 'dialog'.
			PlObjWinPaint(pPlot, hwObjDialog, 0, &rectWin, hDC, &iYpx);
			iYpx += 5;
			hwCtrl = WuButton(hwObjDialog, IDC_PLOBJ_OK, "OK", 
					(rectWin.right - rectWin.left) / 2 - 105, iYpx, 100, 22);
			if (hwCtrl == 0) goto gdi_error;
			hwCtrl = WuButton(hwObjDialog, IDC_PLOBJ_CANCEL, "Cancel", 
					(rectWin.right - rectWin.left) / 2 + 5, iYpx, 100, 22);
			if (hwCtrl == 0) goto gdi_error;
			iYpx += 27;
		}
		rectWin.bottom = rectWin.top + iYpx +
						GetSystemMetrics(SM_CYCAPTION) +
						GetSystemMetrics(SM_CYBORDER);
		MoveWindow(hwObjDialog, rectWin.left, rectWin.top,
						rectWin.right - rectWin.left + 1,
						rectWin.bottom - rectWin.top + 1, FALSE);
		ShowWindow(hwObjDialog, SW_SHOW);
		SetFocus(pPlot->pObjCtx->hwObjTextBox);
	}

done:
	if (hDC != 0) ReleaseDC(hwObjDialog, hDC);
	return retStat;
gdi_error:
	if (hwObjDialog == 0)
		DestroyWindow(hwObjDialog);
	pPlot->pObjCtx->bObjEditDone = 1;
	retStat = PL_GDI_FAIL;
	goto done;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjWinPaint - paint the information in the plot object edit 'dialog'
*
* Purpose:
*		Paints the information in the plot object edit 'dialog'.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-28-96 R. Cole		created, as a clone from pl_prt.c
*--------------------------------------------------------------------------*/
void PASCAL
PlObjWinPaint(
PL_CTX	*pPlot,			// IO pointer to plot context struct
HWND	hwObjDialog,	// I handle of the window
int		bPaint,			// I 1 to print, 0 to lay out window
RECT	*prWin,			// I pointer to window's rect
HDC		hDC,			// IO handle to DC for edit 'dialog'
int		*piYpx)			// IO pointer to Y position
{
	HWND	hwEdit;
	PL_LABEL *pLabel;

	if (!bPaint) {
		pLabel = (PL_LABEL *)pPlot->pObjCtx->pDialogObj;
		hwEdit = WuEdit(hwObjDialog, IDC_PLOBJ_EDIT_BOX,
			5, *piYpx, prWin->right - prWin->left - 9, 25);
		pPlot->pObjCtx->hwObjTextBox = hwEdit;
		if (pLabel != NULL)
			WuEditSetText(hwEdit, pLabel->szText);
		*piYpx += 25;
	}

}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjWinProc - handle events for a plot object edit 'dialog'
*
* Purpose:
*		Handles events for a plot object edit 'dialog'.
*
* Return Value:
*		
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-28-96 R. Cole		created, as a clone from pl_prt.c
*--------------------------------------------------------------------------*/
LRESULT EXPORT16 CALLBACK
PlObjWinProc(
HWND	hWnd,
UINT	iMsg,
WPARAM	wParam,
LPARAM	lParam)
{
	PL_CTX	*pPlot=(PL_CTX *)GetWindowLong(hWnd, 0);
	PL_LABEL *pLabel;

	if (iMsg == WM_COMMAND) {
		if (wParam == IDC_PLOBJ_OK) {
			pLabel = (PL_LABEL *)pPlot->pObjCtx->pDialogObj;
			if (pLabel != NULL) {
				WuEditGetText(pPlot->pObjCtx->hwObjTextBox,
											pLabel->szText, PL_MSG_DIM);
			}
			goto done_exit;
		}
		else if (wParam == IDC_PLOBJ_CANCEL)
			goto done_exit;
		else
			goto default_proc;
	}
	else if (iMsg == WM_PAINT) {
		PAINTSTRUCT ps;
		HDC		hDC;
		RECT	rectWin;
		int		iYpx=5;

		hDC = BeginPaint(hWnd, &ps);
		GetWindowRect(hWnd, &rectWin);
		PlObjWinPaint(pPlot, hWnd, 1, &rectWin, hDC, &iYpx);
		EndPaint(hWnd, &ps);
	}
	else if (iMsg == WM_KEYDOWN) {
		if (wParam == VK_RETURN)
			goto done_exit;
		else {
			MessageBeep(0);
			goto default_proc;
		}
	}
	else if (iMsg == WM_SYSCOMMAND) {
		if (wParam == SC_CLOSE)
			DestroyWindow(hWnd);
		else
			goto default_proc;
	}
	else
		goto default_proc;

default_proc:
	return DefWindowProc(hWnd, iMsg, wParam, lParam);
done_exit:
	pPlot->pObjCtx->bObjEditDone = 1;
	DestroyWindow(pPlot->pObjCtx->hwObjDialog);
	pPlot->pObjCtx->hwObjDialog = 0;
	pPlot->pObjCtx->pDialogObj = NULL;
	SetFocus(pPlot->hwPlot);
	return 0;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlObjWrapup - wrap up plot object operations
*
* Purpose:
*		Wraps up plot object operations, freeing any memory that has been
*		allocated.
*
* Return Value:
*		PL_OK, or
*		PL_DIALOG_BUSY if a PL.LIB dialog box is busy
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
int PASCAL
PlObjWrapup(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	int		retStat=PL_OK;
	PL_OBJ	*pObj;

	if (pPlot->pObjCtx->hwObjDialog != 0) {
		// Attempt to cancel the dialog.
		PlObjWinDialog(pPlot, pPlot->hwPlot, 0, NULL);
		retStat = PL_DIALOG_BUSY;
		goto done;
	}

	while (pPlot->pObjCtx->pHead != NULL) {
		pObj = pPlot->pObjCtx->pHead;
		DLIST_REMOVE(pObj, pPlot->pObjCtx->pHead, pPlot->pObjCtx->pTail);
		if (pObj->eObjType == PL_OBJ_TY_LEGEND) {
			PL_LEGEND *pLegend=(PL_LEGEND *)pObj;
			PL_LEGEND_ENTRY *pEntry;
			for (pEntry=pLegend->pHead; pEntry!=NULL; pEntry=pLegend->pHead) {
				DLIST_REMOVE(pEntry, pLegend->pHead, pLegend->pTail);
				free(pEntry);
			}
		}
		free(pObj);
	}
	if (pPlot->pObjCtx->hwObjDialog != 0) {
		pPlot->pObjCtx->bObjEditDone = 1;
		WuDoEvents();
		DestroyWindow(pPlot->pObjCtx->hwObjDialog);
	}
	PlObjInit(pPlot);

done:
	return retStat;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinMouseInit - initialize PL.LIB pointers
*
* Purpose:
*		Creates pointers that will be used by PL.LIB when the mouse pointer
*		is over the plot window.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinMouseInit(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	HDC		hDC=0, hDCBM=0;
	HBITMAP	hBM;
	HPEN	hPen=0;
	int		nCursX=GetSystemMetrics(SM_CXCURSOR);
	int		nCursY=GetSystemMetrics(SM_CYCURSOR);
	int		iXR=nCursX-1, iXC=iXR/2, iYB=nCursY-1, iYC=iYB/2;
	int		nByteArea=(nCursX / 8) * nCursY;
	int		iXOffset45, iYOffset45;
	char	*pszAND=NULL, *pszXOR=NULL;
	POINT	aptL[]={0,iYC, 3,iYC-2, iXC-5,iYC-2,
								iXC-2,iYC, iXC-5,iYC+2, 3,iYC+2};
	POINT	aptR[]={iXR-1,iYC, iXR-4,iYC+2, iXC+5,iYC+2,
								iXC+2,iYC, iXC+5,iYC-2, iXR-4,iYC-2};
	POINT	aptT[]={iXC,0, iXC+2,3, iXC+2,iYC-5,
								iXC,iYC-2, iXC-2,iYC-5, iXC-2,3};
	POINT	aptB[]={iXC,iYB-1, iXC-2,iYB-4, iXC-2,iYC+5,
								iXC,iYC+2, iXC+2,iYC+5, iXC+2,iYB-4};
	int		nPtResize=sizeof(aptL)/sizeof(POINT);
	POINT	aptMove[]={0,iYC, 3,iYC-2, iXC-2,iYC-2, iXC-2,3,
						iXC,0, iXC+2,3, iXC+2,iYC-2, iXR-4,iYC-2,
						iXR-1,iYC, iXR-4,iYC+2, iXC+2,iYC+2, iXC+2,iYB-4,
						iXC,iYB-1, iXC-2,iYB-4, iXC-2,iYC+2, 3,iYC+2};
	int		nPtMove=sizeof(aptMove)/sizeof(POINT);
	POINT	aptArrow[7];

	memset(pMouse, 0, sizeof(PL_MOUSE));

	pMouse->hPointerPlus = pMouse->hPointerCreate = pMouse->hPointerSelect =
					pMouse->hPointerSize = pMouse->hPointerMove =
					pMouse->hPointerArrow = pMouse->hPointerNOT = 0;

//	pMouse->hPointerCurr = pMouse->hPointerNew = pMouse->hPointerEdit = 0;
	pMouse->hPointerCurr = pMouse->hPointerEdit = 0;

	// Set up the masks that will be used to create the pointer.
	// pszAND is filled with all 1's, and pszXOR is filled in with drawing
	// operations using hDCBM.  With pszAND being 1, then the pointer will
	// be transparent where pszXOR is 0 and inverted-color where pszXOR is 1.
	if ((pszAND = (char *)malloc(nByteArea)) == NULL) goto gdi_error;
	memset(pszAND, 0xFF, nByteArea);
	if ((pszXOR = (char *)malloc(nByteArea)) == NULL) goto gdi_error;

	// Set up a pointer-sized bitmap (1 color plane and 1 bit per pixel) and
	// a DC for drawing in it.
	if ((hBM = CreateBitmap(nCursX, nCursY, 1, 1, NULL)) == 0) goto gdi_error;
	if ((hDC = GetDC(pPlot->hwPlot)) == 0) goto gdi_error;
	if ((hDCBM = CreateCompatibleDC(hDC)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(NULL_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(WHITE_PEN))) == 0) goto gdi_error;
	SelectObject(hDCBM, hBM);

	// Draw the inverted-color 'plus'.  First fill the bitmap with 0's, which
	// will produce 'transparent' bits in pszXOR.  Then draw the 'plus' with
	// 1's, which will produce inverted-color bits in pszXOR.
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, BLACKNESS)) == 0) goto gdi_error;
	MoveToEx(hDCBM,		0,			nCursY/2,	NULL);
	LineTo(hDCBM,		nCursX-1,	nCursY/2	);
	MoveToEx(hDCBM,		nCursX/2,	0,			NULL);
	LineTo(hDCBM, 		nCursX/2,	nCursY-1	);
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerPlus = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						nCursX/2, nCursY/2, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerPlus == 0) goto gdi_error;

	// Make another pointer, with a rectangle over the 'plus'.
	Rectangle(hDCBM, nCursX/4, nCursY/4, 1+3*nCursX/4, 1+3*nCursY/4);
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerCreate = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						nCursX/2, nCursY/2, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerCreate == 0) goto gdi_error;

	// Add little anchor points to the corners of the rectangle.
	if ((SelectObject(hDCBM, GetStockObject(WHITE_BRUSH))) == 0) goto gdi_error;
	Rectangle(hDCBM, -1+nCursX/4, -1+nCursY/4, 3+nCursX/4, 3+nCursY/4);
	Rectangle(hDCBM, -2+3*nCursX/4, -1+nCursY/4, 2+3*nCursX/4, 3+nCursY/4);
	Rectangle(hDCBM, -2+3*nCursX/4, -2+3*nCursY/4, 2+3*nCursX/4, 2+3*nCursY/4);
	Rectangle(hDCBM, -1+nCursX/4, -2+3*nCursY/4, 3+nCursX/4, 2+3*nCursY/4);
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerSelect = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						nCursX/2, nCursY/2, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerSelect == 0) goto gdi_error;

	// AND color	XOR color	result
	// BLACK		BLACK		BLACK
	// BLACK		WHITE		WHITE
	// WHITE		BLACK		transparent
	// WHITE		WHITE		inverted

	// Make a special edit cursor as a cross with an empty center.  This
	// is needed so that the hot spot isn't obscured during resize.
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, WHITENESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptL, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptR, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptT, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptB, nPtResize) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszAND);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, BLACKNESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(WHITE_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptL, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptR, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptT, nPtResize) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptB, nPtResize) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerSize = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						iXC, iYC, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerSize == 0) goto gdi_error;

	// Make a special edit cursor as a cross with a solid center.  This
	// is created for a move, to match the resize cursor above.
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, WHITENESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptMove, nPtMove) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszAND);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, BLACKNESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(WHITE_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptMove, nPtMove) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerMove = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						iXC, iYC, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerMove == 0) goto gdi_error;

	// Make an arrow cursor.
	aptArrow[0].x = aptArrow[0].y = 0;
	aptArrow[1].x = (int)(iXC * .75F); aptArrow[1].y = (int)(iYC * .75F);
	aptArrow[6].x = 0; aptArrow[6].y = (int)(aptArrow[1].y * 1.5F);
	aptArrow[2].x = 2 * aptArrow[1].x / 3; aptArrow[2].y = aptArrow[1].y;
	aptArrow[5].x = 1 + aptArrow[1].x / 3;
				aptArrow[5].y = aptArrow[6].y - 3 - (aptArrow[6].y - aptArrow[1].y) / 3;
	aptArrow[4].x = aptArrow[5].x * 2; aptArrow[4].y = aptArrow[1].y * 2 - 2;
	aptArrow[3].x = aptArrow[4].x + (aptArrow[2].x - aptArrow[5].x);
				aptArrow[3].y = aptArrow[4].y - (aptArrow[5].y - aptArrow[2].y);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, WHITENESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptArrow, 7) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszAND);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, BLACKNESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(WHITE_BRUSH))) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_PEN))) == 0) goto gdi_error;
	if (Polygon(hDCBM, aptArrow, 7) == 0) goto gdi_error;
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerArrow = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						0, 0, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerArrow == 0) goto gdi_error;

	// Make one last pointer with the international "prohibited" circle-and-slash.
	// (This uses a thicker pen, for a bold effect.)
	iXOffset45 = (int)(.707F * nCursX / 4.F);
	iYOffset45 = (int)(.707F * nCursY / 4.F);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, WHITENESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(BLACK_BRUSH))) == 0) goto gdi_error;
	if ((hPen = CreatePen(PS_SOLID, 2, BLACK)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, hPen)) == 0) goto gdi_error;
	Ellipse(hDCBM, -1+nCursX/4, -1+nCursY/4, 2+3*nCursX/4, 2+3*nCursY/4);
	MoveToEx(hDCBM, nCursX/2-iXOffset45, nCursY/2-iYOffset45-1, NULL);
	LineTo(hDCBM, nCursX/2+iXOffset45, nCursY/2+iYOffset45-1);
	SelectObject(hDCBM, GetStockObject(BLACK_PEN));
	DeleteObject(hPen);
	hPen = 0;
	GetBitmapBits(hBM, nByteArea, pszAND);
	if ((PatBlt(hDCBM, 0, 0, nCursX, nCursY, BLACKNESS)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, GetStockObject(WHITE_BRUSH))) == 0) goto gdi_error;
	if ((hPen = CreatePen(PS_SOLID, 2, WHITE)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, hPen)) == 0) goto gdi_error;
	Ellipse(hDCBM, -1+nCursX/4, -1+nCursY/4, 2+3*nCursX/4, 2+3*nCursY/4);
	SelectObject(hDCBM, GetStockObject(BLACK_PEN));
	DeleteObject(hPen);
	hPen = 0;
	if ((SelectObject(hDCBM, GetStockObject(NULL_BRUSH))) == 0) goto gdi_error;
	if ((hPen = CreatePen(PS_SOLID, 2, BLACK)) == 0) goto gdi_error;
	if ((SelectObject(hDCBM, hPen)) == 0) goto gdi_error;
	Ellipse(hDCBM, nCursX/4, nCursY/4, 1+3*nCursX/4, 1+3*nCursY/4);
	MoveToEx(hDCBM, nCursX/2-iXOffset45, nCursY/2-iYOffset45-1, NULL);
	LineTo(hDCBM, nCursX/2+iXOffset45, nCursY/2+iYOffset45-1);
	SelectObject(hDCBM, GetStockObject(BLACK_PEN));
	DeleteObject(hPen);
	hPen = 0;
	GetBitmapBits(hBM, nByteArea, pszXOR);
	pMouse->hPointerNOT = CreateCursor(WuGetHInstance(pPlot->hwPlot),
						nCursX/2, nCursY/2, nCursX, nCursY, pszAND, pszXOR);
	if (pMouse->hPointerNOT == 0) goto gdi_error;

gdi_error:
	if (hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
	if (hDCBM != 0) DeleteObject(hDCBM);
	if (hBM != 0) DeleteObject(hBM);
	if (hPen != 0) DeleteObject(hPen);
	if (pszAND != NULL) free(pszAND);
	if (pszXOR != NULL) free(pszXOR);
	if (hDC != 0) ReleaseDC(pPlot->hwPlot, hDC);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinMousePreProcess - mouse-related pre-processing for events
*
* Purpose:
*		Performs preliminary mouse-related processing for selected Windows
*		events:
*		o	gets the mouse position, in plot window coordinates
*		o	gets the state of the SHIFT and CTRL keys
*		o	captures and releases the mouse
*		o	decides whether the mouse is in a "click" or "drag" state
*		o	makes a preliminary selection for a new mouse pointer
*
* Return Value:
*		void
*
* BUGS:
* 1.	Doesn't deal with the cases where more than 1 mouse button is
*		pressed.
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinMousePreProcess(
PL_CTX	*pPlot,			// IO pointer to plot context structure
HWND	hWnd,			// I handle for plot window
UINT	iMsg,			// I WM_xxx message code
WPARAM	wParam,			// I wParam for message
LPARAM	lParam)			// I lParam for message
{
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ	*pObj;
	int		rowSave=pPlot->iAxisRow, colSave=pPlot->iAxisCol;
	int		bShift=(GetKeyState(VK_SHIFT) < 0) ? 1 : 0;
	int		bCtrl=(GetKeyState(VK_CONTROL) < 0) ? 1 : 0;
	POINT	ptMouse;
	RECT	*pRect;
	double	dWidth, dHeight, dSizeSquared;

	pMouse->bUseNOTPointer = 0;

	if (iMsg != WM_LBUTTONDOWN && iMsg != WM_LBUTTONUP &&
							iMsg != WM_PAINT &&
							iMsg != WM_MBUTTONDOWN && iMsg != WM_MBUTTONUP &&
							iMsg != WM_RBUTTONDOWN && iMsg != WM_RBUTTONUP &&
							iMsg != WM_KEYDOWN && iMsg != WM_KEYUP &&
							iMsg != WM_MOUSEMOVE && iMsg != WM_SETCURSOR) {
		goto done;
	}

	pMouse->bInPlotWin = pMouse->bInOtherArea = pMouse->bInCurrArea =
									pMouse->bPlotCursorClick =
									pMouse->bHitNonSel = pMouse->bHitSelected = 0;
	pMouse->pHitObj = NULL;
	pMouse->iHitType = -1;
	pMouse->hPointerHit = 0;

	// Convert the screen coordinates for the mouse pointer into pixel
	// coordinates in the plot window.  If the .x and/or .y is in a plot
	// area column or row, also get the corresponding data value.
	GetCursorPos(&ptMouse);
	ScreenToClient(hWnd, &ptMouse);
	PlObjCvtXPxYPxToCoord(pPlot, ptMouse.x, ptMouse.y, &pMouse->coCurr);

	// Set appropriate flag if pointer is in a data area.
	if (pMouse->coCurr.y.iRowCol == rowSave && pMouse->coCurr.x.iRowCol == colSave)
		pMouse->bInCurrArea = 1;
	else if (pMouse->coCurr.y.iRowCol >= 0 && pMouse->coCurr.x.iRowCol >= 0)
		pMouse->bInOtherArea = 1;

	pRect = &pPlot->rectPlot;
	if (ptMouse.x >= pRect->left && ptMouse.x <= pRect->right &&
						ptMouse.y >= pRect->top && ptMouse.y <= pRect->bottom) {
		pMouse->bInPlotWin = 1;
	}

	pMouse->bShift = bShift;
	pMouse->bCtrl = bCtrl;
	if (iMsg == WM_LBUTTONDOWN || iMsg == WM_MBUTTONDOWN || iMsg == WM_RBUTTONDOWN) {
		pMouse->bLeftClick = pMouse->bRightClick = pMouse->bMiddleClick = 0;
		if (iMsg == WM_LBUTTONDOWN)
			pMouse->bLeftDown = pMouse->bLeftLast = pMouse->bLeftClick = 1;
		else if (iMsg == WM_RBUTTONDOWN)
			pMouse->bRightDown = pMouse->bRightLast = pMouse->bRightClick = 1;
		else
			pMouse->bMiddleDown = pMouse->bMiddleLast = pMouse->bMiddleClick = 1;
		pMouse->coDown = pMouse->coCurr;
		SetCapture(hWnd);
	}
	else if (iMsg == WM_LBUTTONUP || iMsg == WM_MBUTTONUP || iMsg == WM_RBUTTONUP) {
		pMouse->bLeftDown = pMouse->bRightDown = pMouse->bMiddleDown = 0;
		ReleaseCapture();
	}
	else if (iMsg == WM_MOUSEMOVE) {
		if (pPlot->pObjectFn != NULL && !pObjCtx->bKbEditActive) {
			pPlot->pObjectFn(pPlot, pPlot->pObjectArg, pObjCtx, NULL, PL_MOUSE_MOVE,
					pObjCtx->bCreateActive || pObjCtx->bEditActive ? 1. : 0., 0.);
		}
		if (pMouse->bLeftDown || pMouse->bRightDown || pMouse->bMiddleDown) {
			dWidth = fabs(ptMouse.x - pMouse->coDown.x.dVirtPx);
			dHeight = fabs(ptMouse.y - pMouse->coDown.y.dVirtPx);
			dSizeSquared = dWidth * dWidth + dHeight * dHeight;
			if (pObjCtx->bEditActive)
				pMouse->bLeftClick = pMouse->bRightClick = pMouse->bMiddleClick = 0;
			else if (dSizeSquared < 10.) {
				;	// No action--this still looks like a click.
			}
			else {
				// Up till now, we've been regarding this "mouse down" as a
				// click.  Now the pointer has moved more than the 'click
				// threshold', so we reclassify as a move, not a click.
				pMouse->bLeftClick = pMouse->bRightClick = pMouse->bMiddleClick = 0;
			}
		}
	}

	if (pMouse->bInPlotWin) {
		// Find out if the mouse pointer hits an object.  If it does, the hit
		// type will be one of the PL_OBJ_xxx codes.
		//
		// If there is a selected object, it is treated as the primary target.
		// Thus, if the mouse pointer is in the interior of the selected object,
		// it will be reported as the hit--even if the mouse pointer is on the
		// perimeter of another object.
		if (pObjCtx->pSelObj != NULL) {                      
			pMouse->iHitType = PlObjHitTest(pPlot,
									pObjCtx, pObjCtx->pSelObj, &pMouse->coCurr);
			if (pMouse->iHitType >= 0) {
				pMouse->bHitSelected = 1;
				pMouse->pHitObj = pObjCtx->pSelObj;
			}
		}
		if (pObjCtx->mode.bEditMode && !pMouse->bHitSelected) {
			for (pObj=pObjCtx->pHead; pObj!=NULL; pObj=pObj->pNext) {
				pMouse->iHitType = PlObjHitTest(pPlot,
											pObjCtx, pObj, &pMouse->coCurr);
				if (pMouse->iHitType >= 0) {
					pMouse->bHitNonSel = 1;
					pMouse->pHitObj = pObj;
					break;
				}
			}
		}
	}

	// If it's needed, select (but not "install") a new pointer.
	PlWinMouseSelectPointer(pPlot, hWnd, iMsg, wParam, lParam);

done:
	return;
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinMouseSelectPointer - select a new mouse pointer
*
* Purpose:
*		Makes a preliminary selection for a mouse pointer, based on the
*		assumption that no box is being edited.  (Depending on what
*		happens with editing during the current Windows event, the mouse
*		pointer that's chosen here may or may not be 'installed'.)
*
*		The handle for the new pointer is stored in pMouse->hPointerNew, but
*		a SetCursor call isn't made.  The responsibility for actually
*		setting the pointer lies with the caller.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinMouseSelectPointer(
PL_CTX	*pPlot,			// IO pointer to plot context structure
HWND	hWnd,			// I handle for plot window
UINT	iMsg,			// I WM_xxx message code
WPARAM	wParam,			// I wParam for message
LPARAM	lParam)			// I lParam for message
{
	PL_MOUSE *pMouse=pPlot->pMouse;
	PL_OBJ_CTX *pObjCtx=pPlot->pObjCtx;
	int		bInAnyArea=pMouse->bInCurrArea || pMouse->bInOtherArea;

/*---------------------------------------------------------------------------
*	If an edit/create is active, then the PlObjEdit routines are in
*	charge of pointer selection.  Generally, they use hPointerEdit unless
*	a temporary condition exists where a "NOT!" cursor is needed.  In
*	such cases, those routines set the bUseNOTPointer flag.
*
*	If an edit/create is not active, then the choices are
*	o	if the selected box is hit, use the edit pointer; else
*	o	if a non-selected box is hit, use the select pointer; else
*	o	if create is possible, use the create pointer; else
*	o	if the mouse pointer is in a data area and plot cursor operations
*		are enabled, use the cursor pointer; else
*	o	use the arrow pointer
*--------------------------------------------------------------------------*/

	if (pMouse->bHitSelected)
		pMouse->hPointerNew = pMouse->hPointerHit;
	else if (pMouse->bHitNonSel)
		pMouse->hPointerNew = pMouse->hPointerSelect;
	else if (pObjCtx->mode.bEditMode && pObjCtx->mode.bEnbBoxNew && bInAnyArea &&
							(pObjCtx->nObjects < 1 || pObjCtx->mode.bEnbBoxMany)) {
		pMouse->hPointerNew = pMouse->hPointerCreate;
	}
	else if (pPlot->bEnbCursor && bInAnyArea)
		pMouse->hPointerNew = pMouse->hPointerPlus;
	else
		pMouse->hPointerNew = pMouse->hPointerArrow;//LoadCursor(NULL, IDC_ARROW);
}
/*+/csubr/TOC/internal-------------------------------------------------------
* PlWinMouseWrapup - wrapup PL.LIB pointers
*
* Purpose:
*		Destroys the pointers.
*
* Return Value:
*		void
*
*-Date     Author		Revision
* -------- ------------	--------
* 12-04-96 R. Cole		created
*--------------------------------------------------------------------------*/
void PASCAL
PlWinMouseWrapup(
PL_CTX	*pPlot)			// IO pointer to plot context structure
{
	PL_MOUSE *pMouse=pPlot->pMouse;

	if (pMouse->hPointerPlus != 0) DestroyCursor(pMouse->hPointerPlus);
	if (pMouse->hPointerCreate != 0) DestroyCursor(pMouse->hPointerCreate);
	if (pMouse->hPointerSelect != 0) DestroyCursor(pMouse->hPointerSelect);
	if (pMouse->hPointerSize != 0) DestroyCursor(pMouse->hPointerSize);
	if (pMouse->hPointerMove != 0) DestroyCursor(pMouse->hPointerMove);
	if (pMouse->hPointerArrow != 0) DestroyCursor(pMouse->hPointerArrow);
	if (pMouse->hPointerNOT != 0) DestroyCursor(pMouse->hPointerNOT);

	pMouse->hPointerPlus = pMouse->hPointerCreate = pMouse->hPointerSelect =
					pMouse->hPointerSize = pMouse->hPointerMove =
					pMouse->hPointerArrow = pMouse->hPointerNOT = 0;
}
