#pragma once
struct HtmlElement{
	int ntype ;//: 1 html emement ,2 normal text ,3 js Element
	wchar_t *locStart;//local �Ŀ�ʼ��ַ
	wchar_t *locEnd;//local�Ľ�����ַ
	//wchar_t *raw;//ԭʼ�����ݿ�ʼ��ַ
	void    *rawEditorLine;
};
extern struct Parser CStyleParser_html;