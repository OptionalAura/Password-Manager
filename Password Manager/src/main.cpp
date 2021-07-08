
#include "gui/PasswordManager.h"

/**
 * @fn	int main(int argumentCount, char* arguments[])
 *
 * @brief	Main entry-point for this application
 *
 * @author	Daniel Allen
 * @date	06 Jul 2021
 *
 * @param 	argumentCount	The number of command-line arguments provided.
 * @param 	arguments	 	An array of command-line argument strings.
 *
 * @returns	Exit-code for the process - 0 for success, else an error code.
 */
int main(int argumentCount, char* arguments[])
{
	if (sodium_init() == -1) {
		return 1;
	}
	//D:/Programming/C++/Password Manager/x64/Release/
	std::string filepath;
	bool tryingToOpen = false;
	for (int i = 0; i < argumentCount; i++)
	{
		std::string arg = arguments[i];
		LOG("Checking " << arg);
		std::regex r("^.*\.{1}pmvault+$");
		std::smatch sm;
		std::regex_search(arg, sm, r);
		if (!sm.empty())
		{
			filepath = arg;
			tryingToOpen = true;
			break;
		}
	}
	tryingToOpen = true;
	filepath = "C:/Users/dsato/Desktop/home.pmvault";
	LOG("File path: " << filepath);
	QApplication a(argumentCount, arguments);
	PasswordManager w;
	if (tryingToOpen) {
		w.openVault(filepath.c_str());
	}
	w.show();
	const int res = a.exec();
	w.hide();
	if (w.hasVault)
		w.saveVault();
	while (!pm::english.ready() && pm::english.readying()) {
		Sleep(100);
	}
	return res;
}