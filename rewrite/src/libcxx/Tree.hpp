// std::__tree (std::map / std::set) as the NDK's libc++ compiles it, for the instances over the game's key and value
// types. Layout (32-bit): begin node +0, end node +4 (its left child is the root), size +8, a comparator object +0xC
// when it has state (a function pointer). A node: left +0, right +4, parent +8, is_black (byte) +0xC, value +0x10
// (the key first). The algorithms are libc++ 3.5's; where the compiled code calls another instance member out of
// line (__find_equal, __insert_node_at, __construct_node, destroy, ...) the rewrite calls it too, by address.
#pragma once

#include "libcxx/Libcxx.hpp"

namespace libcxx::tree {

using rw::load;
using rw::save;

constexpr uint32_t BEGIN = 0, ROOT = 4, SIZE = 8, COMPARE = 0xC;
constexpr uint32_t LEFT = 0, RIGHT = 4, PARENT = 8, IS_BLACK = 0xC, VALUE = 0x10;

constexpr uint32_t BALANCE_AFTER_INSERT = 0x493F5B;   // std::__tree_balance_after_insert
constexpr uint32_t LEFT_ROTATE = 0x493FFB;            // std::__tree_left_rotate
constexpr uint32_t RIGHT_ROTATE = 0x49401F;           // std::__tree_right_rotate
constexpr uint32_t TREE_REMOVE = 0x4AF827;            // std::__tree_remove
constexpr uint32_t MEMCMP = 0x40EF2C;                 // memcmp (import)

// ------------------------------------------------------------------------------------------ node links
inline uint32_t left(uint32_t n) { return load<uint32_t>(n + LEFT); }
inline uint32_t right(uint32_t n) { return load<uint32_t>(n + RIGHT); }
inline uint32_t parent(uint32_t n) { return load<uint32_t>(n + PARENT); }
inline bool isBlack(uint32_t n) { return load<uint8_t>(n + IS_BLACK) != 0; }
inline void setBlack(uint32_t n, bool black) { save<uint8_t>(n + IS_BLACK, black); }
inline bool isLeftChild(uint32_t n) { return left(parent(n)) == n; }
inline uint32_t endNode(uint32_t t) { return t + ROOT; }

inline uint32_t treeMin(uint32_t x) {
    while (left(x)) x = left(x);
    return x;
}
inline uint32_t treeMax(uint32_t x) {
    while (right(x)) x = right(x);
    return x;
}
// the in-order successor / predecessor (++it / --it)
inline uint32_t treeNext(uint32_t x) {
    if (right(x)) return treeMin(right(x));
    while (!isLeftChild(x)) x = parent(x);
    return parent(x);
}
inline uint32_t treePrev(uint32_t x) {
    if (left(x)) return treeMax(left(x));
    while (isLeftChild(x)) x = parent(x);
    return parent(x);
}
// a leaf below x (left first)
inline uint32_t treeLeaf(uint32_t x) {
    for (;;) {
        if (left(x)) x = left(x);
        else if (right(x)) x = right(x);
        else return x;
    }
}

// ------------------------------------------------------------------------------------------ rebalancing
inline void leftRotate(uint32_t x) {
    const uint32_t y = right(x);
    save<uint32_t>(x + RIGHT, left(y));
    if (right(x)) save<uint32_t>(right(x) + PARENT, x);
    save<uint32_t>(y + PARENT, parent(x));
    if (isLeftChild(x)) save<uint32_t>(parent(x) + LEFT, y);
    else save<uint32_t>(parent(x) + RIGHT, y);
    save<uint32_t>(y + LEFT, x);
    save<uint32_t>(x + PARENT, y);
}

inline void rightRotate(uint32_t x) {
    const uint32_t y = left(x);
    save<uint32_t>(x + LEFT, right(y));
    if (left(x)) save<uint32_t>(left(x) + PARENT, x);
    save<uint32_t>(y + PARENT, parent(x));
    if (isLeftChild(x)) save<uint32_t>(parent(x) + LEFT, y);
    else save<uint32_t>(parent(x) + RIGHT, y);
    save<uint32_t>(y + RIGHT, x);
    save<uint32_t>(x + PARENT, y);
}

inline void callLeftRotate(uint32_t x) { rw::call(LEFT_ROTATE, {x}); }
inline void callRightRotate(uint32_t x) { rw::call(RIGHT_ROTATE, {x}); }

// x was just linked under the tree's root: recolour / rotate back to a red-black tree
inline void balanceAfterInsert(uint32_t root, uint32_t x) {
    setBlack(x, x == root);
    while (x != root && !isBlack(parent(x))) {
        if (isLeftChild(parent(x))) {
            const uint32_t y = right(parent(parent(x)));
            if (y && !isBlack(y)) {
                x = parent(x);
                setBlack(x, true);
                x = parent(x);
                setBlack(x, x == root);
                setBlack(y, true);
            } else {
                if (!isLeftChild(x)) {
                    x = parent(x);
                    callLeftRotate(x);
                }
                x = parent(x);
                setBlack(x, true);
                x = parent(x);
                setBlack(x, false);
                callRightRotate(x);
                break;
            }
        } else {
            const uint32_t y = left(parent(parent(x)));
            if (y && !isBlack(y)) {
                x = parent(x);
                setBlack(x, true);
                x = parent(x);
                setBlack(x, x == root);
                setBlack(y, true);
            } else {
                if (isLeftChild(x)) {
                    x = parent(x);
                    callRightRotate(x);
                }
                x = parent(x);
                setBlack(x, true);
                x = parent(x);
                setBlack(x, false);
                callLeftRotate(x);
                break;
            }
        }
    }
}

// unlink z from the tree under root, then repair the colours
inline void treeRemove(uint32_t root, uint32_t z) {
    const uint32_t y = (!left(z) || !right(z)) ? z : treeNext(z);
    const uint32_t x = left(y) ? left(y) : right(y);
    uint32_t w = 0;
    if (x) save<uint32_t>(x + PARENT, parent(y));
    if (isLeftChild(y)) {
        save<uint32_t>(parent(y) + LEFT, x);
        if (y != root) w = right(parent(y));
        else root = x;
    } else {
        save<uint32_t>(parent(y) + RIGHT, x);
        w = left(parent(y));
    }
    const bool removedBlack = isBlack(y);
    if (y != z) {
        save<uint32_t>(y + PARENT, parent(z));
        if (isLeftChild(z)) save<uint32_t>(parent(y) + LEFT, y);
        else save<uint32_t>(parent(y) + RIGHT, y);
        save<uint32_t>(y + LEFT, left(z));
        save<uint32_t>(left(y) + PARENT, y);
        save<uint32_t>(y + RIGHT, right(z));
        if (right(y)) save<uint32_t>(right(y) + PARENT, y);
        setBlack(y, isBlack(z));
        if (root == z) root = y;
    }
    if (!removedBlack || !root) return;
    if (x) {
        setBlack(x, true);
        return;
    }
    for (;;) {
        if (!isLeftChild(w)) {
            if (!isBlack(w)) {
                setBlack(w, true);
                setBlack(parent(w), false);
                callLeftRotate(parent(w));
                if (root == left(w)) root = w;
                w = right(left(w));
            }
            if ((!left(w) || isBlack(left(w))) && (!right(w) || isBlack(right(w)))) {
                setBlack(w, false);
                const uint32_t p = parent(w);
                if (p == root || !isBlack(p)) {
                    setBlack(p, true);
                    return;
                }
                w = isLeftChild(p) ? right(parent(p)) : left(parent(p));
            } else {
                if (!right(w) || isBlack(right(w))) {
                    setBlack(left(w), true);
                    setBlack(w, false);
                    callRightRotate(w);
                    w = parent(w);
                }
                setBlack(w, isBlack(parent(w)));
                setBlack(parent(w), true);
                setBlack(right(w), true);
                callLeftRotate(parent(w));
                return;
            }
        } else {
            if (!isBlack(w)) {
                setBlack(w, true);
                setBlack(parent(w), false);
                callRightRotate(parent(w));
                if (root == right(w)) root = w;
                w = left(right(w));
            }
            if ((!left(w) || isBlack(left(w))) && (!right(w) || isBlack(right(w)))) {
                setBlack(w, false);
                const uint32_t p = parent(w);
                if (!isBlack(p) || p == root) {
                    setBlack(p, true);
                    return;
                }
                w = isLeftChild(p) ? right(parent(p)) : left(parent(p));
            } else {
                if (!left(w) || isBlack(left(w))) {
                    setBlack(right(w), true);
                    setBlack(w, false);
                    callLeftRotate(w);
                    w = parent(w);
                }
                setBlack(w, isBlack(parent(w)));
                setBlack(parent(w), true);
                setBlack(left(w), true);
                callRightRotate(parent(w));
                return;
            }
        }
    }
}

// ------------------------------------------------------------------------------------------ key kinds
// less(tree, a, b): the tree's comparator on two keys (addresses)
struct IntKey {
    static bool less(uint32_t, uint32_t a, uint32_t b) { return int32_t(load<uint32_t>(a)) < int32_t(load<uint32_t>(b)); }
};
struct UintKey {
    static bool less(uint32_t, uint32_t a, uint32_t b) { return load<uint32_t>(a) < load<uint32_t>(b); }
};
struct U64Key {
    static uint64_t value(uint32_t k) { return load<uint32_t>(k) | uint64_t(load<uint32_t>(k + 4)) << 32; }
    static bool less(uint32_t, uint32_t a, uint32_t b) { return value(a) < value(b); }
};
// less<string>: memcmp over the shorter length (always called), then the sizes
struct StringKey {
    static bool less(uint32_t, uint32_t a, uint32_t b) {
        const bool aLong = load<uint8_t>(a) & 1, bLong = load<uint8_t>(b) & 1;
        const uint32_t aSize = aLong ? load<uint32_t>(a + 4) : uint32_t(load<uint8_t>(a) >> 1);
        const uint32_t bSize = bLong ? load<uint32_t>(b + 4) : uint32_t(load<uint8_t>(b) >> 1);
        const uint32_t aData = aLong ? load<uint32_t>(a + 8) : a + 1, bData = bLong ? load<uint32_t>(b + 8) : b + 1;
        const int32_t r = int32_t(rw::call(MEMCMP, {aData, bData, aSize < bSize ? aSize : bSize}));
        return r != 0 ? r < 0 : aSize < bSize;
    }
};
// a function pointer comparator stored in the tree (+0xC)
struct FnKey {
    static bool less(uint32_t t, uint32_t a, uint32_t b) { return rw::call(load<uint32_t>(t + COMPARE), {a, b}) != 0; }
};

// ------------------------------------------------------------------------------------------ searches
template <class Key> struct Search {
    // __lower_bound(v, root, result): the first node not less than v, or result
    static uint32_t lowerBound(uint32_t t, uint32_t v, uint32_t root, uint32_t result) {
        while (root) {
            if (!Key::less(t, root + VALUE, v)) {
                result = root;
                root = left(root);
            } else {
                root = right(root);
            }
        }
        return result;
    }

    // find(v) with the lower bound inline: the node, or the end node
    static uint32_t find(uint32_t t, uint32_t v) {
        const uint32_t end = endNode(t);
        const uint32_t p = lowerBound(t, v, load<uint32_t>(t + ROOT), end);
        if (p != end && !Key::less(t, v, p + VALUE)) return p;
        return end;
    }

    // find(v) calling the instance's own __lower_bound
    template <uint32_t LowerBound> static uint32_t findVia(uint32_t t, uint32_t v) {
        const uint32_t end = endNode(t);
        const uint32_t p = rw::call(LowerBound, {t, v, load<uint32_t>(t + ROOT), end});
        if (p != end && !Key::less(t, v, p + VALUE)) return p;
        return end;
    }

    // __find_equal(parent&, v) (and map::__find_equal_key): the child slot where v is or belongs; parent set
    static uint32_t findEqual(uint32_t t, uint32_t parentRef, uint32_t v) {
        uint32_t nd = load<uint32_t>(t + ROOT);
        if (nd) {
            for (;;) {
                if (Key::less(t, v, nd + VALUE)) {
                    if (left(nd)) {
                        nd = left(nd);
                    } else {
                        save<uint32_t>(parentRef, nd);
                        return nd + LEFT;
                    }
                } else if (Key::less(t, nd + VALUE, v)) {
                    if (right(nd)) {
                        nd = right(nd);
                    } else {
                        save<uint32_t>(parentRef, nd);
                        return nd + RIGHT;
                    }
                } else {
                    save<uint32_t>(parentRef, nd);
                    return parentRef;
                }
            }
        }
        save<uint32_t>(parentRef, endNode(t));
        return endNode(t);
    }

    // the const map::__find_equal_key: the same search, returning what the slot holds (the node, or null)
    static uint32_t findEqualValue(uint32_t t, uint32_t parentRef, uint32_t v) {
        const uint32_t slot = findEqual(t, parentRef, v);
        return slot == parentRef ? load<uint32_t>(parentRef) : load<uint32_t>(slot);
    }

    // __find_equal(hint, parent&, v): next to the hint when v belongs there, else the plain search (FindEqual)
    template <uint32_t FindEqual> static uint32_t findEqualHint(uint32_t t, uint32_t hint, uint32_t parentRef, uint32_t v) {
        const uint32_t end = endNode(t);
        if (hint == end || Key::less(t, v, hint + VALUE)) {
            uint32_t prior = hint;
            bool fits = prior == load<uint32_t>(t + BEGIN);
            if (!fits) {
                prior = treePrev(prior);
                fits = Key::less(t, prior + VALUE, v);
            }
            if (fits) {
                if (!left(hint)) {
                    save<uint32_t>(parentRef, hint);
                    return hint + LEFT;
                }
                save<uint32_t>(parentRef, prior);
                return prior + RIGHT;
            }
            return rw::call(FindEqual, {t, parentRef, v});
        }
        if (Key::less(t, hint + VALUE, v)) {
            const uint32_t next = treeNext(hint);
            if (next == end || Key::less(t, v, next + VALUE)) {
                if (!right(hint)) {
                    save<uint32_t>(parentRef, hint);
                    return hint + RIGHT;
                }
                save<uint32_t>(parentRef, next);
                return next + LEFT;
            }
            return rw::call(FindEqual, {t, parentRef, v});
        }
        save<uint32_t>(parentRef, hint);
        return parentRef;
    }

    // __find_leaf_high(parent&, v): the slot after every key equal to v
    static uint32_t findLeafHigh(uint32_t t, uint32_t v, uint32_t &parentOut) {
        uint32_t nd = load<uint32_t>(t + ROOT);
        if (nd) {
            for (;;) {
                if (Key::less(t, v, nd + VALUE)) {
                    if (left(nd)) {
                        nd = left(nd);
                    } else {
                        parentOut = nd;
                        return nd + LEFT;
                    }
                } else if (right(nd)) {
                    nd = right(nd);
                } else {
                    parentOut = nd;
                    return nd + RIGHT;
                }
            }
        }
        parentOut = endNode(t);
        return endNode(t);
    }
};

// ------------------------------------------------------------------------------------------ insertion
// __insert_node_at(parent, child&, nd): nd linked into the slot, begin updated, the tree rebalanced
inline void insertNodeAt(uint32_t t, uint32_t parentNode, uint32_t child, uint32_t nd) {
    save<uint32_t>(nd + LEFT, 0);
    save<uint32_t>(nd + RIGHT, 0);
    save<uint32_t>(nd + PARENT, parentNode);
    save<uint32_t>(child, nd);
    const uint32_t begin = load<uint32_t>(t + BEGIN);
    if (left(begin)) save<uint32_t>(t + BEGIN, left(begin));
    rw::call(BALANCE_AFTER_INSERT, {load<uint32_t>(t + ROOT), load<uint32_t>(child)});
    save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) + 1);
}

