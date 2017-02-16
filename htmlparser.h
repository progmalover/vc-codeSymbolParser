#pragma once
struct HtmlElement{
	int ntype ;//: 1 html emement ,2 normal text ,3 js Element
	wchar_t *locStart;//local 的开始地址
	wchar_t *locEnd;//local的结束地址
	//wchar_t *raw;//原始行数据开始地址
	void    *rawEditorLine;
};
extern struct Parser CStyleParser_html;