## using相比typename的优势
`using` 声明可以被模板化（这种情况下称为**别名模板** `alias templates` ）但是 `typedef` 不能 （C++11程序员可以很直接的表达一些东西，而C++98中只能把 `typedef` 嵌套进模板化的 `struct` 才能表达这些东西）。

C++11中通过添加头文件 `<type_traits>` 可以进行 `type traits`（类型萃取）。(通过**类型萃取**可以改变类型的属性)。