// __node_insert_unique(nd) -> pair<iterator, bool> in `result`
template <uint32_t FindEqual, uint32_t InsertNodeAt> void nodeInsertUnique(uint32_t result, uint32_t t, uint32_t nd) {
    guest::Frame parentRef(4);
    const uint32_t child = rw::call(FindEqual, {t, parentRef.addr(), nd + VALUE});
    uint32_t r = load<uint32_t>(child);
    bool inserted = false;
    if (!r) {
        rw::call(InsertNodeAt, {t, load<uint32_t>(parentRef.addr()), child, nd});
        r = nd;
        inserted = true;
    }
    save<uint32_t>(result, r);
    save<uint8_t>(result + 4, inserted);
}

// __node_insert_unique(hint, nd) -> the node with nd's key (nd when it was linked)
template <uint32_t FindEqualHint, uint32_t InsertNodeAt> uint32_t nodeInsertUniqueHint(uint32_t t, uint32_t hint, uint32_t nd) {
    guest::Frame parentRef(4);
    const uint32_t child = rw::call(FindEqualHint, {t, hint, parentRef.addr(), nd + VALUE});
    uint32_t r = load<uint32_t>(child);
    if (!r) {
        rw::call(InsertNodeAt, {t, load<uint32_t>(parentRef.addr()), child, nd});
        r = nd;
    }
    return r;
}

