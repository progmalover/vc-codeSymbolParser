//
// zhangxy designed for ben zhuangtai in 2011.5.18
// zhuangtai can use this code,but he must keep my author
// name on top of file.
// if someone is  ben,can use this code,other is not. 
// all right reserved.
// 2011.5.18
//
//#include "stdafx.h"

#include  <stdlib.h> 
#include <wchar.h>
#include <string.h>
#include <malloc.h>

#include "editorParser.h"
#include "utils.h"
#include "cstyleparser.h"
#include "htmlparser.h"
//extern size_t   mbstowcs(wchar_t *, const char *, size_t);
static PEditorLine DefaultLanguageParser(PEditorLine pLine ,wchar_t *keyWords[]);
static void DefaultInitKeyWordsPattern();

struct Parser DefaultParser = {
 &DefaultLanguageParser,
 &DefaultInitKeyWordsPattern,
 NULL
};

struct store_parser_ojbect{ 
wchar_t extname[12];
PParser parser;
} store_parser_ojbects[] = {
	{L"\0",&DefaultParser},
	{L"h",&CStyleParser_c_cplusplus},
	{L"c",&CStyleParser_c_cplusplus},
	{L"cpp",&CStyleParser_c_cplusplus},
	{L"java",&CStyleParser_java},
	{L"js",&CStyleParser_js},
	{L"html",&CStyleParser_html},
	{L"htm",&CStyleParser_html}
};

static void DefaultInitKeyWordsPattern()
{
	return;
}

static PEditorLine  DefaultLanguageParser(PEditorLine pLine ,wchar_t *keyWords[])
{
      //no implement.
	return pLine;
}

inline int AddTabCharCookie( PLineDressUp *cookie,int nstLoc,int nendLoc)
{
	PLineDressUp tp = NULL;
	if( *cookie == NULL)
	{
		tp = (PLineDressUp)calloc(sizeof(struct LineDressUp),1);
		*cookie = tp;
	}
	else
	{	
		tp = (PLineDressUp)calloc(sizeof(struct LineDressUp),1);
		tp->next = *cookie;
		(*cookie)->last = tp;
		(*cookie) = tp;
	}

	tp->beginLoc = nstLoc;
	tp->endLoc = nendLoc;
	tp->ntype = 1; // is tab wchar_t.

	return 0;
}


//
//ansi  字符解析接口。
//
inline int Parselines(PEditorLine *ppline,char *pText,int nSize)
{	
#if 0
	int n = 0;
	if(!pText && nSize <= 0)
		return -1;
	PEditorLine pline =  (struct EditorLine *)calloc(sizeof(struct EditorLine),1);
	*ppline = pline;

	char *p,*tp,*pend;

	p = pText;
	tp = p;
	pend = pText + nSize;
	
	int nChTab = 0;
	while( p <= pend)
	{
		if( *p == 0)
			*p = L' '; // zero to ' '
		if( *p ==L'	')
			nChTab++;

		//
		//PC机 用 CRLF \r\n
		//苹果机用CR   \r
		//unix 用 LF   \n
		//
		if(*p == L'\r' || *p==L'\n')
		{
			int nCRLF = 1;
			if( *p == L'\r')
			{
				if( *(++p) == L'\n')
				{
					++p;
					nCRLF++;
				}
			}else
				++p;

			int len = p - tp - nCRLF ;
			//pline->rawLine =  (wchar_t *)calloc(((len & (~0x03)) + 8 + nChTab * 4) * sizeof(wchar_t),1);
		    char *buffString = (char *)calloc(((len & (~0x03)) + 8 + nChTab * 4) ,1);
			if(nChTab)
			{
				for(int i = 0,j=0;i< len;i++)
				{
					if(tp[i] == '	')//extern tab
					{
						buffString[j] = ' ';
						buffString[j + 1] = ' ';
						buffString[j + 2] = ' ';
						buffString[j + 3] = ' ';
						 
						pline->bTabChIn = true;
						AddTabCharCookie(&pline->cookies,j,j+4);
						j+= 4;
					}
					else
					{
						buffString[j] = tp[i];
						j++;
					}

				}
			}
			else
			while(--len >= 0)
				buffString[len] = tp[len];
			int nwcSize = mbstowcs(NULL, buffString,(size_t) 0); // C4996
			pline->rawLine = (wchar_t *)calloc((nwcSize  + 1)*sizeof(wchar_t),1);
			int wcSz = mbstowcs(pline->rawLine,buffString,(size_t)nwcSize);
			pline->rawLine[wcSz] = 0;
			delete buffString;

			PEditorLine tpline = (PEditorLine)calloc(sizeof(struct EditorLine),1);
			pline->next = tpline;
			tpline->last = pline;
			pline = tpline;
			nChTab = 0;
			tp = p;
			n++;
			continue;
		}

		p++;
	}

	//
	//消除空行
	//
	if( n <= 0 )
	{
		pline->rawLine = pline->rawLine =  (wchar_t *)calloc(nSize  + 1,1);
		memcpy(pline->rawLine,pText,nSize*sizeof(wchar_t));
		n = 1;
		//free(pline);
		//*ppline = NULL;
	}else
	{
		if( p - tp > 1)
		{
			int lastLen =pend - tp;
			pline->rawLine = pline->rawLine =  (wchar_t *)calloc(lastLen  + 2,1);
			memcpy(pline->rawLine,tp,lastLen *sizeof(wchar_t));
		}else
		if(pline->rawLine == NULL)
		{
			PEditorLine tpline = pline->last;
			free(pline);
			tpline->next = NULL;
		}
	}
	return n;
#endif
	return 0;
}

