// NexFormularyDlg.cpp : implementation file
//

// (b.savon 2013-08-14 14:04) - PLID 57587 - Created

#include "stdafx.h"
#include "Practice.h"
#include "NexFormularyDlg.h"
#include "PrescriptionUtilsNonAPI.h"
#include "PrescriptionUtilsAPI.h"
#include "SSEligibilityDlg.h"

using namespace NXDATALIST2Lib;

#define INVALID_ROW_SELECTION -1

//TES 8/29/2013 - PLID 57999 - Moved the color definitions to PrescruptionUtilsNonAPI.h

enum NexFormularyInsuranceColumns{
	nficID= 0,
	nficPatientID,
	nficInsuranceCoName,
	nficPBMID, // (s.dhole 2013-09-12 17:26) - PLID 58556
	nficPBMName, // (s.dhole 2013-09-12 17:26) - PLID 58556
	nficInsuranceResponsibilityID,
	nficInsuranceResponsibility,
	nficSureScriptsFormularyID,
	nficSureScriptsAlternativesID,
	nficSureScriptsCoverageID,
	nficSureScriptsCopayID,
	nficPlanName,// (s.dhole 2013-09-12 17:26) - PLID 58556
	nficPharmacyCoverage,// (s.dhole 2013-09-12 17:26) - PLID 58556
	nficLastSureScriptsRequestDate,// (s.dhole 2013-09-12 17:26) - PLID 58556
	nficDemographicsHaveChanged,
	nficDisplay,// (s.dhole 2013-09-12 17:26) - PLID 58556
	nficIsFailure,// (s.dhole 2013-10-07 17:50) - PLID 58886 Added IsFailure
};

enum NexFormularyDrugListColumns{
	nfdcDrugListID = 0,
	nfdcEMRDataID,
	nfdcName,
	nfdcFDBID,
	nfdcRowBackColor,
};

enum NexFormularyColumns{
	nfcDrugListID = 0,
	nfcFDBID,
	nfcNDC,
	nfcWrite,
	nfcMedicationAlternative,
	nfcFormularyStatus,// (s.dhole 2013-09-12 17:26) - PLID 58556
	nfcCoverage,
	nfcCopay,
};

// CNexFormularyDlg dialog

IMPLEMENT_DYNAMIC(CNexFormularyDlg, CNxDialog)

CNexFormularyDlg::CNexFormularyDlg(NexFormularySource nfsSource, long nPatientID, NexFormularyDrug nfdDrug, long nProviderID,long nFInsuranceID,  CWnd* pParent /*=NULL*/)
	: CNxDialog(CNexFormularyDlg::IDD, pParent)
{
	m_nfsSource = nfsSource;
	m_nPatientID = nPatientID;
	m_nFDBID = nfdDrug.nFDBID;
	m_nProviderID = nProviderID; // (r.farnworth 2013-09-24 16:31) - PLID 58386
	m_strDrugName = nfdDrug.strDrugName;
	m_bInsuranceRequeryFinished = FALSE;
	m_bMedicationRequeryFinished = FALSE;
	m_hIconWriteMedication = NULL;
	m_hIconError = NULL;
	m_nDrugListID = -1;
	// (s.dhole 2013-10-28 10:08) - PLID 59084 Set nFInsuranceID
	m_nFInsuranceID = nFInsuranceID; // (s.dhole 2013-10-18 13:01) - PLID 59068
	m_nStatus=-2;// (s.dhole 2013-08-28 10:33) - PLID 58000
	// (b.cardillo 2013-09-24 20:20) - PLID 58749 - Make dialog sizable
	SetMinSize(805, 606);
}

CNexFormularyDlg::~CNexFormularyDlg()
{
	if( m_hIconWriteMedication != NULL ){
		DestroyIcon((HICON)m_hIconWriteMedication);
	}

	if( m_hIconError != NULL ){
		DestroyIcon((HICON)m_hIconError);
	}
}

void CNexFormularyDlg::DoDataExchange(CDataExchange* pDX)
{
	CNxDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btnClose);
	// (b.cardillo 2013-09-24 20:20) - PLID 58749 - Use multiple nxcolors instead of a groupbox inside nxcolor
	DDX_Control(pDX, IDC_NXC_BACKGROUND_NEXFORMULARY_ELIGIBILITY, m_nxcBackgroundEligibility);
	DDX_Control(pDX, IDC_NXC_BACKGROUND_NEXFORMULARY_DRUG, m_nxcBackgroundDrug);
	DDX_Control(pDX, IDC_NXC_BACKGROUND_NEXFORMULARY_ALTERNATES, m_nxcBackgroundAlternates);
	DDX_Control(pDX, IDC_STATIC_NEXFORMULARY_DRUG, m_nxsMedication);
	DDX_Control(pDX, IDC_STATIC_NEXFORMULARY_INSURANCE, m_nxsInsurance);
	DDX_Control(pDX, ID_BTN_WRITERX, m_btnWrit);
	DDX_Control(pDX, IDC_VIEW_DEMS, m_btnViewDerm);
}

