// datadump.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "datadump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

typedef struct
	{
	char* pszName;
	int	iVal;
	} LABEL;

LABEL pLABELS[] =
	{
	 "CAT_MASTER"			   , 0x8000,
	 "CAT_CODECATALOG"		, 0x8100,
	 "CAT_CODE"     		   , 0x8200,
	 "CAT_EQUIPMENTCATALOG" , 0x8300,
	 "CAT_EQUIPMENT" 		   , 0x8400,
	 "CAT_LABORCATALOG" 	   , 0x8500,
	 "CAT_LABOR" 			   , 0x8600,
	 "CAT_MATERIALCATALOG"  , 0x8700,
	 "CAT_MATERIAL" 		   , 0x8800,
	 "CAT_REFPRICECATALOG"  , 0x8900,
	 "CAT_REFPRICE"		  	, 0x8A00,
	 "CAT_BIDHISTORYCATALOG", 0x8B00,
	 "CAT_BIDHISTORY"	  	   , 0x8C00,
	 "CAT_BH_AVERAGE" 		, 0x8D00,
	 "CAT_BH_REGRESSION"    , 0x8E00,
	 "CAT_BH_REGCOEFF_SET"  , 0x8F00,
	 "CAT_AREAMAPCATALOG"   , 0x9000,
	 "CAT_AREAMAP" 			, 0x9100,
	 "CAT_COSTSHEETCATALOG" , 0x9200,
	 "CAT_COSTSHEET"	  	   , 0x9300,
	 "CAT_EQUIPMENTSET" 	   , 0x9400,
	 "CAT_EQUIPMENTEX" 	   , 0x9500,
	 "CAT_LABORSET" 		   , 0x9600,
	 "CAT_LABOREX" 			, 0x9700,
	 "CAT_MATERIALSET" 	   , 0x9800,
	 "CAT_MATERIALEX" 		, 0x9900,
	 "CAT_ITEMCATALOG" 	   , 0x9A00,
	 "CAT_ITEM"			 	   , 0x9B00,
	 "CAT_TASKLISTEX"	  	   , 0x9C00,
	 "CAT_TASKLIST"		  	, 0x9D00,
	 "CAT_TASKLIST_END"  	, 0x9E00,
	 "CAT_REFPRICEEX"		   , 0x9F00,
	 "CAT_BIDHISTORYEX"	   , 0xA000,
	 "CAT_COSTSHEETEX"	  	, 0xA100,
	 "CAT_USERCATALOG"		, 0xA200,
	 "CAT_USER" 				, 0xA300,
	 "CAT_BLANK" 				, 0xFF00,

	 "EST_ESTIMATE"		 	 ,	0x0100,
	 "EST_GROUP"	  			 ,	0x0200,
	 "EST_ITEM"			 	  	 ,	0x0300,
	 "EST_PB_TASKLIST"		 ,	0x0400,
	 "EST_PB_TASKLIST_END"   ,	0x0600,
	 "EST_PB_REFPRICE"		 ,	0x0700,
	 "EST_PB_BIDHISTORY"	  	 ,	0x0800,
	 "EST_PB_BH_AVERAGE" 	 ,	0x0900,
	 "EST_PB_BH_REGRESSION"  ,	0x0A00,
	 "EST_PB_BH_REGCOEFF_SET",	0x0B00,
	 "EST_PB_COSTSHEET"	  	 ,	0x0C00,
	 "EST_PB_EQUIPMENTSET" 	 ,	0x0D00,
	 "EST_PB_EQUIPMENT" 		 ,	0x0E00,
	 "EST_PB_LABORSET" 		 ,	0x0F00,
	 "EST_PB_LABOR" 			 ,	0x1000,
	 "EST_PB_MATERIALSET" 	 ,	0x1100,
	 "EST_PB_MATERIAL" 		 ,	0x1200,
	 "EST_USERSET" 			 ,	0x1300,
	 "EST_USER" 				 ,	0x1400,
	 "EST_AREAMAPSET" 		 ,	0x1500,
	 "EST_AREAMAP" 			 ,	0x1600,

	 NULL, 0
	 };

