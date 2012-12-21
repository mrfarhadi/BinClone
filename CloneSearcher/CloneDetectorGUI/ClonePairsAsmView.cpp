// ClonePairsAsmView.cpp : implementation file
//

#include "stdafx.h"
#include "CloneDetectorGUI.h"
#include "ClonePairsAsmView.h"
#include "ClonePairsAsmFrame.h"


// ClonePairsAsmView

IMPLEMENT_DYNCREATE(ClonePairsAsmView, CRichEditView)
IMPLEMENT_DYNCREATE(ClonePairsListView, ClonePairsAsmView)

ClonePairsAsmView::ClonePairsAsmView()
:m_binit(false),
 m_isListView(false),
 m_beginLine(-1),
 m_endLine(-1),
 m_fileId(-1)
{

}

ClonePairsAsmView::~ClonePairsAsmView()
{
}

ClonePairsAsmDoc* ClonePairsAsmView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ClonePairsAsmDoc)));
	return (ClonePairsAsmDoc*)m_pDocument;
}

void ClonePairsAsmView::initView(const CString & p_filename, const CString & p_content, int p_fileId)
{
	// TODO: Add your specialized code here and/or call the base class
	m_fileId = p_fileId;
	m_filename = p_filename;
	//Set the default font
	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED;
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0x00;
	

	wcscpy_s(cfDefault.szFaceName, _T("Courier"));
	//GetRichEditCtrl().SetBackgroundColor(false,GetSysColor((COLOR_BTNFACE)));//RGB( 224,255,255)); //GetSysColor((COLOR_BTNFACE)));
	//GetRichEditCtrl().SetBackgroundColor(false,RGB( 245,245,220));
	//GetRichEditCtrl().SetBackgroundColor(false,RGB( 240,247,249));

	CRichEditCtrl& richEditCtrl = GetRichEditCtrl();
	richEditCtrl.SetDefaultCharFormat(cfDefault);
	richEditCtrl.SetEventMask(ENM_PROTECTED);
	richEditCtrl.SetEventMask(ENM_MOUSEEVENTS);

	SetWordWrap(false);
	SetWindowText(p_content);
	GetRichEditCtrl().EnableWindow(true);

	FINDTEXTEX findText;
    findText.chrg.cpMin = 0;
    findText.chrg.cpMax = -1;
    GetRichEditCtrl().SetSel(findText.chrgText);// Select all Text
    SetProtected(); 
	SetWordWrap(false);
	GetRichEditCtrl().SetReadOnly(true);

	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();

    // Scroll the rich edit control so that the first visible line
    // is the first line of text.
	if (nFirstVisible > 0)
	{
		GetRichEditCtrl().LineScroll(-nFirstVisible, 0);
	}

	GetRichEditCtrl().HideSelection(TRUE,FALSE);

	m_binit = true;

	// test
	//highLightLines(0,2);
}



void ClonePairsAsmView::initCloneFile( CloneFile & p_cloneFile)
{
	m_isListView = true;
	m_filename = _T("Clone Pairs List");
	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED;
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0x00;
	

	//wcscpy(cfDefault.szFaceName, _T("Courier"));
	GetRichEditCtrl().SetBackgroundColor(false,GetSysColor((COLOR_BTNFACE)));//RGB( 224,255,255)); //GetSysColor((COLOR_BTNFACE)));
	//GetRichEditCtrl().SetBackgroundColor(false,RGB( 245,245,220));
	//GetRichEditCtrl().SetBackgroundColor(false,RGB( 240,247,249));

	CRichEditCtrl& richEditCtrl = GetRichEditCtrl();
	richEditCtrl.SetDefaultCharFormat(cfDefault);
	richEditCtrl.SetEventMask(ENM_PROTECTED);

	SetWordWrap(false);
	int numOfClonePairs = p_cloneFile.getNumberOfClonePairs();
	int iRow = 0;
	CString str(_T(""));
	while(iRow < numOfClonePairs )
	{
		ClonePair pair;
		if( p_cloneFile.getClonePair(iRow,pair))
		{
			CString tmp;
			tmp.Format(_T("[%2d] clone_pair regionA start=\"%d\" regionA end=\"%d\" \t\t regionB start=\"%d\" regionB end=\"%d\"\n"),
				          iRow+1,
				          pair.m_regionA.m_start,
						  pair.m_regionA.m_end,
						  pair.m_regionB.m_start,
						  pair.m_regionB.m_end);
			str += tmp;
			++iRow;
		}
		else
		{
			break;
		}
	}

	SetWindowText(str);
	//SetWordWrap(false);
	
	//SetString(outText);

	//SetWindowText(p_content);

	GetRichEditCtrl().EnableWindow(true);

	FINDTEXTEX findText;
    findText.chrg.cpMin = 0;
    findText.chrg.cpMax = -1;
    GetRichEditCtrl().SetSel(findText.chrgText);// Select all Text
    SetProtected(); 
	SetWordWrap(false);
	GetRichEditCtrl().SetReadOnly(true);

	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();

    // Scroll the rich edit control so that the first visible line
    // is the first line of text.
	if (nFirstVisible > 0)
	{
		GetRichEditCtrl().LineScroll(-nFirstVisible, 0);
	}

	GetRichEditCtrl().HideSelection(TRUE,FALSE);

	m_binit = true;

}

