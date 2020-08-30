#ifndef CORE__PARTITION_HPP
#define CORE__PARTITION_HPP

// c++ includes
#include <vector>

// c++ equivalent includes from c 
#include <cstdint>

// c include
extern "C"{
    #include <unistd.h>
    #include <fcntl.h>
    #include <sys/mman.h>
    #include <sys/stat.h>
}

// libraries

// project includes
#include "../utils/utils.h"


namespace core{
    

    /**
     * @brief This enumeration contains all possible result codes of any operation on a partition
     */
    enum partition_codes{
        OK = 0x00,
        data_invalid = 0x01,
        address_underflow = 0x02,
        address_overflow = 0x03,
        partition_mounted = 0x04,
        partition_not_mounted = 0x05,
        index_out_of_bounds = 0x06,
        no_definition = 0x07,
        file_not_exist = 0x08,
        file_open_failure = 0x09, 
        mapping_failure = 0x0A,

        unknown = 0xFF
    };

    /**
     * @brief This struct contains basic information about a partition
     */
    struct partition_definition{

        uint64_t start_address;
        uint64_t end_address;
        uint8_t file_system_type;
        uint8_t primary;
        uint8_t reserved_1;
        uint8_t reserved_2;
        uint8_t reserved_3;
        uint8_t reserved_4;

        uint16_t block_size;
        uint64_t cluster_size;

        char* path;
    };

    /**
     * @brief This class wraps a part of a virtual disk file which represents a partition.
     */
    class Partition{
        private:
    
        /**
         * @brief Actual partition data
         */
        uint8_t* data_m = nullptr;

        static int counter;
    protected: // attributes
    
        /**
         * @brief This is a function pointer for an error handler
         * @details This fp is called iff an error occurred during one operation
         * @param code The error code of the operation
         */
        void(*error_handler)(uint8_t code);

        /**
         * @brief This struct contains basic information about this partition
         */
        partition_definition* definition;

       


        /**
         * @brief Partition size, startaddress + endaddress
         */
        uint64_t size;

        /**
         * @brief Mounting state of the partition
         */
        bool mounted = false;


    public: //methods;



        Partition() = delete;

        explicit Partition(partition_definition* definition);

        virtual ~Partition();

        /**
         * @brief Mounts the partition
         * @details This methods opens the disk file and mounts the section which correspend to the
         *          partition. It will mmap the part into the data attribute. 
         */
        void mount();

        /**
         * @brief Unmounts the partition
         * @details This methods unmmap the part from memory. 
         */
        void unmount();

        /**
         * @brief Sets a byte
         * @details This methods sets a byte inside the partition
         *          Required a mounted partition.
         * @param address Address of the byte
         * @param data New data
         */
        void setByte(uint64_t address, uint8_t data);

        /**
         * @brief Clears a byte
         * @details This methods clears a byte inside the partition
         *          Required a mounted partition.
         * @param address Address of the byte
         */
        void clearByte(uint64_t address);

        uint8_t inline getByte(uint64_t address){
//            return data_m[(definition->start_address % 4096) + address];
            if(!isMounted()){
                std::cout << "NOT MOUNTED" << std::endl;
                return 0xFF;
            }
            return data_m[512 + address];
        }

    public: // setter
        /**
         * @brief Sets the error handler
         * @param callback Callback for the error handler, signature: void name(uint8_t code);
         */
        void settErrorHandler(void(*callback)(uint8_t code)){
            error_handler = callback;
        }


    public: // getter
        /**
         * @brief This method returns the mounting state.
         * @return True iff the partition is mounted
         */
        bool isMounted(){
            return mounted;
        }

        partition_definition* part_definition(){
            return definition;
        }

    void showPartition(uint64_t start = 0, uint64_t end = -1);
    
        void inline toggle_mount(){
            if(isMounted()){
                unmount();
            }else{
                mount();
            }
        }

    private: // inlined methods
        /**
         * @brief This method checks if a error handler exists and calls it      
         * @param code Errorcode for the error handler
         */
        inline void handle_error(uint8_t code){
            if(nullptr != error_handler){
                error_handler(code);
            }
        }

        /**
         * @brief Validates the data block 
         * @details This inlined method validate the existing data block for nullptr.
         *          Calls an error handler if the validation fails
         * @return True iff the data block is not null otherwise false 
         */
        inline bool validate_data(){
            if(nullptr == data_m){
                handle_error(partition_codes::data_invalid);
                return false;
            }
            return true;
        }

        /**
         * @brief Validates the address
         * @details This methods validates if a address is in range of the partition
         *          Calls an error handler if the validation fails
         * @param address Address which should be validated
         * @return True iff the address is in range of the partition range
         */
        inline bool validate_address(uint64_t address){        
            if(address > definition->end_address){
                handle_error(partition_codes::address_overflow);
                return false;
            }
            
            if(address < definition->start_address){
                handle_error(partition_codes::address_underflow);
                return false;
            }
            return true;
        }
    };
}

#endif //CORE__PARTITION_HPP
