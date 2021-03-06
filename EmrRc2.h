//{{NO_DEPENDENCIES}}
// Used by Emr.rc2
//

#ifndef RC_INVOKED
#pragma once
#endif

#define IDR_EMRFRAME						20000

#define IDB_EMRFRAME_SMALL					19000
#define IDB_EMRFRAME_LARGE					19001

#define IDB_EMRFRAME_TOOLIMAGES_SMALL		19002
#define IDB_EMRFRAME_TOOLIMAGES_LARGE		19003

#define IDB_EMRFRAME_STATUSBARIMAGES_SMALL  19004

/// Menus

#define IDR_POPUP_MDITABS					25000
#define IDR_POPUP_DROP_MDITABS				25001

#define IDR_POPUP_EMR_TOPIC					25002

/// Frame UI
#define ID_MDI_TABBED_DOCUMENT				23100
#define ID_MDI_NEW_HORZ_TAB_GROUP			23101
#define ID_MDI_NEW_VERT_GROUP				23102
#define ID_MDI_MOVE_TO_NEXT_GROUP			23103
#define ID_MDI_MOVE_TO_PREV_GROUP			23104

/// EMR / Frame
#define ID_EMR_SAVE							40000
#define ID_EMR_SAVE_ALL						40001
#define ID_EMR_SAVE_AND_CLOSE				40002
#define ID_EMR_CLOSE						40003
#define ID_EMR_NEW_EMN						40004
#define ID_EMR_STATUS                       40008   // (a.walling 2012-11-27 10:09) - PLID 53891 - EMR Status combo
#define ID_EMR_DESCRIPTION					40009	// (a.walling 2012-05-18 17:18) - PLID 50546

	// panes
#define ID_EMR_VIEW_TREE_PANE				40010
#define ID_EMR_VIEW_PREVIEW_PANE			40011
#define ID_EMR_VIEW_PHOTOS_PANE				40012
#define ID_EMR_VIEW_CLASSIC_BUTTON_PANE		40013
#define ID_EMR_VIEW_MU_PROGRESS_PANE		40014
#define ID_EMR_VIEW_GRAPHING_PANE			40015

	// views / dialogs
#define ID_EMR_VIEW_PIC						40020
#define ID_EMR_VIEW_HISTORY					40021
#define ID_EMR_VIEW_LABS					40022

	// ui / layout
#define ID_EMR_VIEW_FULLSCREEN				40030

	// popups
#define ID_EMR_VIEW_GRAPHS					40040
#define ID_EMR_VIEW_MEDALLERGY				40041
#define ID_EMR_VIEW_CAMERABARCODE			40042
#define ID_EMR_VIEW_EYEMAGINATIONS			40043
#define ID_EMR_VIEW_RECALL					40044
#define ID_EMR_VIEW_DEVICE_IMPORT			40045	// (a.walling 2012-04-02 16:51) - PLID 49360 - Device Import handling
#define ID_EMR_VIEW_EPRESCRIBING			40046
#define ID_EMR_VIEW_DRUGINTERACTIONS		40047	// (j.jones 2012-09-26 14:04) - PLID 52879 - added drug interactions

	// misc
#define ID_EMR_VIEW_MESSAGE_BAR				40050
#define ID_EMR_RESET_LAYOUT					40051

/// EMN
#define ID_EMR_EDIT_MODE					40100


	// status
#define ID_EMR_EMN_WRITE_ACCESS				40101
#define ID_EMR_EMN_STATUS					40102
#define ID_EMR_EMN_CHART					40103
#define ID_EMR_EMN_CATEGORY					40104

	// info
#define ID_EMR_EMN_MORE_INFO				40145	// (a.walling 2012-06-28 17:13) - PLID 51277 - More Info button
#define ID_EMR_EMN_DESCRIPTION				40105
#define ID_EMR_EMN_NOTES					40106
#define ID_EMR_EMN_LOCATION					40107
#define ID_EMR_EMN_PROVIDER					40109
#define ID_EMR_EMN_SECONDARY_PROVIDER		40110
#define ID_EMR_EMN_ASSISTANT				40111
#define ID_EMR_EMN_OTHER_PROVIDERS			40112
#define ID_EMR_SHOW_CONFIDENTIAL_INFO		40116
#define ID_EMR_WOUND_CARE_AUTO_CODING		40146 // (r.gonet 08/03/2012) - PLID 51027
#define ID_EMR_EMN_APPOINTMENT				40147 // (a.walling 2013-02-13 10:39) - PLID 55143 - Emr Appointment linking - UI
#define ID_EMR_EMN_CODES					40148 //TES 2/13/2014 - PLID 60749 - Codes button

	// create
#define ID_EMR_NEW_TODO						40113
#define ID_EMR_NEW_RECORDING				40114

	// auditing
