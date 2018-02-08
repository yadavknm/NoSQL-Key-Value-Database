//////////////////////////////////////////////////////////////////
// NoSqlDb.cpp - Implementation of Key/Value in memory database //
// ver 1.0                                                      //
// Yadav Murthy, CSE687 - Object Oriented Design, Spring 2017   //
//////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package contains implementations of functions from Element<T> class and  NoSqlDb<T> class.
* Its purpose is to support add,delete and update elements into NoSqlDb.
* It is implemented using C++11 STL unordered_map
*
* Required Files:
* ---------------
* NoSqldb.h, Query.h
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



#include "NoSqlDb.h"	// including the necessary header files
#include "Query.h"
#include <iostream>
#include <set>

using StrData = std::string;
using intData = int;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

int main()
{
  std::cout << "\n  Demonstrating NoSql Key Value Database";
  std::cout << "\n =========================================\n";
  title("\n REQUIREMENT 1 - Implemented in C++ using the facilities of the standard C++ Libraries and Visual Studio 2015");

  title("\n REQUIREMENT 3 - Addition of key/value pairs");

  // creating Database object and the Elements to store in it
  NoSqlDb<StrData> db;

  Element<StrData> elem1;
  elem1.name = "elem1";
  elem1.category = "test";
  elem1.description = "elem1's description";
  elem1.data = "elem1's StrData";
  elem1.timeDate = elem1.getTimeDate();

  db.save(elem1.name, elem1);

  Element<StrData> elem2;
  elem2.name = "elem2";
  elem2.category = "test";
  elem2.description = "elem2's description";
  elem2.data = "elem2's StrData";
  elem2.timeDate = elem2.getTimeDate();

  db.save(elem2.name, elem2);

  Element<StrData> elem3;
  elem3.name = "elem3";
  elem3.category = "test";
  elem3.description = "elem3's description";
  elem3.data = "elem3's StrData";
  elem3.timeDate = elem3.getTimeDate();

  db.save(elem3.name, elem3);

  //adding children to the elements 
  elem2.addChild(elem3.name);
  elem1.addChild(elem2.name);
  elem1.addChild(elem3.name);
  elem3.addChild(elem2.name);

  //resaving the contents of Elements
  db.resave(elem2.name, elem2);
  db.resave(elem1.name, elem1);
  db.resave(elem3.name, elem3);

  std::cout << "\n  Retrieving the added elements from NoSqlDb<string>";
  std::cout << "\n -----------------------------------------------------\n";

  std::cout << "\n  size of db = " << db.count() << "\n";
  
  //displaying the Element's metadata and data
  title("\n REQUIREMENT 2 - Implemented a template class consisting of metadata and data as shown below");
  Keys keys = db.keys();
  for (Key key : keys)
  {
    std::cout << "\n  " << key << ":";
    std::cout << db.value(key).show();
  }
  std::cout << "\n\n";


  title("\n REQUIREMENT 3 - Deletion of key/value pairs");
  db.deleteElem(elem3.name, elem3);							// deleting an element of a specified key
  std::cout << "\n  size of db = " << db.count() << "\n";

  title("\n REQUIREMENT 4 - Deletion of relationships");
  std::cout << "\n  Deleting children vector from Element";
  std::cout << "\n ------------------------------------------\n";
  elem2.deleteChild();										// deleyting the children from the Element
  db.resave(elem2.name,elem2);
  for (Key key : keys)
  {
	  std::cout << "\n  " << key << ":";
	  std::cout << db.value(key).show();
  }

  title("\n REQUIREMENT 4 - Editing text metadata");
  std::cout << "\n  Editing text description of Element";
  std::cout << "\n ------------------------------------------\n";
  db.editText(elem1.name, elem1, "NEW TEXT DESCRIPTION");	//editing the text of an Element
  elem1 = db.value("elem1");
  for (Key key : keys)
  {
	  std::cout << "\n  " << key << ":";
	  std::cout << db.value(key).show();
  }

  title("\n REQUIREMENT 4 - replacing an existing value's instance with a new instance");
  db.replaceInstance(elem1.name.getValue(), elem1, elem2);	//replacing the data of an Element
  elem1 = db.value("elem1");
  elem2 = db.value("elem2");
  for (Key key : keys)
  {
	  std::cout << "\n  " << key << ":";
	  std::cout << db.value(key).show();
  }

  title("\n REQUIREMENT 10 and 5 - Submitted with contents, in the form of an XML file, that describe your project's package structure and dependency relationships that can be loaded when your project is graded.\n REQUIREMENT 5 - Persisting database contents to an XML file");
  std::string xml = db.toXml();		//converting the database contents into XML format
  std::cout<<"\n"<<db.toXml();
	
  XmlParser parser("../xml1.xml"); //parsing the contents of an XML file into the database
  XmlDocument* pXmlDoc = parser.buildDocument();
  std::cout << pXmlDoc->toString();

  

  title("\n REQUIREMENT 5 - Database restored from an existing XML file");
  db.fromXml(pXmlDoc->toString());	// augmenting the data from XML file
  keys = db.keys();
  for (Key key : keys)
  {
	  std::cout << "\n  " << key << ":";
	  std::cout << db.value(key).show();
  }

  title("\n REQUIREMENT 6 - Number of writes after which the database contents are persisted");
  if (db.writeCounter > db.saveAfterCounts)	// auto persist after a certain number of writes to database
  {
	  std::string timedXml = db.toXml();
	  std::cout << "\n" << db.toXml();
  }

  // demonstrating the queries
  Query<StrData> query;
  title("\n REQUIREMENT 7 - Supporting Query for the value of a specified key");
  title("Query1");
  Element<StrData> result1 = query.Query1(db, "elem1");	// queries the value of a specified key "elem1"
  std::cout << result1.show();							// displaying the result of the query
  title("\n REQUIREMENT 7 - Supporting Query for the children of a specified key");
  title("Query2");
  Keys result2 = query.Query2(db, "elem1"); // querying the children of "elem1"
  query.showResult(result2);
  title("\n REQUIREMENT 7 - Supporting Query for the set of all keys matching a specified pattern which defaults to all keys.");
  title("Query3");
  Keys result3 = query.Query3(db, "elem");	// returns the keys with a pattern "elem"
  query.showResult(result3);
  title("\n REQUIREMENT 7 - Supporting Query for all keys that contain a specified string in their item name");
  title("Query4");
  Keys result4 = query.Query4(db, "elem");	// returns the keys that contain the pattern "elem" in the item metadata of element
  query.showResult(result4);
  title("\n REQUIREMENT 7 - Supporting Query for all keys that contain a specified string in their category name");
  title("Query5");
  Keys result5 = query.Query5(db, "te");	// returns the keys that contain the pattern "te" in the category metadata of element
  query.showResult(result5);
  title("\n REQUIREMENT 7 - Supporting Query for all keys that contain a specified string in their template data section when that makes sense");
  title("Query6");
  Keys result6 = query.Query6(db, "Data");	// returns the keys that contain the pattern "Data" in the data of element
  query.showResult(result6);

  title("\n REQUIREMENT 8 - Support making the same kinds of queries, on the set of keys returned by an earlier query");
  title("Query4 on result of Query4");
  Keys result8 = query.queryOnAQuery(db, "em2", result4); // supporting query on the result of query4
  query.showResult(result8);

  title("\n REQUIREMENT 9 - Support forming a union of the keys from two or more earlier queries.");
  title("Query4 on union of keys obtained from query2 and query3");
  std::set<Key> set1(result2.begin(), result2.end()); // result of query2
  std::set<Key> set2(result3.begin(), result3.end()); // result of query3
  Keys unionOfQueryResults;
  std::set_union(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(unionOfQueryResults)); // union of result2 and result3
  Keys result9 = query.queryUnion(db, "lem", unionOfQueryResults); // performing a query on the union of results
  query.showResult(result9);

  title("\n REQUIREMENT 11 - Accompanied by a test executive (NoSqlDb.cpp) that clearly demonstrates you've met all the functional requirements #2-#9");
  std::cin.get();
}
