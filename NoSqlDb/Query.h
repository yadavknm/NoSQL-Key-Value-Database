////////////////////////////////////////////////////////////////
// Query.h - Supports Quries on the database values           //
// ver 1.0                                                    //
// Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017 //
////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains a Query<T> class
* Its purpose is to support various queries on the values present in the database
*
* Required Files:
* ---------------
* NoSqlDb.h
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
#include "NoSqlDb.h"
#include <regex>
using Keys = std::vector<std::string>;
template<typename Data>
class Query {
public:
	Element<Data> Query1(NoSqlDb<Data>& db, std::string specifiedKey);	// Query1 for the value of a specified key
	std::vector<std::string> Query2(NoSqlDb<Data>& db, std::string specifiedKey); // Query2 for children of a specified key
	std::vector<std::string> Query3(NoSqlDb<Data>& db, std::string specifiedString);// Query3 for set of all keys matching a specified pattern which defaults to all keys
	std::vector<std::string> Query4(NoSqlDb<Data>& db, std::string specifiedString);// Query4 for keys that contain a specified string in their item name
	std::vector<std::string> Query5(NoSqlDb<Data>& db, std::string specifiedString);//Query5 for keys that contain a specified string in their category name
	std::vector<std::string> Query6(NoSqlDb<Data>& db, std::string specifiedString);// Query6 for keys that contain a specified string in their template data section when that makes sense
    void showResult(Keys); // function to show the results of Quries
	Keys queryOnAQuery(NoSqlDb<Data>& db, std::string specifiedString, Keys initialResult);//Function to support making the same kinds of queries, on the set of keys returned by an earlier query
	Keys queryUnion(NoSqlDb<Data>& db, std::string specifiedString, Keys unionOfTwoQueries);//Function to perform union of the keys from two or more earlier queries
};

//----< Query<T>::Query1() function >---------------------------------------------
template<typename Data>
Element<Data> Query<Data>::Query1(NoSqlDb<Data>& db,std::string specifiedKey)
{
	Element<Data> returnedValue = db.value(specifiedKey);
	return returnedValue;
}

//----< Query<T>::Query2() function >---------------------------------------------
template<typename Data>
std::vector<std::string> Query<Data>::Query2(NoSqlDb<Data>& db, std::string specifiedKey)
{
	return db.value(specifiedKey).children_;
}

//----< Query<T>::Query3() function >---------------------------------------------
template<typename Data>
std::vector<std::string> Query<Data>::Query3(NoSqlDb<Data>& db, std::string specifiedString)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat; 
	std::string pattern = o.str(); //(.*)ke(.*)

	for (Key key : db.keys())
	{
		if (std::regex_match(key, std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}	
	return result;
}

//----< Query<T>::Query4() function >---------------------------------------------
template<typename Data>
std::vector<std::string> Query<Data>::Query4(NoSqlDb<Data>& db, std::string specifiedString)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat;
	std::string pattern = o.str(); //(.*)ke(.*)

	for (Key key : db.keys())
	{
		Element<Data> val = db.value(key);
		
		if (std::regex_match(val.name.getValue(), std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}
	return result;
}

//----< Query<T>::Query5() function >---------------------------------------------
template<typename Data>
std::vector<std::string> Query<Data>::Query5(NoSqlDb<Data>& db, std::string specifiedString)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat;
	std::string pattern = o.str();

	for (Key key : db.keys())
	{
		Element<Data> val = db.value(key);

		if (std::regex_match(val.category.getValue(), std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}
	return result;
}

//----< Query<T>::Query6() function >---------------------------------------------
template<typename Data>
std::vector<std::string> Query<Data>::Query6(NoSqlDb<Data>& db, std::string specifiedString)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat;
	std::string pattern = o.str();

	for (Key key : db.keys())
	{
		Element<Data> val = db.value(key);

		if (std::regex_match(val.data.getValue(), std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}
	return result;
}

//----< Query<T>::show() function >---------------------------------------------
template<typename Data>
void Query<Data>::showResult(Keys result)
{
	for (std::string key : result)
	{
		std::cout << "\n  " << key << ":";
	}
}

//----< Query<T>::queryOnAQuery() function >---------------------------------------------
template<typename Data>
Keys Query<Data>::queryOnAQuery(NoSqlDb<Data>& db, std::string specifiedString, Keys initialResult)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat;
	std::string pattern = o.str(); //(.*)ke(.*)

	for (Key key : initialResult)
	{
		Element<Data> val = db.value(key);

		if (std::regex_match(val.name.getValue(), std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}
	return result;
}

//----< Query<T>::queryUnion() function >---------------------------------------------
template<typename Data>
Keys Query<Data>::queryUnion(NoSqlDb<Data>& db, std::string specifiedString, Keys unionOfTwoQueries)
{
	Keys result;
	std::ostringstream o;
	std::string pat = "(.*)";
	o << pat << specifiedString << pat;
	std::string pattern = o.str(); //(.*)ke(.*)

	for (Key key : unionOfTwoQueries)
	{
		Element<Data> val = db.value(key);

		if (std::regex_match(val.name.getValue(), std::regex(pattern.c_str()), std::regex_constants::match_default))
			result.push_back(key);
	}
	return result;
}
