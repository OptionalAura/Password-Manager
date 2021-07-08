#include "Dictionary.h"
/**
 * @fn	Node::Node()
 *
 * @brief	Default constructor
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 */
pm::Dictionary::Node::Node() = default;
/**
 * @fn	Node::~Node()
 *
 * @brief	Destructor
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 */
pm::Dictionary::Node::~Node()
{
	for (const Node* n : children) {
		delete n;
	}
}
/**
 * @fn	Node* Node::at(char c) const
 *
 * @brief	Ats the given c
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	c	The character.
 *
 * @returns	Null if it fails, else a pointer to a Node.
 */
pm::Dictionary::Node* pm::Dictionary::Node::at(QChar c) const
{
	for (Node* n : children) {
		if (n->key == c.toLower()) return n;
	}
	return nullptr;
}
/**
 * @fn	Node* Node::addChild(char key)
 *
 * @brief	Adds a child
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	key	The key.
 *
 * @returns	Null if it fails, else a pointer to a Node.
 */
pm::Dictionary::Node* pm::Dictionary::Node::addChild(QChar key)
{
	Node* child = newNode(key);
	children.push_back(child);
	return child;
}
/**
 * @fn	static Node* Node::newNode(char key)
 *
 * @brief	Creates a new node
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	key	The key.
 *
 * @returns	Null if it fails, else a pointer to a Node.
 */
pm::Dictionary::Node* pm::Dictionary::Node::newNode(QChar key)
{
	const auto temp = new Node();
	temp->key = key;
	return temp;
}
/**
 * @fn	Node* Tree::at(char c) const
 *
 * @brief	Returns the Node at the specified character. This will return
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	c	The character.
 *
 * @returns	Null if it fails, else a pointer to a Node.
 */
pm::Dictionary::Node* pm::Dictionary::at(const QChar c) const
{
	for (Node* n : children) {
		if (n->key == c) return n;
	}
	return nullptr;
}
/**
 * @fn	bool Tree::contains(const std::string& path) const
 *
 * @brief	Query if this object contains the given path
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	path	The const std::string&amp; to test for containment.
 *
 * @returns	True if the object is in this collection, false if not.
 */
bool pm::Dictionary::contains(const QString & path) const
{
	if (auto origin = at(path[0].toLower()); origin != nullptr) {
		for (int i = 1; i < path.length(); i++) {
			origin = origin->at(path[i].toLower());
			if (origin == nullptr) return false;
		}
		return origin->word;
	}
	return false;
}
/**
 * @fn	bool Tree::containsReverse(const std::string& path) const
 *
 * @brief	Query if this object contains the reverse of the given path
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	path	Full pathname of the file.
 *
 * @returns	True if it succeeds, false if it fails.
 */
bool pm::Dictionary::containsReverse(const QString & path) const
{
	auto origin = at(path[path.length() - 1].toLower());
	if (origin != nullptr) {
		for (int i = path.length() - 1; i > 0; i--) {
			origin = origin->at(path[i - 1].toLower());
			if (origin == nullptr) return false;
		}
		return origin->word;
	}
	return false;
}
/**
 * @fn	void Tree::addWord(const std::string& word)
 *
 * @brief	Adds a word
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	word	The word.
 */
void pm::Dictionary::addWord(const QString word)
{
	if (at(word[0].toLower()) == nullptr) {
		children.push_back(Node::newNode(word[0].toLower()));
	}
	auto origin = at(word[0].toLower());
	//LOG("Created node: " << origin->key.toLatin1());
	for (int i = 1; i < word.length(); i++) {
		if (origin->at(word[i].toLower()) == nullptr) {
			origin->children.push_back(Node::newNode(word[i].toLower()));
		}
		origin = origin->at(word[i].toLower());
		//LOG("Created node: " << origin->key.toLatin1());
	}
	if (origin->word == false) words++;
	origin->word = true;
	//LOG("------------------");
}

/**
 * @fn	bool pm::Dictionary::ready() const
 *
 * @brief	Checks if this object is ready to be used
 *
 * @author	Daniel Allen
 * @date	07 Jul 2021
 *
 * @returns	True if it is initialized, false otherwise.
 */
bool pm::Dictionary::ready() const
{
	return initialized;
}

bool pm::Dictionary::readying() const
{
	return initializing;
}

/**
 * @fn	Dictionary::Dictionary(const char* filename)
 *
 * @brief	Constructor
 *
 * @author	Daniel Allen
 * @date	03 Jul 2021
 */
pm::Dictionary::Dictionary(const char* filename)
{
	this->filename = filename;
}
/**
 * @fn	static bool Evaluator::init()
 *
 * @brief	Initializes the dictionary
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @returns	return true if successful or is already initialized, false if it
 * fails.
 */
