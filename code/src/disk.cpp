#include "../include/disk.h"

const char* Disk::path(){
    return path_m;
}

const uint8_t* Disk::data(){
    if(nullptr == data_m){
        return nullptr;
    }
    return data_m;
}

MasterBootRecord* Disk::MBR(){
    return MBR_m;
}

uint32_t Disk::size() const{
    return size_m;
}

uint32_t Disk::available_size() const{
    return available_size_m;
}

bool Disk::isMounted() const{
    return mounted;
}


Disk::Disk(const char* path){
    MBR_m = nullptr;
    this->path_m = new char[strlen(path) + 1];
    memcpy(this->path_m, path, strlen(path) + 1);
}

Disk::~Disk(){
    unmount();
    delete path_m;
    delete MBR_m;
}


void Disk::createDisk(int64_t size){
    // Creates a new Virtual Disk File with zeros as content.
    // It will inform the user over a progressbar
   
    std::cout << "[create] Start disk creation..." << std::endl;
    if(size < 0){
        std::cerr << utils::COLOR_RED << "[create] A negative file size was entered: " << size << utils::COLOR_RESET << std::endl;
        last_error_m = REQUESTED_DISK_SIZE_NEGATIVE;
        return;
    }    
    if(size > MAX_DISK_SIZE){
        std::cerr << utils::COLOR_RED << "[create] File size of " << size << " exceeds the maximum limit of " << std::numeric_limits<uint32_t>::max()  << utils::COLOR_RESET << std::endl; 
        last_error_m = REQUESTED_DISK_SIZE_TO_BIG;
        return;
    }
    ProgressBar bar;
    // Perfomace decision. One step0x55ed33485740 is one percent of the size.
    uint32_t step = size / 100;
    bar.maximum(100);
    std::cout << utils::COLOR_CYAN << "[create] Using path: " << path_m << utils::COLOR_RESET << std::endl;
    std::cout << utils::COLOR_CYAN << "[create] Desired disk size: " << size << utils::COLOR_RESET << std::endl;

    // Open the virtual disk file
    std::cout << "[create] Open: [";
    auto fp = fopen(path_m, "wb");
    if(nullptr == fp){
        std::cout << utils::COLOR_RED << utils::ICON_DENIED << utils::COLOR_RESET << "]" << std::endl;
        last_error_m = MAPPING_FAILED;
        return;
    }else{
        std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;
    }

    // Write zero to it
    std::cout << "[create] Create virtual disk file" << std::endl;
    auto charval = static_cast<uint8_t>(0);
    for(uint32_t i = 0; i < size; i++){
        fwrite (&charval, 1, 1, fp);
        // update if i is a multiple of the step.
        if(i % step == 0){
            bar.update();
        }
    }
    char c0 = 0xC0;
    char version = CURRENT_VIRTUAL_DISK_FILE_VERSION;
    fwrite (&c0, 1, 1, fp);
    fwrite (&version, 1, 1, fp);   

    fclose(fp);
    std::cout << utils::COLOR_GREEN << "[create] Virtual disk file created under " << path_m << utils::COLOR_RESET << std::endl;
    available_size_m = size;
    last_error_m = OPERATION_SUCCEED;
}

void Disk::deleteDisk(){
//    utils::printModule("Disk", "DiskDeleter");

}

int Disk::lastError(){
    return last_error_m;
}


