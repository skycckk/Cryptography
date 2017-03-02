import hashlib
import os

def get_md5(file_path):
    with open(file_path, "rb") as binary_file:
        # Read the whole file at once
        data = binary_file.read()

    m = hashlib.md5()
    m.update(data)
    return m.hexdigest()

def md5_cmp(file_path1, file_path2, file_path1a, file_path2a):
    with open(file_path1, "rb") as binary_file:
        # Read the whole file at once
        data1 = binary_file.read()

    with open(file_path2, "rb") as binary_file:
        # Read the whole file at once
        data2 = binary_file.read()

    data3 = bytearray(data1)
    data4 = bytearray(data2)

    target_name = 'Stamp'
    target_name_new = 'Huang'
    for i in range(len(data1) - len(target_name)):

        target_found = True
        for j in range(len(target_name)):
            if data3[i + j] != ord(target_name[j]):
                target_found = False
                break

        if target_found:
            # replace the string
            for j in range(len(target_name)):
                data3[i + j] = ord(target_name_new[j])

        target_found = True
        for j in range(len(target_name)):
            if data4[i + j] != ord(target_name[j]):
                target_found = False
                break

        if target_found:
            # replace the string
            for j in range(len(target_name)):
                data4[i + j] = ord(target_name_new[j])

    with open(file_path1a, "wb") as binary_file:
        binary_file.write(data3)

    with open(file_path2a, "wb") as binary_file:
        binary_file.write(data4)


file_1 = "../MD5_collision/rec2.ps"
file_2 = "../MD5_collision/auth2.ps"
file_1_md5 = get_md5(file_1)
file_2_md5 = get_md5(file_2)
if file_1_md5 == file_2_md5:
    indentical_md5 = True
else:
    indentical_md5 = False
print("rec2.ps  MD5:", file_1_md5)
print("auth2.ps MD5:", file_2_md5)
print("identical:", indentical_md5)

file_1a = "../MD5_collision/rec2_new.ps"
file_2a = "../MD5_collision/auth2_new.ps"
md5_cmp(file_1, file_2, file_1a, file_2a)
file_1a_md5 = get_md5(file_1a)
file_2a_md5 = get_md5(file_2a)
if file_1a_md5 == file_2a_md5:
    indentical_md5 = True
else:
    indentical_md5 = False
print("rec2_new.ps  MD5:", file_1a_md5)
print("auth2_new.ps MD5:", file_2a_md5)
print("identical:", indentical_md5)    