#define ID_EMR_SHOW_EMR_AUDIT_HISTORY		40121
#define ID_EMR_SHOW_EMN_AUDIT_HISTORY		40122

	// Classic Button Pane
//#define ID_EMR_EPRESCRIBING					40123
//ID_EMR_VIEW_EPRESCRIBING
//#define ID_EMR_EM_CHECKLIST					40124
#define ID_EMR_POSITION_TOPICS				40125
#define ID_EMR_DEBUG						40126
#define ID_EMR_ADD_TOPICS_BTN_MENU			40127
#define ID_EMR_PROBLEM_LIST_BTN_MENU		40128

	// EM Checklist 
#define ID_EMR_EM_CHECKLIST					40129
#define ID_EMR_EM_VISITTYPE					40130

	// dates -- PLID 50920 - Dates - Modified, Created
#define ID_EMR_EMN_DATE						40108
#define ID_EMR_EMN_DATE_CREATED				40131
#define ID_EMR_EMN_DATE_MODIFIED			40132

	// patient -- PLID 50921 - Patient demographics: name, age, gender
#define ID_EMR_PATIENT_SUMMARY				40115
#define ID_EMR_UPDATE_DEMOGRAPHICS			40117
#define ID_EMR_EMN_PATIENT_NAME				40133
#define ID_EMR_EMN_PATIENT_AGE				40134
#define ID_EMR_EMN_PATIENT_GENDER			40135

	// procedures -- PLID 50918 - Procedures
#define ID_EMR_EMN_PROCEDURES				40136

	// codes
#define ID_EMR_NEW_CHARGE					40137
#define ID_EMR_NEW_DIAG_CODE				40138

	// problems	-- PLID 50894
#define ID_EMR_SHOW_PROBLEMS				40118
#define ID_EMR_ADD_NEW_PROBLEM				40119
#define ID_EMR_LINK_PROBLEM					40120
#define ID_EMR_ADD_NEW_PROBLEM_TO_EMR		40139
#define ID_EMR_ADD_NEW_PROBLEM_TO_EMN		40140
#define ID_EMR_ADD_NEW_PROBLEM_TO_TOPIC		40141
#define ID_EMR_LINK_PROBLEM_TO_EMR			40142
#define ID_EMR_LINK_PROBLEM_TO_EMN			40143
#define ID_EMR_LINK_PROBLEM_TO_TOPIC		40144

	//asc 
	//(b.eyers 2016-02-22) - PLID 68321 - new fields discharge status, admission time, discharge time
#define ID_EMR_ASC_ADMISSION_TIME			40150
#define ID_EMR_ASC_DISCHARGE_TIME			40151
#define ID_EMR_ASC_DISCHARGE_STATUS			40152

/// Topic
	// items
#define ID_EMR_ADD_ITEM						40201
#define ID_EMR_ADD_IMAGE					40202
#define ID_EMR_ADD_TEXT_MACRO				40203
#define	ID_EMR_ADD_SIGNATURE				40204
	#define ID_EMR_ADD_YOUR_SIGNATURE			40205
	#define ID_EMR_ADD_OTHER_USERS_SIGNATURE	40206

	// subtopics
#define ID_EMR_NEW_TOPIC					40210
#define ID_EMR_NEW_SUBTOPIC					40211
#define ID_EMR_IMPORT_TOPICS				40212
#define ID_EMR_IMPORT_SUBTOPICS				40213

	// navigation
#define ID_EMR_TOPIC_LIST					40295	// (a.walling 2012-05-25 17:39) - PLID 50664 - Topic list
#define ID_EMR_FIRST_TOPIC					40296
#define ID_EMR_PREVIOUS_TOPIC				40297
#define ID_EMR_NEXT_TOPIC					40298
#define ID_EMR_LAST_TOPIC					40299


/// Preview pane
#define ID_EMR_PREVIEW_PANE_PRINT			40301
#define ID_EMR_PREVIEW_PANE_PRINT_PREVIEW	40302
#define ID_EMR_PREVIEW_PANE_PRINT_MULTIPLE	40303

// options
#define ID_EMR_PREVIEW_PANE_CONFIGURE		40310
#define ID_EMR_PREVIEW_PANE_AUTO_SCROLL		40311


/// Status bar
#define ID_EMR_STATUS_TOTAL_TIME_OPEN		40400
#define ID_EMR_STATUS_CURRENT_TIME_OPEN		40401


///Progress Bar
#define ID_EMR_PROGRESS_PANE_BAR			40500
#define ID_EMR_PROGRESS_PANE_STATUS_LABEL	40501
#define ID_EMR_PROGRESS_PANE_SHOW_DETAILS	40502
#define ID_EMR_PROGRESS_PANE_CONFIGURE		40503

