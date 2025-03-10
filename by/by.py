import argparse
from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad
import os
import sys

# 密钥（必须是16、24、32字节的长度）
KEY = b'Sixteen byte key'

# 初始化向量（IV，16字节）
IV = os.urandom(16)

# 加密函数
def encrypt_file(input_file, output_file, key):
    # 打开输入文件并读取内容
    with open(input_file, 'rb') as f:
        data = f.read()

    # 创建AES加密器
    cipher = AES.new(key, AES.MODE_CBC, IV)

    # 对数据进行填充，以符合AES块大小（16字节）
    encrypted_data = cipher.encrypt(pad(data, AES.block_size))

    # 将加密的内容和IV一起保存（IV保存在文件开头）
    with open(output_file, 'wb') as f:
        f.write(IV)  # 写入IV
        f.write(encrypted_data)  # 写入加密后的数据

# 解密函数
def decrypt_file(input_file, output_file, key):
    # 打开加密文件并读取IV与加密数据
    with open(input_file, 'rb') as f:
        iv = f.read(16)  # 读取IV（前16字节）
        encrypted_data = f.read()  # 读取加密后的数据

    # 创建AES解密器
    cipher = AES.new(key, AES.MODE_CBC, iv)

    # 解密数据并去除填充
    decrypted_data = unpad(cipher.decrypt(encrypted_data), AES.block_size)

    # 将解密后的数据写入输出文件
    with open(output_file, 'wb') as f:
        f.write(decrypted_data)

# 获取用户输入的密钥，确保长度合法
def get_user_key():
    while True:
        key = input("input keys (16/24/32):").encode('utf-8')
        if len(key) in (16, 24, 32):
            return key
        print("must be 16/24/32!")

# 获取用户输入
def get_user_input():
    # 选择操作类型
    while True:
        mode = input("Please select (1-en, 2-de): ").strip()
        if mode == '1':
            mode = 'encrypt'
            break
        elif mode == '2':
            mode = 'decrypt'
            break
        else:
            print("Error!")

    # 选择输入文件路径
    input_files = []
    while True:
        file_path = input("Input file: ").strip()
        if os.path.isfile(file_path):
            input_files.append(file_path)
            break
        else:
            print("Error!")

    # 选择输出目录
    output_folder = input("Output file：").strip()

    # 获取密钥
    key = get_user_key()

    return mode, input_files, output_folder, key

# 主程序
def main():
    print("Welcome!")
    mode, input_files, output_folder, key = get_user_input()

    # 执行加密或解密
    if mode == 'encrypt':
        for file in input_files:
            encrypt_file(file, output_folder, key)
    elif mode == 'decrypt':
        for file in input_files:
            decrypt_file(file, output_folder, key)

    print("Completed, 3Q!")

if __name__ == "__main__":
    main()


# 示例：加密文件
# encrypt_file('0.txt', '1.bin')

# 示例：解密文件
# decrypt_file('1.bin', '00.txt')

# print("En完成")