// __insert_unique(v) that searches first and builds the node (Size bytes, construct(nd, v)) only for a new key
template <uint32_t Size, void (*Construct)(uint32_t, uint32_t), uint32_t FindEqual, uint32_t InsertNodeAt>
void insertUniqueSearchFirst(uint32_t result, uint32_t t, uint32_t v) {
    guest::Frame parentRef(4);
    const uint32_t child = rw::call(FindEqual, {t, parentRef.addr(), v});
    uint32_t r = load<uint32_t>(child);
    bool inserted = false;
    if (!r) {
        r = guest::newObject(Size);
        Construct(r, v);
        rw::call(InsertNodeAt, {t, load<uint32_t>(parentRef.addr()), child, r});
        inserted = true;
    }
    save<uint32_t>(result, r);
    save<uint8_t>(result + 4, inserted);
}

// ... with a hint -> the node
template <uint32_t Size, void (*Construct)(uint32_t, uint32_t), uint32_t FindEqualHint, uint32_t InsertNodeAt>
uint32_t insertUniqueHintSearchFirst(uint32_t t, uint32_t hint, uint32_t v) {
    guest::Frame parentRef(4);
    const uint32_t child = rw::call(FindEqualHint, {t, hint, parentRef.addr(), v});
    uint32_t r = load<uint32_t>(child);
    if (!r) {
        r = guest::newObject(Size);
        Construct(r, v);
        rw::call(InsertNodeAt, {t, load<uint32_t>(parentRef.addr()), child, r});
    }
    return r;
}