void ClonePairsAsmView::SetProtected(void)
{ 
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_PROTECTED;
 
	DWORD dwSelMask = GetRichEditCtrl().GetSelectionCharFormat(cf);
	// If selection is all the same toggle PROTECTED style
	// turn it on otherwise over the whole selection
	if( (cf.dwMask & CFM_PROTECTED) & (dwSelMask & CFM_PROTECTED) )
	{ 
		cf.dwEffects ^= CFE_PROTECTED; 
	}
	else
	{ 
		cf.dwEffects |= CFE_PROTECTED;
	}
	cf.dwMask = CFM_PROTECTED;
	GetRichEditCtrl().SetSelectionCharFormat(cf);
}

void ClonePairsAsmView::SetColour(COLORREF color)
{ 
	CHARFORMAT cf;
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR | CFM_BOLD;

	GetRichEditCtrl().GetSelectionCharFormat(cf);
	cf.crTextColor = color;
	//if( cf.dwEffects & CFE_AUTOCOLOR )
    { 
		cf.dwEffects ^= CFE_AUTOCOLOR;
	}
	GetRichEditCtrl().SetSelectionCharFormat(cf);
}

void ClonePairsAsmView::SetWordWrap(const bool bOn/*= true*/,const int iLineWidth/* = 0*/)
{ 
	if( bOn )
    {  
		GetRichEditCtrl().SetTargetDevice(NULL, iLineWidth);
	}
	else if( 0 == iLineWidth )
	{  
		GetRichEditCtrl().SetTargetDevice(NULL, 1);
	}
	else
	{  
		GetRichEditCtrl().SetTargetDevice(NULL, iLineWidth);
	}
}


BEGIN_MESSAGE_MAP(ClonePairsAsmView, CRichEditView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_VIEW_CLOSE32780, &ClonePairsAsmView::OnViewClose32780)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_MESSAGE(ID_MENU_COPY,OnCopy)
	ON_COMMAND(ID_TOKEN_VIEW_CLOSE, &ClonePairsAsmView::OnTokenViewClose)
END_MESSAGE_MAP()


// ClonePairsAsmView diagnostics

#ifdef _DEBUG
void ClonePairsAsmView::AssertValid() const
{
	CRichEditView::AssertValid();
}

#ifndef _WIN32_WCE
void ClonePairsAsmView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// ClonePairsAsmView message handlers


void ClonePairsAsmView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
}


void ClonePairsAsmView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CRichEditView::OnLButtonDown(nFlags, point);

	ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
	
	if( m_isListView)
	{
	    int line = 0;
	    line = GetRichEditCtrl().LineFromChar(-1);
		ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
		pFrame->selectedParticularLine(line);
		CString frameText;
		int id = pFrame->getCurId();
		CString xml = pFrame->getXMLFile();
		frameText.Format(_T("ID [%d] - %s"),pFrame->getCurId(),pFrame->getXMLFile());
		pFrame->SetWindowText(frameText);
		return;
	}
	
	pFrame->SetWindowText(m_filename); 
}

