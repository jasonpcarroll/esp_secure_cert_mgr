/*
 * SPDX-FileCopyrightText: 2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once
#include <stdint.h>

#ifdef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
#include "esp_ds.h"
#endif

#define ESP_SECURE_CERT_PKEY_MAGIC_BYTE        0xC1   /* Magic byte of the generated private key */
#define ESP_SECURE_CERT_DEV_CERT_MAGIC_BYTE    0xC2   /* Magic byte of the generated device certificate */
#define ESP_SECURE_CERT_CA_CERT_MAGIC_BYTE     0xC3   /* Magic byte of the CA certificate */
#define ESP_SECURE_CERT_CS_CERT_MAGIC_BYTE     0xC4   /* Magic byte of the generated code signing certificate */

#ifdef CONFIG_ESP_SECURE_CERT_NVS_PARTITION
/* NVS Config */
#define ESP_SECURE_CERT_NVS_PARTITION       CONFIG_ESP_SECURE_CERT_PARTITION_NAME
#define ESP_SECURE_CERT_NVS_KEYS_PARTITION  CONFIG_ESP_SECURE_CERT_KEYS_PARTITION_NAME

#define ESP_SECURE_CERT_PRIV_KEY            "priv_key"
#define ESP_SECURE_CERT_DEV_CERT            "dev_cert"
#define ESP_SECURE_CERT_CA_CERT             "ca_cert"
#define ESP_SECURE_CERT_CS_CERT             "cs_cert"
#define ESP_SECURE_CERT_NAMESPACE           CONFIG_ESP_SECURE_CERT_PARTITION_NAME

#define ESP_SECURE_CERT_CIPHERTEXT          "cipher_c"
#define ESP_SECURE_CERT_RSA_LEN             "rsa_len"
#define ESP_SECURE_CERT_EFUSE_KEY_ID        "ds_key_id"
#define ESP_SECURE_CERT_IV                  "iv"

#elif CONFIG_ESP_SECURE_CERT_CUST_FLASH_PARTITION
#define ESP_SECURE_CERT_METADATA_SIZE                  64     /* 32 bytes are reserved for the metadata (Must be a multiple of 32)*/

#define ESP_SECURE_CERT_DEV_CERT_SIZE                  2048
#define ESP_SECURE_CERT_CA_CERT_SIZE                   4096
#define ESP_SECURE_CERT_CS_CERT_SIZE                   2048
#ifndef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
#define ESP_SECURE_CERT_PRIV_KEY_SIZE                  4096
#else
#define ESP_SECURE_CERT_CIPHERTEXT_SIZE                (ESP_DS_C_LEN + 16)
#define ESP_SECURE_CERT_IV_SIZE                        (ESP_DS_IV_LEN + 16)
#endif

#define ESP_SECURE_CERT_METADATA_OFFSET                0           /* 32 bytes are reserved for the metadata (Must be a multiple of 32)*/
#define ESP_SECURE_CERT_DEV_CERT_OFFSET                (ESP_SECURE_CERT_METADATA_OFFSET + ESP_SECURE_CERT_METADATA_SIZE)
#define ESP_SECURE_CERT_CA_CERT_OFFSET                 (ESP_SECURE_CERT_DEV_CERT_OFFSET + ESP_SECURE_CERT_DEV_CERT_SIZE)
#define ESP_SECURE_CERT_CS_CERT_OFFSET                 (ESP_SECURE_CERT_CA_CERT_OFFSET + ESP_SECURE_CERT_CA_CERT_SIZE)
#ifndef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
#define ESP_SECURE_CERT_PRIV_KEY_OFFSET                (ESP_SECURE_CERT_CS_CERT_OFFSET + ESP_SECURE_CERT_CS_CERT_SIZE)
#define ESP_SECURE_CERT_MAX_SIZE                       (ESP_SECURE_CERT_PRIV_KEY_OFFSET + ESP_SECURE_CERT_PRIV_KEY_SIZE)
#else
#define ESP_SECURE_CERT_CIPHERTEXT_OFFSET              (ESP_SECURE_CERT_CS_CERT_OFFSET + ESP_SECURE_CERT_CS_CERT_SIZE)
#define ESP_SECURE_CERT_IV_OFFSET                      (ESP_SECURE_CERT_CIPHERTEXT_OFFSET + ESP_SECURE_CERT_CIPHERTEXT_SIZE)
#define ESP_SECURE_CERT_MAX_SIZE                       (ESP_SECURE_CERT_IV_OFFSET + ESP_SECURE_CERT_IV_SIZE)
#endif

#define ESP_SECURE_CERT_PARTITION_TYPE          0x3F        /* Custom partition type */
#define ESP_SECURE_CERT_PARTITION_NAME          CONFIG_ESP_SECURE_CERT_PARTITION_NAME  /* Name of the custom pre prov partition */
#define ESP_SECURE_CERT_METADATA_MAGIC_WORD     0x12345678


typedef struct {
    uint32_t dev_cert_crc;                          /* CRC of the dev cert data */
    uint16_t dev_cert_len;                          /* The actual length of the device cert */
    uint32_t ca_cert_crc;                           /* CRC of the ca cert data */
    uint16_t ca_cert_len;                           /* The actual length of the ca cert [The length before the 32 byte alignment] */
    uint32_t cs_cert_crc;                           /* CRC of the cs cert data */
    uint16_t cs_cert_len;                           /* The actual length of the cs cert [The length before the 32 byte alignment] */
#ifndef CONFIG_ESP_SECURE_CERT_DS_PERIPHERAL
    uint32_t priv_key_crc;                          /* CRC of the priv key data */
    uint16_t priv_key_len;                          /* The actual length of the private key */
#else
    uint32_t ciphertext_crc;                        /* CRC of the ciphertext data */
    uint16_t ciphertext_len;                        /* The actual length of the ciphertext */
    uint32_t iv_crc;                                /* CRC of the iv data */
    uint16_t iv_len;                                /* The actual length of iv*/
    uint16_t rsa_length;                            /* Length of the RSA private key that is encrypted as ciphertext */
    uint8_t  efuse_key_id;                          /* The efuse key block id which holds the HMAC key used to encrypt the ciphertext */
#endif
    uint32_t magic_word;                            /* The magic word which shall identify the valid metadata when read from flash */
} esp_secure_cert_metadata;

#else
#error "Invalid type of partition selected"
#endif
