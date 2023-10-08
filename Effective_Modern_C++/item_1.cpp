int main()
{
  // auto pp = new Point();

  auto pp = new Point[2]();
  {
    // 以下代码(需要在windows x64系统下执行)：
    cout << "pp = " << (void *)pp << endl;
    void *psz = (size_t *)pp - 1; // psz指向存放堆空间数组大小的存放位置
    cout << "psz = " << (void *)psz << endl;
    cout << "sz = " << *(long long *)psz << endl;
  }
  pp[0].print();
  pp[1] = Point(3, 4);
  pp[1].print();
  delete[] pp;
}
