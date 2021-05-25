//
// Created by ryan1 on 24/05/2021.
//

#include "UI/winUI.hpp"

Solid::HasChosen SOLID_API Solid::WinSelectFolder()
{
	std::wstring res;
	HasChosen hc;
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED |
	                                  COINIT_DISABLE_OLE1DDE);
	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *pFileOpen;

		// Create the FileOpenDialog object.
		hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
		                      IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
		OPENFILENAME ofn = {0};
		TCHAR szFile[260]={0};
		if (SUCCEEDED(hr))
		{
			// Show the Open dialog box.
			if (FAILED(pFileOpen->SetOptions(FOS_PICKFOLDERS | FOS_PATHMUSTEXIST)))
			{

			}
			hr = pFileOpen->Show(NULL);
			// Get the file name from the dialog box.
			if (SUCCEEDED(hr))
			{
				IShellItem *pItem;
				hr = pFileOpen->GetResult(&pItem);
				if (SUCCEEDED(hr))
				{
					PWSTR pszFilePath;

					hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);

					// Display the file name to the user.
					if (SUCCEEDED(hr))
					{
						res = std::wstring(pszFilePath);
						hc.b = true;
					}
					pItem->Release();
				}
			}
			pFileOpen->Release();
		}
		CoUninitialize();
	}
	using convert_type = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_type, wchar_t> converter;
	hc.vstr.push_back(converter.to_bytes( res ));
	return hc;
}

Solid::HasChosen SOLID_API Solid::WinOpenFile(const char *filter)
{
	OPENFILENAME ofn {0};
	TCHAR szFile[1024]={0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT(filter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	int e =GetOpenFileName(&ofn);
	HasChosen hc;

	if(e == TRUE)
	{
		hc.b = true;
		hc.vstr.push_back(std::string (ofn.lpstrFile));
	}
	return hc;
}

Solid::HasChosen SOLID_API Solid::WinSaveFile(const char *filter)
{

	OPENFILENAME ofn {0};
	TCHAR szFile[1024]={0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT(filter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;

	ofn.Flags = OFN_PATHMUSTEXIST ;
	int e =GetSaveFileName(&ofn);
	HasChosen hc;

	if(e == TRUE)
	{
		hc.b = true;
		hc.vstr.push_back(std::string (ofn.lpstrFile));
	}
	return hc;
}

Solid::HasChosen SOLID_API Solid::WinOpenFileMultiSelect(const char *filter)
{
	OPENFILENAME ofn {0};
	TCHAR szFile[1024]={0};
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = TEXT(filter);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;
	int e =GetOpenFileName(&ofn);
	HasChosen hc;

	if(e == TRUE)
	{
		hc.b = true;
		char* str = ofn.lpstrFile;
		std::string directory = str;
		str += ( directory.length() + 1 );
		bool firstPass = false;
		while ( *str ) {
			firstPass = true;
			std::string filename = str;
			str += ( filename.length() + 1 );
			// use the filename, e.g. add it to a vector
			hc.vstr.push_back(directory+"\\"+filename);
		}
		if(!firstPass)
			hc.vstr.push_back(directory);
	}
	return hc;
}
