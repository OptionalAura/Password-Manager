#pragma once
#include <fstream>
#include <vector>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>
#include "Entry.h"
#include <string>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

struct Vault
{
private:
	bool unlocked = false;
public:
	std::string filepath;
	std::string name;
	std::string hash;
	std::string key;
	std::string salt;
	std::vector<Entry> entries;
	Vault() {
		unsigned char salt[crypto_pwhash_SALTBYTES];
		randombytes_buf(salt, sizeof(salt));
		//this->salt = std::string(reinterpret_cast<char const*>(salt), crypto_pwhash_SALTBYTES);
	}
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& name;
		ar& hash;
		ar& entries;
		ar& salt;
	}

	void close()
	{
		for (Entry& e : entries)
		{
			//e.lock(key.c_str());
		}
		//todo implement code here
		LOG("Saving vault");
		{
			if (std::fstream ofs(filepath, std::ios::out | std::ios::binary); ofs.is_open())
			{
				boost::archive::binary_oarchive oa(ofs);
				oa& this;
			}
		}
		LOG("Saved vault");
	}
	void open(const char* password)
	{
		unsigned char key[crypto_box_SEEDBYTES];
		if (crypto_pwhash(key,
			sizeof(key),
			password,
			strlen(password),
			reinterpret_cast<const unsigned char*>(salt.c_str()),
			crypto_pwhash_OPSLIMIT_INTERACTIVE,
			crypto_pwhash_MEMLIMIT_INTERACTIVE, crypto_pwhash_ALG_DEFAULT) != 0)
			return;
		//this->key = std::string(reinterpret_cast<char const*>(key), crypto_box_SEEDBYTES);
		for (Entry& e : entries)
		{
			//e.unlock(this->key.c_str());
		}
		unlocked = true;
	}
};
