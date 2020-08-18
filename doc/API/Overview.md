# Operating System Project API
>## Table of content
>>- [print(String/Number message)](#print)
>
>>- [clear()](#clear)
>
>>- [createDisk(String path, Integer size)](#createdisk)
>
>>- [loadDisk(String path)](#loaddisk)
>
>>- [selectDisk(Integer index)](#selectdisk)
>
>>- [mount()](#mount)
>
>>- [unmount()](#unmount)   
>
>>- [createMasterBootRecord()](#createMasterBootRecord)
>
>>- [listMasterBootRecord()](#listMasterBootRecord)
>
>>- [wipeMasterBootRecord()](#wipeMasterBootRecord)
>
>>- [createPartition(Integer size, Character primary, Integer file_system)](#createPartition)
>
>>- [deletePartition()](#deletePartition)
>
>>- [wipePartition()](#wipePartition)
>
>>- [Operation Result Codes](#Operation-Result-Codes)

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
>>-- Create a new empty disk located under th /data/operating_system directory with 5GB as size
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


>## createMasterBootRecord
>> ```c++  
>>void createMasterBootRecord(void);
>>```
>
>>### Note
>>* This function create a master boot record on previous selected and mounted disk
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- Load, select and mount a disk
>>loadDisk("/data/operating_system/myDisk5GB.vdf");
>>selectDisk(0);
>>mount();
>>-- Create the master boot record entry
>>createMasterBootRecord();
>>```
>[Table of content](#Table-of-content)

createMasterBootRecord
>## listMasterBootRecord
>> ```c++  
>>void listMasterBootRecord(void);
>>```
>
>>### Note
>>* This function lists meta information about the disk.
>>    * Size of the disk
>>    * Path of the virtual disk
>>    * Mounting state
>>    * Existing partitions, including their meta information
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>listMasterBootRecord();
>>```
>[Table of content](#Table-of-content)


>## wipeMasterBootRecord
>> ```c++  
>>void wipeMasterBootRecord(void);
>>```
>
>>### Note
>>* This function wipes the master boot record.
>>> **Important calling this method will made the entire saved data unrecoverable**
>
>>### Argument(s)
>>* *None*
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>wipeMasterBootRecord();
>>```
>[Table of content](#Table-of-content)


>## createPartition
>> ```c++  
>>void createPartition(int size, char primary, int file_system);
>>```
>
>>### Note
>>* This function creates a partition
>
>>### Argument(s)
>>* *size*: Size of the partition in bytes, the size must be less or equal the total available free disk space
>>* *primary*: If its a primary partition the argument should be *'y'* or *'Y'* otherwise *'n'* or *'N'*
>>* *file_system*: Enter a number from the list below
>>
>>>
>>> Supported file system types 
>>>Filesystem | file_system argument 
>>>    ---    | ---
>>>None       |  0
>>>INode      | 16
>>>FAT        | 32
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>-- Load, select and mount a disk
>>loadDisk("/data/operating_system/myDisk5GB.vdf");
>>selectDisk(0);
>>mount();
>>-- Create the master boot record entry
>>createMasterBootRecord();
>>-- Create a primary partition with INode and a size of 2.5GB
>>createPartition(2684354560, 'y', 16);
>>```
>[Table of content](#Table-of-content)


>## deletePartition
>> ```c++  
>>void deletePartition(int index);
>>```
>
>>### Note
>>* This function delete a partition from the disk.
>>  The deletion will not be secure
>
>>### Argument(s)
>>* *index*: Index of the partition, in range from 0 to 3
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>loadDisk("/data/operating_system/myDisk5GB.vdf");
>>selectDisk(0);
>>mount();
>>createMasterBootRecord();
>>createPartition(2684354560, 'y', 16);
>>deletePartition(0);
>>```
>[Table of content](#Table-of-content)


>## wipePartition
>> ```c++  
>>void wipePartition(int index);
>>```
>
>>### Note
>>* This function wipes a partition from the disk.
>>  The deletion will override the partition with 0, its will be secure.
>
>>### Argument(s)
>>* *index*: Index of the partition, in range from 0 to 3
>
>>### Returns
>>* *Nothing*
>
>>### Example
>>```Lua 
>>loadDisk("/data/operating_system/myDisk5GB.vdf");
>>selectDisk(0);
>>mount();
>>createMasterBootRecord();
>>createPartition(2684354560, 'y', 16);
>>wipePartition(0);
>>```
>[Table of content](#Table-of-content)


>## Operation Result Codes
>>
>>### General
>>* 0x00000000: Operation succeed 
>>* 0xFFFFFFFF: Unknown Error 
>>* 0x00005555: Index out of bounds
>
>>### File IO
>>* 0x01F10404: File Not Found
>>* 0x01F1FF04: File open failed
>>* 0x01F10BAD: File version is incompatible
>
>>### Memory Management
>>* 0x01AADEAD: Disk file could not be mapped to memory
>>* 0x01AADEFF: Disk could not be stated 
>
>>### Disk
>>* 0x01D10500: Disk already loaded
>>* 0x01D10002: Requested disk size is negative
>>* 0x01D1FFF0: Requested disk size is to big
>>* 0x01D10010: Disk is not mounted
>>* 0x01D10011: Disk is mounted
>
>>### Master Boot Record
>>* 0x01C10404: Master boot record not found
>>* 0x01C1FFF0: Master boot record capacity reached
>>* 
>
>>### Partition
>>* 0x01B10002: Partition size is negative
>>* 0x01B1FFF0: Partition size is to big
>>* 0x01B1AAAA: Requested partition already exists
>>* 0x01B1AABB: Partition already wiped
>
>>### File System
>>* 
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
>>```
>[Table of content](#Table-of-content)
