#include "stdafx.h"
#include "XmlDocument.h"
#include <atlconv.h>


CXmlDocument::CXmlDocument(CArchive& ar)
	: m_archive(ar)
	, m_indent(0)
{
}


CXmlDocument::~CXmlDocument()
{
}

void CXmlDocument::openTag(CString& tag)
{
	CString lineIndent;
	CString lineContent;

	USES_CONVERSION;

	for (size_t i = 0; i < m_indent; i++)
	{
		lineIndent += "    ";
	}

	lineContent.Format(_T("%s<%s>\n"), lineIndent, tag);
	char* buffer = OLE2A(lineContent.GetBuffer());
	m_archive.Write(buffer, strlen(buffer));

	m_indent++;
}

void CXmlDocument::closeTag(CString& tag)
{
	CString lineIndent;
	CString lineContent;

	USES_CONVERSION;

	m_indent--;
	for (size_t i = 0; i < m_indent; i++)
	{
		lineIndent += "    ";
	}

	lineContent.Format(_T("%s</%s>\n"), lineIndent, tag);
	char* buffer = OLE2A(lineContent.GetBuffer());
	m_archive.Write(buffer, strlen(buffer));
}

void CXmlDocument::writeTag(CString& tag, CString& content)
{
	CString lineIndent;
	CString lineContent;

	USES_CONVERSION;

	for (size_t i = 0; i < m_indent; i++)
	{
		lineIndent += "    ";
	}

	lineContent.Format(_T("%s<%s>%s</%s>\n"), lineIndent, tag, content, tag);
	char* buffer = OLE2A(lineContent.GetBuffer());
	m_archive.Write(buffer, strlen(buffer));
}

void CXmlDocument::writeTag(CString& tag, int content)
{
	CString lineIndent;
	CString lineContent;

	USES_CONVERSION;

	for (size_t i = 0; i < m_indent; i++)
	{
		lineIndent += "    ";
	}

	lineContent.Format(_T("%s<%s>%d</%s>\n"), lineIndent, tag, content, tag);
	char* buffer = OLE2A(lineContent.GetBuffer());
	m_archive.Write(buffer, strlen(buffer));
}