// __insert_unique(v) that builds the node first (construct(nd, v)), inserts it with __node_insert_unique and
// destroys it again when the key was already there
template <uint32_t Size, void (*Construct)(uint32_t, uint32_t), uint32_t NodeInsertUnique, void (*Destroy)(uint32_t)>
void insertUniqueNodeFirst(uint32_t result, uint32_t t, uint32_t v) {
    const uint32_t nd = guest::newObject(Size);
    Construct(nd, v);
    rw::call(NodeInsertUnique, {result, t, nd});
    if (nd && !load<uint8_t>(result + 4)) {
        Destroy(nd);
        guest::deleteObject(nd);
    }
}

// A __node_holder {node, &allocator, value constructed}: __construct_node fills it.
// __construct_node(v): the node (Size bytes) with its value built by construct(nd, v)
template <uint32_t Size, void (*Construct)(uint32_t, uint32_t)> void constructNode(uint32_t holder, uint32_t t, uint32_t v) {
    const uint32_t nd = guest::newObject(Size);
    save<uint32_t>(holder, nd);
    save<uint32_t>(holder + 4, t + 4);
    save<uint32_t>(holder + 8, 0);
    Construct(nd, v);
    save<uint8_t>(holder + 8, 1);
}

// map::__construct_node_with_key(k): the key copied, the mapped value value-initialised (init(nd)); both parts
// marked constructed (0x101)
template <uint32_t Size, void (*Init)(uint32_t)> void constructNodeWithKey(uint32_t holder, uint32_t m, uint32_t k) {
    const uint32_t nd = guest::newObject(Size);
    copyString(nd + VALUE, k);
    Init(nd);
    save<uint32_t>(holder, nd);
    save<uint32_t>(holder + 8, 0x101);
    save<uint32_t>(holder + 4, m + 4);
}

