# Operating System Project API
>## Table of content
>>- [print(String/Number message)](#print)
>
>>- [clear()](#clear)
>
>>- [createDisk(String path, Number size)](#createdisk)
>
>>- [loadDisk(String path)](#loaddisk)
>
>>- [listDisks()](#listdisks)
>
>>- [selectDisk(Number index)](#selectdisk)
>
>>- [mount()](#mount)
>
>>- [unmount()](#unmount)   


---
<br>

>## print
>> ### Signature
>> ```c++  
>>void print(string message);
>>void print(int number);
>>```
>
>>### Note
>> * Prints a number or message to the default output
>
>>### Argument(s)
>>* *message*: The message which should be displayed
>>* *number*: This is a number which should be displayed
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>print("Foo");
>>print(3);
>>```
>[Table of content](#Table-of-content)

 



>## clear
>> ### Signature
>> ```c++  
>>void clear(void);
>>```
>
>>### Note
>>* The cls command is equivalent to the clear command
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>print("foo");
>>clear();
>>print("bar");
>>cls();
>>```
>[Table of content](#Table-of-content)



>## createDisk
>> ### Signature
>> ```c++  
>>void createDisk(string path, int size);
>>```
>
>>### Note
>>* Create a new disk with given *size* at location which is specified by *path*
>
>>### Argument(s)
>>* *path*: This is the fully qualified name to the disk location
>>* *size*: Total size in bytes
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- Create a new empty disk located under the /data/operating_system directory with 5MB as size
>>createDisk("/data/operating_systems/myDisk5MB.vdf", 5242880);
>>-- Create a new empty disk located under th /data/operatin_system directory with 5GB as size
>>createDisk("/data/operating_systems/myDisk5GB.vdf", 5368709120);
>>```
>[Table of content](#Table-of-content)



>## loadDisk
>> ### Signature
>> ```c++  
>>void loadDisk(string path);
>>```
>
>>### Note
>>* This function loads a virtual disk from the drive
>
>>### Argument(s)
>>* *path*: The fully qualified name of the disk file
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- Loads a virtual disk from /data/operating_system directory
>>loadDisk("/data/operating_system/myDisk5MB.vdf");
>>```
>[Table of content](#Table-of-content)



>## listDisks
>> ### Signature
>> ```c++  
>>void listDisks(void);
>>```
>
>>### Note
>>* This function lists all available disks 
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- List all loaded disks, including previous created ones
>>listDisks();
>>```
>[Table of content](#Table-of-content)



>## selectDisk
>> ### Signature
>> ```c++  
>>void selectDisk(int index);
>>```
>
>>### Note
>>* This function selects a disk from the list as current working disk
>
>>### Argument(s)
>>* *index*: Index of the disk, use *listDisks* to receive the index of each disk
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua
>>-- Load some disks 
>>loadDisk("/data/operating_systems/myDisk5MB.vdf");
>>loadDisk("/data/operating_systems/myDisk5GB.vdf");
>>-- Receive the indices of all loaded disks
>>listDisks();
>>-- Select disk 0, which should be /data/operating_systems/myDisk5MB.vdf
>>selectDisk(0);
>>```
>[Table of content](#Table-of-content)



>## mount
>> ### Signature
>> ```c++  
>>void mount(void);
>>```
>
>>### Note
>>* This function mounts the current working disk into memory
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- Select disk 0, which should be /data/operating_systems/myDisk5MB.vdf
>>>selectDisk(0);
>>-- mount the previous selected disk
>>mount();
>>```
>[Table of content](#Table-of-content)



>## unmount
>> ### Signature
>> ```c++  
>>void unmount(void);
>>```
>
>>### Note
>>* This function unmounts the current working disk from memory
>>> When the application quits all disks will be unmounted regardless of an api call
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>-- Select disk 0, which should be /data/operating_systems/myDisk5MB.vdf
>>>selectDisk(0);
>>-- mount the previous selected disk
>>mount();
>>-- do something
>>-- Finished and now unmount the disk to release the used resources
>>unmount();
>>```
>[Table of content](#Table-of-content)


>## PH
>> ```c++  
>>void ph(void);
>>```
>
>>### Note
>>* This function
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>```
>[Table of content](#Table-of-content)


>## PH
>> ```c++  
>>void ph(void);
>>```
>
>>### Note
>>* This function
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>```
>[Table of content](#Table-of-content)


>## PH
>> ```c++  
>>void ph(void);
>>```
>
>>### Note
>>* This function
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>```
>[Table of content](#Table-of-content)


>## PH
>> ```c++  
>>void ph(void);
>>```
>
>>### Note
>>* This function
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>```
>[Table of content](#Table-of-content)


>## PH
>> ```c++  
>>void ph(void);
>>```
>
>>### Note
>>* This function
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>
>>```
>[Table of content](#Table-of-content)
