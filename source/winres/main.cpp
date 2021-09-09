#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ostream>
#include <map>
#include <signal.h>
#include <regex>

using namespace std;


const wstring rx_file_version = L"FILEVERSION";
const wstring rx_production_version = L"PRODUCTVERSION";
const wstring rx_file_string_version = L"\"FileVersion\"";
const wstring rx_production_string_version = L"\"ProductVersion\"";

bool update_version_in_rc_file(string file, string version);

string user_menu[] = {
    "?                      for help",
    "--help                 for help",
    "--version              app version",
    "",
    "--rc-file=<file>       resource file name",
    "--ver-numb=<version>   used version value",
};

map<string, string> parse_cmd(int argc, char** argv)
{
    map<string, string> cmds;

    // regex expression for pattern to be searched 
    regex regexp("(s*(--)+([a-z]+))*=(s*w+)*");

    // flag type for determining the matching behavior (in this case on string objects)
    smatch m;

    for (int i = 1; i < argc; i++)
    {
        string cmd = argv[i];
        regex_search(cmd, m, regexp);
        if (m.size() > 1)
        {
            cmds[m.prefix()] = m.suffix();
        }
        else
        {
            cmds[cmd] = "";
        }
    }

    return cmds;
}

int main(int argc, char *argv[])
{
    map<string, string> cmds = parse_cmd(argc, argv);

    if (cmds.find("?") != cmds.end())
    {
        cout << user_menu << endl;
    }
    else if (cmds.find("--version") != cmds.end())
    {
        cout << "Not implemented." << endl;
    }
    else if (cmds.find("--rc-file") != cmds.end() && cmds.find("--ver-numb") != cmds.end())
    {
        string file = cmds["--rc-file"];
        string ver = cmds["--ver-numb"];
        update_version_in_rc_file(file, ver);

    }

    return 0;
}

bool update_version_in_rc_file(string file, string version)
{
    wstring ver(version.begin(), version.end());
    wstring ver_str(version.begin(), version.end());
    std::replace(ver_str.begin(), ver_str.end(), '.', ',');

    wstring str_line;
    wifstream rc_file(file);
    if (!rc_file)
    {
        cout << "No such file" << endl;
        return false;
    }

    wofstream new_rc_file(file + "_updated.rc");
    while (getline(rc_file, str_line))
    {
        //// Why is the first line of rc file start with '0xfffe'?
        //if (str_line[0] == 255 && str_line[1] == 254)
        //{
        //    str_line = str_line.substr(2);
        //}

        wstring line = str_line;
        std::remove(line.begin(), line.end(), '\0');
        line.resize(line.length() / 2);

        cout << string(line.begin(), line.end());

        std::wsmatch base_match;
        if (std::regex_search(line, base_match, std::wregex(rx_file_version)))
        {
            line = base_match.prefix().str() + base_match[0].str() + L" " + ver_str + L"\n";
        }
        else if (std::regex_search(line, base_match, std::wregex(rx_production_version)))
        {
            line = base_match.prefix().str() + base_match[0].str() + L" " + ver_str + L"\n";
        }
        else if (std::regex_search(line, base_match, std::wregex(rx_file_string_version)))
        {
            line = base_match.prefix().str() + base_match[0].str() + L", \"" + ver + L"\"\0" + L"\n";
        }
        else if (std::regex_search(line, base_match, std::wregex(rx_production_string_version)))
        {
            line = base_match.prefix().str() + base_match[0].str() + L", \"" + ver + L"\"\0" + L"\n";
        }

        new_rc_file << wstring(line.begin(), line.end());
    }

    new_rc_file.close();
    rc_file.close();

    return true;
}