import os
def replace_field_in_file(oldfile_path = str,  old_text = list, newfile_path = str, new_text = list):
    # 打开文件并读取内容
    with open(oldfile_path, 'r',encoding='utf-8') as file:
        content = file.read()
    # # 使用replace()方法或正则表达式替换字段
    for i in range(len(old_text)) :
        content = content.replace(old_text[i], new_text[i])

    # 将替换后的内容写回文件中
    with open(newfile_path, 'w',encoding='utf-8') as file:
        file.write(content)

# 替换.v文件中指令寄存器 $readmem 路径
dir_path = os.path.dirname(__file__);
old_text = ['`AD']
new_text = [dir_path + "/build/add.v"]
oldfile_path  = './test_v/test_rom.v'
newfile_path  = './test_v/rom.v'
replace_field_in_file(oldfile_path, old_text, newfile_path, new_text)

# 测试 v文件