/// Photos
	// Filtering
#define ID_EMR_PHOTO_FILTER_CATEGORY		40600
#define ID_EMR_PHOTO_FILTER_DATE			40601
#define ID_EMR_PHOTO_FILTER_PROCEDURE		40602
	// Sorting
#define ID_EMR_PHOTO_SORT					40610
#define ID_EMR_PHOTO_SORT_CATEGORY			40611
#define ID_EMR_PHOTO_SORT_ATTACHDATE		40612
#define ID_EMR_PHOTO_SORT_PROCEDURE			40613
#define ID_EMR_PHOTO_SORT_STAFF				40614
#define ID_EMR_PHOTO_SORT_NOTE				40615
#define ID_EMR_PHOTO_SORT_ASCENDING			40616
#define	ID_EMR_PHOTO_SORT_SERVICEDATE		40617
	// Grouping
#define ID_EMR_PHOTO_GROUP					40630
#define ID_EMR_PHOTO_GROUP_CATEGORY			40631
#define ID_EMR_PHOTO_GROUP_PROCEDURE		40632
	// Display
#define ID_EMR_PHOTO_DISPLAY_LABELS			40640
	//Toolbar
#define ID_PHOTO_VIEW_SINGLE				40650
#define ID_PHOTO_VIEW_SIDEBYSIDE			40651
#define ID_PHOTO_VIEW_TOPBOTTOM				40652
#define ID_PHOTO_VIEW_FOURGRID				40653
#define ID_PHOTO_SELECT_ALL					40654


/// Print / Merging
	// Actions
#define ID_EMR_MERGE_FROM_PACKET					40700
	#define ID_EMR_MERGE_FROM_PACKET_TO_WORD		40701
	#define ID_EMR_MERGE_FROM_PACKET_TO_PRINTER		40702

#define ID_EMR_MERGE_FROM_TEMPLATE					40710
	#define ID_EMR_MERGE_FROM_TEMPLATE_TO_WORD		40711
	#define ID_EMR_MERGE_FROM_TEMPLATE_TO_PRINTER	40712

#define ID_EMR_MERGE_EDIT_PACKET					40720
#define ID_EMR_MERGE_EDIT_TEMPLATE					40721
#define ID_EMR_MERGE_NEW_TEMPLATE					40722
#define ID_EMR_MERGE_COPY_TEMPLATE					40723

	// Selections
#define ID_EMR_MERGE_PACKET_COMBO					40730
#define ID_EMR_MERGE_TEMPLATE_COMBO					40731

	// Options
#define ID_EMR_MERGE_ADVANCED_SETUP					40770
#define ID_EMR_MERGE_REFRESH						40771

#define ID_EMR_MERGE_OPTIONS_BEGIN					40780
	#define ID_EMR_MERGE_OPTIONS_REVERSE_ORDER			40780
	#define ID_EMR_MERGE_OPTIONS_SEPARATE_HISTORY		40781

	#define ID_EMR_MERGE_OPTIONS_SAVE_TO_HISTORY		40790
	#define ID_EMR_MERGE_OPTIONS_TO_PRINTER				40791
	#define ID_EMR_MERGE_OPTIONS_ALL_EMNS				40792
#define ID_EMR_MERGE_OPTIONS_END					40800

/// Single EMN Merge

#define ID_EMR_EMN_MERGE_FROM_TEMPLATE				40740
	#define ID_EMR_EMN_MERGE_FROM_TEMPLATE_TO_WORD		40741
	#define ID_EMR_EMN_MERGE_FROM_TEMPLATE_TO_PRINTER	40742
#define ID_EMR_EMN_MERGE_FROM_OTHER				40743
	#define ID_EMR_EMN_MERGE_FROM_OTHER_TO_WORD		40744
	#define ID_EMR_EMN_MERGE_FROM_OTHER_TO_PRINTER	40745

#define ID_EMR_EMN_MERGE_EDIT_TEMPLATE					40746

	// Selections
#define ID_EMR_EMN_MERGE_TEMPLATE_COMBO					40749

#define ID_EMR_EMN_MERGE_TEMPLATE_MAKE_DEFAULT			40750

// (a.walling 2013-11-15 11:16) - PLID 59517 - Care Summary, Clinical Summary
#define ID_EMR_CARE_SUMMARY								40751
#define ID_EMR_CLINICAL_SUMMARY							40752
// (a.walling 2014-05-12 09:24) - PLID 61787 - Customized CCDA summaries
#define ID_EMR_CARE_SUMMARY_CUSTOMIZED					40754
#define ID_EMR_CLINICAL_SUMMARY_CUSTOMIZED				40755

#define ID_EMR_TEMPLATE_HIDE_TITLE						40753
