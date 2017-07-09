/* Examples */
#include "../lib/lib.h"

int main() {
  EC_KEY *ec_key = NULL;
  EC_KEY *eck2 = NULL;
  const BIGNUM *prv = NULL;
  const EC_POINT *pub = NULL;
  unsigned char msg[33] = {0};
  unsigned char prv_str[33] = {0};
  unsigned int digest_len = 0;

  // Test Random Bit String
  random_256bit_string(msg);
  printf("Getting random 256bit string:");
  for (int i = 0; i < 32; i++)
       printf("%02x", msg[i]);
  printf("\n\n");


  // Generat a private and public key pair
  printf("Generating a private and public key pair using init_priv_pub_key_pair()...\n");
  ec_key = init_priv_pub_key_pair();
  const unsigned char *addr = NULL; 

  if ((prv = get_private_key(ec_key)) == NULL) {
    printf("Unable to get private key\n");
    exit(-1);
  }

  BN_bn2bin(prv, prv_str);
  printf("private key 1 %s\npublic key 1: %s\n", BN_bn2hex(prv), pub_key_hex(ec_key));
  
  printf("\n Generating a public key from randomly chose 256-bit string...\n");
  /* Generate Public Key from private key */
  eck2 = gen_pub_key_from_priv_key(msg);

  if ((prv = get_private_key(eck2)) == NULL) {
    printf("Unable to get private key 2\n");
    exit(-1);
  }

  if ((pub = get_public_key(eck2)) == NULL) {
    printf("Unable to get public key 2\n");
    exit(-1);
  }

  printf("private key 2 %s\n public key2: %s\n\n", BN_bn2hex(prv), pub_key_hex(eck2));

  /* Get Address of Wallet */
  // get address
  printf("Generating wallet address using supplied public key...\n");
  addr = mget_address(ec_key, &digest_len);

  printf("Address: \n");
  for (int i = 0; i < digest_len; i++) {
       printf("%02x", addr[i]);
  }
  printf("\nAddress Length is: %d\n", digest_len);
 
  // print checksum of address
  printf("Printing base58 checksum of address. This will loop several times to see if output repeats as it should\n");
  unsigned char *checksum = NULL;
  // Base58Check encode
  for (int i = 0; i < 2; i++) {
    checksum = mbase58EncodeChecksum(0, addr, digest_len, 4);
    printf("checksum 1: ");
    for (int i = 0; i < 4; i++)
         printf("%02x", checksum[i]);
    printf("\nchecksum 2: ");
    checksum = mbase58EncodeChecksum(0, addr, digest_len, 4);
    for (int i = 0; i < 4; i++)
         printf("%02x", checksum[i]);
    printf("\n");
  }

  printf("\n");
  int offset = 0;

  // Base 58 Encoding
  printf("base58Encoding test ... \n");
  unsigned char h[] = "hello";
  const unsigned char *hash = mb58Encode(h, 5, &offset);//addr, digest_len, &b58l);
  printf("hello in b58: %s\n", hash + offset);

  hash = mb58Encode(addr, digest_len, &offset);
  printf("addr in b58: %s\n", hash + offset);

  // Decoding
  printf("Test decoding Base58...\n"); 
  int ret_len = 0;
  unsigned char *hp = mbase58Decode(hash + offset, strlen(hash + offset), &ret_len);   
  printf("Size of Address mbase58 Decoded string is: %d\n", ret_len);

  // Decoded Address
  printf("Decoded Addres:\n");
  for (int i = 0; i < ret_len; i++) {
         printf("%02x", hp[i]);
   }
  printf("\n");
  
  free(hp);
  free(hash);
  return 0;
}