void Disk::mount(){
    // 1. When the disk is mounted the operation will fail
    // Next the virtual disk file will be opened and mapped to memory using mmap.
    //      - 1. Retreive the stats of the file, used for the size
    //      - 2. Open the file
    //      - 3. Map the file to an array using mmap
    // Finally load the MBR from the virtua disk
   

    if(mounted){
        std::cout << utils::COLOR_YELLOW << "[mount] Disk is mounted" << utils::COLOR_RESET << std::endl;
        last_error_m = DISK_MOUNTED;
        return;
    }else{
        std::cout << utils::COLOR_GREEN << "[mount] Start mounting ... " << std::endl;
    }
    fileStats_m = static_cast<struct stat*>(malloc(sizeof(struct stat)));

    std::cout << "[mount] Open VDF file: [";
    auto fd = open(path_m, O_RDWR, S_IRUSR | S_IWUSR);
    if(-1 == fd){
        std::cout << utils::COLOR_RED << utils::ICON_DENIED << utils::COLOR_RESET << "]" << std::endl;
        last_error_m = FILE_OPEN_FAILED;
        return;
    }else{
        std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;
    }

    std::cout << "[mount] Stat VDF: [";
    if(-1 == stat(path_m, fileStats_m)){
        std::cout << utils::COLOR_RED << utils::ICON_DENIED << utils::COLOR_RESET << "]" << std::endl;
        close(fd);
        last_error_m = STAT_FAILED;
        return;
    }else{
        std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;
    }

    std::cout << "[mount] Mapping VDF: [";
    data_m = static_cast<uint8_t*>(mmap(nullptr,  static_cast<size_t>(fileStats_m->st_size), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0));
    if(MAP_FAILED == data_m){
        std::cout << utils::COLOR_RED << utils::ICON_DENIED << utils::COLOR_RESET << "]" << std::endl;
        close(fd);
        delete(fileStats_m);
        fileStats_m = nullptr;
        last_error_m = MAPPING_FAILED;
        return;
    }else{
        std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;
    }

    close(fd);

    available_size_m = fileStats_m->st_size - 2;
    size_m = fileStats_m->st_size - 2;
    std::cout << available_size_m << std::endl;
    mounted = true;
    loadMasterBootRecord();

    std::cout << utils::COLOR_GREEN << utils::COLOR_BOLD << "[mount] Virtual Disk mounted" << utils::COLOR_RESET << std::endl;
    last_error_m = OPERATION_SUCCEED;
}

void Disk::unmount(){
    // If not mounted the operation aborts.
    // Unmap the allocated virtual disk file from memory.
    //      If fails the application will no longer works stable. This case should never occurres
    
    //utils::printModule("Disk", "Unmount");
    //std::cout << utils::TERMINAL_CLEAR;
    if(!mounted){
        last_error_m = DISK_UNMOUNTED;
        return;
    }
    std::cout << "[unmount] Unmounting disk..." << std::endl;
    if(0 != munmap(data_m,  static_cast<size_t>(fileStats_m->st_size))){
        std::cout << utils::COLOR_YELLOW << "[unmount] Unexpected error occurred, VDF is not unmapped" << std::endl;
        last_error_m = MAPPING_FAILED;
        return;
    }
    mounted = false;
    std::cout << utils::COLOR_GREEN << "[unmount] Disk is unmounted" << utils::COLOR_RESET << std::endl;
    last_error_m = OPERATION_SUCCEED;
}


void Disk::loadMasterBootRecord(){

    //  Required that the disk is mounted.
    //  1. Will aboirt if no MBR was found
    //  iterate over the MBR from the virtual disk
    //
    std::cout << "[MBR] Loading MBR from disk" << std::endl;

    if(!mounted){
        std::cout << utils::COLOR_RED << "[MBR] Loading aborted. Disk was not mounted" << utils::COLOR_RESET << std::endl;
        last_error_m = DISK_UNMOUNTED;
        return;
    }
    // 1.
    if(data_m[0] == 0x00){
        last_error_m = UNKNOWN_ERROR;
        return;
    }

    MBR_m = new MasterBootRecord();
    
    available_size_m -= 0x1FF - 1;
    for(int index = 0; index < 4; ++index){
        // Inspect if a the partition exists using the bootable flag
        if(data_m[0x1BE + (index * 16)] > 0x00){
            if(data_m[0x1BE + (index * 16)] == 0x50){
                continue;
            }
            // If the bootable flag is 0x80 this is a primary partition
            bool bootable = (data_m[0x1BE + (index * 16)] == 0x80);

            FileSystemType fs = numberToFileSystemTyp(data_m[0x1BF + (index * 16)]);

            // Calculate the start and end address
            uint32_t start = 0;
            start |= (uint32_t)data_m[0x1C0 + (index * 16)];
            start |= (uint32_t)data_m[0x1C1 + (index * 16)] << 8;
            start |= (uint32_t)data_m[0x1C2 + (index * 16)] << 16;
            start |= (uint32_t)data_m[0x1C3 + (index * 16)] << 24;

            uint32_t end = 0;
            end |= (uint32_t)data_m[0x1C4 + (index * 16)];
            end |= (uint32_t)data_m[0x1C5 + (index * 16)] << 8;
            end |= (uint32_t)data_m[0x1C6 + (index * 16)] << 16;
            end |= (uint32_t)data_m[0x1C7 + (index * 16)] << 24;

            // Create new partition in the MBR Wrapper object
            uint32_t size = end - start;
            
            // update the available size
            available_size_m -= size;
            std::cout << "Add partition" << std::endl;
            MBR_m->addPartition(new Partition(start, end, fs, bootable), index);
        }
    }
    last_error_m = OPERATION_SUCCEED;
}