// __insert_unique(hint, v) through a holder: the node built by ConstructNode, inserted by NodeInsertUniqueHint,
// destroyed when its key was already there -> the node with the key
template <uint32_t ConstructNode, uint32_t NodeInsertUniqueHint, void (*Destroy)(uint32_t)>
uint32_t insertUniqueHintHeld(uint32_t t, uint32_t hint, uint32_t v) {
    guest::Frame holder(12);
    rw::call(ConstructNode, {holder.addr(), t, v});
    const uint32_t nd = load<uint32_t>(holder.addr());
    const uint32_t r = rw::call(NodeInsertUniqueHint, {t, hint, nd});
    save<uint32_t>(holder.addr(), 0);
    if (r != nd && nd) {
        if (load<uint8_t>(holder.addr(8))) Destroy(nd);
        guest::deleteObject(nd);
    }
    return r;
}

// map::operator[](k) with the node built inline: Size bytes, init(nd, k) (the key and a value-initialised value)
// -> the mapped value (at ValueOffset)
template <uint32_t FindEqualKey, uint32_t InsertNodeAt, uint32_t Size, uint32_t ValueOffset, void (*Init)(uint32_t, uint32_t)>
uint32_t subscript(uint32_t m, uint32_t k) {
    guest::Frame parentRef(4);
    const uint32_t child = rw::call(FindEqualKey, {m, parentRef.addr(), k});
    uint32_t r = load<uint32_t>(child);
    if (!r) {
        r = guest::newObject(Size);
        Init(r, k);
        rw::call(InsertNodeAt, {m, load<uint32_t>(parentRef.addr()), child, r});
    }
    return r + ValueOffset;
}

