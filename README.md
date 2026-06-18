# 🌲 AVL Map (Custom std::map-like Container)

A custom implementation of an associative container based on a self-balancing AVL tree, inspired by `std::map`.

This project demonstrates low-level data structure design, iterator implementation, and modern C++ memory semantics.

---

## 🚀 Features

* 🌲 Self-balancing AVL tree
* 🔍 Logarithmic complexity for insert / erase / find
* 🔁 Bidirectional iterators (`Iterator`, `ConstIterator`)
* 📦 STL-like interface
* ⚙️ Copy & Move semantics
* 🔗 Parent pointers for efficient traversal
* 🧠 Operator overloading (`operator[]`)
* 🧹 Automatic memory management (RAII)

---

## 🏗️ Architecture

### Core Components

* **Node**

  * Stores key-value pair
  * Maintains height for balancing
  * Tracks parent for iterator navigation

* **AVL Balancing**

  * Left / Right rotations
  * Balance factor tracking
  * Height updates after each modification

* **Iterators**

  * In-order traversal
  * Bidirectional (`++`, `--`)
  * STL-style interface

---

## ⚙️ Complexity

| Operation | Complexity |
| --------- | ---------- |
| Insert    | O(log n)   |
| Erase     | O(log n)   |
| Find      | O(log n)   |
| Iterate   | O(n)       |

---

## 🧠 Key Concepts Demonstrated

* Tree rotations (LL, RR, LR, RL)
* Memory management with raw pointers
* Copy / Move constructors and assignment
* Iterator design (including const correctness)
* Recursive tree algorithms
* Parent-linked traversal logic

---

## ▶️ Example Usage

```cpp
Map<int, std::string> mp;

mp.insert(10, "ten");
mp.insert(5, "five");
mp.insert(20, "twenty");

// operator[]
mp[15] = "fifteen";

// iteration
for (auto it = mp.begin(); it != mp.end(); ++it)
{
    auto [key, value] = *it;
    std::cout << key << " -> " << value << std::endl;
}

// erase
mp.erase(10);
```

---

## 🔄 Iterator Example

```cpp
for (auto it = mp.begin(); it != mp.end(); ++it)
{
    std::cout << (*it).first << " -> " << (*it).second << std::endl;
}
```

---

## 📌 Design Decisions

* AVL tree chosen for strict balancing guarantees
* Parent pointers used for efficient iterator traversal
* Custom iterator implementation instead of STL reuse
* Explicit memory control (no STL containers internally)

---

## ⚠️ Limitations

* No allocator support (unlike `std::map`)
* Exception safety can be improved
* Iterator `operator->` requires refinement
* Not fully STL-compliant

---

## 🔮 Future Improvements

* Add allocator support
* Improve exception safety guarantees
* Full STL compatibility
* Add unit tests
* Benchmark vs `std::map`

---

## 📊 Educational Value

This project was built to deeply understand:

* How associative containers work internally
* How iterators are implemented
* How balancing trees maintain performance guarantees

---

## 👨‍💻 Author

Anatolii K.
C++ Developer (Systems / Low-level)

GitHub:
https://github.com/Anatolijhack

---

## 💡 Notes

This is a learning-focused implementation aimed at understanding core concepts behind STL containers rather than replacing them in production.