void Disk::createMasterBootRecord(){

    //std::cout << utils::TERMINAL_CLEAR;
    //utils::printModule("Disk", "MBRCreator");

    std::cout << "[MBR] Create Master Boot Record" << std::endl;
    if(!mounted){
        std::cout << utils::COLOR_RED << "[MBR] Operation aborted. Disk was not mounted" << utils::COLOR_RESET << std::endl;
        last_error_m = DISC_NOT_MOUNTED;
        return;
    }

    std::cout << "[MBR] Writing boot code: [";
    for(int i = 0; i < (0x1BE - 4); i += 4){
        data_m[i] = static_cast<uint8_t>(0xB0);
        data_m[i + 1] = static_cast<uint8_t>(0x01);
        data_m[i + 2] = static_cast<uint8_t>(0xC0);
        data_m[i + 3] = static_cast<uint8_t>(0xDE);
    }
    std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;

    std::cout << "[MBR] Filling empty MBR: [";
    for(int i = 0x1BE; i < 0x1F8; i++){
       data_m[i] = 0;
    }
    std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;

    std::cout << "[MBR] Writing reserved flags: [";
    for(int i = 0; i < 4; i++)data_m[0x1BE + (i * 16)] = static_cast<uint8_t>(0x00);
    std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;

    std::cout << "[MBR] Writing magic numbers: [";
    data_m[0x1FE] = static_cast<uint8_t>(0x55);
    data_m[0x1FF] = static_cast<uint8_t>(0xAA);
    std::cout << utils::COLOR_GREEN << utils::ICON_ACCEPT << utils::COLOR_RESET << "]" << std::endl;

    std::cout << utils::COLOR_GREEN << "[MBR] Successfull created." << utils::COLOR_RESET << std::endl;

    MBR_m = new MasterBootRecord();
    available_size_m = size_m - 0x1FF - 1;
    last_error_m = OPERATION_SUCCEED;
}


uint32_t Disk::calculateStartAddress(uint8_t partitionIndex){
    // The partition should be placed at the end of the previous one
    // => It is required to know the end address of the previous partition
    //    The endaddress = Startaddresss + size + 1.
    // If no previous partition exists the operation returns nullptr

    if(partitionIndex == 0){
        return MBR_SIZE + 1;
    }
    Partition* partition = MBR_m->partition(partitionIndex - 1);
    if(nullptr == partition){
        std::cout << "=====> nullptr == partition ... Why this is called<=====" << std::endl;
        return MBR_SIZE + 1;
      }
    return  (partition->startAddress() + partition->size() + 1);
}

