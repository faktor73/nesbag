// ConsoleDebugger.h
//

#pragma once

#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QPushButton>
#include <QFont>
#include <QLabel>
#include <QTimer>
#include <QFrame>
#include <QGroupBox>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QLineEdit>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QScrollBar>

#include "Qt/main.h"
#include "Qt/SymbolicDebug.h"
#include "../../debug.h"

struct dbg_asm_entry_t
{
	int  addr;
	int  bank;
	int  rom;
	int  size;
	int  line;
	uint8  opcode[3];
	std::string  text;

	enum
	{
		ASM_TEXT = 0,
		SYMBOL_NAME,
		SYMBOL_COMMENT
	} type;

	dbg_asm_entry_t(void)
	{
		addr = 0; bank = -1; rom = -1; 
		size = 0; line =  0; type = ASM_TEXT;

		for (int i=0; i<3; i++)
		{
			opcode[i] = 0;
		}
	}
};

class debuggerBookmark_t
{
	public:
		int  addr;
		std::string  name;

		debuggerBookmark_t(void)
		{
			addr = 0;
		}
};

class debuggerBookmarkManager_t
{
	public:
		debuggerBookmarkManager_t(void);
		~debuggerBookmarkManager_t(void);

		int addBookmark( int addr, const char *name = NULL );
		int editBookmark( int addr, const char *name );
		int deleteBookmark( int addr );

		int  size(void);
		void clear(void);
		debuggerBookmark_t *begin(void);
		debuggerBookmark_t *next(void);
		debuggerBookmark_t *getAddr( int addr );
	private:
		std::map <int, debuggerBookmark_t*> bmMap;
		std::map <int, debuggerBookmark_t*>::iterator internal_iter;
};

class ConsoleDebugger;

class QAsmView : public QWidget
{
   Q_OBJECT

	public:
		QAsmView(QWidget *parent = 0);
		~QAsmView(void);

		void setScrollBars( QScrollBar *h, QScrollBar *v );
		void updateAssemblyView(void);
		void asmClear(void);
		int  getAsmLineFromAddr(int addr);
		void setLine(int lineNum);
		void setXScroll(int value);
		void scrollToPC(void);
		void setDisplayROMoffsets( bool value );
		void setSymbolDebugEnable( bool value );
		void setRegisterNameEnable( bool value );
		int  getCtxMenuAddr(void){ return ctxMenuAddr; };
	protected:
		void paintEvent(QPaintEvent *event);
		void keyPressEvent(QKeyEvent *event);
   	void keyReleaseEvent(QKeyEvent *event);
		void mousePressEvent(QMouseEvent * event);
		void mouseMoveEvent(QMouseEvent * event);
		void resizeEvent(QResizeEvent *event);
		void contextMenuEvent(QContextMenuEvent *event);

		void calcFontData(void);
		QPoint convPixToCursor( QPoint p );

	private:
		ConsoleDebugger *parent;
		QFont       font;
		QScrollBar *vbar;
		QScrollBar *hbar;

		int ctxMenuAddr;
		int maxLineLen;
		int pxCharWidth;
		int pxCharHeight;
		int pxCursorHeight;
		int pxLineSpacing;
		int pxLineLead;
		int viewLines;
		int viewWidth;
		int viewHeight;
		int lineOffset;
		int maxLineOffset;
		int pxLineWidth;
		int pxLineXScroll;
		int cursorPosX;
		int cursorPosY;

		dbg_asm_entry_t  *asmPC;
		std::vector <dbg_asm_entry_t*> asmEntry;

		bool  displayROMoffsets;
		bool  symbolicDebugEnable;
		bool  registerNameEnable;
};

class ConsoleDebugger : public QDialog
{
   Q_OBJECT

	public:
		ConsoleDebugger(QWidget *parent = 0);
		~ConsoleDebugger(void);

		void updateWindowData(void);
		void updateRegisterView(void);
		void breakPointNotify(int bpNum);
		void openBpEditWindow(int editIdx = -1, watchpointinfo *wp = NULL );
		void openDebugSymbolEditWindow( int addr );
		void setBookmarkSelectedAddress( int addr );
		int  getBookmarkSelectedAddress(void){ return selBmAddrVal; };
		void edit_BM_name( int addr );
		void queueUpdate(void);

