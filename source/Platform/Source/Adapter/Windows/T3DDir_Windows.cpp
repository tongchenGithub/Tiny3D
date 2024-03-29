/*******************************************************************************
 * This file is part of Tiny3D (Tiny 3D Graphic Rendering Engine)
 * Copyright (C) 2015-2017  Answer Wong
 * For latest info, see https://github.com/asnwerear/Tiny3D
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

#include "T3DDir_Windows.h"
#include <direct.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>

#pragma warning(disable:4244)
#pragma warning(disable:4996)


namespace Tiny3D
{
    Dir_Windows::Dir_Windows()
        : m_hFindFile(INVALID_HANDLE_VALUE)
        , m_bExtractName(false)
    {
        memset(&m_FindFileData, 0, sizeof(m_FindFileData));
    }

    Dir_Windows::~Dir_Windows()
    {
        close();
    }

    bool Dir_Windows::findFile(const String &strPath)
    {
        if (strPath.empty() || strPath == "")
            return false;

#ifdef UNICODE
        WCHAR wszPath[512] = {0};
        ::MultiByteToWideChar(CP_UTF8, 0, strPath.c_str(), strPath.length(), wszPath, sizeof(wszPath));
        m_hFindFile = ::FindFirstFile(wszPath, &m_FindFileData);
#else
        m_hFindFile = ::FindFirstFile(strPath.c_str(), &m_FindFileData);
#endif

        extractRoot(strPath, m_strRoot);

        m_bExtractName = false;

        return (m_hFindFile != INVALID_HANDLE_VALUE);
    }

    bool Dir_Windows::findNextFile()
    {
        BOOL bResult = FALSE;

        if (m_hFindFile != nullptr)
        {
            bResult = ::FindNextFile(m_hFindFile, &m_FindFileData);
            m_bExtractName = false;
        }

        return (bResult == TRUE);
    }

    void Dir_Windows::close()
    {
        if (m_hFindFile != nullptr)
        {
            ::FindClose(m_hFindFile);
            m_hFindFile = INVALID_HANDLE_VALUE;

            memset(&m_FindFileData, 0, sizeof(m_FindFileData));
            m_bExtractName = false;
        }
    }

    String Dir_Windows::getRoot() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return "";

        return m_strRoot;
    }

    String Dir_Windows::getFileName() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return "";

        if (!m_bExtractName)
        {
#ifdef UNICODE
            char szPath[512] = {0};
            ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szPath, sizeof(szPath), nullptr, nullptr);
            String strFilePath(szPath);
#else
            String strFilePath = m_FindFileData.cFileName;
#endif
            extractFileName(strFilePath, m_strName, m_strTitle);
        }

        return m_strName;
    }

    String Dir_Windows::getFilePath() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return "";

#ifdef UNICODE
        char szPath[512] = {0};
        ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szPath, sizeof(szPath), nullptr, nullptr);
        String strPath(szPath);
#else
        String strPath = m_FindFileData.cFileName;
#endif

        if (!m_bExtractName)
        {
            extractFileName(strPath, m_strName, m_strTitle);
        }

        return m_strRoot + strPath;
    }

    String Dir_Windows::getFileTitle() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return "";

        if (!m_bExtractName)
        {
#ifdef UNICODE
            char szPath[512] = {0};
            ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szPath, sizeof(szPath), nullptr, nullptr);
            String strFilePath(szPath);
#else
            String strFilePath = m_FindFileData.cFileName;
#endif
            extractFileName(strFilePath, m_strName, m_strTitle);
        }

        return m_strTitle;
    }

    UINT Dir_Windows::getLength() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return 0;

        return m_FindFileData.nFileSizeLow;
    }

    bool Dir_Windows::isDots() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return false;

        if (!m_bExtractName)
        {
#ifdef UNICODE
            char szPath[512] = {0};
            ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szPath, sizeof(szPath), nullptr, nullptr);
            String strPath(szPath);
#else
            String strPath = m_FindFileData.cFileName;
#endif
            extractFileName(strPath, m_strName, m_strTitle);
        }

        return ((m_FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (m_strName == "." || m_strName == ".."));
    }

    bool Dir_Windows::isDirectory() const
    {
        return (m_hFindFile != INVALID_HANDLE_VALUE && (m_FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY));
    }

    long Dir_Windows::getCreationTime() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return 0;

        struct _stat buffer;
#ifdef UNICODE
        char szFileName[MAX_PATH] = {0};
        ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szFileName, sizeof(szFileName), nullptr, nullptr);
        int result = _stat(szFileName, &buffer);
#else
        int result = _stat(m_FindFileData.cFileName, &buffer);
#endif
        if (result == 0)
        {
            return buffer.st_ctime;
        }

        return 0;
    }

    long Dir_Windows::getLastAccessTime() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return 0;

        struct _stat buffer;
#ifdef UNICODE
        char szFileName[MAX_PATH] = {0};
        ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szFileName, sizeof(szFileName), nullptr, nullptr);
        int result = _stat(szFileName, &buffer);
#else
        int result = _stat(m_FindFileData.cFileName, &buffer);
#endif
        if (result == 0)
        {
            return buffer.st_atime;
        }

        return 0;
    }

    long Dir_Windows::getLastWriteTime() const
    {
        if (m_hFindFile == INVALID_HANDLE_VALUE)
            return 0;

        struct _stat buffer;
#ifdef UNICODE
        char szFileName[MAX_PATH] = {0};
        ::WideCharToMultiByte(CP_UTF8, 0, m_FindFileData.cFileName, MAX_PATH, szFileName, sizeof(szFileName), nullptr, nullptr);
        int result = _stat(szFileName, &buffer);
#else
        String strPath = m_strRoot + m_FindFileData.cFileName;
        int result = _stat(strPath.c_str(), &buffer);
#endif
        if (result == 0)
        {
            return buffer.st_mtime;
        }

        return 0;
    }

    bool Dir_Windows::makeDir(const String &strDir)
    {
        if (strDir.empty() || strDir == "")
            return false;

        return (mkdir(strDir.c_str()) == 0);
    }

    bool Dir_Windows::removeDir(const String &strDir)
    {
        if (strDir.empty() || strDir == "")
            return false;

        return (rmdir(strDir.c_str()) == 0);
    }

    bool Dir_Windows::remove(const String &strFileName)
    {
        if (strFileName.empty() || strFileName == "")
            return false;

        return (::remove(strFileName.c_str()) == 0);
    }

    bool Dir_Windows::exists(const String &strPath) const
    {
        return true;
    }

    String Dir_Windows::getCachePath() const
    {
        char szBuf[MAX_PATH];
#ifdef UNICODE
        WCHAR wszBuf[MAX_PATH];
        ::GetModuleFileName(nullptr, wszBuf, sizeof(wszBuf));
        ::WideCharToMultiByte(CP_UTF8, 0, wszBuf, MAX_PATH, szBuf, sizeof(szBuf), nullptr, nullptr);
#else
        ::GetModuleFileName(nullptr, szBuf, sizeof(szBuf));
#endif
        char *ptr = szBuf;
        while (strchr(ptr, '\\'))
        {
            ptr = strchr(ptr, '\\');
            ptr++;
        }
        *ptr = 0;

        return String(szBuf);
    }

    String Dir_Windows::getAppPath() const
    {
        return getCachePath();
    }

    char Dir_Windows::getNativeSeparator() const
    {
        return '\\';
    }

    bool Dir_Windows::extractRoot(const String &strFilePath, String &strRoot)
    {
        bool bResult = false;
        size_t nPos = strFilePath.rfind("/");

        if (nPos == -1)
        {
            nPos = strFilePath.rfind("\\");
        }

        if (nPos != -1)
        {
            bResult = true;
            size_t nCount = nPos + 1;
            size_t nOffset = 0;
            strRoot = strFilePath.substr(nOffset, nCount);
        }
        else
        {
            bResult = false;
        }

        return bResult;
    }

    bool Dir_Windows::extractFileName(const String &strFilePath, String &strName, String &strTitle) const
    {
        bool bResult = false;
        size_t nLength = strFilePath.length();

        strName = strFilePath;

        size_t nPos = strName.rfind(".");
        size_t nCount = nPos;
        size_t nOffset = 0;
        if (nPos == 0)
            strTitle = "";
        else
            strTitle = strFilePath.substr(nOffset, nCount);

        m_bExtractName = true;

        return bResult;
    }
}
