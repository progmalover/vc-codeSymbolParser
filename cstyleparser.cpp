//
// zhangxy designed for ben zhuangtai in 2011.5.18
// zhuangtai can use this code,but he must keep my author
// name on top of file.
// if someone is  ben,can use this code,other is not. 
// all right reserved.
// 2011.5.18
//


#ifndef  _CSTYLEPARSER
#define  _CSTYLEPARSER
//#include "stdafx.h"

#include "editorparser.h"
#include "cstyleparser.h"
#include "utils.h"
#include "string.h"
#include <wchar.h>
////////////////////////////////////////////
//some special singed like :“ "" , <> , /* */, // ”等需要关键字分析中使用
#define  defaultSign 0
#define  commSign1   1 //
#define  commSign21   2 ///*
#define  commSign221  3 // /**
#define  commSign22   4 ///*
#define  stringSign11  5
#define  stringSign12  6
#define  stringSign21  7
#define  stringSign22  8

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// c/c++ keywords pattern
static wchar_t *CStyleKeyWords_c_cplusplus_b[] ={L"break",L"bool",L"byte",0};
static wchar_t *CStyleKeyWords_c_cplusplus_c[] ={L"char",L"const_cast" ,L"class",L"case",L"continue",L"const",L"catch",0};
static wchar_t *CStyleKeyWords_c_cplusplus_d[] ={L"double",L"do",L"default",L"dynamic_cast",0};
static wchar_t *CStyleKeyWords_c_cplusplus_e[] ={L"else",L"extern",L"export",L"enum",0};
static wchar_t *CStyleKeyWords_c_cplusplus_f[] ={L"float",L"friend",L"false",L"for",0};
static wchar_t *CStyleKeyWords_c_cplusplus_g[] ={L"goto",0};
static wchar_t *CStyleKeyWords_c_cplusplus_i[] ={L"int",L"if",L"inline",0};
static wchar_t *CStyleKeyWords_c_cplusplus_l[] ={L"long",0};
static wchar_t *CStyleKeyWords_c_cplusplus_n[] ={L"namespace",0};
static wchar_t *CStyleKeyWords_c_cplusplus_o[] ={L"operator",0};	
static wchar_t *CStyleKeyWords_c_cplusplus_p[] ={L"protected",L"public",L"private",0};
static wchar_t *CStyleKeyWords_c_cplusplus_r[] ={L"return",L"register",0};
static wchar_t *CStyleKeyWords_c_cplusplus_s[] ={L"short",L"switch",L"signed",L"struct",L"sizeof",L"static",0};
static wchar_t *CStyleKeyWords_c_cplusplus_t[] ={L"try",L"throw",L"true",L"typedef",L"this",L"template",0};
static wchar_t *CStyleKeyWords_c_cplusplus_u[] ={L"unsigned" ,L"using",0};
static wchar_t *CStyleKeyWords_c_cplusplus_v[] ={L"void",L"volatile",L"virtual",0};
static wchar_t *CStyleKeyWords_c_cplusplus_w[] ={L"while",L"wchar_t",0};
static wchar_t *CStyleKeyWords_c_plusplus[]   ={L"__super"};
static wchar_t **CStyleKeyWords_c_cplusplus_pattern[] = {
	0,CStyleKeyWords_c_cplusplus_b,CStyleKeyWords_c_cplusplus_c,CStyleKeyWords_c_cplusplus_d,CStyleKeyWords_c_cplusplus_e,
	CStyleKeyWords_c_cplusplus_f,CStyleKeyWords_c_cplusplus_g,0,CStyleKeyWords_c_cplusplus_i,0,0,CStyleKeyWords_c_cplusplus_l,
	0,CStyleKeyWords_c_cplusplus_n,CStyleKeyWords_c_cplusplus_o,CStyleKeyWords_c_cplusplus_p,0,CStyleKeyWords_c_cplusplus_r,
	CStyleKeyWords_c_cplusplus_s,CStyleKeyWords_c_cplusplus_t,CStyleKeyWords_c_cplusplus_u,CStyleKeyWords_c_cplusplus_v,CStyleKeyWords_c_cplusplus_w,0,0,0
};