		QLabel    *asmLineSelLbl;
	protected:
		void closeEvent(QCloseEvent *event);
		//void keyPressEvent(QKeyEvent *event);
   	//void keyReleaseEvent(QKeyEvent *event);

		//QTreeWidget *tree;
		QScrollBar  *vbar;
		QScrollBar  *hbar;
		QAsmView    *asmView;
		QPlainTextEdit *stackText;
		QLineEdit *seekEntry;
		QLineEdit *pcEntry;
		QLineEdit *regAEntry;
		QLineEdit *regXEntry;
		QLineEdit *regYEntry;
		QLineEdit *cpuCycExdVal;
		QLineEdit *instrExdVal;
		QLineEdit *selBmAddr;
		QGroupBox *stackFrame;
		QGroupBox *bpFrame;
		QGroupBox *sfFrame;
		QGroupBox *bmFrame;
		QTreeWidget *bpTree;
		QTreeWidget *bmTree;
		QCheckBox *brkBadOpsCbox;
		QCheckBox *N_chkbox;
		QCheckBox *V_chkbox;
		QCheckBox *U_chkbox;
		QCheckBox *B_chkbox;
		QCheckBox *D_chkbox;
		QCheckBox *I_chkbox;
		QCheckBox *Z_chkbox;
		QCheckBox *C_chkbox;
		QCheckBox *brkCpuCycExd;
		QCheckBox *brkInstrsExd;
		QCheckBox *romOfsChkBox;
		QCheckBox *symDbgChkBox;
		QCheckBox *regNamChkBox;
		QCheckBox *autoOpenChkBox;
		QCheckBox *debFileChkBox;
		QCheckBox *idaFontChkBox;
		QLabel    *emuStatLbl;
		QLabel    *ppuLbl;
		QLabel    *spriteLbl;
		QLabel    *scanLineLbl;
		QLabel    *pixLbl;
		QLabel    *cpuCyclesLbl1;
		QLabel    *cpuCyclesLbl2;
		QLabel    *cpuInstrsLbl1;
		QLabel    *cpuInstrsLbl2;
		QTimer    *periodicTimer;
		QFont      font;

		int   selBmAddrVal;
		bool  windowUpdateReq;

	private:
		void setRegsFromEntry(void);
		void bpListUpdate( bool reset = false );
		void bmListUpdate( bool reset = false );

   public slots:
      void closeWindow(void);
		void asmViewCtxMenuAddBP(void);
		void asmViewCtxMenuAddBM(void);
		void asmViewCtxMenuAddSym(void);
		void asmViewCtxMenuOpenHexEdit(void);
	private slots:
		void updatePeriodic(void);
		void hbarChanged(int value);
		void vbarChanged(int value);
		void debugRunCB(void);
		void debugStepIntoCB(void);
		void debugStepOutCB(void);
		void debugStepOverCB(void);
		void debugRunLineCB(void);
		void debugRunLine128CB(void);
		void seekToCB(void);
		void seekPCCB(void);
		void add_BP_CB(void);
		void edit_BP_CB(void);
		void delete_BP_CB(void);
		void add_BM_CB(void);
		void edit_BM_CB(void);
		void delete_BM_CB(void);
		void resetCountersCB (void);
		void reloadSymbolsCB(void);
		void displayROMoffsetCB(int value);
		void symbolDebugEnableCB(int value);
		void registerNameEnableCB(int value);
		void autoOpenDebugCB( int value );
		void debFileAutoLoadCB( int value );
		void breakOnBadOpcodeCB(int value);
		void breakOnCyclesCB( int value );
		void breakOnInstructionsCB( int value );
		void bpItemClicked( QTreeWidgetItem *item, int column);
		void bmItemClicked( QTreeWidgetItem *item, int column);
		void bmItemDoubleClicked( QTreeWidgetItem *item, int column);
		void cpuCycleThresChanged(const QString &txt);
		void instructionsThresChanged(const QString &txt);
		void selBmAddrChanged(const QString &txt);

};

bool debuggerWindowIsOpen(void);
void saveGameDebugBreakpoints(void);
void loadGameDebugBreakpoints(void);
void debuggerClearAllBreakpoints(void);
void updateAllDebuggerWindows(void);

extern debuggerBookmarkManager_t dbgBmMgr;
