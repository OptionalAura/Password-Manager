/**
 * @file	Password Manager\src\gui\PasswordManager.cpp.
 *
 * @brief	Implements the password manager class
 */
#pragma once
#include "PasswordManager.h"

 // #GUIInitialization
 /**
  * @fn	PasswordManager::PasswordManager(QWidget* parent)
  *
  * @brief	Constructor
  *
  * @author	Daniel Allen
  * @date	04 Jul 2021
  *
  * @param [in,out]	parent	If non-null, the parent.
  */
PasswordManager::PasswordManager(QWidget* parent)
	: QMainWindow(parent) {
	pm::english.asyncInit();
	ui.setupUi(this);

	ui.addPasswordButton->SetQssProperty("padding", "0px 0px 0px 0px");
	ui.addPasswordButton->SetQssProperty("width", "25px");
	ui.addPasswordButton->SetQssProperty("height", "25px");
	ui.removePasswordButton->SetQssProperty("width", "25px");
	ui.removePasswordButton->SetQssProperty("height", "25px");
	ui.removePasswordButton->SetQssProperty("padding", "0px 0px 0px 0px");
	ui.addPasswordButton->updateStyleSheet();
	ui.removePasswordButton->updateStyleSheet();

	ui.siteOpenButton->SetIconOnly(QIcon(":/PasswordManager/web_white.svg"),
		QIcon(":/PasswordManager/web_gray.svg"));
	ui.saveButton->SetIconOnly(QIcon(":/PasswordManager/check_green.svg"),
		QIcon(":/PasswordManager/check_white.svg"));
	ui.searchbutton->SetIconOnly(QIcon(":/PasswordManager/search_white.svg"),
		QIcon(":/PasswordManager/search_gray.svg"));
	ui.generateCopy->SetIconOnly(QIcon(":/PasswordManager/copy_white.svg"),
		QIcon(":/PasswordManager/copy_gray.svg"));
	ui.passwordCopyButton->SetIconOnly(QIcon(":/PasswordManager/copy_white.svg"),
		QIcon(":/PasswordManager/copy_gray.svg"));
	ui.usernameCopyButton->SetIconOnly(QIcon(":/PasswordManager/copy_white.svg"),
		QIcon(":/PasswordManager/copy_gray.svg"));
	ui.cancelEditButton->SetIconOnly(QIcon(":/PasswordManager/close_red.svg"),
		QIcon(":/PasswordManager/close_white.svg"));
	ui.editButton->SetIconOnly(QIcon(":/PasswordManager/pencil_white.svg"),
		QIcon(":/PasswordManager/pencil_gray.svg"));
	ui.addPasswordButton->SetIconOnly(QIcon(":/PasswordManager/plus_white.svg"),
		QIcon(":/PasswordManager/plus_gray.svg"));
	ui.removePasswordButton->SetIconOnly(QIcon(":/PasswordManager/minus_white.svg"),
		QIcon(":/PasswordManager/minus_gray.svg"));
	ui.saveButton->setVisible(false);
	ui.cancelEditButton->setVisible(false);

	// #connections
	//edit page
	connect(ui.siteOpenButton, &QPushButton::clicked, this, &PasswordManager::onSiteButtonClicked);
	connect(ui.generateButton, &QPushButton::clicked, this, &PasswordManager::onGenerateButtonClicked);
	connect(ui.generateCopy, &QPushButton::clicked, this, &PasswordManager::onGenerateCopyClicked);
	connect(ui.passwordCopyButton, &QPushButton::clicked, this, &PasswordManager::onPasswordCopyClicked);
	connect(ui.viewGeneratedPasswordCheckBox, &QPushButton::clicked, this, &PasswordManager::onGenerateViewClicked);
	connect(ui.viewPasswordCheckbox, &QPushButton::clicked, this, &PasswordManager::onPasswordViewClicked);
	connect(ui.passwordTable, &QTableWidget::cellDoubleClicked, this, &PasswordManager::onPasswordTableDoubleClicked);
	connect(ui.passwordTable, &QTableWidget::cellClicked, this, &PasswordManager::onPasswordTableClicked);
	connect(ui.addPasswordButton, &QPushButton::clicked, this, &PasswordManager::addBlankEntry);
	connect(ui.removePasswordButton, &QPushButton::clicked, this, &PasswordManager::removeEntry);
	connect(ui.searchbutton, &QPushButton::clicked, this, &PasswordManager::onSearch);
	connect(ui.saveButton, &QPushButton::clicked, this, &PasswordManager::saveActiveEntry);
	connect(ui.cancelEditButton, &QPushButton::clicked, this, &PasswordManager::cancelEdit);
	connect(ui.editButton, &QPushButton::clicked, this, &PasswordManager::onEditButtonClicked);

	//create vault page
	connect(ui.createVaultBrowseFileButton, &QPushButton::clicked, this, &PasswordManager::createVaultBrowseFileButtonClicked);
	connect(ui.createVaultCreateButton, &QPushButton::clicked, this, &PasswordManager::createVaultCreateButtonClicked);

	//open vault page

	connect(ui.openVaultButton, &QPushButton::clicked, this, &PasswordManager::openVaultButtonClicked);

	LOG("Debugging");
	ui.passwordTable->setHorizontalHeaderLabels({ "", "Site", "Username" });
	ui.passwordTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeMode::ResizeToContents);
	ui.passwordTable->horizontalHeader()->resizeSection(0, 20);
}

