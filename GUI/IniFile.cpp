// IniFile.cpp: implementation of the CIniFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" // include if used in MFC applications
#include "IniFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIniFile::CIniFile()
{}

CIniFile::CIniFile(LPCTSTR lpPathName)
{
	SetPathName(lpPathName);
}

CIniFile::~CIniFile()
{}

BOOL CIniFile::SetPathName(LPCTSTR lpPathName)
{
	if (lpPathName != NULL)
	{
		m_sPathName = lpPathName;
		return TRUE;
	}

	return FALSE;
}

const CString& CIniFile::GetPathName() const
{
	return m_sPathName;
}

CString CIniFile::GetProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpDefault) const
{
	CString str;
	::GetPrivateProfileString(lpSection, lpEntry, lpDefault, str.GetBuffer(1025) , 1024, m_sPathName);
	str.ReleaseBuffer();
	return str;
}

int CIniFile::GetProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nDefault) const
{
	CString sData = GetProfileString(lpSection, lpEntry, _T("")); // get the string form value

	if (sData.IsEmpty())
		return nDefault;
	else
		return atoi(sData);
}

BOOL CIniFile::GetProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bDefault) const
{
	int nDefault = bDefault ? 1 : 0;
	return (GetProfileInt(lpSection, lpEntry, nDefault) != 0);
}

double CIniFile::GetProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fDefault) const
{
	CString sData = GetProfileString(lpSection, lpEntry, _T("")); // get the string form value

	if (sData.IsEmpty())
		return fDefault;
	else
		return atof(sData);
}

BOOL CIniFile::WriteProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpValue)
{
	return ::WritePrivateProfileString(lpSection, lpEntry, lpValue, m_sPathName);
}

BOOL CIniFile::WriteProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nValue)
{
	CString sVal;
	sVal.Format(_T("%d"), nValue);
	return WriteProfileString(lpSection, lpEntry, sVal);
}

BOOL CIniFile::WriteProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bValue)
{
	return WriteProfileInt(lpSection, lpEntry, bValue ? 1 : 0);
}

BOOL CIniFile::WriteProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fValue)
{
	CString sVal;
	sVal.Format(_T("%f"), fValue);
	return WriteProfileString(lpSection, lpEntry, sVal);
}

DWORD CIniFile::GetProfileSection(LPCTSTR lpSection, CStringArray& aList) const
{
	DWORD seg = 1024;
	TCHAR* pszBuffer = new TCHAR[seg + 1];
	DWORD dwLen = ::GetPrivateProfileSection(lpSection, pszBuffer, seg, m_sPathName);

	while (dwLen > seg - 3)
	{
		seg += 1024;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[seg + 1];
		dwLen = ::GetPrivateProfileSection(lpSection, pszBuffer, seg, m_sPathName);
	}
	
	// By default, delimiter is '\0' (WIN32 API)
	// Replace them by '\n'
	for (DWORD i = 0; i < dwLen; i++)
	{
		if (pszBuffer[i] == _T('\0'))
			pszBuffer[i] = _T('\n');
	}

	CString sAll(pszBuffer);
	delete [] pszBuffer;
	pszBuffer = NULL;

	aList.RemoveAll();

	int nCount = 0;

	CString str;
	int nPosPrev = 0;
	int nPosNew = 0;
	BOOL bContinue = TRUE;

	int n = 0;

	while (bContinue)
	{
		nPosNew = sAll.Find(_T('\n'), nPosPrev);
		if (nPosNew != -1)
		{
			str = sAll.Mid(nPosPrev, nPosNew - nPosPrev);
		}
		else
		{
			str = sAll.Mid(nPosPrev);
			bContinue = FALSE;
		}

		str.TrimLeft();
		str.TrimRight();

		if (!str.IsEmpty())
		{
			aList.Add(str);
		}

		nPosPrev = nPosNew + 1;

		n++;
	}

	return aList.GetSize();
}

DWORD CIniFile::GetProfileSectionNames(CStringArray& aList) const
{
	DWORD nLen = 1024;
	TCHAR* pszBuffer = new TCHAR[nLen + 1];
	DWORD dwLen = ::GetPrivateProfileSectionNames(pszBuffer, nLen, m_sPathName);

	while (dwLen >= nLen - 5)
	{
		nLen += 1024;
		delete [] pszBuffer;
		pszBuffer = new TCHAR[nLen + 1];
		dwLen = ::GetPrivateProfileSectionNames(pszBuffer, nLen, m_sPathName);
	}
	
	// By default, delimiter is '\0' (WIN32 API)
	// Replace them by '\n'
	for (DWORD i = 0; i < dwLen; i++)
	{
		if (pszBuffer[i] == _T('\0'))
			pszBuffer[i] = _T('\n');
	}

	CString sAll(pszBuffer);
	delete [] pszBuffer;
	pszBuffer = NULL;

	aList.RemoveAll();

	int nCount = 0;

	CString str;
	int nPosPrev = 0;
	int nPosNew = 0;
	BOOL bContinue = TRUE;

	int n = 0;

	while (bContinue)
	{
		nPosNew = sAll.Find(_T('\n'), nPosPrev);
		if (nPosNew != -1)
		{
			str = sAll.Mid(nPosPrev, nPosNew - nPosPrev);
		}
		else
		{
			str = sAll.Mid(nPosPrev);
			bContinue = FALSE;
		}

		str.TrimLeft();
		str.TrimRight();

		if (!str.IsEmpty())
		{
			aList.Add(str);
		}

		nPosPrev = nPosNew + 1;

		n++;
	}

	return aList.GetSize();
}

BOOL CIniFile::IncreaseProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nIncrease)
{
	int nVal = GetProfileInt(lpSection, lpEntry, 0);
	nVal += nIncrease;
	return WriteProfileInt(lpSection, lpEntry, nVal);
}

BOOL CIniFile::AppendProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpString)
{
	CString sVal = GetProfileString(lpSection, lpEntry, _T(""));
	sVal += lpString;
	return WriteProfileString(lpSection, lpEntry, sVal);
}

BOOL CIniFile::RemoveProfileSection(LPCTSTR lpSection) const
{
	return ::WritePrivateProfileString(lpSection, NULL, _T(""), m_sPathName);
}

BOOL CIniFile::RemoveProfileEntry(LPCTSTR lpSection, LPCTSTR lpEntry) const
{
	return ::WritePrivateProfileString(lpSection, lpEntry, NULL, m_sPathName);
}







