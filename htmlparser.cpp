#include "htmlparser.h"
#include "editorParser.h"
#include "utils.h"
#include <malloc.h>

//#include "cstyleparser.h"
#define ELEMENT 1
#define TEXT	2
#define JSELEMENT 3.
#define  HTML_BARECK_COLOR    0xff0000
#define  HTML_TAG_COLOR		  0x00ff00
#define  HTML_TAG_PROPER_COLOR 0x0000ff
static long HS_html_bracket_clr  =  HTML_BARECK_COLOR;
static long HS_html_tag_clr = HTML_TAG_COLOR;
static long HS_html_tag_proper_clr = HTML_TAG_PROPER_COLOR;
static void HS_InitLanguageKeyWordPattern()
{

}


////////////////////////////////////////////////////////////////////////////////////
void HS_JumpChars( wchar_t **p)
{
	wchar_t *pchar = *p;
	while(*pchar != L'<' && *pchar)
		pchar++;
	*p= pchar;
}

void ReadElement( PEditorLine pLine,wchar_t **p,struct HtmlElement *pElement)
{
	//read elment ,get start and end type.p
	wchar_t *tp = *p;
	pElement->rawEditorLine = pLine;
	while( *tp !=L'<' && *tp)tp++;
	if(*tp == 0)
	{
		pElement->locStart = *p;
		pElement->locEnd = tp;
		pElement->ntype = TEXT;//normal text
		tp = *p;
		return; 
	}
	
	if(*tp == L'<')
		pElement->locStart = tp;

	while( *tp != L'>' && *tp)
		tp++;

	pElement->locEnd = tp;
	if(*tp == L'>')
		pElement->ntype = ELEMENT;
	else
		pElement->ntype = TEXT;

	*p = tp;
}

//
//处理 bareck的头部，如果没有发现则试图去寻找它，没有找到，返回false.
//nType 处理对js element的识别,normal is 1 ,js is 2.
//
bool ParserBareckHeader(PEditorLine pLine, wchar_t **pStart ,wchar_t **pEnd,int *nType )
{
	return false;
}

//
//处理property部分的内容，当遇到没有bareck尾部的情况，将内容处理后，返回false.
//
bool ParserProperty(PEditorLine pLine, wchar_t **pStart ,wchar_t **pEnd )
{
	return false;
}

//
//处理bareck的尾部
//
bool  ParserBareckTailer(PEditorLine pLine, wchar_t **pStart ,wchar_t **pEnd )
{
	return false;
}

//
//js module
//

void ProcessJsModule( PEditorLine *ppLine)
{
	//create a js parser,and add line by youself ,not use free function  in EditorParser.h
	//and free it by youself.
	EditorParser *pParser = CreateEditorParser(L"js");
	//add line here ,by youself...

	//then process lines.
    ProcessParserLines(pParser);
	//set line list is empty,before you release parser.
	pParser->line = 0;
	ReleaseEditorParser(pParser);
}

void ParseElement(PEditorLine pLine,struct HtmlElement *pElement,int *nType)
{ 
	//return;
	wchar_t *pStart = pElement->locStart;
	wchar_t *pEnd = pStart;
	bool bYes ;
	while(*pEnd)
	{
		bYes = ParserBareckHeader(pLine,&pStart,&pEnd,nType);
		if(bYes)
			bYes = ParserProperty(pLine,&pStart,&pEnd);
		if(bYes)
			ParserBareckTailer(pLine,&pStart,&pEnd);
	}

}

static PEditorLine  HS_LanguageParser(PEditorLine pLine ,wchar_t *keyWords[])
{
	wchar_t *pwchar_t = pLine->rawLine;
	wchar_t *pTpwchar_t = 0;

	pTpwchar_t = pwchar_t;
	HS_JumpChars(&pwchar_t);
	int nType =0;
	while( *pTpwchar_t)
	{
		struct HtmlElement Element;
		ReadElement(pLine,&pTpwchar_t,&Element);
		if(Element.ntype == ELEMENT)
			ParseElement(pLine,&Element,&nType);	
		//
		//fond is js elment,need do some special thing here!
		//current pLine maybe changed.
		//
		if(nType == JSELEMENT)//only support js ,not support other.
		{
				ProcessJsModule( &pLine);
		}
		if( *Element.locEnd == 0)
			break;
	}
	return pLine;
}


 struct Parser CStyleParser_html = {
	&HS_LanguageParser,
	&HS_InitLanguageKeyWordPattern,
	0
};

