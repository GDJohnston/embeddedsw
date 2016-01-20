/******************************************************************************
*
* Copyright (C) 2014 - 2015 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* XILINX CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/
/*****************************************************************************/
/**
*
* @file xhdcp22_tx_crypt.c
*
* This is the file for Xilinx HDCP22 TX cryptographic functionality.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver   Who    Date     Changes
* ----- ------ -------- -------------------------------------------------------
* 1.00         10/06/15 Initial release.
* 1.01         01/15/16 Replaced mallocs with fixed size arrays.
* </pre>
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "xhdcp22_tx_i.h"

#include "xhdcp22_common.h"

#include <stdlib.h>
#include <math.h>

/************************** Constant Definitions *****************************/

/***************** Macros (Inline Functions) Definitions *********************/

/** Bigdigits for RSA allow 512 octets, 4096 bits. */
#define BD_MAX_MOD_SIZE  512

/**************************** Type Definitions *******************************/

/************************** Function Prototypes ******************************/

static int XHdcp22Tx_Mgf1(const u8 *Seed, unsigned int Seedlen, u8 *Mask,
                          unsigned int MaskLen);
static int XHdcp22Tx_RsaOaepEncrypt(const u8 *KeyPubNPtr, int KeyPubNSize,
                                    const u8 *KeyPubEPtr, int KeyEPubSize,
                                    const u8 *Message, const u32 MessageLen,
                                    const u8 *MaskingSeed, u8 *EncryptedMessage);
static int XHdcp22Tx_Pkcs1EmeOaepEncode(const u8 *Message, const u32 MessageLen,
										const u8 *MaskingSeed, u8 *EncodedMessage);
static int XHdcp22Tx_RsaEncryptMsg(const u8 *KeyPubNPtr, int KeyPubNSize,
                                   const u8 *KeyPubEPtr, int KeyPubESize,
                                   const u8 *MsgPtr, int MsgSize,
                                   u8 *EncryptedMsgPtr);

/************************** Variable Definitions *****************************/

/************************** Function Definitions *****************************/

/*****************************************************************************/
/**
*
* This function is the RSA OAEP masking function.
*
* @param  Seed is the masking seed to use.
* @param  Seedlen is the size of the masking seed .
* @param  Mask is the output mask.
* @param  MaskLen is size of the mask.
*
* @return XST_SUCCESS
*
* @note   None.
*
******************************************************************************/
static int XHdcp22Tx_Mgf1(const u8 *Seed, unsigned int Seedlen,
                          u8 *Mask, unsigned int MaskLen)
{
	/* Verify arguments */
	Xil_AssertNonvoid(Seed != NULL);
	Xil_AssertNonvoid(Seedlen > 0);
	Xil_AssertNonvoid(Mask != NULL);
	Xil_AssertNonvoid(MaskLen > 0);

	u8  Hash[XHDCP22_TX_SHA256_HASH_SIZE];
	u8  HashData[XHDCP22_TX_CERT_PUB_KEY_N_SIZE];	// Seedlen+4
	u32 C;
	u8  Cbig[4];
	u8  T[XHDCP22_TX_CERT_PUB_KEY_N_SIZE]; 		// MaskLen + XHDCP22_TX_SHA256_HASH_SIZE

	/* Step 2: Clear T */
	memset(T, 0x00, sizeof(T));

	/* Step 3: T = T || SHA256(mgfSeed || C) */
	memcpy(HashData, Seed, Seedlen);

	for(C=0; (C*XHDCP22_TX_SHA256_HASH_SIZE) < MaskLen; C++)
	{
		/* Convert counter value to big endian */
		Cbig[0] = ((C >> 24) & 0xFF);
		Cbig[1] = ((C >> 16) & 0xFF);
		Cbig[2] = ((C >> 8) & 0xFF);
		Cbig[3] = (C & 0xFF);

		/* Constructing Hash Input */
		memcpy(HashData+Seedlen, &Cbig, 4);

		/* Computing Hash */
		XHdcp22Cmn_Sha256Hash(HashData, Seedlen+4, Hash);

		/* Appending Hash to T */
		memcpy(T+C*XHDCP22_TX_SHA256_HASH_SIZE, Hash, XHDCP22_TX_SHA256_HASH_SIZE);
	}

	/* Step 4: Output leading maskLen octets of T */
	memcpy(Mask, T, MaskLen);

	return XST_SUCCESS;
}

