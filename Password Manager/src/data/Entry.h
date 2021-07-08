#pragma once
#include <string>
#include <boost/serialization/access.hpp>

struct Entry
{
	std::string key;
	std::string username;
	std::string site;
	std::string password;
	std::string note;

	Entry();
	Entry(std::string key, std::string site, std::string password, std::string username, std::string note = "");
	~Entry();
	[[nodiscard]] bool isNew() const;
	void makeOld();
	friend class boost::serialization::access;
	template <class Archive>
	/**
	* @fn	template <class Archive> void Entry::serialize(Archive& ar, const unsigned version)
	*
	* @brief	Serialize this object to the given stream
	*
	* @tparam	Archive	Type of the archive.
	* @param [in,out]	ar	   	The archive.
	* @param 		  	version	The version.
	*/
	void serialize(Archive& ar, const unsigned version) {
		ar& password;
		ar& username;
		ar& site;
		ar& note;
	}
	void lock(const char* key);
	void unlock(const char* key);
private:
	bool blank;
};