typedef struct
	{
	USHORT iID;
	USHORT iSize;
	} CELL;

CELL cCELL;

char szBUFF[32767];

////////////////////////////////////////////////////////////////

char szLABEL[128];
char szLABELP[128];

PSZ GetLabel ()
	{
	USHORT iClass = cCELL.iID & 0xFF00;
	USHORT iExtra = cCELL.iID & 0x00FF;
	for (int i=0; pLABELS[i].pszName; i++)
		{
		if (pLABELS[i].iVal != iClass)
			continue;
//		sprintf (szLABEL, "(%4.4x) %s+0x%2.2x", cCELL.iID, pLABELS[i].pszName, iExtra);
//		return szLABEL;

		PSZ psz;
		if (iExtra == 0) psz = "";
		if (iExtra == 1) psz = "_FLAGS";
		if (iExtra == 2) psz = "_ID   ";
		if (iExtra == 3) psz = "_NOTES";
		if (iExtra == 4) psz = "_DESCR";
		if (iExtra > 4)  {sprintf (szLABELP, "+0x%2.2x ", iExtra); psz = szLABELP;}

		sprintf (szLABEL, "%s%s", pLABELS[i].pszName, psz);
		return szLABEL;
		}	
	return ("**UNKNOWN**");
	}


char szSAFEVALUE[128];

PSZ GetSafeValue ()
	{
	if (cCELL.iSize == 1)
		{
		sprintf (szSAFEVALUE, "0x%2.2x", (UCHAR)*szBUFF);
		return szSAFEVALUE;
		}
	if (cCELL.iSize == 2)
		{
		USHORT i = *(USHORT*)szBUFF;
		sprintf (szSAFEVALUE, "0x%4.4x", i);
		return szSAFEVALUE;
		}
	if (cCELL.iSize == 4)
		{
		sprintf (szSAFEVALUE, "0x%4.4x", *(DWORD*)szBUFF);
		return szSAFEVALUE;
		}
	if (cCELL.iSize == 8)
		{
		sprintf (szSAFEVALUE, "%f", *(double*)szBUFF);
		return szSAFEVALUE;
		}
	if (!cCELL.iSize)
		return "          ";

	for (int i=0; i<cCELL.iSize && i<64; i++)
		{
		BYTE b = (BYTE)szBUFF[i];
		szSAFEVALUE[i] = (b > 31 && b < 129 ? b : '.');
		}
	szSAFEVALUE[i] = '\0';
	return szSAFEVALUE;
	}


int DumpCell ()
	{
	PSZ  pszLabel 		= GetLabel ();
	PSZ  pszSafeValue	= GetSafeValue ();

	if (cCELL.iID & 0x00FF)
		printf ("   [%s, (%4.4x)] %s\n", pszLabel, cCELL.iSize, pszSafeValue);
	else
		printf ("[%s, ID:{%4.4x} LEN:(%4.4x)] %s\n", pszLabel, cCELL.iID, cCELL.iSize, pszSafeValue);
	return 0;
	}


int ReadCell (CFile* pcFile)
	{
	int iRead = (int)pcFile->Read (&cCELL, sizeof (cCELL));
	if (iRead < sizeof (cCELL))
		return 0;
	pcFile->Read (&szBUFF, cCELL.iSize);
	return iRead;
	}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
	{
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);

	//CString strHello;
	//strHello.LoadString(IDS_HELLO);
	//cout << (LPCTSTR)strHello << endl;

	if (argc < 2)
		return printf ("Usage:  DataDump <cellfile>\n");

	CFile cFile;
	if (!cFile.Open (argv[1], CFile::modeRead))
		return printf ("Cannot open input file %s\n", argv[1]);

	// magic number before the cells
	DWORD dwMagicNumber;
	cFile.Read (&dwMagicNumber, sizeof (DWORD));
	printf ("Magic Number : %8.8x\n", dwMagicNumber);

	while (ReadCell (&cFile))
		DumpCell ();

	return 0;
	}