// map::operator[](k) through __construct_node_with_key and its holder
template <uint32_t FindEqualKey, uint32_t ConstructWithKey, uint32_t InsertNodeAt, uint32_t ValueOffset>
uint32_t subscriptWithKey(uint32_t m, uint32_t k) {
    guest::Frame frame(16);   // parent, then the holder
    const uint32_t parentRef = frame.addr(0), holder = frame.addr(4);
    const uint32_t child = rw::call(FindEqualKey, {m, parentRef, k});
    uint32_t r = load<uint32_t>(child);
    if (!r) {
        rw::call(ConstructWithKey, {holder, m, k});
        r = load<uint32_t>(holder);
        rw::call(InsertNodeAt, {m, load<uint32_t>(parentRef), child, r});
        save<uint32_t>(holder, 0);
    }
    return r + ValueOffset;
}

// __node_insert_multi(nd): after the keys equal to nd's -> nd
template <class Key, uint32_t InsertNodeAt> uint32_t nodeInsertMulti(uint32_t t, uint32_t nd) {
    uint32_t parentNode;
    const uint32_t child = Search<Key>::findLeafHigh(t, nd + VALUE, parentNode);
    rw::call(InsertNodeAt, {t, parentNode, child, nd});
    return nd;
}

// ------------------------------------------------------------------------------------------ removal, copies
// erase(it) -> the next node; the value destroyed, the node freed
template <void (*Destroy)(uint32_t)> uint32_t erase(uint32_t t, uint32_t it) {
    const uint32_t next = treeNext(it);
    if (load<uint32_t>(t + BEGIN) == it) save<uint32_t>(t + BEGIN, next);
    save<uint32_t>(t + SIZE, load<uint32_t>(t + SIZE) - 1);
    rw::call(TREE_REMOVE, {load<uint32_t>(t + ROOT), it});
    Destroy(it);
    guest::deleteObject(it);
    return next;
}