/**
 * @fn	void PasswordManager::onSiteButtonClicked() const
 *
 * @brief	Executes the 'site button clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onSiteButtonClicked() const
{
	[[maybe_unused]] const bool result = pm::openURL(ui.siteOutput->text().toStdString());
	LOG((result ? "Successfully opened " : "Failed to open ") << ui.siteOutput->text().toStdString());
}

/**
 * @fn	void PasswordManager::onGenerateButtonClicked() const
 *
 * @brief	Executes the 'generate button clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onGenerateButtonClicked() const
{
	const int length = ui.generateSpinner->value();
	ui.generateOutput->setText(pm::generateRandomPassword(length).c_str());
}

/**
 * @fn	void PasswordManager::onEditButtonClicked()
 *
 * @brief	Executes the 'edit button clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onEditButtonClicked()
{
	if (activePos != -1)
	{
		editing = true;

		ui.cancelEditButton->setVisible(true);
		ui.saveButton->setVisible(true);
		ui.editButton->setVisible(false);

		ui.usernameOutput->setReadOnly(false);
		ui.passwordOutput->setReadOnly(false);
		ui.siteOutput->setReadOnly(false);
		ui.noteTextEdit->setReadOnly(false);
		if (ui.editButton->hovering)
			ui.saveButton->setIcon(ui.saveButton->hoverIcon);
	}
}

/**
 * @fn	void PasswordManager::updateListDisplay()
 *
 * @brief	Updates the list display
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::updateListDisplay()
{
	ui.passwordTable->clearContents();
	ui.passwordTable->setRowCount(activeVault.entries.size());
	LOG("Size of table: " << ui.passwordTable->rowCount());
	for (int i = 0; i < activeVault.entries.size(); i++)
	{
		if (pm::english.ready()) {
			const double strength = pm::english.passwordStrength(activeVault.entries.at(i).password.c_str());
			ui.passwordTable->setItem(i, 0, new QTableWidgetItem(QString::number(strength)));
		}
		else
		{
			ui.passwordTable->setItem(i, 0, new QTableWidgetItem("..."));
		}
		ui.passwordTable->setItem(i, 1, new QTableWidgetItem(activeVault.entries.at(i).site.c_str()));
		ui.passwordTable->setItem(i, 2, new QTableWidgetItem(activeVault.entries.at(i).username.c_str()));
		LOG("Added " << activeVault.entries.at(i).site);
	}
	if (awaitingDictionary)
	{
		awaitingDictionary = false;
		std::thread t([&]
			{
				while (!pm::english.ready()) {}
				for (int i = 0; i < activeVault.entries.size(); i++)
				{
					const double strength = pm::english.passwordStrength(activeVault.entries.at(i).password.c_str());
					ui.passwordTable->setItem(i, 0, new QTableWidgetItem(QString::number(strength)));
				}
			});
		t.detach();
	}
}

/**
 * @fn	void PasswordManager::addItemToDisplay(Entry& entry)
 *
 * @brief	Adds an item to display
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param [in,out]	entry	The entry.
 */
void PasswordManager::addItemToDisplay(Entry& entry)
{
	LOG("Password strength of " << activeVault.entries.at(activeVault.entries.size() - 1).password << " is " << pm::english.passwordStrength(activeVault.entries.at(activeVault.entries.size() - 1).password.c_str()));
	activeVault.entries.emplace_back(entry);
	LOG("Added to display");
	ui.passwordTable->setRowCount(activeVault.entries.size());
	const double strength = pm::english.passwordStrength(activeVault.entries.at(activeVault.entries.size() - 1).password.c_str());
	ui.passwordTable->setItem(activeVault.entries.size() - 1, 0, new QTableWidgetItem(QString::number(strength)));
	ui.passwordTable->setItem(activeVault.entries.size() - 1, 1,
		new QTableWidgetItem(activeVault.entries.at(activeVault.entries.size() - 1).site.c_str()));
	ui.passwordTable->setItem(activeVault.entries.size() - 1, 2,
		new QTableWidgetItem(activeVault.entries.at(activeVault.entries.size() - 1).username.c_str()));
}

