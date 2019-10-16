# C89 词法分析器

- author: FredericDT <frederic.dt.twh@gmail.com>

## 运行开发环境描述

- kernel 4.9.184
- cmake version 3.10.2
- gcc/g++ 7.4.0
- git 2.17.1

## 设计思路

本方案建议使用自动机进行判断，google/re2 采用纯自动机进行字符串匹配，可以使用正则表达式描述自动机，采取贪心搜索方式，所有可能的正则匹配结果中取最长，则为正确匹配的结果。
根据最长匹配的正则表达式对应的类型，将匹配结果存入词属性。

## 实现方案

- 采用 cxxopts 解析 argc, argv 并构造优雅的参数提示，将解析结果存入结构化逻辑存储中
- 采用 re2 构造自动机进行分析，贪心模式下最长结果即最佳匹配结果
- 采用 cmake 进行 include 及包管理，使得本代码可以在 gnu c++ 环境下得以复现
- 采用 doxygen 生成美观实用的 aqi 文档，根据代码中编写的类 javadoc 注释块生成
- 遵守 c89 标准，最终成果可以完成 kernel 早期版本的词法分析

## 测试方法

1. 样例 1

`./src/c89_lexical_parse -f linux-0.11/mm/memory.c`

stdout 中应正确打印 memory.c 的词法分析结果

2. 样例 2

`./src/c89_lexical_parse -f linux-0.11/mm/memory.c -a -o tmp.txt`

工作目录下 tmp.txt 中应有 memory.c 的词法分析结果，stdout 应有 memory.c 的词法分析结果。

3. 样例 3

修改 memory.c，例如在 168 行 `from_page_table` 改为 `f"rom_page_table`

`./src/c89_lexical_parse -f linux-0.11/mm/memory.c`

stdout 应输出错误出现位置，本例中为 168:3 

4. 样例 4

`./src/c89_lexical_parse -f linux-0.11/mm/memory.c -c`

stdout 应正确打印未忽略注释的 token 结果

5. 样例 5

`rsync -a --include='*/' --exclude='*' linux-0.11 test/resources/`
`find linux-0.11 -type f | grep "\.*\.c" | xargs -I {} sh -c "echo {} && ./src/c89_lexical_parse -f {} -o test/resources{}.tokens"`

所有文件应能正确分析，特别注意，需要提前创建目录,此处 rsync 及 find - grep - xargs 仅供参考，若要完成批处理，需要自行修改上述 shell
