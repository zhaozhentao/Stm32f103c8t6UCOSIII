from PIL import Image

def binarize_image(img, threshold=128):
    """
    对图片执行灰度化 + 二值化
    threshold: 二值化阈值，0~255
    """
    img = img.convert("L")  # 灰度
    # 二值化
    img = img.point(lambda p: 255 if p > threshold else 0, "1")
    return img

def image_to_oled_bytes(img, width=128, height=64):
    """
    将二值化后的图片转换成 OLED 页模式数组
    每字节竖向 8 像素
    """
    img = img.resize((width, height))
    pages = height // 8
    oled_bytes = []

    for page in range(pages):
        for x in range(width):
            byte = 0
            for bit in range(8):
                y = page * 8 + bit
                pixel = img.getpixel((x, y))
                # 0 = 黑点 → 点亮（如果你的屏幕是反色可以反转这里）
                if pixel == 0:
                    byte |= (1 << bit)
            oled_bytes.append(byte)

    return oled_bytes

def generate_c_array(data, name="IMAGE"):
    """
    格式化为 C 语言数组
    """
    lines = []
    for i in range(0, len(data), 16):
        part = ", ".join(f"0x{b:02X}" for b in data[i:i+16])
        lines.append("    " + part)
    return f"const unsigned char {name}[] = {{\n" + ",\n".join(lines) + "\n};"

if __name__ == "__main__":
    input_file = "/Users/zhaozhentao/Desktop/舒服.png"              # 源图片
    bin_file = "binarized.png"            # 输出的二值化图片
    output_c = "oled_image.c"             # 输出的取模结果
    threshold = 128                        # 二值化阈值(可调)

    # 1. 打开图片
    img = Image.open(input_file)

    # 2. 二值化
    bin_img = binarize_image(img, threshold)

    # 3. 保存二值化图像
    bin_img.save(bin_file)
    print(f"[OK] 二值化图片已输出：{bin_file}")

    # 4. 转换成 OLED 数据
    oled_bytes = image_to_oled_bytes(bin_img)

    # 5. 写入 C 数组文件
    c_code = generate_c_array(oled_bytes, name="my_image")
    with open(output_c, "w") as f:
        f.write(c_code)

    print(f"[OK] 取模数组已生成：{output_c}")
    print("[DONE] 完成！")