void ClonePairsAsmView::highLightLines(int p_begin, int p_end)
{
	if( !m_binit)
		return;

	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();

	if( (m_endLine >= 0) && (m_beginLine >= 0))
	{
		// reset the pervious selected
	    GetRichEditCtrl().SetSel(m_beginPos, m_endPos);
	    SetColour(RGB(0,0,0));
	    GetRichEditCtrl().HideSelection(TRUE,FALSE);
	}
	else
	{
		GetRichEditCtrl().SetSel(0, 0);
		GetRichEditCtrl().HideSelection(TRUE,FALSE);
	}

	// scroll the current selected line
	//GetRichEditCtrl().LineScroll(-p_begin, 0);

	int beginPos;
	int endPos;
	if ( ((beginPos=GetRichEditCtrl().LineIndex(p_begin)) != -1) &&
	     ((endPos=GetRichEditCtrl().LineIndex(p_end+1))   != -1) )
	{
        m_beginLine = p_begin;
		m_endLine   = p_end;
		m_beginPos  = beginPos;
		m_endPos    = endPos;

		GetRichEditCtrl().SetSel(beginPos, endPos);
		SetColour(RGB(255,0,0));

		nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();
		
		if(nFirstVisible > p_begin)
		{
			int linesToScrollUp = (p_end-p_begin)+1;
			GetRichEditCtrl().LineScroll(-linesToScrollUp, 0);
		}
	}

	GetRichEditCtrl().HideSelection(TRUE,FALSE);
}

void ClonePairsAsmView::OnViewClose32780()
{
	GetParentFrame()->PostMessage(WM_CLOSE);

}

void ClonePairsAsmView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default 
	if( m_isListView)
		return;

	CMenu menu;
	CPoint pt;
	::GetCursorPos(&pt);
	DWORD dwSelectionMade; VERIFY(menu.LoadMenu(IDR_MOUSE_MENU));
	CMenu *pmenuPopup = menu.GetSubMenu(0);
	pmenuPopup->EnableMenuItem(ID_MENU_PASTE,MF_GRAYED);
	ASSERT(pmenuPopup != NULL);                                    
	dwSelectionMade = pmenuPopup->TrackPopupMenu( (TPM_LEFTALIGN|
		                                           TPM_LEFTBUTTON|
												   TPM_NONOTIFY|
												   TPM_RETURNCMD),
												   pt.x, 
												   pt.y, 
												   this     
												   );  

	pmenuPopup->DestroyMenu();
	PostMessage(dwSelectionMade,0,0); 

	CRichEditView::OnRButtonDown(nFlags, point);
}

void ClonePairsAsmView::selectedLine(unsigned int p_line)
{
	ClonePairsAsmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CCSController * pController = pDoc->m_csController; 
	if( !pController)
	{
        AfxMessageBox(_T("CCSController is not valid."), MB_ICONSTOP, 0);					
		return;
    }

	ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();

	const CCSClones* pcsClones = pDoc->m_pClones;
	CString srcFile = pcsClones->GetAt(p_line)->m_srcFilePath; 
    CString tarContent, srcContent;

	if( pDoc->m_currentTarFilePathAndName != pDoc->m_targetFilePathAndName)
	{
		// Get the content of target file
		if (!pController->openRawAssemblyFile(pDoc->m_targetFilePathAndName))
		{
			CString error;
			error.Format(_T("%s is not found!"),pDoc->m_targetFilePathAndName);
			AfxMessageBox(error, MB_ICONSTOP,0);
			PostMessage(WM_CLOSE);
			return;  // error
		}

		pDoc->m_currentTarFilePathAndName = pDoc->m_targetFilePathAndName;

		int lineIdx = 0;
		CString lineStr, lineText;
		while (pController->getRawAssemblyFileLineStr(lineStr)) 
		{
			lineText.Format(_T("%5d: %s\n"), lineIdx++, lineStr);
			tarContent += lineText;
		}

		pController->closeRawAssemblyFile();
		pFrame->displayTarAsmContents(pDoc->m_targetFilePathAndName,tarContent);
	}

	if( pDoc->m_currentSrcFilePathAndName != srcFile)
	{	
		// Get the content of source file
		if (!pController->openRawAssemblyFile(srcFile))
		{
			CString error;
			error.Format(_T("%s is not found!"),srcFile);
			AfxMessageBox(error, MB_ICONSTOP,0);
			PostMessage(WM_CLOSE);
			return;  // error
		}

		pDoc->m_currentSrcFilePathAndName = srcFile;

		int lineIdx = 0;
		CString lineStr, lineText;
		while (pController->getRawAssemblyFileLineStr(lineStr)) 
		{
			lineText.Format(_T("%5d: %s\n"), lineIdx++, lineStr);
			srcContent += lineText;
		}
        
		pController->closeRawAssemblyFile();
		pFrame->displaySrcAsmContents(srcFile,srcContent);
	}

	pFrame->fillSelectedClonePairsOnViews2(p_line, 
		                                   pcsClones->GetAt(p_line)->m_tarRawStart,pcsClones->GetAt(p_line)->m_tarRawEnd,
		                                   pcsClones->GetAt(p_line)->m_srcRawStart,pcsClones->GetAt(p_line)->m_srcRawEnd);

	
	/*
	pFrame->selectedParticularLine(line);
		
	CString frameText;
	int id = pFrame->getCurId();
	CString xml = pFrame->getXMLFile();
	frameText.Format(_T("ID [%d] - %s"),pFrame->getCurId(),pFrame->getXMLFile());
	pFrame->SetWindowText(frameText);
	*/
	return;
}
	