BOOL CNexFormularyDlg::OnInitDialog()
{
	try{
		CNxDialog::OnInitDialog();
		PrepareDialog();
		PrepareControls();
		return TRUE;
	}NxCatchAll(__FUNCTION__);
	return FALSE;
}

void CNexFormularyDlg::PrepareDialog()
{
	try{
		SetTitleBarIcon(IDI_NEXFORMULARY);
		CString strTitleBarText;
		strTitleBarText.Format("NexFormulary Information for %s", GetExistingPatientName(m_nPatientID));
		SetWindowText(strTitleBarText);
	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::PrepareControls()
{
	try{
		m_btnClose.AutoSet(NXB_CLOSE);
		m_btnWrit.AutoSet(NXB_WRITE);  // (s.dhole 2013-08-28 10:33) - PLID 58000 
		// (b.cardillo 2013-09-24 20:20) - PLID 58749 - Use multiple nxcolors instead of a groupbox inside nxcolor
		m_nxcBackgroundEligibility.SetColor(GetNxColor(GNC_PATIENT_STATUS, 1));
		m_nxcBackgroundDrug.SetColor(GetNxColor(GNC_PATIENT_STATUS, 1));
		m_nxcBackgroundAlternates.SetColor(GetNxColor(GNC_PATIENT_STATUS, 1));
		extern CPracticeApp theApp;
		// (s.dhole 2013-09-12 17:26) - PLID 58556
		// Check is we have valid Formulary data , not older than 72 hours
		CheckExistingFormularyData();
		GetDlgItem(IDC_STATIC_OTC)->SetFont(&theApp.m_boldFont);
		GetDlgItem(IDC_FORMULARY_STATUS)->SetFont(&theApp.m_boldFont);
		m_nxdlInsurance = BindNxDataList2Ctrl(IDC_NXDL_NEXFORMULARY_INSURANCE, false);
		m_nLastInsuranceID = 0;
		LoadInsuranceDatalist();
		m_nxdlFormulary = BindNxDataList2Ctrl(IDC_NXDL_NEXFORMULARY_FORMULARY, false);
		// (b.savon 2013-10-07 10:48) - PLID 57377 - Only show active FDB Meds.  Since FDB doesn't allow snapshop isolation, and
		// the datalist defaults it; we have to requery it a bit differently
		m_nxdlDrug = BindNxDataList2Ctrl(IDC_NXDL_NEXFORMULARY_DRUG, false);
		m_nxdlDrug->PutAdoConnection(GetRemoteData().GetInterfacePtr());
		m_nxdlDrug->Requery();
		//TES 9/24/2013 - PLID 58287 - If they're coming from an existing prescription, only allow them to view that specific
		// medication. Also disable the "Write" button, it makes no sense since they're already on a prescription
		if(m_nfsSource == nfsPrescriptionEditDlg) {
			m_nxdlDrug->Enabled = g_cvarFalse;
			m_btnWrit.EnableWindow(FALSE);
		}

		m_hIconWriteMedication = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_WRITE), IMAGE_ICON, 16,16, 0);
		m_hIconError = (HICON)LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_X), IMAGE_ICON, 16, 16, 0);
		LoadSearchMedication(FALSE,"","","","",-2);// (s.dhole 2013-08-28 10:33) - PLID 58000 Reset display
	}NxCatchAll(__FUNCTION__);
}
// (s.dhole 2013-09-12 17:26) - PLID 58556 Check if valid formulary info exist
void CNexFormularyDlg::CheckExistingFormularyData()
{
	//Will check if there is successful formulary request we made during last 72 hours  
	if  (FALSE== ReturnsRecordsParam("Select TOP 1 1 from SureScriptsEligibilityResponseT INNER JOIN "
	" SureScriptsEligibilityRequestT "
	" ON SureScriptsEligibilityResponseT.RequestID  = SureScriptsEligibilityRequestT.ID  "
	" WHERE SureScriptsEligibilityResponseT.IsFailure =0  "
	" AND (DATEDIFF( HH , SureScriptsEligibilityResponseT.ReceivedDateUTC ,GETUTCDATE()) <=  72   AND DATEDIFF( HH, SureScriptsEligibilityResponseT.ReceivedDateUTC,GETUTCDATE()) >=0)   "
	" AND SureScriptsEligibilityRequestT.PatientID = {INT}", m_nPatientID)){
		// Execute new request 
		//Make sure we pass Correct provider information
		// (r.farnworth 2013-09-24 15:41) - PLID 58386 - Added ProviderID
		GetAPI()->RequestSureScriptsEEligibility(GetAPISubkey(), GetAPILoginToken(),AsBstr(AsString(m_nPatientID)),AsBstr(AsString(m_nProviderID)) ,AsBstr(AsString(GetCurrentLocationID())));
	}

}