void Disk::createPartition(int64_t size, FileSystemType fileSystemType, bool isBootable){
    if(size < 0){
        std::cerr << utils::COLOR_RED << "[partition] A negative size was entered: " << size << utils::COLOR_RESET << std::endl;
        last_error_m = REQUESTED_PARTITION_SIZE_NEGATIVE;
        return;
    }    
    if(size > available_size_m){
        std::cerr << utils::COLOR_RED << "[partition] Size of " << size << " exceeds the maximum capacity of " << available_size_m  << utils::COLOR_RESET << std::endl; 
        last_error_m = REQUESTED_PARTITION_SIZE_TO_BIG;
        return;
    }

    // Required a mounted disk, exisitng MBR and a spot inside the Partitiontable
    //  => From the last results a max available amount of partition of 4
    
    //utils::printModule("Disk", "PartitionCreator");
    //std::cout << "[partition] Creation started" << std::endl;
    if(nullptr == MBR_m){
        std::cout << "[partition] Create failed. No MBR found on disk" << std::endl;
        last_error_m = NO_MASTER_BOOT_RECORD;
        return;
    }

    // Find a free partition inside the MBRs Partitiontable
    int index = MBR_m->findFreePartition();

    if(index == MBR_FULL){
       std::cout << "[partition] Creation failed. MBR is FULL." << std::endl;
       last_error_m = FULL_MASTER_BOOT_RECORD;
        return;
    }

    // Calculate the start address. If it the first partition 512 will be used
    //  Otherwise 512 + size of the previous partition + 1 will be used
    uint32_t start = calculateStartAddress(index);
    uint32_t end = start + size;
    // If the partition is not empty the operation will cancel at this point
    // TODO Fix this with primary 0x00 = inactive 0x80 = active
    std::cout << (int)data_m[0x1BE + (index*16)] << std::endl;;
    if(data_m[0x1BE + (index * 16)] != 0x00){
        last_error_m = REQUESTED_PARTITION_ALREADY_EXISTS;
        return;
    }
    MBR_m->addPartition(new Partition(start, end, fileSystemType, isBootable), index);

    // Write primary partition flag and filesystem type.
    // Note an partition entry is exact 16 Bytes

    data_m[0x1BE + (index * 16)] = static_cast<uint8_t>(0x80);

    data_m[0x1BF + (index * 16)] =  static_cast<uint8_t>(fileSystemType);

    // Ill assign the LSB to the smallest position and the HSB to the highest position.
    // Start adress of partition
    data_m[0x1C0 + (index * 16)] = static_cast<uint8_t>(start & 0xFF);
    data_m[0x1C1 + (index * 16)] = static_cast<uint8_t>((start >> 8) & 0xFF);
    data_m[0x1C2 + (index * 16)] = static_cast<uint8_t>((start >> 16) & 0xFF);
    data_m[0x1C3 + (index * 16)] = static_cast<uint8_t>((start >> 24) & 0xFF);

    // End adress of partition
    data_m[0x1C4 + (index * 16)] = static_cast<uint8_t>(end & 0xFF);
    data_m[0x1C5 + (index * 16)] = static_cast<uint8_t>((end >> 8) & 0xFF);
    data_m[0x1C6 + (index * 16)] = static_cast<uint8_t>((end >> 16) & 0xFF);
    data_m[0x1C7 + (index * 16)] = static_cast<uint8_t>((end >> 24) & 0xFF);

    // Unused, e.g. CHS address, etc
    for(uint64_t i = 0x1C8; i < (0x1CE - 4); i+=4){
        data_m[i] = static_cast<uint8_t>(0xBA);
        data_m[i + 1] = static_cast<uint8_t>(0xDF);
        data_m[i + 2] = static_cast<uint8_t>(0xF0);
        data_m[i + 3] = static_cast<uint8_t>(0x0D);
    }
    
    available_size_m -= size;
    
    last_error_m = OPERATION_SUCCEED;
}

void Disk::wipePartition(uint8_t index){
    std::cout << "This operation is disabled";
    last_error_m = NOT_IMPLEMENTED;
    return;
    index--;
    if(nullptr == MBR_m->partition(index)){
        std::cerr << utils::COLOR_RED << "[Wipe] Requested partition is allready wiped." << utils::COLOR_RESET << std::endl;
        last_error_m = REQUESTED_PARTITION_ALREADY_WIPED;
        return;
    }

    uint64_t start = MBR_m->partition(index)->startAddress();
    uint64_t end = MBR_m->partition(index)->endAddress();
    uint64_t size = MBR_m->partition(index)->size();
    std::cout << "0x" << std::hex << (start + size) << std::dec << std::endl;
    std::cout << "[Wipe] Start wiping"<< std::endl; 
    ProgressBar bar;
    // Perfomace decision. One step is one percent of the size.
    uint32_t step = size / 100;
    bar.maximum(100);
    if(step <= 100){
        bar.maximum(size);
        step = 1;
    } 
    for(int i = 0; i < end; i++){
        data_m[start + i] = static_cast<uint8_t>(0);
//        data_m[start + i] = static_cast<uint8_t>(rand() % 0xFF); // 
        if(i % step == 0){ 
            bar.update();
        }
    }
    MBR_m->removePartition(index);

    data_m[0x1BE + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1BF + (index * 16)] = static_cast<uint8_t>(0x00);


    data_m[0x1C0 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C1 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C2 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C3 + (index * 16)] = static_cast<uint8_t>(0x00);

    data_m[0x1C4 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C5 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C6 + (index * 16)] = static_cast<uint8_t>(0x00);
    data_m[0x1C7 + (index * 16)] = static_cast<uint8_t>(0x00);

    last_error_m = OPERATION_SUCCEED;
}

void Disk::removePartition(uint8_t index){
    index--;
    if(nullptr == MBR_m->partition(index)){
        std::cerr << utils::COLOR_RED << "[Wipe] Requested partition is allready wiped." << utils::COLOR_RESET << std::endl;
        last_error_m = REQUESTED_PARTITION_ALREADY_WIPED;
        return;
    }
    MBR_m->removePartition(index);
    data_m[0x1BE + (index * 16)] = static_cast<uint8_t>(0x00);
    last_error_m = OPERATION_SUCCEED;
}
