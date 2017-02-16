#include "editorParser.h"
#include <stdlib.h>
#include <string.h>
//
//free cookies
//
 void  FreeCookies(PLineDressUp cookie)
{
	if(cookie)
	do{
		PLineDressUp tmp = cookie;
		cookie = tmp->next;
		free(tmp);
	}
	while(cookie);
}

//
//free line
//
 void FreeLine(PEditorLine pline)
{
	if(pline)
	{
		FreeCookies(pline->cookies);
		free(pline->rawLine);
		free(pline);
	}
}
//
//free EditorParser lines
//
 void FreeLines( PEditorParser parser)
{
	PEditorLine pline = parser->line ;
	while(pline)
	{
		PEditorLine tp = pline->next;
		FreeLine(pline);
		pline = tp;
	}
}

 //
 //add coolie.
 //
 void AddCookie(PEditorLine pline,int nType,int beginLoc,int endLoc ,long clr)
 {
	 PLineDressUp tp = (PLineDressUp)calloc(sizeof(struct LineDressUp),1);
	 tp->color = clr;
	 tp->beginLoc = beginLoc;
	 tp->endLoc = endLoc;
	 tp->ntype = nType;
	 
	 if(pline->cookies)
	 {
		 tp->next = pline->cookies;
		 pline->cookies->last = tp;
		 pline->cookies = tp;
	 }else
		 pline->cookies = tp;
 }
