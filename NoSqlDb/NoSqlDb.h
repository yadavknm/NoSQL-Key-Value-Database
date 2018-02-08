////////////////////////////////////////////////////////////////
 // NoSqlDb.h - Key/Value in memory database                   //
 // ver 1.0                                                    //
 // Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017 //
 ////////////////////////////////////////////////////////////////
 /*
 * Package Operations:
 * -------------------
 * This package contains an Element<T> class and a NoSqlDb<T> class.
 * Its purpose is to support add,delete and update elements into NoSqlDb.
 * It is implemented using C++11 STL unordered_map
 *
 * Required Files:
 * ---------------
 * XmlDocument.h, XmlElement.h, StrHelper.h, Convert.h
 *
 * Build Process:
 * --------------
 * devenv Project1.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 13 Feb 2017
 * - first release
 *
 */
#pragma once
#pragma warning(disable : 4996) 

#include <unordered_map>					// including the necessary inbuilt header files
#include <string>
#include <sstream>
#include <vector>
#include <iomanip>
#include <iostream>
#include "../CppProperties/CppProperties.h"	// including the necessary userdefined header files
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h" 
#include "../XmlDocument/XmlParser/XmlParser.h"
#include "../StrHelper.h"
#include "../Convert/Convert.h"

using namespace XmlProcessing;
/////////////////////////////////////////////////////////////////////
// Element class represents a data record in our NoSql database
// - in our NoSql database that is just the value in a key/value pair
// - it also stores the child data as a vector
/////////////////////////////////////////////////////////////////////
template<typename Data>
class Element
{
public:
	using Name = std::string;
	using Category = std::string;
	using Descr = std::string;
	using TimeDate = std::string;

	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Descr> description;	  // metadata
	Property<Data> data;            // data
	std::vector<std::string> children_; //metadata

	std::string show();					// function to display Element's contents
	std::string getTimeDate();			// function to get the current Date and Time
	void addChild(const std::string& key);//function to add children to an element
	void deleteChild();					//function to delete an element
	std::vector<std::string>& getChildren();//function to retrieve the children of an element
};

//----< Element<T>::show() function >---------------------------------------------
/*
* Function to display the contents of an element
*/
template<typename Data>
std::string Element<Data>::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name; // displaying each metadata
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "description" << " : " << description;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	if (children_.size() >= 0)		 // displaying the children of the element
	{
		out << "\n    children : ";
		for (std::string thisKey : children_)
		{
			out << thisKey << ", ";
		}
	}
	out << "\n";
	return out.str();
}


//----< Element<T>::getTimeDate() function >---------------------------------------------
/*
* Function to obtain the current Time and Date
*/
template<typename Data>
std::string Element<Data>::getTimeDate()
{
	time_t now = time(0);
	char * dt = ctime(&now);
	std::string timeStr = dt;
	return trim(timeStr);
}

//----< Element<T>::addChild() function >---------------------------------------------
/*
* Function to addChildren to an Element of a specified Key
*/
template<typename Data>
void Element<Data>::addChild(const std::string& key)
{
	children_.push_back(key);
}

//----< Element<T>::deleteChild() function >---------------------------------------------
/*
* Function to delete the children from an Element of a specified Key
*/
template<typename Data>
void Element<Data>::deleteChild()
{
	children_.clear();
}

//----< Element<T>::getChildren() function >---------------------------------------------
/*
* Function to obtain the children of an Element 
*/
template<typename Data>
std::vector<std::string>& Element<Data>::getChildren()
{
	return children_;
}

/////////////////////////////////////////////////////////////////////
// NoSqlDb class is a key/value pair in-memory database
// - stores and retrieves elements
// - implements toXml and fromXml functions
/////////////////////////////////////////////////////////////////////
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	using Sptr = std::shared_ptr<AbstractXmlElement>;

	const int saveAfterCounts = 5;			// counters for autosave, after a certain number of writes
	int writeCounter = 0;
	Keys keys();								//function that stores vector of keys	
	bool save(Key key, Element<Data> elem);	//function to add the element to the database and save it
	void resave(Key key, Element<Data> elem);	// funvoction to resave the database after a modification to an element
	void deleteElem(Key key, Element<Data> elem);									// function to delete an element from the database
	void editText(Key key, Element<Data> elem, std::string newText);				//function to edit the description of an element
	void replaceInstance(Key key, Element<Data> elem, Element<Data> newElem);		// function to replace an existing value's instance
	Element<Data> value(Key key);				// function to return the value of a specified key
	size_t count();							// returns the size of the database
	std::string toXml();						// funcion to persist database contents to an XML file
	void fromXml(const std::string& xml);	//function to restore the database contents from an existing XML file
//	void fromXmlFile(NoSqlDb<Data>& db, std::string file);
private:
	using Item = std::pair<Key, Element<Data>>;
	std::unordered_map<Key, Element<Data>> store;
};

//----< NoSqlDb<T>::keys() function >---------------------------------------------
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first); // storing the keys in a vector 
	}
	return keys;
}

