#pragma once

#include "UB92SetupInfo.h"

#define SCROLL_TOP_POS				((long)0)
#define SCROLL_BOTTOM_POS			((long)1250)
#define SCROLL_POS_PER_PAGE			((long)200)

// (j.jones 2008-06-12 11:14) - PLID 23052 - supported multiple pages of charges
void UpdateUB04Charges(int &firstcharge);//needed to page up/down

/////////////////////////////////////////////////////////////////////////////
// CUB04Dlg dialog

// (j.jones 2007-03-02 11:05) - PLID 23939 - created, migrated from UB92 files

class CUB04Dlg : public CDialog
{
// Construction
public:

	CUB92SetupInfo m_UB92Info;

	CString GetBox76_78ID(int Box76_78Setup, int Box76_78Number);
	void BuildBoxes76_78();

	CUB04Dlg(CWnd* pParent);   // standard constructor
	~CUB04Dlg();
	long m_ID;
	long m_PatientID;
	long m_InsuredPartyID;
	long m_OthrInsuredPartyID;

	BOOL m_ShowWindowOnInit;
	CPrintDialog *m_printDlg;

	BOOL m_bPrintWarnings;

	CString m_strBillName;

	long ScrollBottomPos;

	// (j.dinatale 2010-07-23) - PLID 39692 - New member variable to store a device context so we can pass it to the batchprintdlg
	CDC	*m_pPrintDC;

	// (j.dinatale 2010-07-28) - PLID 39803 - Flag to determine if the dialog should be handling ClaimsHistoryT, ClaimHistoryDetailsT, and HCFATrackT updates
	bool m_bUpdateClaimsTables;

	long DoScrollTo(long nNewTopPos);

	void (*m_pOnCommand)(WPARAM wParam, LPARAM lParam, CDWordArray& dwChangedForms, int FormControlID, class CFormDisplayDlg* dlg);
	void (*m_pOnKeyDown)(CDialog* pFormDisplayDlg, MSG* pMsg);
	BOOL (*m_pPrePrintFunc)(CDWordArray& dwChangedForms, CFormDisplayDlg* dlg);	

	void GetUB04GroupID();

	void OnClickPrint();

	////////////////////////////////////////
	// Saves the whole UB04
	void Save(BOOL boSaveAll, CDialog* pdlgWait, int& iPage);

	////////////////////////////////////////
	// Saves one page of the UB04
	void SaveHistoricalData(BOOL boSaveAll);

// Dialog Data
	//{{AFX_DATA(CUB04Dlg)
	enum { IDD = IDD_HCFA };
	BOOL	m_CapOnPrint;
	// (j.jones 2007-06-25 10:14) - PLID 25663 - changed the buttons to NxIconButtons
	CNxIconButton m_btnRestoreDefaults;
	CNxIconButton m_btnAlignForm;
	CNxIconButton m_btnSave;
	CNxIconButton m_btnPrint;
	CNxIconButton m_btnClose;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUB04Dlg)
	public:
	virtual int DoModal(int billID);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	// (j.jones 2007-06-22 13:31) - PLID 25665 - required for info text labeling
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFormDisplayDlg *m_pframe;
	CButton			*m_pleftbutton,
					*m_prightbutton,
					*m_pupbutton,
					*m_pdownbutton;

	void SetControlPositions(void);
	void SetUB04DateFormats();
	void BuildUB04ChargesT();
	void BuildFormsT_Form6();
	void FindBatch();
	void UpdateBatch();

	// Generated message map functions
	//{{AFX_MSG(CUB04Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioPaper();
	afx_msg void OnRadioElectronic();
	afx_msg void OnRadioNoBatch();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickX();
	afx_msg void OnClickCheck();
	afx_msg void OnClose();	
	afx_msg void OnClickAlign();
	afx_msg void OnClickCapitalizeOnPrint();
	afx_msg void OnClickRestore();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};