LRESULT ClonePairsAsmView::OnCopy(WPARAM, LPARAM)
{
	GetRichEditCtrl().Copy();
	return 1;
}


void ClonePairsAsmView::OnTokenViewClose()
{
	// TODO: Add your command handler code here
	GetParentFrame()->PostMessage(WM_CLOSE);
}


ClonePairsListView::ClonePairsListView(void)
:ClonePairsAsmView()
{
}


ClonePairsListView::~ClonePairsListView(void)
{
}

/* version 2.0 */
void ClonePairsListView::initClonePairList()
{

	ClonePairsAsmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_isListView = true;
	m_filename = _T("Clone Pairs List");

	CHARFORMAT cfDefault;
	cfDefault.cbSize = sizeof(cfDefault);
	cfDefault.dwEffects = CFE_PROTECTED;
	cfDefault.dwMask = CFM_BOLD | CFM_FACE | CFM_SIZE | CFM_CHARSET | CFM_PROTECTED;
	cfDefault.yHeight = 200;
	cfDefault.bCharSet = 0x00;

	GetRichEditCtrl().SetBackgroundColor(false,GetSysColor((COLOR_BTNFACE)));//RGB( 224,255,255)); //GetSysColor((COLOR_BTNFACE)));
	
	CRichEditCtrl& richEditCtrl = GetRichEditCtrl();
	richEditCtrl.SetDefaultCharFormat(cfDefault);
	richEditCtrl.SetEventMask(ENM_PROTECTED);

	SetWordWrap(false);
	int iRow = 0;
	CString str(_T(""));

	const CCSClones* pcsClones = pDoc->m_pClones;
	for (int c = 0; c < pcsClones->GetSize(); ++c)
	{
		CString tmp;
		CString srcFile = pcsClones->GetAt(c)->m_srcFilePath; 
		srcFile	= srcFile.Mid(srcFile.ReverseFind(_TCHAR('\\'))+1);

		tmp.Format(_T("[%2d] target\tstart=\"%6d\" end=\"%6d\"\t\t\tsource: %s\tstart=\"%6d\" end=\"%6d\"\n"),
		          iRow+1,
				  pcsClones->GetAt(c)->m_tarRawStart,
				  pcsClones->GetAt(c)->m_tarRawEnd,
				  srcFile,
				  pcsClones->GetAt(c)->m_srcRawStart,
				  pcsClones->GetAt(c)->m_srcRawEnd);
						  
		str += tmp;
		++iRow;
		
	}

	SetWindowText(str);

	GetRichEditCtrl().EnableWindow(true);

	FINDTEXTEX findText;
    findText.chrg.cpMin = 0;
    findText.chrg.cpMax = -1;
    GetRichEditCtrl().SetSel(findText.chrgText);// Select all Text
    SetProtected(); 
	SetWordWrap(false);
	GetRichEditCtrl().SetReadOnly(true);

	int nFirstVisible = GetRichEditCtrl().GetFirstVisibleLine();

    // Scroll the rich edit control so that the first visible line
    // is the first line of text.
	if (nFirstVisible > 0)
	{
		GetRichEditCtrl().LineScroll(-nFirstVisible, 0);
	}

	GetRichEditCtrl().HideSelection(TRUE,FALSE);

	ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
	if( pFrame)
	{
		unsigned int numOfClonePairs = pcsClones->GetSize();
		pFrame->setNumOfClonePairs(numOfClonePairs);
		/*
		if( numOfClonePairs > 0)
		{
			// initialize the view with the line 0
			pFrame->setCurSelLine(0);
			selectedLine(0);
		    highLightLines(0,0);
		} 
		*/
	}
	m_binit = true;
	  
}

