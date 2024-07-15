

### `std::vector` 常用操作和方法

1. **创建和初始化**

   ```cpp
   vector<int> v1; // 创建一个空的 vector
   vector<int> v2(10); // 创建一个包含 10 个默认值元素的 vector
   vector<int> v3(10, 5); // 创建一个包含 10 个值为 5 的元素的 vector
   vector<int> v4 = {1, 2, 3, 4, 5}; // 使用初始化列表创建 vector
   ```

2. **访问元素**

   ```cpp
   v1[0]; // 访问第一个元素
   v1.at(0); // 访问第一个元素，带边界检查
   v1.front(); // 访问第一个元素
   v1.back(); // 访问最后一个元素
   ```

3. **添加和删除元素**

   ```cpp
   v1.push_back(1); // 在末尾添加一个元素
   v1.pop_back(); // 删除末尾的元素
   v1.insert(v1.begin(), 5); // 在指定位置插入元素
   v1.erase(v1.begin()); // 删除指定位置的元素
   v1.clear(); // 清空所有元素
   ```

4. **大小和容量**

   ```cpp
   v1.size(); // 返回当前元素的数量
   v1.capacity(); // 返回当前分配的容量
   v1.resize(20); // 改变大小，如果新大小大于当前大小，用默认值填充新元素
   v1.reserve(50); // 预留至少能容纳 50 个元素的空间
   ```

5. **迭代器**

   ```cpp
   for (auto it = v1.begin(); it != v1.end(); ++it) {
       cout << *it << " ";
   }

   for (int x : v1) { // 使用范围 for 循环
       cout << x << " ";
   }
   ```

6. **其他方法**

   ```cpp
   v1.empty(); // 检查是否为空
   v1.swap(v2); // 交换两个 vector 的内容
   ```

这些是 `std::vector` 的一些常用方法和操作，熟练掌握这些基本操作可以帮助你更高效地使用 `std::vector`。