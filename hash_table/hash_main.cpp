/* This is a simple .cpp file to show you how to use the hash_table.h
 
 The data type is CHashTable<type>. 

 Use the function
   bool AddKey(std::string key, T* member)
 to add keys. Note the second parameter is an ADDRESS.

 To get an entry use 
     T* GetMember(const std::string& key)
 Note that it returns a POINTER to your data, not the data itself.

 The rest should be pretty straightforward. Check out hash_table.h
 for more documentation and the list of functions available.
 
 "hash_table.h" is the only file you need to include to use
 the hash table, BUT you need to HAVE all three header files:
 "hash_fun.h", "hash_table.h", and "Stl.h"
 in the folder I included for it to work.

 These files will compile and run with Visual Studio 2k5.

 - Daniel 
 johndleon@gmail.com
 281-814-5482
*/


#include "include\hash_table.h"
#include <iostream>

#define NUM_WORDS 3

void main()
{
  std::string keys[NUM_WORDS]    = {"keys", "to", "accessentries"};
  std::string entries[NUM_WORDS] = {"these", "words", "areentries"};

  /* fill the hash table */
  CHashTable<std::string> MyHashTable;
  for(int i = 0; i < NUM_WORDS; i++) MyHashTable.AddKey(keys[i],
                             &entries[i]);

  /* get a member */
  std::string  key     = keys[1];
  std::string*        pMember = MyHashTable.GetMember(key);
  std::cout << "Entry for key " << key << " is " << *pMember << std::endl;


  /* clean up the hash (do not free memory) */
  MyHashTable.RemoveAllKey(false);
}