bool pm::Dictionary::init()
{
	if (initializing) return true;
	if (initialized) return true;
	initializing = true;
	LOG("Loading dictionary");
	// read from file
	QFile file(filename);
	if (!file.exists())
		return false;
	if (file.open(QIODevice::ReadOnly)) {
		while (!file.atEnd()) {
			QString line = file.readLine();
			line = line.left(line.length() - 2);
			//LOG("Word: \"" << line.toStdString() << "\"");
			if (!line.isEmpty())
				addWord(line);
		}
		file.close();
	}
	else {
		initializing = false;
		//return false;
	}
	initialized = true;
	initializing = false;
	LOG("Done loading dictionary");
	return true;
}
/**
 * @fn	static std::future<bool> Evaluator::asyncInit()
 *
 * @brief	Initializes the dictionary asynchronously
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @returns	An std::future<bool>: true if successful or is already
 * initialized, false if it fails.
 */
void pm::Dictionary::asyncInit()
{
	std::thread t(&Dictionary::init, this);
	t.detach();
}
/**
 * @fn	static double Evaluator::passwordStrength(std::string password)
 *
 * @brief	Calculates the entropy of a string
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	password	The password.
 *
 * @returns	A double.
 */
double pm::Dictionary::passwordStrength(QString password)
{
	if (!initialized)
	{
		if (!initializing)
			init();
		else
		{
			while (initializing)
			{
			}
		}
	}
	double entropy = 0;
	std::vector<QString> segments;
	for (int size = password.length(); size >= 2; size--) {
		for (int i = 0; i < password.length() - size; i++) {
			QString segment = password.mid(i, size + 1);
			if (contains(segment) || containsReverse(segment)) {
				entropy++;
				password = password.replace(i, size + 1, "");
				segments.push_back(segment);
				size = password.length();
				break;
			}
		}
	}
	segments.push_back(password);

	entropy += password.length();
	return entropy;
}
/**
 * @fn	static int Evaluator::run(const std::string& string)
 *
 * @brief	finds the run length of a pattern in a string
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	string	The string.
 *
 * @returns	An int representing the length of the first run in this string.
 */
int pm::Dictionary::run(const QString & string)
{
	int run = 1;
	const QChar lastChar = string[0];
	int dir = -2;
	for (int i = 1; i < string.length(); i++, run++) {
		if (string[i].isDigit() && lastChar.isDigit()) {
			if (string.length() > i + 1 && string[i + 1].isDigit()) {
				if (string[i + 1] == lastChar) {
					if (run != 1 && dir != 0) return run;
					dir = 0;
				}
				else if (string[i + 1] < lastChar) {
					if (run != 1 && dir != 1) return run;
					dir = 1;
				}
				else if (string[i + 1] > lastChar) {
					if (run != 1 && dir != -1) return run;
					dir = -1;
				}
			}
		}
		// is letter run
		else if (string[i].isLetter() && lastChar.isLetter()) {
		}
		// is no longer a run
		else {
			return run;
		}
	}
	return run;
}
//TODO move this to Utils
/**
 * @fn	static int Evaluator::charToInt(const char character)
 *
 * @brief	Converts a character to an integer
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	character	The character.
 *
 * @returns	An int with the value of the character, or -1 if it is not a
 * digit.
 */
int pm::Dictionary::charToInt(const char character)
{
	switch (character) {
	case '0':
		return 0;
	case '1':
		return 1;
	case '2':
		return 2;
	case '3':
		return 3;
	case '4':
		return 4;
	case '5':
		return 5;
	case '6':
		return 6;
	case '7':
		return 7;
	case '8':
		return 8;
	case '9':
		return 9;
	default:
		return -1;
	}
}
/**
 * @fn	static bool Evaluator::mixedCase(const std::string& word)
 *
 * @brief	Checks if the input contains both uppercase and lowercase
 * letters
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	word	The input to check.
 *
 * @returns	True if it contains both uppercase and lowercase letters.
 */
bool pm::Dictionary::mixedCase(const QString & word)
{
	bool hasUpper = false;
	bool hasLower = false;
	if (word.isSimpleText())
		for (const QChar i : word) {
			if (i.isUpper()) {
				hasUpper = true;
			}
			else if (i.isLower()) {
				hasLower = true;
			}
			if (hasUpper && hasLower) return true;
		}
	return false;
}
/**
 * @fn	static bool Evaluator::alphanumeric(const std::string& input)
 *
 * @brief	Checks if the input contains both letters and numbers
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	input	The input to check.
 *
 * @returns	True if it contains both letters and numbers.
 */
bool pm::Dictionary::alphanumeric(const std::string & input)
{
	if (input.length() < 2) return false;
	bool hasAlpha = false;
	bool hasNum = false;
	for (char i : input) {
		if (isdigit(i)) {
			hasNum = true;
		}
		else if (isalpha(i)) {
			hasAlpha = true;
		}
		if (hasAlpha && hasNum) return true;
	}
	return false;
}
/**
 * @fn	static std::string Evaluator::reverse(const std::string& input)
 *
 * @brief	reverse a string
 *
 * @author	Daniel Allen
 * @date	02 Jul 2021
 *
 * @param 	input	The input to reverse.
 *
 * @returns	An std::string equal to the input with each character's indices
 * flipped.
 */
QString pm::Dictionary::reverse(QString input)
{
	std::reverse(input.begin(), input.end());
	return input;
}