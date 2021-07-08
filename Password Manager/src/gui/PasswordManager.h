#pragma once
#include <QtQuickControls2/qquickstyle.h>
#include "ui_PasswordManager.h"
#include "../resources/pm.h"
#include "../data/Entry.h"
#include "../data/Vault.h"
#include "../Dictionary.h"
#include <fstream>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>
#ifdef PM_DEBUG
#include <QtWidgets/QTextEdit>
#include <QFileDialog>
#endif
class PasswordManager : public QMainWindow
{
	Q_OBJECT

public:
	PasswordManager(QWidget* parent = Q_NULLPTR);
	bool hasVault = false;
	void onEditButtonClicked();
	void updateListDisplay();
	void setCurrentDisplay(Entry& e);
	void saveVault();
	void openVault(const char* filepath);
private slots:
	//edit page
	void onGenerateButtonClicked() const;
	void onGenerateCopyClicked() const;
	void onSiteButtonClicked() const;
	void onPasswordCopyClicked() const;
	void onGenerateViewClicked(bool on) const;
	void onPasswordViewClicked(bool on) const;
	void onPasswordTableClicked(int y, int x);
	void onPasswordTableDoubleClicked(int y, int x);
	void addItemToDisplay(Entry& entry);
	void addBlankEntry();
	void removeEntry();
	void onSearch() const;
	void saveActiveEntry();
	void cancelEdit();

	//create page
	void createVaultBrowseFileButtonClicked();
	void createVaultCreateButtonClicked();

	//open page
	void openVaultButtonClicked();
private:
	Ui::PasswordManager ui;
	Vault activeVault;
	Entry activeEntry;
	bool editing = false;
	unsigned long long activePos = -1;
	volatile bool awaitingDictionary = true;
	
	
};
