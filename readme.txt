HexProbe 十六进制浏览器
作者：RevoLC build: 7

【编译】
对于 Windows 用户，点击 make.bat 即可生成 hexprobe.exe 文件。
对于 Linux / macOS 用户，请参照 make.bat 的内容自行调用 g++ 编译。

【使用】
hexprobe 文件名 <--start/-s 起始偏移> <--end/-e 终止偏移>
hexprobe 文件名 <--start/-s 起始偏移> <--length/-of 区间长度>

参数含义
--start/-s              程序从起始偏移开始读取数据。起始偏移为十六进制数，接受 0x 前缀。
--end/-e                程序读取数据到终止偏移为止停止。终止位移为十六进制数，接受 0x 前缀。
                        不能与 --length/-l 参数同时使用。
--length/-l             程序读取给定长度的数据。区间长度为十进制数。
                        不能与 --end/-e 参数同时使用。

注意事项
除了文件名，所有参数都是可选的。
起始偏移默认为 0, 区间长度默认为 256. 如果起始偏移距离文件末尾的距离小于 256, 取这段距离。
区间长度不能超过 1024.

【示例】
假设在同级目录下有一个 test.cab 文件。

hexprobe test.cab 含义：打开 test.cab, 输出偏移 0x0000 之后的 256 个字节。
hexprobe test.cab --start 0x0040 含义：打开 test.cab, 输出偏移 0x0040 之后的 256 个字节。
hexprobe test.cab --start 0x0040 --end 0x007F 含义：打开 test.cab, 输出偏移 0x0040 到 0x007F 间的所有字节。（包括 0x007F）
hexprobe test.cab --start 0x0040 --length 512 含义：打开 test.cab, 输出偏移 0x0040 之后的 512 个字节。

【AI 辅助编程使用揭示】
这个程序的大部分基础架构由我独自完成。在以下函数使用了 AI 辅助。
使用工具为 Codex, 连接了 GPT 系模型。
 - convert_hex (main.cpp)
 - convert_dec (main.cpp)
 - is_file_exist (hexprobe.cpp)
 - get_file_size (hexprobe.cpp)
 - read_file (hexprobe.cpp)
 - print_hex (display.cpp)
 - print_byte (display.cpp)