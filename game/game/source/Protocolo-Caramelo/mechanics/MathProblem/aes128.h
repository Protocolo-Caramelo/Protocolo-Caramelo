#ifndef AES128_H
#define AES128_H

/*
 * aes128.h - Implementação mínima de AES-128 no modo CTR.
 *
 * O modo CTR transforma o AES em uma cifra de fluxo: não precisa de
 * padding e a operação de cifrar é idêntica à de decifrar. Por isso,
 * só a função de cifrar bloco do AES precisa ser implementada.
 */

#include <stddef.h>
#include <stdint.h>

#define AES128_KEY_SIZE   16
#define AES128_BLOCK_SIZE 16

/*
 * Cifra ou decifra `length` bytes de `input` e grava em `output`
 * (podem ser o mesmo buffer). Chamar duas vezes com a mesma chave e o
 * mesmo `iv` devolve o texto original.
 *
 * `iv` é o bloco inicial do contador; use um valor diferente a cada
 * mensagem cifrada com a mesma chave.
 */
void AES128_CtrCrypt(const uint8_t key[AES128_KEY_SIZE],
                     const uint8_t iv[AES128_BLOCK_SIZE],
                     const uint8_t *input,
                     uint8_t *output,
                     size_t length);

#endif /* AES128_H */
