// twl_dialogs.cpp

#include "twl_dialogs.h"
#include <shlobj.h>
#include <algorithm>

bool run_ofd(HWND win, TCHAR* result, const gui_string& caption, const gui_string& fltr, bool multi)
{
	std::wstring filter(fltr);
	filter += L"||";
	std::replace(filter.begin(), filter.end(), L'|', L'\0');
	*result = 0;
	OPENFILENAME ofn{};
	ofn.hwndOwner = win;
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrTitle = caption.c_str();
	ofn.lpstrFilter = filter.c_str();
	ofn.nMaxFile = 1024;
	ofn.lpstrFile = result; // buffer for result
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	if (multi) ofn.Flags |= OFN_ALLOWMULTISELECT;
	return GetOpenFileName(&ofn);
}

bool run_colordlg(HWND win, COLORREF& cl)
{
	static COLORREF custom_colours[16]{};
	CHOOSECOLOR m_choose_color{};
	m_choose_color.lStructSize = sizeof(CHOOSECOLOR);
	m_choose_color.hwndOwner = win;
	m_choose_color.rgbResult = cl;
	m_choose_color.lpCustColors = custom_colours;
	m_choose_color.Flags = CC_RGBINIT | CC_FULLOPEN;
	if (!ChooseColor(&m_choose_color)) return false;
	cl = m_choose_color.rgbResult;
	return true;
}

bool run_seldirdlg(HWND win, TCHAR* result, pchar descr, pchar initial_dir)
{
	BROWSEINFO bi{};
	bi.hwndOwner = win;
	bi.lpszTitle = descr;
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = [](HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
	{
		if (uMsg == BFFM_INITIALIZED) SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
		return 0;
	};
	bi.lParam = (LPARAM)initial_dir;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	bool state = false;
	if (pidl)
	{
		//get the name of the folder and put it in path
		state = SHGetPathFromIDList(pidl, result);

		//free memory used
		IMalloc* imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}
	}
	return state;
}