void CNexFormularyDlg::LoadInsuranceDatalist()
{
	try{
		
		// (s.dhole 2013-09-12 17:26) - PLID 58556 Insurance list from clause
		// (s.dhole 2013-10-07 17:50) - PLID 58886 Added IsFailure And Active
		m_nxdlInsurance->PutFromClause(
			// (s.dhole 2013-10-08 10:58) - PLID 58923  move to PrescriptionUtilsAPI
			AsBstr(GetFormularyInsuranceSQL())
		);
                     
		CString strInsuranceWhereClause;
		strInsuranceWhereClause.Format("PatientID = %li", m_nPatientID);
		m_nxdlInsurance->PutWhereClause(AsBstr(strInsuranceWhereClause));

		m_nxdlInsurance->Requery();
	}NxCatchAll(__FUNCTION__);
}

BEGIN_MESSAGE_MAP(CNexFormularyDlg, CNxDialog)
	ON_BN_CLICKED(IDOK, &CNexFormularyDlg::OnBnClickedOk)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(ID_BTN_WRITERX, &CNexFormularyDlg::OnBnClickedBtnWriteERx)
	ON_BN_CLICKED(IDC_VIEW_DEMS, &CNexFormularyDlg::OnBnClickedViewDems)
END_MESSAGE_MAP()


// CNexFormularyDlg message handlers

void CNexFormularyDlg::OnBnClickedOk()
{
	try{
		CNxDialog::OnOK();
	}NxCatchAll(__FUNCTION__);
}

