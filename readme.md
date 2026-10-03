# lr-core-vector

用 C 语言手写一个 `std::vector`：你要做的事只有一件：根据 `include/vector.h` 的描述**把 `src/vector.c` 里的空壳函数填成能用的实现，让 `make test` 全绿**。

[vector 原理可视化](https://lingrui-studio.github.io/vector-playground/)

本题实现的是只存储 `int` 的教学版动态数组，具体约定以 [include/vector.h](include/vector.h) 为准。

## 目录结构

```
lr-core-vector/
├── readme.md         本文件
├── Makefile          构建脚本（不用改）
├── .gitignore        列举 git 需要忽视的文件
├── .clang-format     格式化要求
├── include/
│   └── vector.h      接口声明 + 函数注释（不用改，但要读懂）
├── src/
│   └── vector.c      ★ 你要实现的地方
└── tests/
    └── test.c        单元测试（不用改）
```

## 自检

补全 [include/vector.c](include/vector.c) 中的函数实现后，项目根目录运行 `make test`，若最后输出结果如下即表示你完成了本项目（本项目只有未完成和已完成两种状态，不存在中间值）：

```bash
== 通过 3393 项，失败 0 项 ==
全部通过，可以 commit & push 了
```

测试始终开启 ASan + UBSan，检测到错误会以失败状态退出，不提供关闭开关。常见错误会被直接指出来，例如：

```
ERROR: AddressSanitizer: heap-buffer-overflow on address 0x... at pc 0x...
READ of size 4 at 0x... thread T0
    #0 0x... in get src/vector.c:52
```

行号会直接指到出问题的那一行，看不懂的把完成代码和报错信息复制给 AI 问一下。

## 提交

- 完成下面的`实现思路`一节，简要说明你的各个函数是如何实现的，尤其注意内存管理的说明
- 把所有修改 commit 并 push 到 GitHub 上自己的 vector 仓库
- 在个人仓库的 Actions 页面手动触发一次自动评分工作流

## 实现思路

init：先判断容量是否为零或过大，然后给v->data一块内存统一管理，然后根据end，cap与data的相对关系移动指针
destroy：先通过data释放内存（init时确认过以data进行管理），再清除指针（设为NULL）
size、capacity：二者同理，先查v、v->data不为空指针，防止后面两个NULL相减。然后指针相减得到数据。
empty：通过size（v）运算
get：index与end的移动类似，都是相对于data。index是将指针移动到数组内的一个位置。检查：v非空，index小于size
set：将get的实现转化为设置数
front、back：先判断是否为空（empty），再取数
push_back:先做基本检查：v非空，然后分四种情况：直接加，从零开始加（此时data尚未获取内存，根据题目要求，得在此处实现一个类似init的函数），加多了而退出，reserve成功再push_back四种情况。
pop_back:直接移动end指针就行，end以及后面的空间逻辑无效
reserve：先判断是否增加（reserve只增不减），前文所述“加多了退出”也在此判断，然后通过realloc函数给data分配内存，并把end，cap的值进行修正
shrink_to_fit:本质上和reserve相同，都是给data一块新的内存空间，但此处需要把size（v）==0单独拎出来，因为此处与destroy效果相同。
clear：移动end就行
