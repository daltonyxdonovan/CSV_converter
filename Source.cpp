#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <atlstr.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <shobjidl.h> 

//in the standard namespace
using namespace std;

string fname;

int main()
{
    wchar_t filename[MAX_PATH];
    OPENFILENAMEW ofn;
    ZeroMemory(&filename, sizeof(filename));
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
    ofn.lpstrFilter = L"CSV Files\0*.csv\0Any File\0*.*\0";
    ofn.lpstrFile = filename;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = L"Select a CSV file";
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        fname = CW2A(filename);
        Sleep(1000);
    }
    else
    {
        // All this stuff below is to tell you exactly how you messed up above. 
        // Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
        switch (CommDlgExtendedError())
        {
        case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
        case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
        case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
        case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
        case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
        case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
        case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
        case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
        case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
        case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
        case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
        case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
        case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
        case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
        case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
        default: std::cout << "User cancelled the operation.\n";
        }
    }

    vector<vector<string>> content;
    vector<string> row;
    string line, word;

    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            row.clear();
            stringstream str(line);
            while (getline(str, word, ','))
                row.push_back(word);
            content.push_back(row);
        }
    }

    vector<vector<string>> compiled_csv;
    
    for (int i = 1; i < content.size(); i++)
    {
        for (int j = 0; j < content[i].size(); j++)
        {
            if (j == 0)
            {
                system("CLS");
				cout << "is " << content[i][j] << " a technician?" << endl;
                cout << "accepted answers: any form of yes or y, no or n" << endl;
				string answer;
				cin >> answer;
				
                if (answer == "yes" || answer == "y" || answer == "Y" || answer == "Yes" || answer == "YES")
                {   
                    cout << endl;
                    cout << "do you want to edit the info for " << content[i][j] << "?" << endl;
                    cout << "accepted answers: any form of yes or y, no or n" << endl;
                    cout << endl;
                    string answer2;
                    cin >> answer2;
					
                    if (answer2 == "yes" || answer2 == "y" || answer2 == "Y" || answer2 == "Yes" || answer2 == "YES")
                    {
                        cout << endl;
                        cout << "okay, enter (without the square brackets) [City, State, Radius in miles]" << endl;
                        cout << "keep in mind, if the city has two names (i.e. Saint Cloud, Minnesota), make sure to hyphenate" << endl;
                        cout << "example: Saint-Cloud, Minnesota -- State prefix is okay, shortening words like 'Saint' to 'St' is not." << endl;
                        cout << endl;
                        string city, state, radius;
                        cin >> city >> state >> radius;
                        cout << city << state << radius << endl;
                        content[i][25] = city + " " + state + " " + radius;
						
                        cout << endl;
                        cout << "do you want to edit the phone number for " << content[i][j] << "?" << endl;
                        cout << "accepted answers: any form of yes or y, no or n" << endl;
                        cout << endl;
                        string answer3;
                        cin >> answer3;
						
                        if (answer3 == "yes" || answer3 == "y" || answer3 == "Y" || answer3 == "Yes" || answer3 == "YES")
                        {
                            cout << endl;
                            cout << "okay, now we need the phone number. enter it however you please, it's just for display purposes." << endl;
                            string phone;
                            cin >> phone;
                            content[i][32] = phone;
                        }
                    }
					compiled_csv.push_back(content[i]);
                }
                else if (answer == "whytho")
					i = content.size()-1;
            }
        }
        cout << endl;
    }

    //write compiled_csv to a csv file
	const char filename_cleaner[] = "cleaned_contacts.csv";
	
	ofstream outfile(filename_cleaner);
	if (outfile.is_open())
	{
		for (int i = 0; i < compiled_csv.size(); i++)
		{
			for (int j = 0; j < compiled_csv[i].size(); j++)
			{
				if (j != compiled_csv[i].size())
				    outfile << compiled_csv[i][j] << ",";
                else
					outfile << compiled_csv[i][j];
			}
			outfile << endl;
		}
	}
	
	else
	{
		cout << "unable to open file" << endl;
	}
	
    outfile.close();

    return 0;
}