BEGIN_EVENTSINK_MAP(CNexFormularyDlg, CNxDialog)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_INSURANCE, 18, CNexFormularyDlg::RequeryFinishedNxdlNexformularyInsurance, VTS_I2)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_DRUG, 18, CNexFormularyDlg::RequeryFinishedNxdlNexformularyDrug, VTS_I2)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_DRUG, 1, CNexFormularyDlg::SelChangingNxdlNexformularyDrug, VTS_DISPATCH VTS_PDISPATCH)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_INSURANCE, 1, CNexFormularyDlg::SelChangingNxdlNexformularyInsurance, VTS_DISPATCH VTS_PDISPATCH)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_DRUG, 16, CNexFormularyDlg::SelChosenNxdlNexformularyDrug, VTS_DISPATCH)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_INSURANCE, 16, CNexFormularyDlg::SelChosenNxdlNexformularyInsurance, VTS_DISPATCH)
	ON_EVENT(CNexFormularyDlg, IDC_NXDL_NEXFORMULARY_FORMULARY, 19,  CNexFormularyDlg::OnLeftClickFormularyList, VTS_DISPATCH VTS_I2 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CNexFormularyDlg::RequeryFinishedNxdlNexformularyInsurance(short nFlags)
{
	try{
		BOOL bSetCurRow = FALSE;// (s.dhole 2013-09-12 17:26) - PLID 58556
		if( m_nxdlInsurance->SearchByColumn(nficInsuranceResponsibilityID, AsBstr("1"), NULL, VARIANT_TRUE) == NULL ){
			if( m_nxdlInsurance->SearchByColumn(nficInsuranceResponsibilityID, AsBstr("2"), NULL, VARIANT_TRUE) == NULL ){
				if ( m_nxdlInsurance->SearchByColumn(nficInsuranceResponsibilityID, AsBstr("3"), NULL, VARIANT_TRUE) == NULL ){
					if ( m_nxdlInsurance->SearchByColumn(nficInsuranceResponsibilityID, AsBstr("4"), NULL, VARIANT_TRUE) == NULL ){
						NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlInsurance->GetNewRow();
						pRow->PutValue(nficInsuranceCoName, AsBstr("< No Insurance >"));
						pRow->PutValue(nficDemographicsHaveChanged, g_cvarFalse); // (r.farnworth 2013-09-19 10:39) - PLID 58358
						pRow->PutValue(nficPBMName, AsBstr("<N/A>"));// (s.dhole 2013-09-17 09:20) - PLID 58556
						pRow->PutValue(nficInsuranceResponsibilityID, (long)INVALID_ROW_SELECTION);
						pRow->PutValue(nficID, (long)INVALID_ROW_SELECTION);
						pRow->PutValue(nficPBMID, AsBstr(""));
						pRow->PutValue(nficSureScriptsFormularyID, AsBstr(""));
						pRow->PutValue(nficSureScriptsAlternativesID, AsBstr(""));
						pRow->PutValue(nficSureScriptsCopayID, AsBstr(""));
						pRow->PutValue(nficPlanName, AsBstr("<N/A>"));// (s.dhole 2013-09-12 17:26) - PLID 58556
						pRow->PutValue(nficSureScriptsCoverageID, AsBstr(""));
						pRow->PutValue(nficPharmacyCoverage, AsBstr("<N/A>"));// (s.dhole 2013-09-12 17:26) - PLID 58556
						pRow->PutValue(nficLastSureScriptsRequestDate, _variant_t(g_cdtSqlMin, VT_DATE));
						pRow->PutValue(nficDisplay, _variant_t("PBM: (N/A); Plan: (N/A); Pharmacy Coverage: (N/A)"));// (s.dhole 2013-09-12 17:26) - PLID 58556
						
						m_nxdlInsurance->AddRowBefore(pRow, m_nxdlInsurance->GetFirstRow());
						m_nxdlInsurance->PutCurSel(pRow);
						bSetCurRow = TRUE;
					}
				}
			}
		}
		// (s.dhole 2013-09-12 17:26) - PLID 58556 Select first row if row data is exist
		if (bSetCurRow ==FALSE && m_nxdlInsurance->GetRowCount()>0 ){
			m_nxdlInsurance->PutCurSel(m_nxdlInsurance->GetFirstRow());
		}
		m_bInsuranceRequeryFinished = TRUE;
		// (s.dhole 2013-10-07 17:50) - PLID 58886 set Icon
		if (m_nxdlInsurance->SearchByColumn(nficIsFailure, AsBstr("1"), NULL, VARIANT_FALSE ) != NULL )
		{
			m_btnViewDerm.AutoSet(NXB_WARNING);
		}
		
		
		CheckRequeryStatus();

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::RequeryFinishedNxdlNexformularyDrug(short nFlags)
{
	try{

		CString strFDBID;
		strFDBID.Format("%li", m_nFDBID);
		if( m_nxdlDrug->SearchByColumn(nfdcFDBID, AsBstr(strFDBID), NULL, VARIANT_TRUE) == NULL ){
			NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlDrug->GetNewRow();
			pRow->PutValue(nfdcName, AsBstr("< No Medication Selected >"));
			pRow->PutValue(nfdcDrugListID, (long)INVALID_ROW_SELECTION);
			pRow->PutValue(nfdcFDBID, (long)INVALID_ROW_SELECTION);
			m_nxdlDrug->AddRowBefore(pRow, m_nxdlDrug->GetFirstRow());
			m_nxdlDrug->PutCurSel(pRow);			
		}

		m_bMedicationRequeryFinished = TRUE;
		CheckRequeryStatus();

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::CheckRequeryStatus()
{
	if( m_bMedicationRequeryFinished && m_bInsuranceRequeryFinished ){
		NXDATALIST2Lib::IRowSettingsPtr pRow; 
		// (s.dhole 2013-10-28 10:08) - PLID 59084 select insurance
		if (m_nFInsuranceID>-1){ 
			pRow =m_nxdlInsurance->FindByColumn(nficID, m_nFInsuranceID,NULL, VARIANT_TRUE);
		}
		else{
		// (r.farnworth 2013-09-18 17:01) - PLID 58358 - We only want to display this on init and when a new insurance has been selected
		// (j.fouts 2013-09-12 12:11) - PLID 58358 - Warn that demographics have changed
			pRow = m_nxdlInsurance->CurSel;
		}
		if(pRow)
		{
			if(VarBool(pRow->GetValue(nficDemographicsHaveChanged), g_cvarFalse))
			{
				// (j.fouts 2013-09-19 11:56) - PLID 58701 - Renamed the button
				MessageBox("The patient demographics returned in the formulary eligibility request do not match exactly what is "
					"stored in Practice for this patient. Please review the demographics by clicking 'View Eligibility Details' to be "
					"sure that the patient has been correctly matched.");
			}
			m_nLastInsuranceID = pRow->GetValue(nficID); // (r.farnworth 2013-09-24 10:04) - PLID 58358 - Want to track the last insurance selected.
		}

		GetFormularyInformation();
	}
}

void CNexFormularyDlg::GetFormularyInformation()
{
	try{
		if( IsValidRequestState() ){
			// (b.savon 2014-01-27 13:45) - PLID 60484 - Only request formulary information for the selected 
			// insurance and drug; not all insurances and drug
			CWaitCursor cwait;
			// (b.savon 2013-10-04 10:07) - PLID 57377 - NexFormulary - Practice Integration - Master Item
			GetAPI()->SetTimeoutInMs(180000); //Set it to 3 mins
			m_fdrFormularyResponse = GetAPI()->GetFormularyData(GetAPISubkey(), GetAPILoginToken(), GetFormularyDataRequestPtr());

			if( m_fdrFormularyResponse->GetInsurance() != NULL ){
				NexTech_Accessor::_FormularyInsurancePtr formularyInsurance;
				// (s.dhole 2013-09-17 10:08) - PLID 58000 Load selected insurance data , max row we have to scan are 3 to 4
				Nx::SafeArray<IUnknown *> saryFormularyInsurance = m_fdrFormularyResponse->GetInsurance();
				foreach(NexTech_Accessor::_FormularyInsurancePtr  oFormularyInsurance, saryFormularyInsurance){
					if (atol(oFormularyInsurance->ResponseDetailID)==GetInsuranceResponseDetailID()){
						DisplayFormularyResults(oFormularyInsurance);
					break;
					}
				}
				
			}else{
				AddErrorRow(FormatString("There was no insurance information returned for %s", GetExistingPatientName(m_nPatientID)));
			}
		}
	}NxCatchAll(__FUNCTION__);
}

BOOL CNexFormularyDlg::IsValidRequestState()
{
	return (GetMedicationFDBID() != INVALID_ROW_SELECTION && GetInsuranceResponseDetailID() !=INVALID_ROW_SELECTION) ;
}

long CNexFormularyDlg::GetMedicationFDBID()
{
	if( m_nxdlDrug ){
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlDrug->GetCurSel();
		if( pRow ){
			return VarLong(pRow->GetValue(nfdcFDBID), INVALID_ROW_SELECTION);
		}
	}

	return INVALID_ROW_SELECTION;
}

long CNexFormularyDlg::GetInsuranceResponseDetailID()
{
	if( m_nxdlInsurance ){
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlInsurance->GetCurSel();
		if( pRow ){
			return VarLong(pRow->GetValue(nficID), INVALID_ROW_SELECTION);
		}
	}

	return INVALID_ROW_SELECTION;
}

NexTech_Accessor::_FormularyDataRequestPtr CNexFormularyDlg::GetFormularyDataRequestPtr()
{
	NexTech_Accessor::_FormularyDataRequestPtr pRequest(__uuidof(NexTech_Accessor::FormularyDataRequest));
	//PatientID
	pRequest->PutpatientID(AsBstr(FormatString("%li", m_nPatientID)));
	// (b.savon 2013-09-04 09:16) - PLID 58413 - Added location and provider id
	pRequest->PutlocationID(AsBstr(FormatString("%li", GetCurrentLocationID())));
	// (r.farnworth 2013-09-24 15:41) - PLID 58386 - Added ProviderID
	pRequest->PutproviderID(AsBstr(FormatString("%li", m_nProviderID)));
	// (b.savon 2014-01-27 15:26) - PLID 60488 - Restrict alternatives information returned when writing an Rx
	pRequest->PutExcludeAlternativesResults(VARIANT_FALSE);
	//DrugListID
	CArray<_bstr_t,_bstr_t> aryBstrIDs;
	aryBstrIDs.Add(AsBstr(FormatString("%li", GetMedicationFDBID())));
	Nx::SafeArray<BSTR> saryFDBIDs = Nx::SafeArray<BSTR>::From(aryBstrIDs);
	pRequest->PutFDBID(saryFDBIDs);
	//InsurancePlan
	// (b.savon 2014-01-27 13:45) - PLID 60484 - Only request formulary information for the selected 
	// insurance and drug; not all insurances and drug
	CArray<NexTech_Accessor::_FormularyInsurancePlanPtr, NexTech_Accessor::_FormularyInsurancePlanPtr> aryInsPlan;
	if( m_nxdlInsurance ){
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlInsurance->GetCurSel();
		if( pRow ){
			NexTech_Accessor::_FormularyInsurancePlanPtr fipInsurance(__uuidof(NexTech_Accessor::FormularyInsurancePlan));
			fipInsurance->PutdetailID(AsBstr(FormatString("%li", VarLong(pRow->GetValue(nficID)))));
			fipInsurance->PutInsurancePlacement(AsBstr(FormatString("%li", VarLong(pRow->GetValue(nficInsuranceResponsibilityID)))));
			fipInsurance->PutFormularyPublisher(AsBstr(VarString(pRow->GetValue(nficPBMID), "")));// (s.dhole 2013-09-12 17:26) - PLID 58556
			fipInsurance->PutFormularyID(AsBstr(VarString(pRow->GetValue(nficSureScriptsFormularyID), "")));
			fipInsurance->PutAlternativesID(AsBstr(VarString(pRow->GetValue(nficSureScriptsAlternativesID), "")));
			fipInsurance->PutCopayID(AsBstr(VarString(pRow->GetValue(nficSureScriptsCopayID), "")));
			fipInsurance->PutCoverageID(AsBstr(VarString(pRow->GetValue(nficSureScriptsCoverageID), "")));
			fipInsurance->PutLastSureScriptsRequestDate(VarDateTime(pRow->GetValue(nficLastSureScriptsRequestDate)));
			aryInsPlan.Add(fipInsurance);
		}
	}
	Nx::SafeArray<IUnknown*> saryInsPlan = Nx::SafeArray<IUnknown*>::From(aryInsPlan);
	pRequest->PutInsurancePlan(saryInsPlan);
	return pRequest;
}

void CNexFormularyDlg::SelChangingNxdlNexformularyDrug(LPDISPATCH lpOldSel, LPDISPATCH* lppNewSel)
{
	try{
		if( *lppNewSel == NULL ){
			SafeSetCOMPointer(lppNewSel, lpOldSel);
		}

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::SelChangingNxdlNexformularyInsurance(LPDISPATCH lpOldSel, LPDISPATCH* lppNewSel)
{
	try{
		if( *lppNewSel == NULL ){
			SafeSetCOMPointer(lppNewSel, lpOldSel);
		}

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::SelChosenNxdlNexformularyDrug(LPDISPATCH lpRow)
{
	try{

		GetFormularyInformation();

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::SelChosenNxdlNexformularyInsurance(LPDISPATCH lpRow)
{
	try{
			//// (s.dhole 2013-09-20 09:05) - PLID  58358 - moved from  CNexFormularyDlg::SelChangingNxdlNexformularyInsurance
			// (r.farnworth 2013-09-18 17:01) - PLID 58358 - We only want to display this on init and when a new insurance has been selected
			// (j.fouts 2013-09-12 12:11) - PLID 58358 - Warn that demographics have changed
			NXDATALIST2Lib::IRowSettingsPtr pRow(lpRow);
			if(pRow)
			{
				long nChosenInsuranceID = pRow->GetValue(nficID);
				if(VarBool(pRow->GetValue(nficDemographicsHaveChanged), g_cvarFalse) && m_nLastInsuranceID != nChosenInsuranceID)
				{
					// (j.fouts 2013-09-19 11:57) - PLID 58701 - Renamed the button
					MessageBox("The patient demographics returned in the formulary eligibility request do not match exactly what is "
						"stored in Practice for this patient. Please review the demographics by clicking 'View Eligibility Details' to be "
						"sure that the patient has been correctly matched.");
				}

					m_nLastInsuranceID = nChosenInsuranceID; // (r.farnworth 2013-09-24 10:04) - PLID 58358 - Want to track the last insurance selected.
			}

			GetFormularyInformation();

	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::DisplayFormularyResults(NexTech_Accessor::_FormularyInsurancePtr formularyInsurance)
{
	try{
		LoadSearchMedication(FALSE,"","","","",-2);// (s.dhole 2013-08-28 10:33) - PLID 58000 Reset display
		if( formularyInsurance->GetResponseStatus() == NexTech_Accessor::FormularyResponseStatus_EligibilityRequestFailure ){
			AddUnableToMakeEligibilityRequestRow(BuildDisplayMessage(formularyInsurance->GetResponseDisplayMessage()));
		}else if ( formularyInsurance->GetResponseStatus() == NexTech_Accessor::FormularyResponseStatus_FormularyDataFailure ){
			AddUnableToMakeFormularyRequestRow(BuildDisplayMessage(formularyInsurance->GetResponseDisplayMessage()));
		} else if ( formularyInsurance->GetResponseStatus() == NexTech_Accessor::FormularyResponseStatus_Success ){
			// (s.dhole 2013-08-20 10:10) - PLID 58000 Load alterative 
			m_nxdlFormulary->Clear(); 
			AddFormularySuccessRow(formularyInsurance->GetDrug()); 
		}else{/*NexTech_Accessor::FormularyResponseStatus_UnknownFailure*/
			AddUnknownFailureRow();
		}
	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::AddUnableToMakeEligibilityRequestRow(const CString &strDisplayMessage)
{
	try{
		AddErrorRow(
			FormatString(
				"NexTech was unable to get an eligibility response from SureScripts.\r\n\r\n%s",
				strDisplayMessage
				).TrimRight("\r\n")
		);
	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::AddUnableToMakeFormularyRequestRow(const CString &strDisplayMessage)
{
	try{
		AddErrorRow(
			FormatString(
				"NexTech was unable to get a formulary response from SureScripts.\r\n\r\n%s", 
				strDisplayMessage
			).TrimRight("\r\n")
		);
	}NxCatchAll(__FUNCTION__);
}

// (s.dhole 2013-08-28 10:33) - PLID 58000 Set display
void CNexFormularyDlg::LoadSearchMedication(const BOOL& bEnabale,const CString& strMedication, const CString& strCoverage,const CString& strCopay,const CString& strStatus, const long &nStatus )
{
	m_nStatus =nStatus;
	//TES 9/24/2013 - PLID 58287 - Don't enable the Write button when we come from the Prescription Edit dialog
  	GetDlgItem(ID_BTN_WRITERX)->EnableWindow(bEnabale && m_nfsSource != nfsPrescriptionEditDlg);
	SetDlgItemText(IDC_COVERAGE,strCoverage) ;
	SetDlgItemText(IDC_COPAY,strCopay); 
	SetDlgItemText(IDC_FORMULARY_STATUS,strStatus) ;
	SetDlgItemText(IDC_STATIC_OTC,strMedication) ;
	
}
// (s.dhole 2013-08-20 10:10) - PLID 58000 Add row to list
void CNexFormularyDlg::AddFormularySuccessRow(const Nx::SafeArray<IUnknown *> &saryDruggList)
{
try{
	foreach(NexTech_Accessor::_FormularyDrugPtr pFormularyDrug, saryDruggList){
		CString strDrugType=""; 
		CString strCopay =GetCopayInformation(pFormularyDrug->GetCopay());
		if (pFormularyDrug->IsOverTheCounter != VARIANT_FALSE){
			strDrugType+=" [OTC] "; 
		}
		else{
			// nothing
		}
		if (pFormularyDrug->IsGeneric != VARIANT_FALSE){
			strDrugType+="(Generic)"; 
		}
		else{
			strDrugType+="(Brand)"; 
		}
		long nStatus = VarLong(pFormularyDrug->GetFormulary()->value,-2);
		CString	strCoverage =VarString ( pFormularyDrug->Coverage,"");
		CString	strStatus=VarString ( pFormularyDrug->GetFormulary()->status,"");
		LoadSearchMedication(TRUE,strDrugType,strCoverage,strCopay,strStatus, nStatus);
		AddFormularyAlternative(pFormularyDrug->GetAlternatives());
	}
	}NxCatchAll(__FUNCTION__);
}


// (s.dhole 2013-08-28 10:33) - PLID 58000 Alternative display
void CNexFormularyDlg::AddFormularyAlternative(const Nx::SafeArray<IUnknown *> &saryAlternatives)
{
	foreach(NexTech_Accessor::_FormularyDrugPtr  pAlternativeDrug, saryAlternatives){
		CString strDrugName= AsString(pAlternativeDrug->DrugName); 
		if (pAlternativeDrug->IsOverTheCounter != VARIANT_FALSE){
			strDrugName+=" [OTC]"; 
		}
		else{
			// nothing
		}
		if (pAlternativeDrug->IsGeneric != VARIANT_FALSE){
			strDrugName+=" (Generic)"; 
		}
		else{
			strDrugName+=" (Brand)"; 
		}
		long nStatus = VarLong(pAlternativeDrug->GetFormulary()->value,-2);
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlFormulary->GetNewRow();
		if( pRow ){
			pRow->PutValue(nfcDrugListID, atol(pAlternativeDrug->DrugListID));
			pRow->PutValue(nfcFDBID, atol(pAlternativeDrug->FDBID));
			pRow->PutValue(nfcNDC, pAlternativeDrug->NDC);
			pRow->PutValue(nfcWrite, (long)m_hIconWriteMedication);
			pRow->PutValue(nfcMedicationAlternative, AsBstr(strDrugName));
			pRow->PutValue(nfcCoverage, pAlternativeDrug->Coverage);
			pRow->PutValue(nfcCopay, AsBstr(GetCopayInformation(pAlternativeDrug->GetCopay())));
			pRow->PutValue(nfcFormularyStatus, pAlternativeDrug->GetFormulary()->status);
			//nStatus==1  Non-Formulary
			//nStatus==0  Non-Reimbursable
			//nStatus==-1  Unknown Status
			//nStatus==-2  in case of error 
			//nStatus== 2  On Formulary  nStatus==3 – 99 = On Formulary—Preferred (higher number means “more preferred”)
			if (nStatus >= 2 && nStatus <= 99){
				pRow->PutCellBackColor(nfcFormularyStatus,COLOR_ON_FORMULARY_CELL); 
			}else{
				pRow->PutCellBackColor(nfcFormularyStatus,COLOR_NON_FORMULARY_CELL); 
			}
			m_nxdlFormulary->AddRowSorted(pRow, NULL);
		}
	}
}

void CNexFormularyDlg::AddUnknownFailureRow()
{
	try{
		AddErrorRow("An unknown error occurred.");
	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::AddErrorRow(const CString& strErrorMessage)
{
	if( m_nxdlFormulary ){
		m_nxdlFormulary->Clear();
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlFormulary->GetNewRow();
		if( pRow ){
			pRow->PutValue(nfcDrugListID, (long)-1);
			pRow->PutValue(nfcFDBID, (long)-1);
			pRow->PutValue(nfcNDC, AsBstr("-1"));
			pRow->PutValue(nfcWrite, g_cvarNull);
			pRow->PutValue(nfcMedicationAlternative, AsBstr(strErrorMessage));
			pRow->PutValue(nfcCoverage, AsBstr(""));
			pRow->PutValue(nfcCopay, AsBstr(""));
			pRow->PutCellBackColor(nfcMedicationAlternative, prscFailure);
			pRow->PutValue(nfcWrite, (long)m_hIconError);
			m_nxdlFormulary->AddRowBefore(pRow, NULL);
		}
	}
}


CString CNexFormularyDlg::BuildDisplayMessage(const Nx::SafeArray<BSTR> &saryDisplayMessage)
{
	CString strMessage;
	foreach(_bstr_t message, saryDisplayMessage){
		strMessage += CString((LPCTSTR)message) + "\r\n";
	}
	return strMessage;
}

// (r.farnworth 2013-08-13 16:37) - PLID 58001 - Left Click Event Handler for the Formulary List
void CNexFormularyDlg::OnLeftClickFormularyList(LPDISPATCH lpRow, short nCol, long x, long y, long nFlags)
{
	try {

		// (s.dhole 2013-10-19 11:19) - PLID 59068
		long nInsurance =-1;
		NXDATALIST2Lib::IRowSettingsPtr pRowInsu = m_nxdlInsurance->CurSel ;
		if (pRowInsu ){
			nInsurance =VarLong(pRowInsu->GetValue(nficID));
		}
		

		if(lpRow == NULL) {
			return;
		}

		IRowSettingsPtr pRow(lpRow);

		//Check if we want to write the medication
		if( nCol == nfcWrite ){ //Make sure, we've clicked the 'Write' row
			if(VarLong(pRow->GetValue(nfcWrite)) == (long)m_hIconWriteMedication){
				//TES 9/24/2013 - PLID 58287 - Store to our member variables
				m_strDrugName = VarString(pRow->GetValue(nfcMedicationAlternative));
				m_nFDBID = VarLong(pRow->GetValue(nfcFDBID));

				//Check if we need to import
				if( VarLong(pRow->GetValue(nfcDrugListID), -1) == -1 ){
					ImportMedication(m_nFDBID, m_strDrugName, m_nDrugListID);
				}else{
					//If it's already imported, make sure we assign the druglist id to the one that was clicked.
					m_nDrugListID = VarLong(pRow->GetValue(nfcDrugListID));
				}
			// (s.dhole 2013-10-19 11:19) - PLID 59068
				m_nFInsuranceID = nInsurance;
				//Dismiss this dialog
				OnBnClickedOk();
			} 
		}
	}NxCatchAll(__FUNCTION__);
}

void CNexFormularyDlg::OnSize(UINT nType, int cx, int cy)
{
	try{
		CNxDialog::OnSize(nType, cx, cy);
		SetControlPositions();
	}NxCatchAll(__FUNCTION__);
}


//TES 8/29/2013 - PLID 57999 - Moved GetCopayInformation() to PrescriptionUtilsAPI

// (s.dhole 2013-08-28 10:33) - PLID 58000 Reset display
void CNexFormularyDlg::OnBnClickedBtnWriteERx()
{
	try {
		// (s.dhole 2013-10-19 11:19) - PLID 59068
		long nInsurance =-1;
		NXDATALIST2Lib::IRowSettingsPtr pRow = m_nxdlInsurance->CurSel ;
		if (pRow ){
			nInsurance =VarLong(pRow->GetValue(nficID));
		}
		
		pRow = m_nxdlDrug->CurSel; 
		if(pRow == NULL) {
			return;
		}
		if (VarLong(pRow->GetValue(nfdcDrugListID)) ==  (long)INVALID_ROW_SELECTION || 
			VarLong(pRow->GetValue(nfdcFDBID))== (long)INVALID_ROW_SELECTION)
		{
			return;
		}

		//Check if we want to write the medication
		
		//TES 9/24/2013 - PLID 58287 - Store to our member variables
		m_strDrugName = VarString(pRow->GetValue(nfdcName));
		m_nFDBID = VarLong(pRow->GetValue(nfdcFDBID));

		//Check if we need to import
		if( VarLong(pRow->GetValue(nfdcDrugListID), -1) == -1 ){
			ImportMedication(m_nFDBID, m_strDrugName, m_nDrugListID);
		}else{
			//If it's already imported, make sure we assign the druglist id to the one that was clicked.
			m_nDrugListID = VarLong(pRow->GetValue(nfdcDrugListID));
		}
		// (s.dhole 2013-10-19 11:19) - PLID 59068
		m_nFInsuranceID = nInsurance;
		//Dismiss this dialog
		OnBnClickedOk();
	}NxCatchAll(__FUNCTION__);
}
// (s.dhole 2013-08-28 10:33) - PLID 58000 Reset display
HBRUSH CNexFormularyDlg::OnCtlColor(CDC* pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr;
	if  (pWnd->GetDlgCtrlID() ==IDC_FORMULARY_STATUS){
    
		if (m_nStatus >= 2 && m_nStatus<= 99){
			pDC->SetTextColor(COLOR_ON_FORMULARY);
		}else{
			pDC->SetTextColor(COLOR_NON_FORMULARY);
		}
    }

	hbr = CNxDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

// (j.fouts 2013-09-12 12:11) - PLID 58701 - Show the demographics returned in the response
void CNexFormularyDlg::OnBnClickedViewDems()
{
	try
	{
		// (j.fouts 2013-09-19 11:56) - PLID 58701 - Open the detail dlg
		CSSEligibilityDlg dlg(this);
		dlg.DoModal(m_nPatientID);
	}
	NxCatchAll(__FUNCTION__);
}