// __erase_unique(k) -> how many were erased (0 or 1)
template <uint32_t Find, uint32_t Erase> uint32_t eraseUnique(uint32_t t, uint32_t k) {
    const uint32_t it = rw::call(Find, {t, k});
    if (it == endNode(t)) return 0;
    rw::call(Erase, {t, it});
    return 1;
}

// destroy(nd): the subtree, children first (Self is this function)
template <uint32_t Self, void (*Destroy)(uint32_t)> void destroy(uint32_t t, uint32_t nd) {
    if (!nd) return;
    rw::call(Self, {t, left(nd)});
    rw::call(Self, {t, right(nd)});
    Destroy(nd);
    guest::deleteObject(nd);
}

// __move_assign(other, true_type): our nodes destroyed (DestroyTree), other's taken (with the comparator when the
// tree holds one), other left empty
template <uint32_t DestroyTree, bool HasCompare> void moveAssign(uint32_t t, uint32_t other) {
    rw::call(DestroyTree, {t, load<uint32_t>(t + ROOT)});
    save<uint32_t>(t + BEGIN, load<uint32_t>(other + BEGIN));
    const uint32_t root = load<uint32_t>(other + ROOT);
    save<uint32_t>(t + ROOT, root);
    const uint32_t size = load<uint32_t>(other + SIZE);
    if (HasCompare) save<uint32_t>(t + COMPARE, load<uint32_t>(other + COMPARE));
    save<uint32_t>(t + SIZE, size);
    if (!size) {
        save<uint32_t>(t + BEGIN, endNode(t));
        return;
    }
    save<uint32_t>(root + PARENT, endNode(t));
    save<uint32_t>(other + BEGIN, endNode(other));
    save<uint32_t>(other + ROOT, 0);
    save<uint32_t>(other + SIZE, 0);
}

// static __detach(cache): cache unlinked from its parent -> the next node to reuse (a leaf), or null
inline uint32_t detach(uint32_t cache) {
    const uint32_t p = parent(cache);
    if (!p) return 0;
    if (left(p) == cache) {
        save<uint32_t>(p + LEFT, 0);
        cache = parent(cache);
        return right(cache) ? treeLeaf(right(cache)) : cache;
    }
    save<uint32_t>(p + RIGHT, 0);
    cache = parent(cache);
    return left(cache) ? treeLeaf(left(cache)) : cache;
}

// __assign_multi(first, last): our nodes reused for the new values (assign(nd, from)) while they last, the rest
// destroyed, then the remaining values inserted
template <uint32_t Detach, uint32_t NodeInsertMulti, uint32_t DestroyTree, uint32_t InsertMulti, void (*Assign)(uint32_t, uint32_t)>
void assignMulti(uint32_t t, uint32_t first, uint32_t last) {
    if (load<uint32_t>(t + SIZE)) {
        uint32_t cache = load<uint32_t>(t + BEGIN);
        save<uint32_t>(t + BEGIN, endNode(t));
        save<uint32_t>(load<uint32_t>(t + ROOT) + PARENT, 0);
        save<uint32_t>(t + ROOT, 0);
        save<uint32_t>(t + SIZE, 0);
        if (right(cache)) cache = right(cache);
        while (cache) {
            if (first == last) {
                while (parent(cache)) cache = parent(cache);
                rw::call(DestroyTree, {t, cache});
                break;
            }
            Assign(cache, first);
            const uint32_t next = rw::call(Detach, {cache});
            rw::call(NodeInsertMulti, {t, cache});
            first = treeNext(first);
            cache = next;
        }
    }
    for (; first != last; first = treeNext(first)) rw::call(InsertMulti, {t, first + VALUE});
}

inline void noValue(uint32_t) {}

}  // namespace libcxx::tree