void ClonePairsListView::OnInitialUpdate()
{
	ClonePairsAsmView::OnInitialUpdate();

	ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);

	// TODO: Add your specialized code here and/or call the base class
	// NEWCHANGE 
	initClonePairList();
}
BEGIN_MESSAGE_MAP(ClonePairsListView, ClonePairsAsmView)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



void ClonePairsListView::OnLButtonDown(UINT nFlags, CPoint point)
{
	ClonePairsAsmView::OnLButtonDown(nFlags, point);
	// TODO: Add your message handler code here and/or call default
	

	unsigned int line = 0;
	line = GetRichEditCtrl().LineFromChar(-1);
	if( line >= 0 )
	{
		ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
		if( pFrame)
		{
			if( (line != pFrame->getCurSelLine()) && 
				(line < pFrame->getNumOfClonePairs()))
			{
		        selectedLine(line);
		        highLightLines(line,line);
				pFrame->setCurSelLine(line);
			}
		}
	}
	

	/*
	ClonePairsAsmView::OnLButtonDown(nFlags, point);

	ClonePairsAsmDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CCSController * pController = pDoc->m_csController; 
	if( !pController)
	{
        AfxMessageBox(_T("CCSController is not valid."), MB_ICONSTOP, 0);					
		return;
    }

	ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();
	
	if( m_isListView)
	{
	   int line = 0;
	   line = GetRichEditCtrl().LineFromChar(-1);
	   ClonePairsAsmFrame* pFrame = (ClonePairsAsmFrame*) GetParentFrame();

	   const CCSClones* pcsClones = pDoc->m_pClones;
	   CString srcFile = pcsClones->GetAt(line)->m_srcFilePath; 

	   CString tarContent, srcContent;

	   // Get the content of target file
	   if (!pController->openRawAssemblyFile(pDoc->m_targetFilePathAndName))
	   {
		   CString error;
		   error.Format(_T("%s is not found!"),pDoc->m_targetFilePathAndName);
		   AfxMessageBox(error, MB_ICONSTOP,0);
		   PostMessage(WM_CLOSE);
		   return;  // error
	   }

       int lineIdx = 0;
	   CString lineStr, lineText;
	   while (pController->getRawAssemblyFileLineStr(lineStr)) 
	   {
		   lineText.Format(_T("%5d: %s\n"), lineIdx++, lineStr);
		   tarContent += lineText;
	   }
       pController->closeRawAssemblyFile();

	   // Get the content of source file
	   if (!pController->openRawAssemblyFile(srcFile))
	   {
		   CString error;
		   error.Format(_T("%s is not found!"),srcFile);
		   AfxMessageBox(error, MB_ICONSTOP,0);
		   PostMessage(WM_CLOSE);
		   return;  // error
	   }

       lineIdx = 0;
	   while (pController->getRawAssemblyFileLineStr(lineStr)) 
	   {
		   lineText.Format(_T("%5d: %s\n"), lineIdx++, lineStr);
		   srcContent += lineText;
	   }
       pController->closeRawAssemblyFile();

	   pFrame->displayAsmContents(pDoc->m_targetFilePathAndName,tarContent,srcFile,srcContent);

	   pFrame->fillSelectedClonePairsOnViews2(pcsClones->GetAt(line)->m_tarRawStart,
		                                      pcsClones->GetAt(line)->m_tarRawEnd,
											  pcsClones->GetAt(line)->m_srcRawStart,
											  pcsClones->GetAt(line)->m_srcRawEnd);

	   highLightLines(line,line);
	   */
	
	   /*
	   pFrame->selectedParticularLine(line);
		
	   CString frameText;
		int id = pFrame->getCurId();
		CString xml = pFrame->getXMLFile();
		frameText.Format(_T("ID [%d] - %s"),pFrame->getCurId(),pFrame->getXMLFile());
		pFrame->SetWindowText(frameText);
		*/
		//return;
	
	
	//pFrame->SetWindowText(m_filename); 
}
