/**********************************************************************
 * \file Dictionary.h
 * \brief One-file header to store and access a dictionary
 * \author Daniel Allen
 * \date 02 Jul 2021
 *********************************************************************/
#pragma once
#include <future>
#include <iostream>
#include <QTextStream>
#include "PMGlobalDefs.h"
#include <QtCore/qfile.h>

namespace pm {
	// #Dictionary
	/**
	 * @class	Dictionary
	 *
	 * @brief	An evaluator.
	 *
	 * @author	Daniel Allen
	 * @date	02 Jul 2021
	 */
	class Dictionary {
		// #Node
	
	private:
		/**
		* @class	Node
		*
		* @brief	A node.
		*
		* @author	Daniel Allen
		* @date		02 Jul 2021
		*/
		struct Node {
			/** @brief	The child nodes branching from this node */
			std::vector<Node*> children;
			/** @brief	The character stored in this node */
			QChar key = '\0';
			/** @brief	True if this node makes a complete word */
			bool word = false;
			/** @brief	Destination for the */
			static int dest;

			Node();
			~Node();

			[[nodiscard]] Node* at(QChar c) const;
			Node* addChild(QChar key);

			static Node* newNode(QChar key);
		};

		/** @brief	Child Nodes connected this this Node */
		std::vector<Node*> children;
		/** @brief	The words */
		int words = 0;

	public:
		[[nodiscard]] Node* at(const QChar c) const;

		[[nodiscard]] bool contains(const QString& path) const;
		[[nodiscard]] bool containsReverse(const QString& path) const;

		void addWord(const QString word);
		[[nodiscard]] bool ready() const;
		[[nodiscard]] bool readying() const;
	private:
		/** @brief	True once initialization is complete */
		volatile bool initialized = false;
		/** @brief	True while initialization is in progress */
		volatile bool initializing = false;
		const char* filename;
	public:
		Dictionary(const char* filename);

		bool init();
		void asyncInit();
		[[nodiscard]] double passwordStrength(QString password);
		static int run(const QString& string);
		static int charToInt(const char character);
		static bool mixedCase(const QString& word);
		static bool alphanumeric(const std::string& input);
		static QString reverse(QString input);
	};

	/**
	 * English dictionary containing roughly 370000 words, excluding alphanumerics
	 */
	static Dictionary english(":/PasswordManager/English.txt");
}  // namespace pm