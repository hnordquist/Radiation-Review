#ifndef _ZAPPCOMDLG_INCLUDED
#define _ZAPPCOMDLG_INCLUDED 1
/*
 * (c) Copyright 1991 - 1996 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 **************************************************************************/


#define ZMAXFILELENGTH 256


class Z_DLLSPEC zFileOpenForm {
public:
	zFileOpenForm(zWindow*,const char *title,const char *fname=0,char *types[] =0);
	int completed() const;
	const char *name() const;

private:
	char fileName[ZMAXFILELENGTH];
	int _completed;
};

class Z_DLLSPEC zFileSaveAsForm {
public:
	zFileSaveAsForm(zWindow*,const char *title,const char *fname=0,char *types[] =0);
	int completed() const;
	const char *name() const;

private:
    char fileName[ZMAXFILELENGTH];
    int _completed;
};

#if defined(Z_OS2) || defined(Z_XM)
class Z_DLLSPEC zColorSelForm:public zDialogFrame {
#else
class Z_DLLSPEC zColorSelForm {
#endif
public:
    zColorSelForm(zWindow*,const zColor& c);
    zColorSelForm(zWindow*);
    ~zColorSelForm();

    int completed() const;
    const zColor& color() const;

private:
    int _completed;
    zColor clr;
public:
    void setup(zWindow*, zColor c);
};



class Z_DLLSPEC zFontSelForm {
public:
	zFontSelForm(zWindow*,zColor,zFontSpec* =0);
	~zFontSelForm();

	zFontSpec* fontSpec() const;
	const zColor& color() const;
	int completed() const;

private:

	zFontSpec *fs;
	zColor clr;
	int _completed;
};

inline int zFileOpenForm::completed() const { return _completed; }
inline const char* zFileOpenForm::name() const { return fileName; }
inline int zFileSaveAsForm::completed() const { return _completed; }
inline const char* zFileSaveAsForm::name() const { return fileName; }
inline int zColorSelForm::completed() const { return _completed; }
inline const zColor& zColorSelForm::color() const { return clr; }
inline zFontSpec* zFontSelForm::fontSpec() const { return fs; }
inline const zColor& zFontSelForm::color() const { return clr; }
inline int zFontSelForm::completed() const { return _completed; }


#endif