//----< NoSqlDb<T>::save() function >---------------------------------------------
template<typename Data>
bool NoSqlDb<Data>::save(Key key, Element<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	writeCounter++;
	// if writeCounter > saveAfterCounts, persist the database contents
	return true;
}

//----< NoSqlDb<T>::resave() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::resave(Key key, Element<Data> elem)
{
	store[key] = elem;
	writeCounter++;
	// if writeCounter > saveAfterCounts, persist
}

//----< NoSqlDb<T>::deleteElem() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::deleteElem(Key key, Element<Data> elem)
{
	// delete logic
	store.erase(key);
}

//----< NoSqlDb<T>::editText() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::editText(Key key, Element<Data> elem, std::string newText)
{
	elem.description = newText;
	store[key] = elem;
}

//----< NoSqlDb<T>::replaceInstance() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::replaceInstance(Key key, Element<Data> elem, Element<Data> newElem)
{

	elem.data = newElem.data;
	store[key] = elem;
}

//----< NoSqlDb<T>::value() function >---------------------------------------------
template<typename Data>
Element<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return Element<Data>();
}

//----< NoSqlDb<T>::count() function >---------------------------------------------
template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}


/////////////////////////////////////////////////////////////////////
// Persistance capability through toXML() and fromXml() functions
////////////////////////////////////////////////////////////////////

//----< NoSqlDb<T>::toXml() function >---------------------------------------------
template<typename Data>
std::string NoSqlDb<Data>::toXml()
{
	std::vector<Key> keyvect = keys();
	XmlDocument doc;
	using Sptr = std::shared_ptr<AbstractXmlElement>; //shared pointer to access the tags of XML
	Sptr pRoot = makeTaggedElement("DB");
	doc.docElement() = pRoot;
	for (Key key : keyvect)
	{
		Element<Data> elem = value(key);
		Sptr pElem = makeTaggedElement("ELEMENT");	//creating tagged Elements
		pRoot->addChild(pElem);
		Sptr pElemName = makeTaggedElement("NAME");
		pElem->addChild(pElemName);
		Sptr pTextElemName = makeTextElement(elem.name);	// creating text elements
		pElemName->addChild(pTextElemName);
		Sptr pElemCategory = makeTaggedElement("CATEGORY");
		pElem->addChild(pElemCategory);
		Sptr pTextElemCategory = makeTextElement(elem.category);
		pElemCategory->addChild(pTextElemCategory);
		Sptr pElemTimeDate = makeTaggedElement("TIME_DATE");
		pElem->addChild(pElemTimeDate);
		Sptr pTextElemTimeDate = makeTextElement(elem.timeDate);
		pElemTimeDate->addChild(pTextElemTimeDate);
		Sptr pElemDescription = makeTaggedElement("DESCRIPTION");
		pElem->addChild(pElemDescription);
		Sptr pTextElemDescription = makeTextElement(elem.description);
		pElemDescription->addChild(pTextElemDescription);
		Sptr pElemData = makeTaggedElement("DATA");
		pElem->addChild(pElemData);
		Sptr pTextElemData = makeTextElement(elem.data);
		pElemData->addChild(pTextElemData);
		Sptr pElemChildren = makeTaggedElement("CHILDREN");
		pElem->addChild(pElemChildren);
		if (elem.children_.size() > 0) // iterating the children of an Element
		{
			for (std::string thisKey : elem.children_)
			{
				Sptr pElemChildrenVect = makeTaggedElement("CHILD");
				pElemChildren->addChild(pElemChildrenVect);
				Sptr pTextElemChildrenVect = makeTextElement(thisKey);
				pElemChildrenVect->addChild(pTextElemChildrenVect);
			}
		}
	}
	std::string xml = doc.toString();
	return xml;
}




//----< NoSqlDb<T>::fromXml() function >---------------------------------------------
template<typename Data>
void NoSqlDb<Data>::fromXml(const std::string & xml)
{
	
	try {
		XmlDocument doc(xml, XmlDocument::str);
		std::vector<Sptr> desc = doc.descendents("ELEMENT").select();	// accessing the data from XML, to store into Elements of Database
		for (Sptr descPtr : desc)
		{
			Element<Data> elem;
			std::string name_value = descPtr->children()[0]->children()[0]->value();	
			elem.name = trim(name_value);
			std::string category_value = descPtr->children()[1]->children()[0]->value();
			elem.category = trim(category_value);
			std::string timeDate_value = descPtr->children()[2]->children()[0]->value();
			elem.timeDate = trim(timeDate_value);
			std::string description_value = descPtr->children()[3]->children()[0]->value();
			elem.description = trim(description_value);
			std::string data_value = descPtr->children()[4]->children()[0]->value();
			elem.data = trim(data_value);
			std::vector<Sptr> child = descPtr->children()[5]->children();
			for (Sptr childPtr : child)									//iterating to obtain children information
			{
				std::string child_value = childPtr->children()[0]->value();
				std::string child_value_trim = trim(child_value);
				elem.children_.push_back(child_value_trim);
			}
			save(elem.name, elem);	//saving and augmenting the database
		}
	}
	catch (std::exception& ex)
	{
		std::cout << "\n something bad happend";
		std::cout << "\n " << ex.what();
	}
}
