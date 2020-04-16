#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <iomanip>

using namespace std;

void process_1(int procPID)
{
    HANDLE CONST hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    PROCESSENTRY32 peProcessEntry;

    peProcessEntry.dwSize = sizeof(PROCESSENTRY32);
    Process32First(hSnapshot, &peProcessEntry);

    do {
        if (peProcessEntry.th32ProcessID == procPID) {
            MessageBox(NULL, L"Данный процесс уже запущен", L"Сообщение", MB_OK);
            break;
        }
    } while (Process32Next(hSnapshot, &peProcessEntry));

    cout << "End process_1" << endl;

    CloseHandle(hSnapshot);
}

void fileOpen(bool& showP, bool& runProg, int& timingP) {
    ifstream inf;
    inf.open("Config.inf");
    if (!inf.is_open()) {
        cerr << "ERR_001: файл Config.inf не открылся для чтения, обратитесь к справке!" << endl; // cerr - вывод мгновенног сообщения без участия буфера.
        HWND hWnd = GetConsoleWindow();
        ShowWindow(hWnd, SW_SHOW);
        system("pause");
        exit(1);
    }
    else {
        while (!inf.eof()) {
            string inputline{};
            getline(inf, inputline);
            //cout << inputline << endl;            
            if (!(inputline.find("ShowProcess"))) {
                int size = 0;
                while (inputline[size++]);
                /*cout << "**" << size-1 << endl;
                cout << "**" << inputline.find("progID") << endl;
                cout << "**" << inputline.find(":") <<endl;
                cout << "**buf" << size - 1 - inputline.find(":") << endl;*/
                int buf = size - 1 - inputline.find(":");
                char* str = new char[buf];
                int s = 0;
                for (size_t i = inputline.find(":") + 1; i < size; i++)
                {
                    str[s] = inputline[i];
                    s++;
                }
                //cout << "**" << str << endl;
                showP = atoi(str);
                //cout << "**" << showP << endl;
                delete[] str;
            }
            else if (!(inputline.find("TimingProcess"))) {
                int size = 0;
                while (inputline[size++]);
                int buf = size - 1 - inputline.find(":");
                char* str = new char[buf];
                int s = 0;
                for (size_t i = inputline.find(":") + 1; i < size; i++)
                {
                    str[s] = inputline[i];
                    s++;
                }
                timingP = atoi(str);
                delete[] str;
            }
            else if (!(inputline.find("RunProg"))) {
                int size = 0;
                while (inputline[size++]);
                int buf = size - 1 - inputline.find(":");
                char* str = new char[buf];
                int s = 0;
                for (size_t i = inputline.find(":") + 1; i < size; i++)
                {
                    str[s] = inputline[i];
                    s++;
                }
                runProg = atoi(str);
                delete[] str;
            }
        }
    }

    inf.close();
}

void PrintTask(vector<wstring>& stroki) {
    for (size_t i = 0; i < stroki.size(); i++)
    {
        wcout << setw(3) << i <<": "<< stroki[i] << endl;
    }
}

void LoadTask(vector<wstring>& stroki) {
    wifstream inf;
    inf.open("ToDoList.inf");
    if (!inf.is_open()) {
        cerr << "ERR_002: файл ToDoList.inf не открылся для чтения, обратитесь к справке!" << endl; // cerr - вывод мгновенног сообщения без участия буфера.
        HWND hWnd = GetConsoleWindow();
        ShowWindow(hWnd, SW_SHOW);
        system("pause");
        exit(1);
    }
    else {
        while (!inf.eof()) {
            wstring inputline{};
            getline(inf, inputline);
            stroki.push_back(inputline);
        }
    }

    inf.close();
}

