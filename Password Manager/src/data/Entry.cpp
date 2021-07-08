#include "Entry.h"

#include <sodium/crypto_secretbox.h>
#include <sodium/randombytes.h>
#include <sodium/utils.h>

#include <utility>

/**
 * @fn	Entry::Entry()
 *
 * @brief	Default constructor
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
Entry::Entry() : Entry("", "", "", "")
{
	this->blank = true;
}

/**
 * @fn	Entry::Entry(const QString key, const QString site, const QString password, const QString username)
 *
 * @brief	Constructor
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @param 	key			The key.
 * @param 	site		The site.
 * @param 	password	The password.
 * @param 	username	The username.
 * @param	note		Additional notes linked to this Entry
 */
Entry::Entry(std::string key, std::string site, std::string password, std::string username, std::string note)
{
	this->username = std::move(username);
	this->key = std::move(key);
	this->site = std::move(site);
	this->password = std::move(password);
	this->note = std::move(note);
	blank = false;
}

/**
 * @fn	Entry::~Entry()
 *
 * @brief	Destructor
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
Entry::~Entry()
{
}

/**
 * @fn	bool Entry::isNew() const
 *
 * @brief	Query if this object is new
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 *
 * @returns	True if new, false if not.
 */
bool Entry::isNew() const {
	return blank;
}
//TODO rename this method
/**
 * @fn	void Entry::makeOld()
 *
 * @brief	States that this object is not new
 *
 * @author	Daniel Allen
 * @date	04 Jul 2021
 */
void Entry::makeOld() {
	blank = false;
}
void Entry::lock(const char* key)
{
	
}
void Entry::unlock(const char* key)
{
	
}