/**
 * @fn	void PasswordManager::addBlankEntry()
 *
 * @brief	Adds blank entry
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::addBlankEntry()
{
	if (editing)
		return;
	activePos = activeVault.entries.size();
	Entry e;
	if (activeEntry.isNew())
		activeEntry = e;
	else
		setCurrentDisplay(e);
	onEditButtonClicked();
	ui.siteOutput->setFocus();
	LOG("Added blank entry");
}

void PasswordManager::removeEntry()
{
	if (activePos != -1)
	{
		activeVault.entries.erase(activeVault.entries.begin() + activePos);
		activePos = -1;
		updateListDisplay();
	}
}

/**
 * @fn	void PasswordManager::setCurrentDisplay(Entry& e)
 *
 * @brief	Sets current display
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param [in,out]	e	an Entry to process.
 */
void PasswordManager::setCurrentDisplay(Entry& e)
{
	activeEntry = e;
	ui.passwordOutput->setText(e.password.c_str());
	ui.siteOutput->setText(e.site.c_str());
	ui.usernameOutput->setText(e.username.c_str());
	ui.noteTextEdit->setText(e.note.c_str());
}
void PasswordManager::saveVault()
{
	activeVault.close();
}
void PasswordManager::openVault(const char* filepath)
{
	ui.stackedWidget->setCurrentIndex(1);
	LOG("Opening vault");
	{
		activeVault = Vault();
		if (std::fstream ifs(filepath, std::ios::in | std::ios::binary); ifs.good() && ifs.is_open()) {
			LOG("Deeper inside");
			boost::archive::binary_iarchive ia(ifs);
			ia & activeVault;
		} else
		{
			activeVault.name = "New Vault";
			activeVault.hash = "Hash";
			activeVault.entries = std::vector<Entry>();
			activeVault.salt = "Salt";
		}
		activeVault.filepath = filepath;
		for (Entry& e : activeVault.entries)
		{
			e.makeOld();
		}
		std::stringstream windowTitle;
		windowTitle << "Password Manager - " << activeVault.name;
		window()->setWindowTitle(windowTitle.str().c_str());
	}
	hasVault = true;
	LOG("Opened vault");
	updateListDisplay();
}

/**
 * @fn	void PasswordManager::onGenerateCopyClicked() const
 *
 * @brief	Executes the 'generate copy clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onGenerateCopyClicked() const
{
	std::string text = ui.generateOutput->text().toStdString();
	std::thread(&pm::copyToClipboard, text).detach();

	//pm::copyToClipboard(text);
	LOG("Copied generated " << text);
}

/**
 * @fn	void PasswordManager::onPasswordCopyClicked() const
 *
 * @brief	Executes the 'password copy clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onPasswordCopyClicked() const
{
	std::string text = ui.generateOutput->text().toStdString();
	std::thread(&pm::copyToClipboard, text).detach();
	LOG("Copied password " << text);
}

/**
 * @fn	void PasswordManager::onGenerateViewClicked(bool on) const
 *
 * @brief	Executes the 'generate view clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	on	True to on.
 */
void PasswordManager::onGenerateViewClicked(bool on) const
{
	if (on)
		ui.generateOutput->setEchoMode(QLineEdit::EchoMode::Normal);
	else
		ui.generateOutput->setEchoMode(QLineEdit::EchoMode::Password);
	LOG("Toggled view generated to " << (on ? "true" : "false"));
}

/**
 * @fn	void PasswordManager::onPasswordViewClicked(bool on) const
 *
 * @brief	Executes the 'password view clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	on	True to on.
 */
void PasswordManager::onPasswordViewClicked(bool on) const
{
	if (on)
		ui.passwordOutput->setEchoMode(QLineEdit::EchoMode::Normal);
	else
		ui.passwordOutput->setEchoMode(QLineEdit::EchoMode::Password);
	LOG("Toggled view password to " << (on ? "true" : "false"));
}

/**
 * @fn	void PasswordManager::onPasswordTableDoubleClicked(int y, int x)
 *
 * @brief	Executes the 'password table double clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	y	The y coordinate.
 * @param 	x	The x coordinate.
 */
void PasswordManager::onPasswordTableDoubleClicked(int y, int x)
{
	LOG("Clicked (" << x << ", " << y << ") in table");
	LOG("Length of array: " << activeVault.entries.size());
	if (editing)
		return;
	activePos = -1;
	if (activeVault.entries.empty())
		return;
	activePos = min(activeVault.entries.size() - 1, y);
	LOG("activePos: " << activePos);
	setCurrentDisplay(activeVault.entries.at(activePos));
	onEditButtonClicked();
	switch (x)
	{
	case 1:
		ui.siteOutput->setFocus();
		break;
	case 2:
		ui.usernameOutput->setFocus();
		break;
	}
}