void TimeStrToFind(char curTime[]) {

    time_t now = time(0);
    tm* ltm = localtime(&now);

    string strCurTime = ((ltm->tm_year < 10) ? "0" : "") + to_string(1900 + ltm->tm_year) +
         + ((ltm->tm_mon < 10) ? "0" : "") + to_string(1 + ltm->tm_mon) +
         + ((ltm->tm_mday < 10) ? "0" : "") + to_string(ltm->tm_mday) +
         + ((ltm->tm_hour < 10) ? "0" : "") + to_string(ltm->tm_hour) +
         + ((ltm->tm_min < 10) ? "0" : "") + to_string(ltm->tm_min);

    int i = 0;
    while (strCurTime[0+i])
    {
        curTime[i] = strCurTime[0 + i];
        i++;
    }

    /*string strCurTime = ((ltm->tm_hour < 10) ? "0" : "") + to_string(ltm->tm_hour) +
        ":" + ((ltm->tm_min < 10) ? "0" : "") + to_string(ltm->tm_min) +
        "-" + ((ltm->tm_mday < 10) ? "0" : "") + to_string(ltm->tm_mday) +
        "." + ((ltm->tm_mon < 10) ? "0" : "") + to_string(1 + ltm->tm_mon) +
        "." + ((ltm->tm_year < 10) ? "0" : "") + to_string(1900 + ltm->tm_year);*/
}

__int64 CharToInt64(char *curTime) {
    int i = 0;
    __int64 chislo = 0;
    while (curTime[i]) {
        if (isdigit(curTime[i])) chislo += (curTime[i] - 48);
        chislo *= 10;
        i++;
    }
    return chislo / 10;
}

void PrintLocTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char timstr[17]{ 0 };
    cout << "Local time: " << ((ltm->tm_hour < 10) ? "0" : "") << ltm->tm_hour << ":";
    cout << ((ltm->tm_min < 10) ? "0" : "") << ltm->tm_min << "  ";
    cout << ((ltm->tm_mday < 10) ? "0" : "") << ltm->tm_mday << ".";
    cout << ((ltm->tm_mon < 10) ? "0" : "") << 1 + ltm->tm_mon << ".";
    cout << ((ltm->tm_year < 10) ? "0" : "") << 1900 + ltm->tm_year;
}

void fileRec(int progID, bool showP, int timingP) {

    ofstream outf;
    outf.open("Config.inf");
    if (!outf.is_open()) {
        cerr << "Файл не открылся для записи!" << endl; // cerr - вывод мгновенног сообщения без участия буфера.
        system("pause");
        exit(1);
    }
    else {
        outf << "progID:" << progID << endl;	// endl (flash аналогичная команда) так-же очищает буфер, а это значит запись в файл.
        outf << "ShowProcess:" << showP << endl;
        outf << "TimingProcess:" << timingP << endl;
    }
    outf.close();							// close обнуляет буфер, т.е. дозаписывает все изменения в буфер.

}

int main()
{
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_SHOW);

    setlocale(LC_ALL, "");

    int pidId = GetCurrentProcessId();
    const char TaskRunStop[]{ 'R', 'S' };
    char strCurTime[14]{ 0 };
    bool showP = false;
    bool runProg = false;
    int timingP = 1000;
    unsigned int countCicles = 0;
    vector<wstring> stroki;

    LoadTask(stroki);

    do
    {
        fileOpen(showP, runProg, timingP);
        if (showP) {
            ShowWindow(hWnd, SW_SHOW);
            system("cls");
        }
        else ShowWindow(hWnd, SW_HIDE);


        TimeStrToFind(strCurTime);
        PrintLocTime();
        cout << "    (" << strCurTime << ")" << endl;

        cout << "Process PID: " << pidId << endl;
        cout << "Current count: " << countCicles++ << endl;

        cout << "\nCurrent task (Number task, R-run (S-stop), data (yyyyMMddHHmm)) : " << endl;
        PrintTask(stroki);


        for (size_t i = 0; i < stroki.size(); i++)
        {
            char bufSig[128]{ 0 }, taskTime[14]{ 0 };
            int endSig = 0;
            while (stroki[i][endSig] != '/') {
                bufSig[endSig] = stroki[i][endSig];
                endSig++;
            }
            endSig++;
            if (bufSig[0] == TaskRunStop[0]) {
                int tc = 0;
                while (bufSig[2 + tc] != '_')
                {
                    taskTime[tc] = bufSig[2 + tc];
                    tc++;
                };
                if (CharToInt64(taskTime) <= CharToInt64(strCurTime)) {
                    wchar_t massage[128]{};
                    int mc = 0;
                    while (stroki[i][endSig + mc])
                    {
                        massage[mc] = stroki[i][endSig + mc];
                        mc++;
                    };
                    int flag = MessageBox(NULL, massage, L"Task reminder", MB_OKCANCEL);
                    if (flag == 1) {
                        stroki[i][0] = 'S';
                    };
                }
            }
        }


        Sleep(timingP);
    } while (runProg);
}