//
// fond key word ,and save information in line struct.
//
static PEditorLine  ParseLineKeyCookiers(PParser parser, PEditorLine pline)
{
	if(!pline || !parser)
		return pline;

	//
	//free cookies ,before parsing.
	//
    //FreeCookies(pline->cookies);

	//
	//call keyword parser.
	//
	return (* parser->parser)(pline ,parser->keyWords);
}


//
//根据文件类型选择创建特定的 parser
//
PEditorParser CreateEditorParser(wchar_t *fileExt)
{
	PEditorParser p = (PEditorParser)calloc(sizeof(struct EditorParser ),1);

	//
	//根据文件扩展名挑选 parser,全部小写。
	//
	for( int i = 1;i < sizeof(store_parser_ojbects) / sizeof(store_parser_ojbect) ;i++)
	{
		if( wcscmp(fileExt,store_parser_ojbects[i].extname) == 0)
		{
			p->parser = store_parser_ojbects[i].parser;
			break;
		}
	}
	if(!p->parser)
		p->parser =  store_parser_ojbects[0].parser;
	p->parser->InitKeyWordsPattern();
	return p;
}

//
//只是基于ascii的字符处理，不包括utf8，unicode等，这类型数据需要进行到ascii的转换后，才能使用。
//
bool  LoadTextToParser(PEditorParser pParser ,char *pText,int nSize)
{
	if(!pParser && nSize <= 0) 
		return false;
	if( pParser->nline)
	{
		FreeLines(pParser);
	}
	//
	// chang text to lines,by the '\r' or'\n'
	// in linux and unix  is '\r\n' 
	// in windows is '\n'
	//

	int nline = Parselines( &pParser->line ,pText,nSize);
	pParser->nline = nline;
	PEditorLine line = pParser->line;
	
	//
	//parse keywords in the line,and set the color of line 
	//or keywords of it.
	//
	while(line)
	{
		line = ParseLineKeyCookiers(pParser->parser ,line);
		line = line->next;
	}
	return true;
}

bool  ReleaseEditorParser(PEditorParser pParser)
{

	if(pParser){
		FreeLines(pParser);
		free(pParser);
	}
	return true;
}

wchar_t *CopyWcString(wchar_t *pSrc)
{
	wchar_t *pDesk = 0;
	int len = wcslen(pSrc);
	pDesk = (wchar_t *)malloc( (len + 1)*sizeof(wchar_t));
	pDesk[len] = '\0';

	wcscpy(pDesk,pSrc);
	return pDesk;
}

void  AddTextLineToParser(PEditorParser pParser ,wchar_t *pText)
{
	wchar_t textbuffer[300]={0}; //max line 256.
	int nLen = ::wcslen(pText);
	if(nLen <= 0)
		return;
	//if(nLen > 255)
		//return;
	int nlines = nLen / 255;
	int nNumLast = nLen % 255;

	nLen = 255;
	do
	{
	if(nlines <= 0)
		nLen = nNumLast;
	PEditorLine tpline = (PEditorLine)calloc(sizeof(struct EditorLine),1);
	//统计tab 键值 。
	int j = 0;
	for( int i = 0;i < nLen ;i++,j++)
	{
		if(pText[i] =='	')
		{
			textbuffer[j] = ' ';
			textbuffer[j+1] = ' ';
			textbuffer[j+2] = ' ';
			textbuffer[j+3] = ' ';
			AddTabCharCookie(&tpline->cookies,j,j+=3);
			
		}else
			textbuffer[j] = pText[i];
	}

	textbuffer[j] = 0;
	tpline->rawLine =  CopyWcString(textbuffer);
	memset(textbuffer,0,sizeof(textbuffer));
	pText += nLen;
	//TRACE(L"%s\n",tpline->rawLine );
	if(!pParser->line)
	{
		pParser->line = tpline;
		tpline->last = tpline;
	}
	else
	{
		PEditorLine p = pParser->line->last;
		p->next = tpline;
		tpline->last = p;
		pParser->line->last = tpline;
	}
	pParser->nline++;
	 
	}while(--nlines >= 0);
	

}

void    ProcessParserLines (PEditorParser pParser)
{
	PEditorLine line = pParser->line;
	
	//
	//parse keywords in the line,and set the color of line 
	//or keywords of it.
	//
	while(line)
	{
		line = ParseLineKeyCookiers(pParser->parser ,line);
		line = line->next;
	}
	
}