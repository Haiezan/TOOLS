from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import os

# 密钥（必须是16、24、32字节的长度）
KEY = b'Sixteen byte key'

# 初始化向量（IV，16字节）
IV = os.urandom(16)

# 加密函数
def encrypt_file(input_file, output_file):
    # 打开输入文件并读取内容
    with open(input_file, 'rb') as f:
        data = f.read()

    # 创建AES加密器
    cipher = AES.new(KEY, AES.MODE_CBC, IV)

    # 对数据进行填充，以符合AES块大小（16字节）
    encrypted_data = cipher.encrypt(pad(data, AES.block_size))

    # 将加密的内容和IV一起保存（IV保存在文件开头）
    with open(output_file, 'wb') as f:
        f.write(IV)  # 写入IV
        f.write(encrypted_data)  # 写入加密后的数据

# 解密函数
def decrypt_file(input_file, output_file):
    # 打开加密文件并读取IV与加密数据
    with open(input_file, 'rb') as f:
        iv = f.read(16)  # 读取IV（前16字节）
        encrypted_data = f.read()  # 读取加密后的数据

    # 创建AES解密器
    cipher = AES.new(KEY, AES.MODE_CBC, iv)

    # 解密数据并去除填充
    decrypted_data = unpad(cipher.decrypt(encrypted_data), AES.block_size)

    # 将解密后的数据写入输出文件
    with open(output_file, 'wb') as f:
        f.write(decrypted_data)

# 示例：加密文件
encrypt_file('0.txt', '1.bin')

# 示例：解密文件
#decrypt_file('1.bin', '00.txt')

print("En完成")
