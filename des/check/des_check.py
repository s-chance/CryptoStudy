from Crypto.Cipher import DES
# from Crypto.Util.Padding import pad, unpad
from binascii import hexlify, unhexlify

key = unhexlify('0123456789ABCDEF')
plaintext = unhexlify('0123456789ABCDEF')

cipher = DES.new(key, DES.MODE_ECB)
# ciphertext = cipher.encrypt(pad(plaintext, DES.block_size))
ciphertext = cipher.encrypt(plaintext)
print("密文: ", hexlify(ciphertext).decode())

cipher = DES.new(key, DES.MODE_ECB)
# decrypted = unpad(cipher.decrypt(ciphertext), DES.block_size)
decrypted = cipher.decrypt(ciphertext)
print("明文: ", hexlify(decrypted).decode())