/**
 * @fn	void PasswordManager::onPasswordTableClicked(int y, int x)
 *
 * @brief	Executes the 'password table clicked' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	y	The y coordinate.
 * @param 	x	The x coordinate.
 */
void PasswordManager::onPasswordTableClicked(int y, int x)
{
	LOG("Clicked (" << x << ", " << y << ") in table");
	LOG("Length of array: " << activeVault.entries.size());
	if (editing)
		return;
	activePos = -1;
	if (activeVault.entries.empty())
		return;
	activePos = min(activeVault.entries.size() - 1, y);
	LOG("activePos: " << activePos);
	setCurrentDisplay(activeVault.entries.at(activePos));
	switch (x)
	{
	case 1:
		ui.siteOutput->setFocus();
		break;
	case 2:
		ui.usernameOutput->setFocus();
		break;
	}
}

/**
 * @fn	void PasswordManager::onSearch() const
 *
 * @brief	Executes the 'search' action
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::onSearch() const
{
	QString str = ui.searchbar->text();
	LOG("Searched for " << str.toStdString());
}

/**
 * @fn	void PasswordManager::saveActiveEntry()
 *
 * @brief	Saves the active entry
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::saveActiveEntry()
{
	editing = false;
	ui.cancelEditButton->setVisible(false);
	ui.saveButton->setVisible(false);
	ui.editButton->setVisible(true);

	ui.usernameOutput->setReadOnly(true);
	ui.passwordOutput->setReadOnly(true);
	ui.siteOutput->setReadOnly(true);
	ui.noteTextEdit->setReadOnly(true);

	activeEntry.username = ui.usernameOutput->text().toStdString();
	activeEntry.password = ui.passwordOutput->text().toStdString();
	activeEntry.site = ui.siteOutput->text().toStdString();
	activeEntry.note = ui.noteTextEdit->toPlainText().toStdString();

	if (activeEntry.isNew())
	{
		activeEntry.makeOld();
		activeVault.entries.push_back(activeEntry);
	}
	else
	{
		activeVault.entries.at(activePos) = activeEntry;
	}
	updateListDisplay();
	LOG("Saved edit");
}

/**
 * @fn	void PasswordManager::cancelEdit()
 *
 * @brief	Cancels the current edit
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void PasswordManager::cancelEdit()
{
	editing = false;
	ui.cancelEditButton->setVisible(false);
	ui.saveButton->setVisible(false);
	ui.editButton->setVisible(true);

	ui.usernameOutput->setReadOnly(true);
	ui.passwordOutput->setReadOnly(true);
	ui.siteOutput->setReadOnly(true);
	ui.noteTextEdit->setReadOnly(true);

	ui.usernameOutput->setText(activeEntry.username.c_str());
	ui.passwordOutput->setText(activeEntry.password.c_str());
	ui.siteOutput->setText(activeEntry.site.c_str());
	ui.noteTextEdit->setText(activeEntry.note.c_str());
	if (activeEntry.isNew())
	{
	}
	LOG("Cancelled edit");
}

void PasswordManager::createVaultBrowseFileButtonClicked()
{
	QString path = QFileDialog::getSaveFileName(this, tr("Open Vault"),
		"/home",
		tr("Vaults (*.pmvault)"));
	if (!path.isNull())
	{
		ui.createVaultPathField->setText(path);
	}
}

void PasswordManager::createVaultCreateButtonClicked()
{
	if (ui.createVaultPasswordField->text() != ui.createVaultConfirmPasswordField->text())
		return;
	activeVault.close();
	activeVault = Vault();
	
	activeVault.name = ui.createVaultNameField->text().toStdString();
	activeVault.filepath = ui.createVaultPathField->text().toStdString();
	activeVault.key = ui.createVaultPasswordField->text().toStdString();
	activeVault.hash = pm::passwordHash(activeVault.key.c_str());
	activeVault.open(activeVault.key.c_str());
	std::string filepath = ui.createVaultPathField->text().toStdString();
	LOG("Checking " << filepath);
	std::regex r("^.*\.{1}pmvault+$");
	std::smatch sm;
	std::regex_search(filepath, sm, r);
	if (!sm.empty()) {
		saveVault();
		std::stringstream windowTitle;
		windowTitle << "Password Manager - " << activeVault.name;
		window()->setWindowTitle(windowTitle.str().c_str());
		ui.stackedWidget->setCurrentIndex(2);
		LOG("Opened vault");
	}
}

void PasswordManager::openVaultButtonClicked()
{
	const char* input = ui.openVaultPasswordField->text().toStdString().c_str();
	if (const std::string computedHash = pm::passwordHash(input); activeVault.hash == computedHash)
	{
		activeVault.open(input);
		activeVault.hash = computedHash;
		ui.stackedWidget->setCurrentIndex(2);
	}
	else
	{
		//todo provide user feedback on incorrect password
		LOG("Invalid password");
	}
}