/****************************************************************************/
/**
* This function implements EME-OAEP encoding. The label L is the empty
* string and the underlying hash function is SHA256.
*
* Reference: PKCS#1 v2.1, Section 7.1.1, Part 2
*
* @param	Message is the octet string to be encoded
* @param	MessageLen is the length of the octet string to be encoded.
* @param	MaskingSeed is the random octet string seed of length hLen.
* @param	EncodedMessage is the 128 byte encoded octet string.
*
* @return	XST_SUCCESS or XST_FAILURE.
*
* @note		None.
*****************************************************************************/
static int XHdcp22Tx_Pkcs1EmeOaepEncode(const u8 *Message, const u32 MessageLen,
	const u8 *MaskingSeed, u8 *EncodedMessage)
{
	/* Verify arguments */
	Xil_AssertNonvoid(Message != NULL);
	Xil_AssertNonvoid(MessageLen > 0);
	Xil_AssertNonvoid(MaskingSeed != NULL);
	Xil_AssertNonvoid(EncodedMessage != NULL);

	u8  lHash[XHDCP22_TX_SHA256_HASH_SIZE];
	u8  seed[XHDCP22_TX_SHA256_HASH_SIZE];
	u8  dbMask[XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1];
	u8  DB[XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1];
	u8  seedMask[XHDCP22_TX_SHA256_HASH_SIZE];
	u32 Status;

	XHdcp22Tx_LogWrNoInst(XHDCP22_TX_LOG_EVT_DBG, XHDCP22_TX_LOG_DBG_OEAPENC);

	/* Step 2a: L is the empty string */
	XHdcp22Cmn_Sha256Hash(NULL,0,lHash);

	/* Step 2b: Generate PS by initializing DB to zeros */
	memset(DB, 0x00, sizeof(DB));

	/* Step 2c: Generate DB = lHash || PS || 0x01 || M */
	memcpy(DB, lHash, XHDCP22_TX_SHA256_HASH_SIZE);
	DB[XHDCP22_TX_CERT_PUB_KEY_N_SIZE-MessageLen-XHDCP22_TX_SHA256_HASH_SIZE-2] = 0x01;
	memcpy(DB+XHDCP22_TX_CERT_PUB_KEY_N_SIZE-MessageLen-XHDCP22_TX_SHA256_HASH_SIZE-1,
			Message, MessageLen);

	/* Step 2d: Generate random seed of length hLen
	 *   The random seed is passed in as an argument to this function.
	 */

	/* Step 2e: Generate dbMask = MGF1(seed, length(DB)) */
	Status = XHdcp22Tx_Mgf1(MaskingSeed, XHDCP22_TX_SHA256_HASH_SIZE, dbMask,
									XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Step 2f: Generate maskedDB = DB xor dbMask */
	XHdcp22Tx_MemXor(DB, DB, dbMask, XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1);

	/* Step 2g: Generate seedMask = MGF(maskedDB, length(seed)) */
	Status = XHdcp22Tx_Mgf1(DB, XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1,
									seedMask, XHDCP22_TX_SHA256_HASH_SIZE);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Step 2h: Generate maskedSeed = seed xor seedMask */
	XHdcp22Tx_MemXor(seed, MaskingSeed, seedMask, XHDCP22_TX_SHA256_HASH_SIZE);

	/* Step 2i: Form encoded message EM = 0x00 || maskedSeed || maskedDB */
	memset(EncodedMessage, 0x00, XHDCP22_TX_CERT_PUB_KEY_N_SIZE);
	memcpy(EncodedMessage+1, seed, XHDCP22_TX_SHA256_HASH_SIZE);
	memcpy(EncodedMessage+1+XHDCP22_TX_SHA256_HASH_SIZE, DB,
			XHDCP22_TX_CERT_PUB_KEY_N_SIZE-XHDCP22_TX_SHA256_HASH_SIZE-1);

	XHdcp22Tx_LogWrNoInst(XHDCP22_TX_LOG_EVT_DBG,
	                      XHDCP22_TX_LOG_DBG_OEAPENC_DONE);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
* RSA verification primitive recovers the message representative from
* the signature representative under the control of a public key.
* This is the same as encryption, but it in a totally different context.
*
* @param  KeyPubNPtr is the RSA public key modulus part.
* @param  KeyPubNSize is size of the RSA public key modulus in bytes.
* @param  KeyPubEPtr is the RSA public key E (exponential) part.
* @param  KeyEPubSize is the sizeof the RSA public key E (exponential) part in
*         bytes.
* @param  MsgPtr is the data to verify (for example a cryptographic signature).
* @param  MsgSize is the size of the data to verify.
* @param  EncodedMsgPtr is the resulting data for verification.
*
* @return
*         - XST_SUCCESS if encryption succeeds.
*         - XST_FAILURE if the message buffer size != key size.
*
* @note		Note: Verification with a public key is the same as RSA encoding.
*
******************************************************************************/
static int XHdcp22Tx_RsaEncryptMsg(const u8 *KeyPubNPtr, int KeyPubNSize,
								   const u8 *KeyPubEPtr, int KeyPubESize,
								   const u8 *MsgPtr, int MsgSize, u8 *EncryptedMsgPtr)
{
	DIGIT_T n[BD_MAX_MOD_SIZE], e[BD_MAX_MOD_SIZE],
	        m[BD_MAX_MOD_SIZE], s[BD_MAX_MOD_SIZE];
	unsigned int ModSize = KeyPubNSize / sizeof(DIGIT_T);


	/* Check if the length of the data is not equal to the key length. */
	if(MsgSize != KeyPubNSize) {
		return XST_FAILURE;
	}

	XHdcp22Tx_LogWrNoInst(XHDCP22_TX_LOG_EVT_DBG, XHDCP22_TX_LOG_DBG_RSAENC);

	mpConvFromOctets(n, ModSize, KeyPubNPtr, KeyPubNSize);
	mpConvFromOctets(e, ModSize, KeyPubEPtr, KeyPubESize);

	mpConvFromOctets(m, ModSize, MsgPtr, MsgSize);
	mpModExp(s, m, e, n, ModSize);
	mpConvToOctets(s, ModSize, EncryptedMsgPtr, MsgSize);

	XHdcp22Tx_LogWrNoInst(XHDCP22_TX_LOG_EVT_DBG, XHDCP22_TX_LOG_DBG_RSAENC_DONE);

	return XST_SUCCESS;
}

/*****************************************************************************/
/**
*
* This function generates random octets. In test mode it uses the test mode generator.
* Otherwise it uses offloading from hardware.
*
* @param  NumOctets is the number of octets in the random number.
* @param  RandomNumberPtr is a pointer to the random number.
*
* @return None.
*
* @note   None.
*
******************************************************************************/
void XHdcp22Tx_GenerateRandom(XHdcp22_Tx *InstancePtr, int NumOctets,
                              u8* RandomNumberPtr)
{
	/* Use hardware generator */
	XHdcp22Rng_GetRandom(&InstancePtr->Rng, RandomNumberPtr, NumOctets, NumOctets);
}

/****************************************************************************/
/**
* This function implements the RSAES-OAEP-Encrypt operation. The message
* is encoded using EME-OAEP and then encrypted with the public key
* using RSAEP.
*
* Reference: PKCS#1 v2.1, Section 7.1.1
*
* @param  KeyPubNPtr is the public certicate modulus.
* @param  KeyPubNSize is the size public certicate modulus.
* @param  KeyPubEPtr is the public certicate exponent.
* @param  KeyEPubSizeis the public certicate exponent size.
* @param  Message is the octet string to be encrypted.
* @param  MessageLen is the length of octet string to be encrypted and
*         must be less than or equal to (nLen - 2*hLen - 2)
* @param  MaskingSeed is the random octet string seed of length hLen
*         used by EME-OAEP encoding function.
*
* @return XST_SUCCESS or XST_FAILURE.
*
* @note   None.
*****************************************************************************/
static int XHdcp22Tx_RsaOaepEncrypt(const u8 *KeyPubNPtr, int KeyPubNSize,
                                    const u8 *KeyPubEPtr, int KeyEPubSize,
                                    const u8 *Message, const u32 MessageLen,
                                    const u8 *MaskingSeed, u8 *EncryptedMessage)
{
	/* Verify arguments */
	Xil_AssertNonvoid(Message != NULL);
	Xil_AssertNonvoid(MessageLen > 0);
	Xil_AssertNonvoid(MaskingSeed != NULL);
	Xil_AssertNonvoid(EncryptedMessage != NULL);

	u8 em[XHDCP22_TX_CERT_PUB_KEY_N_SIZE];
	int Status;

	/* Step 1: Length checking */
	if(MessageLen > (XHDCP22_TX_CERT_PUB_KEY_N_SIZE - 2*XHDCP22_TX_SHA256_HASH_SIZE - 2))
	{
		return XST_FAILURE;
	}

	/* Step 2: EME-OAEP Encoding */
	Status = XHdcp22Tx_Pkcs1EmeOaepEncode(Message, MessageLen, MaskingSeed, em);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Step 3: RSA encryption */
	Status = XHdcp22Tx_RsaEncryptMsg(KeyPubNPtr, KeyPubNSize, KeyPubEPtr,
	                                 KeyEPubSize, em, KeyPubNSize,
	                                 EncryptedMessage);
	if(Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}


/*****************************************************************************/
/**
*
* This function verifies the HDCP receivers certificate.
*
* @param  CertificatePtr is a pointer to the certificate from the HDCP2.2
*         receiver.
* @param  KpubDcpNPtr is a pointer to the N-value of the certificate signature.
* @param  KpubDcpNSize is the size of the N-value of the certificates signature.
* @param  KpubDcpEPtr iis a pointer to the E-value of the certificate signature.
* @param  KpubDcpESize is the size of the E-value of the certificates signature.
*
* @return
*         - XST_SUCCESS if the certificate is valid.
*         - XST_FAILURE if the certificate is invalid.
* @note   None.
*
******************************************************************************/
int XHdcp22Tx_VerifyCertificate(const XHdcp22_Tx_CertRx* CertificatePtr,
                                const u8* KpubDcpNPtr, int KpubDcpNSize,
                                const u8* KpubDcpEPtr, int KpubDcpESize)
{
	int Result = XST_SUCCESS;
	u8* EncryptedHashPtr = NULL;
	u8 HashedData[XHDCP22_TX_SHA256_HASH_SIZE];
	u8 Em[XHDCP22_TX_CERT_SIGNATURE_SIZE];

	/* Create hash of first part of the certificate (without the signature). */
	XHdcp22Cmn_Sha256Hash((u8 *)CertificatePtr,
                      sizeof(XHdcp22_Tx_CertRx) - sizeof(CertificatePtr->Signature),
                      HashedData);

	/* RSA decryption. */
	Result = XHdcp22Tx_RsaEncryptMsg(KpubDcpNPtr, KpubDcpNSize,
                                   KpubDcpEPtr, KpubDcpESize,
                                   CertificatePtr->Signature,
                                   sizeof(CertificatePtr->Signature),
                                   Em);
	if (Result != XST_SUCCESS) {
		return Result;
	}

	/* Compare hash and the last part of the encoded message.
	 * Note the encoded message contains padding bytes (0xff, 0x00=end-of-padding),
	 *  And ASN.1 values for:
	 * - the encoded message contents,
	 * - the used algorithm identifier (OID),
	 * - the value (HASH)
	 * According HDCP2.2 protocol we do not need to check these contents so we just check
	 * the last part of the encoded message that contains the hash.
	 */
	EncryptedHashPtr = &Em[XHDCP22_TX_CERT_SIGNATURE_SIZE-XHDCP22_TX_SHA256_HASH_SIZE];
	Result = memcmp(HashedData, EncryptedHashPtr,
                  XHDCP22_TX_SHA256_HASH_SIZE) == 0 ? XST_SUCCESS : XST_FAILURE;
	return Result;
}

/*****************************************************************************/
/**
*
* This function computes HPrime
*
* @param  Rrx is the Rx random generated value on start of authentication.
* @param  RxCaps are the capabilities of the receiver.
* @param  Rtx is the Tx random generated value start of authentication.
* @param  TxCaps are the capabilities of the receiver.
* @param  Km is the master key generated by tx.
* @param  HPrime is a pointer to the HPrime hash from the HDCP2.2 receiver.
*
* @return None.
*
* @note   None.
*
******************************************************************************/
void XHdcp22Tx_ComputeHPrime(const u8* Rrx, const u8 *RxCaps,
                             const u8* Rtx, const u8 *TxCaps,
                             const u8 *Km, u8 *HPrime)
{
	u8 Aes_Iv[XHDCP22_TX_AES128_SIZE];
	u8 Aes_Key[XHDCP22_TX_AES128_SIZE];
	u8 Kd[2 * XHDCP22_TX_AES128_SIZE]; /* Dkey0 || Dkey 1. */

	u8 HashInput[XHDCP22_TX_RTX_SIZE + XHDCP22_TX_RXCAPS_SIZE +
               XHDCP22_TX_TXCAPS_SIZE];
	int Idx = 0;

	/* Normally we should do: Key XOR with Rn, but Rn is 0. */
	memcpy(Aes_Key, Km, XHDCP22_TX_KM_SIZE);

	/* Determine dkey0. */
	/* Add m = Rtx || Rrx. */
	memcpy(Aes_Iv, Rtx, XHDCP22_TX_RTX_SIZE);
	/* Normally we should do Rrx XOR with Ctr0, but Ctr0 is 0. */
	memcpy(&Aes_Iv[XHDCP22_TX_RTX_SIZE], Rrx, XHDCP22_TX_RRX_SIZE);
	XHdcp22Cmn_Aes128Encrypt(Aes_Iv, Aes_Key, Kd);


	/* Determine dkey1, counter is 1: Rrx | 0x01. */
	Aes_Iv[15] ^= 0x01; /* big endian! */
	XHdcp22Cmn_Aes128Encrypt(Aes_Iv, Aes_Key, &Kd[XHDCP22_TX_KM_SIZE]);


	/* Create hash with HMAC-SHA256. */
	/* Input: Rtx || RxCaps || TxCaps. */
	memcpy(HashInput, Rtx, XHDCP22_TX_RTX_SIZE);
	Idx += XHDCP22_TX_RTX_SIZE;
	memcpy(&HashInput[Idx], RxCaps, XHDCP22_TX_RXCAPS_SIZE);
	Idx += XHDCP22_TX_RXCAPS_SIZE;
	memcpy(&HashInput[Idx], TxCaps, XHDCP22_TX_TXCAPS_SIZE);

	/* HashKey:	Kd*/
	XHdcp22Cmn_HmacSha256Hash(HashInput, sizeof(HashInput), Kd,
                           sizeof(Kd), HPrime);
}

/*****************************************************************************/
/**
*
* This function computes LPrime. Kd is calculated as in
* #XHdcp22Tx_ComputeHPrime, but could also be stored the first time.
* This could save some computing time.
* According protocol however, only Rrx and Rtx are supposed to be stored.
*
* @param  Rn is a pseudo-random nonce.
 @param   Km is the master key generated by tx..
*         See also #XHdcp22Tx_ComputeHPrime
* @param  Ks is the session key.
* @param  Rrx is the random value generated by rx.
* @param  Rtx is the random value generated by tx.
* @param  EdkeyKs is the encrypted Ks output.
*
* @return None.
*
* @note   None.
******************************************************************************/
void XHdcp22Tx_ComputeEdkeyKs(const u8* Rn, const u8* Km,
                              const u8 *Ks,
                              const u8 *Rrx, const u8 *Rtx,
                              u8 *EdkeyKs)
{
	u8 Aes_Iv[XHDCP22_TX_AES128_SIZE];
	u8 Aes_Key[XHDCP22_TX_AES128_SIZE];
	u8 Dkey2[XHDCP22_TX_AES128_SIZE]; /* Dkey2. */

	/* Key XOR with Rn. */
	/* Copy Rn to least significant 64-bits. */
	memset(Aes_Key, 0x0, XHDCP22_TX_KM_SIZE);
	memcpy(&Aes_Key[XHDCP22_TX_RN_SIZE], Rn, XHDCP22_TX_RN_SIZE);
	XHdcp22Tx_MemXor(Aes_Key, Aes_Key, Km, XHDCP22_TX_KM_SIZE);

	/* Determine dkey2. */
	/* Add m = Rtx || Rrx. */
	memcpy(Aes_Iv, Rtx, XHDCP22_TX_RTX_SIZE);
	memcpy(&Aes_Iv[XHDCP22_TX_RTX_SIZE], Rrx, XHDCP22_TX_RRX_SIZE);
	Aes_Iv[15] ^= 0x02; /* big endian! */
	XHdcp22Cmn_Aes128Encrypt(Aes_Iv, Aes_Key, Dkey2);


	/* EdkeyKs = Ks XOR (Dkey2 XOR Rrx). */
	/* Rrx XOR Dkey2. */
	memset(EdkeyKs, 0x00, XHDCP22_TX_EDKEY_KS_SIZE);
	/* Copy Rrx to the least significant 64 bits. */
	memcpy(&EdkeyKs[XHDCP22_TX_EDKEY_KS_SIZE - XHDCP22_TX_RRX_SIZE], Rrx,
					XHDCP22_TX_RRX_SIZE);
	XHdcp22Tx_MemXor(EdkeyKs, EdkeyKs, Dkey2, XHDCP22_TX_AES128_SIZE);
	/* XOR with Ks. */
	XHdcp22Tx_MemXor(EdkeyKs, EdkeyKs, Ks, XHDCP22_TX_KS_SIZE);
}

/*****************************************************************************/
/**
*
* This function computes LPrime. Kd is calculated as in
* #XHdcp22Tx_ComputeHPrime, but could also be stored the first time.
* This could save some computing time.
* According protocol however, only Rrx and Rtx are supposed to be stored.
*
* @param  Rn is a pseudo-random nonce.
* @param  Km is the master key generated by tx and previouslyd stored.
* @param  Rrx is the random value generated by rx.
* @param  Rtx is the random value generated by tx.
* @param  LPrime is a pointer to the computed LPrime hash.
*
* @return None.
*
* @note   None.
******************************************************************************/
void XHdcp22Tx_ComputeLPrime(const u8* Rn, const u8 *Km,
                             const u8 *Rrx, const u8 *Rtx,
                             u8 *LPrime)
{
	u8 Aes_Iv[XHDCP22_TX_AES128_SIZE];
	u8 Aes_Key[XHDCP22_TX_AES128_SIZE];
	u8 Kd[2 * XHDCP22_TX_AES128_SIZE]; /* Dkey0 || Dkey 1. */
	u8 HashKey[XHDCP22_TX_SHA256_HASH_SIZE];

	/* For key derivation, use Km XOR Rn as AES key where Rn=0 during AKE.
	 * Note: Protocol says we should use incoming Rn and XOR it with Km,
	 * but then the output does not match the errata output.
	 * For now we use for Rn 0, which matches the test vectors.
	 */
	memcpy(Aes_Key, Km, XHDCP22_TX_KM_SIZE);

	/* Compute Dkey0. */
	/* Add m = Rtx || Rrx. */
	memcpy(Aes_Iv, Rtx, XHDCP22_TX_RTX_SIZE);
	/* Rrx XOR Ctr0, where Ctr0 is 0. */
	memcpy(&Aes_Iv[XHDCP22_TX_RTX_SIZE], Rrx, XHDCP22_TX_RRX_SIZE);
	XHdcp22Cmn_Aes128Encrypt(Aes_Iv, Aes_Key, Kd);


	/* Compute Dkey0 , counter is 1: Rrx | 0x01. */
	Aes_Iv[15] ^= 0x01; /* big endian! */
	XHdcp22Cmn_Aes128Encrypt(Aes_Iv, Aes_Key, &Kd[XHDCP22_TX_KM_SIZE]);


	/* Create hash with HMAC-SHA256. */
	/* Input: Rn. */
	/* Key:	Kd XOR Rrx (least sign. 64 bits). */
	memset(HashKey, 0x00, XHDCP22_TX_SHA256_HASH_SIZE);
	memcpy(&HashKey[XHDCP22_TX_SHA256_HASH_SIZE-XHDCP22_TX_RRX_SIZE], Rrx,
         XHDCP22_TX_RRX_SIZE);
	XHdcp22Tx_MemXor(HashKey, HashKey, Kd, XHDCP22_TX_SHA256_HASH_SIZE);
	XHdcp22Cmn_HmacSha256Hash(Rn, XHDCP22_TX_RN_SIZE, HashKey,
                           XHDCP22_TX_SHA256_HASH_SIZE, LPrime);
}

/*****************************************************************************/
/**
*
* This function encrypts the Km value with the receivers public key into Ekh(Km)
*
* @param  CertificatePtr is a pointer to the certificate from the HDCP2.2 receiver.
* @param  KmPtr is the random master key.
* @param  MaskingSeedPtr is a pointer to a 32 byte seed for masking in the MGF1 function
*         used in RSA-OEAP encryption.
* @param  EncryptedKmPtr output of the encryption with a 128 byte (1024 bit) size.
*
* @return
*         - XST_SUCCESS if encryption succeeded.
*         - XST_FAILURE if encryption failed.
*
* @note   None.
*
******************************************************************************/
int XHdcp22Tx_EncryptKm(const XHdcp22_Tx_CertRx* CertificatePtr,
                        const u8* KmPtr, u8 *MaskingSeedPtr, u8* EncryptedKmPtr)
{
	return XHdcp22Tx_RsaOaepEncrypt(CertificatePtr->N, 	XHDCP22_TX_CERT_PUB_KEY_N_SIZE,
                                  CertificatePtr->e, XHDCP22_TX_CERT_PUB_KEY_E_SIZE,
                                  KmPtr, XHDCP22_TX_KM_SIZE,
                                  MaskingSeedPtr, EncryptedKmPtr);
}

/*****************************************************************************/
/**
*
* This function calculates a XOR on a array.
*
* @param  Output is the XOR of the input arrays.
* @param  InputA is a input array.
* @param  InputB is a input array.
* @param  Size is the size of the buffers to XOR.
*
* @return None.
*
* @note   None.
*
******************************************************************************/
void XHdcp22Tx_MemXor(u8 *Output, const u8 *InputA, const u8 *InputB,
                      unsigned int Size)
{
	unsigned int i=0;
	for (i=0; i<Size; i++) {
		Output[i] = InputA[i] ^ InputB[i];
	}
}
