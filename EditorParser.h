//
// zhangxy designed for ben zhuangtai in 2011.5.18
// zhuangtai can use this code,but he must keep my author
// name on top of file.
// if someone is  ben,can use this code,other is not. 
// all right reserved.
// 2011.5.18
//
#ifndef _EDITORPARSER
#define _EDITORPARSER

typedef struct  LineDressUp *PLineDressUp;
struct  LineDressUp{
  int beginLoc,endLoc;
  int ntype;//type = 1 :tab key wchar_t. 2: key word 3 : text in ""/''/<>
  int color;
  PLineDressUp next,last;
} ;

typedef struct EditorLine  *PEditorLine;
struct EditorLine{
	bool bTabChIn;//for tab key wchar_t
	wchar_t *rawLine;
	PLineDressUp cookies;
	PEditorLine next,last;
};

typedef   PEditorLine(*LanguageParser)(PEditorLine  ,wchar_t *keyWords[]);
typedef   void ( *InitLanguageKeyWordsPattern)();
typedef struct Parser{
  LanguageParser parser;
  InitLanguageKeyWordsPattern  InitKeyWordsPattern;
  wchar_t **keyWords;
}*PParser;

typedef struct EditorParser {
	int nline;
	PEditorLine line;
	PParser		parser;
	int	errcCode;//0 success ,1 failure :no parser exist. 2:memory error.
} *PEditorParser;
#endif

#ifdef __cplusplus   
extern "C"{
#endif

//editor parser functions. 
extern PEditorParser CreateEditorParser(wchar_t *fileExt);
extern bool			 LoadTextToParser(PEditorParser pParser ,char *pText,int nSize);
extern void			 AddTextLineToParser(PEditorParser pParser ,wchar_t *pText);
extern void          ProcessParserLines(PEditorParser pParser);
extern bool			 ReleaseEditorParser(PEditorParser pParser);
#ifdef __cplusplus   
}
#endif
