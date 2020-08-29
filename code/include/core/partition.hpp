#ifndef CORE__PARTITION_HPP
#define CORE__PARTITION_HPP

// c++ includes
#include <vector>

// c++ equivalent includes from c
#include <cstdint>

// c include
extern "C"
{
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
}

// libraries

// project includes
#include "../utils/utils.h"

/*!
 *  \addtogroup core
 *  @{
 */

/**
 * @brief Core Elements
 * @details Defines partitions, their details and codes.
 */
namespace core
{

    /**
     * @brief Contains all possible Result Codes of any operation on a partition
     * @details Mostly used by validators.
     * @version 1.0.0.0
     */
    enum partition_codes
    {
        OK = 0x00,                    ///< Ok
        data_invalid = 0x01,          ///< Invalid Data
        address_underflow = 0x02,     ///< Address Underflow
        address_overflow = 0x03,      ///< Address Overflow
        partition_mounted = 0x04,     ///< Partition is mounted.
        partition_not_mounted = 0x05, ///< Partition is not mountet.
        index_out_of_bounds = 0x06,   ///< Array has been accessed with an illegal index.
        no_definition = 0x07,         ///< Definition is missing
        file_not_exist = 0x08,        ///< File does not exist
        file_open_failure = 0x09,     ///< Can't open file
        mapping_failure = 0x0A,       ///< Mapping failed

        unknown = 0xFF ///< Unknown result
    };

    /**
     * @brief Defines partition details on a virtual disk
     * @details Contains basic information about addresses, type and primary flag
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
     * @brief Wraps a part of a virtual disk file which represents a partition
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

        /**
         * @brief Clears a byte
         * @details Clears a byte inside the partition.
         *          Requires a mounted partition.
         * @param address Byteaddress
         * @todo Add data access
         */
        void clearByte(uint64_t address);


    public: /* setter **************************************/
        /**
         * @brief Sets the error handler
         * @param callback Callback for the error handler, signature: void name(uint8_t code);
         */
        void settErrorHandler(void (*callback)(uint8_t code))
        {
            error_handler = callback;
        }


    public: /* getter **************************************/
        /**
         * @brief Gets mounting state.
         * @return True if the partition is mounted
         */
        bool isMounted()
        {
            return mounted;
        }


    private: /* inlined methods *****************************/
        /**
         * @brief Checks if a error handler exists and calls it      
         * @param code Errorcode for the error handler
         */
        inline void handle_error(uint8_t code)
        {
            if (nullptr != error_handler)
            {
                error_handler(code);
            }
        }

        /**
         * @brief Validates data block 
         * @details Validates the block for nullptr.
         *          Calls an error handler if check was failed.
         * @return true if block is valid, otherwise false 
         */
        inline bool validate_data()
        {
            if (nullptr == data_m)
            {
                handle_error(partition_codes::data_invalid);
                return false;
            }
            return true;
        }

        /**
         * @brief Validates address
         * @details Validates if a address is within partition range.
         *          Calls an error handler if the validation fails.
         * @param address To be validated address
         * @return True if the address within partition range, otherwise false
         */
        inline bool validate_address(uint64_t address)
        {
            ///1. Checks if address is behind end of address range
            if (address > definition->end_address)
            {
                handle_error(partition_codes::address_overflow);
                return false;
            }

            ///2. Checks if address is before start of address range
            if (address < definition->start_address)
            {
                handle_error(partition_codes::address_underflow);
                return false;
            }

            return true;
        }
    };
} // namespace core

#endif //CORE__PARTITION_HPP
