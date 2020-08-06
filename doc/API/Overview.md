# Operating System Project API
## Table of content
- [Operating System Project API](#operating-system-project-api)
  * [Table of content](#Table-of-content)
 
  * [print(String/Number message)](#print-string-number-message-)
    + [Argument(s)](#argument-s-), [Returns](#returns), [Example](#example)
 
  * [clear()](#clear--)
    + [Note](#note-1), [Argument(s)](#argument-s--1), [Returns](#returns-1), [Example](#example-1)
 
  * [createDisk(String path, Number size)](#createdisk-string-path--number-size-)
    + [Note](#note-2), [Argument(s)](#argument-s--2), [Returns](#returns-2), [Example](#example-2)
 
  * [loadDisk(String path)](#loaddisk-string-path-)
    + [Note](#note-3), [Argument(s)](#argument-s--3), [Returns](#returns-3), [Example](#example-3)
 
  * [listDisks()](#listdisks--)
    + [Note](#note-4), [Argument(s)](#argument-s--4), [Returns](#returns-4), [Example](#example-4)
 
  * [selectDisk(Number index)](#selectdisk-number-index-)
    + [Note](#note-5), [Argument(s)](#argument-s--5), [Returns](#returns-5), [Example](#example-5)
 
  * [mount()](#mount--)
    + [Note](#note-6), [Argument(s)](#argument-s--6), [Returns](#returns-6), [Example](#example-6)
 
  * [unmount()](#unmount--)
    + [Note](#note-7), [Argument(s)](#argument-s--7), [Returns](#returns-7), [Example](#example-7)
    
    <br>
---

>## print(String/Number message)
>### Note
>### Argument(s)
>* *message*: The message which should be displayed on the default output device
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>print("Foo");
>print(3);
>```
>[Table of content](#Table-of-content)

 



>## clear()
>### Note
>* The cls command is equivalent to the clear command
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>print("foo");
>clear();
>print("bar");
>cls();
>```
>[Table of content](#Table-of-content)



>## createDisk(String path, Number size)
>### Note
>* Create a new disk with given *size* at location which is specified by *path*
>
>### Arguments
>* *path*: This is the fully qualified name to the disk location
>* *size*: Total size in bytes
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>-- Create a new empty disk located under the /data/operating_system directory with 5MB as size
>createDisk("/data/operating_systems/myDisk5MB.vdf", 5242880);
>-- Create a new empty disk located under th /data/operatin_system directory with 5GB as size
>createDisk("/data/operating_systems/myDisk5GB.vdf", 5368709120);
>```
>[Table of content](#Table-of-content)



>## loadDisk(String path)
>### Note
>* This function loads a virtual disk from the drive
>
>### Argument(s)
>* *path*: The fully qualified name of the disk file
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>-- Loads a virtual disk from /data/operating_system directory
>loadDisk("/data/operating_system/myDisk5MB.vdf");
>```
>[Table of content](#Table-of-content)



>## listDisks()
>### Note
>* This function lists all available disks 
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>-- List all loaded disks, including previous created ones
>listDisks();
>```
>[Table of content](#Table-of-content)



>## selectDisk(Number index)
>### Note
>* This function selects a disk from the list as current working disk
>
>### Argument(s)
>* *index*: Index of the disk, use *listDisks* to receive the index of each disk
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua
>-- Load some disks 
>loadDisk("/data/operating_systems/myDisk5MB.vdf");
>loadDisk("/data/operating_systems/myDisk5GB.vdf");
>-- Receive the indices of all loaded disks
>listDisks();
>-- Select disk 0, which should be /data/operating_systems/myDisk5MB.vdf
>selectDisk(0);
>```
>[Table of content](#Table-of-content)



>## mount()
>### Note
>* This function mounts the current working disk into memory
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>-- Select disk 0, which should be /data/operating_systems/myDisk5MB.vdf
>selectDisk(0);
>-- mount the previous selected disk
>mount();
>```
>[Table of content](#Table-of-content)



>## unmount()
>### Note
>* This function unmounts the current working disk from memory
>> When the application quits all disks will be unmounted regardless of an api call
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>unmount();
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)



>## PH
>### Note
>* This function
>
>### Argument(s)
>* *None*
>
>### Returns
>* *Nothing*
>
>### Example
>```Lua 
>
>```
>[Table of content](#Table-of-content)