enum  MCRO_TAG{ non=0,mcro_endif,mcro_ifndef,mcro_ifdef,mcro_define,mcro_include,mcro_pragma,mcro_erro,
mcro_if,mcro_elif,mcro_else,mcro_import,mcro_undef} ;
static struct CStyleMcroKeyStruct
{
  wchar_t *string;
  int  tag;
}CStyleMcroKeyStructs[]=
{
	{L"#endif", mcro_endif},
	{L"#ifndef",mcro_ifndef},
	{L"#ifdef", mcro_ifdef},
	{L"#define",mcro_define},
	{L"#include",mcro_include},
	{L"#pragma",mcro_pragma},
	{L"#error",mcro_erro},
	{L"#if",mcro_if},
	{L"#elif",mcro_elif},
	{L"#else",mcro_else},
	{L"#import",mcro_import},
	{L"#undef",mcro_undef}

};
static wchar_t ***CStyleKeyWords_pattern =  CStyleKeyWords_c_cplusplus_pattern ;
static wchar_t **CStyleKeyWords = CStyleKeyWords_c_plusplus ;
//////////////////////////////////////////////////////////////////////////////////////////////////
/////java keywords pattern

static wchar_t *CStyleKeyWords_java_a[] ={L"abstract",L"assert",0};
static wchar_t *CStyleKeyWords_java_b[] ={L"break",L"boolean",L"byte",0};
static wchar_t *CStyleKeyWords_java_c[] ={L"char",L"const_cast" ,L"class",L"case",L"continue",L"const",L"catch",0};
static wchar_t *CStyleKeyWords_java_d[] ={L"double",L"do",L"default",L"dynamic_cast",0};
static wchar_t *CStyleKeyWords_java_e[] ={L"else",L"externs",L"enum",0};
static wchar_t *CStyleKeyWords_java_f[] ={L"float",L"false",L"for",L"final",L"finally",0};
static wchar_t *CStyleKeyWords_java_g[] ={L"goto",0};
static wchar_t *CStyleKeyWords_java_i[] ={L"int",L"if",L"interface",L"instanceof",L"implements",L"import",L"interface",0};
static wchar_t *CStyleKeyWords_java_l[] ={L"long",0};
static wchar_t *CStyleKeyWords_java_n[] ={L"native",L"new",L"null",0};
static wchar_t *CStyleKeyWords_java_o[] ={L"operator",0};
static wchar_t *CStyleKeyWords_java_p[] ={L"protected",L"public",L"private",L"package",0};
static wchar_t *CStyleKeyWords_java_r[] ={L"return",0};
static wchar_t *CStyleKeyWords_java_s[] ={L"short",L"switch",L"synchronized",L"strictfp",L"static",L"super",0};
static wchar_t *CStyleKeyWords_java_t[] ={L"try",L"throw",L"true",L"throws",L"transient",L"this",0};
//static wchar_t *CStyleKeyWords_java_u[] ={"unsigned" ,"using",0};
static wchar_t *CStyleKeyWords_java_v[] ={L"void",L"volatile",0};
static wchar_t *CStyleKeyWords_java_w[] ={L"while",0};
static wchar_t *CStyleKeyWords_java[]   ={L"@Override"};
static wchar_t **CStyleKeyWords_java_pattern[] = {
	CStyleKeyWords_java_a,CStyleKeyWords_java_b,CStyleKeyWords_java_c,CStyleKeyWords_java_d,CStyleKeyWords_java_e,
	CStyleKeyWords_java_f,CStyleKeyWords_java_g,0,CStyleKeyWords_java_i,0,0,CStyleKeyWords_java_l,
	0,CStyleKeyWords_java_n,CStyleKeyWords_java_o,CStyleKeyWords_java_p,0,CStyleKeyWords_java_r,
	CStyleKeyWords_java_s,CStyleKeyWords_java_t,0,CStyleKeyWords_java_v,CStyleKeyWords_java_w,0,0,0
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
////js keyword pattern
////暂时不做嵌入对象的识别处理，介时作为关键字插入列表即可。
static wchar_t *CStyleKeyWords_js_a[] ={L"abstract",0};
static wchar_t *CStyleKeyWords_js_b[] ={L"break",L"boolean",L"byte",0};
static wchar_t *CStyleKeyWords_js_c[] ={L"char" ,L"class",L"case",L"continue",L"const",L"catch",0};
static wchar_t *CStyleKeyWords_js_d[] ={L"double",L"do",L"default",L"delete",0};
static wchar_t *CStyleKeyWords_js_e[] ={L"else",L"externs",0};
static wchar_t *CStyleKeyWords_js_f[] ={L"float",L"false",L"for",L"final",L"finally",L"function",0};
static wchar_t *CStyleKeyWords_js_g[] ={L"goto",0};
static wchar_t *CStyleKeyWords_js_i[] ={L"int",L"if",L"interface",L"instanceof",L"implements",L"import",L"interface",0};
static wchar_t *CStyleKeyWords_js_l[] ={L"long",0};
static wchar_t *CStyleKeyWords_js_n[] ={L"native",L"new",L"null",0};
static wchar_t *CStyleKeyWords_js_o[] ={L"operator",0};
static wchar_t *CStyleKeyWords_js_p[] ={L"protected",L"public",L"private",L"package",0};
static wchar_t *CStyleKeyWords_js_r[] ={L"return",0};
static wchar_t *CStyleKeyWords_js_s[] ={L"short",L"switch",L"synchronized",L"static",L"super",0};
static wchar_t *CStyleKeyWords_js_t[] ={L"try",L"throw",L"true",L"throws",L"transient",L"typeof",L"this",0};
//static wchar_t *CStyleKeyWords_java_u[] ={"unsigned" ,"using",0};
static wchar_t *CStyleKeyWords_js_v[] ={L"void",L"var",0};
static wchar_t *CStyleKeyWords_js_w[] ={L"while",L"with",0};
static wchar_t *CStyleKeyWords_js[]   ={L""};
static wchar_t **CStyleKeyWords_js_pattern[] = {
	CStyleKeyWords_js_a,CStyleKeyWords_js_b,CStyleKeyWords_js_c,CStyleKeyWords_js_d,CStyleKeyWords_js_e,
	CStyleKeyWords_js_f,CStyleKeyWords_js_g,0,CStyleKeyWords_js_i,0,0,CStyleKeyWords_js_l,
	0,CStyleKeyWords_js_n,CStyleKeyWords_js_o,CStyleKeyWords_js_p,0,CStyleKeyWords_js_r,
	CStyleKeyWords_js_s,CStyleKeyWords_js_t,0,CStyleKeyWords_js_v,CStyleKeyWords_js_w,0,0,0
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//颜色方案 ，c/c++
#define C_CPLUSPLUS_TEXT_COLOR		0x0000ff 
#define C_CPLUSPLUS_COMM_COLOR		0x008000
#define C_CPLUSPLUS_KEYWORD_COLOR   0xff0000
//颜色方案 ，java ,js
#define JAVA_TEXT_COLOR				0xff0000
#define JAVA_COMM_COLOR				0x5c9524
#define JAVA_COMM_COLOR2            0xe07d64
#define JAVA_KEYWORD_COLOR			0xf906b6
#define JAVA_SEPECIAL_COMM_COLOR    0x686868
//特殊 注释符
static long cs_java_c_cplus_plus_text_color = C_CPLUSPLUS_TEXT_COLOR;
static long cs_java_c_cplus_plus_comm_color = C_CPLUSPLUS_COMM_COLOR;
static long cs_java_c_cplus_plus_comm_color2 = JAVA_COMM_COLOR2;
static long cs_java_c_cplus_plus_keyword_color=C_CPLUSPLUS_KEYWORD_COLOR;
static long cs_java_c_special_comm_color = JAVA_SEPECIAL_COMM_COLOR;

//关键字语法扩展处理,独立出关键分析 特定语言流程。
typedef   bool(*KEYWORD_EXTEND_PROC)(PEditorLine ,wchar_t *);
typedef   bool(*KEYWORD_EXTEND_MCRO_PROC)(PEditorLine ,wchar_t *,int);
typedef   bool(*KEYWORD_EXTEND_MCRO_PRE_PROC)(PEditorLine ,wchar_t *,int *);
static KEYWORD_EXTEND_PROC				CS_Java_c_cplusplus_especial_keyword_proc_p = NULL ;
static KEYWORD_EXTEND_MCRO_PRE_PROC		CS_c_cplusplus_Mcro_keyword_pre_proc_p = NULL ;
static KEYWORD_EXTEND_MCRO_PROC			CS_c_cplusplus_Mcro_keyword_proc_p=NULL ;
//特殊间隔符号处理
typedef  bool(*TAKEN_SIGN_PRROSS_PROC)( wchar_t **,wchar_t *);

static TAKEN_SIGN_PRROSS_PROC CS_java_c_cplusplus_taken_sign_proc_p;
static bool CS_c_cplusplus_taken_sign_proc(wchar_t **,wchar_t *);
static bool CS_java_taken_sign_proc(wchar_t **,wchar_t *);
//语言特殊语法处理函数，用以解析处理 java不同于c/c++的特殊关键字
//c /c++
static bool CS_c_cplusplus_especial_keyword_proc(PEditorLine ,wchar_t *);
static bool CS_c_cplusplus_Mcro_keyword_pre_proc(PEditorLine ,wchar_t *,int *);
static bool CS_c_cplusplus_Mcro_keyword_proc(PEditorLine ,wchar_t *,int);
 
//java
static bool CS_java_especail_keyword_proc(PEditorLine ,wchar_t *);

static void CS_InitLanguageKeyWordPattern1() //c/c++
{
	CStyleKeyWords_pattern = CStyleKeyWords_c_cplusplus_pattern ;
	CStyleKeyWords = CStyleKeyWords_c_plusplus;

	cs_java_c_cplus_plus_text_color = C_CPLUSPLUS_TEXT_COLOR;
	cs_java_c_cplus_plus_comm_color = C_CPLUSPLUS_COMM_COLOR;
	cs_java_c_cplus_plus_keyword_color=C_CPLUSPLUS_KEYWORD_COLOR;

	//taken sign
	CS_java_c_cplusplus_taken_sign_proc_p = &CS_c_cplusplus_taken_sign_proc;
	//关键字扩展处理
	CS_c_cplusplus_Mcro_keyword_pre_proc_p = &CS_c_cplusplus_Mcro_keyword_pre_proc;
	CS_c_cplusplus_Mcro_keyword_proc_p = &CS_c_cplusplus_Mcro_keyword_proc;
	CS_Java_c_cplusplus_especial_keyword_proc_p = &CS_c_cplusplus_especial_keyword_proc;
}

static void CS_InitLanguageKeyWordPattern2() // java
{
	CStyleKeyWords_pattern = CStyleKeyWords_java_pattern ;
	CStyleKeyWords = CStyleKeyWords_java;

	cs_java_c_cplus_plus_text_color = JAVA_TEXT_COLOR;
	cs_java_c_cplus_plus_comm_color = JAVA_COMM_COLOR;
	cs_java_c_cplus_plus_keyword_color= JAVA_KEYWORD_COLOR;

	//java taken sign
	//taken sign
	CS_java_c_cplusplus_taken_sign_proc_p = &CS_java_taken_sign_proc;

	CS_c_cplusplus_Mcro_keyword_proc_p = NULL;//java 不需要宏处理，所以去除。
	CS_Java_c_cplusplus_especial_keyword_proc_p = &CS_java_especail_keyword_proc;
}

static void CS_InitLanguageKeyWordPattern3() // js
{
	CStyleKeyWords_pattern = CStyleKeyWords_js_pattern ;
	CStyleKeyWords = CStyleKeyWords_js;

	cs_java_c_cplus_plus_text_color = JAVA_TEXT_COLOR;
	cs_java_c_cplus_plus_comm_color = JAVA_COMM_COLOR;
	cs_java_c_cplus_plus_keyword_color= JAVA_KEYWORD_COLOR;

	//java taken sign
	//taken sign
	//词法相近，暂时使用java的分析器。
	CS_java_c_cplusplus_taken_sign_proc_p = &CS_java_taken_sign_proc;
	CS_c_cplusplus_Mcro_keyword_proc_p = NULL;//js 不需要宏处理，所以去除。
	CS_Java_c_cplusplus_especial_keyword_proc_p = &CS_java_especail_keyword_proc; 
}
//////////////////////////////////////////////////////////////////////////////////////////////// 
static wchar_t cstakenString[255]={0};
static int	csLastTaken= defaultSign;//default: 0

static bool CS_c_cplusplus_taken_sign_proc(wchar_t **pSrc ,wchar_t *pDesk)
{
	 if( **pSrc == L'\,'||**pSrc == L'\}'||
		   **pSrc == L'\{'||**pSrc == L'\;' ||
		   **pSrc == L'\*' ||**pSrc == L'(' || 
		   **pSrc == L')'||**pSrc ==L' '||
		   **pSrc ==L'&' || **pSrc == L':'||
		   **pSrc == L'='||**pSrc == L'-')
	   { 
		   if( pDesk != cstakenString)
		   {
			   *pDesk = 0;
			   return true;
		   }
		   else
			   (* pSrc)++;
	   }
	   return false;
}

static bool CS_java_taken_sign_proc(wchar_t **pSrc,wchar_t *pDesk)
{
	  if( **pSrc == L'\,'||**pSrc == L'\}'||
		   **pSrc == L'\{'||**pSrc == L'\;' ||
		   **pSrc == L'\*' ||**pSrc == L'(' || 
		   **pSrc == L')'||**pSrc ==L' '||
		   **pSrc ==L'&' || **pSrc == L':'||
		   **pSrc == L'='||**pSrc == L'-'|| **pSrc == L'.')
	   { 
		   if( pDesk != cstakenString)
		   {
			   *pDesk = 0;
			   return true;
		   }
		   else
			   (* pSrc)++;
	   }
	   return false;
}
static wchar_t *  CS_GetTakenwchar_ts(wchar_t *pDesk,wchar_t *pSrc)
{
	   if(*pSrc == L'\0')
	   {
		    *pDesk = 0;
		    return pSrc;
	   }
	   if(csLastTaken >=stringSign11 && csLastTaken <= stringSign22)
		   goto STRING;

	   //处理导致taken结束的符号
	   if((*CS_java_c_cplusplus_taken_sign_proc_p)( &pSrc ,pDesk) == true)
		   return pSrc;

	   //处理注释符号。
	   if( *pSrc == L'\/')
	   {
		    if( pDesk != cstakenString)
		   {
			   *pDesk = 0;
			   return pSrc;
		   }

		    *pDesk = *pSrc;
			 *(++pDesk) = 0;

		   if( *(++pSrc) == L'*' || 
							*pSrc == L'\/')
		   {
			   *pDesk = *pSrc;
			   if( *pSrc ==L'*')
			   {
				   if( *(++pSrc) == L'*')
					   csLastTaken = commSign221;
				   else
					   csLastTaken = commSign21 ;

				   pSrc--;
			   }
			   else if(*pSrc == L'\/')
				   csLastTaken = commSign1;

			   pSrc++;  
			   *(++pDesk) = 0;
			   return pSrc;
		   }
	   }

STRING:
	 if(*pSrc == L'\'')
	 {
		 if(csLastTaken == stringSign11) 
		 {
			 wchar_t ch =  *(--pSrc);
			 pSrc++;
			 if( ch != L'\\')
			 {
				csLastTaken = stringSign12;
				*pDesk = *pSrc ;
				*(++pDesk) = 0;
				pSrc++;
				return pSrc;
			 }
		 }else 
			 if(csLastTaken == 0)
			 {
				 if( pDesk != cstakenString)
				 {
					   *pDesk = 0;
					   return pSrc;
				 }
				 csLastTaken = stringSign11;
			 }
		 
	 }else if(*pSrc == L'\"')
	 {
		 if(csLastTaken == stringSign21) 
		 {
			 wchar_t ch =  *(--pSrc);
			 pSrc++;
			 if( ch != L'\\')
			 {
				csLastTaken = stringSign22;
				*pDesk = *pSrc ;	
				*(++pDesk) = 0;
				pSrc++;
				return pSrc;
			 }
		 }else
			 if(csLastTaken == 0)
			 {
				 if( pDesk != cstakenString)
				 {
					   *pDesk = 0;
					   return pSrc;
				 }
				 csLastTaken = stringSign21;
			 }
	 }
	 
	  *pDesk = *pSrc;
	  return  CS_GetTakenwchar_ts( ++pDesk ,++pSrc);
}

static bool  CS_GetTaken(wchar_t **pRawStr)
{
	wchar_t *p = *pRawStr;
    cstakenString[0] = 0;
	
	*pRawStr = CS_GetTakenwchar_ts(cstakenString ,p);
	if(*(*pRawStr) == 0)
		return false;
	return true;
}

static inline void CS_AddCookie(PEditorLine pLine ,wchar_t *pCurrent,int len,int nTag,long clr )
{
   int 	beginLoc = int((int)pCurrent - (int)pLine->rawLine)/sizeof(wchar_t);
   int	endLoc   = beginLoc + len;
   AddCookie(pLine,nTag,beginLoc,endLoc,clr);
}

static inline void CS_JumpSignwchar_t(wchar_t **p)
{
	wchar_t *pwchar_t = *p;
	while( *pwchar_t == L'\,'||*pwchar_t == L'\}'||*pwchar_t == L'\{'||
		   *pwchar_t == L'\;' ||*pwchar_t == L'\*'||*pwchar_t == L'(' || 
		   *pwchar_t == L')'|| *pwchar_t ==L' '||*pwchar_t ==L'&'||*pwchar_t == L':'
		   ||*pwchar_t == L'='|| *pwchar_t == L'-'||*pwchar_t ==L'\.')
		pwchar_t++;
	*p = pwchar_t;
}
//find */
static PEditorLine  CS_FindCommenSign22(PEditorLine pLine ,wchar_t **pCurwchar_t)
{
	 wchar_t *p = *pCurwchar_t ;
	 long commclr = cs_java_c_cplus_plus_comm_color;
	 if(commSign221 == csLastTaken && CS_c_cplusplus_Mcro_keyword_proc_p == NULL) // java
		 commclr = cs_java_c_cplus_plus_comm_color2;
	 
	 PEditorLine pline = pLine;
	 while(pline)
	 {
			while( *p && *p != L'\*' ) p++;
			if( *p == 0) // line end 
			{
				if( pline == pLine)
				{
					wchar_t *pStart = *pCurwchar_t -2;
					CS_AddCookie(pline, pStart , wcslen(pStart),3, commclr ); 
				}else
					CS_AddCookie(pline, pline->rawLine , wcslen(pline->rawLine),3, commclr ); 

				pline = pline->next;
				if(pline)
				{
					p = pline->rawLine ;
					while( *p == ' ')p++;
				}

			}else
			{
				if( *(++p) == '\/') // fond */
				{
					++p;
					if(pline == pLine) //未换行
					{
							wchar_t *pStart = *pCurwchar_t -2;
							CS_AddCookie(pline, pStart , wcslen(pStart),3, commclr ); 
							break;
					}else
					{
						CS_AddCookie(pline, pline->rawLine ,int(p - pline->rawLine),3, commclr ); 
						break;
					}
				
				}
			}

	 }
	 *pCurwchar_t = p;
	 return pline;
}

//c/c++  java 
static bool CS_c_cplusplus_especial_keyword_proc(PEditorLine pLine ,wchar_t *pTpwchar_t)
{
	for( int i = 0;i < sizeof(CStyleKeyWords)/sizeof(wchar_t *) ;i++ )
	{
		if( *CStyleKeyWords[i] == cstakenString[0]) 
		if(wcscmp(CStyleKeyWords[i],cstakenString) == 0)
		{
			//TRACE("keywords taken:%s \n",cstakenString);
			CS_AddCookie(pLine ,pTpwchar_t,wcslen(cstakenString),5, cs_java_c_cplus_plus_keyword_color);
			break;
		}
	}
	return true;
}
// c 宏关键字 预处理
static bool CS_c_cplusplus_Mcro_keyword_pre_proc(PEditorLine pLine ,wchar_t *pTpwchar_t ,int* lastMcroTaken)
{ 
	if( *cstakenString == '#') // is mcro keyword
	{

		for( int i = 0;i < sizeof(CStyleMcroKeyStructs) /sizeof(struct CStyleMcroKeyStruct) ;i++)
		{
			if(wcscmp(CStyleMcroKeyStructs[i].string,cstakenString) == 0)
			{
			//TRACE("keywords taken:%s \n",cstakenString);
			*lastMcroTaken =  CStyleMcroKeyStructs[i].tag;
			CS_AddCookie(pLine ,pTpwchar_t,wcslen(cstakenString),5, cs_java_c_cplus_plus_keyword_color);
			return true;
			}
		}
	}

	return false;
}
//c 宏关键字 处理
static bool CS_c_cplusplus_Mcro_keyword_proc(PEditorLine pLine ,wchar_t *pTpwchar_t,int lastMcroTaken)
{
	if(lastMcroTaken == mcro_include)
	{
		lastMcroTaken = non;
		if(cstakenString[0] == L'\<')
		{
			CS_AddCookie(pLine ,pTpwchar_t ,wcslen(cstakenString),6, cs_java_c_cplus_plus_text_color);
			*cstakenString = 0;
		}
	}		
	if(lastMcroTaken == mcro_pragma)
	{
		lastMcroTaken = non;
		if( wcscmp(cstakenString ,L"once") == 0 || 
					wcscmp(cstakenString ,L"comment")== 0)
		{
			CS_AddCookie(pLine ,pTpwchar_t ,wcslen(cstakenString),6, cs_java_c_cplus_plus_keyword_color);
			*cstakenString = 0;
		}
	}

	if( lastMcroTaken == mcro_if ||
		lastMcroTaken == mcro_elif||
		lastMcroTaken == mcro_else)
	{
		lastMcroTaken = non;
		if( wcscmp(cstakenString ,L"defined") == 0)
		{
			CS_AddCookie(pLine ,pTpwchar_t ,wcslen(cstakenString),6, cs_java_c_cplus_plus_keyword_color);
			*cstakenString = 0;
		}
	}
	return true;
}
//java
static bool CS_java_especail_keyword_proc(PEditorLine pLine ,wchar_t *pTpwchar_t)
{
	for( int i = 0;i < sizeof(CStyleKeyWords)/sizeof(wchar_t *) ;i++ )
	{
		if( *CStyleKeyWords[i] == cstakenString[0]) 
		if(wcscmp(CStyleKeyWords[i],cstakenString) == 0)
		{
			//TRACE("keywords taken:%s \n",cstakenString);
			CS_AddCookie(pLine ,pTpwchar_t,wcslen(cstakenString),5, cs_java_c_special_comm_color);
			break;
		}
	}
	return true;
}

static PEditorLine  CS_LanguageParser(PEditorLine pLine ,wchar_t *keyWords[])
{
	int  lastMcroTaken = non ;
    wchar_t *pwchar_t = pLine->rawLine;
    wchar_t *ptaken = cstakenString;
    wchar_t *pTpwchar_t = NULL;

	*ptaken = L'\0';
	//first jump space.
	CS_JumpSignwchar_t(&pwchar_t);
	
	pTpwchar_t = pwchar_t;
	
 
	while( *pwchar_t )
	{
		CS_GetTaken( &pwchar_t);
		if(csLastTaken > 0) //当前为注释 或字串 /*
		{
			if( csLastTaken == commSign21 || csLastTaken == commSign221)
			{
				//csLastTaken = 0;
				//TRACE("注释2 taken:%s \n",cstakenString);
				pLine = CS_FindCommenSign22( pLine ,&pwchar_t);
				csLastTaken = 0;
			}
			else if(csLastTaken == commSign1)// 注释 //
			{
				//TRACE("注释1 taken:%s \n",pTpwchar_t);
				CS_AddCookie(pLine ,pTpwchar_t,wcslen(pTpwchar_t),2, cs_java_c_cplus_plus_comm_color);
				csLastTaken = 0;
				return pLine;
			}else // string 
			{
				//TRACE("string taken:%s \n",cstakenString);
				CS_AddCookie(pLine ,pTpwchar_t,wcslen(cstakenString),4, cs_java_c_cplus_plus_text_color);
				csLastTaken = 0;
			}
		}
		else //当前位关键字吗 ？
		{
			    //TRACE("keywords taken before valid:%s \n",cstakenString);
				if(CS_c_cplusplus_Mcro_keyword_proc_p)
					(*CS_c_cplusplus_Mcro_keyword_proc_p)(pLine,pTpwchar_t,lastMcroTaken);

				if(*cstakenString != 0)
				{
					bool bmcroproc = false;
					if(CS_c_cplusplus_Mcro_keyword_pre_proc_p)
						bmcroproc = ( *CS_c_cplusplus_Mcro_keyword_pre_proc_p)(pLine ,pTpwchar_t,&lastMcroTaken);
					 
					if(false == bmcroproc)
					{
						wchar_t ch ;
						int index;

						if( 0 != CStyleKeyWords_pattern ) {
					    ch = *cstakenString ;
						index = ch - 'a';
						
						if( 0 < index && index < 26)
						{
							wchar_t **p = CStyleKeyWords_pattern[index];
							if(p)
							for( int i = 0;p[i] != 0;i++)
							{
								if(wcscmp(p[i], cstakenString) == 0)
								{
									CS_AddCookie(pLine ,pTpwchar_t,wcslen(cstakenString),5, cs_java_c_cplus_plus_keyword_color);
									break;
								}
							}
							
						}else // CS_Java_c_cplusplus_especial_keyword_proc;
						{
							   (*CS_Java_c_cplusplus_especial_keyword_proc_p)(pLine,pTpwchar_t);
						}

					  }
					}
				}

		}

		CS_JumpSignwchar_t(&pwchar_t);
		pTpwchar_t = pwchar_t;
	}

	csLastTaken = 0;
	return pLine;
}

  
 struct Parser CStyleParser_c_cplusplus = {
	&CS_LanguageParser,
	&CS_InitLanguageKeyWordPattern1,
	0
};

 struct Parser CStyleParser_java = {
	&CS_LanguageParser,
	&CS_InitLanguageKeyWordPattern2,
	0
};

 struct Parser CStyleParser_js = {
	&CS_LanguageParser,
	&CS_InitLanguageKeyWordPattern3,
	0
};
#endif