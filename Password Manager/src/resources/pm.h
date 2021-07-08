#pragma once
#ifndef PMUtils
#define PMUtils
#include <sstream>
#include <random>
#include <windows.h>
#include "../PMGlobalDefs.h"
#include <thread>
#include <regex>
#include "sodium.h"
namespace pm {
	static double getQColorLightness(const QColor& bgColor, double gamma = 2.4) {
		double r = static_cast<double>(bgColor.red()) / 255;
		double g = static_cast<double>(bgColor.green()) / 255;
		double b = static_cast<double>(bgColor.blue()) / 255;

		r = r < 0.0031308 ? 12.92 * r : 1.055 * pow(r, 1 / gamma);
		g = g < 0.0031308 ? 12.92 * g : 1.055 * pow(g, 1 / gamma);
		b = b < 0.0031308 ? 12.92 * b : 1.055 * pow(b, 1 / gamma);

		return 0.2126 * r + 0.7152 * g + 0.0722 * b;
	}
	static bool copyToClipboard(std::string str) noexcept {
		if (!OpenClipboard(GetDesktopWindow())) {
			return false;
		}
		EmptyClipboard();
		HGLOBAL glob = GlobalAlloc(GMEM_MOVEABLE, (str.size() + 1));
		if (glob == NULL) {
			CloseClipboard();
			return false;
		}
		LPTSTR lptstr = (LPTSTR)GlobalLock(glob);
		if (lptstr != 0)
			memcpy(lptstr, str.c_str(), (str.size() + 1));
		GlobalUnlock(glob);
		if (lptstr != 0)
			SetClipboardData(CF_TEXT, glob);
		CloseClipboard();
		return true;
	}

	static std::string generateRandomPassword(int length) noexcept {
		if (length <= 0)
			return "";
		std::stringstream ss;

		int start = '0';
		int end = 'z';
		for (int i = 0; i < length; i++) {
			char rand;
			rand = randombytes_uniform('z' + 1 - '0') + '0';
			ss << rand;
		}
		return ss.str();
	}
	static QColor getComplementary(const QColor& color) {
		return QColor(255 - color.red(), 255 - color.green(), 255 - color.blue(), color.alpha());
	}
	static QColor getOptimalFontColor(const QColor& bgColor) {
		LOG(QString("Lightness of (%1, %2, %3)").arg(bgColor.red()).arg(bgColor.green()).arg(bgColor.blue()).toStdString().c_str() << " = " << pm::getQColorLightness(bgColor));
		if (pm::getQColorLightness(bgColor) < 0.6) {
			return QColor(255, 255, 255, 0.8 * 255);
		}
		return QColor(0, 0, 0, 1);
	}
	static int calculatePasswordStrength(const QString& password) {
		return password.length();
	}
	static int calculatePasswordStrength(const QString& password, const QString& key) {
		return 0;
	}
	static bool isValidURL(const QString& url) {
		if (url.length() == 0)
			return false;
		if (url.contains(" ")) {
			return false;
		}
		return true;
	}
	static const QString makeValidURL(const QString& url) {
		QString output;
		if (!url.startsWith("http://www.") && !url.startsWith("https://www.")) {
			output.append("https://www.");
		}
		else if (!url.startsWith("http://www") && !url.startsWith("https://www")) {
			output.append("https://www");
		}
		else if (!url.startsWith("http://") && !url.startsWith("https://")) {
			output.append("https://");
		}
		output.append(url);
		return output;
	}
	static bool openURL(const std::string& url) {
		std::regex r("^(http[s]?|ftp):\/?\/?([^:\/\s]+)");
		std::smatch sm;
		std::regex_search(url, sm, r);
		if(sm.size() != 3){ return false; }
		LOG("Attemping to open URL: " << url);
		
#if defined(_WIN32) || defined(_WIN64)
		ShellExecuteA(NULL, "open", url.c_str(), 0, 0, SW_SHOW);
#elif defined(__APPLE__)
		QString output = "open ";
		output.append(url);
		system(output.toStdString().c_str());
#else
		QString output = "open ";
		output.append(url);
		system(output.toStdString().c_str());
#endif
		return true;
	}
	static std::string passwordHash(const char* plaintext)
	{
		LOG("Generating hash");
		char hash[crypto_pwhash_STRBYTES];
		if (crypto_pwhash_str
		(hash, plaintext, strlen(plaintext),
			crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
			/* out of memory */
		}
		LOG("Generated hash");
		return std::string(hash);
	}
}
#endif