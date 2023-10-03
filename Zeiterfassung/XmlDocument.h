#pragma once

class CXmlDocument
{
public:
	CXmlDocument(CArchive& ar);
	~CXmlDocument();

	void openTag(CString& tag);
	void closeTag(CString& tag);
	void writeTag(CString& tag, CString& content);
	void writeTag(CString& tag, int content);

private:
	CArchive& m_archive;
	size_t m_indent;
};

