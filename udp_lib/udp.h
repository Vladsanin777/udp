/**
 * @file udp_lib/udp.h
 * @author Vladsanin777
 * @brief Header file for work udp package.
 */

#include <stdint.h>
#include <stdlib.h>

/**
 * @defgroup UdpPack work for udp
 * @brief Group function for work udp package.
 * @{
 */

/**
 * @brief Private struct UDP package. (Hidden implementation)
 */
struct udp_pack;

/**
 * @brief UDP packet descriptor.
 *
 * The primary type for working with the library. All functions accept this pointer as an argument.
 */
typedef struct udp_pack * udp_pack_t;

/**
 * @brief Function for create object UDP package.
 * @note You must call @ref destroy_udp_pack after this.
 * @return pointer or null on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * // other code whit using udp_pack_t
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
udp_pack_t init_udp_pack(void);

/**
 * @brief Function for setting source port in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] port Source port to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_port_source_udp_pack(pack, "8001");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_port_source_udp_pack(udp_pack_t pack, const char * const port);

/**
 * @brief Function for setting destination port in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] port Destination port to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_port_destination_udp_pack(pack, "8003");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_port_destination_udp_pack(udp_pack_t pack, const char * const port);

/**
 * @brief Function for setting source ip in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] ip Source ip to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_ip_source_udp_pack(pack, "127.0.0.1");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_ip_source_udp_pack(udp_pack_t pack, const char * const ip);

/**
 * @brief Function for setting destination ip in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] ip Destination ip to send.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_ip_destination_udp_pack(pack, "127.0.0.1");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_ip_destination_udp_pack(udp_pack_t pack, const char * const ip);

/**
 * @brief Function give size data in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return size data in UDP package.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * get_size_data_udp_pack(pack);
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
uint16_t get_size_data_udp_pack(udp_pack_t pack);

/**
 * @brief Function addition data in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] data Addition data in UDP package.
 * @param[in] size Size addition data for UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * const char * const message = "Hello, world";
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * add_date_udp_pack(pack, message, strlen(message));
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t add_date_udp_pack(udp_pack_t pack, void * data, uint16_t size);

/**
 * @brief Function overriding data in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] data New data in UDP package.
 * @param[in] size Size new data for UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * const char * const message = "Hello, world";
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_date_udp_pack(pack, message, strlen(message));
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_data_udp_pack(udp_pack_t pack, void * data,  \
        const uint16_t size);

/**
 * @brief Function addition byte in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] byte Addition byte in UDP package.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_date_udp_pack(pack, 'n');
 * set_date_udp_pack(pack, '\0');
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t add_byte_udp_pack(udp_pack_t pack, \
        const uint8_t byte);

/**
 * @brief Function read data from stdin in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_input_data_udp_pack(pack);
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_input_data_udp_pack(udp_pack_t pack);

/**
 * @brief Function read data from file in UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] file_name File name for read in UDP pack.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_file_data_udp_pack(pack, "Hello.txt");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_file_data_udp_pack( \
        udp_pack_t pack, const char * const file_name);

/**
 * @brief Function for pick interface to send UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @param[in] interface Interface to send UDP pack.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * set_interface_udp_pack(pack, "lo");
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t set_interface_udp_pack( \
        udp_pack_t pack, const char * const interface);

/**
 * @brief Function to send UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * @return 0 or -1 on error.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * send_udp_pack(pack);
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
ssize_t send_udp_pack(udp_pack_t pack);

/**
 * @brief Function free UDP package.
 * @note You must call @ref init_udp_pack before this.
 * @param[in,out] pack UDP package for work.
 * Usage example.
 * @code
 * ssize_t ret = 0;
 * udp_pack_t pack = init_udp_pack();
 * if (pack == NULL) {
 *     ret = -1;
 *     goto get_not_udp_pack;
 * }
 * // other code whit using udp_pack_t
 * get_not_udp_pack:
 * destroy_udp_pack(pack);
 * @endcode
 */
void destroy_udp_pack(udp_pack_t pack);

/** @} */
